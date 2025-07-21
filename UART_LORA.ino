#include <LoRa.h>
#include <SPI.h>

// === Pinos LoRa (padrão do LilyGO LoRa32 com OLED) ===
#define SCK_LORA        5
#define MISO_LORA       19
#define MOSI_LORA       27
#define SS_PIN_LORA     18
#define RESET_PIN_LORA  14
#define LORA_DIO0       26

// === Frequência LoRa ===
#define BAND            915E6
#define HIGH_GAIN_LORA  20

// === Pinos UART (forçados nos pinos I2C) ===
#define RXD1 21  // GPIO21 ← recebe do Arduino Mega (TX1 pino 18)
#define TXD1 22  // GPIO22 → envia para Arduino Mega (RX1 pino 19) – opcional

String buffer = "";

void setup() {
  Serial.begin(115200);  // Debug via USB
  while (!Serial);

  // Inicia UART com pinos 21 e 22
  Serial1.begin(115200, SERIAL_8N1, RXD1, TXD1);

  // Inicializa LoRa
  Serial.println("[ESP32 LoRa] Iniciando LoRa...");
  SPI.begin(SCK_LORA, MISO_LORA, MOSI_LORA, SS_PIN_LORA);
  LoRa.setPins(SS_PIN_LORA, RESET_PIN_LORA, LORA_DIO0);

  if (!LoRa.begin(BAND)) {
    Serial.println("[ESP32 LoRa] Falha ao iniciar LoRa");
    while (1);
  }

  LoRa.setTxPower(HIGH_GAIN_LORA);
  Serial.println("[ESP32 LoRa] LoRa iniciado com sucesso!");
}

void loop() {
  // Lê dados da UART (vindos do Arduino Mega)
  while (Serial1.available()) {
    char c = Serial1.read();

    // Monta o buffer até encontrar \n
    if (c == '\n') {
      Serial.print("[ESP32 LoRa] Linha recebida via UART: ");
      Serial.println(buffer);

      // Envia via LoRa
      LoRa.beginPacket();
      LoRa.print(buffer);
      LoRa.endPacket();

      Serial.println("[ESP32 LoRa] Enviado via LoRa!");
      buffer = "";  // limpa o buffer para a próxima linha
    } else {
      buffer += c;
    }
  }
}
