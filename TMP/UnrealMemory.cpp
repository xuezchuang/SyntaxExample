// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealMemory.h"
#include "MemoryBase.h"

FMalloc* GMalloc = nullptr;

#include "FMemory.inl"

void FMemory::EnablePurgatoryTests()
{
	//if (PLATFORM_USES_FIXED_GMalloc_CLASS)
	//{
	//	UE_LOG(LogMemory, Error, TEXT("Purgatory proxy cannot be turned on because we are using PLATFORM_USES_FIXED_GMalloc_CLASS"));
	//	return;
	//}
	//static bool bOnce = false;
	//if (bOnce)
	//{
	//	UE_LOG(LogMemory, Error, TEXT("Purgatory proxy was already turned on."));
	//	return;
	//}
	//bOnce = true;
	//while (true)
	//{
	//	FMalloc* LocalGMalloc = GMalloc;
	//	FMalloc* Proxy = new FMallocPurgatoryProxy(LocalGMalloc);
	//	if (FPlatformAtomics::InterlockedCompareExchangePointer((void**)&GMalloc, Proxy, LocalGMalloc) == LocalGMalloc)
	//	{
	//		UE_LOG(LogConsoleResponse, Display, TEXT("Purgatory proxy is now on."));
	//		return;
	//	}
	//	delete Proxy;
	//}
}

void FMemory::EnablePoisonTests()
{
	//if ( ! FPlatformProcess::SupportsMultithreading() )
	//{
	//	UE_LOG(LogConsoleResponse, Display, TEXT("SKIPPING Poison proxy - platform does not support multithreads"));
	//	return;
	//}
	//if (PLATFORM_USES_FIXED_GMalloc_CLASS)
	//{
	//	UE_LOG(LogMemory, Error, TEXT("Poison proxy cannot be turned on because we are using PLATFORM_USES_FIXED_GMalloc_CLASS"));
	//	return;
	//}
	//static bool bOnce = false;
	//if (bOnce)
	//{
	//	UE_LOG(LogMemory, Error, TEXT("Poison proxy was already turned on."));
	//	return;
	//}
	//bOnce = true;
	//while (true)
	//{
	//	FMalloc* LocalGMalloc = GMalloc;
	//	FMalloc* Proxy = new FMallocPoisonProxy(LocalGMalloc);
	//	if (FPlatformAtomics::InterlockedCompareExchangePointer((void**)&GMalloc, Proxy, LocalGMalloc) == LocalGMalloc)
	//	{
	//		UE_LOG(LogConsoleResponse, Display, TEXT("Poison proxy is now on."));
	//		return;
	//	}
	//	delete Proxy;
	//}
}

//void FMemory::ExplicitInit(FMalloc& Allocator)
//{
//#if defined(REQUIRE_EXPLICIT_GMALLOC_INIT) && REQUIRE_EXPLICIT_GMALLOC_INIT
//	check(!GMalloc);
//	GMalloc = &Allocator;
//#else
//	checkf(false, TEXT("ExplicitInit() forbidden when global allocator is created lazily"));
//#endif
//}

void FMemory::GCreateMalloc()
{
//#if defined(REQUIRE_EXPLICIT_GMALLOC_INIT) && REQUIRE_EXPLICIT_GMALLOC_INIT
//	checkf(false, TEXT("Allocating before ExplicitInit()"));
//#else
//	// On some platforms (e.g. Mac) GMalloc can be created on multiple threads at once.
//	// This admittedly clumsy construct ensures both thread-safe creation and prevents multiple calls into it.
//	// The call will not be optimized away in Shipping because the function has side effects (touches global vars).
//	static int ThreadSafeCreationResult = FMemory_GCreateMalloc_ThreadUnsafe();
//#endif
	GMalloc = new FMalloc();
}

void* FMemory::MallocExternal(SIZE_T Count, uint32 Alignment)
{
	//if (!GMalloc)
	//{
	//	GCreateMalloc();
	//	CA_ASSUME(GMalloc != NULL);	// Don't want to assert, but suppress static analysis warnings about potentially NULL GMalloc
	//}
	//return GMalloc->Malloc(Count, Alignment);
	return 0;
}

void* FMemory::ReallocExternal(void* Original, SIZE_T Count, uint32 Alignment)
{
	if (!GMalloc)
	{
		GCreateMalloc();
		//CA_ASSUME(GMalloc != NULL);	// Don't want to assert, but suppress static analysis warnings about potentially NULL GMalloc
	}
	return GMalloc->Realloc(Original, Count, Alignment);
}

void FMemory::FreeExternal(void* Original)
{
	//if (!GMalloc)
	//{
	//	GCreateMalloc();
	//	CA_ASSUME(GMalloc != NULL);	// Don't want to assert, but suppress static analysis warnings about potentially NULL GMalloc
	//}
	//if (Original)
	//{
	//	GMalloc->Free(Original);
	//}
}

SIZE_T FMemory::GetAllocSizeExternal(void* Original)
{ 
	//if (!GMalloc)
	//{
	//	GCreateMalloc();
	//	CA_ASSUME(GMalloc != NULL);	// Don't want to assert, but suppress static analysis warnings about potentially NULL GMalloc
	//}
	//SIZE_T Size = 0;
	//return GMalloc->GetAllocationSize(Original, Size) ? Size : 0;
	return 0;
}

SIZE_T FMemory::QuantizeSizeExternal(SIZE_T Count, uint32 Alignment)
{ 
	//if (!GMalloc)
	//{
	//	GCreateMalloc();	
	//	CA_ASSUME(GMalloc != NULL);	// Don't want to assert, but suppress static analysis warnings about potentially NULL GMalloc
	//}
	//return GMalloc->QuantizeSize(Count, Alignment);
	return 0;
}	


void FMemory::Trim(bool bTrimThreadCaches)
{
}

void FMemory::SetupTLSCachesOnCurrentThread()
{
	if (!GMalloc)
	{
		GCreateMalloc();	
	}
	//GMalloc->SetupTLSCachesOnCurrentThread();
}
	
void FMemory::ClearAndDisableTLSCachesOnCurrentThread()
{
	if (GMalloc)
	{
		//GMalloc->ClearAndDisableTLSCachesOnCurrentThread();
	}
}

void FMemory::TestMemory()
{
}

static bool GPersistentAuxiliaryEnabled = true;
static uint8 * GPersistentAuxiliary = nullptr;
static uint8 * GPersistentAuxiliaryEnd = nullptr;
//static TAtomic<SIZE_T> GPersistentAuxiliaryCurrentOffset;
static SIZE_T GPersistentAuxiliarySize = 0;


void FMemory::RegisterPersistentAuxiliary(void* InMemory, SIZE_T InSize)
{
	//check(GPersistentAuxiliary == nullptr);
	//GPersistentAuxiliaryCurrentOffset = 0;
	GPersistentAuxiliarySize = InSize;
	GPersistentAuxiliary = (uint8 *)InMemory;
	GPersistentAuxiliaryEnd = GPersistentAuxiliary + InSize;
}
void* FMemory::MallocPersistentAuxiliary(SIZE_T InSize, uint32 InAlignment)
{
	//if (GPersistentAuxiliary != nullptr && GPersistentAuxiliaryEnabled)
	//{
	//	const uint32 Alignment = FMath::Max<uint32>(InAlignment, 16u);
	//	const SIZE_T AlignedSize = Align(InSize, Alignment);
	//	// 1st check if there is room, this is atomic but could still fail when actually incrementing the offset.
	//	if (GPersistentAuxiliaryCurrentOffset + AlignedSize <= GPersistentAuxiliarySize)
	//	{
	//		SIZE_T OldOffset = GPersistentAuxiliaryCurrentOffset.AddExchange(AlignedSize);
	//		if (OldOffset + AlignedSize <= GPersistentAuxiliarySize)
	//		{
	//			// we were able to increment the offset and it's still within the bounds of the aux memory.
	//			return &GPersistentAuxiliary[OldOffset];
	//		}
	//		// we've gone over the end of the aux memory, this could waste some space, if it's a problem protect with a critical section.
	//	}
	//}
	return FMemory::Malloc(InSize, InAlignment);
}
void FMemory::FreePersistentAuxiliary(void* InPtr)
{
	if (GPersistentAuxiliary != nullptr)
	{
		uint8* Ptr = (uint8*)InPtr;
		if (Ptr >= GPersistentAuxiliary && Ptr < GPersistentAuxiliaryEnd)
		{
			// it is part of the GPersistentAuxiliary
			return;
		}
	}
	return FMemory::Free(InPtr);
}
bool FMemory::IsPersistentAuxiliaryActive()
{
	return GPersistentAuxiliary != nullptr && GPersistentAuxiliaryEnabled;
}
void FMemory::DisablePersistentAuxiliary()
{
	GPersistentAuxiliaryEnabled = false;
}
void FMemory::EnablePersistentAuxiliary()
{
	GPersistentAuxiliaryEnabled = true;
}
SIZE_T FMemory::GetUsedPersistentAuxiliary()
{
	return 0;// GPersistentAuxiliaryCurrentOffset;
}

