#include <iostream>
#include <memory>
#include <tchar.h>
#include "Array.h"
#include "UnrealString.h"

template<typename T, typename Allocator = FDefaultAllocator> class TArray;

int main()
{
	//const TCHAR* abc = L"hello";
	//FString testString(L"hello");
	//FString testString(abc);
	//testString = abc;
	//testString.
	
	TArray<TCHAR> Data;
	Data.Reserve(10);
	return 0;

}