#include "../Headers/objects.h"

/*
 * This file contains functions that are used repeated to edit strings,
 * moreover for taking a json and getting it's elements into a list of strings
 *
 * We use linked lists so we don't have to know the number of elements
 * and it makes sense to parse a JSON in the order of its elements
 *
 * In every function we copy the string and allocate space for it. This
 * is necessary as we only want to use enough space for the new string
 * unfortunately we never free the original space after we copy it,
 * which is bad coding practice.
 */

//Converts a String (char*) to an integer
int StringToInt(char* str){
    char *ptr = str;
    int num = 0;
    while(*ptr != '\0'){
        num = (10 * num) + (*(ptr)++ -48);
    }
    return num;
}

//Split a String at the given delimeter
//Returns a linked list of strings
//used mostly for parsing JSONs, JSON arrays
struct strItem* splitString(char* str, char delim){
    char *ptr = malloc(strlen(str)+1);
    struct strItem *head = malloc(sizeof(struct strItem));
    struct strItem *cur = head;
    
    //j is the iterator, i determines if the next string is found
    // and x determines if we are in a JSON or not
    int i=0,j, x=0;
    //Should end with '\0' already, so we don't need to change it
    int len = strlen(str)-1;
    strcpy(ptr, str);

    //Don't split the elements in a JSON if in a JSON array
    //since we want a list of JSONs to parse later
    for (j=0;j<len;j++){
        if (*ptr=='{') x++; //Like parenthesis, '{' can be nested, so we increment
        if (*ptr=='}') x--; //end of JSON, decrement
        
        //reseting i to 0 means we just ended a string
        //and we want to start the next string immediately after
        if((++i)==1){
            cur->string = ptr;  //Set the string
            cur->next = NULL;
        }
        //If we've found a delimeter and we aren't in a JSON
        if(*ptr == delim && x==0){
            *ptr = '\0';            //End the string
            cur->next = malloc(sizeof(struct strItem));
            cur = cur->next;        //Create space for the next string
            i=0;
        }
        ptr++;
    }
    return head;    //Return the head of the list
}

//return new string with first and last characters removed
//removing brackets from JSON or JSON array
char* ftStr(char* str){
    int length = strlen(str)-2;
    char *ptr = malloc(length*sizeof(char));

    strncpy(ptr, str+1, length);
    *(ptr+length) = '\0';
    return ptr;
}

// Get object member for json
//    ie.  var:member
// numbers don't have quotes, so return the whole string
char* getMember(char *str){
    char *ptr;
    int i=1;
    ptr = str;
    while(*(ptr++)!=':') i++;   //Find the start of the member string
    ptr = malloc(strlen(ptr));  //Make space for a copy of the string
    strcpy(ptr, str+i);
    return ptr;
}