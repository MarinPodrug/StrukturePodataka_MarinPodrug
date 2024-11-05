#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>


struct person;

typedef struct person* Position;

struct person
{

	char fname[20];
	char lname[20];
	int birth_year;
	Position Next;
};

void InsertPerson(Position);
void PrintLinkedList(Position);
Position FindLastPerson(Position);
Position FindPerson(Position);
Position FindPreviousPerson(Position);
void DeletePerson(Position);
void SortLinkedList(Position);
void ReadFile(Position);
void WriteToFile(Position);


void main()
{
	struct person head, * q;
	char izbor = 0;

	head.Next = NULL;
	while (izbor != 'z' && izbor != 'Z')
	{
		printf("\nUnesi:");
		printf("\n\t0 - unesi element na poèetak v. l.");
		printf("\n\t1 - ispiši v.l.");
		printf("\n\t2 - unesi element na kraj v.l.");
		printf("\n\t3 - pronaði element u v. l. po prezimenu");
		printf("\n\t4 - izbriši element u v.l.");
		printf("\n\t5 - dodaj ele,ent iza elementa");
		printf("\n\t6 - dodaj element ispred elementa");
		printf("\n\t7 - sortiraj v. l. po prezimenu");
		printf("\n\t8 - uèitaj podatke iz datoteke");
		printf("\n\t9 - upiši v. l. u datoteku");
		printf("\n\tz - završi program\n\n\t");

		scanf(" %c", &izbor);

		switch (izbor)
		{
		case '0':
			InsertPerson(&head);
			break;
		case '1':
			PrintLinkedList(head.Next);
			break;
		case '2':
			q = FindLastPerson(&head);
			InsertPerson(q);
			break;
		case '3':
			q = FindPerson(head.Next);
			if (NULL == q)
				printf("\n Osoba ne postoji u listi!");
			else
				printf("\n\tOsoba je: %s %s, %d", q->fname, q->lname, q->birth_year);
			break;
		case '4':
			DeletePerson(&head);
			break;
		case '5':
			q = FindPerson(head.Next);
			if (NULL == q)
				printf("\n Osoba ne postoji u listi!");
			else
				InsertPerson(q);
			break;
		case '6':
			q = FindPreviousPerson(&head);
			if (NULL == q)
				printf("\n Osoba ne postoji u listi!");
			else
				InsertPerson(q);
			break;
		case '7':
			SortLinkedList(&head);
			break;
		case '8':
			ReadFile(&head);
			break;
		case '9':
			WriteToFile(head.Next);
			break;
		case 'z':
		case 'Z':
			printf("\nKraj programa!!!\n");
			break;
		default:
			printf("\nKrivi unos!!!\n");
		}
	}
}

void InsertPerson(Position P)
{
	Position temp;

	temp = (Position)malloc(sizeof(struct person));
	printf("\nUnesite podatke o osobi: ");
	scanf(" %s %s %d", temp->fname, temp->lname, &temp->birth_year);

	temp->Next = P->Next;
	P->Next = temp;
}

void PrintLinkedList(Position P)
{
	if (NULL == P)
		printf("\nLista je prazna!!\n");
	else
	{
		printf("\nU listi se nalaze:");
		while (P != NULL)
		{
			printf("\n\t %s %s %d", P->fname, P->lname, P->birth_year);
			P = P->Next;
		}
	}


}

Position FindLastPerson(Position P)
{
	while (NULL != P->Next)
		P = P->Next;
	return P;
}


Position FindPerson(Position P)
{
	char prez[10];


	printf("\nUnesi prezime osobe koju zelis pronaci: ");
	scanf(" %s", prez);

	while (P != NULL && _strcmpi(P->lname, prez) != 0)
		P = P->Next;

	return P;
}

Position FindPreviousPerson(Position P)
{
	char prez[10];

	if (NULL == P->Next)
		P = NULL;
	else
	{
		printf("\nUnesi prezime osobe koju zelis pronaci: ");
		scanf(" %s", prez);


		while (_strcmpi(P->Next->lname, prez) != 0 && P->Next->Next != NULL)
			P = P->Next;

		if (_strcmpi(P->Next->lname, prez) != 0)
			P = NULL;
	}

	return P;
}

void DeletePerson(Position P)
{
	Position prev;

	prev = FindPreviousPerson(P);

	if (NULL != prev)
	{
		P = prev->Next;
		prev->Next = P->Next;
		printf("\n Osoba %s %s %d je obrisana!", P->fname, P->lname, P->birth_year);
		free(P);
	}
	else
		printf("\n Osoba ne postoji u listi!");
}

void SortLinkedList(Position P)
{

	Position i, j, prev_j, temp, end;

	end = NULL;

	i = P;

	while (i->Next != end)
	{
		prev_j = i;
		j = i->Next;
		while (j->Next != end)
		{
			if (strcmp(j->lname, j->Next->lname) > 0)
			{
				temp = j->Next;
				prev_j->Next = temp;
				j->Next = temp->Next;
				temp->Next = j;

				j = temp;
			}

			prev_j = j;
			j = j->Next;
		}
		end = j;
	}

}

void ReadFile(Position P)
{


	FILE* file;
	char filename[20];
	Position temp;

	printf("\nUnesi ime datotke:");
	scanf(" %s", filename);

	file = fopen(filename, "r");

	if (NULL == file)
		printf("\n Ne postoji datoteke s tim imenom");
	else
	{
		while (feof(file) == 0)
		{
			temp = (Position)malloc(sizeof(struct person));

			fscanf(file, " %s %s %d", temp->fname, temp->lname, &temp->birth_year);

			temp->Next = P->Next;
			P->Next = temp;
			P = temp;
		}

		fclose(file);
	}
}

void WriteToFile(Position P)
{


	FILE* file;
	char filename[20];

	printf("\nUnesi ime datoteke u koju zelis spremiti listu:");
	scanf(" %s", filename);

	file = fopen(filename, "w");

	if (NULL == file)
		printf("\n Greska u otvaranju datoteke");
	else
	{
		while (P != NULL)
		{
			fprintf(file, "\n %s %s %d", P->fname, P->lname, P->birth_year);
			P = P->Next;
		}

		fclose(file);
	}
}