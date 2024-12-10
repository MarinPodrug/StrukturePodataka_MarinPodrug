#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024

#define EXIT_SUCCESS 0
#define FILE_OPEN_ERROR -1
#define MALLOC_ERROR -2
#define SCANF_ERROR -3

struct artikl;
typedef struct artikl* ArtiklPosition;
typedef struct artikl {
	char ime[20];
	int kolicina;
	int cijena;
	ArtiklPosition next;
} Artikl;

struct racun;
typedef struct racun* RacunPosition;
typedef struct racun {
	char datum[10];
	Artikl headArtikl;
	RacunPosition next;
} Racun;

int dodajArtikle(ArtiklPosition head, ArtiklPosition artikl);
int dodajRacun(RacunPosition head, RacunPosition racun);
int ucitajRacun(char* fileName, RacunPosition headRacun);
int ispisiArtikle(ArtiklPosition artikl);
int ispisiRacun(RacunPosition racun);
int izracununutarperioda(RacunPosition currentRacun, char* artikl, char* pocetni_datum, char* krajnji_datum, int* iznos, int* broj);
int izbrisiArtikle(ArtiklPosition artiklHead);
int izbrisiRacune(RacunPosition racunHead);

int main()
{
	int iznos = 0, broj = 0;
	char artikl[100] = { 0 };
	char pocetni_datum[20] = { 0 }, krajnji_datum[20] = { 0 };
	char currRacun[MAX_LINE] = { 0 };
	FILE* filePointer = NULL;
	Racun headRacun = {
	{0},      
	NULL,     
	NULL      
	};


	filePointer = fopen("racuni.txt", "r");
	if (filePointer == NULL) {
		printf("Datoteka se ne moze otvoriti.\n");
		return FILE_OPEN_ERROR;
	}

	while (!feof(filePointer)) {
		fgets(currRacun, MAX_LINE, filePointer);
		currRacun[strcspn(currRacun, "\n")] = 0; 
		printf("%s\n", currRacun);
		ucitajRacun(currRacun, &headRacun);
	}

	ispisiRacun(headRacun.next);

	printf("\nUpisi ime artikla:  ");
	if (scanf("%s", artikl) != 1) return SCANF_ERROR;
	printf("Unesi pocetni datum:: ");
	if (scanf("%s", pocetni_datum) != 1) return SCANF_ERROR;
	printf("Unesi krajnji datum: ");
	if (scanf("%s", krajnji_datum) != 1) return SCANF_ERROR;

	izracununutarperioda(&headRacun, artikl, pocetni_datum, krajnji_datum, &iznos, &broj);
	printf("Broj artikla %s je %d, a ukupan iznos je %d\n", artikl, broj, iznos);


	izbrisiRacune(&headRacun);
	fclose(filePointer);

	return 0;
}

int dodajArtikle(ArtiklPosition head, ArtiklPosition artikl)
{
	ArtiklPosition current = head;

	while (current->next != NULL && strcmp(current->next->ime, artikl->ime) == -1)
		current = current->next;

	artikl->next = current->next;
	current->next = artikl;

	return 0;
}

int dodajRacun(RacunPosition head, RacunPosition racun)
{
	RacunPosition current = head;

	while (current->next != NULL && strcmp(current->next->datum, racun->datum) <= 0)
		current = current->next;

	racun->next = current->next;
	current->next = racun;

	return 0;
}

int ucitajRacun(char* fileName, RacunPosition headRacun)
{
	char datumString[20] = { 0 };
	char artiklString[MAX_LINE] = { 0 };
	int broj = 0;
	FILE* filePointer = NULL;
	RacunPosition newRacun = NULL;
	ArtiklPosition newArtikl = NULL;

	filePointer = fopen(fileName, "r");
	if (filePointer == NULL) {
		printf("Datoteka se ne moze otvoriti.\n");
		return FILE_OPEN_ERROR;
	}

	newRacun = (RacunPosition)malloc(sizeof(Racun));
	if (newRacun == NULL) {
		printf("pogreska pri alokaciji memorije.\n");
		return MALLOC_ERROR;
	}

	fgets(datumString, 20, filePointer);
	datumString[strcspn(datumString, "\n")] = 0;

	strcpy(newRacun->datum, datumString);
	newRacun->headArtikl.next = NULL;

	while (!feof(filePointer))
	{
		newArtikl = (ArtiklPosition)malloc(sizeof(Artikl));
		if (newArtikl == NULL) {
			printf("pogreska pri alokaciji memorije.\n");
			return MALLOC_ERROR;
		}

		if (fscanf(filePointer, "%[^,], %d, %d ",
			newArtikl->ime, &newArtikl->kolicina, &newArtikl->cijena) != 3) {
			printf("fscanf error.\n");
			return SCANF_ERROR;
		}

		dodajArtikle(&newRacun->headArtikl, newArtikl);
	}

	dodajRacun(headRacun, newRacun);

	fclose(filePointer);

	return 0;
}

int ispisiArtikle(ArtiklPosition current)
{
	while (current != NULL) {
		printf("\t%s, %d, %d\n", current->ime, current->kolicina, current->cijena);
		current = current->next;
	}

	return 0;
}

int ispisiRacun(RacunPosition current)
{
	while (current != NULL) {
		printf("Raèun za datum %s:\n", current->datum);
		ispisiArtikle(current->headArtikl.next);
		current = current->next;
	}

	return 0;
}

int izracununutarperioda(RacunPosition currentRacun, char* artikl, char* pocetni_datum, char* krajnji_datum, int* iznos, int* broj)
{
	ArtiklPosition currentArtikl = NULL;

	
	while (currentRacun != NULL && strcmp(currentRacun->datum, pocetni_datum) == -1)
		currentRacun = currentRacun->next;

	while (currentRacun != NULL && strcmp(currentRacun->datum, krajnji_datum) <= 0) {
		currentArtikl = currentRacun->headArtikl.next;
		while (currentArtikl != NULL) {
			if (strcmp(currentArtikl->ime, artikl) == 0) {
				*iznos += currentArtikl->kolicina * currentArtikl->cijena;
				*broj += currentArtikl->kolicina;
			}
			currentArtikl = currentArtikl->next;
		}
		currentRacun = currentRacun->next;
	}

	return EXIT_SUCCESS;
}


int izbrisiArtikle(ArtiklPosition artiklHead)
{
	ArtiklPosition temp = NULL;

	while (artiklHead->next != NULL) {
		temp = artiklHead->next;
		artiklHead->next = temp->next;
		free(temp);
	}

	return 0;
}
int izbrisiRacune(RacunPosition racunHead)
{
	RacunPosition temp = NULL;

	while (racunHead->next != NULL) {
		temp = racunHead->next;
		racunHead->next = temp->next;
		izbrisiArtikle(&temp->headArtikl);
		free(temp);

	}

	return 0;
}