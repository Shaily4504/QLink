#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <WiFiClientSecure.h>
#include <MFRC522.h>
#include <SPI.h>
#include <qrcode.h>  
#include "qrcode.h"

// OLED config
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
QRCode qrcode;

int scale = 2;
int offsetX = (SCREEN_WIDTH - qrcode.size * scale) / 2;
int offsetY = (SCREEN_HEIGHT - qrcode.size * scale) / 2;

// RFID pins
constexpr uint8_t RST_PIN = D3;
uint8_t qrcodeData[134];  // ✅ hardcoded size
constexpr uint8_t SS_PIN = D4;
MFRC522 rfid(SS_PIN, RST_PIN);

// WiFi credentials
const char* ssid = "moto";
const char* password = "tere9876";

// State
WiFiClientSecure client;
String lastUID = ""; // To prevent duplicate scans

String urlEncode(const char *msg) {
  const char *hex = "0123456789ABCDEF";
  String encoded = "";

  while (*msg != '\0') {
    if (('a' <= *msg && *msg <= 'z') ||
        ('A' <= *msg && *msg <= 'Z') ||
        ('0' <= *msg && *msg <= '9') ||
        *msg == '-' || *msg == '_' || *msg == '.' || *msg == '~') {
      encoded += *msg;
    } else {
      encoded += '%';
      encoded += hex[*msg >> 4];
      encoded += hex[*msg & 15];
    }
    msg++;
  }

  return encoded;
}

void setup() {
  Serial.begin(115200);
  SPI.begin();
  rfid.PCD_Init();

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Connecting WiFi...");
  display.display();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  client.setInsecure();

  display.clearDisplay();
  display.setCursor(17, 10);
  display.println("WiFi Connected!");
  display.display();
  delay(2000);

  display.clearDisplay();
  display.setCursor(17, 10);
  display.println("Welcome back!");
  display.display();
  delay(2000);

  display.clearDisplay();
  display.setCursor(17, 10);
  display.println("Scan your RFID");
  display.display();
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    return;
  }

  String scannedRFID = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    scannedRFID += String(rfid.uid.uidByte[i]);
  }

  Serial.print("Scanned UID: ");
  Serial.println(scannedRFID);

  if (WiFi.status() == WL_CONNECTED) {
    WiFiClientSecure client;
    client.setInsecure();  // ⚠️ Only for testing, don't use in production

    HTTPClient http;
    String url = "https://qrsend-backend.onrender.com/get-user-by-rfid/" + scannedRFID;

    http.begin(client, url);
    http.setTimeout(10000);  // Set timeout BEFORE sending the request

    int httpCode;
    int retries = 3;

    // Retry logic
    do {
      httpCode = http.GET();
      Serial.print("HTTP Code: ");
      Serial.println(httpCode);
      if (httpCode != -11) break;
      delay(500);
    } while (--retries > 0);

    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println(payload);

      // --- Parse 'label' ---
      int labelStart = payload.indexOf("\"label\":\"") + 9;
      int labelEnd = payload.indexOf("\"", labelStart);
      String label = payload.substring(labelStart, labelEnd);

      // --- Parse 'fileUrl' (inside 'pdf' object) ---
      int urlStart = payload.indexOf("\"smallpdfUrl\":\"") + 16;  // fixed offset
      int urlEnd = payload.indexOf("\"", urlStart);
      String fileUrl = payload.substring(urlStart, urlEnd);

      Serial.println("Label: " + label);
      Serial.println("File URL: " + fileUrl);

      display.clearDisplay();
      display.setCursor(0, 0);
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.println(label);

      generateAndDisplayQR(fileUrl);
      display.display();

    } else {
      display.clearDisplay();
      display.setCursor(0, 0);
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.println("RFID Not Found");
      display.display();
    }

    http.end();
  } else {
    Serial.println("WiFi not connected");
  }

  rfid.PICC_HaltA();
  delay(3000);
}


void generateAndDisplayQR(String url) {
  QRCode qrcode;
  uint8_t qrcodeData[qrcode_getBufferSize(3)];
  qrcode_initText(&qrcode, qrcodeData, 3, ECC_MEDIUM, url.c_str());

  int scale = 2;
  int offsetX = (SCREEN_WIDTH - qrcode.size * scale) / 2;
  int offsetY = (SCREEN_HEIGHT - qrcode.size * scale) / 2;

  display.clearDisplay();
  for (uint8_t y = 0; y < qrcode.size; y++) {
    for (uint8_t x = 0; x < qrcode.size; x++) {
      if (qrcode_getModule(&qrcode, x, y)) {
        display.fillRect(offsetX + x * scale, offsetY + y * scale, scale, scale, SSD1306_WHITE);
      }
    }
  }
  display.display();
}