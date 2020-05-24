// StartProcessSuspended.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "framework.h"
#include "StartProcessSuspended.h"

int _tmain(int argc, _TCHAR* argv[]){
	PROCESS_INFORMATION pi;
	STARTUPINFO si;

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
}