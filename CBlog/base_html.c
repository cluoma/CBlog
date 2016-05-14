//
//  base_html.c
//  CBlog
//
//  Created by Colin Luoma on 2016-05-14.
//  Copyright (c) 2016 Colin Luoma. All rights reserved.
//
//  A collection of functions that are useful/necessary
//  when working with CGI and web stuff
//
//

#include "base_html.h"

// printf mandatory CGI data and layout html
void init_page() {
    char *env_string = getenv("QUERY_STRING");
    unsigned long env_length = 0;
    
    printf("Context-Type: text/html\n\n");
    
    const char *base_page = "<!DOCTYPE html><html><head><meta charset=\"utf-8\"><meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"><meta name=\"description\" content=\"\"><meta name=\"author\" content=\"\"><title>Colin Luoma's Blog</title><!-- jQuery Version 1.11.1 --><script src=\"/js/jquery.js\"></script><!-- Bootstrap Core JavaScript --><script src=\"/js/bootstrap.min.js\"></script><!-- Bootstrap Core CSS --><link href=\"/css/bootstrap.min.css\" rel=\"stylesheet\"><!-- Custom CSS --><style>body {padding-top: 70px; background-color: #bbbbbb;}</style><link rel=\"shortcut icon\" type=\"image/png\" href=\"/images/favicon.png\"/></head><body><!-- Navigation --><nav class=\"navbar navbar-inverse navbar-fixed-top\" role=\"navigation\"><div class=\"container\"><!-- Brand and toggle get grouped for better mobile display --><div class=\"navbar-header\"><button type=\"button\" class=\"navbar-toggle\" data-toggle=\"collapse\" data-target=\"#bs-example-navbar-collapse-1\"><span class=\"sr-only\">Toggle navigation</span><span class=\"icon-bar\"></span><span class=\"icon-bar\"></span><span class=\"icon-bar\"></span></button><a class=\"navbar-brand\" href=\"/cgi-bin/cblog.cgi\">Colin Luoma</a></div><!-- Collect the nav links, forms, and other content for toggling --><div class=\"collapse navbar-collapse\" id=\"bs-example-navbar-collapse-1\"><ul class=\"nav navbar-nav\"><li><a href=\"/cgi-bin/cblog.cgi?\">Blog</a></li><li><a href=\"/cgi-bin/contact.cgi?\">Contact</a></li></ul><form action=\"/cgi-bin/cblog.cgi\" method=\"get\" class=\"navbar-form navbar-right\" role=\"search\"><div class=\"input-group\"><input type=\"text\" name=\"search\" class=\"form-control\" placeholder=\"Search\"><span class=\"input-group-btn\"><button type=\"submit\" class=\"btn btn-default\"><span class=\"glyphicon glyphicon-search\"></span></button></span></input></div></form></div><!-- /.navbar-collapse --></div><!-- /.container --></nav>";
    
    if (env_string != NULL) {
        env_length = strlen(env_string);
        char *prop = malloc(strlen(base_page) + env_length*2 + 1);
        sprintf(prop, "<!DOCTYPE html><html><head><meta charset=\"utf-8\"><meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"><meta name=\"description\" content=\"\"><meta name=\"author\" content=\"\"><title>Colin Luoma's Blog</title><!-- jQuery Version 1.11.1 --><script src=\"/js/jquery.js\"></script><!-- Bootstrap Core JavaScript --><script src=\"/js/bootstrap.min.js\"></script><!-- Bootstrap Core CSS --><link href=\"/css/bootstrap.min.css\" rel=\"stylesheet\"><!-- Custom CSS --><style>body {padding-top: 70px; background-color: #bbbbbb;}</style><link rel=\"shortcut icon\" type=\"image/png\" href=\"/images/favicon.png\"/></head><body><!-- Navigation --><nav class=\"navbar navbar-inverse navbar-fixed-top\" role=\"navigation\"><div class=\"container\"><!-- Brand and toggle get grouped for better mobile display --><div class=\"navbar-header\"><button type=\"button\" class=\"navbar-toggle\" data-toggle=\"collapse\" data-target=\"#bs-example-navbar-collapse-1\"><span class=\"sr-only\">Toggle navigation</span><span class=\"icon-bar\"></span><span class=\"icon-bar\"></span><span class=\"icon-bar\"></span></button><a class=\"navbar-brand\" href=\"/cgi-bin/cblog.cgi\">Colin Luoma</a></div><!-- Collect the nav links, forms, and other content for toggling --><div class=\"collapse navbar-collapse\" id=\"bs-example-navbar-collapse-1\"><ul class=\"nav navbar-nav\"><li><a href=\"/cgi-bin/cblog.cgi?%s\">Blog</a></li><li><a href=\"/cgi-bin/contact.cgi?%s\">Contact</a></li></ul><form action=\"/cgi-bin/cblog.cgi\" method=\"get\" class=\"navbar-form navbar-right\" role=\"search\"><div class=\"input-group\"><input type=\"text\" name=\"search\" class=\"form-control\" placeholder=\"Search\"><span class=\"input-group-btn\"><button type=\"submit\" class=\"btn btn-default\"><span class=\"glyphicon glyphicon-search\"></span></button></span></input></div></form></div><!-- /.navbar-collapse --></div><!-- /.container --></nav>", env_string, env_string);
        prop[strlen(base_page) + env_length*2] = '\0';
        printf("%s", prop);
        free(prop);
    } else {
        printf("%s", base_page);
    }
}

// Prints page footer, copyright, name, etc
void print_footer() {
    printf("<footer style=\"text-align: center\">Copyright 2016 Colin Luoma<br>");
    printf("Made with C and Twitter Bootstrap<br><3");
    printf("</footer>");
}

// Returns a new string with all '\n' characters replaced by "<br>" for use in HTML
char *newline_to_html(const char* string) {
    
    if (string == NULL) return NULL;
    
    int newlines = 0;
    for (int i = 0; i < strlen(string); i++) {
        if (string[i] == '\n')
            newlines++;
    }
    
    // Allocate space for new string with enough room for the <br>'s
    char *new_string = malloc(strlen(string) + newlines*3 + 1);
    
    int new_index = 0;
    for (int old_index = 0; old_index < strlen(string); old_index++) {
        if (string[old_index] == '\n')
        {
            new_string[new_index] = '<';
            new_index++;
            new_string[new_index] = 'b';
            new_index++;
            new_string[new_index] = 'r';
            new_index++;
            new_string[new_index] = '>';
        }
        else
        {
            new_string[new_index] = string[old_index];
        }
        new_index++;
    }
    new_string[new_index] = '\0';
    return new_string;
}

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
    
    // Return NULL if source is empty
    if (source == NULL) return NULL;
    
    char *tmp = strstr(source, var); // Find variable
    
    // Return NULL if string not found
    if (tmp == NULL) return tmp;
    
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

/*
 *  Bootstrap Elements: Panels, Buttons, etc.
 */

// Given a title and body text (and optional time), displays a bootstrap panel
// Argument extra is rendered as HTML
void print_panel(const char* title, const char* text, const char* time, const char* extra) {
    /* Panel */
    printf("<div class=\"panel panel-default\" style=\"border-color: black\">");
    
    /* Panel Heading */
    printf("<div class=\"panel-heading\" style=\"background-color: #222; color: #66bbdd\">%s</div>", title);
    
    /* Panel Body */
    printf("<div class=\"panel-body\">");
    
    /* Display body text if not NULL */
    char *formatted_post_text = newline_to_html(text);
    if (formatted_post_text != NULL)
        printf("<p>%s</p>", formatted_post_text);
    free(formatted_post_text); // Free temporary string
    
    /* Display time if not NULL */
    if (time != NULL) {
        printf("<p>Date Posted: %s</p>", time);
    }
    
    /* Print extra stuff as html if not null */
    if (extra != NULL) {
        printf("%s", extra);
    }
    
    /* Close Body */
    printf("</div>");
    
    /* Close Panel */
    printf("</div>");
}
