#ifndef H_PERT
#define H_PERT 1

#include <stdio.h>
#include <stdbool.h>

/*
 * Módulo PERT/CPM. Não depende de Raylib nem de nenhuma biblioteca externa,
 * só da biblioteca padrão do C.
 */

/* Limites aceitos (evitam estouro de int nos cálculos de tempo) */
#define PERT_MAX_ATIVIDADES 10000
#define PERT_MAX_ID 999999
#define PERT_MAX_DURACAO 100000

/*
 * Uma atividade da tabela. Cada atividade é um vértice do grafo PERT/CPM.
 *
 * Convenção de tempo (a mesma da aula): o projeto começa no instante 1, uma
 * atividade de duração d que começa em t termina em t + d - 1, e a atividade
 * seguinte pode começar em t + d.
 */
struct Atividade {
	int id;
	int duracao;

	int qtdPredId;
	int * predId;	//Ids dos precedentes, como foram informados

	int qtdPred;
	int * pred;		//Posições (no vetor de atividades) dos precedentes
	int qtdSuc;
	int * suc;		//Posições (no vetor de atividades) dos sucessores

	int inicioMin;	//Início mais cedo (tempo mínimo para começar)
	int fimMin;		//Fim mais cedo
	int inicioMax;	//Início mais tarde (tempo máximo para começar sem atrasar o projeto)
	int fimMax;		//Fim mais tarde
	int folga;		//inicioMax - inicioMin
	bool critica;	//folga == 0
	int nivel;		//Coluna do desenho: 0 sem precedentes, senão 1 + maior nível dos precedentes
};

struct Pert {
	int qtd;
	int capacidade;
	struct Atividade * ativ;
	int * ordem;		//Ordem topológica (posições), preenchida por pertCalcular
	int duracaoTotal;	//Maior fim mínimo
	int tempoFim;		//Instante do vértice "Fim" (duracaoTotal + 1)
	bool calculado;
	char erro[512];		//Mensagem do último erro
};

/*
 * Inicializa a estrutura vazia.
 */
void pertIniciar(struct Pert * p);

/*
 * Libera toda a memória e deixa a estrutura vazia, pronta para reuso.
 */
void pertLiberar(struct Pert * p);

/*
 * Adiciona uma linha da tabela (atividade, duração e ids dos precedentes).
 * Os precedentes podem ser atividades que ainda não foram adicionadas.
 * Retorna 0 em sucesso e 1 em falha (mensagem em p->erro).
 */
int pertAdicionar(struct Pert * p, int id, int duracao, const int * precedentes, int qtdPrecedentes);

/*
 * Lê a tabela de um arquivo (ou stdin), uma atividade por linha:
 *     atividade duração [precedentes]
 * Os campos podem ser separados por espaço, tab, vírgula ou ponto e vírgula.
 * Aceita linha de cabeçalho, linhas em branco e comentários com #.
 * Retorna 0 em sucesso e 1 em falha (mensagem em p->erro, com o número da linha).
 */
int pertLerTabela(struct Pert * p, FILE * f);

/*
 * Carrega a tabela de exemplo da aula (6 atividades).
 */
int pertCarregarExemplo(struct Pert * p);

/*
 * Valida a tabela e calcula início/fim mínimos e máximos, folgas e atividades críticas.
 * Detecta ids inexistentes e ciclos entre as precedências.
 * Retorna 0 em sucesso e 1 em falha (mensagem em p->erro).
 */
int pertCalcular(struct Pert * p);

/*
 * Retorna se o arco u -> v (posições no vetor) faz parte de um caminho crítico:
 * as duas atividades são críticas e v começa logo depois que u termina.
 */
bool pertArestaCritica(const struct Pert * p, int u, int v);

/*
 * Escreve em ids[] o primeiro caminho crítico (ids das atividades).
 * Retorna o tamanho total do caminho, que pode ser maior que a capacidade.
 */
int pertPrimeiroCaminhoCritico(const struct Pert * p, int * ids, int capacidade);

/*
 * Conta quantos caminhos críticos existem (limitado a 1 bilhão).
 */
unsigned long pertContarCaminhosCriticos(const struct Pert * p);

/*
 * Imprime até "limite" caminhos críticos, um por linha, no formato "1 -> 3 -> 4 -> 5".
 * Retorna a quantidade total de caminhos críticos.
 */
unsigned long pertImprimirCaminhosCriticos(const struct Pert * p, FILE * out, unsigned long limite);

/*
 * Imprime a tabela com todos os tempos e folgas.
 */
void pertImprimirTabela(const struct Pert * p, FILE * out);

/*
 * Imprime duração total, atividades críticas, atividades com folga e caminho(s) crítico(s).
 */
void pertImprimirResumo(const struct Pert * p, FILE * out);

#endif
