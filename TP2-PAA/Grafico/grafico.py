import matplotlib.pyplot as plt

# Ler os dados do arquivo
tamanhos = [] 
tempos = []
# Abri o arquivo para leitura e salva os dado
with open("Grafico/dados_grafico.txt", "r") as file:
    try:
        for line in file:
            tamanho, tempo = line.split()
            tamanhos.append(int(tamanho))
            tempos.append(float(tempo))
    except ValueError:
        print("Erro ao ler os dados do arquivo. Verifique se o arquivo está no formato correto.")

# Plotar o grafico
plt.plot(tamanhos, tempos, marker='o')
plt.xlabel('Tamanho do Labirinto')
plt.ylabel('Tempo de Execução (segundos)')
plt.title('Gráfico de Complexidade x Tempo')
plt.grid(True)
plt.show()
