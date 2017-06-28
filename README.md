# password-saver-cli
Command line utility to store credentials securely (AES-256 encryption)

## Dependencies

[Openssl](https://www.openssl.org/) - General-purpose cryptography library

##### Installation on Linux:

Debian based:

     sudo apt-get install openssl
Arch based

     sudo pacman -S openssl

## Installation
      git clone 'https://github.com/Toory/password-saver-cli'
      cd password-saver-cli
   	  make
      ./password-saver
	  
## Usage

#### Main Menu:

	-------------------------------------
	-------ENCRYPTED PASSWORD SAVER------
	-------------------------------------
	1. Log in.---------------------------
	2. Sign Up.--------------------------
	3. Delete an account.----------------
	4. Exit------------------------------

1 - Log in a previous account

2 - Create a new accout

3 - Deletes a previously created account (requires log in)

4 - Exit.

#### User Menu:

	-------------------------------------
	              user		
	------------LOGGED IN!---------------

	1. See stored id/passwords.----------
	2. Search by site name.--------------
	3. Search by username.---------------
	4. Input new entries .---------------
	5. Input new entries from file-------
	6. Search and Delete an entry--------
	7. Delete all entries.---------------
	8. Back------------------------------
	9. Exit------------------------------

1 - Prints currently saved sites | username | password

2 - Search an entry by the site name 

3 - Search an entry by the username

4 - Manually add one entry

5 - Input entries from file.

	The text file must contain one entry per row as follows: 
	
	site username password
	
	example:
	
	google.com myemail@google.com hunter1
	
6 - Search an entry by site name and delete it

7 - Delete all entries

8 - Go back to the Main Menu

9 - Exit.


## Security

This software does not connect to the internet in any way, so potential attacks from the internet are to exclude 
(except for malware/viruses compromizing your machine at the moment of usage).

The encryption/decryption process it's done via openssl with the login password,
it utilizes AES-256 (Advanced Encryption Standard) which is a specification for the encryption 
of electronic data established by the U.S. National Institute of Standards and Technology (NIST).
