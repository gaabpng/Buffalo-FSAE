// Leitura do sensor de óleo (DS18B20) e temperatura do objeto (MLX90614)
// Envio dos dados via UART (Serial1)

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// === Configuração dos pinos ===
#define ONE_WIRE_BUS_OIL 2  // Pino do sensor de óleo (DS18B20)

// === Instanciando sensores ===
Adafruit_MLX90614 mlx = Adafruit_MLX90614();           // MLX90614 (I2C)
OneWire oneWireOil(ONE_WIRE_BUS_OIL);                  // DS18B20
DallasTemperature sensorOil(&oneWireOil);              

void setup() {
  Serial.begin(115200);     // Monitor Serial
  Serial1.begin(115200);    // UART para ESP32 ou outro dispositivo

  Serial.println("Iniciando sensores...");
  inicia_sensores();
  Serial.println("Sistema pronto!");
}

void loop() {
  // Leitura do MLX90614 (somente temperatura do objeto)
  float tempObjeto = mlx.readObjectTempC();

  // Leitura do DS18B20
  sensorOil.requestTemperatures();
  float tempOleo = sensorOil.getTempCByIndex(0);

  // Debug
  Serial.print("ÓLEO: "); Serial.print(tempOleo); Serial.println(" °C");
  Serial.print("OBJETO: "); Serial.print(tempObjeto); Serial.println(" °C");
  Serial.println();

  // UART (envio formatado)
  Serial1.print("OLEO:");
  Serial1.print(tempOleo);
  Serial1.print(";OBJ:");
  Serial1.print(tempObjeto);
  Serial1.println(";");

  delay(50); // Espera 1 segundo
}

void inicia_sensores() {
  if (!mlx.begin()) {
    Serial.println("Erro ao inicializar o MLX90614 (verifique I2C).");
    while (true);  // trava o programa
  }
  Serial.println("MLX90614 inicializado com sucesso.");

  sensorOil.begin();
  Serial.println("Sensor DS18B20 (óleo) inicializado.");
}
