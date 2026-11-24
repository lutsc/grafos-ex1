#include "pert_svg.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

/*
 * Desenho em camadas (colunas): cada atividade fica na coluna do seu "nivel".
 * Arcos que pulam colunas passam por pontos intermediários, um em cada coluna,
 * para não atravessar outras atividades. A ordem vertical dentro de cada coluna
 * é escolhida para reduzir cruzamentos (heurística da média das posições dos vizinhos)
 * e as posições verticais são ajustadas para deixar os arcos o mais retos possível.
 */

#define RAIO 30.0
#define LARG_COLUNA 190.0
#define MARGEM_ESQ 130.0
#define MARGEM_TOPO 130.0
#define MARGEM_BASE 130.0
#define SEP_ATIV 135.0
#define SEP_MISTO 85.0
#define SEP_PONTO 40.0
#define FIM_LARG 84.0
#define FIM_ALT 44.0
#define MAX_SEG_CRUZAMENTOS 4000
#define VARREDURAS_ORDEM 24
#define VARREDURAS_Y 20

#define COR_CRITICA "#d40000"

enum TipoNo { NO_ATIV, NO_FIM, NO_PONTO };

struct NoL {
	int tipo;
	int ativ;		//Posição da atividade (tipo NO_ATIV)
	int nivel;
	int ordem;		//Posição vertical dentro da coluna
	double y;
};

struct ArestaL {
	int len;
	int * no;		//Sequência de nós do desenho, da origem ao destino
	bool critica;
};

struct Layout {
	int qtdNos, capNos;
	struct NoL * nos;
	int qtdArestas, capArestas;
	struct ArestaL * arestas;

	int nivelFim;
	int * nivelQtd;
	int ** nivelNos;	//Nós de cada coluna, de cima para baixo

	int * offC, * adjC;	//Vizinhos da coluna anterior (formato CSR)
	int * offB, * adjB;	//Vizinhos da coluna seguinte (formato CSR)
	int segmentos;
};

static void liberarLayout(struct Layout * L) {
	free(L->nos);
	for (int i = 0; i < L->qtdArestas; i++) free(L->arestas[i].no);
	free(L->arestas);
	if (L->nivelNos != NULL) {
		for (int l = 0; l <= L->nivelFim; l++) free(L->nivelNos[l]);
	}
	free(L->nivelNos);
	free(L->nivelQtd);
	free(L->offC);
	free(L->adjC);
	free(L->offB);
	free(L->adjB);
	memset(L, 0, sizeof *L);
}

static int adicionarNo(struct Layout * L, int tipo, int ativ, int nivel) {
	if (L->qtdNos == L->capNos) {
		int nova = L->capNos ? L->capNos * 2 : 64;
		struct NoL * tmp = realloc(L->nos, nova * sizeof *tmp);
		if (tmp == NULL) return -1;
		L->nos = tmp;
		L->capNos = nova;
	}
	struct NoL * no = &L->nos[L->qtdNos];
	no->tipo = tipo;
	no->ativ = ativ;
	no->nivel = nivel;
	no->ordem = 0;
	no->y = 0;
	return L->qtdNos++;
}

/*
 * Cria o arco origem -> destino, com um ponto intermediário em cada coluna que ele atravessa.
 */
static int adicionarAresta(struct Layout * L, int origem, int destino, bool critica) {
	int nivelOrigem = L->nos[origem].nivel;
	int intermediarios = L->nos[destino].nivel - nivelOrigem - 1;

	if (L->qtdArestas == L->capArestas) {
		int nova = L->capArestas ? L->capArestas * 2 : 64;
		struct ArestaL * tmp = realloc(L->arestas, nova * sizeof *tmp);
		if (tmp == NULL) return 1;
		L->arestas = tmp;
		L->capArestas = nova;
	}

	struct ArestaL * e = &L->arestas[L->qtdArestas];
	e->len = 0;
	e->critica = critica;
	e->no = malloc((intermediarios + 2) * sizeof(int));
	if (e->no == NULL) return 1;
	L->qtdArestas++;	//A partir daqui liberarLayout cuida do vetor

	int len = 0;
	e->no[len++] = origem;
	for (int g = 0; g < intermediarios; g++) {
		int id = adicionarNo(L, NO_PONTO, -1, nivelOrigem + 1 + g);
		if (id < 0) {
			L->arestas[L->qtdArestas - 1].len = len;
			return 1;
		}
		L->arestas[L->qtdArestas - 1].no[len++] = id;
	}
	L->arestas[L->qtdArestas - 1].no[len++] = destino;
	L->arestas[L->qtdArestas - 1].len = len;
	return 0;
}

static int construir(const struct Pert * p, struct Layout * L) {
	int n = p->qtd;

	int maiorNivel = 0;
	for (int i = 0; i < n; i++)
		if (p->ativ[i].nivel > maiorNivel) maiorNivel = p->ativ[i].nivel;
	L->nivelFim = maiorNivel + 1;

	//Os n primeiros nós são as atividades (mesma posição do vetor) e o seguinte é o Fim
	for (int i = 0; i < n; i++)
		if (adicionarNo(L, NO_ATIV, i, p->ativ[i].nivel) != i) return 1;
	int fim = adicionarNo(L, NO_FIM, -1, L->nivelFim);
	if (fim != n) return 1;

	for (int u = 0; u < n; u++) {
		const struct Atividade * a = &p->ativ[u];
		for (int k = 0; k < a->qtdSuc; k++) {
			if (adicionarAresta(L, u, a->suc[k], pertArestaCritica(p, u, a->suc[k]))) return 1;
		}
		if (a->qtdSuc == 0) {
			if (adicionarAresta(L, u, fim, a->critica)) return 1;
		}
	}

	int N = L->qtdNos;

	//Nós de cada coluna
	L->nivelQtd = calloc(L->nivelFim + 1, sizeof(int));
	L->nivelNos = calloc(L->nivelFim + 1, sizeof(int *));
	if (L->nivelQtd == NULL || L->nivelNos == NULL) return 1;
	for (int v = 0; v < N; v++) L->nivelQtd[L->nos[v].nivel]++;
	for (int l = 0; l <= L->nivelFim; l++) {
		L->nivelNos[l] = malloc((L->nivelQtd[l] > 0 ? L->nivelQtd[l] : 1) * sizeof(int));
		if (L->nivelNos[l] == NULL) return 1;
		L->nivelQtd[l] = 0;
	}
	for (int v = 0; v < N; v++) {
		int l = L->nos[v].nivel;
		L->nos[v].ordem = L->nivelQtd[l];
		L->nivelNos[l][L->nivelQtd[l]++] = v;
	}

	//Vizinhos entre colunas consecutivas
	L->offC = calloc(N + 1, sizeof(int));
	L->offB = calloc(N + 1, sizeof(int));
	if (L->offC == NULL || L->offB == NULL) return 1;
	int seg = 0;
	for (int e = 0; e < L->qtdArestas; e++) {
		for (int k = 0; k + 1 < L->arestas[e].len; k++) {
			L->offB[L->arestas[e].no[k] + 1]++;
			L->offC[L->arestas[e].no[k + 1] + 1]++;
			seg++;
		}
	}
	for (int v = 0; v < N; v++) {
		L->offB[v + 1] += L->offB[v];
		L->offC[v + 1] += L->offC[v];
	}
	L->segmentos = seg;
	L->adjB = malloc((seg > 0 ? seg : 1) * sizeof(int));
	L->adjC = malloc((seg > 0 ? seg : 1) * sizeof(int));
	int * pB = calloc(N, sizeof(int));
	int * pC = calloc(N, sizeof(int));
	if (L->adjB == NULL || L->adjC == NULL || pB == NULL || pC == NULL) {
		free(pB);
		free(pC);
		return 1;
	}
	for (int e = 0; e < L->qtdArestas; e++) {
		for (int k = 0; k + 1 < L->arestas[e].len; k++) {
			int a = L->arestas[e].no[k], b = L->arestas[e].no[k + 1];
			L->adjB[L->offB[a] + pB[a]++] = b;
			L->adjC[L->offC[b] + pC[b]++] = a;
		}
	}
	free(pB);
	free(pC);
	return 0;
}

/* Reordena uma coluna pela média das posições dos vizinhos da coluna anterior (ou seguinte) */
static void reordenarNivel(struct Layout * L, int l, bool usarAnterior) {
	int q = L->nivelQtd[l];
	int * lista = L->nivelNos[l];
	double * chave = malloc((q > 0 ? q : 1) * sizeof(double));
	if (chave == NULL) return;

	for (int i = 0; i < q; i++) {
		int v = lista[i];
		const int * off = usarAnterior ? L->offC : L->offB;
		const int * adj = usarAnterior ? L->adjC : L->adjB;
		int ini = off[v], fim = off[v + 1];
		if (fim > ini) {
			double soma = 0;
			for (int k = ini; k < fim; k++) soma += L->nos[adj[k]].ordem;
			chave[i] = soma / (fim - ini);
		} else {
			chave[i] = L->nos[v].ordem;
		}
	}

	//Inserção estável: empates mantêm a ordem atual
	for (int i = 1; i < q; i++) {
		double kv = chave[i];
		int nv = lista[i];
		int j = i - 1;
		while (j >= 0 && chave[j] > kv) {
			chave[j + 1] = chave[j];
			lista[j + 1] = lista[j];
			j--;
		}
		chave[j + 1] = kv;
		lista[j + 1] = nv;
	}
	for (int i = 0; i < q; i++) L->nos[lista[i]].ordem = i;
	free(chave);
}

static long cruzamentos(const struct Layout * L) {
	long total = 0;
	for (int l = 0; l < L->nivelFim; l++) {
		int cont = 0;
		for (int i = 0; i < L->nivelQtd[l]; i++) {
			int v = L->nivelNos[l][i];
			cont += L->offB[v + 1] - L->offB[v];
		}
		int * sa = malloc((cont > 0 ? cont : 1) * sizeof(int));
		int * sb = malloc((cont > 0 ? cont : 1) * sizeof(int));
		if (sa == NULL || sb == NULL) {
			free(sa);
			free(sb);
			return total;
		}
		int m = 0;
		for (int i = 0; i < L->nivelQtd[l]; i++) {
			int v = L->nivelNos[l][i];
			for (int k = L->offB[v]; k < L->offB[v + 1]; k++) {
				sa[m] = L->nos[v].ordem;
				sb[m] = L->nos[L->adjB[k]].ordem;
				m++;
			}
		}
		for (int i = 0; i < m; i++)
			for (int j = 0; j < i; j++)
				if ((long) (sa[i] - sa[j]) * (sb[i] - sb[j]) < 0) total++;
		free(sa);
		free(sb);
	}
	return total;
}

static void restaurarOrdem(struct Layout * L, const int * ordem) {
	for (int v = 0; v < L->qtdNos; v++) {
		L->nos[v].ordem = ordem[v];
		L->nivelNos[L->nos[v].nivel][ordem[v]] = v;
	}
}

static void ordenar(struct Layout * L) {
	bool medir = L->segmentos <= MAX_SEG_CRUZAMENTOS;
	int * melhor = malloc(L->qtdNos * sizeof(int));
	long melhorCruz = 0;

	if (medir && melhor != NULL) {
		melhorCruz = cruzamentos(L);
		for (int v = 0; v < L->qtdNos; v++) melhor[v] = L->nos[v].ordem;
	}

	for (int it = 0; it < VARREDURAS_ORDEM; it++) {
		if (it % 2 == 0) {
			for (int l = 1; l <= L->nivelFim; l++) reordenarNivel(L, l, true);
		} else {
			for (int l = L->nivelFim - 1; l >= 0; l--) reordenarNivel(L, l, false);
		}
		if (medir && melhor != NULL) {
			long c = cruzamentos(L);
			if (c < melhorCruz) {
				melhorCruz = c;
				for (int v = 0; v < L->qtdNos; v++) melhor[v] = L->nos[v].ordem;
			}
		}
	}

	if (medir && melhor != NULL) restaurarOrdem(L, melhor);
	free(melhor);
}

/*
 * Regressão isotônica (pool adjacent violators): ajusta z para uma sequência
 * não decrescente com o menor erro quadrático.
 */
static void isotonica(const double * z, int n, double * saida) {
	double * soma = malloc((n > 0 ? n : 1) * sizeof(double));
	int * cont = malloc((n > 0 ? n : 1) * sizeof(int));
	if (soma == NULL || cont == NULL) {
		for (int i = 0; i < n; i++) saida[i] = z[i];
		free(soma);
		free(cont);
		return;
	}

	int nb = 0;
	for (int i = 0; i < n; i++) {
		soma[nb] = z[i];
		cont[nb] = 1;
		nb++;
		while (nb > 1 && soma[nb - 2] / cont[nb - 2] > soma[nb - 1] / cont[nb - 1]) {
			soma[nb - 2] += soma[nb - 1];
			cont[nb - 2] += cont[nb - 1];
			nb--;
		}
	}

	int k = 0;
	for (int b = 0; b < nb; b++) {
		double v = soma[b] / cont[b];
		for (int j = 0; j < cont[b]; j++) saida[k++] = v;
	}
	free(soma);
	free(cont);
}

static double separacao(const struct Layout * L, int a, int b) {
	bool pa = L->nos[a].tipo == NO_PONTO;
	bool pb = L->nos[b].tipo == NO_PONTO;
	if (!pa && !pb) return SEP_ATIV;
	if (pa && pb) return SEP_PONTO;
	return SEP_MISTO;
}

/*
 * Define o y dos nós de uma coluna. Sem "relaxar", centraliza a coluna.
 * Com "relaxar", cada nó tenta ficar na média do y dos vizinhos, respeitando
 * a distância mínima entre os nós da coluna.
 */
static void ajustarNivel(struct Layout * L, int l, bool relaxar) {
	int q = L->nivelQtd[l];
	if (q == 0) return;
	int * lista = L->nivelNos[l];

	double * cum = malloc(q * sizeof(double));
	double * z = malloc(q * sizeof(double));
	double * r = malloc(q * sizeof(double));
	if (cum == NULL || z == NULL || r == NULL) {
		free(cum);
		free(z);
		free(r);
		return;
	}

	cum[0] = 0;
	for (int i = 1; i < q; i++) cum[i] = cum[i - 1] + separacao(L, lista[i - 1], lista[i]);

	for (int i = 0; i < q; i++) {
		int v = lista[i];
		double desejo;
		if (!relaxar) {
			desejo = cum[i] - cum[q - 1] / 2.0;
		} else {
			double soma = 0;
			int cont = 0;
			for (int k = L->offC[v]; k < L->offC[v + 1]; k++, cont++) soma += L->nos[L->adjC[k]].y;
			for (int k = L->offB[v]; k < L->offB[v + 1]; k++, cont++) soma += L->nos[L->adjB[k]].y;
			desejo = cont > 0 ? soma / cont : L->nos[v].y;
		}
		z[i] = desejo - cum[i];
	}

	isotonica(z, q, r);
	for (int i = 0; i < q; i++) L->nos[lista[i]].y = r[i] + cum[i];

	free(cum);
	free(z);
	free(r);
}

/* Define o y de todos os nós; devolve a altura total do desenho */
static double posicionar(struct Layout * L) {
	for (int l = 0; l <= L->nivelFim; l++) ajustarNivel(L, l, false);

	for (int it = 0; it < VARREDURAS_Y; it++) {
		if (it % 2 == 0) {
			for (int l = 0; l <= L->nivelFim; l++) ajustarNivel(L, l, true);
		} else {
			for (int l = L->nivelFim; l >= 0; l--) ajustarNivel(L, l, true);
		}
	}

	double minY = L->nos[0].y, maxY = L->nos[0].y;
	for (int v = 1; v < L->qtdNos; v++) {
		if (L->nos[v].y < minY) minY = L->nos[v].y;
		if (L->nos[v].y > maxY) maxY = L->nos[v].y;
	}
	double desloc = MARGEM_TOPO - minY;
	for (int v = 0; v < L->qtdNos; v++) L->nos[v].y += desloc;

	return (maxY + desloc) + MARGEM_BASE;
}

static double posX(const struct Layout * L, int v) {
	return MARGEM_ESQ + L->nos[v].nivel * LARG_COLUNA;
}

static void desenharAresta(const struct Layout * L, const struct ArestaL * e, FILE * out) {
	int len = e->len;
	double * px = malloc(len * sizeof(double));
	double * py = malloc(len * sizeof(double));
	if (px == NULL || py == NULL) {
		free(px);
		free(py);
		return;
	}

	for (int k = 0; k < len; k++) {
		px[k] = posX(L, e->no[k]);
		py[k] = L->nos[e->no[k]].y;
	}

	//A seta sai da borda do círculo da origem...
	if (L->nos[e->no[0]].tipo == NO_ATIV) {
		double dx = px[1] - px[0], dy = py[1] - py[0];
		double d = sqrt(dx * dx + dy * dy);
		if (d > 0) {
			px[0] += dx / d * RAIO;
			py[0] += dy / d * RAIO;
		}
	}

	//...e chega na borda do destino (círculo ou retângulo do Fim)
	int u = len - 1;
	if (L->nos[e->no[u]].tipo == NO_FIM) {
		double xe = px[u] - FIM_LARG / 2.0;
		double t = (xe - px[u - 1]) / (px[u] - px[u - 1]);
		py[u] = py[u - 1] + t * (py[u] - py[u - 1]);
		px[u] = xe;
	} else {
		double dx = px[u] - px[u - 1], dy = py[u] - py[u - 1];
		double d = sqrt(dx * dx + dy * dy);
		if (d > 0) {
			px[u] -= dx / d * (RAIO + 2);
			py[u] -= dy / d * (RAIO + 2);
		}
	}

	fprintf(out, "<path d=\"M%.1f %.1f", px[0], py[0]);
	for (int k = 1; k < len; k++) fprintf(out, " L%.1f %.1f", px[k], py[k]);
	if (e->critica) fprintf(out, "\" fill=\"none\" stroke=\"" COR_CRITICA "\" stroke-width=\"2.6\" marker-end=\"url(#setaCritica)\"/>\n");
	else fprintf(out, "\" fill=\"none\" stroke=\"#000\" stroke-width=\"1.6\" marker-end=\"url(#setaNormal)\"/>\n");

	free(px);
	free(py);
}

static void escreverSVG(const struct Pert * p, const struct Layout * L, FILE * out, double largura, double altura) {
	fprintf(out, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
	fprintf(out, "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"%.0f\" height=\"%.0f\" viewBox=\"0 0 %.0f %.0f\">\n", largura, altura, largura, altura);
	fprintf(out, "<title>Grafo PERT/CPM</title>\n");
	fprintf(out, "<style>\n"
		"text{font-family:Arial,Helvetica,sans-serif}\n"
		".num{font-size:16px;fill:" COR_CRITICA "}\n"
		".dur{font-size:17px;fill:#000}\n"
		".idn{font-size:20px;font-weight:bold;fill:#000}\n"
		".f0{font-size:16px;font-weight:bold;fill:#1a1aa6}\n"
		".f1{font-size:16px;font-weight:bold;fill:#1e7a1e}\n"
		".tit{font-size:22px;font-weight:bold;fill:#222}\n"
		".cam{font-size:16px;font-weight:bold;fill:" COR_CRITICA "}\n"
		".leg{font-size:13px;fill:#555}\n"
		"</style>\n");
	fprintf(out, "<defs>\n"
		"<marker id=\"setaNormal\" viewBox=\"0 0 10 10\" refX=\"10\" refY=\"5\" markerWidth=\"13\" markerHeight=\"13\" markerUnits=\"userSpaceOnUse\" orient=\"auto\"><path d=\"M0 0 L10 5 L0 10 z\" fill=\"#000\"/></marker>\n"
		"<marker id=\"setaCritica\" viewBox=\"0 0 10 10\" refX=\"10\" refY=\"5\" markerWidth=\"15\" markerHeight=\"15\" markerUnits=\"userSpaceOnUse\" orient=\"auto\"><path d=\"M0 0 L10 5 L0 10 z\" fill=\"" COR_CRITICA "\"/></marker>\n"
		"</defs>\n");
	fprintf(out, "<rect width=\"100%%\" height=\"100%%\" fill=\"#fff\"/>\n");

	//Título e caminho crítico
	fprintf(out, "<text class=\"tit\" x=\"24\" y=\"34\">Grafo PERT/CPM — duração total: %d</text>\n", p->duracaoTotal);
	int ids[24];
	int tamCam = pertPrimeiroCaminhoCritico(p, ids, 24);
	unsigned long qtdCam = pertContarCaminhosCriticos(p);
	fprintf(out, "<text class=\"cam\" x=\"24\" y=\"60\">");
	if (qtdCam > 1) fprintf(out, "%lu caminhos críticos (primeiro: ", qtdCam);
	else fprintf(out, "Caminho crítico: ");
	for (int k = 0; k < tamCam && k < 24; k++) fprintf(out, k ? " → %d" : "%d", ids[k]);
	if (tamCam > 24) fprintf(out, " → …");
	if (qtdCam > 1) fprintf(out, ")");
	fprintf(out, "</text>\n");

	//Setas de entrada nas atividades sem precedentes
	for (int i = 0; i < p->qtd; i++) {
		if (p->ativ[i].qtdPred != 0) continue;
		double x = posX(L, i), y = L->nos[i].y;
		bool crit = p->ativ[i].critica;
		fprintf(out, "<path d=\"M%.1f %.1f L%.1f %.1f\" fill=\"none\" stroke=\"%s\" stroke-width=\"%.1f\" marker-end=\"url(#%s)\"/>\n",
			x - RAIO - 64, y, x - RAIO - 2, y, crit ? COR_CRITICA : "#000", crit ? 2.6 : 1.6, crit ? "setaCritica" : "setaNormal");
	}

	//Arcos: primeiro os normais, depois os críticos por cima
	for (int e = 0; e < L->qtdArestas; e++)
		if (!L->arestas[e].critica) desenharAresta(L, &L->arestas[e], out);
	for (int e = 0; e < L->qtdArestas; e++)
		if (L->arestas[e].critica) desenharAresta(L, &L->arestas[e], out);

	//Atividades
	for (int i = 0; i < p->qtd; i++) {
		const struct Atividade * a = &p->ativ[i];
		double x = posX(L, i), y = L->nos[i].y;
		fprintf(out, "<circle cx=\"%.1f\" cy=\"%.1f\" r=\"%.0f\" fill=\"#fff\" stroke=\"#000\" stroke-width=\"2\"/>\n", x, y, RAIO);
		fprintf(out, "<text class=\"idn\" x=\"%.1f\" y=\"%.1f\" text-anchor=\"middle\">%d</text>\n", x, y + 7, a->id);
		fprintf(out, "<text class=\"dur\" x=\"%.1f\" y=\"%.1f\" text-anchor=\"middle\">%d</text>\n", x, y - RAIO - 8, a->duracao);
		fprintf(out, "<text class=\"num\" x=\"%.1f\" y=\"%.1f\" text-anchor=\"end\">%d</text>\n", x - 24, y - RAIO - 8, a->inicioMin);
		fprintf(out, "<text class=\"num\" x=\"%.1f\" y=\"%.1f\" text-anchor=\"start\">%d</text>\n", x + 24, y - RAIO - 8, a->fimMin);
		fprintf(out, "<text class=\"num\" x=\"%.1f\" y=\"%.1f\" text-anchor=\"end\">%d</text>\n", x - 24, y + RAIO + 17, a->inicioMax);
		fprintf(out, "<text class=\"num\" x=\"%.1f\" y=\"%.1f\" text-anchor=\"start\">%d</text>\n", x + 24, y + RAIO + 17, a->fimMax);
		fprintf(out, "<text class=\"%s\" x=\"%.1f\" y=\"%.1f\" text-anchor=\"middle\">Folga: %d</text>\n", a->folga == 0 ? "f0" : "f1", x, y + RAIO + 38, a->folga);
	}

	//Vértice Fim
	{
		double x = posX(L, p->qtd), y = L->nos[p->qtd].y;
		fprintf(out, "<rect x=\"%.1f\" y=\"%.1f\" width=\"%.0f\" height=\"%.0f\" fill=\"#fff\" stroke=\"#000\" stroke-width=\"2\"/>\n", x - FIM_LARG / 2.0, y - FIM_ALT / 2.0, FIM_LARG, FIM_ALT);
		fprintf(out, "<text class=\"idn\" x=\"%.1f\" y=\"%.1f\" text-anchor=\"middle\">Fim</text>\n", x, y + 7);
		fprintf(out, "<text class=\"num\" x=\"%.1f\" y=\"%.1f\" text-anchor=\"middle\">%d</text>\n", x, y - FIM_ALT / 2.0 - 8, p->tempoFim);
		fprintf(out, "<text class=\"num\" x=\"%.1f\" y=\"%.1f\" text-anchor=\"middle\">%d</text>\n", x, y + FIM_ALT / 2.0 + 19, p->tempoFim);
	}

	//Legenda
	fprintf(out, "<text class=\"leg\" x=\"24\" y=\"%.0f\">Em cada atividade: em cima, início mínimo | duração | fim mínimo; embaixo, início máximo | fim máximo.</text>\n", altura - 38);
	fprintf(out, "<text class=\"leg\" x=\"24\" y=\"%.0f\">Folga = início máximo − início mínimo. Setas vermelhas: caminho crítico (folga 0).</text>\n", altura - 18);

	fprintf(out, "</svg>\n");
}

int pertGerarSVG(const struct Pert * p, const char * arquivo) {
	if (!p->calculado) return 1;

	struct Layout L;
	memset(&L, 0, sizeof L);
	if (construir(p, &L) != 0) {
		liberarLayout(&L);
		return 1;
	}

	ordenar(&L);
	double altura = posicionar(&L);
	double largura = MARGEM_ESQ + L.nivelFim * LARG_COLUNA + FIM_LARG / 2.0 + 70;
	if (largura < 920) largura = 920;

	FILE * f = fopen(arquivo, "w");
	if (f == NULL) {
		liberarLayout(&L);
		return 1;
	}
	escreverSVG(p, &L, f, largura, altura);
	int falhou = ferror(f);
	if (fclose(f) != 0) falhou = 1;

	liberarLayout(&L);
	return falhou ? 1 : 0;
}
