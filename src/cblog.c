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
#include "load_posts.h"
#include "base_html.h"
#include "cblog.h"

// Return maximum value of a or b
int max(int a, int b) {
    if (a > b)
        return a;
    else if (a < b)
        return b;
    else
        return a;
}

void print_archives() {
    struct archives archives = load_archives();

    printf("<div class=\"sidebar-module\"><h4>Archives</h4><ol class=\"list-unstyled\">");

    for (int i = 0; i < archives.row_count; i++) {
        printf("<li><a href=\"/cgi-bin/cblog.cgi?month=%d&year=%d\">%s %d (%d)</a></li>", archives.month[i], archives.year[i], archives.month_s[i], archives.year[i], archives.post_count[i]);
        free(archives.month_s[i]);
    }

    printf("</ol></div>");

    free(archives.month_s);
    free(archives.month);
    free(archives.year);
    free(archives.post_count);
}

// Loads blog posts from database and displays them as bootstrap Panels
// Also prints back, and forward buttons
void print_blog_posts(int start, int end, char *search) {
    // How many posts are between start and end?
    int number_of_posts_requested = max(end-start+1, 0);

    struct Posts posts;
    if (search != NULL) {
        // Loads all posts that contain search
        posts = search_posts(search);
    } else {
        // Load all posts between start and end
        posts = load_posts(number_of_posts_requested, start);
    }
    struct Post *my_posts = posts.posts;
    // This could be less than number_of_posts_requested
    int number_of_posts_returned = posts.number_of_posts;

    // Print each posts
    for (int i = 0; i < number_of_posts_returned; i++) {
        // Use a panel for each post
        print_blog_post(my_posts[i].title, my_posts[i].text, my_posts[i].time, NULL);
    }
    // Free the whole list of posts
    free_posts(&posts);


    // Older and New buttons
    printf("<nav><ul class=\"pager\">");
    // If we got the same number of posts we asked for, show the older button
    if (number_of_posts_requested == number_of_posts_returned) {
        printf("<li><a href=\"/cgi-bin/cblog.cgi?page=cblog&start=%d&end=%d\">Older</a></li>", end+1, end+5);
    }

    // If start > 0, show the newer button (means there are newer posts available)
    if (start > 0) {
       printf("<li><a href=\"/cgi-bin/cblog.cgi?page=cblog&start=%d&end=%d\">Newer</a></li>", start-5, start-1);
    }
    printf("</ul></nav>"); // Close older/newer buttons
}

void print_posts_by_monthyear(int month, int year) {

    struct Posts posts;
    posts = load_posts_monthyear(month, year);

    struct Post *my_posts = posts.posts;
    // This could be less than number_of_posts_requested
    int number_of_posts_returned = posts.number_of_posts;

    // Print each posts
    for (int i = 0; i < number_of_posts_returned; i++) {
        // Use a panel for each post
        print_blog_post(my_posts[i].title, my_posts[i].text, my_posts[i].time, NULL);
    }
    // Free the whole list of posts
    free_posts(&posts);
}

// Shows a notification panel when a search has been made
void print_search_notification(char *keyword) {

    // Search text
    char *text;
    asprintf(&text, "Showing blog posts containing the search phrase: <b>%s</b>", keyword);

    print_panel("Search", text, NULL, NULL);

    free(text);
}

void print_about_box() {
    printf("<div class=\"sidebar-module sidebar-module-inset\"><h4>About</h4><p>");
    printf("This blog is intended as written documentation of my ongoing side-projects. Most everything here is likely to be programming or data related, although something else may creep in every now and then. The blog itself is an example of a personal project of mine. It was written entirely from scratch using HTML, C, Twitter's Bootstrap CSS, and SQLite3.");
    printf("</p></div>");
}

void print_cblog(query_vars *query)
{
    printf("<div class=\"container\">"); // Start container that holds posts

    printf("<div class=\"row\"><div class=\"col-sm-8 blog-main\">");

    if (strcmp(query->search, "") != 0) {
        print_search_notification(query->search);
        print_blog_posts(0, 10000, query->search);
    } else if (strcmp(query->month, "") != 0 && strcmp(query->year, "") != 0) {
        print_posts_by_monthyear(atoi(query->month), atoi(query->year));
    } else if (strcmp(query->start, "") != 0 && strcmp(query->end, "") != 0) {
        int int_start = atoi(query->start);
        int int_end = atoi(query->end);
        print_blog_posts(int_start, int_end, NULL);
    } else {
        print_blog_posts(0, 4, NULL);
    }

    printf("</div>"); // Close Blog

    // Blog sidebar
    printf("<div class=\"col-sm-3 col-sm-offset-1 blog-sidebar\">");

    print_about_box();
    print_archives();

    printf("</div>"); // Close sidebar

    printf("</div>"); // Close row

    printf("</div>"); // Close container
}
