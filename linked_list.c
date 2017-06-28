#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked_list.h"

#define N 101

struct node_{
	Item site,id,password;
	node *next;
};

struct list_{
    node *head;
    int val;
};

static Key key(Item v){
    Key val;
    val = malloc(N * sizeof(Key));
    strcpy(val,v);
    return val;
}

node* newNode(Item site,Item id,Item password,node *next){
    node *x = malloc(sizeof(struct node_));
    if(x == NULL) 
    	return NULL;
    else{
        x->site = strdup(site);
        x->id = strdup(id);
        x->password = strdup(password);
        x->next = next;
    }
    return x;
}

node* SearchAndInsert(list *l,Item site,Item id,Item password){
    node *x,*p;
    Key s = key(site);
    Key i = key(id);
    Key pass = key(password);

    /* head insertion */
    if(l->head == NULL || strcmp(key(l->head->site),s) > 0)
        return l->head = newNode(s,i,pass,l->head);
    /* searching position */
    for (x = l->head->next, p = l->head; x != NULL && strcmp(key(x->site),s) < 0; p = x, x = x->next);
    p->next = newNode(s,i,pass,x);
    l->val++;
    return l->head;
}

static list *CreateList(){
    list *p = malloc(sizeof(list));

    if (p == NULL) {
        fprintf(stderr,"Memory Allocation Error.\n");
        exit(EXIT_FAILURE);
    }
    p->head = NULL;
    p->val = 0;
    return p;
}

list *LoadList(){
    list *p;

    p = CreateList();
    if(p == NULL) 
        exit(EXIT_FAILURE);
    
    return p;
}

void PrintList(list *l) {
    node *p;
    if (l == NULL) 
    	exit(EXIT_FAILURE);
    p = l->head;
    while (p != NULL) {
        printf("%25s%25s%25s\n", p->site, p->id, p->password);
        p = p->next;
    }
}

int SearchbySite (list *l, Key v) {
    int flag=0;
    node *x,*h;
    h = l->head;

    //for loop stops if strcmp > 0 since the list is ordered by site
    for (x=h;x!=NULL && strcmp(key(x->site),v) <= 0;x=x->next){
        //If match is found print the node
        if (strcmp(key(x->site),v) == 0){
            flag++;
            if (flag == 1)
                printf("%25s %25s %25s\n", "SITE","ID NAME","PASSWORD");
            printf("%25s%25s%25s\n", x->site, x->id, x->password);
            return 0;
        }
    }
    if (flag != 0){
        printf("[+] Match for %s Found!\n", v);
        return 0;
    }
    else{
        printf("[-] Match Not Found for %s!\n",v);
        return -1;
    }
}

int SearchbyId (list *l, Key v) {
    int flag=0;
    node *x,*h;
    h = l->head;

    for (x=h;x!=NULL;x=x->next){
        //If match is found print the node
        if (strcmp(key(x->id),v) == 0){
            flag++;
            if (flag == 1)
                printf("%25s %25s %25s\n", "SITE","ID NAME","PASSWORD");
            printf("%25s%25s%25s\n", x->site, x->id, x->password);
        }
    }
    if (flag != 0){
        printf("[+] Match for %s Found!\n", v);
        return 0;
    }
    else{
        printf("[-] Match Not Found for %s!\n",v);
        return -1;
    }

}

int VALget(list *l){
	return l->val;
}

list *SearchAndDelete (list *l, Key v) {
    int flag = 0;
    node *x,*p,*h;
    h = l->head;

    if (h==NULL) 
        return NULL;
    //for loop stops if strcmp > 0 since the list is ordered by site
    for (x=h, p=NULL; x!=NULL && (strcmp(key(x->site),v) <= 0);p=x,x=x->next) {
        //if match is found
        if (strcmp(key(x->site),v) == 0){
            flag = 1;
            //if the found node is in the head of the list
            if (x == h) 
                h = x->next;
            else 
                p->next = x->next;
            //delete node
            free(x->site);
            free(x->id);
            free(x->password);
            break;
        }
    }
    if (flag != 0){
        printf("[+] %s Deleted!\n", v);
        l->val = flag;
    }
    else{
        printf("[-] %s Cannot be Deleted, No Match Found!\n",v);
        l->val = flag;
    }
    return l;
}

void ClearList(list *l){
    node *x,*h;

    if (l->head == NULL)
        return;
    
    //For loop goes on until x==NULL
    for(x=l->head;x!=NULL;x=h){
        //Saves the next element
        h = x->next;
        //Deletes everything in the current node
        free(x->site);
        free(x->id);
        free(x->password);
    }
    //sets head to NULL
    l->head = NULL;
}

void DestroyList(list *l){
    ClearList(l);
    free(l->head);
}
