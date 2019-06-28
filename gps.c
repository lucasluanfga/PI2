#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <errno.h>
#include <string.h>

// Este codigo requer a biblioteca wringPi. Compile-o usando
//    gcc Ex4.c -lwiringPi -o Ex4.out

// Arquivo de acesso a porta serial
//#define TTY "/dev/ttyAMA0"
// Arquivo de acesso a porta serial
// PARA O RASPBERRY PI 3
#define TTY "/dev/ttyS0"

int uart0_fd;
int i, fim = 0;
char s[1000];
char gps[1000];



void ctrl_c(int sig)
{
	puts(" Fechando " TTY "...");
	serialClose(uart0_fd);
	exit(-1);
}

int main(void)
{
	unsigned char msp430_input=1;
	int serial_status;

	signal(SIGINT, ctrl_c);
	uart0_fd = serialOpen(TTY, 9600);
	if(uart0_fd==-1)
	{
		puts("Erro abrindo a UART. Garanta que ela nao");
		puts("esteja sendo usada por outra aplicacao.");
		return -1;
	}
	if(wiringPiSetup() == -1)
	{
		puts("Erro em wiringPiSetup().");
		return -1;
	}

	serialFlush(uart0_fd);
			
	for (i = 0; fim ==0; i++){
		s[i] = serialGetchar(uart0_fd);
		if (i > 0){
			if((s[i-1] == '\r') && (s[i] == '\n'))
				fim = 1;
			}
		}
		//delay(20);

	printf("%s", s );
	
	serialClose(uart0_fd);
 
}
