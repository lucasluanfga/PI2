Antes de usar o SPI é preciso que seja ativado na RPi da seguinte maneira:
* sudo raspi-config

Selecione "Opções avançadas" e, em seguida, selecione a opção relevante.

Para testar se a porta SPI está funcionando, digite:
* ls /dev/spidev*

Deve aparecer o seguinte:
* /dev/spidev0.0  /dev/spidev0.1

Existem 2 dispositivos spidev mostrados. O primeiro número refere-se ao periférico SPI, que em ambos os casos é 0 (o RPi tem apenas 1 porta SPI), o segundo número representa os pinos de seleção de chip CS0 e CS1.
