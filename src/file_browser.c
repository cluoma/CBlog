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
#include <stdarg.h>
#include "base_html.h"

void asdf(const char* string, int n, ...);

// Return maximum value of a or b
int max(int a, int b) {
    if (a > b)
        return a;
    else if (a < b)
        return b;
    else
        return a;
}

//void asdf(const char* string, int n, ...) {
//    va_list vl;
//    va_start(vl, n);
//    
//    size_t length = strlen(string) + 1;
//    for (int i = 0; i < n; i++) {
//        length += strlen(va_arg(vl, char*));
//    }
//    va_end(vl);
//    
//    char *ss = malloc(length);
//    va_start(vl, n);
//    vsnprintf(ss, length, string, vl);
//    va_end(vl);
//    fprintf(stderr, "TOTAL LENGTH: %lu\nSTRING: %s", length, ss);
//    free(ss);
//}

// Given a pictures name, print it, we assume that it is in a root images directory
void picture_panel(const char *name) {
    
    char *image;
    asprintf(&image, "<img src=\"../images/%s\" width=\"100\" height=\"100\" />", name);
    
    print_panel(name, NULL, NULL, image);
    free(image);
}

// Opens directory containing pictures creates a panel for each one
void display_pictures () {
    // Path to images, this is different from the web directory
    char *file_path = "/home/ubuntu/CBlog/images/";
    DIR *directory;
    struct dirent *dirent;
    
    directory = opendir(file_path);
    
    if (directory) {
        // Read each file in directoryt
        while ((dirent = readdir(directory)) != NULL) {
            char *file = dirent->d_name;
            // If filename ends in "png" or "jpg", show it
            if (strcmp(file+strlen(file)-3, "png") == 0 ||
                strcmp(file+strlen(file)-3, "PNG") == 0 ||
                strcmp(file+strlen(file)-3, "jpg") == 0 ||
                strcmp(file+strlen(file)-3, "JPG") == 0) {
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
    printf("<form method=\"post\" action=\"/edit_posts/add_file.cgi\" enctype=\"multipart/form-data\"><input type=\"file\" name=\"file\"></input><input type=\"submit\" value=\"submit\"></input></form>");
}

int main(int argc, const char * argv[]) {
    
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
