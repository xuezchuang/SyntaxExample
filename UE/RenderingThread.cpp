#include "RenderingThread.h"
#include "Runnable.h"
#include "RunableThread.h"

/** The rendering thread main loop */
void RenderingThreadMain()
{
	FTaskGraphInterface::Get().ProcessThreadUntilRequestReturn(ENamedThreads::ActualRenderingThread);
}

/** The rendering thread runnable object. */
class FRenderingThread : public FRunnable
{
public:
	virtual uint32 Run(void) override
	{
		RenderingThreadMain(  );
		return 0;
	}
};

static FRunnable* GRenderingThreadRunnable = NULL;

void StartRenderingThread()
{
	GRenderingThreadRunnable = new FRenderingThread();

	FRunnableThread::Create(GRenderingThreadRunnable);
}
