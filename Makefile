all: cblog contact

cblog:
	gcc -o cblog.cgi -Wall -std=c99 src/cblog.c src/base_html.c src/load_posts.c -lsqlite3

contact:
	gcc -o contact.cgi -Wall -std=c99 src/contact.c src/base_html.c -lsqlite3
