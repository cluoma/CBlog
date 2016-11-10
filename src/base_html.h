//
//  base_html.h
//  CBlog
//
//  Created by Colin Luoma on 2016-05-14.
//  Copyright (c) 2016 Colin Luoma. All rights reserved.
//

#ifndef __CBlog__base_html__
#define __CBlog__base_html__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GET_ENV_VAR(X) ( (getenv(X) == NULL) ? "" : getenv(X) )

typedef struct query_vars {
    char *page;
    char *start;
    char *end;
    char *search;
} query_vars;

// Print HTML header info, loads css js, and navbar
void
init_page(const char* page_name, const char *script_name, const char* query_string);

// Prints page footer, copyright, name, etc
void
print_footer();

// Prints google analytics scripts
void
include_google_analytics();

// Returns a new string with newlines replaces by <br>
char *
newline_to_html(const char* string);

// Given a title and body text (and optional time), displays a bootstrap panel
// Argument extra is rendered as HTML
void
print_panel(const char* title, const char* text, const char* time, const char* extra);

void
print_blog_post(const char* title, const char* text, const char* time, const char* extra);

// Decodes URL strings to text (eg '+' -> ' ' and % hex codes)
void
html_to_text(char *source, char *dest);

// Must include '=' after variable name
// Malloc is used, up to user to free returned string
// Doesn't check beginning of variable (eg post_title= could match against my_post_title=)
char *
get_variable(char *source, char *var);

void
parse_query_string(char *query_string, query_vars *query);

#endif /* defined(__CBlog__base_html__) */
