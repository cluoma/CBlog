//
//  main.c
//  file_browser
//
//  Created by Colin Luoma on 2016-05-05.
//  Copyright (c) 2016 Colin Luoma. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sqlite3.h>
#include <dirent.h>

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

// Given a pictures name, print it, we assume that it is in a root images directory
void picture_panel(const char *name) {
    /* Panel */
    printf("<div class=\"panel panel-default\" style=\"border-color: black\">");
    
    /* Panel Heading */
    printf("<div class=\"panel-heading\" style=\"background-color: #222; color: #66bbdd\">%s</div>", name);
    
    /* Panel Body */
    printf("<div class=\"panel-body\">");
    printf("<img src=\"../images/%s\" width=\"100\" height=\"100\" />", name);
    printf("</div>");
    
    /* Close Panel */
    printf("</div>");
}

// Opens directory containing pictures creates a panel for each one
void display_pictures () {
    // Path to images, this is different from the web directory
    char *file_path = "/Users/Colin/Documents/CBlog/server/images/";
    DIR *directory;
    struct dirent *dirent;
    
    directory = opendir(file_path);
    
    if (directory) {
        // Read each file in directoryt
        while ((dirent = readdir(directory)) != NULL) {
            char *file = dirent->d_name;
            // If filename ends in "png" or "jpg", show it
            if (strcmp(file+strlen(file)-3, "png") == 0 ||
                strcmp(file+strlen(file)-3, "jpg") == 0) {
                picture_panel(file);
            }
        }
    } else {
        fprintf(stderr, "Failed to open directory.\n");
    }
    // Free memory
    closedir(directory);
}

void upload_form() {
    printf("<form method=\"post\" action=\"/cgi-bin/add_file.cgi\" enctype=\"multipart/form-data\"><input type=\"file\" name=\"file\"></input><input type=\"submit\" value=\"submit\"></input></form>");
}

int main(int argc, const char * argv[]) {
    
    char *env_string = getenv("QUERY_STRING");
    
    /* Mandatory HTML info and navbar, etc. */
    init_page();
    
    printf("<div class=\"container\" width=\"80%%\">"); // Start container that holds everything
    
    upload_form();
    
    display_pictures(); // Display all pictures each in a panel
    
    printf("</div>"); // Close container
    
    printf("</body>");
    
    /* Print page footer, copyright, name, etc. */
    printf("<footer style=\"text-align: center\">Copyright 2016 Colin Luoma<br>");
    printf("Made with C and Twitter Bootstrap<br><3");
    printf("</footer>");
    
    return 0;
}
