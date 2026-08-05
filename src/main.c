#include <stdio.h>
#include <stdlib.h>
#include <lista_encadeada.h>

int main ()
{
	int condDirigido, numGrafo;

	printf("\n");
	puts("O gráfico é dirigido?\n[1] Sim\n[0] Não");
	scanf("%d" ,&condDirigido);

	printf("\nQuantos vértices terão em seu grafo: ");
	scanf("%d", &numGrafo);

	printf("Dirigido: %d, Quantidade: %d\n", condDirigido, numGrafo);
	
	// Criando vetor dinâmico e alocando espaço
	struct Node *array = malloc(numGrafo * sizeof(struct Node));
	
	// Alocando valores teste
	for(size_t i = 0; i < (size_t)numGrafo; i++){
		int *valor = malloc(sizeof(int));
		*valor = i+1;
		array[i].data = valor;
	}

	// Testando se valores 
	printf("\n");
	for(size_t i = 0; i < numGrafo; i++){
		printf("%d\n", *(int *)array[i].data);
	}
	printf("\n");
	
	// Liberando dados e vetor
	for(size_t i = 0; i < numGrafo; i++){
		free(array[i].data);
		array[i].data = NULL;
	}
	free(array);
	array = NULL;

	return 0;
}
