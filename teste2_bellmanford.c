#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Graph.h"
#include "GraphBellmanFordAlg.h"

void testBellmanFord() {
    // Criar um grafo com 5 vértices
    Graph* g = GraphCreate(5, 1, 0);  // 5 vértices, não orientado, não ponderado

    // Adicionar arestas ao grafo
    GraphAddEdge(g, 0, 1);
    GraphAddEdge(g, 0, 2);
    GraphAddEdge(g, 1, 3);
    GraphAddEdge(g, 2, 4);
    GraphAddEdge(g, 3, 4);

    printf("Grafo criado:\n");
    GraphDisplay(g);

    // Executar o algoritmo Bellman-Ford a partir do vértice 0
    GraphBellmanFordAlg* alg = GraphBellmanFordAlgExecute(g, 0);

    // Número de vértices no grafo
    unsigned int numVertices = GraphGetNumVertices(g);

    printf("\n=== Verificação das variáveis do Algoritmo Bellman-Ford ===\n");

    // Iterar sobre os vértices e exibir informações
    for (unsigned int v = 0; v < numVertices; v++) {
        int distancia = GraphBellmanFordAlgDistance(alg, v);
        int marcado = GraphBellmanFordAlgReached(alg, v);

        // Exibir valores calculados
        printf("Vértice %u:\n", v);
        printf("  Distância calculada: %d\n", distancia);
        printf("  Marcado (alcançado): %d\n", marcado);

        // Verificar se os valores estão corretos
        if (distancia == -1) {
            printf("  -> Esperado: Não alcançado (Distância = -1, Marcado = 0)\n");
            assert(marcado == 0 && "Erro: Vértices não alcançados devem estar marcados como 0.");
        } else {
            printf("  -> Esperado: Alcançado (Distância >= 0, Marcado = 1)\n");
            assert(marcado == 1 && "Erro: Vértices alcançados devem estar marcados como 1.");
        }
    }

    // Mostrar os caminhos mais curtos
    printf("\n=== Caminhos mais curtos calculados ===\n");
    for (unsigned int v = 0; v < numVertices; v++) {
        printf("Caminho até o vértice %u: ", v);
        GraphBellmanFordAlgShowPath(alg, v);
        printf("\n");
    }

    // Exibir a árvore de caminhos mais curtos no formato DOT
    printf("\n=== Árvore de Caminhos Mais Curtos (Formato DOT) ===\n");
    GraphBellmanFordAlgDisplayDOT(alg);

    // Destruir estruturas
    GraphBellmanFordAlgDestroy(&alg);
    GraphDestroy(&g);

    printf("\nTeste concluído com sucesso!\n");
}

int main() {
    printf("=== Início do Teste do Algoritmo Bellman-Ford ===\n");
    testBellmanFord();
    printf("\n=== Todos os testes foram concluídos ===\n");
    return 0;
}
