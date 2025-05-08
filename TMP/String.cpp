#include <iostream>
#include <memory>
#include <tchar.h>
#include "Array.h"
#include "ContainerAllocationPolicies.h"
#include "UnrealString.h"

FString& FString::operator=(const TCHAR* Other)
{
	//if (Data.GetData() != Other)
	//{
	//	int32 Len = (Other && *Other) ? FCString::Strlen(Other) + 1 : 0;
	//	Data.Empty(Len);
	//	Data.AddUninitialized(Len);

	//	if (Len)
	//	{
	//		FMemory::Memcpy(Data.GetData(), Other, Len * sizeof(TCHAR));
	//	}
	//}
	return *this;
}

template<typename T, typename Allocator = FDefaultAllocator> class TArray;

namespace UE::String::Private
{
	template<typename CharType>
	void ConstructFromCString(/* Out */ TArray<TCHAR>& Data, const CharType* Src)
	{
		//if (Src && *Src)
		//{
		//	int32 SrcLen = TCString<CharType>::Strlen(Src) + 1;
		//	int32 DestLen = FPlatformString::ConvertedLength<TCHAR>(Src, SrcLen);
		//	Data.Reserve(DestLen);
		//	Data.AddUninitialized(DestLen);

		//	FPlatformString::Convert(Data.GetData(), DestLen, Src, SrcLen);
		//}
	}

	template<typename CharType>
	void ConstructWithLength(/* Out */ TArray<TCHAR>& Data, int32 InCount, const CharType* InSrc)
	{
		/*if (InSrc)
		{
			int32 DestLen = FPlatformString::ConvertedLength<TCHAR>(InSrc, InCount);
			if (DestLen > 0 && *InSrc)
			{
				Data.Reserve(DestLen + 1);
				Data.AddUninitialized(DestLen + 1);

				FPlatformString::Convert(Data.GetData(), DestLen, InSrc, InCount);
				*(Data.GetData() + Data.Num() - 1) = TEXT('\0');
			}
		}*/
	}

	template<typename CharType>
	void ConstructWithSlack(/* Out */ TArray<TCHAR>& Data, const CharType* Src, int32 ExtraSlack)
	{
		//if (Src && *Src)
		//{
		//	int32 SrcLen = TCString<CharType>::Strlen(Src) + 1;
		//	int32 DestLen = FPlatformString::ConvertedLength<TCHAR>(Src, SrcLen);
		//	Data.Reserve(DestLen + ExtraSlack);
		//	Data.AddUninitialized(DestLen);

		//	FPlatformString::Convert(Data.GetData(), DestLen, Src, SrcLen);
		//}
		//else if (ExtraSlack > 0)
		//{
		//	Data.Reserve(ExtraSlack + 1);
		//}
	}

} // namespace UE::String::Private

FString::FString(const ANSICHAR* Str)								{ UE::String::Private::ConstructFromCString(Data, Str); }
FString::FString(const WIDECHAR* Str)								{ UE::String::Private::ConstructFromCString(Data, Str); }