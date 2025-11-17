#include <WiFi.h>
#include <PubSubClient.h>   // Biblioteca MQTT
// ------------------------------
// DEFINIÇÕES DE PINOS
// ------------------------------
#define LDR_PIN 34
#define RED 14
#define YELLOW 27
#define GREEN 26
// ------------------------------
// CLASSE TRAFFICLIGHT
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
// CLASSE LDR
// ------------------------------
class LightSensor {
private:
    int pin, threshold;
public:
    LightSensor(int p, int th = 2000) {
        pin = p;
        threshold = th;
    }
    String getMode() {
        int v = analogRead(pin);
        Serial.println(v);
        return (v < threshold) ? "NIGHT" : "DAY";
    }
};
// ==================================================================
// AGORA VEM A PARTE REFEITA → COMUNICAÇÃO MQTT (broker público)
// ==================================================================
// WiFi
const char *ssid = "Inteli.Iot";
const char *password = "%(Yk(sxGMtvFEs.3";
// Broker MQTT público (SEM senha)
const char* mqtt_server = "broker.hivemq.com";
const int   mqtt_port   = 1883;
// Criando cliente WiFi e cliente MQTT
WiFiClient espClient;
PubSubClient client(espClient);
// ------------------------------
// Função para reconectar ao MQTT
// ------------------------------
void reconnectMQTT() {
    while (!client.connected()) {
        Serial.print("Conectando ao MQTT... ");
        // Criar ID único
        String clientId = "ESP32-Semaforo-" + String(random(0xffff), HEX);
        if (client.connect(clientId.c_str())) {
            Serial.println("Conectado!");
        } else {
            Serial.print("Falhou. Erro: ");
            Serial.println(client.state());
            delay(2000);
        }
    }
}
// ------------------------------
// INSTÂNCIAS DOS OBJETOS
// ------------------------------
TrafficLight semaforo(RED, YELLOW, GREEN);
LightSensor ldr(LDR_PIN);
// ------------------------------
// SETUP
// ------------------------------
void setup() {
    Serial.begin(115200);
    // ---- Conectar ao WiFi ----
    WiFi.begin(ssid, password);
    Serial.print("Conectando ao WiFi...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(300);
        Serial.print(".");
    }
    Serial.println(" Conectado!");
    // ---- Configurar MQTT ----
    client.setServer(mqtt_server, mqtt_port);
}
// ------------------------------
// LOOP
// ------------------------------
void loop() {
    // Reconnect MQTT se cair
    if (!client.connected()) {
        reconnectMQTT();
    }
    client.loop();
    // Lê o modo (DAY/NIGHT)
    String mode = ldr.getMode();
    int ldrValue = analogRead(LDR_PIN);
    client.publish("semaforo/ldr", String(ldrValue).c_str());
    // Envia para o broker público
    client.publish("semaforo/mode", mode.c_str());
    // Aplica o ciclo do semáforo
    semaforo.runCycle(mode);
}