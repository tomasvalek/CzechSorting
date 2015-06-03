# Project:	proj4.c
# Author:	Tomas Valek
# Date:   	30.10.2011
# 

NAME = proj4

# compiler of C language:
CC=gcc
# params
CFLAGS=-std=c99 -Wextra -Wall -pedantic -O -g

all: $(NAME)

.PHONY: clean
.PHONY: pack

$(NAME): $(NAME).c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f *.o $(NAME) $(NAME).zip

pack:
	zip $(NAME).zip $(NAME).c Makefile README.md LICENSE
