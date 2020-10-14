#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>

int produzir(int value);    //< definida em helper.c
void consumir(int produto); //< definida em helper.c
void *produtor_func(void *arg);
void *consumidor_func(void *arg);

int indice_produtor, indice_consumidor, tamanho_buffer;
int* buffer;
pthread_t thread1, thread2;
sem_t sem1, sem2;

//Você deve fazer as alterações necessárias nesta função e na função
//consumidor_func para que usem semáforos para coordenar a produção
//e consumo de elementos do buffer.
void *produtor_func(void *arg) {
    //arg contem o número de itens a serem produzidos
    int max = *((int*)arg);
    
    for (int i = 0; i <= max; ++i) {
        int produto;
        if (i == max)
            produto = -1;          //envia produto sinlizando FIM
        else 
            produto = produzir(i); //produz um elemento normal
        indice_produtor = (indice_produtor + 1) % tamanho_buffer; //calcula posição próximo elemento
        sem_wait(&sem2);
        buffer[indice_produtor] = produto; //adiciona o elemento produzido à lista
        sem_post(&sem1);
    }
    pthread_exit(NULL);
}

void *consumidor_func(void *arg) {
    while (1) {
        indice_consumidor = (indice_consumidor + 1) % tamanho_buffer; //Calcula o próximo item a consumir
        sem_wait(&sem1); /* Semaforo 1, esta com 0 logo não tem como funcionar */
        int produto = buffer[indice_consumidor]; //obtém o item da lista
        sem_post(&sem2);
        //Podemos receber um produto normal ou um produto especial
        // Aqui ocorre o erro de memoria
        if (produto >= 0) {
            consumir(produto); //Consome o item obtido.
        }
        else {
            break; //produto < 0 é um sinal de que o consumidor deve parar
        }
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Uso: %s tamanho_buffer itens_produzidos\n", argv[0]);
        return 0;
    }

    tamanho_buffer = atoi(argv[1]);
    int n_itens = atoi(argv[2]);
    printf("n_itens: %d\n", n_itens);

    //Iniciando buffer
    indice_produtor = 0;
    indice_consumidor = 0;
    buffer = malloc(sizeof(int) * tamanho_buffer);

    /* Inicializando os semaforos */
    sem_init(&sem1, 0, 0);
    sem_init(&sem2, 0, 1);

    /* Construção de pthreads. */
    pthread_create(&thread2, NULL, produtor_func, (void *)&n_itens);
    pthread_create(&thread1, NULL, consumidor_func, NULL);

    pthread_join(thread2, NULL);
    pthread_join(thread1, NULL);
    /* Destroi os semaforos. */
    sem_destroy(&sem1);
    sem_destroy(&sem2);
    /* Liberando o espaço de memoria. */
    free(buffer);
    return 0;
}

