#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

//Ordered by order they should be used, with some filler if necessary ie, start <div>
void printHeader();
void printNavbar();
void printAccountNav();
void startBody();
void printBanner();
void endBody();
void printFooter();


//Other functions
void divClass(char* str);
void endDiv();

void input(char*, char*, char*);
void button(char*, char*, char*);

void br();
void tableRow(char*, char*);



//Pages
void loginPage(int);
#endif