#include "../Headers/cgiHeader.h"

/* This page consists of functions to help fill in the html
 * Html looks tough to read sometimes, so the fact of mixing it 
 * with C just makes it horrendous. Thus I put all of these functions
 * in this file so that it doesn't have to be seen with the main code
 *
 * In order to generate html in C, it's just print statements,
 * where the page has to be in the correct format. There are probably
 * libraries out there that exist for this stuff, but I wanted to use
 * pure C, and not need to worry about any additional library files.
 *
 * The newline characters don't do anything special,
 * they only make the plain text of the html file look nice
 */

//Easy access to the header information
const char* title = "Avarizia";     //title of the page
const char* rootPath = "/";         //the root directory of the page
const char* cssPath = "/css/";      //path to the directory of css files

//CSS files to import
const int numCSS = 4;
const char* sharedCSS[4] =
        { "reset.css", "outline.css", "editorLayout.css", "center_form.css" };

//Navigational items for the <ul> tag
//i.e. homepage, temp_page, ... items at the top of the page
const int numNav = 5;
const char* navItems[5][2] = {
        {"./index.html","Home"}, {"./web/index.html","Plex"},
        {"./temp-cgi/login.cgi", "Temperature"},
        {"#", "About"}, {"#","Contact"}
    };


//Print the headers of the html page
void printHeader(){
    printf("<!DOCTYPE html><html><head>");
    printf("<title>%s</title>\n",title);
    printf("<meta http-equiv=\"Content-Type\" content=\"text/html; charset=iso-8859-1\" >\n");
    int i;
    for (i=0;i<numCSS;i++){
        printf("<link rel=\"stylesheet\" type=\"text/css\" href=\"%s%s\">", cssPath, sharedCSS[i]);
    }
    printf("<base href=%s>\n",rootPath);
    printf("</head>\n");
}

//Print Navigation bar
void printNavbar(){
    printf("<ul class=\"nav\">");
    int i,j;
    for (i=0;i<numNav;i++){
        printf("<li><a href=\"%s\">%s</a></li>\n", navItems[i][0], navItems[i][1]);
    }
    printf("<li style=\"float:right\"><t>%s</t></li></ul>\n",title);
}

//print the account/sign out bar
void printAccountNav(){
    printf("<div class=\"sul\">\n");
    printf("<li><a href=\"./temp-cgi/login.cgi\">Sign Out</a></li>\n");
    printf("<li><a href=\"./temp-cgi/login.cgi\">Account</a></li>\n");
    printf("</div>\n");
}

//Open html <body> tag
void startBody(){
    printf("<body>\n");
}

//Close html <body> tag
void endBody(){
    printf("</body>\n");
}


//Print the banner
void printBanner(){
    printf("<hr width=\"60%%\">");
    printf("<h2>Welcome to %s!</h2>",title);
    printf("<hr width=\"60%%\">\n");
}


//Print the footer and close html tag
void printFooter(){
    char *footer = "Copyright (c) 2016 - Avarizia - Adam Bucknell";
    printf("<footer><div id=\"footer\">%s</div></footer>\n", footer);
    printf("</html>");
}

//Open a <div> tag with the inputted class
void divClass(char* str){
    printf("<div class=\"%s\">\n", str);
}

//Close a <div> tag
void endDiv(){
    printf("</div>\n");
}

//input tag to create
//type: text, password, email
//name: what should the html refer to it as
//placeholder: text to show in it when it's empty
void input(char* type, char* name, char* phold){
    printf("<input type=\"%s\" name=\"%s\" placeholder=\"%s\">\n", type, name, phold);
}

//line break tag <br>
void br(){
    printf("<br>");
}

//add a button
//href: link
//style: any css style code applicable
//Button text
void button(char* href, char* style, char* text){
    printf("<a href=\"%s\" class=\"btn\" style=\"%s\">%s</a>\n", href, style, text);
}

//Add a row to the table
//the table elemnt
void tableRow(char* time, char* temp){
    printf("<tr>\n");
    printf("<td>%s</td>", time);
    printf("<td>%s&deg; F</td>", temp);
    printf("</tr>\n");
}
