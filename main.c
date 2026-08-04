#include <stdio.h>

int main ()
{
	int condDirigido, numGrafo;

	puts("O gráfico é dirigido?\n[1] Sim\n[0] Não");
	scanf("%d" ,&condDirigido);

	printf("\nQuantos vértices terão em seu grafo: ");
	scanf("%d", &numGrafo);

	printf("Dirigido: %d, Quantidade: %d\n", condDirigido, numGrafo);

	return 0;
}
