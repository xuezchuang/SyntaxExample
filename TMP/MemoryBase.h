// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

extern class FMalloc* GMalloc;

/** The global memory allocator's interface. */
class FMalloc
{
public:
	virtual void* Realloc(void* Original, SIZE_T Count, uint32 Alignment = DEFAULT_ALIGNMENT) = 0;
};
