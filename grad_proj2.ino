// Define Libraries
#define BLYNK_PRINT Serial

#include "twilio.hpp"
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

BlynkTimer timer;

static const char *account_sid = "ACf5e953389430cd3af4167580bce21468";
static const char *auth_token = "929ea1bdb7fd07e24b3f3c8fcabd4d1e";
static const char *from_number = "+14847158658";
static const char *to_number = "+962777788895";
static const char *message = "Gas Leak Detected!";

char auth[] = "04CK3gAYb7fB1Gkh3GZpoYngbZeA9h2Q";
char ssid[] = "Umniah_2.4G_175C8D";
char pass[] = "Mazen_12345";

//define pins
#define MQ2 35
#define GREEN 22
#define RED 18
#define BEEP 23
#define FAN 19

int GasValue = 0;
int x = 0;
boolean state = false;
Twilio *twilio;

void setup()
{
  // Debug console
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  pinMode(MQ2, INPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(FAN, OUTPUT);
  pinMode(BEEP, OUTPUT);
  timer.setInterval(1000L, sendUptime);
}

void sendUptime()
{
  
  GasValue = analogRead(MQ2);
  Blynk.virtualWrite(V0, GasValue);
  //Serial.println(GasValue);  to print the gas measuerment
  twilio = new Twilio(account_sid, auth_token);

  if (GasValue > 500)
  {
    Blynk.logEvent("gas_alert",message);
    digitalWrite(GREEN, LOW);
    digitalWrite(BEEP, HIGH);
    digitalWrite(RED, HIGH);
    digitalWrite(FAN, HIGH);
    delay(1000);
    String response;
    if(x == 0){
      x = 1;
      bool success = twilio->send_message(to_number, from_number, message, response);
      if (success) {
        Serial.println("Sent message successfully!");
      } else {
        Serial.println(response);
      }
    }
  }

  else
  {
    x = 0;
    digitalWrite(GREEN, HIGH);
    digitalWrite(BEEP, LOW);
    digitalWrite(RED, LOW);
    digitalWrite(FAN, LOW);
  }
}

void loop()
{
  Blynk.run();
  timer.run();
}