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

/* Shows a summary of all blog posts */
void print_blog_posts() {
    
    // Request all blog posts
    struct Posts posts = load_posts(100000, 0);
    struct Post *my_posts = posts.posts;
    int number_of_posts_returned = posts.number_of_posts;
    
    // New Post button, (edit=0 means new post)
    printf("<a href=\"/cgi-bin/post_editor.cgi?edit=0\" class=\"btn btn-info\" role=\"button\">New Post</a><br><br>");
    
    // Print each posts title, date, and edit/delete buttons
    for (int i = 0; i < number_of_posts_returned; i++) {
        
        // Edit and Delete buttons
        char *extra;
        asprintf(&extra, "<a href=\"/cgi-bin/post_editor.cgi?edit=%d\" class=\"btn btn-info\" role=\"button\" style=\"float: left\">Edit</a><a href=\"/cgi-bin/post_editor.cgi?delete=%d\" class=\"btn btn-info\" role=\"button\" style=\"float: right\">Delete</a>", my_posts[i].post_id, my_posts[i].post_id);
        
        print_panel(my_posts[i].title, NULL, my_posts[i].time, extra);
        free(extra);
    }
    
    // Free list of posts
    free_posts(&posts);
}

/* Edit a given blog post */
void edit_post(int post_id) {
    struct Post post = load_post_id(post_id);
    
    /* Start Form */
    char *form_start = "<form role=\"form\" method=\"post\" action=\"/cgi-bin/update_post.cgi\">";
    
    // Title
    char *title;
    asprintf(&title,"<div class=\"form-group\"><label for=\"post_title\">Title:</label><textarea name=\"post_title\" class=\"form-control\" rows=\"1\" id=\"post_title\">%s</textarea></div><br>", post.title);
    
    // Text
    char *text;
    asprintf(&text, "<div class=\"form-group\"><label for=\"post_text\">Post:</label><textarea name=\"post_text\" class=\"form-control\" rows=\"20\" id=\"post_text\">%s</textarea></div>", post.text);
    
    // Post ID (not visible)
    char *id;
    asprintf(&id, "<div class=\"form-group hidden\"><label for=\"post_text\">Post ID:</label><textarea name=\"post_id\" class=\"form-control\" rows=\"1\" id=\"post_id\">%d</textarea></div>", post.post_id);
    
    // Submit button
    char *button = "<button type=\"submit\" class=\"btn btn-info\">Submit</button>";
    
    /* End Form */
    char *end_form = "</form>";
    
    // Put form in extra argument for panel
    char *extra;
    asprintf(&extra, "%s%s%s%s%s%s", form_start, title, text, id, button, end_form);
    print_panel(post.title, NULL, NULL, extra);
    
    // Free memory
    free(title); free(text); free(id);
    free(extra);
    
    // Free post
    free_post(&post);
}

/* Create a new blog post */
void new_post() {
    
    /* Start Form */
    char *start_form = "<form role=\"form\" method=\"post\" action=\"/cgi-bin/update_post.cgi\">";
    
    // Title
    char *title = "<div class=\"form-group\"><label for=\"post_title\">Title:</label><textarea name=\"post_title\" class=\"form-control\" rows=\"1\" id=\"post_title\"></textarea></div><br>";
    
    // Text
    char *text = "<div class=\"form-group\"><label for=\"post_text\">Post:</label><textarea name=\"post_text\" class=\"form-control\" rows=\"20\" id=\"post_text\"></textarea></div>";
    
    // Post ID (not visible), is 0 for new posts
    char *id = "<div class=\"form-group hidden\"><label for=\"post_text\">Post ID:</label><textarea name=\"post_id\" class=\"form-control\" rows=\"1\" id=\"post_id\">0</textarea></div>";
    
    // Submit button
    char *button = "<button type=\"submit\" class=\"btn btn-info\">Submit</button>";
    
    /* End Form */
    char *end_form = "</form>";
    
    // Put Form inside panel Extras
    char *extra;
    asprintf(&extra, "%s%s%s%s%s%s", start_form, title, text, id, button, end_form);
    print_panel("New Blog Post", NULL, NULL, extra);
    
    // Free memory
    free(extra);
}

/* Delete a blog post */
void remove_post(int post_id) {
    
    char *title = "Post Deleted";
    
    /* Success messege */
    if (delete_post(post_id)) {
        print_panel(title, "Post successfully deleted.", NULL, NULL);
    } else {
        print_panel(title, "Failed to delete post.", NULL, NULL);
    }
}

int main(int argc, const char * argv[]) {
    
    char *env_string = getenv("QUERY_STRING");
    
    // DEBUGGING
    //char *env_string = "edit=0";
    //
    
    /* Mandatory HTML info and navbar, etc. */
    init_page();
    
    printf("<div class=\"container\" width=\"80%%\">"); // Start container that holds everything
    
    char *edit = get_variable(env_string, "edit=");
    char *delete = get_variable(env_string, "delete=");
    
    // Print summary of posts if env_string is null or blank
    if (env_string == NULL || strlen(env_string) == 0)
    {
        print_blog_posts();
    }
    // Show edit screen if an edit argument is supplied
    else if (edit != NULL)
    {
        int post_id = atoi(edit);
        if (post_id == 0) {
            new_post();
        } else if (post_id > 0) {
            edit_post(post_id);
        }
    }
    else if (delete != NULL)
    {
        int post_id = atoi(delete);
        if (post_id > 0) {
            remove_post(post_id);
        }
    }
    free(edit); free(delete);
    
    printf("</div>"); // Close container
    
    printf("</body>");
    
    /* Print page footer, copyright, name, etc. */
    print_footer();
    
    return 0;
}