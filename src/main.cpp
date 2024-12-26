#include <Arduino.h>

void setup()
{
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.println("test");
}

void loop()
{
  // put your main code here, to run repeatedly:
  digitalWrite(LED_BUILTIN, 1);
  delay(1000);

  digitalWrite(LED_BUILTIN, 0);
  delay(1000);

  Serial.println("test");
}
