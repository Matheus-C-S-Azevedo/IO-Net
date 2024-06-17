#include <ModbusIP_ESP8266.h>
#include <WiFi.h> // Importe a biblioteca correta para o ESP8266

// Definição dos pinos do relé e do botão
#define PINO_RELE 25
#define PINO_BOTAO 35

// Definição dos registradores Modbus
#define REGISTRO_RELE 0x0001
#define REGISTRO_BOTAO 0x0002

// Credenciais da rede Wi-Fi
const char* ssid = "HUB UEA NOVO";
const char* password = "hub010203";

// Criação de uma instância do servidor Modbus
ModbusIP mb;

void setup() {
  Serial.begin(115200);
  // Inicialização do pino do relé como saída
  pinMode(PINO_RELE, OUTPUT);
  // Inicialização do pino do botão como entrada
  pinMode(PINO_BOTAO, INPUT_PULLUP);

  // Conexão à rede Wi-Fi
  Serial.println("trying to connect to wifi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando à rede Wi-Fi...");
  }
  Serial.println("Conectado à rede Wi-Fi!");
  Serial.println("Enderço de IP:");
  Serial.println(WiFi.localIP());


  // Inicia o servidor Modbus com endereço IP e porta
  mb.server();

  // Adiciona os registradores Modbus
  mb.addCoil(REGISTRO_RELE); // registrador para controlar o relé
  mb.addCoil(REGISTRO_BOTAO); // registrador para ler o estado do botão
}

void loop() {
  // Lê o estado do botão
  bool estadoBotao = digitalRead(PINO_BOTAO);

  // Atualiza o valor do registrador Modbus para refletir o estado do botão
  mb.Coil(REGISTRO_BOTAO, estadoBotao);

  // Lê o valor do registrador Modbus para controlar o relé
  bool estadoRele = mb.Coil(REGISTRO_RELE);

  // Atualiza o estado do relé de acordo com o valor do registrador Modbus
  digitalWrite(PINO_RELE, estadoRele);

  // Manipulação do servidor Modbus
  mb.task();
  delay(100);
}
