//
//  main.c
//  update_post
//
//  Created by Colin Luoma on 2016-05-08.
//  Copyright (c) 2016 Colin Luoma. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "load_posts.h"
#include "base_html.h"

int main(int argc, const char * argv[]) {
    // Needed, says the document return plaintext
    printf("Context-Type: text/plaintext\n\n");
    
    // How many characters should we read in from STDIN?
    char *env_length = getenv("CONTENT_LENGTH");
    if (env_length == NULL) {
        printf("No env query, did nothing.");
        return 0;
    }
    long length = strtol(env_length, NULL, 10);
    
    // Allocate space and read in ENV QUERY
    char *input = malloc(length+2);
    fgets(input, (int)length+1, stdin);
    input[length+1] = '\0';
    
    // Get the variables we want
    char *post_title = get_variable(input, "post_title=");
    char *post_text = get_variable(input, "post_text=");
    char *post_id = get_variable(input, "post_id=");
    int int_post_id = atoi(post_id);
    
    // Print them
    printf("post_title:\n%s\n\n", post_title);
    printf("post_text:\n%s\n\n", post_text);
    printf("post_ID:\n%s\n\n", post_id);
    
    // Push them to database
    if (int_post_id == 0) {
        create_post(post_title, post_text);
    } else {
        update_post(post_id, post_title, post_text);
    }
    
    // Free allocated data
    free(post_id);
    free(post_title);
    free(post_text);
    free(input);
    
    return 0;
}
