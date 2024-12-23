//
// Algoritmos e Estruturas de Dados --- 2024/2025
//
// Joaquim Madeira - Dec 2024
//
// GraphBellmanFord - Bellman-Ford Algorithm
//

// Student Name : Catarina Rabaça
// Student Number : 119582
// Student Name : Francisco Ribeiro
// Student Number : 118993

/*** COMPLETE THE GraphBellmanFordAlgExecute FUNCTION ***/

#include "GraphBellmanFordAlg.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "IntegersStack.h"
#include "instrumentation.h"

struct _GraphBellmanFordAlg {
  unsigned int* marked;  // To mark vertices when reached for the first time
  int* distance;  // The number of edges on the path from the start vertex
                  // distance[i]=-1, if no path found from the start vertex to i
  int* predecessor;  // The predecessor vertex in the shortest path
                     // predecessor[i]=-1, if no predecessor exists
  Graph* graph;
  unsigned int startVertex;  // The root of the shortest-paths tree
};



// Função auxiliar para verificar se existe uma aresta entre dois vértices
static int hasEdge(const Graph* g, unsigned int u, unsigned int v) {
  unsigned int* adjacencies = GraphGetAdjacentsTo(g, u);
  unsigned int numAdjacencies = adjacencies[0];  // Primeiro elemento é o número de adjacências

  for (unsigned int i = 1; i <= numAdjacencies; i++) {  // Percorrer os adjacentes
    if (adjacencies[i] == v) {
      free(adjacencies);  // Liberta a memória alocada pela função
      return 1;  // Aresta encontrada
    }
  }

  free(adjacencies);  // Liberta a memória alocada pela função
  return 0;  // Não existe aresta
}

GraphBellmanFordAlg* GraphBellmanFordAlgExecute(Graph* g,
                                                unsigned int startVertex) {
  assert(g != NULL);
  assert(startVertex < GraphGetNumVertices(g));
  assert(GraphIsWeighted(g) == 0);  // Garantir que o grafo não tem pesos

  // Alocar e inicializar estrutura principal
  GraphBellmanFordAlg* result =
      (GraphBellmanFordAlg*)malloc(sizeof(struct _GraphBellmanFordAlg));
  assert(result != NULL);

  unsigned int numVertices = GraphGetNumVertices(g);
  result->graph = g;
  result->startVertex = startVertex;

  // Inicializar arrays principais
  result->marked = (unsigned int*)calloc(numVertices, sizeof(unsigned int));
  result->distance = (int*)malloc(numVertices * sizeof(int));
  result->predecessor = (int*)malloc(numVertices * sizeof(int));

  for (unsigned int i = 0; i < numVertices; i++) {
    result->distance[i] = -1;  // Inicialmente, nenhuma distância é conhecida
    result->predecessor[i] = -1;  // Sem predecessores conhecidos
  }

  result->distance[startVertex] = 0;  // Distância inicial
  result->marked[startVertex] = 1;   // Vértice inicial alcançado

  // Algoritmo de Bellman-Ford
  for (unsigned int k = 0; k < numVertices - 1; k++) {  // |V| - 1 iterações
    for (unsigned int u = 0; u < numVertices; u++) {
      if (result->marked[u]) {  // Apenas vértices marcados
        for (unsigned int v = 0; v < numVertices; v++) {
          if (hasEdge(g, u, v)) {  // Verificar se há aresta entre u e v
            int newDistance = result->distance[u] + 1;  // Peso é 1
            if (result->distance[v] == -1 || newDistance < result->distance[v]) {
              result->distance[v] = newDistance;
              result->predecessor[v] = u;
              result->marked[v] = 1;  // Marca v como alcançado
            }
          }
        }
      }
    }
  }

  return result;
}



void GraphBellmanFordAlgDestroy(GraphBellmanFordAlg** p) {
  assert(*p != NULL);

  GraphBellmanFordAlg* aux = *p;

  free(aux->marked);
  free(aux->predecessor);
  free(aux->distance);

  free(*p);
  *p = NULL;
}

// Getting the paths information

int GraphBellmanFordAlgReached(const GraphBellmanFordAlg* p, unsigned int v) {
  assert(p != NULL);
  assert(v < GraphGetNumVertices(p->graph));

  return p->marked[v];
}

int GraphBellmanFordAlgDistance(const GraphBellmanFordAlg* p, unsigned int v) {
  assert(p != NULL);
  assert(v < GraphGetNumVertices(p->graph));

  return p->distance[v];
}
Stack* GraphBellmanFordAlgPathTo(const GraphBellmanFordAlg* p, unsigned int v) {
  assert(p != NULL);
  assert(v < GraphGetNumVertices(p->graph));

  Stack* s = StackCreate(GraphGetNumVertices(p->graph));

  if (p->marked[v] == 0) {
    return s;
  }

  // Store the path
  for (unsigned int current = v; current != p->startVertex;
       current = p->predecessor[current]) {
    StackPush(s, current);
  }

  StackPush(s, p->startVertex);

  return s;
}

// DISPLAYING on the console

void GraphBellmanFordAlgShowPath(const GraphBellmanFordAlg* p, unsigned int v) {
  assert(p != NULL);
  assert(v < GraphGetNumVertices(p->graph));

  Stack* s = GraphBellmanFordAlgPathTo(p, v);

  while (StackIsEmpty(s) == 0) {
    printf("%d ", StackPop(s));
  }

  StackDestroy(&s);
}

// Display the Shortest-Paths Tree in DOT format
void GraphBellmanFordAlgDisplayDOT(const GraphBellmanFordAlg* p) {
  assert(p != NULL);

  Graph* original_graph = p->graph;
  unsigned int num_vertices = GraphGetNumVertices(original_graph);

  // The paths tree is a digraph, with no edge weights
  Graph* paths_tree = GraphCreate(num_vertices, 1, 0);

  // Use the predecessors array to add the tree edges
  for (unsigned int w = 0; w < num_vertices; w++) {
    // Vertex w has a predecessor vertex v?
    int v = p->predecessor[w];
    if (v != -1) {
      GraphAddEdge(paths_tree, (unsigned int)v, w);
    }
  }

  // Display the tree in the DOT format
  GraphDisplayDOT(paths_tree);

  // Housekeeping
  GraphDestroy(&paths_tree);
}
