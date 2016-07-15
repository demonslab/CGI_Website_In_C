#include "../Headers/objects.h"
#include "../Headers/cgiHeader.h"

/*
 * This file generates the actual html pages using the functions from head.c
 * There are two pages, the login page and the temperature page
 * 
 * Originally I made the pages with plain text, so then I used those
 * as the template for the c program. These pages don't have variety other
 * than the what is being passed as a parameter
 */


/*
 * The login page. 
 * If error evaluates to true, show the user that the login failed
 */
void loginPage(int error){
    //Start of the page
    printHeader();
    printNavbar();
    startBody();
    divClass("centered");
    printBanner();
    
    //begin sign in display
    printf("<h1>Please sign in below to view the temperature recordings.</h1>\n");
    divClass("frame"); divClass("inFrame");
    
    if (error) //If Login Failed
        printf("<font color=\"#990000\">There was an error, try again.</font>");
    
    printf("<form action=\"\" method=\"get\" onSubmit=\"window.location.reload()\">\n" );
    input("text", "user", "Username");
    br();
    input("password" ,"pass", "Password");
    br();br();

    printf ("<input type=\"submit\" value=\"Sign In!\" class=\"btn\">\n");
    button("./temp-cgi/login.cgi", "float-right", "Sign Up!");
    printf("</form>\n");
    
    //End login form and fill out the rest of the page
    endDiv(); endDiv();
    endDiv();
    endBody();
    printFooter();
}



/*
 * The temperature page, takes a linked list of structs as input
 * Should be generated from the JSON
 */
void tempPage(struct tempList *temps){    //take temp list as parameter
    //Set up the html file for the table
    printHeader();
    printNavbar();
    printAccountNav();
    startBody();
    divClass("centered");
    divClass("content");
    
    //start table
    printf("<table><caption>");
    printf("Recorded temperature measurements for the past 24 hours.");
    printf("</caption>\n");
    
    //Insert the header row of the table
    printf("<tr>\n");
    printf("<th scope=\"col\">Time Recorded</th>\n");
    printf("<th scope=\"col\">Temperature Recorded</th>\n");
    printf("</tr>\n");
    
    //Fill the rows of the table with the temperature val
    while (temps!=NULL){
        tableRow(temps->tempTime, temps->tempVal);
        temps = temps -> next;
    }
    
    //close the table and the rest of the page
    printf("</table>\n");
    endDiv();
    endDiv();
    endBody();
    printFooter();
}


