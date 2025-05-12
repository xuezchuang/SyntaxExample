#pragma once
/**
* Interface for waitable events.
*
* This interface has platform-specific implementations that are used to wait for another
* thread to signal that it is ready for the waiting thread to do some work. It can also
* be used for telling groups of threads to exit.
* 
* Consider using FEventRef as a safer and more convenient alternative.
*/
class FEvent
{
public:

	virtual bool Create( bool bIsManualReset = false ) = 0;

	/**
	* Triggers the event so any waiting threads are activated.
	*
	* @see IsManualReset, Reset
	*/
	virtual void Trigger() = 0;

	/**
	* Resets the event to an untriggered (waitable) state.
	*
	* @see IsManualReset, Trigger
	*/
	virtual void Reset() = 0;

	/**
	* Waits the specified amount of time for the event to be triggered.
	*
	* A wait time of MAX_uint32 is treated as infinite wait.
	*
	* @param WaitTime The time to wait (in milliseconds).
	* @param bIgnoreThreadIdleStats If true, ignores ThreadIdleStats
	* @return true if the event was triggered, false if the wait timed out.
	*/
	virtual bool Wait(uint32 WaitTime, const bool bIgnoreThreadIdleStats = false) = 0;
};

/**
* Implements the Windows version of the FEvent interface.
*/
class FEventWin
	: public FEvent
{
public:

	/** Default constructor. */
	FEventWin()
		: Event(nullptr)
	{
		Create();
	}

	/** Virtual destructor. */
	virtual ~FEventWin()
	{
		if (Event != nullptr)
		{
			CloseHandle(Event);
		}
	}

	// FEvent interface

	virtual bool Create( bool bIsManualReset = false ) override
	{
		// Create the event and default it to non-signaled
		Event = CreateEvent(nullptr, bIsManualReset, 0, nullptr);
		ManualReset = bIsManualReset;

		return Event != nullptr;
	}

	//virtual bool IsManualReset() override
	//{
	//	return ManualReset;
	//}

	virtual void Trigger() override
	{
		//TriggerForStats();
		//check(Event);
		SetEvent(Event);
	}
	virtual void Reset() override
	{
		//ResetForStats();
		//check(Event);
		ResetEvent(Event);
	}
	virtual bool Wait( uint32 WaitTime, const bool bIgnoreThreadIdleStats = false ) override
	{
		//WaitForStats();

		//SCOPE_CYCLE_COUNTER(STAT_EventWait);
		//CSV_SCOPED_WAIT(WaitTime);
		//check(Event);

		//FThreadIdleStats::FScopeIdle Scope(bIgnoreThreadIdleStats);
		DWORD result = WaitForSingleObject(Event, WaitTime);
		//if (result == WAIT_OBJECT_0)
		//{
		//	printf("Event was signaled.\n");
		//}
		//else if (result == WAIT_TIMEOUT)
		//{
		//	printf("Timed out.\n");
		//}
		//else
		//{
		//	printf("Wait failed. Error: %lu\n", GetLastError());
		//}

		return result == WAIT_OBJECT_0;
	}

private:
	/** Holds the handle to the event. */
	HANDLE Event;

	/** Whether the signaled state of the event needs to be reset manually. */
	bool ManualReset;
};