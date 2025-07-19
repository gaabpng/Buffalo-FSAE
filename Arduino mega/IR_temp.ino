#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Configuração do sensor MLX90614
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

// O fio de dados é conectado no pino digital 4 no Arduino
#define ONE_WIRE_BUS 4
// Prepara uma instância oneWire para comunicar com qualquer outro dispositivo oneWire
OneWire oneWire(ONE_WIRE_BUS); 
// Passa uma referência oneWire para a biblioteca DallasTemperature
DallasTemperature sensors(&oneWire);

void setup() {
  // Inicializa a comunicação serial
  Serial.begin(115200);

  // Inicializa o sensor MLX90614
  if (!mlx.begin()) {
    Serial.println("Erro ao detectar o sensor MLX90614. Verifique as conexões.");
    while (1);
  }
  Serial.println("Sensor IR iniciado");
  
  // Inicializa o sensor DS18B20
  sensors.begin();
  Serial.println("Sensor DS18B20 iniciado");
}

void loop() {
  // Lê as temperaturas do sensor
  float tempAmbiente = mlx.readAmbientTempC();
  float tempObjeto = mlx.readObjectTempC();
 
  // Manda comando para ler temperaturas
  sensors.requestTemperatures();

  // Mostra os dados no Serial

  Serial.print("\tAmbiente: ");
  Serial.print(tempAmbiente);
  Serial.print(" °C\tObjeto: ");
  Serial.print(tempObjeto);
  Serial.print(" °C\tArrefecimento: ");
  Serial.print(sensors.getTempCByIndex(0));
  Serial.println(" °C");

  delay(1000); // Aguarda 1 segundo
}