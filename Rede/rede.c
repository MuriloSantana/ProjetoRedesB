/*MURILO SANTANA
  LEONARDO BONETTI MAFFEI
  MURILO BERGO
*/

#include "library.h"

typedef struct
{
	int id;
	char ip[50];
	int port;
	
}no; no nos[ENTIRE];


int main(int argc, char *argv[])
{
   readingFile_one();
   pthread_t threads[NUM_THREADS];
   int rc;
   long t;

   for(t=0; t<NUM_THREADS; t++)
   {

	      printf("In main: creating thread %ld\n", t);
	      rc = pthread_create(&threads[t], NULL, DisclosesRouters, (void *)t);

	      if (rc)
	      {
	         printf("ERROR; return code from pthread_create() is %d\n", rc);
	         exit(-1);
	      }

   }

   return 0;
}

void *DisclosesRouters(void *arg)
{
	int i;
	int j = (long)arg;
	int k=0; 
	int soc;
	char routes_discloses[100];
	char n_no[3]; 
	char cost[10]; 
	char data[100];
	
	for( ; ; )
	{
			sprintf(n_no, "%d", emulated);
			strcpy(routes_discloses, n_no);

			while( k<4 )
			{
				strcat(routes_discloses," ");
				sprintf(cost, "%d", routes_cost[emulated-1][k]);
				strcat(routes_discloses, cost);
				k++;
			}

			while( k<j )
			{

					sprintf(data, "%d", neighbor[k][k]);		// atribui o numero do enlace vizinho ao inicio do dado
					memcpy(data+1, routes_discloses, 99);	   // coloca o conteudo da tabela como dado

					if(pthread_mutex_lock(&mutexlinkTransport0)!=0)
					{
						printf("Erro destravar");
						exit(1);
					}
					
					 
					//Colocar rotas_div no buffer para divulgar
					controlTransportTable=1;
					memcpy(transportNetwork, data, 100);		// passa para a camada de rede

					if(pthread_mutex_unlock(&mutexlinkTransport1)!=0)
					{
						printf("Erro travar");
						exit(1);
					}

					k++;
			}



	}

}
void *sendLinkUnion(void *arg)
{

}

void *recevLinkUnion(void *arg)
{

}

int InitiNetwork(int l_no_emulado)
{

	//inicializando os mutex
	if(pthread_mutex_init(&mutexroutes0, NULL)!=0)
	{
		printf("Erro Mutex\n");
		return (-1);
	}

	if(pthread_mutex_init(&mutexroutes1, NULL)!=0)
	{
		printf("Erro Mutex\n");
		return (-1);
	}

	if(pthread_mutex_lock(&mutexroutes1)!=0)
	{
		 printf("Erro destravar");
		 return (-1);
	}

	//cria threads para enviar e receber
	if(pthread_create(&sendLink, NULL, sendLinkUnion, (void* )1))
	{
		printf("Erro thread\n");
		return (-1);
	}
	usleep(20);
	if(pthread_create(&recevLink, NULL, recevLinkUnion, (void* )1))
	{
		printf("Erro thread\n");
		return (-1);
	}

	//divulga e recebe a tabela de rotas
	//inicia_rotas();

	if(pthread_join(sendLink, NULL))
	{
		printf("Erro thread Join\n");
	}

	if(pthread_join(recevLink, NULL))
	{
		printf("Erro thread Join\n");
	}

	if(pthread_mutex_destroy(&mutexroutes0)!=0)
	{
		printf("Erro destroy\n");
		return (-1);
	}

	if(pthread_mutex_destroy(&mutexroutes1)!=0)
	{
		printf("Erro destroy\n");
		return (-1);
	}
}

int TopologyLink_one()
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

int checksNeighbors_one()
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
void showResults_one()
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
}

int readingFile_one()
{
   TopologyLink_one();
   checksNeighbors_one();
   showResults_one();
   return 0;
}