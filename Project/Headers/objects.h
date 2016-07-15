#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h> /* struct hostent, gethostbyname */

#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

//Parsed JSON object stores structs of strings that can
//be iterated over the elements array
struct parseObj{
    union {
        struct {
            char* message;
        };
        char** elements;
    };
};

////////////////////////////////////////////////////////////////////////////////////////
//                                      JSON Objects                                  //
////////////////////////////////////////////////////////////////////////////////////////

// JSON Types used when parsing the JSON
typedef enum{
    MESSAGE,
    ACCOUNT,
    TEMP
}TYPE;

//Message Object
struct message{
    char* text;
};

//Account Object
struct account{
    char* email;
    int myid;
    char* joinDate;
    char* lastLogin;
    char* username;
};

//Temp Object
struct tempList{
    int tempID;
    char* tempTime;
    char* tempVal;
    int userID;
    struct tempList *next;
};

////////////////////////////////////////////////////////////////////////////////////////
//                                      Util Objects                                  //
////////////////////////////////////////////////////////////////////////////////////////

//Linked list of strings
struct strItem{
    char* string;
    struct strItem *next;
};


////////////////////////////////////////////////////////////////////////////////////////
//                                         Functions                                  //
////////////////////////////////////////////////////////////////////////////////////////

//Gets a JSON
void getJSON(int, char *[], char*);

//Parses a json into a parseObj struct
void* parseJson(TYPE, char*, void*);
struct parseObj parseString(char *, int);

//Parses the parseObj into the object type
struct message* newMessage(struct parseObj);
struct account* newAccount(struct parseObj);
struct tempList* newTempList(struct parseObj);

//util functions
int StringToInt(char *);
struct strItem* splitString(char*, char);
char* ftStr(char*);
char* getMember(char *);


//Extra
void tempPage(struct tempList*);
#endif