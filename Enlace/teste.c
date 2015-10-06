// Murilo Santana 12662185
// Murilo Bergo 10587525
// Leonardo Bonetti Maffei 11742236

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "library.h"
/*
*	Variaveis globais
*/
pthread_mutex_t mutex[5];

void *sendNetwork(void* threadid) 
{
        char read[100];
        int erro;

        pthread_mutex_unlock(&mutex1);
        while(1)
        {
            pthread_mutex_lock(&mutex0);
            
            __fpurge(stdin);
            printf("Introduza a mensagem: ");
            scanf("%s", read);
            __fpurge(stdin);
            printf(" \n");

            memcpy(linkPhysical, read, 100);
            
            pthread_mutex_unlock(&mutex1);

            usleep(100);
            erro = errorDataLink;

            if(erro != 0)
            {
                printf("\n Erro numero %d: \n", erro);
                if(erro == -10)
                {
                    printf("no nao encontrado.\n");
                } 
                if(erro == -20)
                {
                   printf("\n dados excedem MTU.\n"); 
                } 
                fflush(stdout);
            }
            usleep(100);
        }
}

void* receiveNetworkLink(void* threadid)
{
    char read[100];
    while(1)
    {
        pthread_mutex_lock(&mutex3);
        memcpy(read, linkPhysical, 100);

        printf("\nMensagem recebida: %s\n", read);
        fflush(stdout);

        pthread_mutex_unlock(&mutex4);
    }
}

void initnetwork()
{
    pthread_create(&tdispach, NULL, sendNetwork, (void* )1);
    usleep(20);
    pthread_create(&treap, NULL, receiveNetworkLink, (void* )1);

    pthread_join(tdispach, NULL);
    pthread_join(treap, NULL);
}
