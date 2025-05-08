
#include "UnrealMemory.h"

struct FMemory;
struct FScopedMallocTimer;

void* FMemory::Malloc(SIZE_T Count, uint32 Alignment)
{
	void* Ptr=0;
	//if (!FMEMORY_INLINE_GMalloc)
	//{
	//	Ptr = MallocExternal(Count, Alignment);
	//}
	//else
	//{
	//	DoGamethreadHook(0);
	//	FScopedMallocTimer Timer(0);
	//	Ptr = FMEMORY_INLINE_GMalloc->Malloc(Count, Alignment);
	//}
	//// optional tracking of every allocation
	//LLM_IF_ENABLED(FLowLevelMemTracker::Get().OnLowLevelAlloc(ELLMTracker::Default, Ptr, Count, ELLMTag::Untagged, ELLMAllocType::FMalloc));
	return Ptr;
}

void* FMemory::Realloc(void* Original, SIZE_T Count, uint32 Alignment)
{
	void* Ptr = 0;
	if (!GMalloc)
	{
		Ptr = ReallocExternal(Original, Count, Alignment);
	}
	else
	{
		//DoGamethreadHook(1);
		//FScopedMallocTimer Timer(1);
		//Ptr = FMEMORY_INLINE_GMalloc->Realloc(Original, Count, Alignment);
	}

	// optional tracking of every allocation - a realloc with a Count of zero is equivalent to a call 
	// to free() and will return a null pointer which does not require tracking. If realloc returns null
	// for some other reason (like failure to allocate) there's also no reason to track it
	//LLM_IF_ENABLED(if (Ptr != nullptr) FLowLevelMemTracker::Get().OnLowLevelAlloc(ELLMTracker::Default, Ptr, Count, ELLMTag::Untagged, ELLMAllocType::FMalloc));

	return Ptr;
}

void FMemory::Free(void* Original)
{
	//if (!Original)
	//{
	//	FScopedMallocTimer Timer(3);
	//	return;
	//}

	//// optional tracking of every allocation
	//LLM_IF_ENABLED(FLowLevelMemTracker::Get().OnLowLevelFree(ELLMTracker::Default, Original, ELLMAllocType::FMalloc));

	//if (!FMEMORY_INLINE_GMalloc)
	//{
	//	FreeExternal(Original);
	//	return;
	//}
	//DoGamethreadHook(2);
	//FScopedMallocTimer Timer(2);
	//GMalloc->Free(Original);
}

SIZE_T FMemory::GetAllocSize(void* Original)
{
	//if (!FMEMORY_INLINE_GMalloc)
	//{
	//	return GetAllocSizeExternal(Original);
	//}

	//SIZE_T Size = 0;
	//return FMEMORY_INLINE_GMalloc->GetAllocationSize(Original, Size) ? Size : 0;
	return 0;
}

SIZE_T FMemory::QuantizeSize(SIZE_T Count, uint32 Alignment)
{
	if (!GMalloc)
	{
		return Count;
	}
	//return GMalloc->QuantizeSize(Count, Alignment);
	return 0;
}

