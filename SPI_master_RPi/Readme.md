Antes de usar o SPI é preciso que seja ativado na RPi da seguinte maneira:
* sudo raspi-config

Selecione "Opções avançadas" e, em seguida, selecione a opção relevante.

Reinicie a rasp

Para testar se a porta SPI está funcionando, digite:
* ls /dev/spidev*

Deve aparecer o seguinte:
* /dev/spidev0.0  /dev/spidev0.1

Existem 2 dispositivos spidev mostrados. O primeiro número refere-se ao periférico SPI, que em ambos os casos é 0 (o RPi tem apenas 1 porta SPI), o segundo número representa os pinos de seleção de chip CS0 e CS1.

Agora é preciso instalar a biblioteca bcm2835:

Nos comandos abaixo, altere o .XX para corresponder ao número da versão atual da biblioteca, por exemplo, ".37".

Usando o RPi, faça o download do arquivo .tar.gz da página da biblioteca para o diretório raiz "/ home / pi /". Você pode fazer isso usando wget na linha de comando:

* cd / home / pi /

* wget http://www.airspayce.com/mikem/bcm2835/bcm2835-1.XX.tar.gz

Instale os arquivos da biblioteca usando os seguintes comandos

Descompacte o arquivo baixado (Altere o número da versão do arquivo para corresponder à sua versão baixada)

* tar zxvf bcm2835-1.XX.tar.gz

Os arquivos serão descompactados em uma pasta chamada "/home/pi/bcm2835-#.#", em que # é o número da versão. (As seguintes instruções são baseadas nas instruções do arquivo bcm2835.h, portanto, se algo não funcionar, verifique lá se as instruções foram alteradas)

Altere para o diretório os arquivos que foram descompactados (Altere o número da versão do diretório para que corresponda à versão baixada)

* cd bcm2835-1.XX

Execute o configure exe

* ./configure

Execute o makefile

* make

Então

* sudo make check

Então

* sudo make install

(O sudo é necessário para elevar as permissões para o usuário root)

A biblioteca está agora pronta para uso.

Para compilar o código, basta usar o seguinte comando exemplo:

* sudo example.c -o example -lbcm2835

Qualquer dúvida, acesse o seguinte site:

https://raspberry-projects.com/pi/programming-in-c/io-pins/bcm2835-by-mike-mccauley


#include <bcm2835.h>


