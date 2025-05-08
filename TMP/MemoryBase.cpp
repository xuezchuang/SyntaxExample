#include "MemoryBase.h"


//void FMalloc::InitializeStatsMetadata()
//{
//	// Initialize stats metadata here instead of UpdateStats.
//	// Mostly to avoid dead-lock when stats malloc profiler is enabled.
//	GET_STATFNAME(STAT_MallocCalls);
//	GET_STATFNAME(STAT_ReallocCalls);
//	GET_STATFNAME(STAT_FreeCalls);
//	GET_STATFNAME(STAT_TotalAllocatorCalls);
//}
//
//void FMalloc::UpdateStats()
//{
//#if	STATS
//	GetCurrentFrameCalls().Update();
//
//	SET_DWORD_STAT( STAT_MallocCalls, GetCurrentFrameCalls().MallocCalls );
//	SET_DWORD_STAT( STAT_ReallocCalls, GetCurrentFrameCalls().ReallocCalls );
//	SET_DWORD_STAT( STAT_FreeCalls, GetCurrentFrameCalls().FreeCalls );
//	SET_DWORD_STAT( STAT_TotalAllocatorCalls, GetCurrentFrameCalls().AllocatorCalls );
//#endif
//}
//
//void* FMalloc::TryMalloc(SIZE_T Count, uint32 Alignment)
//{
//	return Malloc(Count, Alignment);
//}
//
//void* FMalloc::TryRealloc(void* Original, SIZE_T Count, uint32 Alignment)
//{
//	return Realloc(Original, Count, Alignment);
//}
