#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "library.h"
#include "garbler.h"

//estrutura para armazenar os nós e enlaces
typedef struct
{
	int id;
	char ip[20];
	int port;

}no; no nos[6];


struct sockaddr_in physicalSend, physicalReceive;

int matrix[4][4];
int neighbor[4][4];


//========================================================= send Physical ==========================================//
void *sendPhy(void *arg)
{
	char package[100];
	int mtu, error, sock;
	char no_dest[2];

	printf("%s\n", linkPhysical);
	while(1)
	{
		error = 0;
		if(pthread_mutex_lock(&mutex2)!=0)
		{
			printf("Erro ao travar o mutex 2");
			break;
			//return (-1);
		}
	
		if(pthread_mutex_lock(&mutex1)!=0)
		{
			printf("Erro ao travar o mutex 1");
			break;
			//return (-1);
		}
		memcpy(package, linkPhysical, 100);

		nodeDest=package[0] - 40;
        mtu = neighbor[nodeSrc-1][nodeDest-1];

        printf(" MTU: %d", mtu);
		if(mtu <= 0)
		{
			error = -404;
		}

		if(analyzeMTU(package, mtu)==-1 && error == 0)
		{
			error=-20;
		}
		if(error >= 0)
		{
				if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
				{
					perror("socket()");
					exit(1);
				}

				physicalSend.sin_family      = AF_INET;										
				physicalSend.sin_port        = htons(nos[nodeDest-1].port);	  
				physicalSend.sin_addr.s_addr = inet_addr(nos[nodeDest-1].ip);

				printf("IP: %s port: %d\n", nos[nodeDest - 1].ip, nos[nodeDest - 1].port);
			
				
				if((sendto_garbled(sock, package, 100, 0, (struct sockaddr *)&physicalSend, sizeof(physicalSend))) < 0)
				{
					if (error >=0 )
					{
						error = -606;
					}
				}
				
				close(sock);
		}

        if(pthread_mutex_unlock(&mutex0)!=0)
        {
			printf("Erro ao destravar o mutex 0");
			break;
			//return (-1);
		}
		
		errorDataLink = error;

		if(pthread_mutex_unlock(&mutex2)!=0)
		{
			printf("Erro ao destravar o mutex 2");
			break;
			//return (-1);
		}
		
	}
	
}
//========================================================= receive Physical =======================================//


void *receivePhy(void *arg)
{
		char package[100];
		int address_size;

		int l_soc= 3;//(int )arg;

		if((l_soc = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
		{
			perror("socket()");
			exit(1);
		}

		physicalReceive.sin_family      = AF_INET;										
		physicalReceive.sin_port        = htons(nos[2].port);	
		physicalReceive.sin_addr.s_addr = INADDR_ANY;


		while(1)
		{
				if((pthread_mutex_lock(&mutex4))!=0)
				{
					printf("Erro ao travar o mutex 4");
					break;
					// return (-1);
				}

				if(recvfrom(l_soc, package, 100, 0, (struct sockaddr *)&physicalReceive, &address_size) <0)
				{
					perror("recvfrom()");
					exit(1);
				}
				//Checksum

		    	if(pthread_mutex_unlock(&mutex3)!=0)
		    	{
				    printf("Erro ao destravar o mutex 3");
				    break;
				    // return (-1);
				}

		}
}

//========================================================= Topology Link ==========================================//

int TopologyLink()
{
	char textoNos[20];
	char textoEnlace[20];
	int no;
	int PrimeiroNumber;
	int SegundoNumber;
	int TerceiroNumber;
	int QuartoNumber;
	int porta;
	int regress;
	int node=0;
	int origem, destino;
	int mtu;
	int nodeEnlace=0;
	
	FILE *f = fopen("topologia1.txt", "r");
	if(f == NULL)
	{
		printf("Erro na abertura \n");
		exit(1);
	}

	fscanf(f, "%s", textoNos);
	
	do
	{
	      regress = fscanf(f, "%d: IP = %d.%d.%d.%d, Porta = %d;", &no, &PrimeiroNumber, &SegundoNumber, &TerceiroNumber, &QuartoNumber, &porta);
	      
	      if(regress >= 6)
	      {
		  //printf("%d: %d.%d.%d.%d, %d\n", no, PrimeiroNumber, SegundoNumber, TerceiroNumber, QuartoNumber, porta);
			  nos[node].id = no;
			  sprintf(nos[node].ip,"%d.%d.%d.%d",PrimeiroNumber,SegundoNumber,TerceiroNumber,QuartoNumber);
			  nos[node].port = porta;
			  node++;
	      }

	}while(regress>=6);

	fscanf(f, "%s", textoEnlace);

	do
	{
	      regress = fscanf(f,"%d -> %d, MTU = %d;", &origem,&destino,&mtu);

	      if(regress >=3)
	      {
		      matrix[origem - 1][destino - 1] = mtu;
	      }

	}while(regress>=3);

	return 0;
}
//========================================================= CheckNeighbors ==========================================//

int checksNeighbors()
{
	int i;
	int j;
	int auxmtu;

	neighbor[0][0] = 0;
	neighbor[0][1] = 0;
	neighbor[1][0] = 0;
	neighbor[1][1] = 0;
	neighbor[2][0] = 0;
	neighbor[2][1] = 0;
	neighbor[3][0] = 0;
	neighbor[3][1] = 0;
	
	for(i=0; i<4; i++)
	{
	    for(j=1; j<4; j++)
	    {
		   if(matrix[i][j] != 0)
           {
				neighbor[i][j] = matrix[i][j]; 
		   }	    
	    }
	}
	return 0;
}

//========================================================= showResults ==========================================//
void showResults()
{

	int k = 0;
	int a, b;
	int j = 0;
	
	
	printf(" \n Nos \n");
	while(k < 4) 
	{
			printf("%d: IP = %s Porta = %d;\n", nos[k].id, nos[k].ip, nos[k].port);
			k++;
	}
	
	printf(" \n Enlaces \n");
	
	for(a=0; a<4; a++)
	{

		for(j=0; j<4; j++)
		{
			if(neighbor[a][j] != 0)
			{
				printf("%d -> %d, MTU = %d\n", a+1, j+1, neighbor[a][j]);
			}
		}
	}
	
	printf("\n");
	
	printf("\n");
}

//========================================================= start link ==========================================//
int startLink(int noemulated)
{

	int sock;
	readingFile();

	if((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("socket()");
		exit(1);
	}

	physicalReceive.sin_family      = AF_INET;										
	physicalReceive.sin_port        = htons(nos[noemulated].port);	
	physicalReceive.sin_addr.s_addr = INADDR_ANY;

	if (bind(sock, (struct sockaddr *)&physicalReceive, sizeof(physicalReceive)) < 0)
	{
		perror("bind()");
		exit(1);
	}

	if(pthread_create(&tdispach, NULL, sendPhy, (void *)noemulated))
	{
		printf("Erro ao criar a thread para enviar\n");
		return (-1);
	}

	if(pthread_create(&treap, NULL, receivePhy, (void *)sock))
	{
		printf("Erro ao criar a thread para receber\n");
		return (-1);
	}

	if(pthread_join(tdispach, NULL)!=0)
	{
		printf("Erro Thread Join\n");
	}

	if(pthread_join(treap, NULL)!=0)
	{
		printf("Erro Thread Join\n");
	}
}

//========================================================= analyzeMTU ==========================================//

int analyzeMTU(char pacote[], int mtu)
{
	int tam_pacote;
	tam_pacote=strlen(pacote);

	if(tam_pacote>mtu)
	{ 
		return -1;
	}else
	{
		return 0;
	}
		
}

//========================================================= reading file ==========================================//

readingFile()
{
   TopologyLink();
   checksNeighbors();
   //showResults();
   return 0;
}





