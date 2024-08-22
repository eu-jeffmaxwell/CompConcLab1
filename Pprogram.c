#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 4
#define N 100
#define M 3

// Estrutura para passar argumentos para as threads
typedef struct {
    int *array;
    int start;
    int end;
} ThreadArgs;

// Função executada por cada thread
void* soma_um(void* args) {
    ThreadArgs *tArgs = (ThreadArgs*)args;
    for (int i = tArgs->start; i < tArgs->end; i++) {
        tArgs->array[i] += 1;
    }
    pthread_exit(NULL);
}

// Função para inicializar o vetor
void vector(int* vetor, int n) {
    for (int i = 0; i < n; i++) {
        vetor[i] = i;  // inicializa vetor com valores [0, 1, 2, ..., n-1]
    }
}

// Função para verificar se o resultado está correto
int verificar_resultado(int* vetor, int n) {
    for (int i = 0; i < n; i++) {
        if (vetor[i] != i + 1) {
            return 0;  // Retorna 0 se algum valor estiver incorreto
        }
    }
    return 1;  // Retorna 1 se todos os valores estiverem corretos
}

int main(int argc, char* argv[]) {

    int* vetor = (int*)malloc(N * sizeof(int));

    // Inicializar o vetor
    vector(vetor , N);

    // Criar as threads
    pthread_t threads[M];
    ThreadArgs args[M];
    int elementos_por_thread = N / M;
    int resto = N % M;

    for (int i = 0; i < M; i++) {
        int start = i * elementos_por_thread + (i < resto ? i : resto);
        int end = start + elementos_por_thread + (i < resto ? 1 : 0);
        args[i].array = vetor;
        args[i].start = start;
        args[i].end = end;

        pthread_create(&threads[i], NULL, soma_um, (void*)&args[i]);
    }

    // Aguardar as threads terminarem
    for (int i = 0; i < M; i++) {
        pthread_join(threads[i], NULL);
    }

    // Verificar o resultado
    if (verificar_resultado(vetor, N)) {
        printf("Todos os valores estão corretos.\n");
    } else {
        printf("Erro nos valores do vetor.\n");
    }

    // Liberar memória alocada
    free(vetor);

    return 0;
}
