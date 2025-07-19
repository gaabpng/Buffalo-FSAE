#include <Arduino.h>

// Código para receber dados do Arduino Mega via UART e armazenar em variáveis
// ESP32 LoRa receptor

// Variáveis para armazenar as temperaturas recebidas
float tempIR = 0.0;
float tempOleo = 0.0;
float tempAmbiente = 0.0;
float tempObjeto = 0.0;

// Buffer para leitura serial
String serialBuffer = "";
bool dataReady = false;

void setup() {
  Serial.begin(115200);   // Monitor serial para debug
  Serial2.begin(115200);  // UART para comunicação com Arduino Mega
  
  Serial.println("ESP32 pronto para receber dados do Arduino Mega");
}

void loop() {
  // Verifica se há dados disponíveis na Serial2 (UART)
  if (Serial2.available()) {
    char receivedChar = Serial2.read();
    
    if (receivedChar == '\n') {
      // Fim da linha - processa o dado recebido
      processarDado(serialBuffer);
      serialBuffer = "";  // Limpa o buffer
    } else {
      // Adiciona caractere ao buffer
      serialBuffer += receivedChar;
    }
  }
  
  // Se dados estão prontos, exibe as variáveis
  if (dataReady) {
    exibirDados();
    dataReady = false;
  }
  
  delay(10);
}

void processarDado(String dados) {
  // Remove espaços em branco
  dados.trim();
  
  // Processa cada tipo de temperatura baseado no prefixo
  if (dados.startsWith("Temp IR: ")) {
    String valor = dados.substring(9);  // Remove "Temp IR: "
    valor.replace(" °C", "");  // Remove " °C"
    tempIR = valor.toFloat();
    
  } else if (dados.startsWith("Temp Óleo: ")) {
    String valor = dados.substring(12);  // Remove "Temp Óleo: "
    valor.replace(" °C", "");  // Remove " °C"
    tempOleo = valor.toFloat();
    dataReady = true;  // Marca que um ciclo completo foi recebido
    
  } else if (dados.startsWith("Temp Ambiente: ")) {
    String valor = dados.substring(15);  // Remove "Temp Ambiente: "
    valor.replace(" °C", "");  // Remove " °C"
    tempAmbiente = valor.toFloat();
    
  } else if (dados.startsWith("Temp Objeto: ")) {
    String valor = dados.substring(13);  // Remove "Temp Objeto: "
    valor.replace(" °C", "");  // Remove " °C"
    tempObjeto = valor.toFloat();
  }
}

void exibirDados() {
  Serial.println("=== Dados Recebidos ===");
  Serial.print("Temperatura IR: ");
  Serial.print(tempIR);
  Serial.println(" °C");
  
  Serial.print("Temperatura Óleo: ");
  Serial.print(tempOleo);
  Serial.println(" °C");
  
  Serial.print("Temperatura Ambiente: ");
  Serial.print(tempAmbiente);
  Serial.println(" °C");
  
  Serial.print("Temperatura Objeto: ");
  Serial.print(tempObjeto);
  Serial.println(" °C");
  
  Serial.println("=======================");
  Serial.println();
}