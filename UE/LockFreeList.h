#pragma once
#include "ThreadSafeCounter.h"

#define MAX_LOCK_FREE_LINKS_AS_BITS (26)
#define MAX_LOCK_FREE_LINKS (1 << 26)
#define MAX_TagBitsValue (uint64(1) << (64 - MAX_LOCK_FREE_LINKS_AS_BITS))
	

typedef unsigned __int64	SIZE_T;

void LockFreeTagCounterHasOverflowed();

namespace FPlatformAtomics
{
	static FORCEINLINE int64 AtomicRead(volatile const int64* Src)
	{
		return (int64)::_InterlockedCompareExchange64((int64*)Src, 0, 0);
	}

	static FORCEINLINE int64 InterlockedCompareExchange(volatile int64* Dest, int64 Exchange, int64 Comparand)
	{
		return (int64)::_InterlockedCompareExchange64(Dest, Exchange, Comparand);
	}
}

void LockFreeLinksExhausted(uint32 TotalNum);
void* LockFreeAllocLinks(SIZE_T AllocSize);

template<class T, unsigned int MaxTotalItems, unsigned int ItemsPerPage>
class TLockFreeAllocOnceIndexedAllocator
{
	enum
	{
		MaxBlocks = (MaxTotalItems + ItemsPerPage - 1) / ItemsPerPage
	};
public:

	TLockFreeAllocOnceIndexedAllocator()
	{
		NextIndex.Increment(); // skip the null ptr
		for (uint32 Index = 0; Index < MaxBlocks; Index++)
		{
			Pages[Index] = nullptr;
		}
	}

	FORCEINLINE uint32 Alloc(uint32 Count = 1)
	{
		uint32 FirstItem = NextIndex.Add(Count);
		if (FirstItem + Count > MaxTotalItems)
		{
			LockFreeLinksExhausted(MaxTotalItems);
		}
		for (uint32 CurrentItem = FirstItem; CurrentItem < FirstItem + Count; CurrentItem++)
		{
			new (GetRawItem(CurrentItem)) T();
		}
		return FirstItem;
	}
	FORCEINLINE T* GetItem(uint32 Index)
	{
		if (!Index)
		{
			return nullptr;
		}
		uint32 BlockIndex = Index / ItemsPerPage;
		uint32 SubIndex = Index % ItemsPerPage;
		return Pages[BlockIndex] + SubIndex;
	}

private:
	void* GetRawItem(uint32 Index)
	{
		uint32 BlockIndex = Index / ItemsPerPage;
		uint32 SubIndex = Index % ItemsPerPage;
		if (!Pages[BlockIndex])
		{
			T* NewBlock = (T*)LockFreeAllocLinks(ItemsPerPage * sizeof(T));
			if (::_InterlockedCompareExchangePointer((void**)&Pages[BlockIndex], NewBlock, nullptr) != nullptr)
			{
				// we lost discard block
				LockFreeFreeLinks(ItemsPerPage * sizeof(T), NewBlock);
			}
			else
			{
			}
		}
		return (void*)(Pages[BlockIndex] + SubIndex);
	}

	alignas(PLATFORM_CACHE_LINE_SIZE) FThreadSafeCounter NextIndex;
	alignas(PLATFORM_CACHE_LINE_SIZE) T* Pages[MaxBlocks];
};

__declspec(align(8))
struct FIndexedPointer
{
	// no constructor, intentionally. We need to keep the ABA double counter in tact

	// This should only be used for FIndexedPointer's with no outstanding concurrency.
	// Not recycled links, for example.
	void Init()
	{
		static_assert(((MAX_LOCK_FREE_LINKS - 1) & MAX_LOCK_FREE_LINKS) == 0, "MAX_LOCK_FREE_LINKS must be a power of two");
		Ptrs = 0;
	}
	FORCEINLINE void SetAll(uint32 Ptr, uint64 CounterAndState)
	{
		Ptrs = (uint64(Ptr) | (CounterAndState << MAX_LOCK_FREE_LINKS_AS_BITS));
	}

	FORCEINLINE uint32 GetPtr() const
	{
		return uint32(Ptrs & (MAX_LOCK_FREE_LINKS - 1));
	}

	FORCEINLINE void SetPtr(uint32 To)
	{
		SetAll(To, GetCounterAndState());
	}

	FORCEINLINE uint64 GetCounterAndState() const
	{
		return (Ptrs >> MAX_LOCK_FREE_LINKS_AS_BITS);
	}

	FORCEINLINE void SetCounterAndState(uint64 To)
	{
		SetAll(GetPtr(), To);
	}

	FORCEINLINE void AdvanceCounterAndState(const FIndexedPointer& From, uint64 TABAInc)
	{
		SetCounterAndState(From.GetCounterAndState() + TABAInc);
		if (GetCounterAndState() < From.GetCounterAndState())
		{
			// this is not expected to be a problem and it is not expected to happen very often. When it does happen, we will sleep as an extra precaution.
			LockFreeTagCounterHasOverflowed();
		}
	}

	template<uint64 TABAInc>
	FORCEINLINE uint64 GetState() const
	{
		return GetCounterAndState() & (TABAInc - 1);
	}

	template<uint64 TABAInc>
	FORCEINLINE void SetState(uint64 Value)
	{
		SetCounterAndState((GetCounterAndState() & ~(TABAInc - 1)) | Value);
	}

	FORCEINLINE void AtomicRead(const FIndexedPointer& Other)
	{
		Ptrs = uint64(FPlatformAtomics::AtomicRead((volatile const int64*)&Other.Ptrs));
	}

	FORCEINLINE bool InterlockedCompareExchange(const FIndexedPointer& Exchange, const FIndexedPointer& Comparand)
	{
		return uint64(FPlatformAtomics::InterlockedCompareExchange((volatile int64*)&Ptrs, Exchange.Ptrs, Comparand.Ptrs)) == Comparand.Ptrs;
	}

	FORCEINLINE bool operator==(const FIndexedPointer& Other) const
	{
		return Ptrs == Other.Ptrs;
	}
	FORCEINLINE bool operator!=(const FIndexedPointer& Other) const
	{
		return Ptrs != Other.Ptrs;
	}

private:
	uint64 Ptrs;

};

struct FIndexedLockFreeLink
{
	FIndexedPointer DoubleNext;
	void* Payload;
	uint32 SingleNext;
};

// there is a version of this code that uses 128 bit atomics to avoid the indirection, that is why we have this policy class at all.
struct FLockFreeLinkPolicy
{
	enum
	{
		MAX_BITS_IN_TLinkPtr = MAX_LOCK_FREE_LINKS_AS_BITS
	};
	typedef FIndexedPointer TDoublePtr;
	typedef FIndexedLockFreeLink TLink;
	typedef uint32 TLinkPtr;
	typedef TLockFreeAllocOnceIndexedAllocator<FIndexedLockFreeLink, MAX_LOCK_FREE_LINKS, 16384> TAllocator;

	static FORCEINLINE FIndexedLockFreeLink* DerefLink(uint32 Ptr)
	{
		return LinkAllocator.GetItem(Ptr);
	}
	static FORCEINLINE FIndexedLockFreeLink* IndexToLink(uint32 Index)
	{
		return LinkAllocator.GetItem(Index);
	}
	static FORCEINLINE uint32 IndexToPtr(uint32 Index)
	{
		return Index;
	}

	static uint32 AllocLockFreeLink();
	static void FreeLockFreeLink(uint32 Item);
	static TAllocator LinkAllocator;
};


#define UE_NONCOPYABLE(TypeName) \
	TypeName(TypeName&&) = delete; \
	TypeName(const TypeName&) = delete; \
	TypeName& operator=(const TypeName&) = delete; \
	TypeName& operator=(TypeName&&) = delete;

#define TSAN_SAFE


template<int TPaddingForCacheContention, uint64 TABAInc = 1>
class FLockFreePointerListLIFORoot
{
	UE_NONCOPYABLE(FLockFreePointerListLIFORoot)

	typedef FLockFreeLinkPolicy::TDoublePtr TDoublePtr;
	typedef FLockFreeLinkPolicy::TLink TLink;
	typedef FLockFreeLinkPolicy::TLinkPtr TLinkPtr;

public:
	FORCEINLINE FLockFreePointerListLIFORoot()
	{
		// We want to make sure we have quite a lot of extra counter values to avoid the ABA problem. This could probably be relaxed, but eventually it will be dangerous. 
		// The question is "how many queue operations can a thread starve for".
		static_assert(MAX_TagBitsValue / TABAInc >= (1 << 23), "risk of ABA problem");
		static_assert((TABAInc & (TABAInc - 1)) == 0, "must be power of two");
		Reset();
	}

	void Reset()
	{
		Head.Init();
	}

	void Push(TLinkPtr Item) TSAN_SAFE
	{
		while (true)
		{
			TDoublePtr LocalHead;
			LocalHead.AtomicRead(Head);
			TDoublePtr NewHead;
			NewHead.AdvanceCounterAndState(LocalHead, TABAInc);
			NewHead.SetPtr(Item);
			FLockFreeLinkPolicy::DerefLink(Item)->SingleNext = LocalHead.GetPtr();
			if (Head.InterlockedCompareExchange(NewHead, LocalHead))
			{
				break;
			}
		}
	}

	/*bool PushIf(TFunctionRef<TLinkPtr(uint64)> AllocateIfOkToPush) TSAN_SAFE
	{
		static_assert(TABAInc > 1, "method should not be used for lists without state");
		while (true)
		{
			TDoublePtr LocalHead;
			LocalHead.AtomicRead(Head);
			uint64 LocalState = LocalHead.GetState<TABAInc>();
			TLinkPtr Item = AllocateIfOkToPush(LocalState);
			if (!Item)
			{
				return false;
			}

			TDoublePtr NewHead;
			NewHead.AdvanceCounterAndState(LocalHead, TABAInc);
			FLockFreeLinkPolicy::DerefLink(Item)->SingleNext = LocalHead.GetPtr();
			NewHead.SetPtr(Item);
			if (Head.InterlockedCompareExchange(NewHead, LocalHead))
			{
				break;
			}
		}
		return true;
	}*/


	TLinkPtr Pop() TSAN_SAFE
	{
		TLinkPtr Item = 0;
		while (true)
		{
			TDoublePtr LocalHead;
			LocalHead.AtomicRead(Head);
			Item = LocalHead.GetPtr();
			if (!Item)
			{
				break;
			}
			TDoublePtr NewHead;
			NewHead.AdvanceCounterAndState(LocalHead, TABAInc);
			TLink* ItemP = FLockFreeLinkPolicy::DerefLink(Item);
			NewHead.SetPtr(ItemP->SingleNext);
			if (Head.InterlockedCompareExchange(NewHead, LocalHead))
			{
				ItemP->SingleNext = 0;
				break;
			}
		}
		return Item;
	}

	TLinkPtr PopAll() TSAN_SAFE
	{
		TLinkPtr Item = 0;
		while (true)
		{
			TDoublePtr LocalHead;
			LocalHead.AtomicRead(Head);
			Item = LocalHead.GetPtr();
			if (!Item)
			{
				break;
			}
			TDoublePtr NewHead;
			NewHead.AdvanceCounterAndState(LocalHead, TABAInc);
			NewHead.SetPtr(0);
			if (Head.InterlockedCompareExchange(NewHead, LocalHead))
			{
				break;
			}
		}
		return Item;
	}

	/*TLinkPtr PopAllAndChangeState(TFunctionRef<uint64(uint64)> StateChange) TSAN_SAFE
	{
		static_assert(TABAInc > 1, "method should not be used for lists without state");
		TLinkPtr Item = 0;
		while (true)
		{
			TDoublePtr LocalHead;
			LocalHead.AtomicRead(Head);
			Item = LocalHead.GetPtr();
			TDoublePtr NewHead;
			NewHead.AdvanceCounterAndState(LocalHead, TABAInc);
			NewHead.SetState<TABAInc>(StateChange(LocalHead.GetState<TABAInc>()));
			NewHead.SetPtr(0);
			if (Head.InterlockedCompareExchange(NewHead, LocalHead))
			{
				break;
			}
		}
		return Item;
	}*/

	FORCEINLINE bool IsEmpty() const
	{
		return !Head.GetPtr();
	}

	FORCEINLINE uint64 GetState() const
	{
		TDoublePtr LocalHead;
		LocalHead.AtomicRead(Head);
		return LocalHead.GetState<TABAInc>();
	}

private:
	alignas(TPaddingForCacheContention) TDoublePtr Head;
};

template<class T, int TPaddingForCacheContention, uint64 TABAInc = 1>
class FLockFreePointerListLIFOBase
{
	UE_NONCOPYABLE(FLockFreePointerListLIFOBase)

	typedef FLockFreeLinkPolicy::TDoublePtr TDoublePtr;
	typedef FLockFreeLinkPolicy::TLink TLink;
	typedef FLockFreeLinkPolicy::TLinkPtr TLinkPtr;

public:
	FLockFreePointerListLIFOBase() = default;

	~FLockFreePointerListLIFOBase()
	{
		while (Pop()) {};
	}

	void Reset()
	{
		while (Pop()) {};
		RootList.Reset();
	}

	void Push(T* InPayload) TSAN_SAFE
	{
		TLinkPtr Item = FLockFreeLinkPolicy::AllocLockFreeLink();
		FLockFreeLinkPolicy::DerefLink(Item)->Payload = InPayload;
		RootList.Push(Item);
	}

	//bool PushIf(T* InPayload, TFunctionRef<bool(uint64)> OkToPush) TSAN_SAFE
	//{
	//	TLinkPtr Item = 0;

	//	auto AllocateIfOkToPush = [&OkToPush, InPayload, &Item](uint64 State)->TLinkPtr
	//		{
	//			if (OkToPush(State))
	//			{
	//				if (!Item)
	//				{
	//					Item = FLockFreeLinkPolicy::AllocLockFreeLink();
	//					FLockFreeLinkPolicy::DerefLink(Item)->Payload = InPayload;
	//				}
	//				return Item;
	//			}
	//			return 0;
	//		};
	//	if (!RootList.PushIf(AllocateIfOkToPush))
	//	{
	//		if (Item)
	//		{
	//			// we allocated the link, but it turned out that the list was closed
	//			FLockFreeLinkPolicy::FreeLockFreeLink(Item);
	//		}
	//		return false;
	//	}
	//	return true;
	//}


	T* Pop() TSAN_SAFE
	{
		TLinkPtr Item = RootList.Pop();
		T* Result = nullptr;
		if (Item)
		{
			Result = (T*)FLockFreeLinkPolicy::DerefLink(Item)->Payload;
			FLockFreeLinkPolicy::FreeLockFreeLink(Item);
		}
		return Result;
	}

	//void PopAll(TArray<T*>& OutArray) TSAN_SAFE
	//{
	//	TLinkPtr Links = RootList.PopAll();
	//	while (Links)
	//	{
	//		TLink* LinksP = FLockFreeLinkPolicy::DerefLink(Links);
	//		OutArray.Add((T*)LinksP->Payload);
	//		TLinkPtr Del = Links;
	//		Links = LinksP->SingleNext;
	//		FLockFreeLinkPolicy::FreeLockFreeLink(Del);
	//	}
	//}

	//void PopAllAndChangeState(TArray<T*>& OutArray, TFunctionRef<uint64(uint64)> StateChange) TSAN_SAFE
	//{
	//	TLinkPtr Links = RootList.PopAllAndChangeState(StateChange);
	//	while (Links)
	//	{
	//		TLink* LinksP = FLockFreeLinkPolicy::DerefLink(Links);
	//		OutArray.Add((T*)LinksP->Payload);
	//		TLinkPtr Del = Links;
	//		Links = LinksP->SingleNext;
	//		FLockFreeLinkPolicy::FreeLockFreeLink(Del);
	//	}
	//}

	FORCEINLINE bool IsEmpty() const
	{
		return RootList.IsEmpty();
	}

	FORCEINLINE uint64 GetState() const
	{
		return RootList.GetState();
	}

private:

	FLockFreePointerListLIFORoot<TPaddingForCacheContention, TABAInc> RootList;
};

template<class T, int TPaddingForCacheContention, uint64 TABAInc = 1>
class FLockFreePointerFIFOBase
{
	UE_NONCOPYABLE(FLockFreePointerFIFOBase)

	typedef FLockFreeLinkPolicy::TDoublePtr TDoublePtr;
	typedef FLockFreeLinkPolicy::TLink TLink;
	typedef FLockFreeLinkPolicy::TLinkPtr TLinkPtr;
public:

	FORCEINLINE FLockFreePointerFIFOBase()
	{
		// We want to make sure we have quite a lot of extra counter values to avoid the ABA problem. This could probably be relaxed, but eventually it will be dangerous. 
		// The question is "how many queue operations can a thread starve for".
		static_assert(TABAInc <= 65536, "risk of ABA problem");
		static_assert((TABAInc & (TABAInc - 1)) == 0, "must be power of two");

		Head.Init();
		Tail.Init();
		TLinkPtr Stub = FLockFreeLinkPolicy::AllocLockFreeLink();
		Head.SetPtr(Stub);
		Tail.SetPtr(Stub);
	}

	~FLockFreePointerFIFOBase()
	{
		while (Pop()) {};
		FLockFreeLinkPolicy::FreeLockFreeLink(Head.GetPtr());
	}

	void Push(T* InPayload) TSAN_SAFE
	{
		TLinkPtr Item = FLockFreeLinkPolicy::AllocLockFreeLink();
		FLockFreeLinkPolicy::DerefLink(Item)->Payload = InPayload;
		TDoublePtr LocalTail;
		while (true)
		{
			LocalTail.AtomicRead(Tail);
			TLink* LocalTailP = FLockFreeLinkPolicy::DerefLink(LocalTail.GetPtr());
			TDoublePtr LocalNext;
			LocalNext.AtomicRead(LocalTailP->DoubleNext);
			TDoublePtr TestLocalTail;
			TestLocalTail.AtomicRead(Tail);
			if (TestLocalTail == LocalTail)
			{
				if (LocalNext.GetPtr())
				{
					//TestCriticalStall();
					TDoublePtr NewTail;
					NewTail.AdvanceCounterAndState(LocalTail, TABAInc);
					NewTail.SetPtr(LocalNext.GetPtr());
					Tail.InterlockedCompareExchange(NewTail, LocalTail);
				}
				else
				{
					//TestCriticalStall();
					TDoublePtr NewNext;
					NewNext.AdvanceCounterAndState(LocalNext, TABAInc);
					NewNext.SetPtr(Item);
					if (LocalTailP->DoubleNext.InterlockedCompareExchange(NewNext, LocalNext))
					{
						break;
					}
				}
			}
		}
		{
			//TestCriticalStall();
			TDoublePtr NewTail;
			NewTail.AdvanceCounterAndState(LocalTail, TABAInc);
			NewTail.SetPtr(Item);
			Tail.InterlockedCompareExchange(NewTail, LocalTail);
		}
	}

	T* Pop() TSAN_SAFE
	{
		T* Result = nullptr;
		TDoublePtr LocalHead;
		while (true)
		{
			LocalHead.AtomicRead(Head);
			TDoublePtr LocalTail;
			LocalTail.AtomicRead(Tail);
			TDoublePtr LocalNext;
			LocalNext.AtomicRead(FLockFreeLinkPolicy::DerefLink(LocalHead.GetPtr())->DoubleNext);
			TDoublePtr LocalHeadTest;
			LocalHeadTest.AtomicRead(Head);
			if (LocalHead == LocalHeadTest)
			{
				if (LocalHead.GetPtr() == LocalTail.GetPtr())
				{
					if (!LocalNext.GetPtr())
					{
						return nullptr;
					}
					//TestCriticalStall();
					TDoublePtr NewTail;
					NewTail.AdvanceCounterAndState(LocalTail, TABAInc);
					NewTail.SetPtr(LocalNext.GetPtr());
					Tail.InterlockedCompareExchange(NewTail, LocalTail);
				}
				else
				{
					//TestCriticalStall();
					Result = (T*)FLockFreeLinkPolicy::DerefLink(LocalNext.GetPtr())->Payload;
					TDoublePtr NewHead;
					NewHead.AdvanceCounterAndState(LocalHead, TABAInc);
					NewHead.SetPtr(LocalNext.GetPtr());
					if (Head.InterlockedCompareExchange(NewHead, LocalHead))
					{
						break;
					}
				}
			}
		}
		FLockFreeLinkPolicy::FreeLockFreeLink(LocalHead.GetPtr());
		return Result;
	}

	//void PopAll(TArray<T*>& OutArray)
	//{
	//	while (T* Item = Pop())
	//	{
	//		OutArray.Add(Item);
	//	}
	//}


	FORCEINLINE bool IsEmpty() const
	{
		TDoublePtr LocalHead;
		LocalHead.AtomicRead(Head);
		TDoublePtr LocalNext;
		LocalNext.AtomicRead(FLockFreeLinkPolicy::DerefLink(LocalHead.GetPtr())->DoubleNext);
		return !LocalNext.GetPtr();
	}

private:
	alignas(TPaddingForCacheContention) TDoublePtr Head;
	alignas(TPaddingForCacheContention) TDoublePtr Tail;
};

template<typename T32BITS, typename T64BITS, int PointerSize>
struct SelectIntPointerType
{
	// nothing here are is it an error if the partial specializations fail
};

template<typename T32BITS, typename T64BITS>
struct SelectIntPointerType<T32BITS, T64BITS, 8>
{
	// Select the 64 bit type.
	typedef T64BITS TIntPointer;
};

template<typename T32BITS, typename T64BITS>
struct SelectIntPointerType<T32BITS, T64BITS, 4>
{
	// Select the 32 bit type.
	typedef T32BITS TIntPointer;
};

typedef SelectIntPointerType<uint32, uint64, sizeof(void*)>::TIntPointer UPTRINT;

template<class T, int TPaddingForCacheContention, int NumPriorities>
class FStallingTaskQueue
{
	UE_NONCOPYABLE(FStallingTaskQueue)

	typedef FLockFreeLinkPolicy::TDoublePtr TDoublePtr;
	typedef FLockFreeLinkPolicy::TLink TLink;
	typedef FLockFreeLinkPolicy::TLinkPtr TLinkPtr;
public:
	FStallingTaskQueue()
	{
		MasterState.Init();
	}
	int32 Push(T* InPayload, uint32 Priority)
	{
		//checkLockFreePointerList(Priority < NumPriorities);
		TDoublePtr LocalMasterState;
		LocalMasterState.AtomicRead(MasterState);
		PriorityQueues[Priority].Push(InPayload);
		TDoublePtr NewMasterState;
		NewMasterState.AdvanceCounterAndState(LocalMasterState, 1);
		int32 ThreadToWake = FindThreadToWake(LocalMasterState.GetPtr());
		if (ThreadToWake >= 0)
		{
			NewMasterState.SetPtr(TurnOffBit(LocalMasterState.GetPtr(), ThreadToWake));
		}
		else
		{
			NewMasterState.SetPtr(LocalMasterState.GetPtr());
		}
		while (!MasterState.InterlockedCompareExchange(NewMasterState, LocalMasterState))
		{
			LocalMasterState.AtomicRead(MasterState);
			NewMasterState.AdvanceCounterAndState(LocalMasterState, 1);
			ThreadToWake = FindThreadToWake(LocalMasterState.GetPtr());
			if (ThreadToWake >= 0)
			{
				NewMasterState.SetPtr(TurnOffBit(LocalMasterState.GetPtr(), ThreadToWake));
			}
			else
			{
				NewMasterState.SetPtr(LocalMasterState.GetPtr());
			}
		}
		return ThreadToWake;
	}

	T* Pop(int32 MyThread, bool bAllowStall)
	{
		//check(MyThread >= 0 && MyThread < FLockFreeLinkPolicy::MAX_BITS_IN_TLinkPtr);

		while (true)
		{
			TDoublePtr LocalMasterState;
			LocalMasterState.AtomicRead(MasterState);
			//checkLockFreePointerList(!TestBit(LocalMasterState.GetPtr(), MyThread) || !FPlatformProcess::SupportsMultithreading()); // you should not be stalled if you are asking for a task
			for (int32 Index = 0; Index < NumPriorities; Index++)
			{
				T* Result = PriorityQueues[Index].Pop();
				if (Result)
				{
					while (true)
					{
						TDoublePtr NewMasterState;
						NewMasterState.AdvanceCounterAndState(LocalMasterState, 1);
						NewMasterState.SetPtr(LocalMasterState.GetPtr());
						if (MasterState.InterlockedCompareExchange(NewMasterState, LocalMasterState))
						{
							return Result;
						}
						LocalMasterState.AtomicRead(MasterState);
						//checkLockFreePointerList(!TestBit(LocalMasterState.GetPtr(), MyThread) || !FPlatformProcess::SupportsMultithreading()); // you should not be stalled if you are asking for a task
					}
				}
			}
			if (!bAllowStall)
			{
				break; // if we aren't stalling, we are done, the queues are empty
			}
			{
				TDoublePtr NewMasterState;
				NewMasterState.AdvanceCounterAndState(LocalMasterState, 1);
				NewMasterState.SetPtr(TurnOnBit(LocalMasterState.GetPtr(), MyThread));
				if (MasterState.InterlockedCompareExchange(NewMasterState, LocalMasterState))
				{
					break;
				}
			}
		}
		return nullptr;
	}

private:

	static int32 FindThreadToWake(TLinkPtr Ptr)
	{
		int32 Result = -1;
		UPTRINT Test = UPTRINT(Ptr);
		if (Test)
		{
			Result = 0;
			while (!(Test & 1))
			{
				Test >>= 1;
				Result++;
			}
		}
		return Result;
	}

	static TLinkPtr TurnOffBit(TLinkPtr Ptr, int32 BitToTurnOff)
	{
		return (TLinkPtr)(UPTRINT(Ptr) & ~(UPTRINT(1) << BitToTurnOff));
	}

	static TLinkPtr TurnOnBit(TLinkPtr Ptr, int32 BitToTurnOn)
	{
		return (TLinkPtr)(UPTRINT(Ptr) | (UPTRINT(1) << BitToTurnOn));
	}

	static bool TestBit(TLinkPtr Ptr, int32 BitToTest)
	{
		return !!(UPTRINT(Ptr) & (UPTRINT(1) << BitToTest));
	}

	FLockFreePointerFIFOBase<T, TPaddingForCacheContention> PriorityQueues[NumPriorities];
	// not a pointer to anything, rather tracks the stall state of all threads servicing this queue.
	alignas(TPaddingForCacheContention) TDoublePtr MasterState;
};