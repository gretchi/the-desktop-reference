#include <Arduino.h>

#include <WiFi.h>

#include "WiFiManager.h"

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

#define PIN_DISPLAY_WIRE_SDA 16
#define PIN_DISPLAY_WIRE_SCL 17
#define I2C_DISPLAY_ADDRESS 0x3C

#define CONFIGURE_WIFI_SSID "Connect Me!"
#define CONFIGURE_WIFI_PASS "password123!" // ToDo: MACアドレスか何か固有値を使用

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

WiFiManager wm(CONFIGURE_WIFI_SSID, CONFIGURE_WIFI_PASS);

bool setClock()
{
  NTP.begin("ntp.nict.jp", "time.google.com");
  return NTP.waitSet();
}

void setup()
{
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);

  Wire.setSDA(PIN_DISPLAY_WIRE_SDA);
  Wire.setSCL(PIN_DISPLAY_WIRE_SCL);
  Wire.begin();

  display.begin(SSD1306_SWITCHCAPVCC, I2C_DISPLAY_ADDRESS);
  display.setTextColor(SSD1306_WHITE);

  display.clearDisplay();
  display.display();

  // ディスプレイの動作確認用
  display.fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
  display.display();
  delay(1500);

  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.printf(" SSID: %s\n", CONFIGURE_WIFI_SSID);
  display.printf(" PASS: %s\n", CONFIGURE_WIFI_PASS);

  display.display();

  wm.autoConnect();
}

void loop()
{
  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(4, 0);
  display.println("Person:");
  display.setCursor(4, 10);
  display.println("Detection");

  digitalWrite(LED_BUILTIN, LOW);
  delay(950);

  digitalWrite(LED_BUILTIN, HIGH);
  delay(50);

  Serial.println("test");

  display.display();
}
