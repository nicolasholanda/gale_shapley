/*
 * gale_shapley.c    03/06/21
 *
 * Copyright 2021, Nicolas Holanda e Marcelo de Sousa,
 * Instituto Federal de Educação, Ciência e Tecnologia do Ceará - IFCE
 * Todos os direitos reservados.
 *
 * Créditos:
 * https://www.geeksforgeeks.org/
 *
 * Descrição:
 * O presente arquivo contém a implementação do algoritmo Gale-Shapley,
 * escrito em linguagem C. Dados n homens e n mulheres, e as listas de
 * pessoas com as quais cada um dos indivíduos gostaria de se relacionar,
 * o algoritmo tem por objetivo formar n casais, de maneira que não haja
 * 2 pessoas do sexo oposto que preferiam estar juntas, em vez de seus
 * atuais cônjuges. A matriz será lida do arquivo file.dat.
 *
 * Plataforma alvo:
 * Linux
 *
 * Requisitos para executar:
 * GCC
 *
 * Como executar:
 * Abra o terminal no diretório, contendo o arquivo gale_shapley.c e então execute:
 * gcc gale_shapley.c -o gale_shapley
 * ./gale_shapley <número de homens ou mulheres>
 *
 * Autores:
 * Nicolas Holanda e Marcelo de Sousa.
 *
 * Contexto:
 * Trabalho da disciplina de Sistemas Embarcados.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

 
// Número de homens ou mulheres.
int n;


/*
 * Função:  wPrefersM1OverM
 * --------------------
 *  Checa se mulher w prefere o homem m1 em vez do homem m.
 *
 *  w:      mulher em questão.
 *  m:      novo possível parceiro da mulher m.
 *  m1:     atual parceiro da mulher w.
 *  prefer: matriz de preferências, onde as linhas de 0 a n-1 são as listas
 *          de preferências do homem 0 ao homem n-1. E as linhas de n a 2n-1
 *          são as listas de preferências da mulher n à mulher 2n-1. Cada lista
 *          está ordenada de acordo com a preferência, sendo o primeiro item o
 *          mais preferido.
 *
 *  returns: 1 se a mulher w prefere m1 no lugar de m. 0 caso contrário.
 */
int wPrefersM1OverM(int prefer[2*n][n], int w, int m, int m1)
{
    // Para cada homem da matriz
    for (int i = 0; i < n; i++)
    {
        // Se m1 vem antes de m na lista de w, então w prefere o homem
        // m1 (atual parceiro), retorna 1
        if (prefer[w][i] == m1)
            return 1;
 
        // Se m vem antes de m1 na lista de w, então w prefere o homem
        // m, retorna 0
        if (prefer[w][i] == m)
           return 0;
    }
}


/*
 * Função:  galeShapley
 * --------------------
 *  Calcula o casamento estável de N homens e N mulheres, de maneira que não
 *  haja duas pessoas do sexo oposto que prefiram casar um com o outro, em vez
 *  de seus atuais cônjuges.
 *
 *  prefer: matriz de preferências, onde as linhas de 0 a n-1 são as listas
 *          de preferências do homem 0 ao homem n-1. E as linhas de n a 2n-1
 *          são as listas de preferências da mulher n à mulher 2n-1. Cada lista
 *          está ordenada de acordo com a preferência, sendo o primeiro item o
 *          mais preferido.
 *
 *  returns: mostra a lista resultante wPartner no console. O valor de wPartner[i]
 *           indica o parceiro da mulher n+i.
 */
void galeShapley(int prefer[2*n][n])
{

    // Lista resultante. Note que mulheres começam da linha n na matriz prefer, logo
    // o valor de wPartner[i] indica o parceiro da mulher n+i.
    int wPartner[n];
 
    // Armazena os homens solteiros. Se mFree[i] é 0
    // então homem 'i' é solteiro, caso contrário é noivo.
    int mFree[n];
 
    // Inicializa todas as mulheres e homens como solteiros.
    memset(wPartner, -1, sizeof(wPartner));
    memset(mFree, 0, sizeof(mFree));

    // Armazena o número de homens solteiros.
    int freeCount = n;
 
    // Enquanto existir homem solteiro.
    while (freeCount > 0)
    {
        // Pegue o primeiro homem solteiro e atribua a variável m.
        int m;
        for (m = 0; m < n; m++)
            if (mFree[m] == 0)
                break;
 
        // Para cada mulher w da lista de preferências do homem m, enquanto m for solteiro.
        for (int i = 0; i < n && mFree[m] == 0; i++)
        {

            // Atribua a mulher da lista de preferências de m à variável w.
            int w = prefer[m][i];
 
            // Se w está solteira, então w e m viram parceiros e decrementa
            // o contador de homens solteiros.
            if (wPartner[w-n] == -1)
            {
                wPartner[w-n] = m;
                mFree[m] = 1;
                freeCount--;
            }
 
            // Se w não está solteira.
            else 
            {
                // Ache o atual parceiro de w.
                int m1 = wPartner[w-n];
 
                // Se w prefere m no lugar de seu atual parceiro m1, então
                // deixe o homem m1 solteiro e torne m e w parceiros.
                if (wPrefersM1OverM(prefer, w, m, m1) == 0)
                {
                    wPartner[w-n] = m;
                    mFree[m] = 1;
                    mFree[m1] = 0;
                }
            } // Fim do Else
        } // Fim do for que percorre todas as mulheres da lista de m
    } // Fim do while principal
 
 
    // Mostre a solução
    for (int i = 0; i < n; i++)
       printf("%d ", wPartner[i]);
}


/*
 * Função:  readMatrix
 * --------------------
 *  Converte o conteúdo do arquivo (cujo nome é enviado por parâmetro) em uma
 *  matriz de preferências e salva na variável matrix. O arquivo deve conter
 *  2n linhas e n colunas, sendo as linhas de 0 a n-1 as preferências dos homens
 *  e n até 2n-1 as preferências das mulheres.
 *  Cada linha i do arquivo deve conter uma lista de preferências da pessoa i,
 *  separadas por espaço.
 *  Por exemplo, para n=2 (homens 0 e 1 e as mulheres 2 e 3), o arquivo poderia
 *  ter o seguinte formato:
 *
 *  2 3
 *  3 2
 *  0 1
 *  1 0
 *
 *  matrix:   matriz vazia, de tamanho 2n x n. Que será preenchida com o
 *            conteúdo do arquivo.
 *  filename: nome do arquivo onde está definida a matriz de preferências.
 *
 *  returns:  1 caso tudo ocorra normalmente. 0 caso contrário.
 */
int readMatrix(int (*matrix)[n], const char* filename)
{

    // Abre o arquivo para leitura e o atribui à pf
    FILE *pf;
    pf = fopen (filename, "r");

    // Se o arquivo for nulo, então retorna 0.
    if (pf == NULL)
        return 0;

    // Para cada linha do arquivo.
    for(size_t i = 0; i < 2*n; ++i)
    {
        // Para cada item da linha, adiciona na matriz.
        for(size_t j = 0; j < n; ++j)
            fscanf(pf, "%d", matrix[i] + j);
    }

    // Fecha o arquivo e retorna 1 indicando que funcionou.
    fclose (pf); 
    return 1; 
}


/*
 * Função:  main
 * --------------------
 *  Recebe valor via linha de comando e atribui à variável global n, invoca
 *  as funções principais e mede o tempo de execução do algoritmo.
 *
 *
 *  argc:   número de argumentos enviados via linha de comando. No mínimo,
 *          terá valor 1, que é o próprio nome do algoritmo.
 *  argv:   lista de argumentos enviados via linha de comando. O primeiro
 *          deve ser o nome do algoritmo e o segundo deve ser o número de
 *          homens ou mulheres. Por exemplo, para n=4:
 *
 *          ./gale_shapley 4
 *
 *  returns: 0 se executar normalmente. 1 caso contrário.
 */
int main(int argc, char *argv[])
{
    // Se vier apenas o nome do algoritmo como argumento, exibe
    // um erro no consle e sai com status 1.
    if(argc == 1) {
    	printf("Informe o número de homens/mulheres.");
    	return 1;
    }

    // Armazena o número de homens/mulheres. Valor passado via terminal.
    n = atoi(argv[1]);
    
    // Lê o arquivo e forma a matriz de preferências
    int matrix[2*n][n];
    readMatrix(matrix, "file.dat");

    // Armazena o momento do início da execução
    clock_t begin = clock();

    // Executa o algoritmo principal
    galeShapley(matrix);
    
    // Armazena o momento do fim da execução
    clock_t end = clock();
    
    // Calcula o tempo gasto na execução e mostra no console
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("\n%f\n", time_spent);
 
    return 0;
}
