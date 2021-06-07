#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

 
// Número de homens e mulheres
#define N  4
 
// Retorna 1 se mulher 'w' prefere o homem 'm1' em vez do homem 'm'
int wPrefersM1OverM(int prefer[2*N][N], int w, int m, int m1)
{
    // Checa se w prefere m no lugar do atual noivo (m1)
    for (int i = 0; i < N; i++)
    {
        // Se m1 vem antes de m na lista de w, então w prefere o
        // noivo atual, não faz nada
        if (prefer[w][i] == m1)
            return 1;
 
        // Se m vem antes de m1 na lista de w, então termina o atual
        // noivado de w e noiva w com m
        if (prefer[w][i] == m)
           return 0;
    }
}
 
// Calcula o casamento estável de N homens e N mulheres. Homens são numerado de 0 a
// N-1. Mulheres são numeradas de N a 2N-1.
void galeShapley(int prefer[2*N][N])
{

    // O valor de wPartner[i] indica o parceiro da mulher N+i.
    // Note que os números das mulheres varia entre N e 2*N-1.
    // O valor -1 indica que a (N+i)-ésima mulher está solteira.
    int wPartner[N];
 
    // Armazena os homens solteiros. Se mFree[i] é 0
    // então homem 'i' é solteiro, caso contrário é noivo.
    int mFree[N];
 
    // Inicializa todas as mulheres e homens como solteiros
    memset(wPartner, -1, sizeof(wPartner));
    memset(mFree, 0, sizeof(mFree));
    int freeCount = N;
 
    // Enquanto existir homem solteiro
    while (freeCount > 0)
    {
        // Pegue o primeiro homem e atribua a m
        int m;
        for (m = 0; m < N; m++)
            if (mFree[m] == 0)
                break;
 
        // Para cada mulher w da lista de preferências de m, enquanto m for solteiro:
        for (int i = 0; i < N && mFree[m] == 0; i++)
        {
            int w = prefer[m][i];
 
            // Se w está solteira, então w e m viram parceiros
            if (wPartner[w-N] == -1)
            {
                wPartner[w-N] = m;
                mFree[m] = 1;
                freeCount--;
            }
 
            // Se w não está solteira
            else 
            {
                // Ache o atual parceiro de w
                int m1 = wPartner[w-N];
 
                // Se w prefere m no lugar de seu atual parceiro m1,
                // então termine o relacionamento de w e m1 e
                // torne m e w parceiros.
                if (wPrefersM1OverM(prefer, w, m, m1) == 0)
                {
                    wPartner[w-N] = m;
                    mFree[m] = 1;
                    mFree[m1] = 0;
                }
            } // Fim do Else
        } // Fim do for que percorre todas as mulheres da lista de m
    } // Fim do while principal
 
 
    // Mostre a solução
    printf("Mulher  Homem\n");
    for (int i = 0; i < N; i++)
       printf("%d\t%d\n", i+N, wPartner[i]);
}
 
// Função main, que testa as funções acima
int main()
{

    /*
    	Matriz de preferências, onde as linhas 0 até N-1
    	são as listas de preferências do homem 0 até o homem N-1
    	e as linhas de N até 2N-1 são as listas de preferências
    	da mulher N até a mulher 2N-1. Todas as listas estão ordenadas
    	por ordem de preferência, sendo o primeiro item da lista a pessoa
    	mais preferida.
    */
    int prefer[2*N][N] = { {7, 5, 6, 4},
        {5, 4, 6, 7},
        {4, 5, 6, 7},
        {4, 5, 6, 7},
        {0, 1, 2, 3},
        {0, 1, 2, 3},
        {0, 1, 2, 3},
        {0, 1, 2, 3},
    };
    
    // Armazena o momento do início da execução
    clock_t begin = clock();

    // Executa o algoritmo principal
    galeShapley(prefer);
    
    // Armazena o momento do fim da execução
    clock_t end = clock();
    
    // Calcula o tempo gasto na execução e mostra no console
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("\nTempo gasto: %f segundos.\n", time_spent);
    
 
    return 0;
}
