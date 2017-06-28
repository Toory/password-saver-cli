#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aes.h"

#define N 101

void Encrypt(char *entry,char *password){
	char cmd[N];
	FILE *commd;

	sprintf(cmd,"echo \"%s\" | openssl enc -aes-256-cbc -a -nosalt -k %s",entry,password);
	commd = popen(cmd, "r");
	fscanf(commd,"%s",entry);

	pclose(commd);
}

void Decrypt(char *entry,char *password){
	char cmd[N];
	FILE *commd;

	sprintf(cmd,"echo \"%s\" | openssl enc -d -aes-256-cbc -a -nosalt -k %s",entry,password);
	commd = popen(cmd, "r");
	fscanf(commd,"%s",entry);
	
	pclose(commd);
}
