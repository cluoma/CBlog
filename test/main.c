//
//  main.c
//  test
//
//  Created by Colin Luoma on 2016-05-15.
//  Copyright (c) 2016 Colin Luoma. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

struct archives {
    char **month;
    int *year;
    int *post_count;
    int row_count;
};

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

static int archives_callback(void *data, int argc, char **argv, char **azColName){
    // Recast void *data to struct
    struct archives *archives = data;
    
    // Realloc memory for a new row
    char **month = (char **) realloc(archives->month, sizeof(char *)*(1+archives->row_count));
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
            month[row_count] = malloc(strlen(tmp) + 1);
            strcpy(month[row_count], tmp);
            month[row_count][strlen(tmp)] = '\0';
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
    archives->month = month;
    archives->year = year;
    archives->post_count = post_count;
    archives->row_count = row_count + 1;
    
    return 0;
}

int main(int argc, const char * argv[]) {
    
//    printf("Context-Type: text/plaintext\n\n");
//    
//    
//    
//    printf("REMOTE_HOST: %s\n", getenv("REMOTE_HOST"));
//    printf("REMOTE_ADDR: %s\n", getenv("REMOTE_ADDR"));
//    printf("AUTH_TYPE: %s\n", getenv("AUTH_TYPE"));
//    printf("REMOTE_USER: %s\n", getenv("REMOTE_USER"));
//    printf("REMOTE_IDENT: %s\n", getenv("REMOTE_IDENT"));
//    printf("CONTENT_TYPE: %s\n", getenv("CONTENT_TYPE"));
//    printf("CONTENT_LENGTH: %s\n", getenv("CONTENT_LENGTH"));
//    printf("HTTP_FROM: %s\n", getenv("HTTP_FROM"));
//    printf("HTTP_ACCEPT: %s\n", getenv("HTTP_ACCEPT"));
//    printf("HTTP_USER_AGENT: %s\n", getenv("HTTP_USER_AGENT"));
//    printf("HTTP_REFERER: %s\n", getenv("HTTP_REFERER"));
    
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    char *sql;
    //const char* data = "Callback function called";
    char *db_location = "/Users/colin/Documents/CBlog/server/personl_blog.db";
    
    struct archives archives;
    archives.month = NULL;
    archives.year = NULL;
    archives.post_count = NULL;
    archives.row_count = 0;
    
    
    /* Open database */
    rc = sqlite3_open(db_location, &db);
    if( rc ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        exit(0);
    }else{
        fprintf(stderr, "Opened database successfully\n");
    }
    
    /* Create SQL statement */
    sql = "SELECT strftime('%m',time,'unixepoch') AS month, strftime('%Y',time,'unixepoch') AS year, COUNT(*) AS num_posts FROM posts WHERE CAST(strftime('%m',time,'unixepoch') AS INT) = 5 GROUP BY month, year ORDER BY time DESC";
    
    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, archives_callback, &archives, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }else{
        //fprintf(stdout, "Operation done successfully\n");
    }
    sqlite3_close(db);
    
    
    // Print post months
    for (int i = 0; i < archives.row_count; i++) {
        printf("%s %d (%d)\n", archives.month[i], archives.year[i], archives.post_count[i]);
        free(archives.month[i]);
    }
    free(archives.month);
    free(archives.year);
    free(archives.post_count);
    
    return 0;
}
