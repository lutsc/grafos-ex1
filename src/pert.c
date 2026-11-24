#include "pert.h"

#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TOKENS 2100
#define LIMITE_CAMINHOS_IMPRESSOS 20
#define CAP_CAMINHOS 1000000000UL

static void definirErro(struct Pert * p, const char * fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(p->erro, sizeof p->erro, fmt, ap);
	va_end(ap);
}

void pertIniciar(struct Pert * p) {
	memset(p, 0, sizeof *p);
}

/*
 * Libera tudo o que pertCalcular alocou (sucessores, precedentes resolvidos e ordem).
 */
static void liberarCalculo(struct Pert * p) {
	for (int i = 0; i < p->qtd; i++) {
		free(p->ativ[i].pred);
		free(p->ativ[i].suc);
		p->ativ[i].pred = NULL;
		p->ativ[i].suc = NULL;
		p->ativ[i].qtdPred = 0;
		p->ativ[i].qtdSuc = 0;
	}
	free(p->ordem);
	p->ordem = NULL;
	p->calculado = false;
}

void pertLiberar(struct Pert * p) {
	liberarCalculo(p);
	for (int i = 0; i < p->qtd; i++) free(p->ativ[i].predId);
	free(p->ativ);
	pertIniciar(p);
}

int pertAdicionar(struct Pert * p, int id, int duracao, const int * precedentes, int qtdPrecedentes) {
	if (id < 1 || id > PERT_MAX_ID) {
		definirErro(p, "Atividade %d: o número da atividade deve estar entre 1 e %d.", id, PERT_MAX_ID);
		return 1;
	}
	if (duracao < 1 || duracao > PERT_MAX_DURACAO) {
		definirErro(p, "Atividade %d: a duração deve estar entre 1 e %d (recebido %d).", id, PERT_MAX_DURACAO, duracao);
		return 1;
	}
	if (p->qtd >= PERT_MAX_ATIVIDADES) {
		definirErro(p, "Limite de %d atividades atingido.", PERT_MAX_ATIVIDADES);
		return 1;
	}
	for (int i = 0; i < p->qtd; i++) {
		if (p->ativ[i].id == id) {
			definirErro(p, "Atividade %d aparece mais de uma vez na tabela.", id);
			return 1;
		}
	}
	for (int k = 0; k < qtdPrecedentes; k++) {
		if (precedentes[k] < 1 || precedentes[k] > PERT_MAX_ID) {
			definirErro(p, "Atividade %d: precedente %d inválido.", id, precedentes[k]);
			return 1;
		}
	}

	if (p->qtd == p->capacidade) {
		int nova = p->capacidade ? p->capacidade * 2 : 16;
		struct Atividade * tmp = realloc(p->ativ, nova * sizeof *tmp);
		if (tmp == NULL) {
			definirErro(p, "Falha de alocação de memória.");
			return 1;
		}
		p->ativ = tmp;
		p->capacidade = nova;
	}

	struct Atividade * a = &p->ativ[p->qtd];
	memset(a, 0, sizeof *a);
	a->id = id;
	a->duracao = duracao;
	if (qtdPrecedentes > 0) {
		a->predId = malloc(qtdPrecedentes * sizeof(int));
		if (a->predId == NULL) {
			definirErro(p, "Falha de alocação de memória.");
			return 1;
		}
		memcpy(a->predId, precedentes, qtdPrecedentes * sizeof(int));
		a->qtdPredId = qtdPrecedentes;
	}

	p->qtd++;
	liberarCalculo(p);
	return 0;
}

static bool ehSeparador(char c) {
	return c == ' ' || c == '\t' || c == '\r' || c == '\n' || c == ',' || c == ';';
}

int pertLerTabela(struct Pert * p, FILE * f) {
	char linha[4096];
	int valores[MAX_TOKENS];
	int numLinha = 0;
	bool primeiraLinhaUtil = true;

	while (fgets(linha, sizeof linha, f) != NULL) {
		numLinha++;

		size_t tam = strlen(linha);
		if (tam == sizeof linha - 1 && linha[tam - 1] != '\n') {
			definirErro(p, "Linha %d: linha longa demais.", numLinha);
			return 1;
		}

		char * c = linha;
		//Arquivos salvos pelo Bloco de Notas podem começar com BOM UTF-8
		if (numLinha == 1 && (unsigned char) c[0] == 0xEF && (unsigned char) c[1] == 0xBB && (unsigned char) c[2] == 0xBF) c += 3;

		int qtd = 0;
		bool cabecalho = false;

		while (*c != '\0' && *c != '#') {
			if (ehSeparador(*c)) {
				c++;
				continue;
			}

			char * ini = c;
			while (*c != '\0' && *c != '#' && !ehSeparador(*c)) c++;
			size_t len = (size_t) (c - ini);
			char tok[32];
			if (len >= sizeof tok) {
				definirErro(p, "Linha %d: valor longo demais.", numLinha);
				return 1;
			}
			memcpy(tok, ini, len);
			tok[len] = '\0';

			//Primeira linha que não é comentário nem está em branco: se não começa com número, é o cabeçalho
			if (qtd == 0 && primeiraLinhaUtil && !isdigit((unsigned char) tok[0])) {
				cabecalho = true;
				break;
			}

			//Um "-" na coluna de precedentes significa "nenhum"
			if (qtd >= 2 && strcmp(tok, "-") == 0) continue;

			char * fim;
			long v = strtol(tok, &fim, 10);
			if (fim == tok || *fim != '\0') {
				definirErro(p, "Linha %d: \"%s\" não é um número.", numLinha, tok);
				return 1;
			}
			if (v < 1 || v > PERT_MAX_ID) {
				definirErro(p, "Linha %d: o valor %ld está fora do intervalo permitido (1 a %d).", numLinha, v, PERT_MAX_ID);
				return 1;
			}
			if (qtd >= MAX_TOKENS) {
				definirErro(p, "Linha %d: informações demais.", numLinha);
				return 1;
			}
			valores[qtd++] = (int) v;
		}

		if (cabecalho) {
			primeiraLinhaUtil = false;
			continue;
		}
		if (qtd == 0) continue;
		primeiraLinhaUtil = false;

		if (qtd < 2) {
			definirErro(p, "Linha %d: informe pelo menos a atividade e a duração.", numLinha);
			return 1;
		}

		if (pertAdicionar(p, valores[0], valores[1], valores + 2, qtd - 2) != 0) {
			char msg[sizeof p->erro];
			memcpy(msg, p->erro, sizeof msg);
			definirErro(p, "Linha %d: %s", numLinha, msg);
			return 1;
		}
	}

	if (p->qtd == 0) {
		definirErro(p, "Nenhuma atividade encontrada na tabela.");
		return 1;
	}
	return 0;
}

int pertCarregarExemplo(struct Pert * p) {
	const int p2[] = {1};
	const int p3[] = {1};
	const int p4[] = {3};
	const int p5[] = {2, 4};
	const int p6[] = {3};

	if (pertAdicionar(p, 1, 10, NULL, 0)) return 1;
	if (pertAdicionar(p, 2, 4, p2, 1)) return 1;
	if (pertAdicionar(p, 3, 7, p3, 1)) return 1;
	if (pertAdicionar(p, 4, 5, p4, 1)) return 1;
	if (pertAdicionar(p, 5, 5, p5, 2)) return 1;
	if (pertAdicionar(p, 6, 2, p6, 1)) return 1;
	return 0;
}

/*
 * Converte os ids dos precedentes em posições no vetor (pred) e valida a tabela.
 */
static int resolverPrecedentes(struct Pert * p) {
	int n = p->qtd;
	int ret = 1;
	int * tabela = malloc((PERT_MAX_ID + 1) * sizeof(int));
	if (tabela == NULL) {
		definirErro(p, "Falha de alocação de memória.");
		return 1;
	}
	memset(tabela, 0xFF, (PERT_MAX_ID + 1) * sizeof(int)); //Tudo -1
	for (int i = 0; i < n; i++) tabela[p->ativ[i].id] = i;

	for (int i = 0; i < n; i++) {
		struct Atividade * a = &p->ativ[i];
		a->pred = malloc((a->qtdPredId > 0 ? a->qtdPredId : 1) * sizeof(int));
		if (a->pred == NULL) {
			definirErro(p, "Falha de alocação de memória.");
			goto fim;
		}
		for (int k = 0; k < a->qtdPredId; k++) {
			int j = tabela[a->predId[k]];
			if (j < 0) {
				definirErro(p, "Atividade %d: o precedente %d não existe na tabela.", a->id, a->predId[k]);
				goto fim;
			}
			if (j == i) {
				definirErro(p, "Atividade %d: uma atividade não pode ser precedente de si mesma.", a->id);
				goto fim;
			}
			bool repetido = false;
			for (int m = 0; m < a->qtdPred; m++) {
				if (a->pred[m] == j) {
					repetido = true;
					break;
				}
			}
			if (!repetido) a->pred[a->qtdPred++] = j;
		}
	}
	ret = 0;

fim:
	free(tabela);
	return ret;
}

static int construirSucessores(struct Pert * p) {
	int n = p->qtd;
	int * cont = calloc(n, sizeof(int));
	if (cont == NULL) {
		definirErro(p, "Falha de alocação de memória.");
		return 1;
	}

	for (int i = 0; i < n; i++)
		for (int k = 0; k < p->ativ[i].qtdPred; k++)
			p->ativ[p->ativ[i].pred[k]].qtdSuc++;

	for (int i = 0; i < n; i++) {
		struct Atividade * a = &p->ativ[i];
		a->suc = malloc((a->qtdSuc > 0 ? a->qtdSuc : 1) * sizeof(int));
		if (a->suc == NULL) {
			free(cont);
			definirErro(p, "Falha de alocação de memória.");
			return 1;
		}
	}

	for (int i = 0; i < n; i++) {
		for (int k = 0; k < p->ativ[i].qtdPred; k++) {
			int v = p->ativ[i].pred[k];
			p->ativ[v].suc[cont[v]++] = i;
		}
	}

	free(cont);
	return 0;
}

/*
 * Monta a mensagem de erro com um ciclo real entre as precedências.
 * "grau" tem valor > 0 nas atividades que não puderam ser ordenadas.
 * Toda atividade assim tem pelo menos um precedente também não ordenado,
 * então andar "para trás" sempre acaba voltando a uma atividade já visitada.
 */
static void descreverCiclo(struct Pert * p, const int * grau) {
	int n = p->qtd;
	int * visto = calloc(n, sizeof(int));
	int * trilha = malloc(n * sizeof(int));
	char desc[300];
	desc[0] = '\0';

	if (visto != NULL && trilha != NULL) {
		int cur = -1;
		for (int i = 0; i < n; i++) {
			if (grau[i] > 0) {
				cur = i;
				break;
			}
		}

		int len = 0;
		while (cur >= 0 && !visto[cur]) {
			visto[cur] = len + 1;
			trilha[len++] = cur;
			int prox = -1;
			for (int k = 0; k < p->ativ[cur].qtdPred; k++) {
				if (grau[p->ativ[cur].pred[k]] > 0) {
					prox = p->ativ[cur].pred[k];
					break;
				}
			}
			cur = prox;
		}

		if (cur >= 0) {
			//trilha[k + 1] é precedente de trilha[k]; o ciclo na ordem do fluxo é trilha[len-1] -> ... -> trilha[ini] -> trilha[len-1]
			int ini = visto[cur] - 1;
			size_t uso = 0;
			for (int k = len - 1; k >= ini; k--) {
				if (uso + 24 >= sizeof desc) {
					snprintf(desc + uso, sizeof desc - uso, "...");
					uso = sizeof desc;
					break;
				}
				uso += (size_t) snprintf(desc + uso, sizeof desc - uso, "%d -> ", p->ativ[trilha[k]].id);
			}
			if (uso < sizeof desc) snprintf(desc + uso, sizeof desc - uso, "%d", p->ativ[trilha[len - 1]].id);
		}
	}

	free(visto);
	free(trilha);

	if (desc[0] != '\0') definirErro(p, "Há um ciclo nas precedências (%s). O projeto precisa ser uma sequência sem voltas.", desc);
	else definirErro(p, "Há um ciclo nas precedências. O projeto precisa ser uma sequência sem voltas.");
}

/*
 * Ordenação topológica (algoritmo de Kahn). A ordem fica em p->ordem.
 */
static int ordenarTopologicamente(struct Pert * p) {
	int n = p->qtd;
	int * grau = malloc(n * sizeof(int));
	p->ordem = malloc(n * sizeof(int));
	if (grau == NULL || p->ordem == NULL) {
		free(grau);
		definirErro(p, "Falha de alocação de memória.");
		return 1;
	}

	int ini = 0, fim = 0;
	for (int i = 0; i < n; i++) {
		grau[i] = p->ativ[i].qtdPred;
		if (grau[i] == 0) p->ordem[fim++] = i;
	}
	while (ini < fim) {
		int u = p->ordem[ini++];
		for (int k = 0; k < p->ativ[u].qtdSuc; k++) {
			int s = p->ativ[u].suc[k];
			if (--grau[s] == 0) p->ordem[fim++] = s;
		}
	}

	if (fim < n) {
		descreverCiclo(p, grau);
		free(grau);
		return 1;
	}
	free(grau);
	return 0;
}

static void calcularTempos(struct Pert * p) {
	int n = p->qtd;
	struct Atividade * a = p->ativ;
	int total = 0;

	//Passagem para frente: tempos mínimos
	for (int t = 0; t < n; t++) {
		int u = p->ordem[t];
		int inicio = 1;
		int nivel = 0;
		for (int k = 0; k < a[u].qtdPred; k++) {
			int v = a[u].pred[k];
			if (a[v].fimMin + 1 > inicio) inicio = a[v].fimMin + 1;
			if (a[v].nivel + 1 > nivel) nivel = a[v].nivel + 1;
		}
		a[u].inicioMin = inicio;
		a[u].fimMin = inicio + a[u].duracao - 1;
		a[u].nivel = nivel;
		if (a[u].fimMin > total) total = a[u].fimMin;
	}
	p->duracaoTotal = total;
	p->tempoFim = total + 1;

	//Passagem para trás: tempos máximos e folgas
	for (int t = n - 1; t >= 0; t--) {
		int u = p->ordem[t];
		int fim = total;
		for (int k = 0; k < a[u].qtdSuc; k++) {
			int s = a[u].suc[k];
			if (a[s].inicioMax - 1 < fim) fim = a[s].inicioMax - 1;
		}
		a[u].fimMax = fim;
		a[u].inicioMax = fim - a[u].duracao + 1;
		a[u].folga = a[u].inicioMax - a[u].inicioMin;
		a[u].critica = (a[u].folga == 0);
	}
}

int pertCalcular(struct Pert * p) {
	if (p->qtd == 0) {
		definirErro(p, "Nenhuma atividade foi informada.");
		return 1;
	}

	liberarCalculo(p);
	if (resolverPrecedentes(p) || construirSucessores(p) || ordenarTopologicamente(p)) {
		liberarCalculo(p);
		return 1;
	}

	calcularTempos(p);
	p->calculado = true;
	return 0;
}

bool pertArestaCritica(const struct Pert * p, int u, int v) {
	const struct Atividade * a = &p->ativ[u];
	const struct Atividade * b = &p->ativ[v];
	return a->critica && b->critica && a->fimMin + 1 == b->inicioMin;
}

int pertPrimeiroCaminhoCritico(const struct Pert * p, int * ids, int capacidade) {
	if (!p->calculado) return 0;

	int u = -1;
	for (int i = 0; i < p->qtd; i++) {
		if (p->ativ[i].critica && p->ativ[i].qtdPred == 0) {
			u = i;
			break;
		}
	}

	int len = 0;
	while (u >= 0) {
		if (len < capacidade) ids[len] = p->ativ[u].id;
		len++;

		int prox = -1;
		for (int k = 0; k < p->ativ[u].qtdSuc; k++) {
			if (pertArestaCritica(p, u, p->ativ[u].suc[k])) {
				prox = p->ativ[u].suc[k];
				break;
			}
		}
		u = prox;
	}
	return len;
}

unsigned long pertContarCaminhosCriticos(const struct Pert * p) {
	if (!p->calculado) return 0;

	int n = p->qtd;
	unsigned long * c = calloc(n, sizeof *c);
	if (c == NULL) return 0;

	//Programação dinâmica na ordem topológica inversa: caminhos críticos que saem de cada atividade
	for (int t = n - 1; t >= 0; t--) {
		int u = p->ordem[t];
		if (!p->ativ[u].critica) continue;

		unsigned long soma = 0;
		bool temSucessor = false;
		for (int k = 0; k < p->ativ[u].qtdSuc; k++) {
			int s = p->ativ[u].suc[k];
			if (pertArestaCritica(p, u, s)) {
				temSucessor = true;
				soma += c[s];
				if (soma > CAP_CAMINHOS) soma = CAP_CAMINHOS;
			}
		}
		c[u] = temSucessor ? soma : 1;
	}

	unsigned long total = 0;
	for (int i = 0; i < n; i++) {
		if (p->ativ[i].critica && p->ativ[i].qtdPred == 0) {
			total += c[i];
			if (total > CAP_CAMINHOS) total = CAP_CAMINHOS;
		}
	}

	free(c);
	return total;
}

unsigned long pertImprimirCaminhosCriticos(const struct Pert * p, FILE * out, unsigned long limite) {
	unsigned long total = pertContarCaminhosCriticos(p);
	if (total == 0 || limite == 0) return total;

	int n = p->qtd;
	int * caminho = malloc(n * sizeof(int));
	int * prox = malloc(n * sizeof(int));
	bool * achou = malloc(n * sizeof(bool));
	if (caminho == NULL || prox == NULL || achou == NULL) {
		free(caminho);
		free(prox);
		free(achou);
		return total;
	}

	//Busca em profundidade (iterativa) só pelos arcos críticos, a partir de cada início crítico
	unsigned long impressos = 0;
	for (int s = 0; s < n && impressos < limite; s++) {
		if (!p->ativ[s].critica || p->ativ[s].qtdPred != 0) continue;

		int d = 0;
		caminho[0] = s;
		prox[0] = 0;
		achou[0] = false;

		while (d >= 0 && impressos < limite) {
			int u = caminho[d];
			const struct Atividade * a = &p->ativ[u];

			int v = -1;
			while (prox[d] < a->qtdSuc) {
				int cand = a->suc[prox[d]++];
				if (pertArestaCritica(p, u, cand)) {
					v = cand;
					break;
				}
			}

			if (v >= 0) {
				achou[d] = true;
				d++;
				caminho[d] = v;
				prox[d] = 0;
				achou[d] = false;
			} else {
				if (!achou[d]) {
					//Nenhum sucessor crítico: o caminho termina aqui
					fprintf(out, "  ");
					for (int k = 0; k <= d; k++) fprintf(out, k ? " -> %d" : "%d", p->ativ[caminho[k]].id);
					fprintf(out, "\n");
					impressos++;
				}
				d--;
			}
		}
	}

	free(caminho);
	free(prox);
	free(achou);
	return total;
}

/* Largura na tela de um texto UTF-8 (conta caracteres, não bytes) */
static int largura(const char * s) {
	int w = 0;
	for (; *s; s++)
		if (((unsigned char) *s & 0xC0) != 0x80) w++;
	return w;
}

static void celula(FILE * out, const char * txt, int larg, bool direita) {
	int pad = larg - largura(txt);
	if (pad < 0) pad = 0;
	if (direita) fprintf(out, "%*s%s  ", pad, "", txt);
	else fprintf(out, "%s%*s  ", txt, pad, "");
}

static void textoPrecedentes(const struct Pert * p, const struct Atividade * a, char * buf, size_t tam) {
	if (a->qtdPred == 0) {
		snprintf(buf, tam, "-");
		return;
	}

	buf[0] = '\0';
	size_t uso = 0;
	for (int k = 0; k < a->qtdPred; k++) {
		int w = snprintf(buf + uso, tam - uso, k ? ", %d" : "%d", p->ativ[a->pred[k]].id);
		if (w < 0 || (size_t) w >= tam - uso) {
			if (tam >= 4) memcpy(buf + tam - 4, "...", 4);
			return;
		}
		uso += (size_t) w;
	}
}

void pertImprimirTabela(const struct Pert * p, FILE * out) {
	if (!p->calculado) return;

	static const char * cab[] = {"Atividade", "Duração", "Precedentes", "Início mín.", "Fim mín.", "Início máx.", "Fim máx.", "Folga", "Crítica"};
	static const int larg[] = {9, 7, 12, 11, 8, 11, 8, 5, 7};
	enum { COLUNAS = 9 };

	int total = 0;
	for (int c = 0; c < COLUNAS; c++) {
		celula(out, cab[c], larg[c], false);
		total += larg[c] + 2;
	}
	fprintf(out, "\n");
	for (int i = 0; i < total - 2; i++) fputc('-', out);
	fprintf(out, "\n");

	for (int i = 0; i < p->qtd; i++) {
		const struct Atividade * a = &p->ativ[i];
		char b[COLUNAS][256];
		snprintf(b[0], sizeof b[0], "%d", a->id);
		snprintf(b[1], sizeof b[1], "%d", a->duracao);
		textoPrecedentes(p, a, b[2], sizeof b[2]);
		snprintf(b[3], sizeof b[3], "%d", a->inicioMin);
		snprintf(b[4], sizeof b[4], "%d", a->fimMin);
		snprintf(b[5], sizeof b[5], "%d", a->inicioMax);
		snprintf(b[6], sizeof b[6], "%d", a->fimMax);
		snprintf(b[7], sizeof b[7], "%d", a->folga);
		snprintf(b[8], sizeof b[8], "%s", a->critica ? "Sim" : "Não");

		for (int c = 0; c < COLUNAS; c++) celula(out, b[c], larg[c], c != 2 && c != 8);
		fprintf(out, "\n");
	}
}

void pertImprimirResumo(const struct Pert * p, FILE * out) {
	if (!p->calculado) return;

	fprintf(out, "Duração total do projeto: %d (o vértice Fim ocorre no instante %d)\n", p->duracaoTotal, p->tempoFim);

	fprintf(out, "Atividades críticas (folga 0): ");
	bool primeiro = true;
	for (int i = 0; i < p->qtd; i++) {
		if (!p->ativ[i].critica) continue;
		fprintf(out, primeiro ? "%d" : ", %d", p->ativ[i].id);
		primeiro = false;
	}
	fprintf(out, "\n");

	fprintf(out, "Atividades com folga: ");
	primeiro = true;
	for (int i = 0; i < p->qtd; i++) {
		if (p->ativ[i].critica) continue;
		fprintf(out, primeiro ? "%d (folga %d)" : ", %d (folga %d)", p->ativ[i].id, p->ativ[i].folga);
		primeiro = false;
	}
	if (primeiro) fprintf(out, "nenhuma");
	fprintf(out, "\n");

	unsigned long total = pertContarCaminhosCriticos(p);
	if (total == 1) {
		fprintf(out, "Caminho crítico (duração %d):\n", p->duracaoTotal);
	} else if (total >= CAP_CAMINHOS) {
		fprintf(out, "Caminhos críticos (mais de %lu, duração %d cada):\n", CAP_CAMINHOS, p->duracaoTotal);
	} else {
		fprintf(out, "Caminhos críticos (%lu caminhos, duração %d cada):\n", total, p->duracaoTotal);
	}
	pertImprimirCaminhosCriticos(p, out, LIMITE_CAMINHOS_IMPRESSOS);
	if (total > LIMITE_CAMINHOS_IMPRESSOS) {
		fprintf(out, "  ... e mais %lu caminho(s) crítico(s) não mostrados.\n", total - LIMITE_CAMINHOS_IMPRESSOS);
	}
}
