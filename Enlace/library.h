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

#include "garbler.h"

pthread_mutex_t mutex0; 
pthread_mutex_t mutex1; 
pthread_mutex_t mutex2; 
pthread_mutex_t mutex3;
pthread_mutex_t mutex4;

char linkPhysical[100];
int errorDataLink;

pthread_t tdispach; //para o send
pthread_t treap; //para p recev

int nodeSrc;
int nodeDest;
