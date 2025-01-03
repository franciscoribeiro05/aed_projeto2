//
// Algoritmos e Estruturas de Dados --- 2024/2025
//
// Joaquim Madeira - Dec 2024
//
// GraphEccentricityMeasures
//

// Student Name : Francisco Ribeiro
// Student Number : 118993
// Student Name : Catarina Rabaça
// Student Number : 119582

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
// Função para calcular as medidas de excentricidade do grafo
GraphEccentricityMeasures* GraphEccentricityMeasuresCompute(Graph* g) {
  assert(g != NULL); // Verifica se o grafo não é NULL

  unsigned int numVertices = GraphGetNumVertices(g); // Obtém o número de vértices no grafo

  // Aloca memória para armazenar as medidas de excentricidade
  GraphEccentricityMeasures* measures = (GraphEccentricityMeasures*)malloc(sizeof(GraphEccentricityMeasures));
  assert(measures != NULL); // Verifica se a alocação foi bem-sucedida

  measures->graph = g; // Associa o grafo à estrutura
  measures->eccentricity = (int*)malloc(numVertices * sizeof(int)); // Aloca memória para o vetor de excentricidade
  assert(measures->eccentricity != NULL); // Verifica a alocação bem-sucedida

  // Inicializa o raio e o diâmetro com valores extremos
  measures->graphRadius = INT_MAX;
  measures->graphDiameter = INT_MIN;

  // Calcula as distâncias mais curtas entre todos os pares de vértices
  GraphAllPairsShortestDistances* apsd = GraphAllPairsShortestDistancesExecute(g);

  // Calcula a excentricidade de cada vértice
  for (unsigned int i = 0; i < numVertices; i++) {
    int maxDistance = INT_MIN;

    for (unsigned int j = 0; j < numVertices; j++) {
      int distance = GraphGetDistanceVW(apsd, i, j); // Obtém a distância entre os vértices i e j
      if (distance != -1) { // Ignora distâncias indefinidas
        if (distance > maxDistance) {
          maxDistance = distance; // Atualiza a maior distância encontrada
        }
      }
    }

    measures->eccentricity[i] = maxDistance; // Armazena a excentricidade do vértice i

    // Atualiza o raio do grafo (menor excentricidade)
    if (maxDistance < measures->graphRadius) {
      measures->graphRadius = maxDistance;
    }

    // Atualiza o diâmetro do grafo (maior excentricidade)
    if (maxDistance > measures->graphDiameter) {
      measures->graphDiameter = maxDistance;
    }
  }

  // Identifica os vértices centrais (aqueles com excentricidade igual ao raio)
  unsigned int centralCount = 0;
  for (unsigned int i = 0; i < numVertices; i++) {
    if (measures->eccentricity[i] == measures->graphRadius) {
      centralCount++; // Conta os vértices centrais
    }
  }

  // Aloca memória para armazenar os vértices centrais
  measures->centralVertices = (unsigned int*)malloc((centralCount + 1) * sizeof(unsigned int));
  assert(measures->centralVertices != NULL); // Verifica a alocação bem-sucedida

  measures->centralVertices[0] = centralCount; // Armazena o número de vértices centrais
  unsigned int index = 1;
  for (unsigned int i = 0; i < numVertices; i++) {
    if (measures->eccentricity[i] == measures->graphRadius) {
      measures->centralVertices[index++] = i; // Adiciona o vértice central ao vetor
    }
  }

  // Libera a memória usada para calcular as distâncias
  GraphAllPairsShortestDistancesDestroy(&apsd);

  return measures; // Retorna a estrutura preenchida com as medidas
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
  assert(p != NULL); // Verifica se a estrutura não é NULL
  assert(p->centralVertices != NULL); // Verifica se o vetor de vértices centrais não é NULL

  // Obtém o número de vértices centrais
  unsigned int count = p->centralVertices[0];

  // Aloca memória para armazenar a cópia do vetor
  unsigned int* centralVerticesCopy = (unsigned int*)malloc((count + 1) * sizeof(unsigned int));
  assert(centralVerticesCopy != NULL); // Verifica a alocação bem-sucedida

  // Copia os valores do vetor original
  for (unsigned int i = 0; i <= count; i++) {
    centralVerticesCopy[i] = p->centralVertices[i];
  }

  return centralVerticesCopy; // Retorna a cópia do vetor de vértices centrais
}



// Print the graph radius and diameter
// Print the vertex eccentricity values
// Print the set of central vertices
void GraphEccentricityMeasuresPrint(const GraphEccentricityMeasures* p) {
  assert(p != NULL); // Verifica se a estrutura não é NULL

  // Imprime o raio e o diâmetro do grafo
  printf("Raio do Grafo: %d\n", p->graphRadius);
  printf("Diâmetro do Grafo: %d\n", p->graphDiameter);

  // Imprime as excentricidades de cada vértice
  printf("Excentricidades dos Vértices:\n");
  unsigned int numVertices = GraphGetNumVertices(p->graph);
  for (unsigned int i = 0; i < numVertices; i++) {
    printf("Vértice %u: Excentricidade = %d\n", i, p->eccentricity[i]);
  }

  // Imprime o conjunto de vértices centrais
  printf("Vértices Centrais (Raio = %d):\n", p->graphRadius);
  unsigned int count = p->centralVertices[0];
  for (unsigned int i = 1; i <= count; i++) {
    printf("Vértice %u\n", p->centralVertices[i]);
  }
}


