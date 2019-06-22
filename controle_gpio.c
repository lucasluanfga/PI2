// Configurando os pinos GPIO em um sistema de arquivos em memória com a biblioteca Sysfs

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <signal.h>
#include <stdio.h>
#include <stdint.h>



#define HIGH   1
#define LOW    0
#define INPUT  0
#define OUTPUT 1

int arquive;
float timeSleep = 1, pin = 13; // pin = pino gpio da placa
char buffer[3];
char path[35];



//bool access_gpio(int pin);
//bool export_gpio(int pin);
//bool direction_gpio(int pin, int direction);
//ool value_gpio(int pin, int value);
//bool unexport_gpio(int pin);

//void delay(float time);

void delay(float time)
{
	struct timespec t;
	int seg;
	seg = time;
	t.tv_sec = seg;
	t.tv_nsec = (time-seg)*1e9;
	nanosleep(&t, NULL);
}

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



int main (int argc, char * argv[]){
     

               export_gpio(pin);
               direction_gpio(pin, OUTPUT);
               
               while(1){
                    
               value_gpio(pin, HIGH);
               delay(timeSleep);
               value_gpio(pin, LOW);
               delay(timeSleep);
               }
               unexport_gpio(pin);
          
     return 0;
}
