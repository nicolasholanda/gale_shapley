"""
gale_shapley_test.py    07/06/21

Copyright 2021, Nicolas Holanda e Marcelo de Sousa,
Instituto Federal de Educação, Ciência e Tecnologia do Ceará - IFCE
Todos os direitos reservados.

* Parâmetros (respectivamente):
  n: Número de mulheres ou homens. Padrão: 8
  rounds: Número de testes. Padrão: 10

* Descrição:
  Script feito em linguagem Python, usado para testar o algoritmo Gale-Shapley escritoem linguagem C.
  Para cada round, o script gera um arquivo file.dat, possuindo uma matriz de teste que será utilizada
  para simular o problema, de acordo com o parâmetro n recebido. E então compila o arquivo C e executa
  passando o mesmo parâmetro n (recebido via linha de comando por este script).
  A lista resultante é armazenada em uma variável e, usando o mesmos parâmetro, é feita uma nova
  resolução utilizando o pacote matching, que internamente utiliza o algoritmo Gale-Shapley. Por fim,
  o script compara se as duas listas resultantes coincidem. Ao fim da execução de todos os rounds, é
  exibido no console um resumo com os resultados.

* Plataforma alvo:
  Linux

* Requisitos para executar:
  Python3, pip3 e GCC.

* Como executar:
  Abra o terminal no diretório, contendo este script e o arquivo gale_shapley.c e então execute:

  pip3 install --upgrade virtualenv
  virtualenv -p python3 venv
  source venv/bin/activate
  pip3 install matching
  python3 gale_shapley_test.py <número de homens ou mulheres> <número de rounds>

* Autores:
  Nicolas Holanda e Marcelo de Sousa.

* Contexto:
  Trabalho da disciplina de Sistemas Embarcados.

"""


import os
import sys
import random
import subprocess
from matching.games import StableMarriage


# Quantidade de homens ou mulheres. Essa variável deve ser sobrescrita
# pelo primeiro parâmetro da linha de comando, no momento da execução.
n = 8

# Quantidade de vezes que o teste irá se repetir. Essa variável deve ser
# sobrescrita pelo primeiro parâmetro da linha de comando, no momento da
# execução.
rounds = 10

# Nome do arquivo que contém a matriz de preferências.
preferences_file = 'file.dat'


def compile_c_file():
    """
    Método responsável por usar o GCC pra compilar o arquivo C.

    :return:
    """
    os.system("gcc gale_shapley.c -o gale_shapley")


def read_file_matrix():
    """
    Lê o arquivo definido pela variável global preferences_file e devolve a matriz de preferências
    em forma de um dicionário, do tipo:

    {'0': [2, 3], '1': [3, 2], '2': [0, 1], '3': [1, 0]}

    :return: Dicionário onde a chave é o nome da pessoa e o valor é a lista de preferências dela.
    """

    with open(preferences_file, 'r') as f:
        matrix = {str(i): [num.replace('\n', '') for num in line.split(' ')] for i, line in enumerate(f)}
    return matrix


def python_gale_shapley():
    """
    Lê o dicionário de preferências e divide em 2: um com preferências dos homens e um
    com preferências das mulheres, invoca a classe StableMarriage da lib matching passando
    os dicionários como parâmetros, e retorna o dicionário resultante em forma de uma lista
    contendo apenas os valores. Por exemplo:

    Dicionário retornado pela lib matching: {'0': 2, '1': 3, '2': 0, '3': 1}
    Lista contendo apenas os valores: [2, 3, 0, 1]

    Isso porque, a partir do índice de cada elemento da lista de valores, conseguimos saber
    quem é a chave no dicionário original.

    :return: Lista de inteiros, onde o i-ésimo valor representa a pessoa casada com a pessoa i.
    """

    matrix = read_file_matrix()

    # Divide a matriz de preferências em duas: homens e mulheres
    men_preferences = dict(list(matrix.items())[:len(matrix)//2])
    women_preferences = dict(list(matrix.items())[len(matrix)//2:])

    # Passa as matrizes de preferências para a lib matching e transforma o dicionário
    # resultante em uma lista apenas com os valores do dicionário resultante.
    game = StableMarriage.create_from_dictionaries(women_preferences, men_preferences)
    p_list = list(map(lambda p: int(p.name), game.solve(optimal="reviewer").values()))
    
    return p_list


def generate_random_preferences():
    """
    Preenche o arquivo preferences_file com uma matriz de preferências aleatória, com
    base na quantidade de homens/mulheres, definda pela variável n.

    :return:
    """

    # Matriz de preferências aleatória
    matrix = []

    # Para as linhas dos homens, gera uma lista
    # com as mulheres em ordem aleatória.
    for i in range(n):
        l = list(range(n, 2 * n))
        random.shuffle(l)
        matrix.append(l)

    # Para as linhas das mulheres, gera uma lista
    # com os homens em ordem aleatória.
    for i in range(n):
        l = list(range(n))
        random.shuffle(l)
        matrix.append(l)

    # Escreve a matriz no arquivo preferences_file
    with open(preferences_file, 'w') as test_file:
        for row in matrix:
            test_file.write(' '.join([str(a) for a in row]) + '\n')


def c_gale_shapley():
    """
    Compila o código C contendo o algoritmo Gale-Shapley e invoca passando a variável
    global n.

    :return: Lista de inteiros, onde o i-ésimo valor representa a pessoa casada com a pessoa i.
    """

    # Invoca o algoritmo C, passando a variável global n e salva o retorno na variável out.
    proc = subprocess.Popen(["./gale_shapley " + str(n)], stdout=subprocess.PIPE, shell=True)
    (out, err) = proc.communicate()

    # Transforma o retorno do algoritmo em uma lista de inteiros e um tempo de execução.
    result = out.split()
    time = float(result.pop())
    c_list = list(map(int, result))

    return c_list, time


def execute_rounds():
    """
    Executa testes, mostrando os resultados de cada um e um
    resumo no final. O número de vezes que irá rodar está
    definido pela variável rounds.

    :return:
    """

    # Tempos que o algoritmo C levou em cada round
    c_times = []

    # Número de rounds que o resultado de C foi igual
    # ao de Python.
    success_rounds = 0

    for i in range(rounds):
        print(f'\n----------- ROUND {i + 1} -----------')
        generate_random_preferences()

        # Calcula e mostra o resultado e tempo do problema usando C
        c_result = c_gale_shapley()
        c_times.append(c_result[1])
        print("Tempo: " + str(c_result[1]))
        print("C: " + str(c_result[0]))

        # Calcula e mostra o resultado do problema usando Python
        python_result = python_gale_shapley()
        print("Python: " + str(python_result))

        # Compara se resultados são iguais e incrementa success_rounds
        # caso sejam
        if python_result == c_result[0]:
            print("\nListas iguais!")
            success_rounds += 1
        else:
            print("\nListas diferentes!")

    print("\n------------ RESUMO ---------------")
    print(f"Rounds executados: {rounds}")
    print(f"Rounds com sucesso: {success_rounds}")
    print(f"Rounds com falha: {rounds - success_rounds}")
    print(f"Média de tempo (s): {sum(c_times) / len(c_times)}")
    print(f"Quantidade de mulheres/homens: {n}")
    print(f"Tamanho da matriz: {2*(n**2)}")
    print("-----------------------------------\n")


def main():
    """
    Atribui os valores da linha de comando às variáveis globais n e preferences_file.
    E verifica se as listas resultantes da lib matching e do algoritmo C estão iguais.

    :return:
    """

    global n
    global rounds

    if len(sys.argv) >= 2:
        n = int(sys.argv[1])

    if len(sys.argv) == 3:
        rounds = int(sys.argv[2])

    compile_c_file()
    execute_rounds()


if __name__ == '__main__':
    main()
