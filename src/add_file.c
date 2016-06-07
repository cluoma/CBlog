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

#define MAX_FILENAME 50

int main(int argc, const char * argv[]) {
    
    // Needed, says the document return plaintext
    printf("Context-Type: text/plaintext\n\n<html><pre>");
    
    // How many characters should we read in from STDIN?
    char *env_length = getenv("CONTENT_LENGTH");
    if (env_length == NULL) {
        printf("No env query, did nothing.");
        return 0;
    }
    long length = strtol(env_length, NULL, 10);
    
    // Allocate space and read in ENV QUERY
    char *input = malloc(length+2);
    fread(input, 1, length, stdin);
    input[length+1] = '\0';
    
    // Find filename from POST data
    char *file_name = strstr(input, "filename=\"");
    char file_name_buf[MAX_FILENAME+1];
    if (file_name != NULL && file_name != input) {
        file_name += 10;
        int i = 0;
        while(*file_name != '"' && i < MAX_FILENAME) {
            file_name_buf[i] = *file_name;
            file_name++;
            i++;
        }
        file_name_buf[i] = '\0';
    }
    
    // Only add picture if it ends correctly
    if (strcmp(file_name_buf+strlen(file_name_buf)-3, "png") == 0 ||
        strcmp(file_name_buf+strlen(file_name_buf)-3, "PNG") == 0 ||
        strcmp(file_name_buf+strlen(file_name_buf)-3, "jpg") == 0 ||
        strcmp(file_name_buf+strlen(file_name_buf)-3, "JPG") == 0) {
        
        // Complete file path with file name
        char *file_path;
        asprintf(&file_path, "/home/ubuntu/CBlog/images/%s", file_name_buf);
        
        // Open file using filepath and filename
        FILE *out;
        out = fopen(file_path, "wb+");
        
        // Print stuff
        printf("BEST FILENAME: %s\n", file_name_buf);
        printf("COMPLETE PATH: %s\n", file_path);
        
        // Get index in input for the start of file data
        int i = 0;
        for (i = 0; i < length-4; i++) {
            // Double newline indicates start of data
            if ((int)input[i] == 13 && (int)input[i+1] == 10 &&
                (int)input[i+2] == 13 && (int)input[i+3] == 10) {
                i = i + 4;
                break;
            }
        }
        
        // Write out data into file and free data
        fwrite(input+i, 1, length-i, out);
        fclose(out);
        free(file_path);
    }
    
    // Free allocated data
    free(input);
    
    return 0;
}