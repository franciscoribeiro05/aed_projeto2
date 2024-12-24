//
// Algoritmos e Estruturas de Dados --- 2024/2025
//
// Joaquim Madeira - Dec 2024
//
// GraphEccentricityMeasures
//

// Student Name :
// Student Number :
// Student Name :
// Student Number :

/*** COMPLETE THE GraphEccentricityMeasuresCompute FUNCTION ***/
/*** COMPLETE THE GraphGetCentralVertices FUNCTION ***/
/*** COMPLETE THE GraphEccentricityMeasuresPrint FUNCTION ***/

#include <limits.h>

#include "GraphEccentricityMeasures.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "GraphAllPairsShortestDistances.h"

struct _GraphEccentricityMeasures {
  unsigned int*
      centralVertices;  // centralVertices[0] = number of central vertices
                        // array size is (number of central vertices + 1)
  int* eccentricity;    // the eccentricity value of each vertex
  Graph* graph;         // the graph
  int graphRadius;      // the graph radius
  int graphDiameter;    // the graph diameter
};

// Allocate memory
// Compute the vertex eccentricity values
// Compute graph radius and graph diameter
// Compute the set of central vertices
GraphEccentricityMeasures* GraphEccentricityMeasuresCompute(Graph* g) {
  assert(g != NULL);

  unsigned int numVertices = GraphGetNumVertices(g);

  // Inicializar a estrutura para armazenar as medidas de excentricidade
  GraphEccentricityMeasures* measures = (GraphEccentricityMeasures*)malloc(sizeof(GraphEccentricityMeasures));
  assert(measures != NULL);

  measures->graph = g;
  measures->eccentricity = (int*)malloc(numVertices * sizeof(int));
  assert(measures->eccentricity != NULL);

  measures->graphRadius = INT_MAX;  // Valor inicial do raio
  measures->graphDiameter = INT_MIN;  // Valor inicial do diâmetro

  // Obter todas as distâncias mais curtas
  GraphAllPairsShortestDistances* apsd = GraphAllPairsShortestDistancesExecute(g);

  for (unsigned int i = 0; i < numVertices; i++) {
    int maxDistance = INT_MIN;

    for (unsigned int j = 0; j < numVertices; j++) {
      int distance = GraphGetDistanceVW(apsd,i,j);
      if (distance != -1) {  // Ignorar distâncias indefinidas
        if (distance > maxDistance) {
          maxDistance = distance;
        }
      }
    }

    measures->eccentricity[i] = maxDistance;

    if (maxDistance < measures->graphRadius) {
      measures->graphRadius = maxDistance;
    }
    if (maxDistance > measures->graphDiameter) {
      measures->graphDiameter = maxDistance;
    }
  }

  // Identificar os vértices centrais
  unsigned int centralCount = 0;
  for (unsigned int i = 0; i < numVertices; i++) {
    if (measures->eccentricity[i] == measures->graphRadius) {
      centralCount++;
    }
  }

  measures->centralVertices = (unsigned int*)malloc((centralCount + 1) * sizeof(unsigned int));
  assert(measures->centralVertices != NULL);

  measures->centralVertices[0] = centralCount;
  unsigned int index = 1;
  for (unsigned int i = 0; i < numVertices; i++) {
    if (measures->eccentricity[i] == measures->graphRadius) {
      measures->centralVertices[index++] = i;
    }
  }

  // Libertar a memória da estrutura de distâncias
  GraphAllPairsShortestDistancesDestroy(&apsd);

  return measures;
}

void GraphEccentricityMeasuresDestroy(GraphEccentricityMeasures** p) {
  assert(*p != NULL);

  GraphEccentricityMeasures* aux = *p;

  free(aux->centralVertices);
  free(aux->eccentricity);

  free(*p);
  *p = NULL;
}

// Getting the computed measures

int GraphGetRadius(const GraphEccentricityMeasures* p) {
  assert(p != NULL);

  return p->graphRadius;
}

int GraphGetDiameter(const GraphEccentricityMeasures* p) {
  assert(p != NULL);

  return p->graphDiameter;
}

int GraphGetVertexEccentricity(const GraphEccentricityMeasures* p,
                               unsigned int v) {
  assert(p != NULL);
  assert(v < GraphGetNumVertices(p->graph));
  assert(p->eccentricity != NULL);

  return p->eccentricity[v];
}

// Getting a copy of the set of central vertices
// centralVertices[0] = number of central vertices in the set
unsigned int* GraphGetCentralVertices(const GraphEccentricityMeasures* p) {
  assert(p != NULL);
  assert(p->centralVertices != NULL);

  // Obter o número de vértices centrais
  unsigned int count = p->centralVertices[0];

  // Alocar memória para a cópia (número de vértices centrais + 1)
  unsigned int* centralVerticesCopy = (unsigned int*)malloc((count + 1) * sizeof(unsigned int));
  assert(centralVerticesCopy != NULL);

  // Copiar os valores do vetor original
  for (unsigned int i = 0; i <= count; i++) {
    centralVerticesCopy[i] = p->centralVertices[i];
  }

  return centralVerticesCopy;  // Retorna o vetor copiado
}


// Print the graph radius and diameter
// Print the vertex eccentricity values
// Print the set of central vertices
void GraphEccentricityMeasuresPrint(const GraphEccentricityMeasures* p) {
  assert(p != NULL);

  // Imprimir o raio e o diâmetro do grafo
  printf("Graph Radius: %d\n", p->graphRadius);
  printf("Graph Diameter: %d\n", p->graphDiameter);

  // Imprimir os valores de excentricidade de cada vértice
  printf("Vertex Eccentricities:\n");
  unsigned int numVertices = GraphGetNumVertices(p->graph);
  for (unsigned int i = 0; i < numVertices; i++) {
    printf("Vertex %u: Eccentricity = %d\n", i, p->eccentricity[i]);
  }

  // Imprimir o conjunto de vértices centrais
  printf("Central Vertices (Radius = %d):\n", p->graphRadius);
  unsigned int count = p->centralVertices[0];
  for (unsigned int i = 1; i <= count; i++) {
    printf("Vertex %u\n", p->centralVertices[i]);
  }
}

