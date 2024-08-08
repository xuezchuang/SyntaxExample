// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

/**
 * Base class for unicast delegates.
 */
#include "IDelegateInstance.h"
class FDelegateBase //-V1062
{
	template <typename>
	friend class TDelegateBase;

protected:
	/**
	 * Creates and initializes a new instance.
	 *
	 * @param InDelegateInstance The delegate instance to assign.
	 */
	explicit FDelegateBase()
		: DelegateSize(0)
	{
	}

	~FDelegateBase()
	{
		Unbind();
		delete ptr;
	}

	/**
	 * Move constructor.
	 */
	FDelegateBase(FDelegateBase&& Other)
	{
		//DelegateAllocator.MoveToEmpty(Other.DelegateAllocator);
		//DelegateSize = Other.DelegateSize;
		//Other.DelegateSize = 0;
	}

	/**
	 * Move assignment.
	 */
	FDelegateBase& operator=(FDelegateBase&& Other)
	{
		//UE_DELEGATES_MT_SCOPED_WRITE_ACCESS(AccessDetector);

		//Unbind();
		//DelegateAllocator.MoveToEmpty(Other.DelegateAllocator);
		//DelegateSize = Other.DelegateSize;
		//Other.DelegateSize = 0;
		return *this;
	}

	/**
	 * Unbinds this delegate
	 */
	void Unbind()
	{
		//UE_DELEGATES_MT_SCOPED_WRITE_ACCESS(AccessDetector);

		//if (IDelegateInstance* Ptr = GetDelegateInstanceProtected())
		//{
		//	Ptr->~IDelegateInstance();
		//	DelegateAllocator.ResizeAllocation(0, 0, sizeof(FAlignedInlineDelegateType));
		//	DelegateSize = 0;
		//}
	}

	/**
	 * Gets the delegate instance.  Not intended for use by user code.
	 *
	 * @return The delegate instance.
	 * @see SetDelegateInstance
	 */
	IDelegateInstance* GetDelegateInstanceProtected() const
	{
		return (IDelegateInstance*)ptr;
	}


private:
	friend void* operator new(size_t Size, FDelegateBase& Base);
	friend void operator delete(void* ptr, FDelegateBase& Base);
	//friend void operator delete(void* ptr);
	void* Allocate(int Size)
	{
		ptr = new byte[Size];
		return ptr;
	}

private:
	//FDelegateAllocatorType::ForElementType<FAlignedInlineDelegateType> DelegateAllocator;
	int DelegateSize;
	int DelegateSize2;
	int DelegateSize3;
	void* ptr;
};


struct FDefaultDelegateUserPolicy
{
	// To extend delegates, you should implement a policy struct like this and pass it as the second template
	// argument to TDelegate and TMulticastDelegate.  This policy struct containing three classes called:
	// 
	// FDelegateInstanceExtras:
	//   - Must publicly inherit IDelegateInstance.
	//   - Should contain any extra data and functions injected into a binding (the object which holds and
	//     is able to invoke the binding passed to FMyDelegate::CreateSP, FMyDelegate::CreateLambda etc.).
	//   - This binding is not available through the public API of the delegate, but is accessible to FDelegateExtras.
	//
	// FDelegateExtras:
	//   - Must publicly inherit FDelegateBase.
	//   - Should contain any extra data and functions injected into a delegate (the object which holds an
	//     FDelegateInstance-derived object, above).
	//   - Public data members and member functions are accessible directly through the TDelegate object.
	//   - Typically member functions in this class will forward calls to the inner FDelegateInstanceExtras,
	//     by downcasting the result of a call to GetDelegateInstanceProtected().
	//
	// FMulticastDelegateExtras:
	//   - Must publicly inherit TMulticastDelegateBase<FYourUserPolicyStruct>.
	//   - Should contain any extra data and functions injected into a multicast delegate (the object which
	//     holds an array of FDelegateExtras-derived objects which is the invocation list).
	//   - Public data members and member functions are accessible directly through the TMulticastDelegate object.
	using FDelegateInstanceExtras  = IDelegateInstance;
	using FDelegateExtras          = FDelegateBase;
	//using FMulticastDelegateExtras = TMulticastDelegateBase<FDefaultDelegateUserPolicy>;
};

inline void* operator new(size_t Size, FDelegateBase& Base)
{
	return Base.Allocate((int)Size);
}
// 在类外定义匹配的 operator delete 函数
inline void operator delete(void* ptr, FDelegateBase& Base) 
{
	::operator delete(ptr);
}

//// 必须提供一个常规的 operator delete，用于 delete 语句
//inline void operator delete(void* ptr) 
//{
//	::operator delete(ptr);
//}
//inline void operator delete(void* ptr)
//{
//	delete ptr;
//}


template <typename UserPolicy>
class TDelegateBase : public UserPolicy::FDelegateExtras
{
	template <typename>
	friend class TMulticastDelegateBase;

	using Super = typename UserPolicy::FDelegateExtras;
	
public:

	/**
	 * Checks to see if the user object bound to this delegate is still valid.
	 *
	 * @return True if the user object is still valid and it's safe to execute the function call.
	 */
	bool IsBound( ) const
	{
		IDelegateInstance* Ptr = Super::GetDelegateInstanceProtected();

		return Ptr && Ptr->IsSafeToExecute();
	}

	/** 
	 * Returns a pointer to an object bound to this delegate, intended for quick lookup in the timer manager,
	 *
	 * @return A pointer to an object referenced by the delegate.
	 */
	const void* GetObjectForTimerManager() const
	{
		IDelegateInstance* Ptr = Super::GetDelegateInstanceProtected();

		const void* Result = Ptr ? Ptr->GetObjectForTimerManager() : nullptr;
		return Result;
	}

	/**
	 * Returns the address of the method pointer which can be used to learn the address of the function that will be executed.
	 * Returns nullptr if this delegate type does not directly invoke a function pointer.
	 *
	 * Note: Only intended to be used to aid debugging of delegates.
	 *
	 * @return The address of the function pointer that would be executed by this delegate
	 */
	int GetBoundProgramCounterForTimerManager() const
	{
		//if (IDelegateInstance* Ptr = Super::GetDelegateInstanceProtected())
		//{
		//	return Ptr->GetBoundProgramCounterForTimerManager();
		//}

		return 0;
	}

	/** 
	 * Checks to see if this delegate is bound to the given user object.
	 *
	 * @return True if this delegate is bound to InUserObject, false otherwise.
	 */
	bool IsBoundToObject( void const* InUserObject ) const
	{
		if (!InUserObject)
		{
			return false;
		}

		IDelegateInstance* Ptr = Super::GetDelegateInstanceProtected();

		return Ptr /*&& Ptr->HasSameObject(InUserObject)*/;
	}
};
