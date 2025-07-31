#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// === Pino do DS18B20 ===
#define ONE_WIRE_BUS_COOLING 2  // Pino para DS18B20

// === Instância do sensor de arrefecimento ===
OneWire oneWireCooling(ONE_WIRE_BUS_COOLING);
DallasTemperature sensorCooling(&oneWireCooling);

// === Controle de tempo ===
unsigned long ultimaLeituraArrefecimento = 0;
const unsigned long intervaloArrefecimento = 30000;     // 30 segundos

void setup() {
  Serial.begin(115200);  // Inicializa a comunicação Serial para monitoramento

  // Inicializa o sensor de temperatura do arrefecimento
  sensorCooling.begin();
  sensorCooling.setResolution(9);  // Resolução mais rápida (~93 ms)
  Serial.println("✅ DS18B20 OK.");
}

void loop() {
  unsigned long agora = millis();

  // === Atualização da temperatura do arrefecimento a cada 30 segundos ===
  if (agora - ultimaLeituraArrefecimento >= intervaloArrefecimento) {
    ultimaLeituraArrefecimento = agora;

    // Lê a temperatura do sensor de arrefecimento
    sensorCooling.requestTemperatures();
    float tempArrefecimento = sensorCooling.getTempCByIndex(0);

    // Verifica se a leitura foi válida
    if (tempArrefecimento == DEVICE_DISCONNECTED_C || isnan(tempArrefecimento)) {
      Serial.println("⚠️ Erro: DS18B20 desconectado ou leitura inválida!");
    } else {
      // Se leitura for válida, imprime a temperatura
      Serial.print("Temperatura do arrefecimento: ");
      Serial.print(tempArrefecimento);
      Serial.println(" °C");
    }
  }
}
