#include "../Headers/objects.h"

/*
 * This file handles parsing a JSON and converting it
 * into an object.
 *
 * We parse each JSON with an object-oriented approach
 * using the following methods, as well as the methods
 * from 'util.c'
 */

//Parse any given JSON object that is defined
// in the ENUM of "objects.h"
// *v is where the JSON will be stored, created using
// an uncast malloc. The return type must be cast
// in order to use the returned struct object
void* parseJson(TYPE type, char* JSON, void *v){
    switch (type){
        case MESSAGE: //Message
            v = newMessage(parseString(JSON,1));
            break;
            
        case ACCOUNT: //Account
            v = newAccount(parseString(JSON,5));
            break;
            
        case TEMP:{ //Temperature list
            struct strItem *head = malloc(sizeof(struct strItem));
            struct tempList *l=v, *q=NULL;
            head = splitString(ftStr(JSON),',');
            struct strItem *tmp = head, *cur=tmp;
            
            //If the list is empty
            if (strcmp(JSON, "[]")==0)
                return NULL;
            
            //Iterate through the list
            //p is the iterator, keeping track of the current element
            //q keeps track of the previous element
            int i=0;
            while (tmp!=NULL){
                l = newTempList(parseString(tmp->string,4));
                tmp = tmp -> next;
                
                //for some reason mallocing each when on the server
                // seperates each element in the linked list, but
                // it's necessary since we to make space for each struct
                // So this part connects them
                if (q!=NULL) {
                    q->next = l;
                    q=q->next;
                }
                if (i++==0){    //Sets the head of the list
                    v=l;
                    q=l;
                }
                //allocate space for the next element
                l->next = malloc(sizeof(struct tempList));
                l = l->next;
            }
            l=NULL;     //End the list with null elements
            q->next=NULL;
            break;
        }
        default:
            v = NULL;
            break;
    }
    return v;
}


//Takes a full json and parses it into a parseObj
// objSize is the number of parameters in the JSON object
struct parseObj parseString(char *str, int objSize){
    struct parseObj parse;
    struct strItem *tmp,*head = malloc(sizeof(struct strItem));
    //Remove the brackets and Split the JSON at each element
    head = splitString(ftStr(str),',');
    tmp = head;
    parse.elements = malloc(objSize*sizeof(char*));
    int i;
    char **ptr = parse.elements;
    //Add to the list and make space for the next item
    while (tmp!=NULL){
        *ptr = getMember(tmp->string);
        tmp=tmp->next;
        ptr++;
    }
    return parse;
}


// The following functions assign each string in the parsObj
// to the corresponding variable in the object itself

//Returns a new message object
struct message* newMessage(struct parseObj messObj){
    struct message *thisMessage = malloc(sizeof(struct message));
    thisMessage->text = ftStr(*(messObj.elements));
    return thisMessage;
}


//Returns a new account object
struct account* newAccount(struct parseObj accObj){
    struct account* thisAccount = malloc(sizeof(struct account));
    char **ptr = accObj.elements;
    thisAccount->email = ftStr(*(ptr++));
    thisAccount->myid = StringToInt(*(ptr++));
    thisAccount->joinDate = ftStr(*(ptr++));
    thisAccount->lastLogin = ftStr(*(ptr++));
    thisAccount->username = ftStr(*ptr);
    return thisAccount;
}

///Returns a new Temperature list object
struct tempList* newTempList(struct parseObj tempObj){
    struct tempList *thisTempList = malloc(sizeof(struct tempList));
    char **ptr = tempObj.elements;
    thisTempList->tempID = StringToInt(*(ptr++));
    thisTempList->tempTime = ftStr(*(ptr++));
    
    //Float value, but couldn't get a working conversion function.
    //In a perfect object-oriented world, we would store it as a
    //float, rather than using the string
    thisTempList->tempVal = *(ptr++);
    thisTempList->userID = StringToInt(*ptr);
    
    return thisTempList;
}

