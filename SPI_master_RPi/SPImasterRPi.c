#include <stdio.h>
#include <bcm2835.h>
#include <stdint.h>


#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <signal.h>


#define CS_1 2
#define CS_2 3
#define CS_3 4
#define CS_4 17
#define CS_5 18
#define CS_6 27
#define CS_7 22
#define CS_8 23
#define CS_9 24
#define CS_10 25
#define INIT_ESP 5
#define INIT_RASP 6
#define delay_calibration 1000

	// Configurando os pinos SPI
	//bcm2835_spi_begin();
	
	// Especificando a ordem dos bits de dados SPI
	//bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST); 
	
	// Definindo a polaridade dos pinos CS para baixo
	//bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, 0);
	//bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS1, 0);
	
	// Definindo a velocidade do clock do SPI
	//	BCM2835_SPI_CLOCK_DIVIDER_65536 = 0,       ///< 65536 = 262.144us = 3.814697260kHz (total H+L clock period) 
	//	BCM2835_SPI_CLOCK_DIVIDER_32768 = 32768,   ///< 32768 = 131.072us = 7.629394531kHz
	//	BCM2835_SPI_CLOCK_DIVIDER_16384 = 16384,   ///< 16384 = 65.536us = 15.25878906kHz
	//	BCM2835_SPI_CLOCK_DIVIDER_8192  = 8192,    ///< 8192 = 32.768us = 30/51757813kHz
	//	BCM2835_SPI_CLOCK_DIVIDER_4096  = 4096,    ///< 4096 = 16.384us = 61.03515625kHz
	//	BCM2835_SPI_CLOCK_DIVIDER_2048  = 2048,    ///< 2048 = 8.192us = 122.0703125kHz
	//	BCM2835_SPI_CLOCK_DIVIDER_1024  = 1024,    ///< 1024 = 4.096us = 244.140625kHz
	//	BCM2835_SPI_CLOCK_DIVIDER_512   = 512,     ///< 512 = 2.048us = 488.28125kHz
	//	BCM2835_SPI_CLOCK_DIVIDER_256   = 256,     ///< 256 = 1.024us = 976.5625MHz
	//	BCM2835_SPI_CLOCK_DIVIDER_128   = 128,     ///< 128 = 512ns = = 1.953125MHz
	//	BCM2835_SPI_CLOCK_DIVIDER_64    = 64,      ///< 64 = 256ns = 3.90625MHz
	//	BCM2835_SPI_CLOCK_DIVIDER_32    = 32,      ///< 32 = 128ns = 7.8125MHz
	//	BCM2835_SPI_CLOCK_DIVIDER_16    = 16,      ///< 16 = 64ns = 15.625MHz
	//	BCM2835_SPI_CLOCK_DIVIDER_8     = 8,       ///< 8 = 32ns = 31.25MHz
	//	BCM2835_SPI_CLOCK_DIVIDER_4     = 4,       ///< 4 = 16ns = 62.5MHz
	//	BCM2835_SPI_CLOCK_DIVIDER_2     = 2,       ///< 2 = 8ns = 125MHz, fastest you can get
	//	BCM2835_SPI_CLOCK_DIVIDER_1     = 1,       ///< 1 = 262.144us = 3.814697260kHz, same as 0/65536
	//bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_256);

	// Definindo o modo de dados SPI
	//	BCM2835_SPI_MODE0 = 0,  // CPOL = 0, CPHA = 0, Clock idle low, data is clocked in on rising edge, output data (change) on falling edge
	//	BCM2835_SPI_MODE1 = 1,  // CPOL = 0, CPHA = 1, Clock idle low, data is clocked in on falling edge, output data (change) on rising edge
	//	BCM2835_SPI_MODE2 = 2,  // CPOL = 1, CPHA = 0, Clock idle high, data is clocked in on falling edge, output data (change) on rising edge
	//	BCM2835_SPI_MODE3 = 3,  // CPOL = 1, CPHA = 1, Clock idle high, data is clocked in on rising, edge output data (change) on falling edge
	//bcm2835_spi_setDataMode(BCM2835_SPI_MODE3);		//(SPI_MODE_# | SPI_CS_HIGH)=Chip Select active high, (SPI_MODE_# | SPI_NO_CS)=1 device per bus no Chip Select

	// Definindo o pino CS para usar nas próximas transferências
	//bcm2835_spi_chipSelect(BCM2835_SPI_CS0);

	// Transferir 1 byte
	//uint8_t data;
	//data = bcm2835_spi_transfer((uint8_t)0x55);
	
	// Transferir muitos bytes
	//char data_buffer[10];
	//int Count;
	//for (Count = 0; Count < 10; Count++)
		//data_buffer[Count] = 0x80 + Count;
	//bcm2835_spi_transfern(&data_buffer[0], 10);			//data_buffer usado para tx e rx
	
	// Retorna os pinos SPI ao estado de entradas padrão
	//bcm2835_spi_end();

//*****************************************************
//*****************************************************
//********** DELAY FOR # uS WITHOUT SLEEPING **********
//*****************************************************
//*****************************************************
//Using delayMicroseconds lets the linux scheduler decide to jump to another process.  Using this function avoids letting the
//scheduler know we are pausing and provides much faster operation if you are needing to use lots of delays.

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



C
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





//-----------------------------------------------------------------------------------------------

void general_CS (const int CS){
	
	digitalWrite(CS, LOW);
	uint8_t ReadData;
	ReadData = bcm2835_spi_transfer((uint8_t)0x00);
	digitalWrite(CS, HIGH);
	delay(20);
	// criando a variável ponteiro para o arquivo
	FILE *pont_arq;
  
	//abrindo o arquivo
	pont_arq = fopen("/home/pi/laser.txt", "a");
	fprintf(pont_arq, "%u", ReadData);  

 	 // fechando arquivo
  	fclose(pont_arq);

}

int main(void) {

	export_gpio(CS_1);
	direction_gpio(CS_1, OUTPUT);
	//value_in_gpio(CS_1,HIGH)
	value_gpio(CS_1, HIGH);
	

        export_gpio(CS_2);
        direction_gpio(CS_2, OUTPUT);
        //value_in_gpio(CS_1,HIGH)
        value_gpio(CS_2, HIGH);
        

        export_gpio(CS_3);
        direction_gpio(CS_3, OUTPUT);
        //value_in_gpio(CS_1,HIGH)
        value_gpio(CS_3, HIGH);
        

        export_gpio(CS_4);
        direction_gpio(CS_4, OUTPUT);
        //value_in_gpio(CS_1,HIGH)
        value_gpio(CS_4, HIGH);


        export_gpio(CS_5);
        direction_gpio(CS_5, OUTPUT);
        //value_in_gpio(CS_1,HIGH)
        value_gpio(CS_5, HIGH);
       

        export_gpio(CS_6);
        direction_gpio(CS_6, OUTPUT);
        //value_in_gpio(CS_1,HIGH)
        value_gpio(CS_6, HIGH);
        

        export_gpio(CS_7);
        direction_gpio(CS_7, OUTPUT);
        //value_in_gpio(CS_1,HIGH);
        value_gpio(CS_7, HIGH);
        
	export_gpio(CS_8);
        direction_gpio(CS_8, OUTPUT);
        //value_in_gpio(CS_1,HIGH)
        value_gpio(CS_8, HIGH);
        

        export_gpio(CS_9);
        direction_gpio(CS_9, OUTPUT);
        //value_in_gpio(CS_1,HIGH)
        value_gpio(CS_9, HIGH);
        
	
	
        export_gpio(CS_10);
        direction_gpio(CS_10, OUTPUT);
        //value_in_gpio(CS_1,HIGH)
        value_gpio(CS_10, HIGH);
	
	export_gpio(INIT_RASP);
        direction_gpio(INIT_RASP, INPUT);
        value_in_gpio(INIT_RASP, HIGH);
        //value_gpio(CS_1, HIGH)
	
        
        export_gpio(INIT_ESP);
        direction_gpio(INIT_ESP, OUTPUT);
        //value_in_gpio(CS_1,HIGH)
        value_gpio(INIT_ESP, HIGH);
	delay(delay_calibration);
        

        
        
	bcm2835_spi_begin();
	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST); 
	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_256);
	bcm2835_spi_setDataMode(BCM2835_SPI_MODE3);

	
        while(digitalRead(INIT_RASP) == HIGH) {

                general_CS(CS_1);
		general_CS(CS_2);
		general_CS(CS_3);
		general_CS(CS_4);
		general_CS(CS_5);
		general_CS(CS_6);
		general_CS(CS_7);
		general_CS(CS_8);
                general_CS(CS_9);
                general_CS(CS_10);
		




        }
	bcm2835_spi_end();

	unexport_gpio(CS_1);
	unexport_gpio(CS_2);
	unexport_gpio(CS_3);
	unexport_gpio(CS_4);
	unexport_gpio(CS_5);
	unexport_gpio(CS_6);
	unexport_gpio(CS_7);
	unexport_gpio(CS_8);
	unexport_gpio(CS_9);
	unexport_gpio(CS_10);
	unexport_gpio(INIT_ESP);
	unexport_gpio(INIT_RASP);




        return 0;
}
       
