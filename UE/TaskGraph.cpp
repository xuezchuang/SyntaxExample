#include "TaskGraph.h"
#include "RenderingThread.h"
#include "TaskGraphInterfaces.h"
#include "LockFreeList.h"
#include "RunableThread.h"
#include "Event.h"
#include <malloc.h> 
#include <thread>  
#include <chrono>  

#pragma region TaskGraph

/************************************************************************/
/*					TaskGraph											*/
/************************************************************************/

/**
ENQUEUE_RENDER_COMMAND(...)
↓
TGraphTask 构建任务，设置目标线程 = RenderThread
↓
调用 ConstructAndDispatchWhenReady → PrerequisitesComplete
↓
前置条件满足，调用 QueueTask
↓
任务被加入 RenderThread 的任务队列
↓
下一帧 RenderThread 调用 ProcessThreadUntilIdle 拉取并执行任务

 */

#pragma endregion

static FTaskGraphInterface* TaskGraphImplementationSingleton = NULL;

/**
*	FWorkerThread
*	Helper structure to aggregate a few items related to the individual threads.
**/
struct FWorkerThread
{
	/** The actual FTaskThread that manager this task **/
	FTaskThreadBase* TaskGraphWorker;
	/** For internal threads, the is non-NULL and holds the information about the runable thread that was created. **/
	FRunnableThread* RunnableThread;
	/** For external threads, this determines if they have been "attached" yet. Attachment is mostly setting up TLS for this individual thread. **/
	bool				bAttached;

	/** Constructor to set reasonable defaults. **/
	FWorkerThread()
		: TaskGraphWorker(nullptr)
		, RunnableThread(nullptr)
		, bAttached(false)
	{
	}
};

/**
*	FTaskGraphImplementation
*	Implementation of the centralized part of the task graph system.
*	These parts of the system have no knowledge of the dependency graph, they exclusively work on tasks.
**/

/** 
*	FTaskThreadBase
*	Base class for a thread that executes tasks
*	This class implements the FRunnable API, but external threads don't use that because those threads are created elsewhere.
**/
class FTaskThreadBase
{
public:
	/** Constructor, initializes everything to unusable values. Meant to be called from a "main" thread. **/
	FTaskThreadBase()
		: OwnerWorker(nullptr)
	{
		NewTasks.resize(128);
	}

	/** 
	*	Sets up some basic information for a thread. Meant to be called from a "main" thread. Also creates the stall event.
	*	@param InThreadId; Thread index for this thread.
	*	@param InPerThreadIDTLSSlot; TLS slot to store the pointer to me into (later)
	**/
	void Setup(ENamedThreads::Type InThreadId,/* uint32 InPerThreadIDTLSSlot,*/ FWorkerThread* InOwnerWorker)
	{
		ThreadId = InThreadId;
		//check(ThreadId >= 0);
		//PerThreadIDTLSSlot = InPerThreadIDTLSSlot;
		OwnerWorker = InOwnerWorker;
	}


	uint32 Run()
	{
		//check(OwnerWorker); // make sure we are started up
		ProcessTasksUntilQuit(0);
		//FMemory::ClearAndDisableTLSCachesOnCurrentThread();
		return 0;
	}

	/** Used for named threads to start processing tasks until the thread is idle and RequestQuit has been called. **/
	virtual void ProcessTasksUntilQuit(int32 QueueIndex) = 0;

	virtual bool EnqueueFromOtherThread(int32 QueueIndex, FBaseGraphTask* Task) = 0;

protected:
	std::vector<FBaseGraphTask*> NewTasks;
	/** Id / Index of this thread. **/
	ENamedThreads::Type									ThreadId;
	/** back pointer to the owning FWorkerThread **/
	FWorkerThread* OwnerWorker;
};

/** 
*	FNamedTaskThread
*	A class for managing a named thread. 
*/
class FNamedTaskThread : public FTaskThreadBase
{
public:

	virtual void ProcessTasksUntilQuit(int32 QueueIndex) override
	{
		//check(Queue(QueueIndex).StallRestartEvent); // make sure we are started up

		Queues.QuitForReturn = false;
		//verify(++Queue(QueueIndex).RecursionGuard == 1);
		const bool bIsMultiThread = true;// FTaskGraphInterface::IsMultithread();
		do
		{
			const bool bAllowStall = bIsMultiThread;
			ProcessTasksNamedThread(QueueIndex, bAllowStall);
		} while (!Queues.QuitForReturn && !Queues.QuitForShutdown && bIsMultiThread); // @Hack - quit now when running with only one thread.
		//verify(!--Queues.RecursionGuard);
	}

	uint64 ProcessTasksNamedThread(int32 QueueIndex, bool bAllowStall)
	{
		uint64 ProcessedTasks = 0;

		//TStatId StallStatId;
		//bool bCountAsStall = false;
		//const bool bIsRenderThreadMainQueue = (ENamedThreads::GetThreadIndex(ThreadId) == ENamedThreads::ActualRenderingThread) && (QueueIndex == 0);
		while (!Queues.QuitForReturn)
		{
			//const bool bIsRenderThreadAndPolling = bIsRenderThreadMainQueue && (GRenderThreadPollPeriodMs >= 0);
			//const bool bStallQueueAllowStall = bAllowStall && !bIsRenderThreadAndPolling;
			FBaseGraphTask* Task = Queues.StallQueue.Pop(0, true);
			//TestRandomizedThreads();
			if (!Task)
			{
				//std::this_thread::sleep_for(std::chrono::milliseconds(200));  
				//if (bAllowStall)
				//{
				//	TRACE_CPUPROFILER_EVENT_SCOPE(WaitForTasks);
				//	{
				//		FScopeCycleCounter Scope(StallStatId, EStatFlags::Verbose);
						Queues.StallRestartEvent->Wait(INFINITE,false);
				//		if (Queue(QueueIndex).QuitForShutdown)
				//		{
				//			return ProcessedTasks;
				//		}
				//		//TestRandomizedThreads();
				//	}
				//	continue;
				//}
				//else
				//{
				//	break; // we were asked to quit
				//}
			}
			else
			{
				Task->Execute(NewTasks, ENamedThreads::ActualRenderingThread, true);
				ProcessedTasks++;
				//TestRandomizedThreads();
			}
		}
		return ProcessedTasks;
	}

	virtual bool EnqueueFromOtherThread(int32 QueueIndex, FBaseGraphTask* Task) override
	{
		//TestRandomizedThreads();
		//checkThreadGraph(Task && Queue(QueueIndex).StallRestartEvent); // make sure we are started up

		uint32 PriIndex = /*ENamedThreads::GetTaskPriority(Task->GetThreadToExecuteOn()) ? 0 :*/ 0;
		int32 ThreadToStart = Queues.StallQueue.Push(Task, PriIndex);

		if (ThreadToStart >= 0)
		{
			//checkThreadGraph(ThreadToStart == 0);
			//QUICK_SCOPE_CYCLE_COUNTER(STAT_TaskGraph_EnqueueFromOtherThread_Trigger);
			//TASKGRAPH_SCOPE_CYCLE_COUNTER(1, STAT_TaskGraph_EnqueueFromOtherThread_Trigger);
			Queues.StallRestartEvent->Trigger();
			return true;
		}
		return false;
	}

private:
	/** Grouping of the data for an individual queue. **/
	struct FThreadTaskQueue
	{
		FStallingTaskQueue<FBaseGraphTask, PLATFORM_CACHE_LINE_SIZE, 2> StallQueue;

		/** We need to disallow reentry of the processing loop **/
		uint32 RecursionGuard;

		/** Indicates we executed a return task, so break out of the processing loop. **/
		bool QuitForReturn;

		/** Indicates we executed a return task, so break out of the processing loop. **/
		bool QuitForShutdown;

		/** Event that this thread blocks on when it runs out of work. **/
		FEvent* StallRestartEvent;

		FThreadTaskQueue()
			: RecursionGuard(0)
			, QuitForReturn(false)
			, QuitForShutdown(false)
			, StallRestartEvent(new FEventWin())
		{

		}
		~FThreadTaskQueue()
		{
			//FPlatformProcess::ReturnSynchEventToPool(StallRestartEvent);
			//StallRestartEvent = nullptr;
		}
	};

	FThreadTaskQueue Queues;

};



class FTaskGraphImplementation final : public FTaskGraphInterface
{
public:
	FTaskGraphImplementation(int32)
	{
		int32 NumThreads = ENamedThreads::AnyThread;
		for (int32 ThreadIndex = 0; ThreadIndex < NumThreads; ThreadIndex++)
		{
			void* mem = _aligned_malloc(sizeof(FNamedTaskThread), alignof(FNamedTaskThread));


			WorkerThreads[ThreadIndex].TaskGraphWorker = new (mem)FNamedTaskThread();
			WorkerThreads[ThreadIndex].TaskGraphWorker->Setup(ENamedThreads::Type(ThreadIndex), /*PerThreadIDTLSSlot,*/ &WorkerThreads[ThreadIndex]);
		}

		TaskGraphImplementationSingleton = this;
	}


	/** 
	*	Function to queue a task, called from a FBaseGraphTask
	*	@param	Task; the task to queue
	*	@param	ThreadToExecuteOn; Either a named thread for a threadlocked task or ENamedThreads::AnyThread for a task that is to run on a worker thread
	*	@param	CurrentThreadIfKnown; This should be the current thread if it is known, or otherwise use ENamedThreads::AnyThread and the current thread will be determined.
	**/
	virtual void QueueTask(FBaseGraphTask* Task, bool bWakeUpWorker, ENamedThreads::Type ThreadToExecuteOn, ENamedThreads::Type InCurrentThreadIfKnown = ENamedThreads::AnyThread) final override
	{
		//TASKGRAPH_SCOPE_CYCLE_COUNTER(2, STAT_TaskGraph_QueueTask);

		//if (ENamedThreads::GetThreadIndex(ThreadToExecuteOn) == ENamedThreads::AnyThread)
		{
			//TASKGRAPH_SCOPE_CYCLE_COUNTER(3, STAT_TaskGraph_QueueTask_AnyThread);
			//if (FTaskGraphInterface::IsMultithread())
			{
				//uint32 TaskPriority = ENamedThreads::GetTaskPriority(Task->GetThreadToExecuteOn());
				//int32 Priority = ENamedThreads::GetThreadPriorityIndex(Task->GetThreadToExecuteOn());
				//if (Priority == (ENamedThreads::BackgroundThreadPriority >> ENamedThreads::ThreadPriorityShift) && (!bCreatedBackgroundPriorityThreads || !ENamedThreads::bHasBackgroundThreads))
				//{
				//	Priority = ENamedThreads::NormalThreadPriority >> ENamedThreads::ThreadPriorityShift; // we don't have background threads, promote to normal
				//	TaskPriority = ENamedThreads::NormalTaskPriority >> ENamedThreads::TaskPriorityShift; // demote to normal task pri
				//}
				//else if (Priority == (ENamedThreads::HighThreadPriority >> ENamedThreads::ThreadPriorityShift) && (!bCreatedHiPriorityThreads || !ENamedThreads::bHasHighPriorityThreads))
				//{
				//	Priority = ENamedThreads::NormalThreadPriority >> ENamedThreads::ThreadPriorityShift; // we don't have hi priority threads, demote to normal
				//	TaskPriority = ENamedThreads::HighTaskPriority >> ENamedThreads::TaskPriorityShift; // promote to hi task pri
				//}
				//uint32 PriIndex = TaskPriority ? 0 : 1;
				//check(Priority >= 0 && Priority < MAX_THREAD_PRIORITIES);
				//{
				//	TASKGRAPH_SCOPE_CYCLE_COUNTER(4, STAT_TaskGraph_QueueTask_IncomingAnyThreadTasks_Push);
				//	int32 IndexToStart = IncomingAnyThreadTasks[Priority].Push(Task, PriIndex);
				//	if (IndexToStart >= 0)
				//	{
				//		StartTaskThread(Priority, IndexToStart);
				//	}
				//}
				//return;
			}
			//else
			//{
			//	ThreadToExecuteOn = ENamedThreads::GameThread;
			//}
		}
		//ENamedThreads::Type CurrentThreadIfKnown;
		//if (ENamedThreads::GetThreadIndex(InCurrentThreadIfKnown) == ENamedThreads::AnyThread)
		//{
		//	CurrentThreadIfKnown = GetCurrentThread();
		//}
		//else
		//{
		//	CurrentThreadIfKnown = ENamedThreads::GetThreadIndex(InCurrentThreadIfKnown);
		//	checkThreadGraph(CurrentThreadIfKnown == ENamedThreads::GetThreadIndex(GetCurrentThread()));
		//}
		{
			//int32 QueueToExecuteOn = ENamedThreads::GetQueueIndex(ThreadToExecuteOn);
			//ThreadToExecuteOn = ENamedThreads::GetThreadIndex(ThreadToExecuteOn);
			FTaskThreadBase* Target = &Thread(ENamedThreads::ActualRenderingThread);
			//if (ThreadToExecuteOn == ENamedThreads::GetThreadIndex(CurrentThreadIfKnown))
			//{
			//	Target->EnqueueFromThisThread(QueueToExecuteOn, Task);
			//}
			//else
			{
				Target->EnqueueFromOtherThread(0, Task);
			}
		}
	}

private:

	/** 
	*	Internal function to verify an index and return the corresponding FTaskThread
	*	@param	Index; Id of the thread to retrieve.
	*	@return	Reference to the corresponding thread.
	**/
	FTaskThreadBase& Thread(int32 Index)
	{
		//checkThreadGraph(Index >= 0 && Index < NumThreads);
		//checkThreadGraph(WorkerThreads[Index].TaskGraphWorker->GetThreadId() == Index);
		return *WorkerThreads[Index].TaskGraphWorker;
	}

	void ProcessThreadUntilRequestReturn(ENamedThreads::Type CurrentThread) final override
	{
		//int32 QueueIndex = ENamedThreads::GetQueueIndex(CurrentThread);
		//CurrentThread = ENamedThreads::GetThreadIndex(CurrentThread);
		//check(CurrentThread >= 0 && CurrentThread < NumNamedThreads);
		//check(CurrentThread == GetCurrentThread());
		Thread(CurrentThread).ProcessTasksUntilQuit(0);
	}

	enum
	{
		/** Compile time maximum number of threads. Didn't really need to be a compile time constant, but task thread are limited by MAX_LOCK_FREE_LINKS_AS_BITS **/
		MAX_THREADS = 0xFFFF,
		MAX_THREAD_PRIORITIES = 3
	};

	/** Per thread data. **/
	FWorkerThread		WorkerThreads[MAX_THREADS];
};

FTaskGraphInterface& FTaskGraphInterface::Get()
{
	//checkThreadGraph(TaskGraphImplementationSingleton);
	return *TaskGraphImplementationSingleton;
}

void FTaskGraphInterface::Startup(int32 NumThreads)
{
	new FTaskGraphImplementation(NumThreads);
}




int main()
{
	FTaskGraphInterface::Get().Startup(0);




	StartRenderingThread();

	int a = 0;
	do 
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(500));  
		a++;
		ENQUEUE_RENDER_COMMAND(CaptureCommand)(
			[&a]()
			{
				printf("%d\n",a);
			}
			);

	} while (1);

	return 0;
}

/*

FORCEINLINE_DEBUGGABLE void EnqueueUniqueRenderCommand(LAMBDA&& Lambda)


FRHICommandList::DrawIndexedPrimitive
FRHICOMMAND_MACRO(FRHICommandDrawIndexedPrimitive)


//RHI Thread
FD3D12CommandContext::RHIDrawIndexedPrimitive


bool FMeshDrawCommand::SubmitDrawBegin
void FDeferredShadingSceneRenderer::RenderPrePass(FRDGBuilder& GraphBuilder, FRDGTextureRef SceneDepthTexture, FInstanceCullingManager& InstanceCullingManager)
FDeferredShadingSceneRenderer::Render
RenderViewFamilies_RenderThread
ENQUEUE_RENDER_COMMAND(FDrawSceneCommand)

Trace.File
Trace.Stop

SCOPED_NAMED_EVENT_TEXT
SCOPED_NAMED_EVENT

[GameThread]

UEditorEngine::UpdateSingleViewportClient
	FViewport::Draw
		void FEditorViewportClient::Draw
			CalcSceneView
				View->EndFinalPostprocessSettings
			for (auto ViewExt : ViewFamily.ViewExtensions)
			{
				ViewExt->SetupViewFamily(ViewFamily);
			}
			GetRendererModule().BeginRenderingViewFamily

FRendererModule::BeginRenderingViewFamilies
[RenderThread]
RenderViewFamilies_RenderThread
	ViewExtensionPreRender_RenderThread
		ISceneViewExtension::PreRenderView_RenderThread
	FDeferredShadingSceneRenderer::Render
		Scene->UpdateAllPrimitiveSceneInfos					2445
		BeginInitViews										2638
			ComputeViewVisibility							5293
				GatherDynamicMeshElements
				SetupMeshPass
					FParallelMeshDrawCommandPass::DispatchPassSetup
		CommitFinalPipelineState							2670
			CommitIndirectLightingState
		FSceneTextures::InitializeViewFamily(GraphBuilder, ViewFamily);		2805
		FSceneTextures& SceneTextures = GetActiveSceneTextures();			2806
		EndInitViews										2953
			FinishInitDynamicShadows
				GatherShadowDynamicMeshElements
					SetupMeshDrawCommandsForShadowDepth
						ShadowDepthPass.DispatchPassSetup
		RenderPrePass(GraphBuilder, SceneTextures.Depth.Target, InstanceCullingManager);	2993
			View.ParallelMeshDrawCommandPasses[EMeshPass::DepthPass].DispatchDraw
		RenderOcclusion										3271
		FDeferredShadingSceneRenderer::RenderBasePass		3431
			RenderBasePassInternal
				View.ParallelMeshDrawCommandPasses[EMeshPass::BasePass].DispatchDraw
				View.ParallelMeshDrawCommandPasses[EMeshPass::SkyPass].DispatchDraw
		CompositionLighting.ProcessAfterBasePass			3691
			AddPostProcessingAmbientOcclusion
		RenderLights										3757
			RenderDeferredShadowProjections
			RenderLight
		RenderDiffuseIndirectAndAmbientOcclusion			3764
		RenderDeferredReflectionsAndSkyLighting				3775
			AddSkyReflectionPass							2012
		AddSubsurfacePass
		AddPostProcessingPasses
			AddDownsamplePass

	
*/

/*

FParallelMeshDrawCommandPass::DispatchPassSetup
	FMeshDrawCommandPassSetupTask
		GenerateDynamicMeshDrawCommands
			FDepthPassMeshProcessor::AddMeshBatch
				FMeshPassProcessor::BuildMeshDrawCommands

FParallelMeshDrawCommandPass::DispatchDraw
	FRHICommandList* CmdList = ParallelCommandListSet->NewParallelCommandList();
	FDrawVisibleMeshCommandsAnyThreadTask(CmdList) 开启并行线程录制命令
		FInstanceCullingContext::SubmitDrawCommands
			FMeshDrawCommand::SubmitDraw
	ParallelCommandListSet->AddParallelCommandList(CmdList, AnyThreadCompletionEvent, NumDraws);

从 TaskContext.PassType可以看到Type
*/


/**
* 
* FDepthPassMeshProcessor::Process
*/

/**

FScene::AddPrimitive

SetGraphicsPipelineState
FRHICommandList::SetGraphicsPipelineState
FRHICOMMAND_MACRO(FRHICommandSetGraphicsPipelineState)
FRHICommandSetGraphicsPipelineState::Execute
RHISetGraphicsPipelineState


FRHICommandList::SDrawIndexedPrimitive


 */

/**
 * 
 * *((UnrealEditor-Engine-Win64-Debug.dll!FLocalVertexFactory*)VertexFactory)->GetType()
 * UnrealEditor-Engine-Win64-Debug.dll!FLocalVertexFactory
 * ((TArray<FRDGPass *,TSizedInlineAllocator<1,32,TRDGArrayAllocator<0> > >::ElementType*)ResourcePass->ResourcesToBegin.AllocatorInstance.InlineData)[0]->TextureStates
 * 
 * 
 */

/*

FD3D12TransientResourceHeapAllocator::CreateTexture
GraphBuilder.AllocParameters
GraphBuilder.CreateUniformBuffer

*/


/**
 * 取消勾选PostProcessingEnable,在AmbientOcclusion的Intensity勾选,还是不能计算AO的原因
 * FEngineShowFlags::DisableAdvancedFeatures
 * 在FSceneView::EndFinalPostprocessSettings中,会强制对AmbientOcclusionIntensity设置的数据设置为0.
 * 	if(!Family->EngineShowFlags.AmbientOcclusion || !Family->EngineShowFlags.ScreenSpaceAO)
	{
		FinalPostProcessSettings.AmbientOcclusionIntensity = 0;
	}

 */

/**	RHIThread 是怎么和其他线程(RenderThread,其他线程也可以分配ParentCmdList)消费/生产锁的.
* FRHICommandBase*    Root
* FRHICommandListImmediate::ExecuteAndReset
* FRHICommandList::DrawIndexedPrimitiveIndirect
* FParallelCommandListSet::Dispatch中调用<FRHICommandListImmediate::QueueAsyncCommandListSubmit>
*	将ParallelCommand 给到FRHICommandListImmediate也就是RHI的总Command中
* FRDGBuilder::Execute
* 
* 
* QueueAsyncCommandListSubmit
*	CmdList->WaitForDispatchEvent();在RHIThread中会检查等待命令列表是否录制完成.DispatchEvent->IsComplete()
* FRHICommandListBase::FinishRecording
*	DispatchEvent->DispatchSubsequents();在别的线程中录制命令完成
* 
* 在FMeshDrawCommandPassSetupTask中,这个任务在FParallelMeshDrawCommandPass::DispatchPassSetup创建
* GenerateDynamicMeshDrawCommands
 */

/**
 * GatherDynamicMeshElements
	PrimitiveSceneInfo->Proxy->GetDynamicMeshElements(FStaticMeshSceneProxy::GetDynamicMeshElements)
		FStaticMeshSceneProxy::GetMeshElement
		Collector.AddMesh(ViewIndex, MeshElement);
			
 *
 * MeshDrawCommand.VertexStreams	VertexBuffer
 * FVertexFactory::GetStreams
 * 
 * 顶点数据在这个函数初始化FStaticMeshLODResources::InitResources
 * 在RenderThread0中是FPositionVertexBuffer::InitRHI
 *		FShaderResourceViewRHIRef FD3D12DynamicRHI::RHICreateShaderResourceView(const FShaderResourceViewInitializer& Initializer)
 * FBufferRHIRef FPositionVertexBuffer::CreateRHIBuffer_Internal() 创建顶点资源
 * FStaticMeshRenderData::InitResources
 * FStaticMeshVertexFactories::InitVertexFactory 将创建好的PositionVertexBuffer
 * FStaticMeshVertexFactories::InitResources这个函数做的就是把FStaticMeshVertexFactories::VertexFactory绑定到刚刚创建的
 * UStaticMeshComponent::CreateSceneProxy
 */


/**  --- 顶点数据从文件加载后,创建资源到d3d12绑定数据的过程        ---
 * 1.创建资源	FStaticMeshRenderData::InitResources
 *		在创建/复制一个StaticMesh后,UStaticMesh::FinishPostLoadInternal --- UStaticMesh::InitResources  --- FStaticMeshRenderData::InitResources
 *		LODResource和LODVertexFactories会初始化资源.资源存储在FLocalVertexFactory VertexFactory(LODVertexFactories);
 * 2.拷贝数据	FStaticMeshSceneProxy::GetMeshElement
 *		callstack
 *		FDeferredShadingSceneRenderer::BeginInitViews
 *			FSceneRenderer::ComputeViewVisibility
 *				FSceneRenderer::GatherDynamicMeshElements
 *					FStaticMeshSceneProxy::GetDynamicMeshElements
 *						FStaticMeshSceneProxy::GetMeshElement
 *		这个流程中,会把需要绘制的数据拷贝到	FSceneRenderer::TArray<FViewInfo> Views;中TArray<FMeshBatchAndRelevance,SceneRenderingAllocator> DynamicMeshElements
 *		数据是在View[0].DynamicMeshElements中 
 *	3.传Pass	FMeshPassProcessor::BuildMeshDrawCommands
 *		FParallelMeshDrawCommandPass::DispatchPassSetup会调用FMeshDrawCommandPassSetupTask在任务中执行以下操作.
 *		VertexFactory->GetStreams(FeatureLevel, InputStreamType, SharedMeshDrawCommand.VertexStreams);
 *		会从VertexFactory取到需要的顶点资源放到SharedMeshDrawCommand.VertexStreams
 *	4.绑定RHI
 *		在FDeferredShadingSceneRenderer::RenderPrePass或者其他pass中执行绘图命令
 *		GraphBuilder.AddPass中的View.ParallelMeshDrawCommandPasses[EMeshPass::DepthPass].DispatchDraw分配任务FDrawVisibleMeshCommandsAnyThreadTask
 *		任务中执行FMeshDrawCommand::SubmitDrawBegin,RHICmdList.SetStreamSource(Stream.StreamIndex, ScenePrimitiveIdsBuffer, PrimitiveIdOffset);
 *		绑定了顶点的GPU资源
 *			
 *			
 * 
 */