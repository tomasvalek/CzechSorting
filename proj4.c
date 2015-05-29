/*
 * Soubor:  	seznam.c
 * Datum:   	13.12.2011
 * Autor:   	Tomas Valek, xvalek02@stud.fit.vutbr.cz
 * Projekt: 	Ceske razeni, projekt c. 4 pro predmet IZP
 * Popis:   	Program cte radky ze vstupniho souboru a serazene je zapise do 
				vystupniho soboru. Razeni je provedeno podle normy CSN976030.
 * Podprogramy: Soucasti programu je 15 podprogramu.
 * EXTRA:		Usort. Viz. napoveda.
*/

#include <stdio.h>	//vstup, vystup
#include <string.h> //strcmp
#include <stdlib.h>	//obecne funkce jazyka C, EXIT_SUCCESS a EXIT_FAILURE
#include <stdbool.h>
#include <assert.h>
#include <wchar.h>
#include <locale.h>
#include <stdbool.h>
#include <errno.h>
#include <time.h>	//pro testovaní u alokaci

#define	ZNAK_ch	L'@'
#define ZNAK_Ch L'^'
#define ZNAK_CH L'_'
#define DAVKA	32
#define VETSI	1
#define MENSI	-1
#define ROVNO	0
#define USORT	2

#define DIAG 0	//slouzi pro zapnuti diagnostickych vypisu

const int tabulka1[] = {
//Primarni tabulka
//v poli na indexu A je takova hodnota
//to L co dáme před 'a' nam rika, ze to nebude 8bitovy znak, ale siroky znak
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
	[ZNAK_Ch] = 11, [ZNAK_ch] = 11, [ZNAK_CH] = 11,
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

const int tabulka2[] = {
//Sekundarni tabulka
//v poli na indexu A je takova hodnota
//to L co dáme před 'a' nam rika, ze to nebude 8bitovy znak, ale siroky znak
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
	[ZNAK_Ch] = 15, [ZNAK_ch] = 15, [ZNAK_CH] = 15,
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

typedef struct polozka Tpolozka;

struct polozka {
	wchar_t* uk_na_jmeno;	//ukazatel do na jednorozmerne pole wcharu
	Tpolozka* nasledujici;	//nasledujici je ukazatel na dalsi polozku
};

typedef struct {
	Tpolozka* prvni;	//ukazatel seznamu na prvni polozku v seznamu
} Tseznam;

typedef struct {
//Struktura pro retezec
	wchar_t* retezec;
	int alok_vel;
	int delka;
}Tstring;

//CHYBOVE STAVY:
enum ERROR {
	ERR_PARAM = 0,
	ERR_MALLOC,
	ERR_SOUBOR,
	ERR_LOCALE,
};

char *chybove_zpravy [] = {
	//CHYBA_PARAM:
	"Chyba v parametru.\n",
	//ERR_MALLOC
	"Nepovedlo se alokovat pamet.\n",
	//ERR_SOUBOR
	"Chyba pri praci se souborem.\n",
	//ERR_LOCALE
	"Chyba funkce setlocale.\n",
};


//STAVOVE KODY PROGRAMU:
enum stav {
	CHYBA_LOCALE = -5,
	CHYBA = -4,
	CHYBA_SOUBOR = -3,
	CHYBA_PARAM = -2,
	CHYBA_MALLOC = -1,
	PARAM_NAPOVEDA = 0,
	PARAM_LOCALE,
	PARAM_BEZ_LOCALE,
	PARAM_USORT,
	PARAM_LOCALE_USORT,
};

//NAPOVEDA:
char NAPOVEDA[] =
"\nProgram projekt4-Ceske razeni.\n"
"Autor: Tomas Valek.\n"
"Datum: 13.12.2011.\n"
"Program nacte radky ze vstupniho souboru a serazene podle normy CSN976030 je \
vypise do souboru. Prazdne radky preskakuje.\n"
"Pouziti: -h\n"
"		[--loc LOCALE]\n"
"		 [--usort]\n"
"Popis parametru:\n\
-h\tVypise tuto obrazovku s napovedou.\n\
[--loc LOCALE]\tJe nepovinny parametr, ktery reprezentuje znakovou sadu.\n\
./proj4 [--loc LOCALE] soubor1.txt soubor2.txt\n\
		[--usort]\t Je nepovinny parametr, ktery provede unikatni sort.\
 Tento parametr se zadava jako posledni.\n";

//PROTOTYPY:
int main( int argc, char* argv[] );
int inicializuj_string(Tstring* str);
int pridej_znak( wchar_t znak, Tstring* str );
int orizni_pole(Tstring* str);
int zkontroluj_parametry( int argc, char* argv[] );
int vloz_prvni(Tpolozka polozka, Tseznam* seznam, Tstring str);
int porovnej(wchar_t* a, wchar_t* b);
void zarad(Tseznam* seznam, int usort);
int nacti_radek(FILE* fr, Tseznam* seznam, Tstring* str);
int nacti_a_serad_seznam(char soubor[], Tseznam* s, int usort);
void vymaz_seznam(Tseznam* seznam);
void zpracuj_chybu(int co, Tseznam* s, FILE* f);
int otevri_soubor(FILE** popisovac, char* nazev,char* rezim);
int zavri_soubor(FILE* popisovac);
int vypis_seznam(char* argv, Tseznam* s);

//***********************************FUNKCE************************************

int inicializuj_string(Tstring* str) {
/*Funkce inicializuje string. Tzn. vytvori pro nej pamet velikosti 
DAVKA(32bajtů).Funkce vraci chybu v pripade neprideleni pameti nebo 0 jako
 uspech.*/

//Pro test:
//str->retezec = rand() & 0x1 ? NULL : (wchar_t*)malloc(sizeof(wchar_t)*DAVKA);

	//if ( str->retezec == NULL )
	if ( (str->retezec = (wchar_t*)malloc(sizeof(wchar_t)*DAVKA)) == NULL )	{
	//pokud se alokace nepodarila
		return CHYBA_MALLOC;
	}

	str->retezec[0] = L'\0';
	str->alok_vel = DAVKA;
	str->delka = 0;

	return EXIT_SUCCESS;
}

int pridej_znak( wchar_t znak, Tstring* str ){ 
/*Funkce zkontroluje zda nedosla pamet a prida na konec retezce 1 znak.
 Pokud dosla prialokuje. Funkce vraci chybu malloc nebo konci uspechem 0.*/

	if ( str->delka + 1 >= str->alok_vel  )
	{//dosla pamet pro string, musime reallokovat
#if DIAG
		wprintf(L"Prialokuji\n");
#endif
		wchar_t* realloc_address = NULL;

//Pro test:
/*realloc_address = rand() & 0x1 ? NULL : (wchar_t*)realloc(str->retezec,\
sizeof(wchar_t)*str->alok_vel*2 );*/

		if ( (realloc_address = (wchar_t*)realloc(str->retezec,sizeof(wchar_t)\
				*str->alok_vel*2 )) == NULL )
		//if ( realloc_address == NULL )
		{
			free((void*)str->retezec);
			str->retezec = NULL;
			return CHYBA_MALLOC;
		}

		str->alok_vel = str->alok_vel*2;	//upravime delku stringu
		str->retezec = realloc_address;		//predame adresu
	}

	//pridavame znak do stringu:
	str->retezec[str->delka] = znak;
	str->delka++;
	str->retezec[str->delka] = L'\0';

	return EXIT_SUCCESS;
}

int orizni_pole(Tstring* str) {
//Oriznuti pole na presnou velikost. Vraci bud novou velikost nebo CHYBA_MALLOC

	wchar_t* realloc_address = NULL;

//Pro test:
/*realloc_address = rand() & 0x1 ? NULL : (wchar_t*)realloc(str->retezec,\
sizeof(wchar_t)*(str->delka+1));*/

	if ( (realloc_address = (wchar_t*)realloc(str->retezec,sizeof(wchar_t)\
		*(str->delka+1))) == NULL )
//	if ( realloc_address == NULL )
	{
		free((void*)str->retezec);
		str->retezec = NULL;
		return CHYBA_MALLOC;
	}

	str->alok_vel = str->delka;			//upravime delku stringu
	str->retezec = realloc_address;		//predame adresu

	return EXIT_SUCCESS;
}

int zkontroluj_parametry( int argc, char* argv[] ){
/*Funkce zkontroluje parametry a vrati jaky prikaz bude zavolan. Pripadne vrati
 chybu.
*/
	if ( argc == 2 ) {
		if ( (strcmp("-h", argv[1])) == 0 )
			return PARAM_NAPOVEDA;
	}
	else if ( argc == 3 )
		return PARAM_BEZ_LOCALE;
	else if ( argc == 5 && ((strcmp("--loc", argv[1])) == 0) )
		return PARAM_LOCALE;
	else if ( argc == 4 && ((strcmp("--usort", argv[3])) == 0) )
		return PARAM_USORT;
	else if ( argc == 6 && ((strcmp("--loc", argv[1])) == 0) &&\
			 ((strcmp("--usort", argv[5])) == 0) )
		return PARAM_LOCALE_USORT;

	return CHYBA_PARAM;
}

int vloz_prvni(Tpolozka polozka, Tseznam* seznam, Tstring str) {
/*Funkce naalokuje v pameti misto pro polozku a vlozi ji na zacatek seznamu.
Vraci bud chybu pri alokaci nebo konci EXIT_SUCCESS.*/

	assert( seznam != NULL );

	Tpolozka* nova_polozka;

//Pro test
//nova_polozka = rand() & 0x1 ? NULL : malloc(sizeof(Tpolozka));

	if ( (nova_polozka = malloc(sizeof(Tpolozka))) == NULL )
	//if ( nova_polozka == NULL )
		return CHYBA_MALLOC;

	//do vytvoreneho mista v pameti s velikosti polozka si polozku vlozime:
	*nova_polozka = polozka;
	nova_polozka->nasledujici = seznam->prvni;
	/*hlavicka seznamu ukazuje na aktualne vlozeny prvek, protoze vkladame
	 vzdycky prvni:*/
	seznam->prvni = nova_polozka;
	nova_polozka->uk_na_jmeno = str.retezec;

	return EXIT_SUCCESS;
}

int porovnej(wchar_t* a, wchar_t* b){
/*Funkce porovnava po kazdem znaku dve pole. Pokud je a > b, tak vraci VETSI,
pokud a < b, tak vraci MENSI, a == b vraci ROVNO.*/

	int i = 0;
	int j = 0;
	int mezera_a = 0;
	int mezera_b = false;
	int pruchod = 1;

	while( pruchod <= 2 ) {
	//slouzi pouze pro prvni a druhy pruchod
		while( a[i] != L'\0' && b[j] != L'\0' ) {
		//Prochazime retezce dokud jeden z nich nedojde nakonec

			//FIX MEZER:
			if ( a[i] == L' ' ) {
			//Pokud je znak mezera
				++mezera_a;
				if ( mezera_a > 1 )	{
				//Pokud je za sebou uz vice mezer, bere se jako jedna
					while( a[i] == L' ' )
						++i;//preskakuji mezery
					mezera_a = 0;
					if ( a[i] == '\0' )
						break;
				}
			} else
				mezera_a = 0;
			if ( b[j] == L' ' ) {
				++mezera_b;
				if ( mezera_b > 1 ) {
					while( b[j] == L' ' )
						++j;
					mezera_b = 0;
					if ( b[j] == '\0' )
						break;
				}
			} else
				mezera_b = 0;
			//KONEC FIX MEZER

			if ( pruchod == 1 ) {//PRUCHOD 1
				if ( tabulka1[a[i]] > tabulka1[b[j]] )
					return VETSI; //prvni znak je vetsi jak druhy
				else if ( tabulka1[a[i]] < tabulka1[b[j]] ) 
					return MENSI; //prvni znak je mensi jak druhy
				else {//znaky jsou stejne
					i++;
					j++;
				}
			} else {//PRUCHOD 2
				if ( tabulka2[a[i]] > tabulka2[b[j]] )
					return VETSI;
				else if ( tabulka2[a[i]] < tabulka2[b[j]] ) 
					return MENSI;
				else { //znaky jsou stejne
					i++;
					j++;
				}
			}
		}

		if ( pruchod == 1 ) {//Napr. pro: Ábel, Abelard rozhodne az delka stringu
			if ( a[i] == L'\0' && b[j] != L'\0' )
				return MENSI;
			else if ( a[i] != L'\0' && b[j] == L'\0' )
				return VETSI;
		}

		++pruchod;
		i = 0;
		j = 0;
		mezera_a = 0;
		mezera_b = 0;
	}

	return ROVNO;
}

void zarad(Tseznam* seznam, int usort){
/*Funkce zaradi novou polozku do seznamu na spravne misto.*/

	Tpolozka* a = NULL;
	Tpolozka* b = NULL;
	Tpolozka* c = NULL;

	bool zmena = true;
	int stav = 0;

	while ( zmena == true ) {//Pokud se provedla nejaka zmena zkontroluj, zda je jeste neco k vymene
		a = seznam->prvni;

		if ( a->nasledujici == NULL )
			return;	//neni co radit v seznamu je jen jedna polozka

		//POUZE PRO PRVNI DVE JMENA:
		zmena = false;
		stav = 0;

		a = seznam->prvni->nasledujici;

		stav = porovnej(seznam->prvni->uk_na_jmeno, a->uk_na_jmeno);

#if DIAG
		wprintf(L"----------------------------------------------------------\n");
		wprintf(L"PRUCHOD1\n");
		wprintf(L"seznam->prvni->uk_na_jmeno:\t%ls\n",seznam->prvni->uk_na_jmeno);
		wprintf(L"a->uk_na_jmeno:\t\t\t%ls\n",a->uk_na_jmeno);
		wprintf(L"Vysledek porovnani %d\n", stav);
		wprintf(L"----------------------------------------------------------\n");
#endif

		if ( usort == USORT && stav == ROVNO ) {
		/*Pokud je povoleno unikatni razeni a polozky jsou si rovny, prvni
		 odstranime*/

			Tpolozka* tmp = NULL;
			tmp = seznam->prvni;

			seznam->prvni = seznam->prvni->nasledujici;

			free(tmp->uk_na_jmeno);
			free(tmp);
		} else if ( stav == VETSI ) {
			/*prvni je vetsi nez druhy, tzn. musi se prehodit. Protoze se serazuje
			 od mensich k vetsim A-Z.*/
#if DIAG
			wprintf(L"Vymenuji\n");
#endif
			zmena = true;

			//prenastaveni ukazatelu
			seznam->prvni->nasledujici = a->nasledujici;
			a->nasledujici = seznam->prvni;
			seznam->prvni = a;
		}

#if DIAG
		wprintf(L"-------------------------\n");
		for ( Tpolozka* tmp = seznam->prvni; tmp != NULL; tmp = tmp->nasledujici) {
			//Kontrolni vypis jednosmerneho seznamu:
			wprintf(L"%ls\n", tmp->uk_na_jmeno);
		}
		wprintf(L"-------------------------\n");
#endif

		if ( usort == USORT && a->nasledujici == NULL ) //kvuli usort
			return;	//neni co radit v seznamu zbyla jen jedna polozka

		//nastaveni ukazatelu na dalsi polozky
		a = seznam->prvni;
		b = a->nasledujici;
		c = b->nasledujici;

		while( c != NULL ) {//zkontrolujeme se zbytkem seznamu

			stav = 0;
			stav = porovnej(b->uk_na_jmeno, c->uk_na_jmeno);

#if DIAG
		wprintf(L"----------------------------------------------------------\n");
		wprintf(L"PRUCHOD2\n");
		wprintf(L"seznam->prvni->uk_na_jmeno:\t%ls\n",seznam->prvni->uk_na_jmeno);
		wprintf(L"a->uk_na_jmeno:\t\t\t%ls\n",a->uk_na_jmeno);
		wprintf(L"Vysledek porovnani %d\n", stav);
		wprintf(L"----------------------------------------------------------\n");
#endif			

			if ( usort == USORT && stav == ROVNO ) {
			/*Pokud je povoleno unikatni razeni a polozky jsou si rovny, prvni
				 odstranime*/

				Tpolozka* tmp = NULL;
				tmp = b;

				a->nasledujici = b->nasledujici;

				free(tmp->uk_na_jmeno);
				free(tmp);

				a = a->nasledujici;
				b = c->nasledujici;
				if ( b == NULL )//usort
					return;
				c = c->nasledujici->nasledujici;
			} else if ( stav == VETSI ) {
			/*prvni je vetsi nez druhy, tzn. musi se prehodit. Protoze se 
				serazuje od mensich k vetsim A-Z.*/
#if DIAG
				wprintf(L"Vymenuji\n");
#endif
				zmena = true;

				b->nasledujici = c->nasledujici;
				c->nasledujici = b;
				a->nasledujici = c;
				
				c = b->nasledujici;
				a = a->nasledujici;
			} else {
			 //prvni je mensi jak druhy
		  	//nastaveni ukazatelu na dalsi polozky
				a = a->nasledujici;
				b = b->nasledujici;
				c = c->nasledujici;
			}
		}	
	}
}

int nacti_radek(FILE* fr, Tseznam* seznam, Tstring* str){
/*Funkce nacte cely radek do jednorozmerneho pole wcharu. Specialne hlida znaky
CH a ch. Vraci bud CHYBA_MALLOC nebo EXIT_SUCCESS.*/

	//Tpolozka polozka;
	wint_t znak;
	int stav = 0;
	bool priznak_c = false;
	bool priznak_C = false;
	int error;

	stav = inicializuj_string(str);

	if ( stav == CHYBA_MALLOC )	{
		zpracuj_chybu(CHYBA_MALLOC, seznam, fr);
		return CHYBA;
	}

	while ( (znak = fgetwc(fr)) != L'\n' ) {//Prochazime jeden radek v souboru

		error = errno;

		if ( error == EILSEQ ) {
			free(str->retezec);
			zpracuj_chybu(CHYBA, seznam, fr);
			perror("Chyba funkce fgetwc");
			return CHYBA;
		}

		if ( znak == WEOF )	{//konec souboru
			free(str->retezec);
			return EOF;
		} else if ( znak == L'c' ) {// c
			priznak_c = true;
			continue;
		} else if ( znak == L'C' ) {// C
			priznak_C = true;
			continue;
		}

		if ( priznak_c == true && znak == L'h' ) {// ch
			znak = ZNAK_ch;
			priznak_c = false;
		} else if ( priznak_C == true ) {// C
			if ( znak == L'h' )	{// Ch
				znak = ZNAK_Ch;
				priznak_C = false;
			} else if ( znak == L'H' ) {// CH
				znak = ZNAK_CH;
				priznak_C = false;
			}
		}

		stav = 0;

		/*Pokud jeden z priznaku je true, znamena to ze za 'c' nebo 'C'
		 neprislo 'h'.Tzn. ze musime vlozit 'c' nebo 'C' a znak, ktery jde po
		 nem se vlozi az za podminkou. Nebo se redukuje vice mezer na jednu.*/
		if ( priznak_c == true ) {
			stav = pridej_znak(L'c', str);
			priznak_c = false;
		} else if ( priznak_C == true ) {
			stav = pridej_znak(L'C', str);
			priznak_C = false;
		}
		
		stav = pridej_znak(znak, str);

		if ( stav == CHYBA_MALLOC ) {
			zpracuj_chybu(CHYBA_MALLOC, seznam, fr);
			return CHYBA;
		}
	}

	/*Zde se osetruje pripad, kdy jsme ukoncili radek, ale znak 'C' nebo 'c' 
	 nebo mezera zustal nezapsat, protoze cekal, zda dalsim znakem nebude 'H'
	 nebo 'h'.*/
	if ( priznak_c == true )
		stav = pridej_znak(L'c', str);
	else if ( priznak_C == true )
		stav = pridej_znak(L'C', str);

	stav = 0;

	if ( stav == CHYBA_MALLOC ) {
		zpracuj_chybu(CHYBA_MALLOC, seznam, fr);
		return CHYBA;
	}
#if DIAG
	wprintf(L"Alokovano %d, delka pole %d\n", str->alok_vel, str->delka);
#endif
	stav = 0;
	stav = orizni_pole(str);
#if DIAG
	wprintf(L"NEW: \tAlokovano %d, delka pole %d\n", str->alok_vel, str->delka);
#endif

	if ( stav == CHYBA_MALLOC ) {
		zpracuj_chybu(CHYBA_MALLOC, seznam, fr);
		return CHYBA;
	}

	return EXIT_SUCCESS;
}

int nacti_a_serad_seznam(char soubor[], Tseznam* s, int usort){
/*Obalovaci funkce, ve funkci se v cyklu nacitaji radky. Kazdy radek je v jine
polozce. Ihned se vola funkce zarad, ktera polozku zaradi na spravne misto dle 
normy.*/

	FILE* fr;
	Tpolozka polozka;
	Tstring str;

	int navrat_hodnota = 0;

	if ( (otevri_soubor(&fr,soubor,"r")) == CHYBA_SOUBOR ) {
		zpracuj_chybu(CHYBA_SOUBOR, NULL, NULL);
		return CHYBA;
	}

	while( navrat_hodnota != EOF ) {
		navrat_hodnota = nacti_radek(fr, s, &str);

		if ( navrat_hodnota == CHYBA )
			return CHYBA;
		else if ( navrat_hodnota == EOF )
			break;

		navrat_hodnota = 0;
		navrat_hodnota = vloz_prvni(polozka, s, str);

		if ( navrat_hodnota == CHYBA_MALLOC ) {
			free(str.retezec);
			zpracuj_chybu(CHYBA_MALLOC, s, fr);
			return CHYBA;
		}

		if ( usort == USORT )
			zarad(s,USORT);
		else
			zarad(s,0);
	}
	
	if ( (zavri_soubor(fr)) == CHYBA_SOUBOR ) {
		zpracuj_chybu(CHYBA_SOUBOR, NULL, fr);
		return CHYBA;
	}

	return EXIT_SUCCESS;
}

void vymaz_seznam(Tseznam* seznam){
/*Funkce vymaze seznam a to tak, ze nejdriv uvolni jednorozmerne pole, na ktere
ukazuje polozka->uk_na_jmeno a po-te se postara o ukazatele okolo ruseneho prvku
a ruseny prvek zrusi.*/

	if ( seznam->prvni == NULL || seznam == NULL )
		return; 

	assert( seznam != NULL );
	assert( seznam->prvni != NULL ); //musi byt first

	while( seznam->prvni != NULL ) {//dokud se neprojde cely seznam

		Tpolozka* polozka;

		/*preklenuti ukazatelu, rusim prvni prvek, tzn. hlavicka bude ukazovat
		 tam kam ukazuje rusena(prva) polozka*/

		polozka = seznam->prvni;
		seznam->prvni = polozka->nasledujici;
#if DIAG
		wprintf(L"Uvolnuji: %ls\n",polozka->uk_na_jmeno );
#endif
		free(polozka->uk_na_jmeno); //uvolneni naalokovaneho radku
		free(polozka);	//uvolneni polozky z pameti
	}
}

void zpracuj_chybu(int co, Tseznam* s, FILE* f) {
/*Funkce, ktera zpracovava chyby. Stara se o uvolneni pameti, zavreni souboru
a vypisu chyby.*/

	//uvolneni zdroju:
	switch ( co ) {
		case CHYBA:
		case CHYBA_MALLOC:
			if ( s->prvni != NULL )
				vymaz_seznam(s);

		case CHYBA_SOUBOR:
			if ( f != NULL )
				fclose(f);
	}

	//vypis chyby:
	if ( co == CHYBA_MALLOC )
		fprintf(stderr, "%s",chybove_zpravy[ERR_MALLOC]);
	else if ( co == CHYBA_SOUBOR )
		fprintf(stderr, "%s",chybove_zpravy[ERR_SOUBOR]);
}

int otevri_soubor(FILE** popisovac, char* nazev,char* rezim){
//Funkce otevre soubor. V pripade uspechu vraci 0, jinak chybu.

	if ( (*popisovac = fopen(nazev,rezim)) == NULL )
		return CHYBA_SOUBOR;
	return EXIT_SUCCESS;
}

int zavri_soubor(FILE* popisovac){
//Funkce zavre soubor. V pripade uspechu vraci 0, jinak chybu.

	if ( (fclose(popisovac)) == EOF)
		return CHYBA_SOUBOR;

	return EXIT_SUCCESS;
}

int vypis_seznam(char* argv, Tseznam* s){
/*Funkce vypise seznam s tim ze osetruje znaky Ch a ch. Funkce konci bud chybou
souboru nebo 0.*/

#if DIAG
	wprintf(L"Vypisuji seznam\n");
#endif

	FILE* fw;
	int i = 0;

	if ( (fw = fopen(argv, "w")) == NULL ) {
		zpracuj_chybu(CHYBA_SOUBOR, NULL, fw);
		return CHYBA;
	}

	for ( Tpolozka* pom = s->prvni; pom != NULL; pom = pom->nasledujici ) {
	//Prochazime kazdym jmenem

		i = 0;

		while ( *(pom->uk_na_jmeno+i) != '\0' )	{
		//Projdeme kazdy radek a vypisujeme po znacich
			if ( *(pom->uk_na_jmeno+i) == ZNAK_ch ) //ch
				fwprintf(fw, L"ch");
			else if ( *(pom->uk_na_jmeno+i) == ZNAK_Ch ) //Ch
				fwprintf(fw, L"Ch");
			else if ( *(pom->uk_na_jmeno+i) == ZNAK_CH ) //CH
				fwprintf(fw, L"CH");
			else
				fwprintf(fw, L"%lc", *(pom->uk_na_jmeno+i));
			i++;
		}
		fwprintf(fw, L"\n");
	}

	if ( (zavri_soubor(fw)) == CHYBA_SOUBOR ) {
		zpracuj_chybu(CHYBA_SOUBOR, NULL, fw);
		return CHYBA;
	}

	return EXIT_SUCCESS;

}

//**************************************MAIN***********************************/

int main( int argc, char* argv[] ){
/*HLAVNI FUNKCE. Vraci 0 v pripade uspechu. Jinak chybu.
argc je pocet parametru
argv[] je pole parametru
*/
	//srand(time(NULL)); pro test

	int stav_param; //slouzi pro navratovou hodnotu z funkce:

	stav_param = zkontroluj_parametry(argc, argv);

	if ( stav_param == CHYBA_PARAM ) {
	//Chyba parametru
		fprintf(stderr,"%s", chybove_zpravy[ERR_PARAM]);
		printf("%s", NAPOVEDA);
		return EXIT_FAILURE;
	} else if ( stav_param == PARAM_NAPOVEDA ){
		//Napoveda
		printf("%s", NAPOVEDA);
		return EXIT_SUCCESS;
	} else {
		int stav = 0;
		Tseznam s;
		s.prvni = NULL;

		if ( stav_param == PARAM_LOCALE || stav_param == PARAM_LOCALE_USORT ) {
			if ( (setlocale(LC_CTYPE,argv[2])) == NULL ) {//Chyba setlocale
				fprintf(stderr,"%s", chybove_zpravy[ERR_LOCALE]);
				return EXIT_FAILURE;
			}

			if ( stav_param == PARAM_LOCALE_USORT )
				stav = nacti_a_serad_seznam(argv[3], &s, USORT);
			else
				stav = nacti_a_serad_seznam(argv[3], &s, 0);
		} else if ( stav_param == PARAM_BEZ_LOCALE || stav_param == PARAM_USORT ) {
			if ( (setlocale(LC_CTYPE,"")) == NULL )	{//Chyba setlocale
				fprintf(stderr,"%s", chybove_zpravy[ERR_LOCALE]);
				return EXIT_FAILURE;
			}

			if ( stav_param == PARAM_USORT )
				stav = nacti_a_serad_seznam(argv[1], &s, USORT);
			else
				stav = nacti_a_serad_seznam(argv[1], &s, 0);
		}

		if ( stav == CHYBA )
			return EXIT_FAILURE;

#if DIAG //Kontrolni vypis jednosmerneho seznamu:
		for ( Tpolozka* tmp = s.prvni; tmp != NULL; tmp = tmp->nasledujici )
			wprintf(L"KONTROLA(main): %ls\n", tmp->uk_na_jmeno);
#endif

		if ( stav_param == PARAM_LOCALE || stav_param == PARAM_LOCALE_USORT )
			vypis_seznam(argv[4], &s);
		else
			vypis_seznam(argv[2], &s);

		vymaz_seznam(&s);

	}

	return EXIT_SUCCESS;
}
