import random

# Definindo o tamanho da matriz
tamanho = 20

# Criando a matriz com valores aleatórios entre 0 e 9
matriz = [[random.randint(1, tamanho) for j in range(tamanho)] for i in range(tamanho)]

# Nome do arquivo a ser criado
nome_arquivo = "output.txt"

# Escrevendo a matriz no arquivo
with open(nome_arquivo, "w") as f:
    for linha in matriz:
        # Convertendo os elementos da linha em uma string separada por espaços
        linha_str = " ".join(str(elem) for elem in linha)
        # Escrevendo a linha no arquivo
        f.write(linha_str + "\n")