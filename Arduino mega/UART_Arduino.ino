#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// === Pinos ===
#define ONE_WIRE_BUS_COOLING 2  // Pino para DS18B20

// === Sensores ===
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
OneWire oneWireCooling(ONE_WIRE_BUS_COOLING);
DallasTemperature sensorCooling(&oneWireCooling);

// === Controle de tempo ===
unsigned long ultimaLeituraObj = 0;
unsigned long ultimaLeituraArrefecimento = 0;

const unsigned long intervaloIR = 50;      // 50 ms
const unsigned long intervaloArrefecimento = 30000;     // 30 segundos

// === Temperatura atual do arrefecimento ===
float tempArrefecimento = 0.0;

// === Funções de leitura segura ===
float lerTempIRSegura() {
  float temp_IR = mlx.readObjectTempC();
  
  // Verificando se a leitura foi válida
  if (isnan(temp_IR)) {
    Serial.println("⚠️ Erro: leitura MLX90614 inválida!");
    return -1000;  // valor inválido para detectar erro
  }

  return temp_IR;
}

float lerTempArrefecimentoSegura() {
  sensorCooling.requestTemperatures();
  float temp_cooling = sensorCooling.getTempCByIndex(0);

  // Verificando se a leitura foi válida
  if (temp_cooling == DEVICE_DISCONNECTED_C || isnan(temp_cooling)) {
    Serial.println("⚠️ Erro: DS18B20 desconectado ou leitura inválida!");
    return -1000;  // valor inválido para detectar erro
  }

  return temp_cooling;
}

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200); // UART
  
  // Inicializa o sensor de temperatura IR
  if (!mlx.begin()) {
    Serial.println("❌ Erro ao inicializar o MLX90614. Verifique conexões.");
    while (true);  // trava o sistema
  }
  Serial.println("✅ MLX90614 OK.");
  
  // Inicializa o sensor de temperatura do arrefecimento
  sensorCooling.begin();
  sensorCooling.setResolution(9);  // Resolução mais rápida (~93 ms)
  Serial.println("✅ DS18B20 OK.");

  // Leitura inicial do arrefecimento
  tempArrefecimento = lerTempArrefecimentoSegura();
  ultimaLeituraArrefecimento = millis();
}

void loop() {
  unsigned long agora = millis();

  // === Envio da temperatura do IR a cada 50ms ===
  if (agora - ultimaLeituraObj >= intervaloIR) {
    ultimaLeituraObj = agora;

    float tempIR = lerTempIRSegura();

    // Se leitura for válida, envia
    if (tempIR > -999) {
      Serial1.print("Arrefecimento:");
      Serial1.print(tempArrefecimento);
      Serial1.print(";OBJ:");
      Serial1.print(tempIR);
      Serial1.println(";");
    }
  }

  // === Atualização da temperatura do arrefecimento a cada 30 segundos ===
  if (agora - ultimaLeituraArrefecimento >= intervaloArrefecimento) {
    ultimaLeituraArrefecimento = agora;

    float novaLeitura = lerTempArrefecimentoSegura();
    if (novaLeitura > -999) {
      tempArrefecimento = novaLeitura;
    }
  }
}
