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

const char *week_char[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
const char *time_zone = "JST-9";

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

  // Wi-Fi
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.printf("Configure Wi-Fi\n");
  display.setCursor(0, 15);
  display.printf("  SSID:\n     %s\n", CONFIGURE_WIFI_SSID);
  display.setCursor(0, 35);
  display.printf("  PASS:\n     %s\n", CONFIGURE_WIFI_PASS);

  display.display();

  wm.autoConnect();

  setenv("TZ", time_zone, 1);
  tzset();

  // NTP.begin("ntp.nict.jp", "time.google.com");
  // NTP.waitSet();

  // configTzTime(time_zone, "ntp.nict.jp");
  bool set_clock_result = setClock();

  display.printf("set_clock_result=%d\n", set_clock_result);

  delay(1500);
}

void loop()
{
  static time_t now;
  static struct tm *timeinfo;
  time(&now);
  timeinfo = localtime(&now);

  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(4, 0);
  display.printf("%04d/%02d/%02d\n  %02d:%02d:%02d\n",
                 timeinfo->tm_year + 1900,
                 timeinfo->tm_mon + 1,
                 timeinfo->tm_mday,
                 timeinfo->tm_hour,
                 timeinfo->tm_min,
                 timeinfo->tm_sec,
                 timeinfo->tm_wday,
                 week_char[timeinfo->tm_wday]);

  digitalWrite(LED_BUILTIN, LOW);
  delay(950);

  digitalWrite(LED_BUILTIN, HIGH);
  delay(50);

  Serial.println("test");

  display.display();
}
