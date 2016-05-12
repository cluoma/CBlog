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
#include <sqlite3.h>
#include "load_posts.h"

// Return maximum value of a or b
int max(int a, int b) {
    if (a > b)
        return a;
    else if (a < b)
        return b;
    else
        return a;
}

// Returns a new string with all '\n' characters replaced by "<br>" for use in HTML
char *newline_to_html(char *string) {
    int newlines = 0;
    for (int i = 0; i < strlen(string); i++) {
        if (string[i] == '\n')
            newlines++;
    }
    
    // Allocate space for new string with enough room for the <br>'s
    char *new_string = malloc(strlen(string) + newlines*3 + 1);
    
    int new_index = 0;
    for (int old_index = 0; old_index < strlen(string); old_index++) {
        if (string[old_index] == '\n')
        {
            new_string[new_index] = '<';
            new_index++;
            new_string[new_index] = 'b';
            new_index++;
            new_string[new_index] = 'r';
            new_index++;
            new_string[new_index] = '>';
        }
        else
        {
            new_string[new_index] = string[old_index];
        }
        new_index++;
    }
    new_string[new_index] = '\0';
    return new_string;
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

void print_blog_posts(int start, int end) {
    // How many posts are between start and end?
    int number_of_posts_requested = max(end-start+1, 0);
    
    // Load all posts between start and end
    struct Posts posts = load_posts(number_of_posts_requested, start);
    struct Post *my_posts = posts.posts;
    // This could be less than number_of_posts_requested
    int number_of_posts_returned = posts.number_of_posts;
    
    printf("<div class=\"container\" width=\"80%%\">"); // Start container that holds posts
    
    // Print each posts
    for (int i = 0; i < number_of_posts_returned; i++) {
        /* Panel */
        printf("<div class=\"panel panel-default\" style=\"border-color: black\">");
        
        /* Panel Heading */
        printf("<div class=\"panel-heading\" style=\"background-color: #222; color: #66bbdd\">%s</div>", my_posts[i].title);
        
        /* Panel Body */
        printf("<div class=\"panel-body\">");
        char *formatted_post_text = newline_to_html(my_posts[i].text);
        printf("<p>%s</p>", formatted_post_text);
        printf("<p>Date Posted: %s</p>", my_posts[i].time);
        printf("</div>");
        
        /* Close Panel */
        printf("</div>");
        
        // Free temporary string
        free(formatted_post_text);
    }
    // Free the whole list of posts
    free_posts(&posts);
    
    // If we got the same number of posts we asked for, show the older button
    if (number_of_posts_requested == number_of_posts_returned) {
        printf("<a href=\"/cgi-bin/cblog.cgi?start=%d&end=%d\" class=\"btn btn-info\" role=\"button\">Older</a>", end+1, end+5);
    }
    
    // If start > 0, show the newer button (means there are newer posts available)
    if (start > 0) {
       printf("<a href=\"/cgi-bin/cblog.cgi?start=%d&end=%d\" class=\"btn btn-info\" role=\"button\" style=\"float: right\">Newer</a>", start-5, start-1);
    }
    
    printf("</div>"); // Close container
}

int parse_env(const char *var, char *query) {
    int start = 0;
    int end = 0;
    
    sscanf(query, "start=%d&end=%d", &start, &end);
    
    if (strcmp(var, "start") == 0) {
        return start;
    } else if (strcmp(var, "end") == 0) {
        return end;
    } else {
        return 0;
    }
}

int main(int argc, const char * argv[]) {
    
    /* Mandatory HTML info and navbar, etc. */
    init_page();
    
    /* Print Blog entries */
    char *env_string = getenv("QUERY_STRING");
    if (env_string != NULL && strstr(env_string, "start=") != NULL && strstr(env_string, "end=") != NULL) {
        print_blog_posts(parse_env("start", env_string),
                         parse_env("end",   env_string));
    } else {
        print_blog_posts(0, 4);
    }
    
    printf("</body>");
    
    /* Print page footer, copyright, name, etc. */
    printf("<footer style=\"text-align: center\">Copyright 2016 Colin Luoma<br>");
    printf("Made with C and Twitter Bootstrap<br><3");
    printf("</footer>");
    
    printf("</html>");
    
    return 0;
}
