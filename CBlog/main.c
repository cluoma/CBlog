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

// Return maximum value of a or b
int max(int a, int b) {
    if (a > b)
        return a;
    else if (a < b)
        return b;
    else
        return a;
}

// Loads blog posts from database and displays them as bootstrap Panels
// Also prints back, and forward buttons
void print_blog_posts(int start, int end, char *search) {
    // How many posts are between start and end?
    int number_of_posts_requested = max(end-start+1, 0);
    
    // Load all posts between start and end
    struct Posts posts;
    if (search != NULL) {
        posts = search_posts(search);
    } else {
        posts = load_posts(number_of_posts_requested, start);
    }
    struct Post *my_posts = posts.posts;
    // This could be less than number_of_posts_requested
    int number_of_posts_returned = posts.number_of_posts;
    
    // Print each posts
    for (int i = 0; i < number_of_posts_returned; i++) {
        // Use a panel for each post
        print_panel(my_posts[i].title, my_posts[i].text, my_posts[i].time, NULL);
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
}

// Shows a notification panel when a search has been made
void print_search_notification(char *keyword) {
    
    // Search text
    char *text;
    asprintf(&text, "Showing blog posts containing the search phrase: %s", keyword);
    
    print_panel("Search", text, NULL, NULL);
    
    free(text);
}

int main(int argc, const char * argv[]) {
    
    /* Mandatory HTML info and navbar, etc. */
    init_page();
    
    /* Print Blog entries */
    char *env_string = getenv("QUERY_STRING");
    
    // Get the variables we want from env variable
    char *start = get_variable(env_string, "start=");
    char *end = get_variable(env_string, "end=");
    char *search = get_variable(env_string, "search=");
    
    printf("<div class=\"container\" width=\"80%%\">"); // Start container that holds posts
    
    if (search != NULL) {
        print_search_notification(search);
        print_blog_posts(0, 10000, search);
    } else if (start != NULL && end != NULL) {
        int int_start = atoi(start);
        int int_end = atoi(end);
        print_blog_posts(int_start, int_end, NULL);
    } else {
        print_blog_posts(0, 4, NULL);
    }
    // Free the variables we got
    free(start); free(end); free(search);
    
    printf("</div>"); // Close container
    
    printf("</body>"); // End HTML body, is opened in init_page()
    
    /* Print page footer, copyright, name, etc. */
    print_footer();
    
    printf("</html>");
    
    return 0;
}
