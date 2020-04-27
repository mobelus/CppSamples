#include "stdafx.h"
#include "ReadDirectoryChanges.h"

// WORKS TOO

#if 0

LPCWSTR ExplainAction( DWORD dwAction );
bool TryGetKeyboardInput( HANDLE hStdIn, bool &bTerminate, char* buf );


//
// When the application starts, it immediately starts monitoring your home
// directory, including children, as well as C:\, not including children.
// The application exits when you hit Esc.
// You can add a directory to the monitoring list by typing the directory
// name and hitting Enter. Notifications will pause while you type.
//

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	const DWORD dwNotificationFlags =
//		FILE_NOTIFY_CHANGE_CREATION
//		;
		FILE_NOTIFY_CHANGE_LAST_WRITE
		| FILE_NOTIFY_CHANGE_CREATION
		| FILE_NOTIFY_CHANGE_FILE_NAME
		;

	// Create the monitor and add two directories.
	CReadDirectoryChanges changes;
	//changes.AddDirectory(_tgetenv(_T("USERPROFILE")), true, dwNotificationFlags);
	changes.AddDirectory(_T("C:\\Users\\Public\\_"), false, dwNotificationFlags);

	HANDLE hStdIn =  ::GetStdHandle(STD_INPUT_HANDLE);
	const HANDLE handles[] = { hStdIn, changes.GetWaitHandle() };

	char buf[MAX_PATH];
	bool bTerminate = false;

	while (!bTerminate)
	{
		DWORD rc = ::WaitForMultipleObjectsEx(_countof(handles), handles, false, INFINITE, true);
		switch (rc)
		{
		case WAIT_OBJECT_0 + 0:
			// hStdIn was signaled. This can happen due to mouse input, focus change,
			// Shift keys, and more.  Delegate to TryGetKeyboardInput().
			// TryGetKeyboardInput sets bTerminate to true if the user hits Esc.
			if (TryGetKeyboardInput(hStdIn, bTerminate, buf))
				changes.AddDirectory(CStringW(buf), false, dwNotificationFlags);
			break;
//*
		case WAIT_OBJECT_0 + 1:
			// We've received a notification in the queue.
			{
				DWORD dwAction;
				CStringW wstrFilename;
				if (changes.CheckOverflow())
					wprintf(L"Queue overflowed.\n");
				else
				{
					//changes.First(wstrFilename);
					changes.Pop(dwAction, wstrFilename);
					//wprintf(L"%s %s\n", ExplainAction(dwAction), wstrFilename);
					wprintf(L"%s \n", wstrFilename);
				}
			}
			break;
		case WAIT_IO_COMPLETION:
			// Nothing to do.
			break;
//*/
		}
	}

	// Just for sample purposes. The destructor will
	// call Terminate() automatically.
	changes.Terminate();

	return EXIT_SUCCESS;
}

LPCWSTR ExplainAction( DWORD dwAction )
{
	switch (dwAction)
	{
	case FILE_ACTION_ADDED            :
		return L"Added";
	case FILE_ACTION_REMOVED          :
		return L"Deleted";
	case FILE_ACTION_MODIFIED         :
		return L"Modified";
	case FILE_ACTION_RENAMED_OLD_NAME :
		return L"Renamed From";
	case FILE_ACTION_RENAMED_NEW_NAME :
		return L"Renamed To";
	default:
		return L"BAD DATA";
	}
}

bool TryGetKeyboardInput( HANDLE hStdIn, bool &bTerminate, char* buf )
{
	DWORD dwNumberOfEventsRead=0;
	INPUT_RECORD rec = {0};

	if (!::PeekConsoleInput(hStdIn, &rec, 1, &dwNumberOfEventsRead))
		return false;

	if (rec.EventType == KEY_EVENT)
	{
		if (rec.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE)
			bTerminate = true;
		else if (rec.Event.KeyEvent.wVirtualKeyCode > VK_HELP)
		{
			// End of file, usually Ctrl-Z
			if (false) //(!gets(buf))	
				bTerminate = true;
			else
				return true;
		}
	}

	::FlushConsoleInputBuffer(hStdIn);

	return false;
}

#endif

// WORKS PERFECTLY

#if 1

#include "stdafx.h"
#include "ReadDirectoryChanges.h"

#include <list>

LPCWSTR ExplainAction(DWORD dwAction);
bool TryGetKeyboardInput(HANDLE hStdIn, bool &bTerminate, char* buf);


#define BUFFER_SIZE 10
#define PRODUCER_SLEEP_TIME_MS 5000
#define CONSUMER_SLEEP_TIME_MS 5000

LONG Buffer[BUFFER_SIZE];
LONG LastItemProduced;
ULONG QueueSize, QueueStartOffset;
ULONG TotalItemsProduced, TotalItemsConsumed;
CONDITION_VARIABLE BufferNotEmpty, BufferNotFull;
CRITICAL_SECTION   BufferLock;
BOOL StopRequested = FALSE;

//CThreadSafeQueue<CStringW> m_pathsList;
std::list<CStringW> m_pathsList;

DWORD WINAPI ConsumerThreadProc(PVOID p)
{
	ULONG ConsumerId = (ULONG)(ULONG_PTR)p;
	while (true)
	{
		EnterCriticalSection(&BufferLock);
		QueueSize = m_pathsList.size();
		//		while (m_pathsList.size() /*QueueSize*/ == 0 && StopRequested == FALSE) { // Buffer is empty - sleep so producers can create items.
		while (QueueSize == 0 && StopRequested == FALSE) { // Buffer is empty - sleep so producers can create items.
			SleepConditionVariableCS(&BufferNotEmpty, &BufferLock, INFINITE);
			QueueSize = m_pathsList.size();
		}

		//		if (StopRequested == TRUE && m_pathsList.size() /*QueueSize*/ == 0) {
		if (StopRequested == TRUE && QueueSize == 0) {
			LeaveCriticalSection(&BufferLock);
			break;
		}
		// Consume the first available item.
		//LONG Item = Buffer[QueueStartOffset];
		CStringW path = m_pathsList.front();
		//QueueSize--;
		m_pathsList.pop_front();
		//QueueStartOffset++;
		//TotalItemsConsumed++;

		//if (QueueStartOffset == BUFFER_SIZE) {
		//	QueueStartOffset = 0;
		//}
		//printf("Consumer %u: item %2d, queue size %2u\r\n", ConsumerId, Item, QueueSize);
		printf("\r\nConsumer %u: item %ls, queue size %2u", ConsumerId, path.GetBuffer(), QueueSize);

		LeaveCriticalSection(&BufferLock);
		// If a producer is waiting, wake it.
//		WakeConditionVariable(&BufferNotFull);
		// Simulate processing of the item.
//		Sleep(rand() % CONSUMER_SLEEP_TIME_MS);
		Sleep(CONSUMER_SLEEP_TIME_MS);
	}
	printf("Consumer %u exiting\r\n", ConsumerId);
	return 0;
}

#include <map>
#include <set>
#include <string>


bool clearDirWinApi(std::wstring _path)
{
	WIN32_FIND_DATAW fileInfo;

	//removeLastSeparator(_path);
	int iLastSeparator = _path.find_last_of(L"\\");
	if(iLastSeparator == _path.size() - 1)
		_path = _path.substr(0, iLastSeparator);

	_path += L"\\";

	auto searchPattern = _path + L"*.*";
	HANDLE hFile = FindFirstFileW(searchPattern.c_str(), &fileInfo);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (fileInfo.cFileName[0] != L'.')
			{
				//if (fileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				//{
				//	std::wstring fullDirPath = _path + fileInfo.cFileName;
				//	getDirHierarchy(_path + fileInfo.cFileName, _hierarchy);
				//}
				//else
				//{
				std::wstring fullFilePath = _path + fileInfo.cFileName;
				::DeleteFileW(fullFilePath.c_str());
				//}
			}

		} while (FindNextFileW(hFile, &fileInfo) == TRUE);

		FindClose(hFile);
	} 
	else
	{
		return false;
	}

	return true;
}


//
// When the application starts, it immediately starts monitoring your home
// directory, including children, as well as C:\, not including children.
// The application exits when you hit Esc.
// You can add a directory to the monitoring list by typing the directory
// name and hitting Enter. Notifications will pause while you type.
//

//#if 0

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	bool clearDirWinApi(L"C:\\Users\\Public\\_Temp_4214BE1A_9275_435C_AE6C_0B4F8585532D");

	//
	// Consumer
	//
	InitializeConditionVariable(&BufferNotEmpty);
	InitializeConditionVariable(&BufferNotFull);

	InitializeCriticalSection(&BufferLock);

	DWORD id;
	HANDLE hConsumer1 = CreateThread(NULL, 0, ConsumerThreadProc, (PVOID)1, 0, &id);

	//puts("Press enter to stop...");
	//getchar();

	//
	// Producer 
	//
	const DWORD dwNotificationFlags =
		//		FILE_NOTIFY_CHANGE_CREATION
		//		;
		FILE_NOTIFY_CHANGE_LAST_WRITE
		| FILE_NOTIFY_CHANGE_CREATION
		| FILE_NOTIFY_CHANGE_FILE_NAME
		;

	// Create the monitor and add two directories.
	CReadDirectoryChanges changes;
	//changes.AddDirectory(_tgetenv(_T("USERPROFILE")), true, dwNotificationFlags);
	changes.AddDirectory(_T("C:\\Users\\Public\\_"), false, dwNotificationFlags);

	HANDLE hStdIn = ::GetStdHandle(STD_INPUT_HANDLE);
	const HANDLE handles[] = { hStdIn, changes.GetWaitHandle() };

	char buf[MAX_PATH];
	bool bTerminate = false;

	while (!bTerminate)
	{
		DWORD rc = ::WaitForMultipleObjectsEx(_countof(handles), handles, false, INFINITE, true);
		switch (rc)
		{
		case WAIT_OBJECT_0 + 0:
			// hStdIn was signaled. This can happen due to mouse input, focus change,
			// Shift keys, and more.  Delegate to TryGetKeyboardInput().
			// TryGetKeyboardInput sets bTerminate to true if the user hits Esc.
			if (TryGetKeyboardInput(hStdIn, bTerminate, buf))
				changes.AddDirectory(CStringW(buf), false, dwNotificationFlags);
			break;
		case WAIT_OBJECT_0 + 1:
			// We've received a notification in the queue.
		{
			DWORD dwAction;
			CStringW wstrFilename;
			if (changes.CheckOverflow())
				wprintf(L"Queue overflowed.\n");
			else
			{
				//EnterCriticalSection(&BufferLock);
				//m_pathsList = changes.GetContainer();
				//LeaveCriticalSection(&BufferLock);

				//changes.First(wstrFilename);
				changes.Pop(dwAction, wstrFilename);
				//wprintf(L"%s %s\n", ExplainAction(dwAction), wstrFilename);
				m_pathsList.push_back(wstrFilename);
				wprintf(L"%ls \n", wstrFilename.GetBuffer());

				// If a consumer is waiting, wake it.
				WakeConditionVariable(&BufferNotEmpty);
			}
		}
		break;
		case WAIT_IO_COMPLETION:
			// Nothing to do.
			break;
		}
	}

	// Just for sample purposes. The destructor will
	// call Terminate() automatically.
	changes.Terminate();
	//
	// Producer End
	//

	EnterCriticalSection(&BufferLock);
	StopRequested = TRUE;
	LeaveCriticalSection(&BufferLock);

	WakeAllConditionVariable(&BufferNotFull);
	WakeAllConditionVariable(&BufferNotEmpty);

	WaitForSingleObject(hConsumer1, INFINITE);
	//
	// Consumer End
	//

	return EXIT_SUCCESS;
}

//#endif

LPCWSTR ExplainAction(DWORD dwAction)
{
	switch (dwAction)
	{
	case FILE_ACTION_ADDED:
		return L"Added";
	case FILE_ACTION_REMOVED:
		return L"Deleted";
	case FILE_ACTION_MODIFIED:
		return L"Modified";
	case FILE_ACTION_RENAMED_OLD_NAME:
		return L"Renamed From";
	case FILE_ACTION_RENAMED_NEW_NAME:
		return L"Renamed To";
	default:
		return L"BAD DATA";
	}
}

bool TryGetKeyboardInput(HANDLE hStdIn, bool &bTerminate, char* buf)
{
	DWORD dwNumberOfEventsRead = 0;
	INPUT_RECORD rec = { 0 };

	if (!::PeekConsoleInput(hStdIn, &rec, 1, &dwNumberOfEventsRead))
		return false;

	if (rec.EventType == KEY_EVENT)
	{
		if (rec.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE)
			bTerminate = true;
		else if (rec.Event.KeyEvent.wVirtualKeyCode > VK_HELP)
		{
			// End of file, usually Ctrl-Z
			if (false) //(!gets(buf))	
				bTerminate = true;
			else
				return true;
		}
	}

	::FlushConsoleInputBuffer(hStdIn);

	return false;
}

#endif



