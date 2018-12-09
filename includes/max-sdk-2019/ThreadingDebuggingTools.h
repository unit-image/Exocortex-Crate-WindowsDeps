//////////////////////////////////////////////////////////////////////////////
//
//  Copyright 2018 Autodesk, Inc.  All rights reserved.
//
//  Use of this software is subject to the terms of the Autodesk license 
//  agreement provided at the time of installation or download, or which 
//  otherwise accompanies this software in either electronic or hard copy form.   
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "utilexp.h"

namespace MaxSDK
{
	/** The functions in this namespace are provided to assist in debugging operations performed on a worker thread.
	Many operations in 3ds Max should normally always be performed in the main thread. This includes operations
	such as creating or deleting Animatable-derived objects and Hold system operations. In some controlled cases, these
	operations can be performed in worker threads, but in these cases these operations can only be performed in a single
	worker thread. For example, an importer may display a modal dialog with a progress bar in the main thread and perform
	the import in a worker thread. */
	namespace ThreadingDebuggingTools
	{
		/** Returns true if in main thread. */
		UtilExport bool IsInMainThread();
		/** Sets the worker thread id to use for testing in IsExclusivelyInMainOrWorkerThread function.
		\param threadID - The worker thread id. If a threadID value of -1 is used, the worker thread id is set to the current thread id*/
		UtilExport void SetWorkerThread(DWORD threadID = (DWORD)-1);
		/** Returns the worker thread id set via SetWorkerThread. If no worker thread is set, the returned value is 0.*/
		UtilExport DWORD GetWorkerThread();
		/** Clears the worker thread id value.*/
		UtilExport void ClearWorkerThread();
		/** Returns true if (a) the worker thread id is not set and in main thread, or (b) the worker thread id is set and in that thread. */
		UtilExport bool IsExclusivelyInMainOrWorkerThread();
		/** Disables checking.*/
		UtilExport void Disable();

		/**  A class to help control the setting of the worker thread. Create an instance of this class, and when it is
		destructed the worker thread is set to its original value. */
		class WorkerThreadSet : public MaxHeapOperators {
		public:
			WorkerThreadSet(DWORD threadID = (DWORD)-1) : mResetCalled(false) {
				mOriginalWorkerThreadID = GetWorkerThread();
				SetWorkerThread(threadID);
			}
			~WorkerThreadSet() {
				Reset();
			}
			void Reset()
			{
				if (!mResetCalled)
					SetWorkerThread(mOriginalWorkerThreadID);
				mResetCalled = true;
			}
		private:
			DWORD mOriginalWorkerThreadID;
			bool mResetCalled;
		};
	}
}