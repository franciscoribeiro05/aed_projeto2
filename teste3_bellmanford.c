#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Graph.h"
#include "GraphBellmanFordAlg.h"
#include "instrumentation.h"

// Função para gerar um grafo aleatório com densidade específica
Graph* generate_random_graph(int num_vertices, double density) {
    Graph* g = GraphCreate(num_vertices, 1, 0);
    
    for (int i = 0; i < num_vertices; i++) {
        for (int j = i + 1; j < num_vertices; j++) {
            if ((double)rand() / RAND_MAX < density) {
                GraphAddEdge(g, i, j);
            }
        }
    }
    return g;
}

// Estrutura para armazenar resultados
typedef struct {
    int vertices;
    double density;
    double execution_time;
    long long comparisons;
} TestResult;

// Função para executar um teste único
TestResult run_single_test(int num_vertices, double density) {
    TestResult result;
    result.vertices = num_vertices;
    result.density = density;
    
    // Gerar grafo aleatório
    Graph* g = generate_random_graph(num_vertices, density);
    
    // Medir tempo de início
    clock_t start = clock();
    
    // Executar Bellman-Ford
    GraphBellmanFordAlg* alg = GraphBellmanFordAlgExecute(g, 0);
    
    // Medir tempo final
    clock_t end = clock();
    
    // Calcular tempo de execução em segundos
    result.execution_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    // Limpar memória
    GraphBellmanFordAlgDestroy(&alg);
    GraphDestroy(&g);
    
    return result;
}

void run_performance_tests() {
    // Abrir arquivo para resultados
    FILE* fp = fopen("bellman_ford_results.csv", "w");
    fprintf(fp, "vertices,density,execution_time\n");
    
    // Arrays com os tamanhos e densidades a testar
    int sizes[] = {10, 50, 100, 200, 500, 1000};
    double densities[] = {0.1, 0.3, 0.5, 0.7};
    
    // Número de repetições para cada combinação
    const int NUM_REPEATS = 5;
    
    // Para cada tamanho
    for (int i = 0; i < sizeof(sizes)/sizeof(sizes[0]); i++) {
        // Para cada densidade
        for (int j = 0; j < sizeof(densities)/sizeof(densities[0]); j++) {
            double avg_time = 0.0;
            
            // Repetir teste várias vezes para ter média
            for (int k = 0; k < NUM_REPEATS; k++) {
                TestResult result = run_single_test(sizes[i], densities[j]);
                avg_time += result.execution_time;
            }
            
            // Calcular média
            avg_time /= NUM_REPEATS;
            
            // Salvar resultados
            fprintf(fp, "%d,%.2f,%f\n", sizes[i], densities[j], avg_time);
            
            // Mostrar progresso
            printf("Completed test: %d vertices, %.2f density\n", 
                   sizes[i], densities[j]);
        }
    }
    
    fclose(fp);
    
    // Gerar script Python para plotar gráfico
    FILE* py = fopen("plot_results.py", "w");
    fprintf(py, "import pandas as pd\n");
    fprintf(py, "import matplotlib.pyplot as plt\n\n");
    fprintf(py, "# Ler dados\n");
    fprintf(py, "df = pd.read_csv('bellman_ford_results.csv')\n\n");
    fprintf(py, "# Criar gráfico\n");
    fprintf(py, "plt.figure(figsize=(10, 6))\n");
    fprintf(py, "for density in df['density'].unique():\n");
    fprintf(py, "    data = df[df['density'] == density]\n");
    fprintf(py, "    plt.plot(data['vertices'], data['execution_time'], \n");
    fprintf(py, "             label=f'Densidade {density}')\n\n");
    fprintf(py, "plt.xlabel('Número de Vértices')\n");
    fprintf(py, "plt.ylabel('Tempo de Execução (s)')\n");
    fprintf(py, "plt.title('Análise de Desempenho - Bellman-Ford')\n");
    fprintf(py, "plt.legend()\n");
    fprintf(py, "plt.grid(True)\n");
    fprintf(py, "plt.savefig('bellman_ford_analysis.png')\n");
    fclose(py);
}

int main() {
    srand(time(NULL));  // Inicializar gerador de números aleatórios
    
    printf("Iniciando testes de performance do Bellman-Ford...\n");
    run_performance_tests();
    
    printf("\nTestes concluídos! Resultados salvos em 'bellman_ford_results.csv'\n");
    printf("Execute o script Python 'plot_results.py' para gerar o gráfico.\n");
    
    return 0;
}