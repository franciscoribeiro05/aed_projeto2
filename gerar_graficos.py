import pandas as pd
import matplotlib.pyplot as plt

# Carregar os resultados do arquivo CSV
df = pd.read_csv('resultados_transitive_closure.csv')

# Salvar os resultados em um arquivo Excel
excel_file = 'teste_1.xlsx'
df.to_excel(excel_file, index=False)
print(f'Resultados salvos no arquivo: {excel_file}')

# Gerar um único gráfico com todas as densidades
plt.figure(figsize=(12, 8))
for density in df['Densidade'].unique():
    subset = df[df['Densidade'] == density]
    plt.plot(subset['Numero de Vertices'], 
             subset['Tempo de Execucao (s)'], 
             marker='o', 
             label=f'Densidade {density}')

plt.title('Tempo de Execução vs. Número de Vértices por Densidade')
plt.xlabel('Número de Vértices')
plt.ylabel('Tempo de Execução (s)')
plt.grid(True)
plt.legend()
graph_file = 'grafico_transitive_closure.png'
plt.savefig(graph_file)
plt.show()
print(f'Gráfico salvo no arquivo: {graph_file}')
