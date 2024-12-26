import pandas as pd
import matplotlib.pyplot as plt

# Ler dados
df = pd.read_csv('bellman_ford_results.csv')

# Criar gráfico
plt.figure(figsize=(10, 6))
for density in df['density'].unique():
    data = df[df['density'] == density]
    plt.plot(data['vertices'], data['execution_time'], 
             label=f'Densidade {density}')

plt.xlabel('Número de Vértices')
plt.ylabel('Tempo de Execução (s)')
plt.title('Análise de Desempenho - Bellman-Ford')
plt.legend()
plt.grid(True)
plt.savefig('bellman_ford_analysis.png')
