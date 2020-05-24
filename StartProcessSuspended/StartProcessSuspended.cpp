// StartProcessSuspended.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "framework.h"
#include "StartProcessSuspended.h"

const TCHAR* version = TEXT("0.0.0");

void splash();

int _tmain(int argc, _TCHAR* argv[]){
	PROCESS_INFORMATION pi;
	STARTUPINFO si;

	splash();

	if(argc < 2){
		fwprintf(stderr, TEXT("Usage:\n"));
		fwprintf(stderr, TEXT("%s <program.exe>\n"), argv[0]);
		return 1;		
	}

	memset(&pi, 0, sizeof(pi));
	memset(&si, 0, sizeof(si));
	si.cb = sizeof(STARTUPINFO);

	BOOL r = CreateProcess(
		NULL, argv[1], NULL, NULL, FALSE,
		NORMAL_PRIORITY_CLASS | CREATE_SUSPENDED,
		NULL, NULL, &si, &pi
	);
	if(!r) return -1;

	fwprintf(stderr, TEXT("\nStarted process successfully.\n\n"));
	fwprintf(stderr, TEXT("PID: %d\n"), pi.dwProcessId);
	fwprintf(stderr, TEXT("ThreadID: %d\n"), pi.dwThreadId);
}

void splash(){
	fwprintf(stderr, TEXT("spsp - StartProcessSuspended\n"));
	fwprintf(stderr, TEXT("v%s\n"), version);
	fwprintf(stderr, TEXT("by yuma140902\n"));
	fwprintf(stderr, TEXT("\n"));
	fwprintf(stderr, TEXT("spsp starts a process suspended.\n"));
	fwprintf(stderr, TEXT("spsp is a member of S4 software series.\n"));
}