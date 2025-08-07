#include <Arduino.h>
#include <sys/time.h>
#include <WiFi.h>
#include <WiFiManager.h>
#include <Ticker.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <qrcode.h>
// #include <TFT_eSPI.h>
// #include <SPI.h>

#include "main.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

#define SEGA 18
#define SEGB 19
#define SEGC 20
#define SEGD 21

#define PIN_DISPLAY_WIRE_SDA 16
#define PIN_DISPLAY_WIRE_SCL 17
#define I2C_DISPLAY_ADDRESS 0x3C

#define CONFIGURE_WIFI_SSID "configure_me"
#define CONFIGURE_WIFI_PASS "H3deysMf"
#define WIFI_QR "WIFI:S:configure_me;T:WPA;P:H3deysMf;;"
#define TIME_OFFSET_JST 3600 * 9

#define ST7789_DRIVER

#define TFT_RGB_ORDER TFT_RGB

#define TFT_WIDTH 240
#define TFT_HEIGHT 240

#define TFT_SCLK 4
#define TFT_MISO 5
#define TFT_MOSI 6
#define TFT_CS 7

#define BUZZER_PIN 0

#define SPI_FREQUENCY 27000000

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// WiFiManager wm(CONFIGURE_WIFI_SSID, CONFIGURE_WIFI_PASS);

const char *month_char_short[12] = {"Jan.", "Feb.", "Mar.", "Apr.", "May.", "Jun.", "Jul.", "Aug.", "Sep.", "Oct.", "Nov.", "Dec."};
const char *month_char_long[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
const char *week_char_short[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
const char *week_char_long[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
const char *time_zone = "JST-9";

Ticker display_seconds_tk;
// TFT_eSPI tft = TFT_eSPI();
void setup()
{
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(SEGA, OUTPUT);
  pinMode(SEGB, OUTPUT);
  pinMode(SEGC, OUTPUT);
  pinMode(SEGD, OUTPUT);

  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  digitalWrite(SEGA, LOW);
  digitalWrite(SEGB, LOW);
  digitalWrite(SEGC, LOW);
  digitalWrite(SEGD, LOW);

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

  // Wi-Fi
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(2, 2);
  display.printf("Please set up Wi-Fi\n");
  display.setCursor(50, 15);
  display.printf("SSID:");
  display.setCursor(50, 25);
  display.printf("%s", CONFIGURE_WIFI_SSID);
  display.setCursor(50, 36);
  display.printf("PASS:");
  display.setCursor(50, 46);
  display.printf("%s", CONFIGURE_WIFI_PASS);
  display_qrcode(WIFI_QR, 10, 17);
  display.display();

  display.clearDisplay();
  display.display();
}

void loop()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  time_t now = tv.tv_sec;
  suseconds_t micros = tv.tv_usec;
  struct tm *timeinfo;

  timeinfo = localtime(&now);

  display.clearDisplay();

  int x_base = 4;
  int y_base = 6;
  display.setCursor(x_base + 0, y_base + 0);
  display.setTextSize(2);
  display.printf("%04d.%02d.%02d",
                 timeinfo->tm_year + 1900,
                 timeinfo->tm_mon + 1,
                 timeinfo->tm_mday);

  x_base = 4;
  y_base = 26;
  display.setCursor(x_base + 0, y_base + 0);
  display.setTextSize(1);
  display.printf("%s, %s, %s", week_char_short[timeinfo->tm_wday], month_char_long[timeinfo->tm_mon], get_era(timeinfo).c_str());

  x_base = 1;
  y_base = 40;
  display.setCursor(x_base + 0, y_base);
  display.setTextSize(3);
  display.printf("%2d", timeinfo->tm_hour);

  display.setCursor(x_base + 34, y_base + 4);
  display.setTextSize(2);
  display.printf(":");

  display.setCursor(x_base + 43, y_base);
  display.setTextSize(3);
  display.printf("%02d", timeinfo->tm_min);

  display.setCursor(x_base + 76, y_base + 7);
  display.setTextSize(2);
  display.printf(":");

  display.setCursor(x_base + 86, y_base + 7);
  display.setTextSize(2);
  display.printf("%02d", timeinfo->tm_sec);

  display.setCursor(x_base + 109, y_base + 14);
  display.setTextSize(1);
  display.printf(".%01ld", micros / 1000 / 100);

  disp_7seg(timeinfo->tm_sec);

  if (WiFi.status() != WL_CONNECTED)
  {
    // display_qrcode(WIFI_QR, 10, 25);
    // display.setCursor(50, 25);
    // display.printf("not connect\n");
  }

  display.display();
}

void disp_7seg(int num)
{
  digitalWrite(SEGA, (((num & 0x01) != 0) ? HIGH : LOW));
  digitalWrite(SEGB, (((num & 0x02) != 0) ? HIGH : LOW));
  digitalWrite(SEGC, (((num & 0x04) != 0) ? HIGH : LOW));
  digitalWrite(SEGD, (((num & 0x08) != 0) ? HIGH : LOW));
}

void setup1()
{
  init_tks();

  WiFi.begin("gn-labo", "Aj2p3fQ4");
  setenv("TZ", time_zone, 1);
  tzset();

  configTime(3600, TIME_OFFSET_JST, "3.jp.pool.ntp.org", "1.jp.pool.ntp.org");

  bool set_clock_result = set_clock();
  if (set_clock_result)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("Clock set successfully.");
  }
  else
  {
    Serial.println("Failed to set clock.");
  }
}

void loop1()
{

  struct timeval tv;
  gettimeofday(&tv, NULL);
  time_t now = tv.tv_sec;
  suseconds_t micros = tv.tv_usec;
  struct tm *timeinfo;

  static bool last_sec_state_state = false;

  if (micros / 1000 < 20)
  {
    if (!last_sec_state_state)
    {
      digitalWrite(BUZZER_PIN, HIGH);
      last_sec_state_state = true;
    }
  }
  else
  {
    digitalWrite(BUZZER_PIN, LOW);
    last_sec_state_state = false;
  }
}

void init_tks()
{
  display_seconds_tk.attach_ms(1000, display_seconds);
}

void display_seconds()
{
  // digitalWrite(LED_BUILTIN, HIGH);
  // delay(5);
  // digitalWrite(LED_BUILTIN, LOW);
}

bool set_clock()
{
  NTP.begin("ntp.nict.jp", "time.google.com");
  return NTP.waitSet();
}

void display_qrcode(char *text, int offset_x, int offset_y)
{
  QRCode qrcode;
  uint8_t qrcodeData[qrcode_getBufferSize(4)];
  qrcode_initText(&qrcode, qrcodeData, 4, ECC_LOW, text);
  // int offset_x = 10;
  // int offset_y = 17;

  // display.clearDisplay();
  display.fillRect(0 + offset_x - 2, 0 + offset_y - 2, qrcode.size + 4, qrcode.size + 4, WHITE);

  for (uint8_t y = 0; y < qrcode.size; y++)
  {
    for (uint8_t x = 0; x < qrcode.size; x++)
    {
      if (qrcode_getModule(&qrcode, x, y))
      {
        display.drawPixel(x + offset_x, y + offset_y, BLACK);
      }
    }
  }
}

String get_era(const struct tm *timeinfo)
{
  int year = timeinfo->tm_year + 1900;

  if (year >= 2019)
  {
    return "R" + String(year - 2018); // 令和
  }
  else if (year >= 1989)
  {
    return "H" + String(year - 1988); // 平成
  }
  else if (year >= 1926)
  {
    return "S" + String(year - 1925); // 昭和（おまけ）
  }
  else
  {
    return "";
  }
}
