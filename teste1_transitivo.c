#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void simulate_transitive_closure(int num_vertices, double density, FILE* results_file) {
    clock_t start_time = clock();
    int num_edges = (int)(num_vertices * num_vertices * density);
    for (int i = 0; i < num_edges; i++) {
        for (int j = 0; j < 1000; j++); // Simula um custo computacional
    }
    clock_t end_time = clock();
    double elapsed_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    fprintf(results_file, "%d,%.2f,%.4f\n", num_vertices, density, elapsed_time);
}

int main() {
    int vertex_counts[] = {10, 50, 100, 500, 1000, 2000, 5000};
    int num_tests = sizeof(vertex_counts) / sizeof(vertex_counts[0]);
    double densities[] = {0.1, 0.3, 0.5, 0.7};
    int num_densities = sizeof(densities) / sizeof(densities[0]);

    FILE* results_file = fopen("resultados_transitive_closure.csv", "w");
    if (!results_file) {
        perror("Erro ao abrir o arquivo de resultados");
        return 1;
    }
    fprintf(results_file, "Numero de Vertices,Densidade,Tempo de Execucao (s)\n");

    for (int d = 0; d < num_densities; d++) {
        for (int i = 0; i < num_tests; i++) {
            simulate_transitive_closure(vertex_counts[i], densities[d], results_file);
            printf("Teste com %d vertices e densidade %.2f concluido.\n", vertex_counts[i], densities[d]);
        }
    }
    fclose(results_file);

    // Gera o script Python modificado para criar um único gráfico
    FILE* python_script = fopen("gerar_graficos.py", "w");
    if (!python_script) {
        perror("Erro ao criar o script Python");
        return 1;
    }

    fprintf(python_script, "import pandas as pd\n");
    fprintf(python_script, "import matplotlib.pyplot as plt\n\n");
    fprintf(python_script, "# Carregar os resultados do arquivo CSV\n");
    fprintf(python_script, "df = pd.read_csv('resultados_transitive_closure.csv')\n\n");
    fprintf(python_script, "# Salvar os resultados em um arquivo Excel\n");
    fprintf(python_script, "excel_file = 'teste_1.xlsx'\n");
    fprintf(python_script, "df.to_excel(excel_file, index=False)\n");
    fprintf(python_script, "print(f'Resultados salvos no arquivo: {excel_file}')\n\n");
    fprintf(python_script, "# Gerar um único gráfico com todas as densidades\n");
    fprintf(python_script, "plt.figure(figsize=(12, 8))\n");
    fprintf(python_script, "for density in df['Densidade'].unique():\n");
    fprintf(python_script, "    subset = df[df['Densidade'] == density]\n");
    fprintf(python_script, "    plt.plot(subset['Numero de Vertices'], \n");
    fprintf(python_script, "             subset['Tempo de Execucao (s)'], \n");
    fprintf(python_script, "             marker='o', \n");
    fprintf(python_script, "             label=f'Densidade {density}')\n\n");
    fprintf(python_script, "plt.title('Tempo de Execução vs. Número de Vértices por Densidade')\n");
    fprintf(python_script, "plt.xlabel('Número de Vértices')\n");
    fprintf(python_script, "plt.ylabel('Tempo de Execução (s)')\n");
    fprintf(python_script, "plt.grid(True)\n");
    fprintf(python_script, "plt.legend()\n");
    fprintf(python_script, "graph_file = 'grafico_transitive_closure.png'\n");
    fprintf(python_script, "plt.savefig(graph_file)\n");
    fprintf(python_script, "plt.show()\n");
    fprintf(python_script, "print(f'Gráfico salvo no arquivo: {graph_file}')\n");

    fclose(python_script);
    printf("Testes concluídos e script Python gerado.\n");
    return 0;
}