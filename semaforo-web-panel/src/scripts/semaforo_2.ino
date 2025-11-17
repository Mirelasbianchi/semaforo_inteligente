#include <WiFi.h>
#include <PubSubClient.h>
// ------------------------------
// CONFIGURAÇÕES DE REDE E MQTT
// ------------------------------
const char *ssid = "Inteli.Iot";
const char *password = "%(Yk(sxGMtvFEs.3";
const char* mqtt_server = "broker.hivemq.com";
const int   mqtt_port   = 1883;
const char* mqtt_topic_subscribe = "semaforo/mode";
// --- Clientes de Rede ---
WiFiClient espClient;
PubSubClient client(espClient);
// --- Variável Global de Modo (CORRIGIDO) ---
// Usar um tipo simples (bool) com volatile, em vez de um objeto String
volatile bool isDayMode = true; // true = DAY, false = NIGHT
// ------------------------------
// DEFINIÇÕES DE PINOS
// ------------------------------
#define RED 14
#define YELLOW 27
#define GREEN 26
// ------------------------------
// CLASSE TRAFFICLIGHT (Sem alterações)
// ------------------------------
class TrafficLight {
private:
    int pinRed, pinYellow, pinGreen;
    int tRedDay = 6, tYellow = 2, tGreenDay = 4;
    int tRedNight = 4, tGreenNight = 6;
public:
    TrafficLight(int r, int y, int g) {
        pinRed = r;
        pinYellow = y;
        pinGreen = g;
        pinMode(pinRed, OUTPUT);
        pinMode(pinYellow, OUTPUT);
        pinMode(pinGreen, OUTPUT);
    }
    void setLights(bool r, bool y, bool g) {
        digitalWrite(pinRed,   r);
        digitalWrite(pinYellow,y);
        digitalWrite(pinGreen, g);
    }
    void runCycle(String mode) {
        int tR = (mode == "DAY") ? tRedDay   : tRedNight;
        int tG = (mode == "DAY") ? tGreenDay : tGreenNight;
        setLights(1,0,0); delay(tR * 1000);
        setLights(0,1,0); delay(tYellow * 1000);
        setLights(0,0,1); delay(tG * 1000);
    }
};
// ------------------------------
// INSTÂNCIA DO SEMÁFORO
// ------------------------------
TrafficLight semaforo(RED, YELLOW, GREEN);
// ----------------------------------------------------
// FUNÇÕES AUXILIARES (WiFi e MQTT)
// ----------------------------------------------------
/**
 * @brief Função de Callback do MQTT (CORRIGIDO)
 * Atualiza a variável booleana volátil.
 */
void mqtt_callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Mensagem recebida no tópico [");
    Serial.print(topic);
    Serial.print("]: ");
    String message;
    for (int i = 0; i < length; i++) {
        message += (char)payload[i];
    }
    Serial.println(message);
    // Atualiza a flag booleana volátil
    if (message == "DAY") {
        isDayMode = true;
        Serial.println("Modo atualizado para: DAY");
    } else if (message == "NIGHT") {
        isDayMode = false;
        Serial.println("Modo atualizado para: NIGHT");
    } else {
        Serial.println("Mensagem MQTT ignorada.");
    }
}
/**
 * @brief Conecta ao WiFi
 */
void setup_wifi() {
    delay(10);
    Serial.println();
    Serial.print("Conectando-se a ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi conectado!");
    Serial.print("Endereço IP: ");
    Serial.println(WiFi.localIP());
}
/**
 * @brief Reconecta ao broker MQTT (se a conexão cair)
 */
void reconnect_mqtt() {
    while (!client.connected()) {
        Serial.print("Tentando conexão MQTT...");
        String clientId = "ESP32-Semaforo-SLAVE-" + String(random(0xffff), HEX);
        if (client.connect(clientId.c_str())) {
            Serial.println("Conectado!");
            client.subscribe(mqtt_topic_subscribe);
            Serial.println("Inscrito no tópico: " + String(mqtt_topic_subscribe));
        } else {
            Serial.print("Falha, rc=");
            Serial.print(client.state());
            Serial.println(" Tentando novamente em 5 segundos");
            delay(5000);
        }
    }
}
// ------------------------------
// SETUP
// ------------------------------
void setup() {
    Serial.begin(115200);
    setup_wifi();
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(mqtt_callback);
}
// ------------------------------
// LOOP (CORRIGIDO)
// ------------------------------
void loop() {
    if (!client.connected()) {
        reconnect_mqtt();
    }
    client.loop();
    // --- Lógica de leitura segura ---
    // 1. Copia o valor volátil para uma variável local.
    // Esta é uma "leitura atômica" segura.
    bool currentModeIsDay = isDayMode;
    // 2. Converte o booleano local para a String que a função espera.
    String modeStringToSend;
    if (currentModeIsDay) {
        modeStringToSend = "DAY";
    } else {
        modeStringToSend = "NIGHT";
    }
    // 3. Passa a String local (não-volátil) para a função.
    semaforo.runCycle(modeStringToSend);
}














