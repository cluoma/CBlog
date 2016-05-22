//
//  load_posts.c
//  CBlog
//
//  Created by Colin Luoma on 2016-05-05.
//  Copyright (c) 2016 Colin Luoma. All rights reserved.
//
//  Interface functions to SQLite3 database
//

#include "load_posts.h"

// Callback function for use with exec.
// Loads each post into an array of struct Post
int callback(void *data, int argc, char **argv, char **azColName) {
    
    // Recast void *data to *struct Posts
    struct Posts *posts = data;
    
    // Realloc memory for a new post
    struct Post *post = (struct Post *) realloc(posts->posts, sizeof(struct Post)*(1+posts->number_of_posts));
    
    // Tmp variable for post count
    int post_count = posts->number_of_posts;
    
    // For each returned row
    for(int i=0; i<argc; i++)
    {
        if (strcmp(azColName[i], "title") == 0)             // Post title
        {
            post[post_count].title = malloc(strlen(argv[i]) + 1);
            strcpy(post[post_count].title, argv[i]);
            post[post_count].title[strlen(argv[i])] = '\0';
        }
        else if (strcmp(azColName[i], "post_id") == 0)      // Post ID
        {
            post[post_count].post_id = atoi(argv[i]);
        }
        else if (strcmp(azColName[i], "text") == 0)         // Post text
        {
            post[post_count].text = malloc(strlen(argv[i]) + 1);
            strcpy(post[post_count].text, argv[i]);
            post[post_count].text[strlen(argv[i])] = '\0';
        }
        else if (strcmp(azColName[i], "time") == 0)         // Post time
        {
            post[post_count].time = malloc(strlen(argv[i]) + 1);
            strcpy(post[post_count].time, argv[i]);
            post[post_count].time[strlen(argv[i])] = '\0';
        }
    }
    
    // Point posts to new list of posts and increment post count
    posts->posts = post;
    posts->number_of_posts = post_count + 1;
    
    return 0;
}

static int archives_callback(void *data, int argc, char **argv, char **azColName){
    // Recast void *data to struct
    struct archives *archives = data;
    
    // Realloc memory for a new row
    char **month_s = (char **) realloc(archives->month_s, sizeof(char *)*(1+archives->row_count));
    int *month = (int *) realloc(archives->month, sizeof(int)*(1+archives->row_count));
    int *year = (int *) realloc(archives->year, sizeof(int)*(1+archives->row_count));
    int *post_count = (int *) realloc(archives->post_count, sizeof(int)*(1+archives->row_count));
    
    // Tmp variable for row count
    int row_count = archives->row_count;
    
    // For each returned row
    for(int i=0; i<argc; i++)
    {
        if (strcmp(azColName[i], "month") == 0)             // Post title
        {
            char *tmp = nmonth_to_smonth(atoi(argv[i]));
            month_s[row_count] = malloc(strlen(tmp) + 1);
            strcpy(month_s[row_count], tmp);
            month_s[row_count][strlen(tmp)] = '\0';
            
            month[row_count] = atoi(argv[i]);
        }
        else if (strcmp(azColName[i], "year") == 0)      // Post ID
        {
            year[row_count] = atoi(argv[i]);
        }
        else if (strcmp(azColName[i], "num_posts") == 0)         // Post text
        {
            post_count[row_count] = atoi(argv[i]);
        }
    }
    
    // Point posts to new list of posts and increment post count
    archives->month_s = month_s;
    archives->month = month;
    archives->year = year;
    archives->post_count = post_count;
    archives->row_count = row_count + 1;
    
    return 0;
}

// Helper function to open SQLite3 DB
sqlite3 *open_database() {
    sqlite3 *db;
    int rc;
    char *db_location = "/Users/colin/Documents/CBlog/server/personl_blog.db";
    
    /* Open connection to SQLite db */
    rc = sqlite3_open(db_location, &db);
    if( rc ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return NULL;
    }else{
        fprintf(stderr, "Opened database successfully\n");
        return db;
    }
}

// Used to edit and existing post, post_id is the primary key
// 0 - FAIL, 1 - SUCCESS
int update_post(char *post_id, char *post_title, char *post_text) {
    /* SQLite variable declarations */
    int rc;
    sqlite3_stmt *results;
    
    char *sql = "UPDATE posts SET title = ?, text = ? WHERE post_id = ?";
    
    /* Connect to DB */
    sqlite3 *db = open_database();
    if (db == NULL) {
        fprintf(stderr, "Failed to open DB connection.\n");
        return 0;
    }
    
    /* Prepare sql statement */
    rc = sqlite3_prepare_v2(db, sql, -1, &results, 0);
    
    // Bind input data to sql statement
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to update table: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    sqlite3_bind_text(results, 1, post_title, (int)strlen(post_title), SQLITE_TRANSIENT);
    sqlite3_bind_text(results, 2, post_text, (int)strlen(post_text), SQLITE_TRANSIENT);
    sqlite3_bind_text(results, 3, post_id, (int)strlen(post_id), SQLITE_TRANSIENT);
    
    // Execute statement
    sqlite3_step(results);
    
    // Free statement, close DB
    sqlite3_finalize(results);
    sqlite3_close(db);
    
    return 1;
}

// Used to create a new post, post_id is automatically generated by the DB
int create_post(char *post_title, char *post_text) {
    /* SQLite variable declarations */
    int rc;
    sqlite3_stmt *results;
    
    char *sql = "INSERT INTO posts (title, text, time) VALUES(?, ?, (strftime('%s', 'now')))";
    
    /* Connect to DB */
    sqlite3 *db = open_database();
    if (db == NULL) {
        fprintf(stderr, "Failed to open DB connection.\n");
        return 0;
    }
    
    /* Prepare sql statement */
    rc = sqlite3_prepare_v2(db, sql, -1, &results, 0);
    
    // Bind input data to sql statement
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to update table: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    sqlite3_bind_text(results, 1, post_title, (int)strlen(post_title), SQLITE_TRANSIENT);
    sqlite3_bind_text(results, 2, post_text, (int)strlen(post_text), SQLITE_TRANSIENT);
    
    // Execute statement
    sqlite3_step(results);
    
    // Free statement, close DB
    sqlite3_finalize(results);
    sqlite3_close(db);
    
    return 1;
}

// Used to remove a post
int delete_post(int post_id) {
    /* SQLite variable declarations */
    int rc;
    char sql[200];
    char *zErrMsg = 0;
    
    /* Create SQL statement */
    sprintf(sql, "DELETE FROM posts WHERE post_id = %d", post_id);
    
    /* Connect to DB */
    sqlite3 *db = open_database();
    
    /* Execute SQL statement */
    int success = 0;
    rc = sqlite3_exec(db, sql, NULL, NULL, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }else{
        fprintf(stderr, "Operation done successfully\n");
        success = 1;
    }
    sqlite3_close(db);
    
    return success;
}

// Loads all posts inta a struct *post
// Packs the array of posts in a struct Posts, which is a struct
struct Posts load_posts(int number_of_posts, int offset) {
    
    // Initialize struct to hold posts
    struct Posts all_posts;
    // Initialize pointer to posts as NULL so we can realloc
    all_posts.posts = NULL;
    // Initialize post count to 0
    all_posts.number_of_posts = 0;
    
    /* SQLite variable declarations */
    int rc;
    char sql[300];
    char *zErrMsg = 0;
    
    /* Create SQL statement */
    sprintf(sql, "SELECT title, post_id, text, datetime(time, 'unixepoch') AS time from posts order by time DESC limit %d offset %d", number_of_posts, offset);
    
    /* Connect to DB */
    sqlite3 *db = open_database();
    
    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, &all_posts, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }else{
        fprintf(stderr, "Operation done successfully\n");
    }
    sqlite3_close(db);
    
    return all_posts;
}

// Loads all posts inta a struct *post
// Packs the array of posts in a struct Posts, which is a struct
struct Posts load_posts_monthyear(int month, int year) {
    
    // Initialize struct to hold posts
    struct Posts all_posts;
    // Initialize pointer to posts as NULL so we can realloc
    all_posts.posts = NULL;
    // Initialize post count to 0
    all_posts.number_of_posts = 0;
    
    /* SQLite variable declarations */
    int rc;
    char sql[300];
    char *zErrMsg = 0;
    
    /* Create SQL statement */
    sprintf(sql, "SELECT title, post_id, text, datetime(time, 'unixepoch') AS time from posts WHERE CAST(strftime('%%m',time,'unixepoch') AS INT) = %d AND CAST(strftime('%%Y',time,'unixepoch') AS INT) = %d order by time DESC", month, year);
    
    /* Connect to DB */
    sqlite3 *db = open_database();
    
    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, &all_posts, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }else{
        fprintf(stderr, "Operation done successfully\n");
    }
    sqlite3_close(db);
    
    return all_posts;
}

struct archives load_archives() {
    
    struct archives archives;
    archives.month_s = NULL;
    archives.month = NULL;
    archives.year = NULL;
    archives.post_count = NULL;
    archives.row_count = 0;
    
    /* SQLite variable declarations */
    int rc;
    char *sql;
    char *zErrMsg = 0;
    
    /* Create SQL statement */
    sql = "SELECT strftime('%m',time,'unixepoch') AS month, strftime('%Y',time,'unixepoch') AS year, COUNT(*) AS num_posts FROM posts GROUP BY month, year ORDER BY time DESC";
    
    /* Connect to DB */
    sqlite3 *db = open_database();
    
    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, archives_callback, &archives, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }else{
        fprintf(stderr, "Operation done successfully\n");
    }
    sqlite3_close(db);
    
    return archives;
}

// Given a keyword string, returns all posts where it appears in the body text
struct Posts search_posts(char *keyword) {
    
    // Initialize struct to hold posts
    struct Posts all_posts;
    // Initialize pointer to posts as NULL so we can realloc
    all_posts.posts = NULL;
    // Initialize post count to 0
    all_posts.number_of_posts = 0;
    
    /* SQLite variable declarations */
    int rc;
    char *sql;
    char *zErrMsg = 0;
    
    /* Create SQL statement */
    asprintf(&sql, "SELECT title, post_id, text, datetime(time, 'unixepoch') AS time from posts where lower(text) like lower('%%%s%%') order by time DESC", keyword);
    
    /* Connect to DB */
    sqlite3 *db = open_database();
    
    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, &all_posts, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }else{
        fprintf(stderr, "Operation done successfully\n");
    }
    sqlite3_close(db);
    
    // Free memory allocated for sql query string
    free(sql);
    
    return all_posts;
}

// Loads a single post with a given post_id
struct Post load_post_id(int post_id) {
    
    // Initialize struct to hold posts
    struct Posts all_posts;
    // Initialize pointer to posts as NULL so we can realloc
    all_posts.posts = NULL;
    // Initialize post count to 0
    all_posts.number_of_posts = 0;
    
    /* SQLite variable declarations */
    int rc;
    char sql[200];
    char *zErrMsg = 0;
    
    /* Create SQL statement */
    sprintf(sql, "SELECT title, post_id, text, datetime(time, 'unixepoch') AS time from posts where post_id = %d limit 1", post_id);
    
    /* Connect to DB */
    sqlite3 *db = open_database();
    
    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, &all_posts, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }else{
        fprintf(stderr, "Operation done successfully\n");
    }
    sqlite3_close(db);
    
    // Get first post from returned posts (there should only be one)
    struct Post post = all_posts.posts[0];
    // Free list of posts since we just made a copy
    free(all_posts.posts);
    
    return post;
}

// Return the name of a month given the integer
char *nmonth_to_smonth(int month) {
    switch (month) {
        case 1:
            return "January";
        case 2:
            return "February";
        case 3:
            return "March";
        case 4:
            return "April";
        case 5:
            return "May";
        case 6:
            return "June";
        case 7:
            return "July";
        case 8:
            return "August";
        case 9:
            return "September";
        case 10:
            return "October";
        case 11:
            return "November";
        case 12:
            return "December";
        default:
            return "Unknown";
    }
}

// Given a pointer to a single Post
void free_post(struct Post *post) {
    free(post[0].title);
    free(post[0].text);
    free(post[0].time);
}

// Given a pointer to a Posts struct, frees everything
void free_posts(struct Posts *posts) {
    for (int i = 0; i < posts->number_of_posts; i++) {
        free_post(posts->posts + i);
    }
    free(posts->posts);
}