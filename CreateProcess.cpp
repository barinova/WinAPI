#include <windows.h>
#include <iostream>
#include <process.h>
void commandLine()
{
	 STARTUPINFO siForCmd = {sizeof(siForCmd)};
     PROCESS_INFORMATION piForCmd;
     TCHAR czCommandLine[] = "tasklist /FI \"imagename eq notepad.exe\"";     
     std::cout << "notepad.exe: \n";
     if(!(CreateProcess(NULL, czCommandLine, NULL, NULL, FALSE, 0, NULL, NULL, &siForCmd, &piForCmd)))
    {
         std::cout << "Not found\n";
         system("PAUSE");
         exit(1);
    }
     Sleep(2000);//пауза 2 сек
	 PROCESS_INFORMATION piCurProc;
     std::cout << "\nID: ";
     std::cin >> piCurProc.dwProcessId;
	 HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, piCurProc.dwProcessId);
	 
	  if(piCurProc.hProcess)
     {
         std::cout << "Handle Notepad.exe: " << piCurProc.hProcess << std::endl;
		 CloseHandle(processHandle); 
	 }

	CloseHandle(piForCmd.hProcess);
	CloseHandle(piForCmd.hThread);
}

void createProcess()
{
	STARTUPINFO si = { sizeof(si) };
	SECURITY_ATTRIBUTES saProcess, saThread;
	PROCESS_INFORMATION piProcessB, piProcessC;

	TCHAR szPath[MAX_PATH];
	saProcess.nLength = sizeof(saProcess);
	saProcess.lpSecurityDescriptor = NULL;
	saProcess.bInheritHandle = TRUE;
	saThread.nLength = sizeof(saThread);
	saThread.lpSecurityDescriptor = NULL;
	saThread.bInheritHandle = FALSE;
	lstrcpy(szPath, TEXT("C:\\WINDOWS\\system32\\notepad.exe"));
	CreateProcess(NULL, szPath, &saProcess, &saThread,
	FALSE, 0, NULL, NULL, &si, &piProcessB);
	commandLine();
	lstrcpy(szPath, TEXT("C:\\WINDOWS\\system32\\notepad.exe"));
	CreateProcess(NULL, szPath, NULL, NULL,
	TRUE, 0, NULL, NULL, &si, &piProcessC);
	commandLine();
	CloseHandle(piProcessB.hProcess);
	CloseHandle(piProcessC.hProcess);
	CloseHandle(piProcessB.hThread);
	CloseHandle(piProcessC.hThread);
}

unsigned __stdcall tmpFunc( void * arg) // Функция потока
{
	char ** str = (char**)arg;
	std::cout << str[0] << " " << str[1] << std::endl;
	_endthreadex( 0 );
	return 0;
};

void createThreads()
{
	char * InitStr1[2] = {"First thread running!","11111"};
	char * InitStr2[2] = {"Second thread running!","22222"};
	unsigned uThreadIDs[2];

	HANDLE hThreads[2];
	hThreads[0] = (HANDLE)_beginthreadex( NULL, 0, &tmpFunc, InitStr1, 0,&uThreadIDs[0]);
	hThreads[1] = (HANDLE)_beginthreadex( NULL, 0, &tmpFunc, InitStr2, 0,&uThreadIDs[1]);

	WaitForMultipleObjects(2, hThreads, TRUE, INFINITE ); 
	CloseHandle( hThreads[0] );
	CloseHandle( hThreads[1] );
}

int main(int argc, char* argv[])
{
	createThreads();
	return 0;
}