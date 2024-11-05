#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//sortirani unos

int InsertSorted(Position head, Position nE) {
	Position temp = head;
	while (temp->next && temp->next->exp<nE->exp)
	{
		temp = temp->next;
	}
	if (temp->next==NULL || temp->next->exp!=nE->exp)
	{
		InsertSorted(temp, nE);
	}
	else
	{
		int resultCoefficient = nE->coeff + temp->nextnext->coeff;
		if (resultCoefficient==0)
		{
			DeleteAfter(temp);
			free(nE);
		}
		else
		{
			temp->next->coeff = resultCoeff;
			free(nE);
		}
	}
	fgets(buffer, MAX_LINE, file);
	while (strlen(buffer)>0)
	{
		int status = sscanf(buffer, " %d %d %n,", &c, &e, %numBytes);

		//...//
		buffer = buffer + numBytes;
	}
}

//zbrajanje polinoma


//moženje 

for (i) {
	for (j)
	{
		/
		int result = temp->next->coeff * temp->next->next->coeff;


	}
}






