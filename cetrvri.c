#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024
#define MAX_SIZE 100

#define EXIT_SUCCESS 0
#define FILE_ERROR_OPEN -1
#define MALLOC_ERROR -2
#define SCANF_ERROR -3

typedef struct monomial {
	int coef;
	int exp;
	struct monomial* next;
} Mono;

int strToPolinom(Mono*, char*);
int addMonoToPolinom(Mono*, Mono*);
int printMono(Mono*);
int printPolinom(Mono*);
int deletePolinom(Mono*);
int polinomSum(Mono*, Mono*, Mono*);
int polinomProduct(Mono*, Mono*, Mono*);

int main()
{
	/*Mono polinom1Head = {.coef = 0, .exp = 0, .next = NULL};
	Mono polinom2Head = { .coef = 0, .exp = 0, .next = NULL };
	Mono sumHead = { .coef = 0, .exp = 0, .next = NULL };
	Mono productHead = { .coef = 0, .exp = 0, .next = NULL };*\*/

	Mono polinom1Head;
	polinom1Head.coef = 0;
	polinom1Head.exp = 0;
	polinom1Head.next = NULL;

	Mono polinom2Head;
	polinom2Head.coef = 0;
	polinom2Head.exp = 0;
	polinom2Head.next = NULL;

	Mono sumHead;
	sumHead.coef = 0;
	sumHead.exp = 0;
	sumHead.next = NULL;

	Mono productHead;
	productHead.coef = 0;
	productHead.exp = 0;
	productHead.next = NULL;



	int i = 0, size = 0;
	char buffer[MAX_LINE] = { 0 };

	FILE* filePointer;
	filePointer = fopen("polinomi.txt", "r");

	if (filePointer == NULL) {
		printf("Ne može se otvoriti datoteka\n");
		return FILE_ERROR_OPEN;
	}


	fgets(buffer, MAX_LINE, filePointer);
	strToPolinom(&polinom1Head, buffer);


	fgets(buffer, MAX_LINE, filePointer);
	strToPolinom(&polinom2Head, buffer);

	printf("Polinom 1: ");
	printPolinom(polinom1Head.next);
	printf("Polinom 2: ");
	printPolinom(polinom2Head.next);

	polinomSum(&sumHead, polinom1Head.next, polinom2Head.next);
	printf("Sum: ");
	printPolinom(sumHead.next);

	polinomProduct(&productHead, polinom1Head.next, polinom2Head.next);
	printf("Product: ");
	printPolinom(productHead.next);


	fclose(filePointer);

	deletePolinom(&polinom1Head);
	deletePolinom(&polinom2Head);
	deletePolinom(&sumHead);
	deletePolinom(&productHead);

	return EXIT_SUCCESS;
}

int addMonoToPolinom(Mono* head, Mono* mono)
{
	Mono* prev = head;
	Mono* temp = NULL;

	// ax^0 = 0
	if (mono->coef == 0) {
		free(mono);
		return EXIT_SUCCESS;
	}


	while (prev->next != NULL && prev->next->exp > mono->exp)
		prev = prev->next;


	if (prev->next != NULL && mono->exp == prev->next->exp)
	{
		prev->next->coef += mono->coef;
		free(mono);

		if (prev->next->coef == 0)
		{
			temp = prev->next;
			prev->next = prev->next->next;
			free(temp);
		}
	}


	else {
		mono->next = prev->next;
		prev->next = mono;
	}

	return EXIT_SUCCESS;
}

int strToPolinom(Mono* head, char* str)
{
	int i = 0, size = 0;
	Mono* mono = NULL;

	while (strlen(str) > 0)
	{
		mono = (Mono*)malloc(sizeof(Mono));
		if (mono == NULL) return MALLOC_ERROR;

		if (sscanf(str, " %dx^%d %n", &mono->coef, &mono->exp, &size) != 2) return SCANF_ERROR;

		addMonoToPolinom(head, mono);
		str += size;
	}

	return EXIT_SUCCESS;
}

int printMono(Mono* mono)
{
	mono->coef > 0 ? printf("%d", mono->coef) : printf("- %d", -mono->coef);

	if (mono->exp == 1) printf("x ");
	else if (mono->exp != 0) printf("x^%d ", mono->exp);

	return EXIT_SUCCESS;
}

int printPolinom(Mono* current)
{
	if (current != NULL) {
		printMono(current);
		current = current->next;
	}

	while (current != NULL)
	{
		if (current->coef > 0) printf("+ ");
		printMono(current);
		current = current->next;
	}

	printf("\n");

	return EXIT_SUCCESS;
}

int deletePolinom(Mono* head)
{
	Mono* temp = NULL;

	while (head->next != NULL)
	{
		temp = head->next;
		head->next = temp->next;
		free(temp);
	}

	return EXIT_SUCCESS;
}

int polinomSum(Mono* sum, Mono* polinom1, Mono* polinom2)
{
	Mono* temp = NULL;

	while (polinom1 != NULL)
	{
		temp = (Mono*)malloc(sizeof(Mono));
		if (temp == NULL) return MALLOC_ERROR;

		temp->coef = polinom1->coef;
		temp->exp = polinom1->exp;

		addMonoToPolinom(sum, temp);
		polinom1 = polinom1->next;
	}

	while (polinom2 != NULL)
	{
		temp = (Mono*)malloc(sizeof(Mono));
		if (temp == NULL) return MALLOC_ERROR;

		temp->coef = polinom2->coef;
		temp->exp = polinom2->exp;

		addMonoToPolinom(sum, temp);
		polinom2 = polinom2->next;
	}

	return EXIT_SUCCESS;
}

int polinomProduct(Mono* product, Mono* polinom1, Mono* polinom2)
{
	Mono* temp = NULL;
	Mono* polinom2first = polinom2;

	while (polinom1 != NULL)
	{
		polinom2 = polinom2first;
		while (polinom2 != NULL)
		{
			temp = (Mono*)malloc(sizeof(Mono));
			if (temp == NULL) return MALLOC_ERROR;

			temp->coef = polinom1->coef * polinom2->coef;
			temp->exp = polinom1->exp + polinom2->exp;

			addMonoToPolinom(product, temp);
			polinom2 = polinom2->next;
		}
		polinom1 = polinom1->next;
	}

	return EXIT_SUCCESS;
}