#include "RunableThread.h"
#include "WindowsRunnableThread.h"

FRunnableThread::FRunnableThread()
	: Runnable(nullptr)
	, ThreadID(0)
{

}

FRunnableThread::~FRunnableThread()
{

}

FRunnableThread* FRunnableThread::Create(class FRunnable* InRunnable)
{
	FRunnableThread* NewThread = new FRunnableThreadWin();

	if(NewThread)
	{
		SetupCreatedThread(NewThread, InRunnable);
	}
	return NewThread;
}

void FRunnableThread::SetupCreatedThread(FRunnableThread*& NewThread, class FRunnable* InRunnable)
{
	// Call the thread's create method
	NewThread->CreateInternal(InRunnable);

}