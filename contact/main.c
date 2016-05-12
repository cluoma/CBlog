//
//  main.c
//  CBlog
//
//  Created by Colin Luoma on 2016-05-05.
//  Copyright (c) 2016 Colin Luoma. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Return maximum value of a or b
int max(int a, int b) {
    if (a > b)
        return a;
    else if (a < b)
        return b;
    else
        return a;
}

// printf mandatory CGI data and layout html
void init_page() {
    char *env_string = getenv("QUERY_STRING");
    unsigned long env_length = 0;
    
    printf("Context-Type: text/html\n\n");
    
    const char *base_page = "<html><head><meta charset=\"utf-8\"><meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"><meta name=\"description\" content=\"\"><meta name=\"author\" content=\"\"><!-- jQuery Version 1.11.1 --><script src=\"/js/jquery.js\"></script><!-- Bootstrap Core JavaScript --><script src=\"/js/bootstrap.min.js\"></script><!-- Bootstrap Core CSS --><link href=\"../css/bootstrap.min.css\" rel=\"stylesheet\"><!-- Custom CSS --><style>body {padding-top: 70px; background-color: #bbbbbb;}</style></head><body><!-- Navigation --><nav class=\"navbar navbar-inverse navbar-fixed-top\" role=\"navigation\"><div class=\"container\"><!-- Brand and toggle get grouped for better mobile display --><div class=\"navbar-header\"><button type=\"button\" class=\"navbar-toggle\" data-toggle=\"collapse\" data-target=\"#bs-example-navbar-collapse-1\"><span class=\"sr-only\">Toggle navigation</span><span class=\"icon-bar\"></span><span class=\"icon-bar\"></span><span class=\"icon-bar\"></span></button><a class=\"navbar-brand\" href=\"/cgi-bin/cblog.cgi\">Colin Luoma</a></div><!-- Collect the nav links, forms, and other content for toggling --><div class=\"collapse navbar-collapse\" id=\"bs-example-navbar-collapse-1\"><ul class=\"nav navbar-nav\"><li><a href=\"/cgi-bin/cblog.cgi?\">Blog</a></li><li><a href=\"/cgi-bin/contact.cgi?\">Contact</a></li></ul></div><!-- /.navbar-collapse --></div><!-- /.container --></nav>";
    
    if (env_string != NULL) {
        env_length = strlen(env_string);
        char *prop = malloc(strlen(base_page) + env_length*2 + 1);
        sprintf(prop, "<html><head><meta charset=\"utf-8\"><meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"><meta name=\"description\" content=\"\"><meta name=\"author\" content=\"\"><!-- jQuery Version 1.11.1 --><script src=\"/js/jquery.js\"></script><!-- Bootstrap Core JavaScript --><script src=\"/js/bootstrap.min.js\"></script><!-- Bootstrap Core CSS --><link href=\"../css/bootstrap.min.css\" rel=\"stylesheet\"><!-- Custom CSS --><style>body {padding-top: 70px; background-color: #bbbbbb;}</style></head><body><!-- Navigation --><nav class=\"navbar navbar-inverse navbar-fixed-top\" role=\"navigation\"><div class=\"container\"><!-- Brand and toggle get grouped for better mobile display --><div class=\"navbar-header\"><button type=\"button\" class=\"navbar-toggle\" data-toggle=\"collapse\" data-target=\"#bs-example-navbar-collapse-1\"><span class=\"sr-only\">Toggle navigation</span><span class=\"icon-bar\"></span><span class=\"icon-bar\"></span><span class=\"icon-bar\"></span></button><a class=\"navbar-brand\" href=\"/cgi-bin/cblog.cgi\">Colin Luoma</a></div><!-- Collect the nav links, forms, and other content for toggling --><div class=\"collapse navbar-collapse\" id=\"bs-example-navbar-collapse-1\"><ul class=\"nav navbar-nav\"><li><a href=\"/cgi-bin/cblog.cgi?%s\">Blog</a></li><li><a href=\"/cgi-bin/contact.cgi?%s\">Contact</a></li></ul></div><!-- /.navbar-collapse --></div><!-- /.container --></nav>", env_string, env_string);
        prop[strlen(base_page) + env_length*2] = '\0';
        printf("%s", prop);
        free(prop);
    } else {
        printf("%s", base_page);
    }
}

void print_about_panel() {
    /* Panel */
    printf("<div class=\"panel panel-default\" style=\"border-color: black\">");
    
    /* Panel Heading */
    printf("<div class=\"panel-heading\" style=\"background-color: #222; color: #66bbdd\">About This Blog</div>");
    
    /* Panel Body */
    printf("<div class=\"panel-body\"><p>");
    
    printf("I love working with computers and data. This blog is intended as written documentation of my ongoing side-projects. Most everything here is likely to be programming or data related, although something else may creep in every now and then. The blog itself is an example of a personal project of mine. It was written entirely from scratch using HTML, C, Twitter's Bootstrap CSS, and SQLite3.<br><br>");
    
    printf("As of writing this I'm currently working as a data analyst at Ubisoft.");
    
    printf("</p></div>");
    
    /* Close Panel */
    printf("</div>");
}

void print_ongoing_projects() {
    /* Panel */
    printf("<div class=\"panel panel-default\" style=\"border-color: black\">");
    
    /* Panel Heading */
    printf("<div class=\"panel-heading\" style=\"background-color: #222; color: #66bbdd\">Current Projects</div>");
    
    /* Panel Body */
    printf("<div class=\"panel-body\"><p>");
    
    printf("Below are some of my projects that I am currently working on:<br><br>");
    
    // This blog
    printf("<b><a href=\"http://new.cluoma.com\">This website</a></b><br><br>");
    
    // Haloviewer
    printf("<b><a href=\"http://haloviewer.bluewizard.ca\">Halo Viewer:</a></b> A website written in C++ using the Wt web framework intended to provide player statistics from the video game Halo 5<br><br>");
    
    // Halo 5 API
    printf("<b><a href=\"http://github.com/cluoma/h5api\">Halo 5 API R Package:</a></b> An R package with quick functions to access data from 343i's Halo 5 web API.<br><br>");
    
    // BlueWizard
    printf("<b><a href=\"http://bluewizard.ca\">Blue Wizard Needs Food Badly:</a></b> My video games blog. Most of the content there has been mirrored here lately but anything that is strictly games related goes there.<br><br>");
    
    // My github
    printf("<b><a href=\"http://github.com/cluoma\">My Github:</a></b> A link to my Github page. I'm fairly new to github so there isn't much there. The Halo Viewer code should be added soon as well as the code for this page.");
    
    printf("</p></div>");
    
    /* Close Panel */
    printf("</div>");
}

void print_contact() {
    /* Panel */
    printf("<div class=\"panel panel-default\" style=\"border-color: black\">");
    
    /* Panel Heading */
    printf("<div class=\"panel-heading\" style=\"background-color: #222; color: #66bbdd\">Contact Info</div>");
    
    /* Panel Body */
    printf("<div class=\"panel-body\"><p>");
    
    printf("Feel free to get in contact with me:<br><br>");
    printf("Email: <a href=\"mailto:cluoma@ualberta.ca\">cluoma@ualberta.ca</a> or <a href=\"mailto:themjolnir999@gmail.com\">themjolnir999@gmail.com</a>");
    
    printf("</p></div>");
    
    /* Close Panel */
    printf("</div>");
}

int main(int argc, const char * argv[]) {
    
    /* Mandatory HTML info and navbar, etc. */
    init_page();
    
    printf("<div class=\"container\" width=\"80%%\">"); // Start container that holds everything
    
    print_about_panel();
    
    print_ongoing_projects();
    
    print_contact();
    
    printf("</div>"); // Close container
    
    printf("</body>");
    
    /* Print page footer, copyright, name, etc. */
    printf("<footer style=\"text-align: center\">Copyright 2016 Colin Luoma<br>");
    printf("Made with C and Twitter Bootstrap<br><3");
    printf("</footer>");
    
    return 0;
}
