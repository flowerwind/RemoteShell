#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "baseFunction.h"
void trim(char *str) {
	int len;
	len = strlen(str);
	for (int i = 0; i < len; i++) {
		if (str[i] == ' ' || str[i]=='\n') {
			str[i] = '\0';
		}
	}

}