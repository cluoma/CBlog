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

/* Shows a summary of all blog posts */
void print_blog_posts() {
    
    // Request all blog posts
    struct Posts posts = load_posts(100000, 0);
    struct Post *my_posts = posts.posts;
    int number_of_posts_returned = posts.number_of_posts;
    
    // New Post button, (edit=0 means new post)
    printf("<a href=\"/cgi-bin/post_editor.cgi?edit=0\" class=\"btn btn-info\" role=\"button\">New Post</a><br><br>");
    
    // Print each posts title and date
    for (int i = 0; i < number_of_posts_returned; i++) {
        /* Panel */
        printf("<div class=\"panel panel-default\" style=\"border-color: black\">");
        
        /* Panel Heading */
        printf("<div class=\"panel-heading\" style=\"background-color: #222; color: #66bbdd\">%s</div>", my_posts[i].title);
        
        /* Panel Body */
        printf("<div class=\"panel-body\">");
        // Posting date
        printf("<p>Date Posted: %s</p>", my_posts[i].time);
        // Edit button
        printf("<a href=\"/cgi-bin/post_editor.cgi?edit=%d\" class=\"btn btn-info\" role=\"button\" style=\"float: left\">Edit</a>", my_posts[i].post_id);
        // Delete button
        printf("<a href=\"/cgi-bin/post_editor.cgi?delete=%d\" class=\"btn btn-info\" role=\"button\" style=\"float: right\">Delete</a>", my_posts[i].post_id);
        printf("</div>");
        
        /* Close Panel */
        printf("</div>");
        
        // Free data from posts after it has been printed
        free(my_posts[i].title);
        free(my_posts[i].text);
        free(my_posts[i].time);
    }
    // Free list of posts
    free(my_posts);
}

/* Edit a given blog post */
void edit_post(int post_id) {
    struct Post post = load_post_id(post_id);
    
    /* Panel */
    printf("<div class=\"panel panel-default\" style=\"border-color: black\">");
    
    /* Panel Heading */
    printf("<div class=\"panel-heading\" style=\"background-color: #222; color: #66bbdd\">%s</div>", post.title);
    
    /* Panel Body */
    printf("<div class=\"panel-body\">");
    
    /* Start Form */
    printf("<form role=\"form\" method=\"post\" action=\"/cgi-bin/update_post.cgi\">");
    
    // Title
    printf("<div class=\"form-group\">");
    printf("<label for=\"post_title\">Title:</label><textarea name=\"post_title\" class=\"form-control\" rows=\"1\" id=\"post_title\">%s</textarea>", post.title);
    printf("</div>");
    printf("<br>");
    
    // Text
    printf("<div class=\"form-group\">");
    printf("<label for=\"post_text\">Post:</label><textarea name=\"post_text\" class=\"form-control\" rows=\"20\" id=\"post_text\">%s</textarea>", post.text);
    printf("</div>");
    
    // Post ID (not visible)
    printf("<div class=\"form-group hidden\">");
    printf("<label for=\"post_text\">Post ID:</label><textarea name=\"post_id\" class=\"form-control\" rows=\"1\" id=\"post_id\">%d</textarea>", post.post_id);
    printf("</div>");
    
    // Submit button
    printf("<button type=\"submit\" class=\"btn btn-info\">Submit</button>");
    
    /* End Form */
    printf("</form>");
    
    /* Close Panel Body */
    printf("</div>");
    /* Close Panel */
    printf("</div>");
    
    // Free post
    free_post(&post);
}

/* Create a new blog post */
void new_post() {
    
    /* Panel */
    printf("<div class=\"panel panel-default\" style=\"border-color: black\">");
    
    /* Panel Heading */
    printf("<div class=\"panel-heading\" style=\"background-color: #222; color: #66bbdd\">New Blog Post</div>");
    
    /* Panel Body */
    printf("<div class=\"panel-body\">");
    
    /* Start Form */
    printf("<form role=\"form\" method=\"post\" action=\"/cgi-bin/update_post.cgi\">");
    
    // Title
    printf("<div class=\"form-group\">");
    printf("<label for=\"post_title\">Title:</label><textarea name=\"post_title\" class=\"form-control\" rows=\"1\" id=\"post_title\"></textarea>");
    printf("</div>");
    printf("<br>");
    
    // Text
    printf("<div class=\"form-group\">");
    printf("<label for=\"post_text\">Post:</label><textarea name=\"post_text\" class=\"form-control\" rows=\"20\" id=\"post_text\"></textarea>");
    printf("</div>");
    
    // Post ID (not visible), is 0 for new posts
    printf("<div class=\"form-group hidden\">");
    printf("<label for=\"post_text\">Post ID:</label><textarea name=\"post_id\" class=\"form-control\" rows=\"1\" id=\"post_id\">0</textarea>");
    printf("</div>");
    
    // Submit button
    printf("<button type=\"submit\" class=\"btn btn-info\">Submit</button>");
    
    /* End Form */
    printf("</form>");
    
    /* Close Panel Body */
    printf("</div>");
    /* Close Panel */
    printf("</div>");
}

/* Delete a blog post */
void remove_post(int post_id) {
    /* Panel */
    printf("<div class=\"panel panel-default\" style=\"border-color: black\">");
    
    /* Panel Heading */
    printf("<div class=\"panel-heading\" style=\"background-color: #222; color: #66bbdd\">Post Deleted</div>");
    
    /* Panel Body */
    printf("<div class=\"panel-body\">");
    
    /* Print success messege */
    if (delete_post(post_id)) {
        printf("Post successfully deleted.");
    } else {
        printf("Failed to delete post.");
    }
    
    /* Close Panel Body */
    printf("</div>");
    /* Close Panel */
    printf("</div>");
}

int main(int argc, const char * argv[]) {
    
    char *env_string = getenv("QUERY_STRING");
    
    /* Mandatory HTML info and navbar, etc. */
    init_page();
    
    printf("<div class=\"container\" width=\"80%%\">"); // Start container that holds everything
    
    // Print summary of posts if env_string is null or blank
    if (env_string == NULL || strlen(env_string) == 0)
    {
        print_blog_posts();
    }
    // Show edit screen if an edit argument is supplied
    else if (strstr(env_string, "edit="))
    {
        int post_id = -1; // Init post id to -1
        sscanf(env_string, "edit=%d", &post_id);
        if (post_id == 0) {
            new_post();
        } else if (post_id > 0) {
            edit_post(post_id);
        }
    }
    else if (strstr(env_string, "delete="))
    {
        int post_id = -1;
        sscanf(env_string, "delete=%d", &post_id);
        if (post_id > 0) {
            remove_post(post_id);
        }
    }
    
    printf("</div>"); // Close container
    
    printf("</body>");
    
    /* Print page footer, copyright, name, etc. */
    printf("<footer style=\"text-align: center\">Copyright 2016 Colin Luoma<br>");
    printf("Made with C and Twitter Bootstrap<br><3");
    printf("</footer>");
    
    return 0;
}