//
// Algoritmos e Estruturas de Dados --- 2024/2025
//
// Joaquim Madeira - Dec 2024
//
// GraphTransitiveClosure - Transitive Closure of a directed graph
//

// Student Name : Francisco Ribeiro
// Student Number : 118993
// Student Name : Catarina Rabaça
// Student Number : 119582

/*** COMPLETE THE GraphComputeTransitiveClosure FUNCTION ***/

#include "GraphTransitiveClosure.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"
#include "GraphBellmanFordAlg.h"
#include "instrumentation.h"

// Compute the transitive closure of a directed graph
// Return the computed transitive closure as a directed graph
// Use the Bellman-Ford algorithm
Graph* GraphComputeTransitiveClosure(Graph* g) {
  assert(g != NULL);
  assert(GraphIsDigraph(g));
  assert(GraphIsWeighted(g) == 0);

  // COMPLETE THE CODE
  
  // Número de vértices no grafo original
  unsigned int numVertices = GraphGetNumVertices(g);

  Graph* transitiveClosure = GraphCreate(numVertices,1,0); // Grafo direcionado

   // Iterar por cada vértice como ponto de partida
    for (unsigned int u = 0; u < numVertices; u++) {
        // Executar o algoritmo de Bellman-Ford a partir do vértice u
        GraphBellmanFordAlg* result = GraphBellmanFordAlgExecute(g, u);
        assert(result != NULL);

        // Iterar pelos vértices para verificar conectividade
        for (unsigned int v = 0; v < numVertices; v++) {
            // Usar GraphBellmanFordAlgReached para verificar se v é alcançável a partir de u
            if (GraphBellmanFordAlgReached(result, v) == 1) {
                // Adicionar a aresta (u, v) ao grafo do fecho transitivo
                GraphAddEdge(transitiveClosure, u, v);
            }
        }

        // Destruir o resultado do Bellman-Ford para liberar memória
        GraphBellmanFordAlgDestroy(&result);
    }

  return transitiveClosure;
}

