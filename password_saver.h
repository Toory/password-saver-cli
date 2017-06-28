#ifndef PASSWORD_KEEPER_H
#define PASSWORD_KEEPER_H

typedef struct user_ user;

/*Adds from user input one entry into the Vault*/
void AddEntryToVault(user*,list*,char *);

/*Print the Linked List*/
void Show(list*);

/*Decrypt a previous Vault file 
  and Load the entries in the Linked List*/
void LoadVaultFromFile(list *,user*,char*);

/*Load the Vault from a plaintext file which cointains for each row:
  site username password */
list *LoadEntriesFromFile(list*,char*,user*,char*);

/*Search and Deletes an entry both from the
  Linked List and the Vault file
*/
void DeleteEntry(user *,list *,char*,char*);

/*Delete the Vault file*/
void DeleteVaultFile(char *);

/*Delete both the Session and Vault file*/
void DeleteSession(user *p);

#endif
