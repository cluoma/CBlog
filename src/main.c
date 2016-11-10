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
#include "cblog.h"
#include "contact.h"

void print_page(char *page, char *env_string)
{
    if (page == NULL || (strcmp(page, "cblog") == 0))
    {
        print_cblog(env_string);
    } else if (strcmp(page, "contact") == 0)
    {
        print_contact();
    }
}

int main()
{
    /* Get all required environment variables */
    char *request_method = GET_ENV_VAR("REQUEST_METHOD");
    char *script_name = GET_ENV_VAR("SCRIPT_NAME");
    char *query_string = GET_ENV_VAR("QUERY_STRING");

    char *post_data;
    size_t content_length;
    if (EOF == sscanf(getenv("CONTENT_LENGTH"), "%zu", &content_length))
    {
        content_length = 0;
    }
    else
    {
        post_data = malloc(content_length);
        memset(post_data, 0, content_length+1);
        fgets(post_data, (int)content_length, stdin);
    }

    /* Parse query string */
    query_vars query;
    parse_query_string(query_string, &query);

    /* Mandatory HTML info and navbar, etc. */
    init_page(query.page, script_name, query_string);

    printf("page: '%s'\nstart: '%s'\nend: '%s'\nsearch: '%s'\n",
            query.page, query.start, query.end, query.search);
    return 0;

    /* Google analytics script */
    include_google_analytics();

    print_page(query.page, query_string);

    printf("</body>"); // End HTML body, is opened in init_page()

    /* Print page footer, copyright, name, etc. */
    print_footer();

    printf("</html>");
}
