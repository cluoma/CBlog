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

struct archives {
    char **month_s;
    int *month;
    int *year;
    int *post_count;
    int row_count;
};

int callback(void *data, int argc, char **argv, char **azColName);
static int archives_callback(void *data, int argc, char **argv, char **azColName);

struct Posts load_posts(int number_of_posts, int offset);
struct Post load_post_id(int post_id);
struct Posts load_posts_monthyear(int month, int year);
// Given a keyword string, returns all posts where it appears in the body text
struct Posts search_posts(char *keyword);

int update_post(char *post_id, char *post_title, char *post_text);
int create_post(char *post_title, char *post_text);
int delete_post(int post_id);

struct archives load_archives();

char *nmonth_to_smonth(int month);

void free_post(struct Post *post);
void free_posts(struct Posts *posts);

#endif /* defined(__CBlog__load_posts__) */
