// Murilo Santana 12662185
// Murilo Bergo 10587525
// Leonardo Bonetti Maffei  


#include <stdlib.h>
#include <stdio.h>
#include "library.h"



pthread_t layerLink;
pthread_t layerNetwork;
// pthread_mutex_t mutex[5];

void* layerinLink(void* no_emulado)
{
    startLink(atoi((char* )no_emulado));
}

void* layerinNetwork(void* threadid)
{
    initnetwork();
}

int main(int argc, char* argv[])
{
	nodeSrc = atoi(argv[1]);

	/*
	*	Cria os mutex para os buffers de envio e recebimento
	*/
	if(pthread_mutex_init(&mutex0, NULL)!=0)
	{
		printf("Erro Mutex\n");
		return (-1);
	}

	if(pthread_mutex_init(&mutex1, NULL)!=0)
	{
		printf("Erro Mutex\n");
		return (-1);
	}

	if(pthread_mutex_init(&mutex2, NULL)!=0)
	{
		printf("Erro Mutex\n");
		return (-1);
	}

	if(pthread_mutex_init(&mutex3, NULL)!=0)
	{
		printf("Erro Mutex\n");
		return (-1);
	}

	if(pthread_mutex_init(&mutex4, NULL)!=0)
	{
		printf("Erro Mutex\n");
		return (-1);
	}
    pthread_mutex_lock(&mutex1);
    pthread_mutex_lock(&mutex3);
	/*
	*	Inicia teste do enlace (camada de rede)
	*/
	pthread_create(&layerNetwork, NULL, layerinNetwork, (void *)1);
    usleep(1000);
	/*
	*	Função que executa a camada de enlace
	*/

	pthread_create(&layerLink, NULL, layerinLink, (void *)argv[1]);

    pthread_join(layerLink, NULL);
    pthread_join(layerNetwork, NULL);

	/*
	*	Finaliza os mutex
	*/
	if(pthread_mutex_destroy(&mutex0)!=0)
	{
		printf("Erro destroy\n");
		return (-1);
	}

	if(pthread_mutex_destroy(&mutex1)!=0)
	{
		printf("Erro destroy\n");
		return (-1);
	}

	if(pthread_mutex_destroy(&mutex2)!=0)
	{
		printf("Erro destroy\n");
		return (-1);
	}

	if(pthread_mutex_destroy(&mutex3)!=0)
	{
		printf("Erro destroy\n");
		return (-1);
	}

	if(pthread_mutex_destroy(&mutex4)!=0)
	{
		printf("Erro destroy\n");
		return (-1);
	}
}
