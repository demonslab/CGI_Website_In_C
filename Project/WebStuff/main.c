#include "../Headers/objects.h"
#include "../Headers/cgiHeader.h"

//Function prototypes for attempting to log in and parsing a query string
//They are specific only to this file, so we didn't put them in a header file
struct account* doLogin(char *str, int numParams, char *path);
char** parseQuery(char *str, int numParams, char *path);

char* password;                         //Store password and account info globally
struct account *thisAccount;

/*
 * Main function for the project;
 *
 * Handles determining which html page to generate
 * Always start by attempting to login, and if so
 * we display the temperature table
 * 
 * We store the password and account info globally so if we 
 * wanted to add on to this program, such as an edit-account
 * page since the user is already logged in, they shouldn't
 * need to reenter their information on every page to do so.
 */
int main(int argc, char *argv[]){
    password = malloc(sizeof(char*));             //Malloc space for the password
    thisAccount = malloc(sizeof(struct account)); //Malloc space for the account information
    
    printf("%s%c%c\n", "Content-Type:text/html;charset=iso-8859-1",13,10);
    char* data = getenv("QUERY_STRING");              //Get the query string from the URL
    //char* data = "?user=demonslab&pass=password";   //used for local testing
    
    //Attempt to login with the given credentials
    thisAccount = doLogin(data, 2,"users/login");
    fflush(stdout);
    
    if (thisAccount != NULL){                     //If the login is successful
        //Get the temperature array using the credentials
        char* args[3] = {"temperature/get", thisAccount->username, password};
        char *json = malloc(sizeof(char*));
        getJSON(3, args, json);
        
        //printf("%s %s %s\n",args[0], args[1],args[2]);
        //printf("%s", json);
        
        struct tempList *temps = malloc(sizeof(struct tempList));
        temps = parseJson(TEMP, json, temps);
        tempPage(temps);
    }
    return 0;
}

/* 
 * Attempt to login to the webpage
 * If the login fails make the login page and return NULL
 * else return the account
 *
 * str: the query string
 * numParams: number of parameters in the string
 * path: path for getting the JSON
 */
struct account* doLogin(char *str, int numParams, char *path){
    //Get the parameters from the query string
    char **params = parseQuery(str, numParams, path);
    
    if (params != NULL){
        //check if all parameters are there
        int j;
        for (j=0;j<numParams+1;j++){
            if (strlen(params[j])==0){
                loginPage(1);
                return NULL;
            }
        }
        //else get the json
        char* json = malloc(sizeof(char*));
        getJSON(3, params, json);
    
        struct message *mess = malloc(sizeof(struct message));
        mess = (struct message*) parseJson(MESSAGE, json, mess);
        
        //Login successful, get the account from the api
        if (strcmp(mess->text, "success") == 0){
            password = params[2];           //Save the password
            params[0] = "users/account";
            json = malloc(sizeof(char*));
            getJSON(3, params, json);
            struct account *getAccount = malloc(sizeof(struct account));
            getAccount = parseJson(ACCOUNT, json, getAccount);
            
            return getAccount;
        }
        else loginPage(1);
    }
    else loginPage(0);
 
    return NULL;
}

/*
 * Parse the contents of the query string
 * returning a pointer to an array of the values
 *
 * This is used as the input array for getting a JSON
 */
char** parseQuery(char *str, int numParams, char *path){
    char *ptr, *ptr2, **params;
    
    ptr = malloc(strlen(str) + 1);  //Malloc enough for str + '\0'
    strcpy(ptr, str);               //Copy str to ptr
    ptr2 = ptr;                     //have ptr2 point to ptr
    *(ptr2+strlen(str)) = '\0';     //Set the null terminator '\0'
    
    //Make space for the params array
    params = malloc((numParams+1)*sizeof(char*));
    *(params) = path; //Set the first parameter to the path
    
    //i is boolean for param, j is parameter number, k is char number
    int i=1, j=0, k=0;
    while (j<numParams){        //don't get more parameters than expected
        if (i==1){              //next parameter found, look for value
            if (*ptr2 == '='){  //value found, add to params
                params[++j] = ++ptr2;
                i=0;            //wait for next parameter
                continue;
            }
        }
        else if (*ptr2=='&'){      //Parameters are seperated by '&'
            *ptr2 = '\0';
            i=1;
        }
        ptr2++;                    //Get next character
        if (++k >= strlen(str)){   //Reached end of string
            *ptr2='\0';
            break;
        }
    }
    //If there are fewer parameters than expected return NULL
    if (j<numParams)
        return NULL;
    return params;
}

