//
// Algoritmos e Estruturas de Dados --- 2024/2025
//
// Joaquim Madeira - Dec 2024
//
// GraphAllPairsShortestDistances
//

// Student Name : Catarina Rabaça
// Student Number : 119582
// Student Name : Francisco Ribeiro
// Student Number : 118993

/*** COMPLETE THE GraphAllPairsShortestDistancesExecute FUNCTION ***/

#include "GraphAllPairsShortestDistances.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "GraphBellmanFordAlg.h"

struct _GraphAllPairsShortestDistances {
  int** distance;  // The 2D matrix storing the all-pairs shortest distances
                   // It is stored as an array of pointers to 1D rows
                   // Idea: an INDEFINITE distance value is stored as -1
  Graph* graph;
};

// Alocar memória e inicializar a matriz de distâncias
// Calcular as distâncias entre os vértices executando o algoritmo de Bellman-Ford
GraphAllPairsShortestDistances* GraphAllPairsShortestDistancesExecute(
    Graph* g) {
  assert(g != NULL); // Certificar que o grafo fornecido não é nulo

  // Alocam memória para a estrutura principal
  GraphAllPairsShortestDistances* result =
      (GraphAllPairsShortestDistances*)malloc(
          sizeof(struct _GraphAllPairsShortestDistances));
  assert(result != NULL); // Garantem que a memória foi alocada com sucesso

  unsigned int numVertices = GraphGetNumVertices(g);
  result->graph = g; // Guardam o grafo original na estrutura

  // Alocam a matriz de distâncias (array de ponteiros para linhas)
  result->distance = (int**)malloc(numVertices * sizeof(int*));
  assert(result->distance != NULL); // Garantem que a memória foi alocada

  for (unsigned int i = 0; i < numVertices; i++) {
    result->distance[i] = (int*)malloc(numVertices * sizeof(int));
    assert(result->distance[i] != NULL); // Garantem que cada linha foi alocada

    for (unsigned int j = 0; j < numVertices; j++) {
      result->distance[i][j] = -1; // Inicializam todas as distâncias como indefinidas (-1)
    }
  }

  // Iteram sobre todos os vértices para calcular as distâncias mais curtas
  for (unsigned int i = 0; i < numVertices; i++) {
    // Executam o algoritmo de Bellman-Ford a partir do vértice i
    GraphBellmanFordAlg* bfResult = GraphBellmanFordAlgExecute(g, i);

    for (unsigned int j = 0; j < numVertices; j++) {
      result->distance[i][j] = GraphBellmanFordAlgDistance(bfResult, j);
      // Guardam a distância calculada do vértice i para o vértice j
    }

    // Libertam a memória usada pela execução de Bellman-Ford
    GraphBellmanFordAlgDestroy(&bfResult);
  }

  return result; // Retornam a estrutura preenchida com a matriz de distâncias
}

void GraphAllPairsShortestDistancesDestroy(GraphAllPairsShortestDistances** p) {
  assert(*p != NULL);

  GraphAllPairsShortestDistances* aux = *p;
  unsigned int numVertices = GraphGetNumVertices(aux->graph);

  for (unsigned int i = 0; i < numVertices; i++) {
    free(aux->distance[i]);
  }

  free(aux->distance);

  free(*p);
  *p = NULL;
}

// Getting the result

int GraphGetDistanceVW(const GraphAllPairsShortestDistances* p, unsigned int v,
                       unsigned int w) {
  assert(p != NULL);
  assert(v < GraphGetNumVertices(p->graph));
  assert(w < GraphGetNumVertices(p->graph));

  return p->distance[v][w];
}

// DISPLAYING on the console

void GraphAllPairsShortestDistancesPrint(
    const GraphAllPairsShortestDistances* p) {
  assert(p != NULL);

  unsigned int numVertices = GraphGetNumVertices(p->graph);
  printf("Graph distance matrix - %u vertices\n", numVertices);

  for (unsigned int i = 0; i < numVertices; i++) {
    for (unsigned int j = 0; j < numVertices; j++) {
      int distanceIJ = p->distance[i][j];
      if (distanceIJ == -1) {
        // INFINITY - j was not reached from i
        printf(" INF");
      } else {
        printf(" %3d", distanceIJ);
      }
    }
    printf("\n");
  }
}
