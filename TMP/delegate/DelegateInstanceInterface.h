// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

class FDelegateBase;
class IDelegateInstance;

template <typename FuncType, typename UserPolicy>
struct IBaseDelegateInstance;

template <typename RetType, typename... ArgTypes, typename UserPolicy>
struct IBaseDelegateInstance<RetType(ArgTypes...), UserPolicy> : public UserPolicy::FDelegateInstanceExtras
{
	/**
	 * Emplaces a copy of the delegate instance into the given base delegate.
	 */
	virtual void CreateCopy(typename UserPolicy::FDelegateExtras& Base) const = 0;

	/**
	 * Execute the delegate.  If the function pointer is not valid, an error will occur.
	 */
	virtual RetType Execute(ArgTypes...) const = 0;

	/**
	 * Execute the delegate, but only if the function pointer is still valid
	 *
	 * @return  Returns true if the function was executed
	 */
	// NOTE: Currently only delegates with no return value support ExecuteIfSafe()
	virtual bool ExecuteIfSafe(ArgTypes...) const = 0;
};
