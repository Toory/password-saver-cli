#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "aes.h"
#include "linked_list.h"
#include "password_saver.h"

#define N 100

struct user_{
	char *username,*password;  

};

void AddEntryToVault(user *p,list *l,char *filename){
	FILE *fout;
	char site[N],id[N],password[N];

	if((fout = fopen(filename,"a")) == NULL){
		printf("%s not found.\n",filename);
		exit(-1);
	}

	printf("Enter site name: \n");
	printf("~ ");
	scanf("%s", site);

	printf("Enter user name: \n");
	printf("~ ");
	scanf("%s", id);

	printf("Enter password: \n");
	printf("~ ");
	strcpy(password,getpass(""));

	SearchAndInsert(l,site,id,password);

	Encrypt(site,p->password);
	Encrypt(id,p->password);
	Encrypt(password,p->password);

	fprintf(fout, "%s %s %s \n",site,id,password);
	fflush(fout);
}

void Show(list *l){
	printf("%25s %25s %25s\n", "SITE","ID NAME","PASSWORD");
	PrintList(l);
}

void LoadVaultFromFile(list *metadata,user *p,char *filename){
	FILE *fp;
	char *site,*id,*password;

	site = malloc(N * sizeof(char));
	id = malloc(N * sizeof(char));
	password = malloc(N * sizeof(char));

    if((fp = fopen(filename,"r")) == NULL){
        printf("[!] Saved password file not found, creating one...\n");
        if((fp = fopen(filename,"w")) == NULL){
        	fprintf(stderr,"Error creating %s, exiting...\n",filename);
        	exit(0);
        }
        return;
    }

    //Read site,id,password from file until EOF is reached
    while(fscanf(fp,"%s %s %s \n",site,id,password) != EOF){

		Decrypt(site,p->password);
		Decrypt(id,p->password);
		Decrypt(password,p->password);

		SearchAndInsert(metadata,site,id,password);
    	printf("[+] %s Loaded\n", site);
	}
	fclose(fp);
}

list *LoadEntriesFromFile(list *l,char *filename,user *p,char *session){
	FILE *fin,*fout;
	char site[N],id[N],password[N];

	if((fin = fopen(filename,"r")) == NULL){ 
		fprintf(stderr,"Input File not found.\n");
		return l;
	}

	if((fout = fopen(session,"a")) == NULL){ 
		fprintf(stderr,"File not found, generating a new one...\n");
	}

	while(fscanf(fin,"%s %s %s",site,id,password) != EOF){
		SearchAndInsert(l,site,id,password);
		printf("[+] %s Loaded\n", site);

		Encrypt(site,p->password);
		Encrypt(id,p->password);
		Encrypt(password,p->password);

		fprintf(fout, "%s %s %s \n",site,id,password);
	}

	printf("[+] All Entries were added from %s!\n",filename);
	fclose(fin);
	fclose(fout);
	return l;
}

void DeleteEntry(user *p,list *l,char *fileIn, char *Todelete){
	int ret,x;
	char site[N],id[N],password[N],fileOut[N];

	x = SearchbySite(l,Todelete);
	if(x == -1)
		return;

	l = SearchAndDelete(l,Todelete);
	if(VALget(l) == 0){
		fprintf(stderr, "listDeleteOrd Error\n");
		return;
	}

	sprintf(fileOut,".%s002.dat",p->username);
	FILE *fin = fopen(fileIn,"r");
	FILE *fout = fopen(fileOut,"w");

	if(fin == NULL || fout == NULL){ 
		printf("File not found.\n");
		exit(-1);
	}


	Encrypt(Todelete,p->password);

	//Read all the entries from FileIn
	while(fscanf(fin,"%s %s %s",site,id,password) != EOF){
		//Write in FileOut all the Entries except the one to delete 
		if(strcmp(Todelete,site) != 0)
			fprintf(fout,"%s %s %s \n",site,id,password);
	}

	//Delete the old vault file
	ret = remove(fileIn);
	if(ret != 0) 
		fprintf(stderr,"[-] Error: could not remove %s\n",fileIn);
	/*
	  Rename the fileOut with fileIn 
	  This is needed because certain fuctions use an arbitrary filename
	  to access the vault file : $user_Vault.dat
	*/
	ret = rename(fileOut,fileIn);
	if(ret != 0){
		fprintf(stderr, "[-] Error could not rename %s.\n",fileOut);
		exit(-1);
	} 

	fclose(fin);
	fclose(fout);
}

void DeleteVaultFile(char *VaultFile){
	int flag = remove(VaultFile);

	if(flag == 0)
		printf("[+] Vault File deleted successfully!\n");
	else
		fprintf(stderr,"[-] Error: unable to delete %s\n",VaultFile);
}

void DeleteSession(user *p){
	int flag;
	char vault[N],session[N];

	sprintf(vault,".%s_Vault.dat",p->username);
	flag = remove(vault);
	if(flag == 0)
		printf("[+] Vault File deleted successfully!\n");
	else
		fprintf(stderr,"[-] Error: unable to delete %s\n",vault);
	
	sprintf(session,".%s_session.dat",p->username);
	flag = remove(session);
	if(flag == 0) 
		printf("[+] Session File deleted successfully!\n");
	else
		fprintf(stderr,"[-] Error: unable to delete %s\n",session);
}

static user *NewSession(){
	FILE *fout;
	char id[N],password[N],session[N];
	user *p = malloc(sizeof(user));

	printf("Enter new user name: \n");
	printf("~ ");
	scanf("%s", id);
	
	sprintf(session,".%s_session.dat",id);
	if((fout = fopen(session,"w")) == NULL){
		printf("File not found.\n");
		exit(-1);
	}

	printf("Enter password: \n");
	printf("~ ");
	strcpy(password,getpass(""));
	p->username = strdup(id);
	p->password = strdup(password);
	
	Encrypt(id,p->password);
	Encrypt(password,p->password);

	fprintf(fout, "%s\n%s\n", id,password);
	fclose(fout);
	return p;
}

static user* LoginSession(){
	FILE *fin;
	char id[N],password[N],encrypted_id[N],encrypted_pass[N],session[N],choose;
	user *p = malloc(sizeof(user));

	printf("Enter user name: \n");
	printf("~ ");
	scanf("%s", id);

	sprintf(session,".%s_session.dat",id);
	if ((fin = fopen(session, "r")) == NULL) {
		printf("Old session not found. \nDo you want to create a new session? (y/n):\n");
		printf("~ ");
		scanf(" %c",&choose);
		if(choose == 'y'){
			p = NewSession();
			return p;
		}
		//if not 'y', exit
		else
			exit(-1);
	}

	printf("Enter password: \n");
	printf("~ ");
	strcpy(password,getpass(""));

	//Retrieve id and password from the previous encrypted file 
	fscanf(fin,"%s %s",encrypted_id, encrypted_pass);

	Decrypt(encrypted_id,password);
	Decrypt(encrypted_pass,password);

	//Compare user input to previous id e password, if match login, else exit
	if(strcmp(encrypted_pass,password)!=0 || strcmp(encrypted_id,id)!=0){
		printf("Wrong username or password.\n");
		exit(-1);
	}

	//Save id,password in plaintext in a struct
	p->password = strdup(password);
	p->username = strdup(id);

	fclose(fin);
	return p;
}

static void Session(user *p){
	int num,choose=-1,flag;
	char *str,*input;
	list *metadata;
	str = malloc(N * sizeof(char));
	input=malloc(N*sizeof(char));
	//Load the linked list
	metadata = LoadList();
	sprintf(str,".%s_Vault.dat",p->username);
	/*Get data from previous File and 
	  Insert it into an Ordered Linked List */
	LoadVaultFromFile(metadata,p,str);

	do{
		printf("-------------------------------------\n");
		printf("\t\t%s\t\t\n",p->username);
		printf("------------LOGGED IN!---------------\n");
		printf("\n1. See stored id/passwords.----------\n");
		printf("2. Search by site name.--------------\n");
		printf("3. Search by username.---------------\n");
		printf("4. Input new entries .---------------\n");
		printf("5. Input new entries from file-------\n");
		printf("6. Search and Delete an entry--------\n");
		printf("7. Delete all entries.---------------\n");
		printf("8. Back------------------------------\n");
		printf("9. Exit------------------------------\n");
		printf("~ ");
		flag = scanf("%100d", &num);
		//if num is not a number, exit
		if(flag==0) {
			printf("Invalid Input.\n");
			exit(-1);
		}

		switch(num){
			case 1:{
				//Print the Linked List
				Show(metadata);
				break;
			}
			case 2:{
				input = strdup("");
				printf("Input site name: [0 to go back]\n");
				printf("~ ");
				scanf("%s",input);
				if(strcmp(input,"0") != 0) 
					SearchbySite(metadata,input);
				break;
			}
			case 3:{
				input = strdup("");
				printf("Input name id: [0 to go back]\n");
				printf("~ ");
				scanf("%s",input);
				if(strcmp(input,"0") != 0) 
					SearchbyId(metadata,input);
				break;
			}
			case 4:{
				AddEntryToVault(p,metadata,str);
				break;
			}
			case 5:{
				input = strdup("");
				printf("Text file must contain one entry per row as follows: \nsite username password\n");
				printf("Input file name: [0 to go back]\n");
				printf("~ ");
				scanf("%s",input);
				if(strcmp(input,"0") != 0) 
					metadata = LoadEntriesFromFile(metadata,input,p,str);
				break;
			}
			case 6:{
				input = strdup("");
				printf("Input site name: [0 to go back]\n");
				printf("~ ");
				scanf("%100s",input);
				if(strcmp(input,"0") != 0) 
					DeleteEntry(p,metadata,str,input);
				break;
			}
			case 7:{
				ClearList(metadata);
				DeleteVaultFile(str);
				break;
			}
			case 8:{
				choose = 0;
				DestroyList(metadata);
				break;
			}
			case 9:{
				DestroyList(metadata);
				free(str);
				free(input);
				free(p->username);
				free(p->password);
				exit(0);
			}
			default:{
				printf("Choose a listed option, please! [0 to go back]\n");
				choose = -1;
				break;
			}
		}
	}while(choose!=0);

	free(str);
	free(input);
	free(p->username);
	free(p->password);
}

int main(void){
	int choose=-1,num,flag;
	user *p;

	do{
		printf("\n");
		printf("-------------------------------------\n");
		printf("-------ENCRYPTED PASSWORD SAVER------\n");
		printf("-------------------------------------\n");
		printf("1. Log in.---------------------------\n");
		printf("2. Sign Up.--------------------------\n");
		printf("3. Delete an account.----------------\n");
		printf("4. Exit------------------------------\n");
		printf("~ ");
		flag = scanf("%100d", &num);
		//if num is not a number, exit
		if(flag==0) {
			printf("Invalid Input.\n");
			exit(-1);
		}
		
		switch(num){
			case 1:{
				p = LoginSession();
				printf("-------------------------------------\n");
				printf("--------Login Successful!------------\n");
				printf("-------------------------------------\n");
				Session(p);
				break;
			}
			case 2:{
				p = NewSession();
				Session(p);
				break;
			}
			case 3:{
				printf("Login is needed to delete the session.\n");
				p = LoginSession();
				printf("-------------------------------------\n");
				printf("--------Login Successful!------------\n");
				printf("-------------------------------------\n");
				DeleteSession(p);
				break;
			}
			case 4:{
				choose = 0;
				break;
			}
			default:{
				printf("Choose a listed option, please.\n");
				choose = -1;
				break;
			}
		}
	}while(choose != 0);

	return 0;
}
