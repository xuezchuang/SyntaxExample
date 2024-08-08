// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once



class IDelegateInstance
{
public:
	/**
	 * Virtual destructor.
	 */
	virtual ~IDelegateInstance() = default;

	/**
	 * Returns a pointer to an object bound to this delegate instance, intended for quick lookup in the timer manager,
	 *
	 * @return A pointer to an object referenced by the delegate instance.
	 */
	virtual const void* GetObjectForTimerManager() const = 0;

	/**
	 * Returns the address of the method pointer which can be used to learn the address of the function that will be executed.
	 * Returns nullptr if this delegate type does not directly invoke a function pointer.
	 *
	 * Note: Only intended to be used to aid debugging of delegates.
	 *
	 * @return The address of the function pointer that would be executed by this delegate
	 */
	virtual int GetBoundProgramCounterForTimerManager() const = 0;

	/**
	 * Returns true if this delegate is bound to the specified UserObject,
	 *
	 * Deprecated.
	 *
	 * @param InUserObject
	 *
	 * @return True if delegate is bound to the specified UserObject
	 */
	virtual bool HasSameObject( const void* InUserObject ) const = 0;

	/**
	 * Checks to see if the user object bound to this delegate can ever be valid again.
	 * used to compact multicast delegate arrays so they don't expand without limit.
	 *
	 * @return True if the user object can never be used again
	 */
	virtual bool IsCompactable( ) const
	{
		return !IsSafeToExecute();
	}

	/**
	 * Checks to see if the user object bound to this delegate is still valid
	 *
	 * @return True if the user object is still valid and it's safe to execute the function call
	 */
	virtual bool IsSafeToExecute( ) const = 0;
};
