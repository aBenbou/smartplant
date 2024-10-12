#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>


// Définition des pins
#define PHOTO_PIN A4          // Pin analogique pour le photoresistor
#define DHTPIN 9              // Pin numérique pour le DHT11
#define DHTTYPE DHT11         // Type de capteur DHT
#define RELAY_PIN A5          // Pin analogique pour le relais
#define LED_PIN 11            // Pin numérique pour la LED
#define RX_PIN 2              // Pin RX pour la communication Bluetooth
#define TX_PIN 3              // Pin TX pour la communication Bluetooth

DHT_Unified dht(DHTPIN, DHTTYPE);
SoftwareSerial btSerial(RX_PIN, TX_PIN);

// Variables pour la communication Bluetooth
String btReceivedData;
#define MAX_BT_RECEIVE_DATA 16
int btSendFreq = 1000;
int lastTimeBTSend = 0;

// Variables des capteurs
int photo_value;             // Valeur du photoresistor
float humidity, temperature; // Valeurs de l'humidité et de la température

void setup() {
  Serial.begin(9600);
  btSerial.begin(9600);

  dht.begin();
  pinMode(PHOTO_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(RELAY_PIN, LOW); // Initialisation du relais à l'état bas
}

void loop() {
  int currentTime = millis();

  // Lecture du capteur DHT11
  sensors_event_t event;  
  dht.temperature().getEvent(&event);
  if (!isnan(event.temperature)) {
    temperature = event.temperature;
  }
  dht.humidity().getEvent(&event);
  if (!isnan(event.relative_humidity)) {
    humidity = event.relative_humidity;
  }

  // Lecture du photoresistor
  photo_value = analogRead(PHOTO_PIN);

  // Envoi des données via Bluetooth
  if(currentTime - lastTimeBTSend >= btSendFreq) {
    String telemetry = "Humidity:" + String(humidity) +
                       ",Temp:" + String(temperature) +
                       ",Light:" + String(photo_value);
    Serial.println(telemetry);
    lastTimeBTSend = currentTime;
  }

  // Réception des commandes Bluetooth
  while(btSerial.available()) {
    char c = btSerial.read();
    if (c == '\n' || btReceivedData.length() == MAX_BT_RECEIVE_DATA) {
      // Traiter les commandes reçues ici
      Serial.println("Received from BT: " + btReceivedData);

      // Commandes pour le relais ou la LED
      if (btReceivedData == "relayON") {
        digitalWrite(RELAY_PIN, HIGH);
      } else if (btReceivedData == "relayOFF") {
        digitalWrite(RELAY_PIN, LOW);
      } else if (btReceivedData == "ledON") {
        digitalWrite(LED_PIN, HIGH);
      } else if (btReceivedData == "ledOFF") {
        digitalWrite(LED_PIN, LOW);
      }

      // Réinitialiser les données reçues
      btReceivedData = "";
    } else if (c != '\r') {
      btReceivedData += c;
    }
  }

  delay(100);
}