#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>

// Instância do sensor IR
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

// Controle de tempo
unsigned long ultimaLeituraObj = 0;
const unsigned long intervaloIR = 50;      // 50 ms

void setup() {
  // Inicializa a comunicação UART (pinos 0 e 1)
  Serial.begin(115200);

  // Inicializa o sensor de temperatura IR
  mlx.begin();
}

void loop() {
  unsigned long agora = millis();

  // Leitura do IR a cada 50ms
  if (agora - ultimaLeituraObj >= intervaloIR) {
    ultimaLeituraObj = agora;

    // Lê a temperatura do sensor IR
    float tempIR = mlx.readObjectTempC();

    // Se a leitura for válida, envia via UART
    if (!isnan(tempIR)) {
      String tempStr = String(tempIR, 2);  // Convertendo para string com 2 casas decimais
      Serial.print(tempStr);  // Envia a temperatura para o ESP
      Serial.write('\n');     // Adiciona uma nova linha para separar as leituras
    }
  }
}
