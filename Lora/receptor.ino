
#include <LoRa.h>
#include <SPI.h>

/* Estrutura para dados de temperatura */
struct DadosTemperatura {
    float tempIR;
    float tempOleo;
    float tempAmbiente;
    float tempObjeto;
};

// Variáveis globais para armazenar os dados recebidos
float temperaturaIR = 0.0;
float temperaturaOleo = 0.0;
float temperaturaAmbiente = 0.0;
float temperaturaObjeto = 0.0;
unsigned long ultimaLeitura = 0;

/* Definições para comunicação com rádio LoRa */
#define SCK_LORA           5
#define MISO_LORA         19
#define MOSI_LORA         27
#define RESET_PIN_LORA    14
#define SS_PIN_LORA       18
#define LORA_DIO0         26  // Pino DIO0 (ajuste se necessário)

#define HIGH_GAIN_LORA    20   // dBm
#define BAND              915E6  // 915 MHz

/* Definições gerais */
#define DEBUG_SERIAL_BAUDRATE 115200

/* Função: inicia comunicação com chip LoRa */
bool init_comunicacao_lora(void)
{
    bool status_init = false;
    Serial.println("[LoRa Receiver] Tentando iniciar comunicacao com o radio LoRa...");
    
    SPI.begin(SCK_LORA, MISO_LORA, MOSI_LORA, SS_PIN_LORA);
    LoRa.setPins(SS_PIN_LORA, RESET_PIN_LORA, LORA_DIO0);
    
    if (!LoRa.begin(BAND)) {
        Serial.println("[LoRa Receiver] Comunicacao com o radio LoRa falhou. Nova tentativa em 1 segundo...");
        delay(1000);
        status_init = false;
    } else {
        LoRa.setTxPower(HIGH_GAIN_LORA);
        Serial.println("[LoRa Receiver] Comunicacao com o radio LoRa OK!");
        status_init = true;
    }

    return status_init;
}

/* Setup */
void setup() 
{
    Serial.begin(DEBUG_SERIAL_BAUDRATE);
    while (!Serial);

    // Inicializa comunicação com LoRa até conseguir
    while (!init_comunicacao_lora());
}

/* Loop principal */
void loop() 
{
    int packet_size = LoRa.parsePacket();
    
    if (packet_size == sizeof(long)) 
    {
        long informacao_recebida = 0;
        uint8_t buffer[sizeof(long)];
        int i = 0;

        while (LoRa.available() && i < sizeof(long)) {
            buffer[i++] = (uint8_t)LoRa.read();
        }

        if (i == sizeof(long)) {
            memcpy(&informacao_recebida, buffer, sizeof(long));
            Serial.print("[LoRa Receiver] Valor recebido: ");
            Serial.println(informacao_recebida);
        } else {
            Serial.println("[LoRa Receiver] Pacote recebido com tamanho incorreto.");
        }
    }

    delay(100);
}
