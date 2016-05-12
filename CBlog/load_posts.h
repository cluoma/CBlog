//
//  load_posts.c
//  CBlog
//
//  Created by Colin Luoma on 2016-05-05.
//  Copyright (c) 2016 Colin Luoma. All rights reserved.
//
//  Interface functions to SQLite3 database
//

#ifndef __CBlog__load_posts__
#define __CBlog__load_posts__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

struct Post {
    int post_id;
    char *title;
    char *text;
    char *time;
};

struct Posts {
    int number_of_posts;
    struct  Post *posts;
};

int callback(void *data, int argc, char **argv, char **azColName);

struct Posts load_posts(int number_of_posts, int offset);
struct Post load_post_id(int post_id);

int update_post(char *post_id, char *post_title, char *post_text);
int create_post(char *post_title, char *post_text);
int delete_post(int post_id);

void free_post(struct Post *post);
void free_posts(struct Posts *posts);

#endif /* defined(__CBlog__load_posts__) */
