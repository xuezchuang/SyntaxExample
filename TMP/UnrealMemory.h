// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include <memory>
#include <windows.h>
#include "IsPointer.h"

#define FORCEINLINE __forceinline
using uint8 = uint8_t;
using uint16 = uint16_t;
using uint32 = uint32_t;
using uint64 = uint64_t;

enum class EMemcpyCachePolicy : uint8
{
	// Writes to destination memory are cache-visible (default).
	// This should be used if copy results are immediately accessed by CPU.
	StoreCached,

	// Writes to destination memory bypass cache (avoiding pollution).
	// Optimizes for large copies that aren't read from soon after.
	StoreUncached,
};


enum
{
	// Default allocator alignment. If the default is specified, the allocator applies to engine rules.
	// Blocks >= 16 bytes will be 16-byte-aligned, Blocks < 16 will be 8-byte aligned. If the allocator does
	// not support allocation alignment, the alignment will be ignored.
	DEFAULT_ALIGNMENT = 0,

	// Minimum allocator alignment
	MIN_ALIGNMENT = 8,
};

template <typename T>
static FORCEINLINE void Valswap(T& A, T& B)
{
	// Usually such an implementation would use move semantics, but
	// we're only ever going to call it on fundamental types and MoveTemp
	// is not necessarily in scope here anyway, so we don't want to
	// #include it if we don't need to.
	T Tmp = A;
	A = B;
	B = Tmp;
}

static inline void Memswap(void* Ptr1, void* Ptr2, SIZE_T Size)
{
	switch (Size)
	{
	case 0:
		break;

	case 1:
		Valswap(*(uint8*)Ptr1, *(uint8*)Ptr2);
		break;

	case 2:
		Valswap(*(uint16*)Ptr1, *(uint16*)Ptr2);
		break;

	case 3:
		Valswap(*((uint16*&)Ptr1)++, *((uint16*&)Ptr2)++);
		Valswap(*(uint8*)Ptr1, *(uint8*)Ptr2);
		break;

	case 4:
		Valswap(*(uint32*)Ptr1, *(uint32*)Ptr2);
		break;

	case 5:
		Valswap(*((uint32*&)Ptr1)++, *((uint32*&)Ptr2)++);
		Valswap(*(uint8*)Ptr1, *(uint8*)Ptr2);
		break;

	case 6:
		Valswap(*((uint32*&)Ptr1)++, *((uint32*&)Ptr2)++);
		Valswap(*(uint16*)Ptr1, *(uint16*)Ptr2);
		break;

	case 7:
		Valswap(*((uint32*&)Ptr1)++, *((uint32*&)Ptr2)++);
		Valswap(*((uint16*&)Ptr1)++, *((uint16*&)Ptr2)++);
		Valswap(*(uint8*)Ptr1, *(uint8*)Ptr2);
		break;

	case 8:
		Valswap(*(uint64*)Ptr1, *(uint64*)Ptr2);
		break;

	case 16:
		Valswap(((uint64*)Ptr1)[0], ((uint64*)Ptr2)[0]);
		Valswap(((uint64*)Ptr1)[1], ((uint64*)Ptr2)[1]);
		break;

	default:
		//MemswapGreaterThan8(Ptr1, Ptr2, Size);
		break;
	}
}

struct FMemory
{
	/** Some allocators can be given hints to treat allocations differently depending on how the memory is used, it's lifetime etc. */
	enum AllocationHints
	{
		None = -1,
		Default,
		Temporary,
		SmallPool,

		Max
	};


	/** @name Memory functions (wrapper for FPlatformMemory) */

	static FORCEINLINE void* Memmove(void* Dest, const void* Src, SIZE_T Count)
	{
		return memmove(Dest, Src, Count);
	}

	//static FORCEINLINE int32 Memcmp(const void* Buf1, const void* Buf2, SIZE_T Count)
	//{
	//	return memcmp(Buf1, Buf2, Count);
	//}

	static FORCEINLINE void* Memset(void* Dest, uint8 Char, SIZE_T Count)
	{
		return memset(Dest, Char, Count);
	}

	template< class T >
	static FORCEINLINE void Memset(T& Src, uint8 ValueToSet)
	{
		static_assert(!TIsPointer<T>::Value, "For pointers use the three parameters function");
		memset(&Src, ValueToSet, sizeof(T));
	}

	static FORCEINLINE void* Memzero(void* Dest, SIZE_T Count)
	{
		return memset(Dest, 0, Count);
	}

	/** Returns true if memory is zeroes, false otherwise. */
	static FORCEINLINE bool MemIsZero(const void* Ptr, SIZE_T Count)
	{
		// first pass implementation
		uint8* Start = (uint8*)Ptr;
		uint8* End = Start + Count;
		while (Start < End)
		{
			if ((*Start++) != 0)
			{
				return false;
			}
		}

		return true;
	}

	template< class T >
	static FORCEINLINE void Memzero(T& Src)
	{
		static_assert(!TIsPointer<T>::Value, "For pointers use the two parameters function");
		memset(&Src, 0, sizeof(T));
	}

	static FORCEINLINE void* Memcpy(void* Dest, const void* Src, SIZE_T Count)
	{
		return memcpy(Dest, Src, Count);
	}

	template< class T >
	static FORCEINLINE void Memcpy(T& Dest, const T& Src)
	{
		static_assert(!TIsPointer<T>::Value, "For pointers use the three parameters function");
		memcpy(&Dest, &Src, sizeof(T));
	}

	static FORCEINLINE void* BigBlockMemcpy(void* Dest, const void* Src, SIZE_T Count)
	{
		return memcpy(Dest, Src, Count);
	}

	static FORCEINLINE void* StreamingMemcpy(void* Dest, const void* Src, SIZE_T Count)
	{
		return memcpy(Dest, Src, Count);
	}

	static FORCEINLINE void* ParallelMemcpy(void* Dest, const void* Src, SIZE_T Count, EMemcpyCachePolicy Policy = EMemcpyCachePolicy::StoreCached)
	{
		(void)Policy;
		return memcpy(Dest, Src, Count);
	}

	static FORCEINLINE void Memswap(void* Ptr1, void* Ptr2, SIZE_T Size)
	{
		Memswap(Ptr1, Ptr2, Size);
	}

	//
	// C style memory allocation stubs that fall back to C runtime
	//
	static FORCEINLINE void* SystemMalloc(SIZE_T Size)
	{
		/* TODO: Trace! */
		return ::malloc(Size);
	}

	static FORCEINLINE void SystemFree(void* Ptr)
	{
		/* TODO: Trace! */
		::free(Ptr);
	}

	//
	// C style memory allocation stubs.
	//

	static void* Malloc(SIZE_T Count, uint32 Alignment = DEFAULT_ALIGNMENT);
	static void* Realloc(void* Original, SIZE_T Count, uint32 Alignment = DEFAULT_ALIGNMENT);
	static void Free(void* Original);
	static SIZE_T GetAllocSize(void* Original);

	static void* MallocZeroed(SIZE_T Count, uint32 Alignment = DEFAULT_ALIGNMENT)
	{
		void* Memory = Malloc(Count, Alignment);
		Memzero(Memory, Count);
		return Memory;
	}

	/**
	* For some allocators this will return the actual size that should be requested to eliminate
	* internal fragmentation. The return value will always be >= Count. This can be used to grow
	* and shrink containers to optimal sizes.
	* This call is always fast and threadsafe with no locking.
	*/
	static SIZE_T QuantizeSize(SIZE_T Count, uint32 Alignment = DEFAULT_ALIGNMENT);

	/**
	* Releases as much memory as possible. Must be called from the main thread.
	*/
	static void Trim(bool bTrimThreadCaches = true);

	/**
	* Set up TLS caches on the current thread. These are the threads that we can trim.
	*/
	static void SetupTLSCachesOnCurrentThread();

	/**
	* Clears the TLS caches on the current thread and disables any future caching.
	*/
	static void ClearAndDisableTLSCachesOnCurrentThread();

	/**
	 * A helper function that will perform a series of random heap allocations to test
	 * the internal validity of the heap. Note, this function will "leak" memory, but another call
	 * will clean up previously allocated blocks before returning. This will help to A/B testing
	 * where you call it in a good state, do something to corrupt memory, then call this again
	 * and hopefully freeing some pointers will trigger a crash.
	 */
	static void TestMemory();
	/**
	* Called once main is started and we have -purgatorymallocproxy.
	* This uses the purgatory malloc proxy to check if things are writing to stale pointers.
	*/
	static void EnablePurgatoryTests();
	/**
	* Called once main is started and we have -purgatorymallocproxy.
	* This uses the purgatory malloc proxy to check if things are writing to stale pointers.
	*/
	static void EnablePoisonTests();
	/**
	* Set global allocator instead of creating it lazily on first allocation.
	* Must only be called once and only if lazy init is disabled via a macro.
	*/
	//static void ExplicitInit(FMalloc& Allocator);

	/**
	* Functions to handle special memory given to the title from the platform
	* This memory is allocated like a stack, it's never really freed
	*/
	static void RegisterPersistentAuxiliary(void* InMemory, SIZE_T InSize);
	static void* MallocPersistentAuxiliary(SIZE_T InSize, uint32 InAlignment = 0);
	static void FreePersistentAuxiliary(void* InPtr);
	static bool IsPersistentAuxiliaryActive();
	static void DisablePersistentAuxiliary();
	static void EnablePersistentAuxiliary();
	static SIZE_T GetUsedPersistentAuxiliary();
private:
	static void GCreateMalloc();
	// These versions are called either at startup or in the event of a crash
	static void* MallocExternal(SIZE_T Count, uint32 Alignment = DEFAULT_ALIGNMENT);
	static void* ReallocExternal(void* Original, SIZE_T Count, uint32 Alignment = DEFAULT_ALIGNMENT);
	static void FreeExternal(void* Original);
	static SIZE_T GetAllocSizeExternal(void* Original);
	static SIZE_T QuantizeSizeExternal(SIZE_T Count, uint32 Alignment = DEFAULT_ALIGNMENT);
};
