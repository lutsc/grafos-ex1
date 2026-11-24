#include "pert.h"
#include "pert_svg.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#endif

#define SAIDA_PADRAO "pert.svg"

/*
 * Lê uma linha do teclado sem o '\n'. Retorna false no fim da entrada.
 */
static bool lerLinha(char * buf, size_t tam) {
	if (fgets(buf, (int) tam, stdin) == NULL) return false;

	size_t len = strlen(buf);
	if (len > 0 && buf[len - 1] == '\n') {
		buf[len - 1] = '\0';
	} else {
		//Linha maior que o buffer: descarta o resto
		int c;
		while ((c = getchar()) != '\n' && c != EOF);
	}
	return true;
}

/*
 * Pede um número inteiro entre min e max, repetindo até receber um válido.
 * Se a entrada acabar (Ctrl+D / Ctrl+Z) o programa termina.
 */
static long lerInteiro(const char * msg, long min, long max) {
	char linha[64];
	while (true) {
		printf("%s [%ld a %ld]: ", msg, min, max);
		fflush(stdout);
		if (!lerLinha(linha, sizeof linha)) {
			printf("\n");
			exit(0);
		}

		char * fim;
		long v = strtol(linha, &fim, 10);
		while (*fim == ' ' || *fim == '\t' || *fim == '\r') fim++;
		if (fim != linha && *fim == '\0' && v >= min && v <= max) return v;

		printf("Valor inválido. Digite um número inteiro entre %ld e %ld.\n", min, max);
	}
}

/*
 * Calcula, mostra os resultados na tela e gera o desenho do grafo.
 */
static int processar(struct Pert * p, const char * saida) {
	if (pertCalcular(p) != 0) {
		printf("\nErro: %s\n", p->erro);
		return 1;
	}

	printf("\n");
	pertImprimirTabela(p, stdout);
	printf("\n");
	pertImprimirResumo(p, stdout);

	if (pertGerarSVG(p, saida) != 0) {
		printf("\nNão foi possível gerar o arquivo \"%s\".\n", saida);
		return 1;
	}
	printf("\nDesenho do grafo salvo em \"%s\".\n", saida);
	return 0;
}

/*
 * Digitação da tabela. As atividades são numeradas de 1 até n.
 */
static int digitarTabela(struct Pert * p) {
	long n = lerInteiro("Quantidade de atividades", 1, PERT_MAX_ATIVIDADES);

	for (long i = 1; i <= n; i++) {
		printf("\n-- Atividade %ld --\n", i);
		long duracao = lerInteiro("Duração", 1, PERT_MAX_DURACAO);

		long qtd = 0;
		if (n > 1) qtd = lerInteiro("Quantidade de precedentes (0 se não tiver)", 0, n - 1);

		int * preds = malloc((qtd > 0 ? qtd : 1) * sizeof(int));
		if (preds == NULL) {
			printf("Falha de alocação de memória.\n");
			return 1;
		}

		for (long k = 0; k < qtd; k++) {
			char msg[64];
			snprintf(msg, sizeof msg, "Precedente %ld (número da atividade)", k + 1);
			while (true) {
				long v = lerInteiro(msg, 1, n);
				if (v == i) {
					printf("Uma atividade não pode ser precedente de si mesma.\n");
					continue;
				}
				bool repetido = false;
				for (long m = 0; m < k; m++) if (preds[m] == v) repetido = true;
				if (repetido) {
					printf("Esse precedente já foi informado.\n");
					continue;
				}
				preds[k] = (int) v;
				break;
			}
		}

		int r = pertAdicionar(p, (int) i, (int) duracao, preds, (int) qtd);
		free(preds);
		if (r != 0) {
			printf("Erro: %s\n", p->erro);
			return 1;
		}
	}
	return 0;
}

static int carregarArquivo(struct Pert * p, const char * caminho) {
	FILE * f = strcmp(caminho, "-") == 0 ? stdin : fopen(caminho, "r");
	if (f == NULL) {
		printf("Não foi possível abrir o arquivo \"%s\".\n", caminho);
		return 1;
	}
	int r = pertLerTabela(p, f);
	if (f != stdin) fclose(f);
	if (r != 0) printf("Erro: %s\n", p->erro);
	return r;
}

static int pedirArquivo(struct Pert * p) {
	printf("\nO arquivo deve ter uma atividade por linha: atividade duração precedentes\n");
	printf("Exemplo:  5 5 2,4   (atividade 5, duração 5, precedida pelas atividades 2 e 4)\n");
	printf("Nome do arquivo: ");
	fflush(stdout);

	char caminho[512];
	if (!lerLinha(caminho, sizeof caminho)) {
		printf("\n");
		exit(0);
	}

	//Remove espaços e aspas nas pontas (arrastar o arquivo para o terminal coloca aspas)
	char * ini = caminho;
	while (*ini == ' ' || *ini == '\t' || *ini == '"') ini++;
	size_t len = strlen(ini);
	while (len > 0 && (ini[len - 1] == ' ' || ini[len - 1] == '\t' || ini[len - 1] == '\r' || ini[len - 1] == '"')) ini[--len] = '\0';

	if (len == 0) {
		printf("Nenhum nome informado.\n");
		return 1;
	}
	return carregarArquivo(p, ini);
}

static void menu(const char * saida) {
	while (true) {
		printf("\n===== PERT/CPM =====\n");
		printf("1 - Digitar a tabela de atividades\n");
		printf("2 - Carregar a tabela de um arquivo\n");
		printf("0 - Sair\n");
		long opcao = lerInteiro("Opção", 0, 2);
		if (opcao == 0) return;

		struct Pert p;
		pertIniciar(&p);

		int r;
		if (opcao == 1) r = digitarTabela(&p);
		else if (opcao == 2) r = pedirArquivo(&p);
		else r = pertCarregarExemplo(&p);

		if (r == 0) processar(&p, saida);
		pertLiberar(&p);
	}
}

static void uso(void) {
	printf("PERT/CPM - monta o grafo, o caminho crítico e as folgas a partir de uma tabela de atividades.\n\n");
	printf("Uso:\n");
	printf("  pert                      menu interativo\n");
	printf("  pert tabela.txt           lê a tabela do arquivo (use - para ler da entrada padrão)\n");
	printf("  pert --exemplo            usa a tabela de exemplo da aula\n");
	printf("  pert tabela.txt -o g.svg  escolhe o nome do arquivo de saída (padrão: %s)\n\n", SAIDA_PADRAO);
	printf("Formato da tabela: uma atividade por linha, \"atividade duração precedentes\".\n");
	printf("Os precedentes podem ser separados por vírgula ou espaço. Exemplo:\n");
	printf("  1 10\n  2 4 1\n  5 5 2,4\n");
}

int main(int argc, char ** argv) {
#ifdef _WIN32
	SetConsoleOutputCP(CP_UTF8);
#endif

	const char * saida = SAIDA_PADRAO;
	const char * arquivo = NULL;
	bool exemplo = false;

	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
			saida = argv[++i];
		} else if (strcmp(argv[i], "--exemplo") == 0) {
			exemplo = true;
		} else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--ajuda") == 0) {
			uso();
			return 0;
		} else if (argv[i][0] == '-' && argv[i][1] != '\0') {
			fprintf(stderr, "Opção desconhecida: %s (use --ajuda)\n", argv[i]);
			return 1;
		} else {
			arquivo = argv[i];
		}
	}

	if (arquivo == NULL && !exemplo) {
		menu(saida);
		return 0;
	}

	struct Pert p;
	pertIniciar(&p);

	int r;
	if (arquivo != NULL) r = carregarArquivo(&p, arquivo);
	else r = pertCarregarExemplo(&p);

	if (r == 0) r = processar(&p, saida);
	pertLiberar(&p);
	return r;
}
