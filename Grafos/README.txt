Algoritmos em Grafos - Prof Rodrigo Calvo
Universidade Estadual de Maringá - 2016

Código implementado por Bruno Emori (RA 88736) & Christian Nakata (RA 90558)
/--------------------------------------------------------------------------\
O programa foi escrito em Linguagem C, utilizando-se o compilador GCC, no Sistema Operacional Ubuntu 15.04 (Linux).

Para compilar:
  - Abra o terminal e vá no diretório onde se localiza o código fonte;
  - Digite "gcc main.c adjlist.c graphfunctions.c", sem aspas;

Para executar:
  - Crie um arquivo de texto para definir a entrada do programa:
    - A primeira linha se refere a quantidade de vértices;
    - A segunda linha se refere a quantidade de arestas;
    - A terceira linha admite apenas 0 (grafos não direcionados) ou 1(grafos direcionados);
    - A linhas seguintes definem um vértice v1, outro vértice v2 e o peso da aresta entre v1 e v2. Caso não seja um grafo ponderado, defina os pesos como 0;
  - Digite no terminal "./a.out < (arquivo_de_entrada)", onde (arquivo_de_entrada) é o seu grafo;
  - Dois grafos de exemplo estão no arquivo, caso haja dúvidas em relação ao arquivo de entrada;

