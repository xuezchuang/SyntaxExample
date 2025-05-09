#include "WindowsRunnableThread.h"
#include "Runnable.h"

uint32 FRunnableThreadWin::GuardedRun()
{
	uint32 ExitCode = 0;

	//FPlatformProcess::SetThreadAffinityMask(ThreadAffinityMask);

    const wchar_t* ThreadName = L"Render";
	SetThreadDescription(Thread, ThreadName);
	//FPlatformProcess::SetThreadName(*ThreadName);

	ExitCode = Run();

	return ExitCode;
}

uint32 FRunnableThreadWin::Run()
{
	uint32 ExitCode = 1;
	//check(Runnable);
	
	if (Runnable->Init() == true)
	{
		//ThreadInitSyncEvent->Trigger();

		//// Setup TLS for this thread, used by FTlsAutoCleanup objects.
		//SetTls();

		ExitCode = Runnable->Run();

		// Allow any allocated resources to be cleaned up
		Runnable->Exit();

		//FreeTls();
	}
	else
	{
		// Initialization has failed, release the sync event
		//ThreadInitSyncEvent->Trigger();
	}

	return ExitCode;
}
