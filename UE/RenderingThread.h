#pragma once
#include "TaskGraphInterfaces.h"
#include "UnrealTemplate.h"

/** The parent class of commands stored in the rendering command queue. */
class FRenderCommand
{
public:
	// All render commands run on the render thread
	static ENamedThreads::Type GetDesiredThread()
	{
		//check(!GIsThreadedRendering || ENamedThreads::GetRenderThread() != ENamedThreads::GameThread);
		return ENamedThreads::RHIThread;
	}

	//static ESubsequentsMode::Type GetSubsequentsMode()
	//{
	//	// Don't support tasks having dependencies on us, reduces task graph overhead tracking and dealing with subsequents
	//	return ESubsequentsMode::FireAndForget;
	//}
};


#define ENQUEUE_RENDER_COMMAND(Type) \
	struct Type##Name \
	{  \
		static const char* CStr() { return #Type; } \
		/*static const TCHAR* TStr() { return TEXT(#Type);*/\
	}; \
	EnqueueUniqueRenderCommand<Type##Name>


template<typename TSTR, typename LAMBDA>
class TEnqueueUniqueRenderCommandType : public FRenderCommand
{
public:
	TEnqueueUniqueRenderCommandType(LAMBDA&& InLambda) : Lambda(Forward<LAMBDA>(InLambda)) {}

	void DoTask()
	{
		//TRACE_CPUPROFILER_EVENT_SCOPE_ON_CHANNEL_STR(TSTR::TStr(), RenderCommandsChannel);
		//FRHICommandListImmediate& RHICmdList = GetImmediateCommandList_ForRenderCommand();
		//Lambda(RHICmdList);
		Lambda();
	}
private:
	LAMBDA Lambda;
};

template<typename TSTR, typename LAMBDA>
void EnqueueUniqueRenderCommand(LAMBDA&& Lambda)
{
	typedef TEnqueueUniqueRenderCommandType<TSTR, LAMBDA> EURCType;
	TGraphTask<EURCType>::CreateTask().ConstructAndDispatchWhenReady(Forward<LAMBDA>(Lambda));
}

void StartRenderingThread();