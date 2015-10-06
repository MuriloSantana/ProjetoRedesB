#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <crypt.h>
#include <pthread.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio_ext.h>
#include <unistd.h>
#include <netdb.h>
#include <fcntl.h>
#include <signal.h>

#define MAXPACKAGE 20000
#define  ENTIRE 6
#define  NUM_THREADS 5

int routes_cost[ENTIRE][ENTIRE];
int neighbors[ENTIRE-1];
int emulated;
int matrix[ENTIRE][ENTIRE]; //
int neighbor[ENTIRE][ENTIRE]; //matriz onde armazena os vizinhos

pthread_t sendLink;    
pthread_t recevLink;

pthread_mutex_t mutex0; 
pthread_mutex_t mutex1; 
pthread_mutex_t mutex2; 
pthread_mutex_t mutex3;
pthread_mutex_t mutex4;

pthread_mutex_t mutexroutes0;
pthread_mutex_t mutexroutes1;


//mutex referente a camada de rede transport
pthread_mutex_t mutexlinkTransport0;
pthread_mutex_t mutexlinkTransport1;
pthread_mutex_t mutexlinkTransport2;
pthread_mutex_t mutexlinkTransport3;

//definir se o dado veio da tabela ou da Camada de rede
int controlTransportTable;


char transportNetwork[MAXPACKAGE];
char networkTransport[MAXPACKAGE];

void *DisclosesRouters(void *arg);
int readingFile_one(); 			