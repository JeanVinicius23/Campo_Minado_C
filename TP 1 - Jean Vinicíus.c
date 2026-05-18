#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX_SIZE 30

// aloca memória pra matriz
int** CriaMatriz(int tamanho) {
    int** matriz = (int**)malloc(tamanho * sizeof(int*));
   
    for (int i = 0; i < tamanho; i++) {
        matriz[i] = (int*)malloc(tamanho * sizeof(int));
    }
    return matriz;
}
// Libera memória da matriz
void liberaMatriz(int** matriz, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        free(matriz[i]);
    }
    free(matriz);
}

void criaCampo(int** campo, int tamanho, int minas) {
  
    // cria o campo com zeros
    for (int i = 0; i < tamanho; i++) {
        for (int j = 0; j < tamanho; j++) {
            campo[i][j] = 0;
        }
    }
    // Coloca as minas aleatoriamente no campo minado
    for (int i = 0; i < minas; i++) {
        int x, y;
        
        do {
            x = rand() % tamanho;
            y = rand() % tamanho;
        } 
        while (campo[x][y] == -1); // ve se nao tem uma mina sobreposta
            campo[x][y] = -1; // -1 representa uma mina
            
        // Atualiza a contagem de minas ao redor
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                // Ignora a própria mina
                if (i == 0 && j == 0) {
                    continue; 
                }
                
                int novaLinha = x + i; // nova coordenada na horizontal
                int novaColuna = y + j; // nova coordenada na vertical 

                // Verifica se estão dentro dos limites da matriz
                if (novaLinha >= 0 && novaLinha < tamanho && novaColuna >= 0 && novaColuna < tamanho) {
                    if (campo[novaLinha][novaColuna] != -1) {
                        campo[novaLinha][novaColuna]++;
                    }
                }
            }
        }
    }
}
// imprime todo o campo
void imprimeCampo(int** campo, int tamanho) {
   
    for (int i = 0; i < tamanho; i++) {
        for (int j = 0; j < tamanho; j++) {
            
            if (campo[i][j] == -1) {
                printf("-1 "); // Mostra a mina
            } else {
                printf("%d ", campo[i][j]);
            }
        }
        printf("\n");
    }
}

// Função para imprimir o campo atual revelado
void imprime(int** campo, int tamanho, int** revelado) {
    
    for (int i = 0; i < tamanho; i++) {
        for (int j = 0; j < tamanho; j++) {
            
            if (revelado[i][j]) {
                if (campo[i][j] == -1) {
                    printf("-1 "); 
                } else {
                    printf("%d ", campo[i][j]); // Exibe a contagem de minas q tem ao redor
                }
            } else {
                printf("+ ");  // preenche o campo minado com o sinal de +
            }
        }
        printf("\n");
    }
}
// Função que vai verificar se a coordenada existe
int entrada(int x, int y, int tamanho, int** revelado) {
   
    // Verifica se as coordenadas estão dentro dos limites da matriz do campo minado escolhido 
    if (x < 1 || x > tamanho || y < 1 || y > tamanho) {
        printf("Coordenada nao existe\n"); 
        return 0; // Coordenadas fora do limite
    }

    int linha = x - 1;
    int coluna = y - 1;

    // Verifica se a coordenada já foi escolhida
    if (revelado[linha][coluna]) {
        printf("Coordenada repetida\n");
        return 0; 
    }
    return 1; 
}
int main() {

    srand(time(NULL)); // gera números aleatórios

    int tamanho, minas;
    char dificuldade[10];

    printf("Escolha a dificuldade do campo minado: facil, medio ou a dificil (se tiver coragem...): ");
    scanf("%s", dificuldade);

    // Define o tamanho do campo e a quantidade de minas com base na dificuldade q foi escolhida
    if (dificuldade[0] == 'f' && dificuldade[1] == 'a') {
        tamanho = 10;
        minas = 3;
    } else if (dificuldade[0] == 'm' && dificuldade[1] == 'e') {
        tamanho = 20;
        minas = 6;
    } else if (dificuldade[0] == 'd' && dificuldade[1] == 'i') {
        tamanho = 30;
        minas = 9;
    } else {
        printf("Escolha somente entre as 3 dificuldades disponiveis\n");
        return 1;
    }

    int** campo = CriaMatriz(tamanho);
    int** revelado = CriaMatriz(tamanho);

    // Inicializa as células como não reveladas
    for (int i = 0; i < tamanho; i++) {
        for (int j = 0; j < tamanho; j++) {
            revelado[i][j] = 0; // Todas as células começam ocultas
        }
    }

    criaCampo(campo, tamanho, minas);

    // Loop do jogo!!  :) 
    while (1) {
        imprime(campo, tamanho, revelado);
        
        int x, y; // linha e coluna
        printf("Escolha uma coordenada (x,y): ");
        
        if (scanf("%d,%d", &x, &y) != 2) {
            while (getchar() != '\n'); // Limpa o buffer d
            continue;
        }
        // Verifica se a coordenada é válida
        if (!entrada(x, y, tamanho, revelado)) {
            continue; // Se não for válida, joga dnv
        }

        int linha = x - 1;
        int coluna = y - 1;

        revelado[linha][coluna] = 1; // Marca a coordenada como revelada

        if (campo[linha][coluna] == -1) { // se caiu em -1 , perdeu
            printf("Game Over\n");
            imprimeCampo(campo, tamanho); // Revela todo o campo quando perder e para a execução
            break;
        }

        int celulas_descobertas = 0;

        // Conta as células que foram descobertas 
        for (int i = 0; i < tamanho; i++) {
            for (int j = 0; j < tamanho; j++) {
                
                if (revelado[i][j] && campo[i][j] != -1) {
                    celulas_descobertas++;
                }
            }
        }
        //calcula o total de células que podem ser reveladas no campo minado, excluindo as que tem minas
        int total = tamanho * tamanho - minas;

        //ex: se o campo tiver um tamanho de 10 (o que significa que é um campo de 10 x 10, totalizando 100 células)
        //e houver 3 minas, o cálculo vai ser :  10 * 10 - 3 = 97 células que podem ser reveladas sem encontrar uma mina    

        // Verifica se o jogador ganhou
        if (celulas_descobertas == total) { 
            printf("parabéns, vc eh fera\n");
            imprimeCampo(campo, tamanho); // Revela todo o campo se vencer
            break;
        }
    }
    // Libera a memória alocada
    liberaMatriz(campo, tamanho);
    liberaMatriz(revelado, tamanho);

    return 0;
}
