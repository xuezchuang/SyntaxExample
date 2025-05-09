#pragma once
#include "GenericPlatform.h"
#include <windows.h>

class FRunnable;
/**
* Interface for runnable threads.
*
* This interface specifies the methods used to manage a thread's life cycle.
*/
class FRunnableThread
{
public:

	/**
	* Factory method to create a thread with the specified stack size and thread priority.
	*
	* @param InRunnable The runnable object to execute
	* @param ThreadName Name of the thread
	* @param InStackSize The size of the stack to create. 0 means use the current thread's stack size
	* @param InThreadPri Tells the thread whether it needs to adjust its priority or not. Defaults to normal priority
	* @return The newly created thread or nullptr if it failed
	*/
	static FRunnableThread* Create(class FRunnable* InRunnable);


	/** Default constructor. */
	FRunnableThread();

	/** Virtual destructor */
	virtual ~FRunnableThread();

private:

	/** Used by the thread manager to tick threads in single-threaded mode */
	virtual void Tick() {}
protected:

	/**
	* Creates the thread with the specified stack size and thread priority.
	*
	* @param InRunnable The runnable object to execute
	* @param ThreadName Name of the thread
	* @param InStackSize The size of the stack to create. 0 means use the current thread's stack size
	* @param InThreadPri Tells the thread whether it needs to adjust its priority or not. Defaults to normal priority
	* @return True if the thread and all of its initialization was successful, false otherwise
	*/
	virtual bool CreateInternal(FRunnable* InRunnable) = 0;

	/** The runnable object to execute on this thread. */
	FRunnable* Runnable;

	/** ID set during thread creation. */
	DWORD ThreadID = 0;

private:
	/** Called to setup a newly created RunnableThread */
	static void SetupCreatedThread(FRunnableThread*& NewThread, class FRunnable* InRunnable);


};

