#include "../Headers/objects.h"

//////////////////////////////////////////////////////////////////////////////////////////
// This code is slightly modified from an answer to a question found on stack overflow.
//
//
// Here is the link:
//http://stackoverflow.com/questions/22077802/simple-c-example-of-doing-an-http-post-and-consuming-the-response
//////////////////////////////////////////////////////////////////////////////////////////

/*
 * Originally we tried to return the string, which worked on Debian on a 
 * virtual machine. However when I uploaded the program and compiled it
 * on my server it caused a segmentation fault. There were many instances 
 * of this other than just with this function, yet the work around was to
 * just assign the string to the string passed as a parameter, which we
 * allocate memory for before calling this function.
 *
 * In many locations we may have ended up mallocing space for a single string
 * multiple times, though mallocing was necessary so we could use the string
 * at later intervals in the program. The extra times are fail-safe thanks to
 * the discrepencies with my virtual machine and server.
 *
 * There was a lot of trial and error to making a successful connection
 * with this program.
 */

/*
 * argC is the number of elements in argV
 * argV contains <method> <username> <password>
 */
void getJSON(int argc, char *argv[], char *json){
    /* Fill out the following info from the servers
       Port Number
       Hostname
       Message Query, along with HTML Headers
     */
    int portno =        8080;
    char *host =        "avarizia.duckdns.org";
    
    //The following headers are necessary for the API to accept a valid connection
    // and took many tries to get it right. Each header is seperated by return
    // characters and newline characters and are necessary as well.
    // * /temp_cgi/ is the path of the api
    // * the string following it is the method being called
    // * afterwards are the parameters to that function seperated by '&',
    //   which in the methods of our API, the parameters are always
    //   the username and password of a user stored in our MySQL database.
    //   This is why the url contains "username=" and "password=", as they are
    //   the names of the parameters, followed by the given values
    // * HTTP type, Content-type, and Accept come next, defining the page contents
    // * Authorization: Basic is the username and password of the API itself in
    //   base64
    char *message_fmt = "POST /temp_cgi/%s?username=%s&password=%s HTTP/1.0\r\nContent-Type: application/x-www-form-urlencoded\r\nAccept: application/json\r\nAuthorization: Basic dGgzYzBPbGVzdFU1ZXJuNG1lOkwzR09NSUVHRzA=\r\n\n";
    
   // printf("%s, %s, %s\n",argv[1], argv[2], argv[3]);
    
    struct hostent *server;
    struct sockaddr_in serv_addr;
    int sockfd, bytes, sent, received, total;
    char message[1024],response[4096];
    
    if (argc < 3){
        json = NULL;
        return;
    }
    
    /* fill in the parameters */
    sprintf(message,message_fmt,argv[0],argv[1],argv[2]);
        
    /* create the socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){
        json = NULL;
        return;
    }
    
    /* lookup the ip address */
    server = gethostbyname(host);
    if (server == NULL){
        json = NULL;
        return;
    }
    
    
    /* fill in the structure */
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    memcpy(&serv_addr.sin_addr.s_addr,server->h_addr,server->h_length);
    
    /* connect the socket */
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0){
        json = NULL;
        return;
    }
    
    /* send the request */
    total = strlen(message);
    sent = 0;
    do {
        bytes = write(sockfd,message+sent,total-sent);
        if (bytes < 0){
            json = NULL;
            return;
        }
        if (bytes == 0) break;
        sent+=bytes;
    } while (sent < total);
    
    /* receive the response */
    memset(response,0,sizeof(response));
    total = sizeof(response)-1;
    received = 0;
    do {
        bytes = read(sockfd,response+received,total-received);
        if (bytes < 0) {
            json = NULL;
            return;
        }
        if (bytes == 0) break;
        received+=bytes;
    } while (received < total);
    
    if (received == total) {
        json = NULL;
        return;
    }
    
    /* close the socket */
    close(sockfd);
    
    /* Add a null string to the end of the response, and get the JSON from it */
    response[strlen(response)] = '\0';
    char *str = response;
    
    //the response has a bunch of header information that we don't want
    //which is always the same since it's server information, so we skip it.
    //
    //the last line before the JSON reads "Connection: close"
    //and str returns a pointer to the first instance, i.e. in this case
    //the pointer will return the 'c' in "close".
    //
    //initially used '{' for JSONs, but JSON arrays use '[' and
    //some header information uses '{', so we went with matching
    //the last String, which happened to be unique.
    char *result = strstr(response, "close");
    
    //9 characters are inbetween the 'c' in close and the JSON
    //this was determined through repeated testing, nothing more
    result+=9;

    
    if (result != NULL){
        char *copy = malloc(strlen(result)*sizeof(char));
        strcpy(json, result);
     //   printf("%s\n", json);
        //return strdup(result);
        //return copy;
    //    return copy;
    }
    
   // return NULL;
    

}