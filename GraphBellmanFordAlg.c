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

GraphBellmanFordAlg* GraphBellmanFordAlgExecute(Graph* g, unsigned int startVertex) {
  assert(g != NULL);
  assert(startVertex < GraphGetNumVertices(g));
  assert(GraphIsWeighted(g) == 0);  // O grafo deve ser não ponderado

  // Inicializar estrutura principal
  GraphBellmanFordAlg* result =
      (GraphBellmanFordAlg*)malloc(sizeof(struct _GraphBellmanFordAlg));
  assert(result != NULL);

  unsigned int numVertices = GraphGetNumVertices(g);
  result->graph = g;
  result->startVertex = startVertex;

  // Alocar e inicializar os arrays principais
  result->marked = (unsigned int*)calloc(numVertices, sizeof(unsigned int));
  result->distance = (int*)malloc(numVertices * sizeof(int));
  result->predecessor = (int*)malloc(numVertices * sizeof(int));

  for (unsigned int i = 0; i < numVertices; i++) {
    result->distance[i] = -1;  // Nenhuma distância conhecida
    result->predecessor[i] = -1;  // Sem predecessores
  }

  result->distance[startVertex] = 0;  // Vértice inicial tem distância 0

  // Pré-processar arestas do grafo
  unsigned int totalEdges = 0;
  unsigned int** edgeList = (unsigned int**)malloc(numVertices * sizeof(unsigned int*));
  unsigned int* edgeCount = (unsigned int*)calloc(numVertices, sizeof(unsigned int));

  for (unsigned int u = 0; u < numVertices; u++) {
    unsigned int* adjacencies = GraphGetAdjacentsTo(g, u);
    edgeList[u] = adjacencies;
    edgeCount[u] = adjacencies[0];
    totalEdges += adjacencies[0];
  }

  // Bellman-Ford
  for (unsigned int k = 0; k < numVertices - 1; k++) {  // |V| - 1 iterações
    for (unsigned int u = 0; u < numVertices; u++) {
      if (result->distance[u] != -1) {  // Apenas vértices alcançados
        for (unsigned int i = 1; i <= edgeCount[u]; i++) {
          unsigned int v = edgeList[u][i];
          int newDistance = result->distance[u] + 1;  // Peso = 1
          if (result->distance[v] == -1 || newDistance < result->distance[v]) {
            result->distance[v] = newDistance;
            result->predecessor[v] = u;
          }
        }
      }
    }
  }

  // Liberar memória temporária
  for (unsigned int u = 0; u < numVertices; u++) {
    free(edgeList[u]);
  }
  free(edgeList);
  free(edgeCount);

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
