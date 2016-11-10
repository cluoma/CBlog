all: cblog

cblog:
	gcc -o cblog.cgi -Wall -std=c99 src/main.c src/cblog.c src/contact.c src/base_html.c src/load_posts.c -lsqlite3
