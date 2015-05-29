# Projekt: proj4.c
# Autor:   Tomas Valek
# Datum:   30.10.2011
# 
 
CC=gcc							  				# prekladac jazyka C
CFLAGS=-std=c99 -Wextra -Wall -pedantic -O -g	# parametry prekladace

proj4: proj4.c
	$(CC) $(CFLAGS) proj4.c -o proj4
