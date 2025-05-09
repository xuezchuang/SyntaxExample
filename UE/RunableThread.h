#pragma once

/**
* Interface for runnable threads.
*
* This interface specifies the methods used to manage a thread's life cycle.
*/
class FRunnableThread
{
private:

	/** Used by the thread manager to tick threads in single-threaded mode */
	virtual void Tick() {}
};
