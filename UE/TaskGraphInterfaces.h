#pragma once
#include "GenericPlatform.h"
#include "ThreadSafeCounter.h"
#include <vector>
#include "UnrealTemplate.h"


namespace ENamedThreads
{
	enum Type : int32
	{
		UnusedAnchor = -1,
		/** The always-present, named threads are listed next **/
		RHIThread,
		GameThread,
		ActualRenderingThread = GameThread + 1,
		AnyThread,
	};
}

/** Interface to the task graph system **/
class FTaskGraphInterface
{
	friend class FBaseGraphTask;

	/**
	*	Internal function to queue a task
	*	@param	Task; the task to queue
	*	@param	ThreadToExecuteOn; Either a named thread for a threadlocked task or ENamedThreads::AnyThread for a task that is to run on a worker thread
	*	@param	CurrentThreadIfKnown; This should be the current thread if it is known, or otherwise use ENamedThreads::AnyThread and the current thread will be determined.
	**/
	virtual void QueueTask(class FBaseGraphTask* Task, bool bWakeUpWorker, ENamedThreads::Type ThreadToExecuteOn, ENamedThreads::Type CurrentThreadIfKnown = ENamedThreads::AnyThread) = 0;


public:

	virtual ~FTaskGraphInterface()
	{
	}

	/** 
	*	Explicit start call for the system. The ordinary singleton pattern does not work because internal threads start asking for the singleton before the constructor has returned.
	*	@param	NumThreads; Total number of threads in the system, must be 0 to disable separate taskgraph thread, at least 2 if !threadedrendering, else at least 3
	**/
	static void Startup(int32 NumThreads);

	static FTaskGraphInterface& Get();

	/** 
	*	Requests that a named thread, which must be this thread, run until an explicit return request is received, then return.
	*	@param	CurrentThread; The name of this thread
	**/
	virtual void ProcessThreadUntilRequestReturn(ENamedThreads::Type CurrentThread)=0;

};


/** 
*	Base class for all tasks. 
*	Tasks go through a very specific life stage progression, and this is verified.
**/
class FBaseGraphTask //: private UE::FInheritedContextBase
{
protected:
	/** 
	*	Constructor
	*	@param InNumberOfPrerequistitesOutstanding; the number of prerequisites outstanding. We actually add one to this to prevent the task from firing while we are setting up the task
	**/
	FBaseGraphTask(int32 InNumberOfPrerequistitesOutstanding) :
		NumberOfPrerequistitesOutstanding(InNumberOfPrerequistitesOutstanding + 1) // + 1 is not a prerequisite, it is a lock to prevent it from executing while it is getting prerequisites, one it is safe to execute, call PrerequisitesComplete
	{
		//checkThreadGraph(LifeStage.Increment() == int32(LS_Contructed));
		//CaptureInheritedContext();
	}
	/** 
	*	Sets the desired execution thread. This is not part of the constructor because this information may not be known quite yet duiring construction.
	*	@param InThreadToExecuteOn; the desired thread to execute on.
	**/
	void SetThreadToExecuteOn(ENamedThreads::Type InThreadToExecuteOn)
	{
		ThreadToExecuteOn = InThreadToExecuteOn;
		//checkThreadGraph(LifeStage.Increment() == int32(LS_ThreadSet));
	}

	/** 
	*	Indicates that the prerequisites are set up and that the task can be executed as soon as the prerequisites are finished.
	*	@param NumAlreadyFinishedPrequistes; the number of prerequisites that have not been set up because those tasks had already completed.
	*	@param bUnlock; if true, let the task execute if it can
	**/
	void PrerequisitesComplete(ENamedThreads::Type CurrentThread, int32 NumAlreadyFinishedPrequistes, bool bUnlock = true)
	{
		//checkThreadGraph(LifeStage.Increment() == int32(LS_PrequisitesSetup));
		int32 NumToSub = NumAlreadyFinishedPrequistes + (bUnlock ? 1 : 0); // the +1 is for the "lock" we set up in the constructor
		//if (NumberOfPrerequistitesOutstanding.Subtract(NumToSub) == NumToSub) 
		{
			bool bWakeUpWorker = true;
			QueueTask(CurrentThread, bWakeUpWorker);	
		}
	}
	/** destructor, just checks the life stage **/
	virtual ~FBaseGraphTask()
	{
#if DO_GUARD_SLOW
		int32 Stage = LifeStage.Increment();
		checkf(Stage == int32(LS_Deconstucted), TEXT("LifeStage was %d"), Stage);
#endif
	}


	/** 
	*	An indication that a prerequisite has been completed. Reduces the number of prerequisites by one and if no prerequisites are outstanding, it queues the task for execution.
	*	@param CurrentThread; provides the index of the thread we are running on. This is handy for submitting new taks. Can be ENamedThreads::AnyThread if the current thread is unknown.
	**/
	void ConditionalQueueTask(ENamedThreads::Type CurrentThread, bool& bWakeUpWorker)
	{
		if (NumberOfPrerequistitesOutstanding.Decrement()==0)
		{
			QueueTask(CurrentThread, bWakeUpWorker);
			bWakeUpWorker = true;
		}
	}

	//TaskTrace::FId GetTraceId() const
	//{
	//	return TaskTrace::InvalidId;
	//}

	//void SetTraceId(TaskTrace::FId InTraceId)
	//{
	//}

	//LowLevelTasks::FTask& GetTaskHandle()
	//{
	//	return TaskHandle;
	//}

	//ENamedThreads::Type GetThreadToExecuteOn() const
	//{
	//	return ThreadToExecuteOn;
	//}

private:
	friend class FNamedTaskThread;
	friend class FTaskThreadBase;
	friend class FTaskThreadAnyThread;
	friend class FGraphEvent;
	friend class FTaskGraphImplementation;
	friend class FTaskGraphCompatibilityImplementation;

	//LowLevelTasks::FTask TaskHandle;

	// Subclass API

	/** 
	*	Virtual call to actually execute the task. This will also call the destructor and free any memory in the old backend if bDeleteOnCompletion is set to true.
	*	@param CurrentThread; provides the index of the thread we are running on. This is handy for submitting new tasks.
	*  @param bDeleteOnCompletion; specifies if the task should delete itself after completion.
	**/
	virtual void ExecuteTask(std::vector<FBaseGraphTask*>& NewTasks, ENamedThreads::Type CurrentThread, bool bDeleteOnCompletion)=0;

	/** 
	*	Virtual call to actually delete the task any memory. This is used for the New Backend.
	**/
	virtual void DeleteTask() = 0;

	// API called from other parts of the system
	
	/** 
	*	Called by the system to execute this task after it has been removed from an internal queue.
	*	Just checks the life stage and passes off to the virtual ExecuteTask method.
	*	@param CurrentThread; provides the index of the thread we are running on. This is handy for submitting new tasks.
	*  @param bDeleteOnCompletion; specifies if the task should delete itself after completion.
	**/
	FORCEINLINE void Execute(std::vector<FBaseGraphTask*>& NewTasks, ENamedThreads::Type CurrentThread, bool bDeleteOnCompletion)
	{
		//checkThreadGraph(LifeStage.Increment() == int32(LS_Executing));

		//UE::FInheritedContextScope InheritedContextScope = RestoreInheritedContext();
		ExecuteTask(NewTasks, CurrentThread, bDeleteOnCompletion);
	}

	// Internal Use

	/** 
	*	Queues the task for execution.
	*	@param CurrentThread; provides the index of the thread we are running on. This is handy for submitting new taks. Can be ENamedThreads::AnyThread if the current thread is unknown.
	**/
	void QueueTask(ENamedThreads::Type CurrentThreadIfKnown, bool bWakeUpWorker)
	{
		//checkThreadGraph(LifeStage.Increment() == int32(LS_Queued));
		//TaskTrace::Scheduled(GetTraceId());
		FTaskGraphInterface::Get().QueueTask(this, bWakeUpWorker, ThreadToExecuteOn, CurrentThreadIfKnown);
	}

	/**	Thread to execute on, can be ENamedThreads::AnyThread to execute on any unnamed thread **/
	ENamedThreads::Type			ThreadToExecuteOn;
	/**	Number of prerequisites outstanding. When this drops to zero, the thread is queued for execution.  **/
	FThreadSafeCounter			NumberOfPrerequistitesOutstanding; 
};

using FGraphEventRef = class FGraphEvent*;
/** 
*	A FGraphEvent is a list of tasks waiting for something.
*	These tasks are call the subsequents.
*	A graph event is a prerequisite for each of its subsequents.
*	Graph events have a lifetime managed by reference counting.
**/
class FGraphEvent
{
public:
	static FGraphEventRef CreateGraphEvent()
	{
		return new FGraphEvent();
	}
};



/** 
*	TGraphTask
*	Embeds a user defined task, as exemplified above, for doing the work and provides the functionality for setting up and handling prerequisites and subsequents
**/
template<typename TTask>
class TGraphTask final : /*public TConcurrentLinearObject<TGraphTask<TTask>, FTaskGraphBlockAllocationTag>,*/ public FBaseGraphTask
{
public:
	/** 
	*	This is a helper class returned from the factory. It constructs the embeded task with a set of arguments and sets the task up and makes it ready to execute.
	*	The task may complete before these routines even return.
	**/
	class FConstructor
	{
	public:
		/** Passthrough internal task constructor and dispatch. Note! Generally speaking references will not pass through; use pointers */
		template<typename...T>
		FGraphEventRef ConstructAndDispatchWhenReady(T&&... Args)
		{
			new ((void *)&Owner->TaskStorage) TTask(Forward<T>(Args)...);
			return Owner->Setup(/*Prerequisites,*/ CurrentThreadIfKnown);
		}

		/** Passthrough internal task constructor and hold. */
		template<typename...T>
		TGraphTask* ConstructAndHold(T&&... Args)
		{
			new ((void *)&Owner->TaskStorage) TTask(Forward<T>(Args)...);
			return Owner->Hold(/*Prerequisites,*/ CurrentThreadIfKnown);
		}

	private:
		friend class TGraphTask;

		/** The task that created me to assist with embeded task construction and preparation. **/
		TGraphTask*						Owner;
		/** The list of prerequisites. **/
		//const FGraphEventArray*			Prerequisites;
		/** If known, the current thread.  ENamedThreads::AnyThread is also fine, and if that is the value, we will determine the current thread, as needed, via TLS. **/
		ENamedThreads::Type				CurrentThreadIfKnown;

		/** Constructor, simply saves off the arguments for later use after we actually construct the embeded task. **/
		FConstructor(TGraphTask* InOwner, /*const FGraphEventArray* InPrerequisites, */ENamedThreads::Type InCurrentThreadIfKnown)
			: Owner(InOwner)
			//, Prerequisites(InPrerequisites)
			, CurrentThreadIfKnown(InCurrentThreadIfKnown)
		{
		}
		/** Prohibited copy construction **/
		FConstructor(const FConstructor& Other)
		{
			//check(0);
		}
		/** Prohibited copy **/
		void operator=(const FConstructor& Other)
		{
			//check(0);
		}
	};

	/** 
	*	Factory to create a task and return the helper object to construct the embedded task and set it up for execution.
	*	@param Prerequisites; the list of FGraphEvents that must be completed prior to this task executing.
	*	@param CurrentThreadIfKnown; provides the index of the thread we are running on. Can be ENamedThreads::AnyThread if the current thread is unknown.
	*	@return a temporary helper class which can be used to complete the process.
	**/
	static FConstructor CreateTask(/*const FGraphEventArray* Prerequisites = NULL,*/ ENamedThreads::Type CurrentThreadIfKnown = ENamedThreads::AnyThread)
	{
		int32 NumPrereq = /*Prerequisites ? Prerequisites->Num() :*/ 0;
		return FConstructor(new TGraphTask(FGraphEvent::CreateGraphEvent(), NumPrereq), /*Prerequisites,*/ CurrentThreadIfKnown);
	}

	void Unlock(ENamedThreads::Type CurrentThreadIfKnown = ENamedThreads::AnyThread)
	{
		//TaskTrace::Launched(GetTraceId(), nullptr, Subsequents.IsValid(), ((TTask*)&TaskStorage)->GetDesiredThread());

		bool bWakeUpWorker = true;
		ConditionalQueueTask(CurrentThreadIfKnown, bWakeUpWorker);
	}

	FGraphEventRef GetCompletionEvent()
	{
		return Subsequents;
	}

private:
	friend class FConstructor;
	friend class FGraphEvent;

	// API derived from FBaseGraphTask

	/** 
	*	Virtual call to actually execute the task. 
	*	@param CurrentThread; provides the index of the thread we are running on. This is handy for submitting new taks.
	*  @param bDeleteOnCompletion; specifies if the task will will delete itself after execution (true) or if DeleteTask has to be called manually (false).
	*	Executes the embedded task. 
	*  Destroys the embedded task.
	*	Dispatches the subsequents.
	*	Destroys myself.
	**/
	void ExecuteTask(std::vector<FBaseGraphTask*>& NewTasks, ENamedThreads::Type CurrentThread, bool bDeleteOnCompletion) override
	{
		//checkThreadGraph(TaskConstructed);

		// Fire and forget mode must not have subsequents
		// Track subsequents mode must have subsequents
		//checkThreadGraph(XOR(TTask::GetSubsequentsMode() == ESubsequentsMode::FireAndForget, IsValidRef(Subsequents)));

		//if (TTask::GetSubsequentsMode() == ESubsequentsMode::TrackSubsequents)
		//{
		//	Subsequents->CheckDontCompleteUntilIsEmpty(); // we can only add wait for tasks while executing the task
		//}

		TTask& Task = *(TTask*)&TaskStorage;
		{
			//TaskTrace::FTaskTimingEventScope TaskEventScope(GetTraceId());
			//FScopeCycleCounter Scope(Task.GetStatId(), true);
			Task.DoTask(/*CurrentThread, Subsequents*/);
			Task.~TTask();
			//checkThreadGraph(ENamedThreads::GetThreadIndex(CurrentThread) <= ENamedThreads::GetRenderThread() || FMemStack::Get().IsEmpty()); // you must mark and pop memstacks if you use them in tasks! Named threads are excepted.
		}

		TaskConstructed = false;

		//if (TTask::GetSubsequentsMode() == ESubsequentsMode::TrackSubsequents)
		//{
		//	FPlatformMisc::MemoryBarrier();
		//	Subsequents->DispatchSubsequents(NewTasks, CurrentThread);
		//}

		if (bDeleteOnCompletion)
		{
			DeleteTask();
		}
	}

	void DeleteTask() final override
	{
		delete this;
	}

	// Internals 

	/** 
	*	Private constructor, constructs the base class with the number of prerequisites.
	*	@param InSubsequents subsequents to associate with this task. This refernence is destroyed in the process!
	*	@param NumberOfPrerequistitesOutstanding the number of prerequisites this task will have when it is built.
	**/
	TGraphTask(FGraphEventRef InSubsequents, int32 NumberOfPrerequistitesOutstanding)
		: FBaseGraphTask(NumberOfPrerequistitesOutstanding)
		, TaskConstructed(false)
	{
		//Subsequents.Swap(InSubsequents);
		//SetTraceId(Subsequents.IsValid() ? Subsequents->GetTraceId() : TaskTrace::GenerateTaskId());
	}

	/** 
	*	Private destructor, just checks that the task appears to be completed
	**/
	~TGraphTask() override
	{
		//checkThreadGraph(!TaskConstructed);
	}

	/** 
	*	Call from FConstructor to complete the setup process
	*	@param Prerequisites; the list of FGraphEvents that must be completed prior to this task executing.
	*	@param CurrentThreadIfKnown; provides the index of the thread we are running on. Can be ENamedThreads::AnyThread if the current thread is unknown.
	*	@param bUnlock; if true, task can execute right now if possible
	* 
	*	Create the completed event
	*	Set the thread to execute on based on the embedded task
	*	Attempt to add myself as a subsequent to each prerequisite
	*	Tell the base task that I am ready to start as soon as my prerequisites are ready.
	**/
	void SetupPrereqs(/*const FGraphEventArray* Prerequisites, */ENamedThreads::Type CurrentThreadIfKnown, bool bUnlock)
	{
		//checkThreadGraph(!TaskConstructed);
		//TaskConstructed = true;
		//TTask& Task = *(TTask*)&TaskStorage;
		//SetThreadToExecuteOn(Task.GetDesiredThread());
		//int32 AlreadyCompletedPrerequisites = 0;
		//if (Prerequisites)
		//{
		//	for (int32 Index = 0; Index < Prerequisites->Num(); Index++)
		//	{
		//		FGraphEvent* Prerequisite = (*Prerequisites)[Index];
		//		if (Prerequisite == nullptr || !Prerequisite->AddSubsequent(this))
		//		{
		//			AlreadyCompletedPrerequisites++;
		//		}
		//	}
		//}
		PrerequisitesComplete(CurrentThreadIfKnown,/* AlreadyCompletedPrerequisites, */bUnlock);
	}

	/** 
	*	Call from FConstructor to complete the setup process
	*	@param Prerequisites; the list of FGraphEvents that must be completed prior to this task executing.
	*	@param CurrentThreadIfKnown; provides the index of the thread we are running on. Can be ENamedThreads::AnyThread if the current thread is unknown.
	*	@return A new graph event which represents the completion of this task.
	* 
	*	Create the completed event
	*	Set the thread to execute on based on the embedded task
	*	Attempt to add myself as a subsequent to each prerequisite
	*	Tell the base task that I am ready to start as soon as my prerequisites are ready.
	**/
	FGraphEventRef Setup(/*const FGraphEventArray* Prerequisites = NULL,*/ ENamedThreads::Type CurrentThreadIfKnown = ENamedThreads::AnyThread)
	{
		//TaskTrace::Launched(GetTraceId(), nullptr, Subsequents.IsValid(), ((TTask*)&TaskStorage)->GetDesiredThread());

		FGraphEventRef ReturnedEventRef = Subsequents; // very important so that this doesn't get destroyed before we return
		SetupPrereqs(/*Prerequisites, */CurrentThreadIfKnown, true);
		return ReturnedEventRef;
	}

	/** 
	*	Call from FConstructor to complete the setup process, but doesn't allow the task to dispatch yet
	*	@param Prerequisites; the list of FGraphEvents that must be completed prior to this task executing.
	*	@param CurrentThreadIfKnown; provides the index of the thread we are running on. Can be ENamedThreads::AnyThread if the current thread is unknown.
	*	@return a pointer to the task
	* 
	*	Create the completed event
	*	Set the thread to execute on based on the embedded task
	*	Attempt to add myself as a subsequent to each prerequisite
	*	Tell the base task that I am ready to start as soon as my prerequisites are ready.
	**/
	TGraphTask* Hold(/*const FGraphEventArray* Prerequisites = NULL, */ENamedThreads::Type CurrentThreadIfKnown = ENamedThreads::AnyThread)
	{
		//TaskTrace::Created(GetTraceId());

		SetupPrereqs(/*Prerequisites, */CurrentThreadIfKnown, false);
		return this;
	}

	/** 
	*	Factory to create a gather task which assumes the given subsequent list from some other tasks.
	*	This is used to support "WaitFor" during a task execution.
	*	@param SubsequentsToAssume; subsequents to "assume" from an existing task
	*	@param Prerequisites; the list of FGraphEvents that must be completed prior to dispatching my seubsequents.
	*	@param CurrentThreadIfKnown; provides the index of the thread we are running on. Can be ENamedThreads::AnyThread if the current thread is unknown.
	**/
	//static FConstructor CreateTask(FGraphEventRef SubsequentsToAssume, /*const FGraphEventArray* Prerequisites = NULL, */ENamedThreads::Type CurrentThreadIfKnown = ENamedThreads::AnyThread)
	//{
	//	return FConstructor(new TGraphTask(SubsequentsToAssume, Prerequisites ? Prerequisites->Num() : 0), Prerequisites, CurrentThreadIfKnown);
	//}

	/** An aligned bit of storage to hold the embedded task **/
	TAlignedBytes<sizeof(TTask),alignof(TTask)> TaskStorage;
	/** Used to sanity check the state of the object **/
	bool						TaskConstructed;
	/** A reference counted pointer to the completion event which lists the tasks that have me as a prerequisite. **/
	FGraphEventRef				Subsequents;
};
