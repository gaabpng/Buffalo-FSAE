#include <SPI.h>
#include <LoRa.h>

// Pinos para o LoRa (LilyGO T3 1.6.1)
#define SS_PIN    5      // Chip Select (NSS)
#define RST_PIN   14     // Reset
#define DIO0_PIN  26     // Interrupção DIO0

// Configuração UART
#define RX_PIN    3      // Pino RX do ESP32 (padrão)
#define TX_PIN    1      // Pino TX do ESP32 (padrão)

#define UART_BAUD 115200

void setup() {
  // Inicializa o monitor serial
  Serial.begin(115200);
  
  // Inicializa a UART com o Arduino
  Serial1.begin(UART_BAUD, SERIAL_8N1, RX_PIN, TX_PIN);

  // Inicializa o módulo LoRa
  if (!LoRa.begin(915E6, SS_PIN, RST_PIN, DIO0_PIN)) {
    Serial.println("Erro ao inicializar o LoRa.");
    while (1);  // Se falhar, trava o sistema
  }
  
  Serial.println("LoRa Iniciado com sucesso.");
}

void loop() {
  // Verifica se há dados disponíveis via UART do Arduino
  if (Serial1.available()) {
    // Lê a string de temperatura enviada pela UART
    String tempStr = Serial1.readString();

    // Envia a temperatura recebida via LoRa
    LoRa.beginPacket();
    LoRa.print(tempStr);  // Envia a string de temperatura
    LoRa.endPacket();

    // Exibe os dados enviados no monitor serial (opcional)
    Serial.print("Temperatura enviada via LoRa: ");
    Serial.println(tempStr);
  }

  delay(100);  // Pequeno delay entre as leituras
}
