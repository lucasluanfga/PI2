//Incluindo bibliotecas para comunicação SPI

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <unistd.h>

//bibliotecas para controle dos pinos GPIO

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdbool.h>
#include <time.h>
#include <stdint.h>


//Definindo os pinos
 
#define CS_1 2
//#define CS_2 3
//#define CS_3 5
//#define CS_4 17
//#define CS_5 18
//#define CS_6 27
//#define CS_7 22
//#define CS_8 23
//#define CS_9 24
//#define CS_10 25
//#define INIT_ESP 20
#define SYNC 19


int spi_fd;
const int speed = 1000000;  //Velocidade do clock em HZ


int arquive;
char buffer[3];
char path[35];

//Incluindo funções para setar os pinos GPIO  
//--------------------------------------------------------------------------


//Exportando o pino
bool export_gpio(int pin)
{
    	arquive = open ("/sys/class/gpio/export", O_WRONLY);
    	if (arquive==-1)
    	{
            	printf("Arquivo abriu incorretamente\n");
            	return false;
    	}
    	snprintf(buffer, 3, "%d", pin);
    	if(write(arquive, buffer, 3) == -1)
    	{
            	close(arquive);
            	return false;
    	}


    	close(arquive);


    	return true;
}

//Configurando o pino como INPUT ou OUTPUT
bool direction_gpio(int pin, int direction)
{
     arquive=0;
     snprintf(path, 35, "/sys/class/gpio/gpio%d/direction", pin);
     arquive = open (path, O_WRONLY);
     if (arquive==-1)
     {
             return false;
     }
     snprintf(buffer, 3, "%d", pin);
     if (write( arquive, ((direction == INPUT)?"in":"out"), 3 )==-1)
     {
             close(arquive);
             return false;
     }
     close(arquive);
     return true;
}

//Efetuando a leitura
int value_in_gpio(int pin, int value)
{
     arquive=0;
     char retorno[3];
     snprintf(path, 35, "/sys/class/gpio/gpio%d/value", pin);
     arquive = open(path, O_RDONLY);
     //printf("Descritor do arquivo: %d \n", arquive);
     if (arquive == -1)
     {
             return false;
     }
     if (read(arquive, retorno, 3) == -1)
     {
             close(arquive);
             return false;
     }
     close(arquive);
     printf("Valor do pino: %c \n", retorno[0]);
 
 
     return atoi(retorno);
}


//Escrita no pino
bool value_gpio(int pin, int value)
{
     arquive=0;
     snprintf(path, 35, "/sys/class/gpio/gpio%d/value", pin);
     arquive = open(path, O_WRONLY);
     if (arquive == -1)
     {
             return false;
     }
     if (write (arquive, ((value == HIGH)?"1":"0"), 1) == -1)
     {
             close(arquive);
             return false;
     }
     close(arquive);
     return true;
}

//Desutilizando o pino exportado
bool unexport_gpio(int pin)
{
    	arquive = open ("/sys/class/gpio/unexport", O_WRONLY);
    	if (arquive==-1)
    	{
            	printf("Arquivo abriu incorretamente\n");
            	return false;
    	}
    	if(write(arquive, buffer, 3) == -1)
    	{
            	close(arquive);
            	return false;
    	}
    	return true;
}

//------------------------------------------------------------------------

//Função para controle do chip select

void general_CS (const int CS){
	
	
	value_gpio(CS, LOW);
	
	uint8_t ReadData;
	wiringPiSPIDataRW(0, &ReadData, 1);
	ReadData = ReadData - 56;
	printf("ReadData: %u \n", ReadData);
	value_gpio(CS, HIGH);
	// criando a variável ponteiro para o arquivo
	FILE *pont_arq;
  
	//abrindo o arquivo
	pont_arq = fopen("/home/pi/laser.txt", "a");
	fprintf(pont_arq, "%u\n", ReadData);  

 	 // fechando arquivo
  	fclose(pont_arq);

}


//----------------------------------------------------------------------
//Função principal

int main(void) {
	
	//Verificando se há erro ao inicializar 
	if(wiringPiSetup() == -1){		
			puts("Erro em wiringPiSetup().");
			return -1;
	}
	
	spi_fd = wiringPiSPISetupMode (0, speed, 1);
	
	if(spi_fd == -1){
		puts("Erro abrindo a SPI. Garanta que ela nao");
		puts("esteja sendo usada por outra aplicacao.");
		return -1;
	}
	
//-------------------------------------------------------------------------------------
	
	export_gpio(SYNC);
	direction_gpio(SYNC, OUTPUT);
	value_gpio(SYNC, LOW);
	

	export_gpio(CS_1);
	direction_gpio(CS_1, OUTPUT);
	value_gpio(CS_1, HIGH);
	
//      export_gpio(CS_2);
//      direction_gpio(CS_2, OUTPUT);
//      value_gpio(CS_2, HIGH);
        
//      export_gpio(CS_3);
//      direction_gpio(CS_3, OUTPUT);
//      value_gpio(CS_3, HIGH);

//      export_gpio(CS_4);
//	direction_gpio(CS_4, OUTPUT);
//	value_gpio(CS_4, HIGH);

//	export_gpio(CS_5);
//	direction_gpio(CS_5, OUTPUT);
//	value_gpio(CS_5, HIGH);       

//	export_gpio(CS_6);
//	direction_gpio(CS_6, OUTPUT);
//	value_gpio(CS_6, HIGH);
        
//	export_gpio(CS_7);
//	direction_gpio(CS_7, OUTPUT);
//	value_gpio(CS_7, HIGH);
        
//	export_gpio(CS_8);
//	direction_gpio(CS_8, OUTPUT);
//	value_gpio(CS_8, HIGH);

//	export_gpio(CS_9);
//	direction_gpio(CS_9, OUTPUT);
//	value_gpio(CS_9, HIGH);
        
//	export_gpio(CS_10);
//	direction_gpio(CS_10, OUTPUT);
//	value_gpio(CS_10, HIGH);
        
//	export_gpio(INIT_ESP);
//	direction_gpio(INIT_ESP, OUTPUT);
//	value_in_gpio(CS_1,HIGH)
//	value_gpio(INIT_ESP, HIGH);
	
while(1){

	value_gpio(SYNC, HIGH);
	delay(20);

	value_gpio(SYNC, LOW);
	
	general_CS(CS_1);
//	general_CS(CS_2);

	
  

//	general_CS(CS_3);
//	general_CS(CS_4);
//	general_CS(CS_5);
//	general_CS(CS_6);
//	general_CS(CS_7);
//	general_CS(CS_8);
//	general_CS(CS_9);
//	general_CS(CS_10)
	
 
        
}	

	close(spi_fd);
	
	unexport_gpio(CS_1);
//	unexport_gpio(CS_2);
//	unexport_gpio(CS_3);
//	unexport_gpio(CS_4);
//	unexport_gpio(CS_5);
//	unexport_gpio(CS_6);
//	unexport_gpio(CS_7);
//	unexport_gpio(CS_8);
//	unexport_gpio(CS_9);
//	unexport_gpio(CS_10);

//	unexport_gpio(INIT_ESP);
	unexport_gpio(SYNC);


	return 0;
}
