#define BLYNK_TEMPLATE_ID           "TMPLuuaTZt1y"
#define BLYNK_TEMPLATE_NAME         "Quickstart Device"
#define BLYNK_AUTH_TOKEN            "v1KFQI05STIyvhrHBnLG8xLlYESUxdco"
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char ssid[] = "SIR";
char pass[] = "23112idw71";

#include <DHT.h>

#define ENA 5                              // Digital pin D1
#define IN1 4                             // Digital pin D2
#define IN2 0                            // Digital pin D3
#define LED 12                          // Digital pin D6
#define Motion 13                      // Digital pin D7
#define DHTPIN 2                      // Digital pin D4
#define DHTTYPE DHT11                // Digital pin connected to DHT11
DHT dht(DHTPIN, DHTTYPE);           // Initialize dht type as DHT 11
int val, reg;


void setup()
{
  Serial.begin(115200);
  dht.begin();
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);//Blynk

  pinMode(Motion, INPUT);   // declare sensor as input
  pinMode(LED, OUTPUT);    // declare LED as output
  digitalWrite (LED, LOW);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
}

BLYNK_WRITE(V0)
{
  val = param.asInt();
  regulate();
}

BLYNK_WRITE(V1)
{
  reg = param.asInt();
  regulate();
}


void regulate()
{
  Blynk.virtualWrite(V5, reg/200);
  if (val == 0 && reg > 199)
  {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, reg);
  }

  else
  {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
  }
}


void loop()
{
  Blynk.run();
  long state = digitalRead(Motion);                              //detect movement by 0 or 1 
  float h = dht.readHumidity();                                 // Read Humidity
  float t = dht.readTemperature();                             // Read temperature

  if (isnan(h) || isnan(t))                                  // Checking sensor working
  {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  else
  {
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print("%  Temperature: ");
    Serial.print(t);
    Serial.println("°C ");
    Blynk.virtualWrite(V2, t);
    Blynk.virtualWrite(V4, h);

    if (val == 1)
    {
      if (state == 1 && t > 25.00 && t < 28.00)
      {
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        analogWrite(ENA, 400);
      }

      else if (state == 1 && t > 27.00 && t < 32.00)
      {
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        analogWrite(ENA, 700);
      }

      else if (state == 1 && t > 31.00 && t < 35.00)
      {
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        analogWrite(ENA, 1000);
      }

      else if (state == 1 && t > 34.00)
      {
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        analogWrite(ENA, 1400);
        digitalWrite(LED, HIGH);
        delay(5000);
        digitalWrite(LED, LOW);
      }

      else
      {
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
      }
    }
  }
}
