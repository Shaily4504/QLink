#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <WiFiClientSecure.h>
#include <MFRC522.h>
#include <SPI.h>
#include <qrcode.h>  

// OLED config
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
QRCode qrcode;

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
  display.setCursor(0, 0);
  display.println("WiFi Connected!");
  Serial.print("Wifi Connected");
  display.println("RFID Scanner Ready");
  display.display();
  delay(2000);
  display.clearDisplay();
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
    HTTPClient http;
    String url = "https://qrsend-backend.onrender.com/get-user-by-rfid/" + scannedRFID;

    http.begin(client, url);  // ✅ Corrected

    int httpCode = http.GET();
    Serial.print("HTTP Code: ");
    Serial.println(httpCode);

    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println(payload);

      int nameStart = payload.indexOf("label\":\"") + 8;
      int nameEnd = payload.indexOf("\"", nameStart);
      String name = payload.substring(nameStart, nameEnd);
      int fileStart = payload.indexOf("pdfUrl\":\"") + 8;
      int fileNameEnd = payload.indexOf("\"", nameStart);
      String fileName = payload.substring(filestart, filenameEnd);

      String fileurl = "https://yourserver.com/files/" + fileName;

      display.clearDisplay();
      display.setCursor(50, 0);
      display.setTextSize(1);
      display.setTextColor(WHITE);
      //display.println("RFID Matched");
      // display.println(name);
      generateAndDisplayQR(fileurl.c_str()); // Send as const char*
      Serial.println(name);
      display.display();
    } else {
      display.clearDisplay();
      display.setCursor(0, 0);
      display.println("RFID Not Found");
      display.display();
    }

    http.end();
  } else {
    Serial.println("WiFi not connected");
  }

  rfid.PICC_HaltA();  // Stop reading
  delay(3000);        // 3 seconds before next read
}

void generateAndDisplayQR(const char* text) {
  display.clearDisplay();
  
  // Initialize QR code
  qrcode_initText(&qrcode, qrcodeData, 3, ECC_LOW, text); // version 3, error correction LOW

  int scale = 2; // each QR pixel becomes 2x2 screen pixels
  int offsetX = 10;  // Padding left
  int offsetY = 10;  // Padding top

  for (int y = 0; y < qrcode.size; y++) {
    for (int x = 0; x < qrcode.size; x++) {
      if (qrcode_getModule(&qrcode, x, y)) {
        // Draw a filled rectangle for each "black" module
        display.fillRect(offsetX + x * scale, offsetY + y * scale, scale, scale, WHITE);
      }
    }
  }

  display.display();
}