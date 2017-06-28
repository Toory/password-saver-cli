#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef char* Item;
typedef char* Key;
typedef struct node_ node;
typedef struct list_ list;

//Create a new node
node* newNode(Item,Item,Item,node *);

//Search and Insert an entry , O(N)
node* SearchAndInsert(list *,Item,Item,Item);

//Create a new Linked List and initialize it
list *LoadList();

//Print the Linked List
void PrintList(list *);

//Search a node by the Item 'site'
int SearchbySite (list*,Key); 

//Search a node by the Item 'id'
int SearchbyId (list*,Key);

//Seach a node by the Item 'site' and delete it , O(N)
list *SearchAndDelete (list*,Key);

//Delete all nodes inside the Linked List
void ClearList(list*);

//Delete the Linked List
void DestroyList(list *);

//Get value of l->val
int VALget(list*);

#endif
