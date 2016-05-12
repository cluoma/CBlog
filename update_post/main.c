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

// Decodes URL strings to text (eg '+' -> ' ' and % hex codes)
void html_to_text(char *source, char *dest) {
    while (*source != '\0') {
        if (*source == '+') {
            *dest = ' ';
        }
        else if (*source == '%') {
            int hex_char;
            sscanf(source+1, "%2x", &hex_char);
            *dest = hex_char;
            source += 2;
        } else {
            *dest = *source;
        }
        source++;
        dest++;
    }
    *dest = '\0';
}

// Must include '=' after variable name
// Malloc is used, up to user to free returned string
// Doesn't check beginning of variable (eg post_title= could match against my_post_title=)
char *get_variable(char *source, char *var) {
    char *tmp = strstr(source, var); // Find variable
    
    // Return NULL if string not found
    if (tmp == NULL) {
        return tmp;
    }
    
    tmp += strlen(var); // Offset the variable name
    
    // How many characters until '&'
    int length = 0;
    while (*tmp != '&' && *tmp != '\0') {
        length++;
        tmp++;
    }
    tmp = tmp - length;
    
    // Space for 2 new strings
    char *variable_raw = malloc(length+1);
    char *variable_filtered = malloc(length+1);
    
    // Use source to fill raw string
    for (int i = 0; i < length; i++) {
        variable_raw[i] = tmp[i];
    }
    variable_raw[length] = '\0';
    
    // Decode raw string to filtered string and free raw string
    html_to_text(variable_raw, variable_filtered);
    free(variable_raw);
    
    return variable_filtered;
}

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
