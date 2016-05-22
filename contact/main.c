//
//  main.c
//  CBlog
//
//  Created by Colin Luoma on 2016-05-05.
//  Copyright (c) 2016 Colin Luoma. All rights reserved.
//
//  A page with information about me and my ongoing projects
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "base_html.h"

// Return maximum value of a or b
int max(int a, int b) {
    if (a > b)
        return a;
    else if (a < b)
        return b;
    else
        return a;
}

// Displays a panel with information about this blog
void print_about_panel() {
    print_panel("About This Blog", "This blog is intended as written documentation of my ongoing side-projects. Most everything here is likely to be programming or data related, although something else may creep in every now and then. The blog itself is an example of a personal project of mine. It was written entirely from scratch using HTML, C, Twitter's Bootstrap CSS, and SQLite3.", NULL, NULL);
}

// Displays a panel with links to ongoing projects
void print_ongoing_projects() {
    
    char *title = "Current Projects";
    
    char *intro = "Below are some of my projects that I am currently working on:<br><br>";
    
    // This blog
    char *this_blog = "<b><a href=\"http://new.cluoma.com\">This website</a></b><br><br>";
    
    // Haloviewer
    char *haloviewer = "<b><a href=\"http://haloviewer.bluewizard.ca\">Halo Viewer:</a></b> A website written in C++ using the Wt web framework intended to provide player statistics from the video game Halo 5<br><br>";
    
    // Halo 5 API
    char *h5api = "<b><a href=\"http://github.com/cluoma/h5api\">Halo 5 API R Package:</a></b> An R package with quick functions to access data from 343i's Halo 5 web API.<br><br>";
    
    // BlueWizard
    char *bluewizard = "<b><a href=\"http://bluewizard.ca\">Blue Wizard Needs Food Badly:</a></b> My video games blog. Most of the content there has been mirrored here lately but anything that is strictly games related goes there.<br><br>";
    
    // My github
    char *github = "<b><a href=\"http://github.com/cluoma\">My Github:</a></b> A link to my Github page. I'm fairly new to github so there isn't much there. The Halo Viewer code should be added soon as well as the code for this page.";
    
    // Concat text
    size_t length = strlen(intro) + strlen(this_blog) + strlen(haloviewer) +
                    strlen(h5api) + strlen(bluewizard) + strlen(github) + 1;
    char *text = malloc(sizeof(char) * length);
    snprintf(text, length, "%s%s%s%s%s%s%c", intro, this_blog, haloviewer, h5api, bluewizard, github, '\0');
    
    // Print panel
    print_panel(title, text, NULL, NULL);
    
    // Free concatenated text
    free(text);
}

// Displays a panel with email contact information
void print_contact() {
    print_panel("Contact Info", "Feel free to get in contact with me:<br><br>Email: <a href=\"mailto:cluoma@ualberta.ca\">cluoma@ualberta.ca</a> or <a href=\"mailto:themjolnir999@gmail.com\">themjolnir999@gmail.com</a>", NULL, NULL);
}

int main(int argc, const char * argv[]) {
    
    /* Mandatory HTML info and navbar, etc. */
    init_page("contact");
    
    printf("<div class=\"container\" width=\"80%%\">"); // Start container that holds everything
    
    // Show all panels
    //print_about_panel();
    print_ongoing_projects();
    print_contact();
    
    printf("</div>"); // Close container
    
    printf("</body>"); // Close HTML body, opened in init_page()
    
    /* Print page footer, copyright, name, etc. */
    print_footer();
    
    return 0;
}
