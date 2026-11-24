# Implementação de DFS, BFS, Fecho Transitivo Direto/Inverso e Conectividade por Vetor Dinâmico de Listas Encadeadas 

### Como baixar e compilar (Linux)

```
$ git clone https://github.com/lutsc/grafos-ex1
$ cd grafos-ex1

$ make clean
$ make run
```

### Como baixar e compilar (Windows)

```
$ git clone https://github.com/lutsc/grafos-ex1
$ cd grafos-ex1

$ ./makeAll.sh
$ ./bin/main
```

### Dependências

- Makefile (Apenas Linux)
- Raylib (Linux e Windows)

O programa PERT/CPM (abaixo) **não** usa Raylib: precisa apenas do `gcc` para compilar e o executável gerado roda sozinho.

## PERT/CPM

Recebe uma tabela de atividades, durações e precedentes, monta o grafo PERT/CPM (atividades nos vértices), calcula os tempos mínimos e máximos de início e fim de cada atividade, as folgas e o caminho crítico.

### Como compilar

```
Linux:    $ make pert-static        (ou ./makePert.sh)
Windows:  $ ./makePert.sh           (Git Bash / MSYS2 com gcc)
```

O resultado é o executável `bin/pert` (`bin/pert.exe` no Windows). Compilado com `-static`, ele roda em qualquer máquina sem instalar nada. No macOS use `make pert` (sem `-static`).

### Como usar

```
$ ./bin/pert                    menu interativo (só números)
$ ./bin/pert exemplos/aula.txt  lê a tabela de um arquivo
$ ./bin/pert --exemplo          usa a tabela de exemplo da aula
$ ./bin/pert tabela.txt -o grafo.svg
```

Na tela aparecem a tabela com todos os tempos e folgas, a duração total e o(s) caminho(s) crítico(s). O desenho do grafo é salvo em `pert.svg`, que abre em qualquer navegador.

Notação do desenho (a mesma da aula): em cada atividade, em cima ficam o início mínimo, a duração e o fim mínimo; embaixo, o início máximo e o fim máximo; abaixo, a folga. As setas do caminho crítico são vermelhas e o vértice `Fim` fecha o grafo.

Convenção de tempo: o projeto começa no instante 1 e uma atividade de duração `d` que começa em `t` termina em `t + d - 1`.

### Formato do arquivo de entrada

Uma atividade por linha: `atividade duração precedentes`. Os precedentes podem ser separados por vírgula ou espaço, e uma atividade sem precedentes fica sem nada ou com `-`. O cabeçalho, as linhas em branco e os comentários (`#`) são ignorados, então dá para copiar a tabela direto do enunciado:

```
Atividade Duração Precedente
1 10
2 4 1
3 7 1
4 5 3
5 5 2,4
6 2 3
```

O programa avisa se algum precedente não existe, se uma atividade aparece duas vezes ou se há ciclo entre as precedências (mostrando o ciclo encontrado).

### Testes

```
$ make test-pert
```