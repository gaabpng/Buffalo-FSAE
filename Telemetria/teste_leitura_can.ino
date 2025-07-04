#include <SPI.h>
#include <mcp_can.h>

// SPI alternativo
#define CAN_CS    13
#define SPI_MISO  12
#define SPI_MOSI  15
#define SPI_SCK   14

MCP_CAN CAN(CAN_CS);

void setup() {
  Serial.begin(115200);

  // Inicia SPI com pinos definidos
  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI, CAN_CS);

  Serial.println("Inicializando MCP2515...");
  while (CAN_OK != CAN.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ)) {
    Serial.println("Falha ao iniciar o módulo CAN. Tentando novamente...");
    delay(1000);
  }

  CAN.setMode(MCP_NORMAL);
  Serial.println("CAN inicializado com sucesso!");
}

void loop() {
  Serial.println("Inicio do loop");

  if (CAN_MSGAVAIL == CAN.checkReceive()) {
    long unsigned int canId = 0;
    byte len = 0;
    byte buf[8];

    CAN.readMsgBuf(&canId, &len, buf);

    Serial.print("ID: 0x");
    Serial.print(canId, HEX);
    Serial.print(" DLC: ");
    Serial.print(len);
    Serial.print(" Data: ");
    for (int i = 0; i < len; i++) {
      Serial.printf("%02X ", buf[i]);
    }
    Serial.println();
  }
}