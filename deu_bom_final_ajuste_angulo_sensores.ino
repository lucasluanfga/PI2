#include<Wire.h>
                                                // Nota: g = aceleração devido a gravidade = 9.81 m/s^2 

                                                // Pequenas vibrações requer sensibilidade mais alta
// Definindo o endereço I2C do MPU
int MPU = 0x68;

// Definindo os vetores de valores bruto das leituras do acelerometro e giroscopio
int16_t accResult[3], gyroResult[3];

// Definimos as taxas de conversão
#define A_R 16384.0                     // mais ou menos 250 graus/seg
#define G_R 131.0                       // mais ou menos 2 g

// Definimos a conversão de radianos para graus 180/PI
#define RAD_A_DEG = 57.295779

// define números de pinos do driver no arduino
const int stepPin = D3;
const int dirPin = D4;

// Angulos
float Acc[2];
float Gy[3];
float Angle[2];

//Função de escrita em um dispositivo I2C
void writeTo(byte device, byte toAddress, byte val) {
  Wire.beginTransmission(device);     // Inicia a transmissão de dados para o endereço do dispositivo 
  Wire.write(toAddress);               // Endereça o registrador que irá ser escrito a informacao  
  Wire.write(val);                      // Valor que será enviado via SDA para o endereço descrito anteriormente (8bits)
  Wire.endTransmission();                // Encerra a transmissao, liberando o barramento para uma nova operacao 
}

//Função de leitura em um dispositivo I2C
void readFrom(byte device, byte fromAddress, int num, byte result[]) {
  Wire.beginTransmission(device);
  Wire.write(fromAddress);
  Wire.endTransmission();
  Wire.requestFrom((int)device, num);       // Informa o dispositivo identificado por endereço que queremos ler uma posição de memória. num = numero de bytes requeridos
  int i = 0;
  while(Wire.available()) {                 // Verifica se há informação disponível para leitura
    result[i] = Wire.read();                // Lê um byte de cada vez e armazena no veotr result[i]
    i++;
  }
}

//Função que lê as leituras do acelerômetro
void GetAccelerometerReadings(int16_t Result[]){
byte buffer[6];
readFrom(MPU, 0x3B, 6, buffer);                   //Lê 6 bytes no registrador de apenas leitura 0x3B(ACCEL_XOUT) e armazena no buffer os valores
Result[0] = (((int16_t)buffer[0]) << 8) | buffer[1];  // Armazena os valores do sensor num vetor
Result[1] = (((int16_t)buffer[2]) << 8) | buffer[3];
Result[2] = (((int16_t)buffer[4]) << 8) | buffer[5];
}

//Função que lê as leituras do giroscópio
void GetGyroscopeReadings(int16_t Result[])
{
byte buffer[6];
readFrom(MPU, 0x43, 6, buffer);                   // leitura do registrador 0x43 (GYRO_XOUT[15:8])
Result[0] = (((int16_t)buffer[0]) << 8) | buffer[1];
Result[1] = (((int16_t)buffer[2]) << 8) | buffer[3];
Result[2] = (((int16_t)buffer[4]) << 8) | buffer[5];
}

void setup()
{
  Wire.begin(D1,D2);        // Inicia a comuinicação I2C (D1 = SDA, D2 = SCL)
//Wire.begin();
  writeTo(MPU, 0x6B, 0);    // Inicializa o MPU-6050
  Serial.begin(115200);     // Inicia a comunicação serial a uma dada taxa de transmissão (bits/sec)

  // Define os dois pinos como Saídas
pinMode (stepPin, OUTPUT);
pinMode (dirPin, OUTPUT);
}

void loop()
{
  GetAccelerometerReadings(accResult);
  GetGyroscopeReadings(gyroResult);

// Com os valores do acelerômetro é calculado os ângulos Y e X com a fórmula da tangente.
   Acc[1] = atan(-1*(accResult[0]/A_R)/sqrt(pow((accResult[1]/A_R),2) + pow((accResult[2]/A_R),2)))*RAD_TO_DEG;         // Ângulo Y
   Acc[0] = atan((accResult[1]/A_R)/sqrt(pow((accResult[0]/A_R),2) + pow((accResult[2]/A_R),2)))*RAD_TO_DEG;            // Ângulo X

// Calculando os ângulos do giroscópio
   Gy[0] = gyroResult[0]/G_R;
   Gy[1] = gyroResult[1]/G_R;
   Gy[2] = gyroResult[2]/G_R;

// Aplicando o Filtro Complementar
   Angle[0] = 0.98 *(Angle[0]+Gy[0]*0.010) + 0.02*Acc[0];
   Angle[1] = 0.98 *(Angle[1]+Gy[1]*0.010) + 0.02*Acc[1];

   Angle[2] = Angle[2] + Gy[2] * 0.010; 

// Mostrando os valores pelo monitor serial
   //Serial.print("Ângulo X: "); Serial.print(Angle[0]); Serial.print("\n");
  // Serial.print("Ângulo Y: "); Serial.print(Angle[1]); Serial.print("\n------------\n");
  // Serial.print("Ângulo Y: "); Serial.print(Angle[1]); Serial.print("\n");
   //Serial.print("Ângulo Z: "); Serial.print(Angle[2]); Serial.print("\n------------\n");
 
   delay(10); // Nosso dt será de 0.010, que é o intervalo de tempo em cada medição

   int angulo;
   angulo = map(Angle[0], -90, 90, 0, 180); // Para mudar o grau de liberdade basta trocar aqui Algle[0] por Algle[1]
   Serial.print("Ângulo X: "); Serial.print(angulo); Serial.print("\n");

   // -----------------------------------------------------------------------------------------------------------------------
   // Controle do motor de passo


  int default_calibracao = 89;

  pinMode(D5, INPUT); //setando pino de calibração como entrada para receber o nível lógico da raspberry


  if (digitalRead(D5) == HIGH){ // se em nivel logico alto, inicia a calibração
    
      if(angulo > default_calibracao + 1 ){
        digitalWrite(dirPin, LOW);

        // faz 1 pulso no sentido horario
        for(int x = 0; x < 1; x++) {
          digitalWrite(stepPin,HIGH);
          delayMicroseconds(500);
          digitalWrite(stepPin,LOW);
          delayMicroseconds(500);
        }
        delayMicroseconds(1000);    
      }
             
      if(angulo < default_calibracao){
    
          digitalWrite(dirPin,HIGH);
    

          // faz 400 pulsos para fazer duas rotações de ciclo completo
 n0-          for(int x = 0; x < 1; x++) {
          digitalWrite(stepPin,HIGH);
          delayMicroseconds(500);
          digitalWrite(stepPin,LOW);
          delayMicroseconds(500);
          }
          delayMicroseconds(1000);    
        }          
          
   
    }
    
  
  
}
