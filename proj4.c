/*
 * File:	  	proj4.c
 * Date:   		13.12.2011
 * Author:   	Tomas Valek, xvalek02@stud.fit.vutbr.cz
 * Project: 	Czech sorting, project n. 4 IZP subject.
 * Desc.:   	Program reads lines from input file and save sorted output file.
				Sorting is by standard CSN976030. Blank lines are skips.
 * EXTRA:		Usort. Viz. help (-h).
*/

#include <stdio.h>	//standard input/output library functions
#include <string.h> //strcmp
#include <stdlib.h>	//general funciton of language C
#include <stdbool.h>
#include <assert.h>
#include <wchar.h>
#include <locale.h>
#include <stdbool.h>
#include <errno.h>
#include <time.h>	//for testing

#define	CHAR_ch	L'@'
#define CHAR_Ch L'^'
#define CHAR_CH L'_'
#define BULK	32
#define GREATER	1
#define LESS	-1
#define EQUALS	0
#define USORT	2

#define DIAG 0	//enable/disable testing message

const int table1[] = {
//Primary table
//in array on index A is value ...
//L on left side == not 8bit char, but at least 32bit
	[L'\0'] = 0,
	[L' '] = 1,
	[L'A'] = 2, [L'a'] = 2,	/*Á*/ [L'\u00C1'] = 2, /*á*/ [L'\u00E1'] = 2,		
	[L'B'] = 3, [L'b'] = 3,
	[L'C'] = 4, [L'c'] = 4, /*Č*/ [L'\u010C'] = 5, /*č*/ [L'\u010D'] = 5,
	[L'D'] = 6, [L'd'] = 6, /*Ď*/ [L'\u010E'] = 6, /*ď*/ [L'\u010F'] = 6,
	[L'E'] = 7, [L'e'] = 7, /*É*/ [L'\u00C9'] = 7, /*é*/[L'\u00E9'] = 7, 
							/*Ě*/ [L'\u011A'] = 7, /*ě*/[L'\u011B'] = 7,
	[L'F'] = 8, [L'f'] = 8,
	[L'G'] = 9, [L'g'] = 9,
	[L'H'] = 10, [L'h'] = 10,
	[CHAR_Ch] = 11, [CHAR_ch] = 11, [CHAR_CH] = 11,
	[L'I'] = 12, [L'i'] = 12, /*Í*/ [L'\u00CD'] = 12, /*í*/ [L'\u00ED'] = 12,
	[L'J'] = 13, [L'j'] = 13,
	[L'K'] = 14, [L'k'] = 14,
	[L'L'] = 15, [L'l'] = 15,
	[L'M'] = 16, [L'm'] = 16,
	[L'N'] = 17, [L'n'] = 17, /*Ň*/ [L'\u0147'] = 17, /*ň*/ [L'\u0148'] = 17,
	[L'O'] = 18, [L'o'] = 18, /*Ó*/ [L'\u00D3'] = 18, /*ó*/ [L'\u00F3'] = 18,
	[L'P'] = 19, [L'p'] = 19,
	[L'Q'] = 20, [L'q'] = 20,
	[L'R'] = 21, [L'r'] = 21, /*Ř*/ [L'\u0158'] = 22, /*ř*/ [L'\u0159'] = 22,
	[L'S'] = 23, [L's'] = 23, /*Š*/ [L'\u0160'] = 24, /*š*/ [L'\u0161'] = 24,
	[L'T'] = 25, [L't'] = 25, /*Ť*/ [L'\u0164'] = 25, /*ť*/ [L'\u0165'] = 25,
	[L'U'] = 26, [L'u'] = 26, /*Ú*/ [L'\u00DA'] = 26, /*ú*/ [L'\u00FA'] = 26, 
							  /*Ů*/ [L'\u016E'] = 26, /*ů*/ [L'\u016F'] = 26,
	[L'V'] = 27, [L'v'] = 27,
	[L'W'] = 28, [L'w'] = 28,
	[L'X'] = 29, [L'x'] = 29,
	[L'Y'] = 30, [L'y'] = 30, /*Ý*/ [L'\u00DD'] = 30, /*ý*/ [L'\u00FD'] = 30,
	[L'Z'] = 31, [L'z'] = 31, /*Ž*/ [L'\u017D'] = 32, /*ž*/ [L'\u017E'] = 32,
	[L'0'] = 33, [L'1'] = 34, [L'2'] = 35, [L'3'] = 36, [L'4'] = 37,
	[L'5'] = 38, [L'6'] = 39, [L'7'] = 40, [L'8'] = 41, [L'9'] = 42,
	[L'.'] = 43, [L','] = 44, [L';'] = 45, [L'?'] = 46, [L'!'] = 47, [L':'] = 48,
	[L'"'] = 49, [L'-'] = 50, [L'|'] = 51, [L'/'] = 52, [L'\\'] = 53, [L'('] = 54,
	[L')'] = 55, [L'['] = 56, [L']'] = 57, [L'<'] = 58, [L'>'] = 59, [L'{'] = 60,
	[L'}'] = 61, [L'&'] = 62, [L'£'] = 63, [L'§'] = 64, [L'%'] = 65, [L'$'] = 66,
	[L'='] = 67, [L'+'] = 68, [L'×'] = 69, [L'*'] = 70, [L'#'] = 71, [L'~'] = 72,
};

const int table2[] = {
//secondary table
	[L'\0'] = 0,
	[L' '] = 1,
	[L'A'] = 2, [L'a'] = 2,	/*Á*/ [L'\u00C1'] = 3, /*á*/ [L'\u00E1'] = 3,		
	[L'B'] = 4, [L'b'] = 4,
	[L'C'] = 5, [L'c'] = 5, /*Č*/ [L'\u010C'] = 6, /*č*/ [L'\u010D'] = 6,
	[L'D'] = 7, [L'd'] = 7, /*Ď*/ [L'\u010E'] = 8, /*ď*/ [L'\u010F'] = 8,
	[L'E'] = 9, [L'e'] = 9, /*É*/ [L'\u00C9'] = 10, /*é*/[L'\u00E9'] = 10,
							/*Ě*/ [L'\u011A'] = 11, /*ě*/[L'\u011B'] = 11,
	[L'F'] = 12, [L'f'] = 12,
	[L'G'] = 13, [L'g'] = 13,
	[L'H'] = 14, [L'h'] = 14,
	[CHAR_Ch] = 15, [CHAR_ch] = 15, [CHAR_CH] = 15,
	[L'I'] = 16, [L'i'] = 16, /*Í*/ [L'\u00CD'] = 17, /*í*/ [L'\u00ED'] = 17,
	[L'J'] = 18, [L'j'] = 18,
	[L'K'] = 19, [L'k'] = 19,
	[L'L'] = 20, [L'l'] = 20,
	[L'M'] = 21, [L'm'] = 21,
	[L'N'] = 22, [L'n'] = 22, /*Ň*/ [L'\u0147'] = 23, /*ň*/ [L'\u0148'] = 23,
	[L'O'] = 24, [L'o'] = 24, /*Ó*/ [L'\u00D3'] = 25, /*ó*/ [L'\u00F3'] = 25,
	[L'P'] = 26, [L'p'] = 26,
	[L'Q'] = 27, [L'q'] = 27, 
	[L'R'] = 28, [L'r'] = 28, /*Ř*/ [L'\u0158'] = 29, /*ř*/ [L'\u0159'] = 29,
	[L'S'] = 30, [L's'] = 30, /*Š*/ [L'\u0160'] = 31, /*š*/ [L'\u0161'] = 31,
	[L'T'] = 32, [L't'] = 32, /*Ť*/ [L'\u0164'] = 33, /*ť*/ [L'\u0165'] = 33,
	[L'U'] = 34, [L'u'] = 34, /*Ú*/ [L'\u00DA'] = 35, /*ú*/ [L'\u00FA'] = 35,
						  /*Ů*/ [L'\u016E'] = 36, /*ů*/ [L'\u016F'] = 36,
	[L'V'] = 37, [L'v'] = 37,
	[L'W'] = 38, [L'w'] = 38,
	[L'X'] = 39, [L'x'] = 39,
	[L'Y'] = 40, [L'y'] = 40, /*Ý*/ [L'\u00DD'] = 41, /*ý*/ [L'\u00FD'] = 41,
	[L'Z'] = 42, [L'z'] = 42, /*Ž*/ [L'\u017D'] = 43, /*ž*/ [L'\u017E'] = 43,
	[L'0'] = 44, [L'1'] = 45, [L'2'] = 46, [L'3'] = 47, [L'4'] = 48,
	[L'5'] = 49, [L'6'] = 50, [L'7'] = 51, [L'8'] = 52, [L'9'] = 53,
	[L'.'] = 54, [L','] = 55, [L';'] = 56, [L'?'] = 57, [L'!'] = 58, [L':'] = 59,
	[L'"'] = 60, [L'-'] = 61, [L'|'] = 62, [L'/'] = 63, [L'\\'] = 64, [L'('] = 65,
	[L')'] = 66, [L'['] = 67, [L']'] = 68, [L'<'] = 69, [L'>'] = 70, [L'{'] = 71,
	[L'}'] = 72, [L'&'] = 73, [L'£'] = 74, [L'§'] = 75, [L'%'] = 76, [L'$'] = 77,
	[L'='] = 78, [L'+'] = 79, [L'×'] = 80, [L'*'] = 81, [L'#'] = 82, [L'~'] = 83,
};

typedef struct item Titem;

struct item {
	wchar_t* pointerToName;	//pointer to wchar_t array
	Titem* next;			//pointer to next item
};

typedef struct {
	Titem* first;			//pointer to first item in list
} Tlist;

typedef struct {
//struct for string
	wchar_t* string;		//pointer to string
	int size;				//size
	int length;				//length
} Tstring;

//ERROR STATE:
enum ERROR {
	ERROR_PARAM = 0,
	ERROR_MALLOC,
	ERROR_FILE,
	ERROR_LOCALE,
};

char *errorMessages [] = {
	//ERR_PARAM:
	"Params are not valid.\n",
	//ERR_MALLOC
	"Error allocate memory.\n",
	//ERR_FILE
	"Error with file.\n",
	//ERR_LOCALE
	"Setlocale fail.\n",
};


//STATE CODE:
enum enumState {
	ERR_LOCALE = -5,
	ERR = -4,
	ERR_FILE = -3,
	ERR_PARAM = -2,
	ERR_MALLOC = -1,
	PARAM_HELP = 0,
	PARAM_LOCALE,
	PARAM_WITHOUT_LOCALE,
	PARAM_USORT,
	PARAM_LOCALE_USORT,
};

//HELP:
char HELP[] =
"\nProject number 4 -- Czech sorting by CSN976030.\n"
"Author: Tomas Valek.\n"
"Date: 13.12.2011.\n"
"Program reads lines from input file and save sorted output file.\
Sorting is by standard CSN976030. Blank lines are skips.\n"
"Use: -h [--loc LOCALE] input output [--usort] \n"
"Parameter description:\n\
-h For help.\n\
[--loc LOCALE] For charset.\n\
[--usort] For Unique sort.\n"
"./proj4 [--loc LOCALE] input output [--usort] \n";

//PROTOTYPES:
int main( int argc, char* argv[] );
int allocString(Tstring* str);
int addChar( wchar_t character, Tstring* str );
int alignArray(Tstring* str);
int checkParams( int argc, char* argv[] );
int putFirst(Titem item, Tlist* list, Tstring str);
int compare(wchar_t* a, wchar_t* b);
void classItem(Tlist* list, int usort);
int readLine(FILE* fr, Tlist* list, Tstring* str);
int readAndSortList(char file[], Tlist* s, int usort);
void removeList(Tlist* list);
void processError(int what, Tlist* s, FILE* f);
int openFile(FILE** fd, char* name, char* mode);
int closeFile(FILE* fd);
int writeList(char* argv, Tlist* s);

//***********************************FUNCTIONS**********************************

/*Initialize string. Allocate memory size:BULK(32bajtů).
OK == EXIT_SUCCESS or error.*/
int allocString(Tstring* str) {

	//DEBUG
	//str->string = rand() & 0x1 ? NULL : (wchar_t*)malloc(sizeof(wchar_t)*BULK);

	//DEBUG
	//if ( str->string == NULL )
	if ( (str->string = (wchar_t*)malloc(sizeof(wchar_t)*BULK)) == NULL )	{
	//malloc fail
		return ERR_MALLOC;
	}

	str->string[0] = L'\0';
	str->size = BULK;
	str->length = 0;

	return EXIT_SUCCESS;
}

/* Check is it has more memory, and add 1 char to end of string. If program
has low memory, it malloc more.
OK == EXIT_SUCCESS or ERR_MALLOC.*/
int addChar( wchar_t character, Tstring* str ){ 

	if ( str->length + 1 >= str->size  ){ //low memory
#if DIAG
		wprintf(L"Realloc\n");
#endif
		wchar_t* reallocAddress = NULL;

		//DEBUG:
		/*reallocAddress = rand() & 0x1 ? NULL : (wchar_t*)realloc(str->string,\
		sizeof(wchar_t)*str->size*2 );*/

		if ( (reallocAddress = (wchar_t*)realloc(str->string,sizeof(wchar_t)\
				*str->size*2 )) == NULL ) {
		//DEBUG
		//if ( reallocAddress == NULL )
		
			free((void*)str->string);
			str->string = NULL;
			return ERR_MALLOC;
		}

		str->size = str->size*2;			//2x more memory
		str->string = reallocAddress;		//save pointer
	}

	//add char to string
	str->string[str->length] = character;
	str->length++;
	str->string[str->length] = L'\0';

	return EXIT_SUCCESS;
}

/* Align array to specific size.
Return new size or ERR_MALLOC.
*/
int alignArray(Tstring* str) {

	wchar_t* reallocAddress = NULL;

	//DEBUG
	/*reallocAddress = rand() & 0x1 ? NULL : (wchar_t*)realloc(str->string,\
	sizeof(wchar_t)*(str->length+1));*/

	if ( (reallocAddress = (wchar_t*)realloc(str->string,sizeof(wchar_t)\
		*(str->length+1))) == NULL ) {
//	if ( reallocAddress == NULL )
	
		free((void*)str->string);
		str->string = NULL;
		return ERR_MALLOC;
	}

	str->size = str->length;			//change string length
	str->string = reallocAddress;		//save pointer

	return EXIT_SUCCESS;
}

/*Check params.*/
int checkParams( int argc, char* argv[] ){

	if ( argc == 2 ) {
		if ( (strcmp("-h", argv[1])) == 0 )
			return PARAM_HELP;
	}
	else if ( argc == 3 )
		return PARAM_WITHOUT_LOCALE;
	else if ( argc == 5 && ((strcmp("--loc", argv[1])) == 0) )
		return PARAM_LOCALE;
	else if ( argc == 4 && ((strcmp("--usort", argv[3])) == 0) )
		return PARAM_USORT;
	else if ( argc == 6 && ((strcmp("--loc", argv[1])) == 0) &&\
			 ((strcmp("--usort", argv[5])) == 0) )
		return PARAM_LOCALE_USORT;

	return ERR_PARAM;
}

/* Allocate memory for first item and put to list.
OK == EXIT_SUCCESS or error.*/
int putFirst(Titem item, Tlist* list, Tstring str) {

	assert( list != NULL );

	Titem* newItem;

	//DEBUG:
	//newItem = rand() & 0x1 ? NULL : malloc(sizeof(Titem));

	if ( (newItem = malloc(sizeof(Titem))) == NULL ){
	//DEBUG:
	//if ( newItem == NULL )
		return ERR_MALLOC;
	}

	//to created place in memory (size for item) add item:
	*newItem = item;
	newItem->next = list->first;
	//list's header pointer to actual item:
	list->first = newItem;
	newItem->pointerToName = str.string;

	return EXIT_SUCCESS;
}

/* Compare two characters.
If a > b, return GREATER,
If a < b, return LESS,
If a == b return EQUALS.*/
int compare(wchar_t* a, wchar_t* b){

	int i = 0;
	int j = 0;
	int space_a = 0;
	int space_b = 0;
	int iterate = 1;

	while( iterate <= 2 ) { //for first and second iterate
	
		while( a[i] != L'\0' && b[j] != L'\0' ) { //Compare strings until one of them ends
		
			//FIX SPACES FOR A:
			if ( a[i] == L' ' ) { //char is space
				++space_a;
				if ( space_a > 1 )	{ //more chars are spaces
					while( a[i] == L' ' )
						++i; //skip spaces
					space_a = 0;
					if ( a[i] == '\0' )
						break;
				}
			} else {
				space_a = 0;
			}

			//FIX SPACES FOR B:
			if ( b[j] == L' ' ) {
				++space_b;
				if ( space_b > 1 ) {
					while( b[j] == L' ' )
						++j;
					space_b = 0;
					if ( b[j] == '\0' )
						break;
				}
			} else
				space_b = 0;
			//end of skip space

			if ( iterate == 1 ) { //ITERATE 1
				if ( table1[a[i]] > table1[b[j]] )
					return GREATER; //first char is greater then second
				else if ( table1[a[i]] < table1[b[j]] ) 
					return LESS; 	//first char is less then second
				else {	//characters are equals
					i++;
					j++;
				}
			} else { //ITERATE 2
				if ( table2[a[i]] > table2[b[j]] )
					return GREATER;
				else if ( table2[a[i]] < table2[b[j]] ) 
					return LESS;
				else { //characters are equals
					i++;
					j++;
				}
			}
		}

		if ( iterate == 1 ) {//For example for: Ábel, Abelard decides to length string
			if ( a[i] == L'\0' && b[j] != L'\0' )
				return LESS;
			else if ( a[i] != L'\0' && b[j] == L'\0' )
				return GREATER;
		}

		++iterate;

		//Reset variables
		i = 0;
		j = 0;
		space_a = 0;
		space_b = 0;
	}

	return EQUALS;
}

/*Place new item to correct place in list.*/
void classItem(Tlist* list, int usort){

	Titem* a = NULL;
	Titem* b = NULL;
	Titem* c = NULL;

	bool changeFlag = true;
	int state = 0;

	while ( changeFlag == true ) {//if change is true -> check, whether it is something else to exchange
		a = list->first;

		if ( a->next == NULL )
			return;	//list has only one item, nothing to exchange

		//ONLY FOR FIRST TWO'S NAMES:
		changeFlag = false;
		state = 0;

		a = list->first->next;

		state = compare(list->first->pointerToName, a->pointerToName);

#if DIAG
		wprintf(L"----------------------------------------------------------\n");
		wprintf(L"ITERATE 1\n");
		wprintf(L"list->first->pointerToName:\t%ls\n",list->first->pointerToName);
		wprintf(L"a->pointerToName:\t\t\t%ls\n",a->pointerToName);
		wprintf(L"Result equals %d\n", state);
		wprintf(L"----------------------------------------------------------\n");
#endif

		//If usort == true and items are equals, first item will be deleted
		if ( usort == USORT && state == EQUALS ) {

			Titem* tmp = NULL;
			tmp = list->first;

			list->first = list->first->next;

			free(tmp->pointerToName);
			free(tmp);
		} else if ( state == GREATER ) {
			//first is greater than second item -> exchange it. Sorting is from less to great.
#if DIAG
			wprintf(L"Changing\n");
#endif
			changeFlag = true;

			//edit pointers
			list->first->next = a->next;
			a->next = list->first;
			list->first = a;
		}

#if DIAG
		wprintf(L"-------------------------\n");
		for ( Titem* tmp = list->first; tmp != NULL; tmp = tmp->next) {
			//Show list
			wprintf(L"%ls\n", tmp->pointerToName);
		}
		wprintf(L"-------------------------\n");
#endif

		if ( usort == USORT && a->next == NULL ) //usort
			return;	//nothing to sort, we have only one item

		//set pointer to next item
		a = list->first;
		b = a->next;
		c = b->next;

		while( c != NULL ) { //check with other list

			state = 0;
			state = compare(b->pointerToName, c->pointerToName);

#if DIAG
		wprintf(L"----------------------------------------------------------\n");
		wprintf(L"ITERATE2\n");
		wprintf(L"list->first->pointerToName:\t%ls\n",list->first->pointerToName);
		wprintf(L"a->pointerToName:\t\t\t%ls\n",a->pointerToName);
		wprintf(L"Result equals %d\n", state);
		wprintf(L"----------------------------------------------------------\n");
#endif			

			//If usort == true and items are equals, first item will be deleted
			if ( usort == USORT && state == EQUALS ) {

				Titem* tmp = NULL;
				tmp = b;

				a->next = b->next;

				free(tmp->pointerToName);
				free(tmp);

				a = a->next;
				b = c->next;
				if ( b == NULL )//usort
					return;
				c = c->next->next;
			} else if ( state == GREATER ) {
			//first is greater than second item -> exchange it. Sorting is from less to great.
#if DIAG
				wprintf(L"Changing\n");
#endif
				changeFlag = true;

				b->next = c->next;
				c->next = b;
				a->next = c;
				
				c = b->next;
				a = a->next;
			} else { //first is less than second item
		  		//set pointer to next item
				a = a->next;
				b = b->next;
				c = c->next;
			}
		}	
	}
}

/* Read one line to wchar array. Specifically monitors characters: "CH" and "ch".
Return 0 == EXIT_SUCCESS or ERR_MALLOC.*/
int readLine(FILE* fr, Tlist* list, Tstring* str){

	wint_t character;
	int state = 0;
	bool flag_c = false;
	bool flag_C = false;
	int error;

	state = allocString(str);

	if ( state == ERR_MALLOC )	{
		processError(ERROR_MALLOC, list, fr);
		return ERR;
	}

	while ( (character = fgetwc(fr)) != L'\n' ) { //iterate throught one line in file

		error = errno;

		if ( error == EILSEQ ) {
			free(str->string);
			processError(ERR, list, fr);
			perror("Error function fgetwc.");
			return ERR;
		}

		if ( character == WEOF ){		//end of file
			free(str->string);
			return EOF;
		} else if ( character == L'c' ) {// c
			flag_c = true;
			continue;
		} else if ( character == L'C' ) {// C
			flag_C = true;
			continue;
		}

		if ( flag_c == true && character == L'h' ) {// ch
			character = CHAR_ch;
			flag_c = false;
		} else if ( flag_C == true ) {// C
			if ( character == L'h' )	{// Ch
				character = CHAR_Ch;
				flag_C = false;
			} else if ( character == L'H' ) {// CH
				character = CHAR_CH;
				flag_C = false;
			}
		}

		state = 0;

		/* If one of Flags is true, it meanings that behind 'c' or 'C' not comming 'h'.
		This means, put 'c' or 'C' and next character after 'c' or 'C'.*/
		if ( flag_c == true ) {
			state = addChar(L'c', str);
			flag_c = false;
		} else if ( flag_C == true ) {
			state = addChar(L'C', str);
			flag_C = false;
		}
		
		state = addChar(character, str);

		if ( state == ERR_MALLOC ) {
			processError(ERROR_MALLOC, list, fr);
			return ERR;
		}
	}

	/*Line is end, but 'c' or 'C' are still not write, because algorithm waiting
	for 'h' or 'H'.*/
	if ( flag_c == true )
		state = addChar(L'c', str);
	else if ( flag_C == true )
		state = addChar(L'C', str);

	state = 0;

	if ( state == ERR_MALLOC ) {
		processError(ERROR_MALLOC, list, fr);
		return ERR;
	}
#if DIAG
	wprintf(L"Allocate %d, length array %d\n", str->size, str->length);
#endif
	state = 0;
	state = alignArray(str);
#if DIAG
	wprintf(L"NEW: \tAllocate %d, length array %d\n", str->size, str->length);
#endif

	if ( state == ERR_MALLOC ) {
		processError(ERROR_MALLOC, list, fr);
		return ERR;
	}

	return EXIT_SUCCESS;
}

/*Wrapping function.*/
int readAndSortList(char file[], Tlist* s, int usort){

	FILE* fr;
	Titem item;
	Tstring str;

	int returnValue = 0;

	if ( (openFile(&fr,file,"r")) == ERR_FILE ) {
		processError(ERROR_FILE, NULL, NULL);
		return ERR;
	}

	while( returnValue != EOF ) {
		returnValue = readLine(fr, s, &str);

		if ( returnValue == ERR )
			return ERR;
		else if ( returnValue == EOF )
			break;

		returnValue = 0;
		returnValue = putFirst(item, s, str);

		if ( returnValue == ERR_MALLOC ) {
			free(str.string);
			processError(ERROR_MALLOC, s, fr);
			return ERR;
		}

		if ( usort == USORT )
			classItem(s,USORT);
		else
			classItem(s,0);
	}
	
	if ( (closeFile(fr)) == ERR_FILE ) {
		processError(ERROR_FILE, NULL, fr);
		return ERR;
	}

	return EXIT_SUCCESS;
}

/* Remove list and free all alocations. First is release one-dimensional array
which pointer item->pointerToName and after release item.*/
void removeList(Tlist* list){

	if ( list->first == NULL || list == NULL )
		return; 

	assert( list != NULL );
	assert( list->first != NULL ); //must be first

	while( list->first != NULL ) {//until iterate throught whole list

		Titem* item;

		/*edit pointers; remove first item, so header will be point to there,
		where point removing(first) item*/
		item = list->first;
		list->first = item->next;
#if DIAG
		wprintf(L"Free: %ls\n",item->pointerToName );
#endif
		free(item->pointerToName); 	//free allocated line
		free(item);					//free item from list
	}
}

/*Process error -> free memory -> close file descriptor.*/
void processError(int what, Tlist* s, FILE* f) {

	switch ( what ) {
		case ERR:
		case ERR_MALLOC:
			if ( s->first != NULL )
				removeList(s);

		case ERR_FILE:
			if ( f != NULL )
				fclose(f);
	}

	//write error to stdout:
	if ( what == ERR_MALLOC )
		fprintf(stderr, "%s",errorMessages[ERROR_MALLOC]);
	else if ( what == ERR_FILE )
		fprintf(stderr, "%s",errorMessages[ERROR_FILE]);
}

int openFile(FILE** fd, char* name, char* mode){

	if ( (*fd = fopen(name, mode)) == NULL )
		return ERR_FILE;
	return EXIT_SUCCESS;
}

int closeFile(FILE* fd){

	if ( (fclose(fd)) == EOF)
		return ERR_FILE;

	return EXIT_SUCCESS;
}

/*Write list with prevent with "Ch" and "ch".*/
int writeList(char* argv, Tlist* s){

#if DIAG
	wprintf(L"Writing list\n");
#endif

	FILE* fw;
	int i = 0;

	if ( (fw = fopen(argv, "w")) == NULL ) {
		processError(ERROR_FILE, NULL, fw);
		return ERR;
	}

	for ( Titem* pom = s->first; pom != NULL; pom = pom->next ) { 

		i = 0;

		while ( *(pom->pointerToName+i) != '\0' )	{
		//iterate throught lines and write throught characters
			if ( *(pom->pointerToName+i) == CHAR_ch ) //ch
				fwprintf(fw, L"ch");
			else if ( *(pom->pointerToName+i) == CHAR_Ch ) //Ch
				fwprintf(fw, L"Ch");
			else if ( *(pom->pointerToName+i) == CHAR_CH ) //CH
				fwprintf(fw, L"CH");
			else
				fwprintf(fw, L"%lc", *(pom->pointerToName+i));
			i++;
		}
		fwprintf(fw, L"\n");
	}

	if ( (closeFile(fw)) == ERR_FILE ) {
		processError(ERROR_FILE, NULL, fw);
		return ERR;
	}

	return EXIT_SUCCESS;

}

//**************************************MAIN***********************************/

int main( int argc, char* argv[] ){

	//DEBUG:
	//srand(time(NULL));

	int stateParam; //return value

	stateParam = checkParams(argc, argv);

	if ( stateParam == ERR_PARAM ) { //Params fail
		fprintf(stderr,"%s", errorMessages[ERROR_PARAM]);
		printf("%s", HELP);
		return EXIT_FAILURE;
	} else if ( stateParam == PARAM_HELP ){ //Help
		printf("%s", HELP);
		return EXIT_SUCCESS;
	} else {
		int state = 0;
		Tlist s;
		s.first = NULL;

		if ( stateParam == PARAM_LOCALE || stateParam == PARAM_LOCALE_USORT ) {
			if ( (setlocale(LC_CTYPE,argv[2])) == NULL ) {//setlocale fail
				fprintf(stderr,"%s", errorMessages[ERROR_LOCALE]);
				return EXIT_FAILURE;
			}

			if ( stateParam == PARAM_LOCALE_USORT )
				state = readAndSortList(argv[3], &s, USORT);
			else
				state = readAndSortList(argv[3], &s, 0);
		} else if ( stateParam == PARAM_WITHOUT_LOCALE || stateParam == PARAM_USORT ) {
			if ( (setlocale(LC_CTYPE,"")) == NULL )	{//setlocale fail
				fprintf(stderr,"%s", errorMessages[ERROR_LOCALE]);
				return EXIT_FAILURE;
			}

			if ( stateParam == PARAM_USORT )
				state = readAndSortList(argv[1], &s, USORT);
			else
				state = readAndSortList(argv[1], &s, 0);
		}

		if ( state == ERR )
			return EXIT_FAILURE;

#if DIAG //show list
		for ( Titem* tmp = s.first; tmp != NULL; tmp = tmp->next )
			wprintf(L"Show list(from main): %ls\n", tmp->pointerToName);
#endif

		if ( stateParam == PARAM_LOCALE || stateParam == PARAM_LOCALE_USORT )
			writeList(argv[4], &s);
		else
			writeList(argv[2], &s);

		removeList(&s);

	}

	return EXIT_SUCCESS;
}
