#include <stdio.h>
#include <stdlib.h>
#include <lista_encadeada.h>

int main ()
{
	int condDirigido, numGrafo;

	puts("\nO gráfico é dirigido?\n[1] Sim\n[0] Não");
	scanf("%d" ,&condDirigido);
	if(condDirigido < 0 || condDirigido > 1){
		printf("Opção inválida.\n");
		return 1;
	}

	printf("\nQuantos vértices terão em seu grafo: ");
	scanf("%d", &numGrafo);
	if(numGrafo <= 0){
		printf("Opção inválida.\n");
		return 1;
	}

	printf("Dirigido: %d, Quantidade: %d\n", condDirigido, numGrafo);
	
	// Criando vetor dinâmico e alocando espaço
	struct Node * array = (struct Node *)calloc(numGrafo, sizeof(struct Node));
	if(array == NULL){
		printf("\n Erro ao alocar memória para o vetor dinâmico.");
		return 1;
	}

	// Alocando valores temp
	for(size_t i = 0; i < (size_t)numGrafo; i++){
		int * valor; 
		valor = (int *)malloc(sizeof(int));
		* valor = i+1;
		array[i].data = valor;
	}

	// Testando se elementos do vetor dinâmico estão funcionais
	printf("\n");
	for(size_t i = 0; i < numGrafo; i++){
		printf("Elemento %d ativo.\n", *(int *)array[i].data);
	}
	printf("\n");

	// Valores teste para representar as conexões das vértices
	char conexoes1[] = {'A', 'B', 'C'};
	char conexoes2[] = {'D', 'E', 'F'};

	// Alocando conexões
	for (size_t i = 0; i < 3; i++) {
		char * temp = (char *)calloc(1, sizeof(char));
		* temp = conexoes1[i];
		insereListaFim(&(array[0].next), temp);
		temp = (char *)calloc(1, sizeof(char));
		* temp = conexoes2[i];
		insereListaFim(&(array[1].next), temp);
	}

	// Verificando se elementos tiveram suas conexões alocadas corretamente
	printf("Conexões do 1° elemento: ");
	imprimirLista(array[0].next, imprimeChar);
	printf("\n");
	printf("Conexões do 2° elemento: ");
	imprimirLista(array[1].next, imprimeChar);
	printf("\n");

	// Liberando dados e vetor
	for(size_t i = 0; i < numGrafo; i++){
		liberarLista(array[i].next);
		free(array[i].data);
		array[i].data = NULL;
	}
	
	return 0;
}
