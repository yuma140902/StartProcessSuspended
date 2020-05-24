// StartProcessSuspended.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "framework.h"
#include <locale.h>
#include "StartProcessSuspended.h"

const TCHAR* version = TEXT("0.0.0");

void splash();
void showProcessInfo(PROCESS_INFORMATION pi);
void showLastErrorMessage(DWORD errorCode);
void printSubCommands();
BOOL killProcess(HANDLE hProcess);

int _tmain(int argc, _TCHAR* argv[]){
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	BOOL targetProcAlive = false;

	setlocale(LC_ALL, "Japanese");

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

	targetProcAlive = true;
	fwprintf(stderr, TEXT("\nStarted process successfully.\n\n"));
	showProcessInfo(pi);

	fwprintf(stderr, TEXT("Type ? to show help\n"));
	for(;;){
		TCHAR cmd[2];
		fwprintf(stderr, TEXT("> "));
		FlushFileBuffers(stderr);
		fwscanf_s(stdin, TEXT("%s"), cmd, _countof(cmd));
		if(cmd[0] == '?'){
			printSubCommands();
		}
		else if(cmd[0] == 'l'){
			CloseHandle(pi.hThread);
			CloseHandle(pi.hProcess);
			exit(0);
		}
		else if(cmd[0] == 'x'){
			BOOL shouldExit = true;
			if(targetProcAlive)
				shouldExit = killProcess(pi.hProcess);

			if(shouldExit){
				CloseHandle(pi.hThread);
				CloseHandle(pi.hProcess);
				exit(0);
			}
		}
		else if(cmd[0] == 'k'){
			if(killProcess(pi.hProcess))
				targetProcAlive = false;
		}
		else if(cmd[0] == 'r'){
			if(ResumeThread(pi.hThread) < 0)
				showLastErrorMessage(GetLastError());
		}
		else if(cmd[0] == 'i'){
			showProcessInfo(pi);
		}
		else{
			fwprintf(stderr, TEXT("No such command: %s"), cmd);
		}
	}
}

void splash(){
	fwprintf(stderr, TEXT("spsp - StartProcessSuspended\n"));
	fwprintf(stderr, TEXT("v%s\n"), version);
	fwprintf(stderr, TEXT("by yuma140902\n"));
	fwprintf(stderr, TEXT("\n"));
	fwprintf(stderr, TEXT("spsp starts a process suspended.\n"));
	fwprintf(stderr, TEXT("spsp is a member of S4 software series.\n"));
}

void showProcessInfo(PROCESS_INFORMATION pi){
	fwprintf(stderr, TEXT("PID: %d\n"), pi.dwProcessId);
	fwprintf(stderr, TEXT("ThreadID: %d\n"), pi.dwThreadId);
}

void showLastErrorMessage(DWORD errorCode){
	LPVOID buffer;
	DWORD result = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |	// テキストのメモリ割り当てを要求する
		FORMAT_MESSAGE_FROM_SYSTEM |			// エラーメッセージはWindowsが用意しているものを使用
		FORMAT_MESSAGE_IGNORE_INSERTS,		// 次の引数を無視してエラーコードに対するエラーメッセージを作成する, 
		NULL,
		errorCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<TCHAR*>(&buffer), 0, NULL
	);
	if(result > 0 && buffer != NULL){
		TCHAR* msg = reinterpret_cast<TCHAR*>(buffer);
		fwprintf(stderr, TEXT("%s"), msg);
		TCHAR c0 = msg[0];
		TCHAR c1 = msg[1];
		TCHAR c2 = msg[2];
		LocalFree(buffer);
	}
}

void printSubCommands(){
	fwprintf(stderr, TEXT("\nCommands:\n"));
	fwprintf(stderr, TEXT(" - l ... Exit spsp with the target process alive\n"));
	fwprintf(stderr, TEXT(" - x ... Kill the target process and exit spsp\n"));
	fwprintf(stderr, TEXT(" - k ... Kill the target process\n"));
	fwprintf(stderr, TEXT(" - r ... Resume the target process\n"));
	fwprintf(stderr, TEXT(" - i ... Print PID and ThreadID of the target process\n"));
	fwprintf(stderr, TEXT(" - ? ... Print this message\n"));
}

BOOL killProcess(HANDLE hProcess){
	BOOL b = TerminateProcess(hProcess, 0);
	if(!b){
		fwprintf(stderr, TEXT("Some error occurred during killing the target process:\n"));
		showLastErrorMessage(GetLastError());
	}
	else{
		fwprintf(stderr, TEXT("Successfully kill the target process.\n"));
	}
	return b;
}