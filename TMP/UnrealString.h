// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#define UE_NODISCARD [[nodiscard]]

class FString
{
public:
	using AllocatorType = TSizedDefaultAllocator<32>;

private:
	/** Array holding the character data */
	typedef TArray<TCHAR, AllocatorType> DataType;
	DataType Data;

public:
	using ElementType = TCHAR;
	typedef char				ANSICHAR;
	typedef wchar_t				WIDECHAR;

	FString() = default;
	FString(FString&&) = default;
	FString(const FString&) = default;
	FString& operator=(FString&&) = default;
	FString& operator=(const FString&) = default;

	FString(const ANSICHAR* Str);
	FString(const WIDECHAR* Str);

	FString& operator=(const TCHAR* Str);

};