#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

#define timeSeconds 3

const char* SSID = "";
const char* PASSWORD = "";

#define BOT_TOKEN ""
#define CHAT_ID ""

WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);

int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

const int led = 2;
const int motionSensor = 27;

unsigned long now = millis();
unsigned long lastTrigger = 0;
boolean startTimer = false;

void IRAM_ATTR detectsMovement() {
  Serial.println("Movimentação detectada!");
  digitalWrite(led, HIGH);
  startTimer = true;
  lastTrigger = millis();
}

void setup() {
  Serial.begin(115200);
  
  pinMode(motionSensor, INPUT_PULLUP);
  
  attachInterrupt(digitalPinToInterrupt(motionSensor), detectsMovement, RISING);

  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);

  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  
  Serial.println(WiFi.localIP());
}

void loop() {
  now = millis();
  
  if (startTimer && (now - lastTrigger > (timeSeconds * 1000))) {
    bot.sendMessage(CHAT_ID, "Alguém entrou na sua casa", "");
    Serial.println("Movimentação parada.");
    digitalWrite(led, LOW);
    startTimer = false;
  }
}
