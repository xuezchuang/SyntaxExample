#pragma once
#include "GenericPlatform.h"
#include "RunableThread.h"
#include <windows.h>

class FRunnable;

/**
* This is the base interface for all runnable thread classes. It specifies the
* methods used in managing its life cycle.
*/
class FRunnableThreadWin
	: public FRunnableThread
{
	/** The thread handle for the thread. */
	HANDLE Thread = 0;


	/**
	* The thread entry point. Simply forwards the call on to the right
	* thread main function
	*/
	static ::DWORD __stdcall _ThreadProc(LPVOID pThis)
	{
		//check(pThis);
		auto* ThisThread = (FRunnableThreadWin*)pThis;
		//FThreadManager::Get().AddThread(ThisThread->GetThreadID(), ThisThread);
		return ThisThread->GuardedRun();
		return 0;
	}

	/** Guarding works only if debugger is not attached or GAlwaysReportCrash is true. */
	uint32 GuardedRun();

	/**
	* The real thread entry point. It calls the Init/Run/Exit methods on
	* the runnable object
	*/
	uint32 Run();

protected:
	virtual bool CreateInternal(FRunnable* InRunnable)override
	{
		uint32 InStackSize = 0;

		Runnable = InRunnable;

		Thread = ::CreateThread(NULL, InStackSize, _ThreadProc, this, STACK_SIZE_PARAM_IS_A_RESERVATION | CREATE_SUSPENDED, (::DWORD*)&ThreadID);

		ResumeThread(Thread);

		return Thread != NULL;
	}
};

