#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "lista_encadeada.h"
#include "grafos.h"

int main()
{
	struct Graph graph;
	bool ** matriz = NULL;

	bool testMat[4][4] = {
		{0, 1, 0, 0},
		{0, 0, 0, 1},
		{0, 1, 0, 0},
		{0, 0, 1, 0},
	};

	int32_t  ftd1[4];
	int32_t  ftd2[4];
	int32_t  ftd3[4];
	int32_t  ftd4[4];

	int32_t  ftdi1[4];
	int32_t  ftdi2[4];
	int32_t  ftdi3[4];
	int32_t  ftdi4[4];
	bool ** mat = calloc(4, sizeof(bool *));

	for(int i = 0; i < 4; i++)
	{
		mat[i] = calloc(4, sizeof(bool));
		for(int j = 0; j < 4; j++)
		{
			mat[i][j] = testMat[i][j];
		}
	}

	ftd(mat, 4, 0, ftd1);
	ftd(mat, 4, 1, ftd2);
	ftd(mat, 4, 2, ftd3);
	ftd(mat, 4, 3, ftd4);

	ftdi(mat, 4, 0, ftdi1);
	ftdi(mat, 4, 1, ftdi2);
	ftdi(mat, 4, 2, ftdi3);
	ftdi(mat, 4, 3, ftdi4);

	printf("ftd1: ");
	for(int j = 0; j < 4; j++) {
		printf("%d ", ftd1[j]);
	}
	puts("");

	printf("ftd2: ");
	for(int j = 0; j < 4; j++) {
		printf("%d ", ftd2[j]);
	}
	puts("");

	printf("ftd3: ");
	for(int j = 0; j < 4; j++) {
		printf("%d ", ftd3[j]);
	}
	puts("");

	printf("ftd4: ");
	for(int j = 0; j < 4; j++) {
		printf("%d ", ftd4[j]);
	}
	puts("");



	puts("");
	printf("ftdi1: ");
	for(int j = 0; j < 4; j++) {
		printf("%d ", ftdi1[j]);
	}
	puts("");

	printf("ftdi2: ");
	for(int j = 0; j < 4; j++) {
		printf("%d ", ftdi2[j]);
	}
	puts("");

	printf("ftdi3: ");
	for(int j = 0; j < 4; j++) {
		printf("%d ", ftdi3[j]);
	}
	puts("");

	printf("ftdi4: ");
	for(int j = 0; j < 4; j++) {
		printf("%d ", ftdi4[j]);
	}
	puts("");

	if(eConexo(mat, 4)) {
		printf("A matriz é conexa\n");
	}
	else {
		printf("A matriz não é conexa\n");
	}

	

}
