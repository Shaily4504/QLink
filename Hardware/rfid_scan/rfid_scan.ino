#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "qrcode.h"

// OLED config
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// RFID config
constexpr uint8_t RST_PIN = D3;
constexpr uint8_t SS_PIN = D4;
MFRC522 rfid(SS_PIN, RST_PIN);

String qrText = "https://github.com/Neel123priyansh";

void setup() {
  Serial.begin(115200);
  SPI.begin();
  rfid.PCD_Init();

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("RFID Scanner Ready");
  display.display();
  delay(2000);
}

void loop() {
  String tag = "";

  if (!rfid.PICC_IsNewCardPresent()) return;
  if (!rfid.PICC_ReadCardSerial()) return;

  for (byte i = 0; i < rfid.uid.size; i++) {
    tag += String(rfid.uid.uidByte[i]);
  }

  Serial.print("Scanned Tag: ");
  Serial.println(tag);

  display.clearDisplay();

  if (tag == "3518510840") {
    Serial.println("Access Granted");

    // Centered welcome text
    display.setTextSize(1);
    display.setCursor((SCREEN_WIDTH - 90) / 2, 10);
    display.println("Access Granted");
    display.setCursor((SCREEN_WIDTH - 110) / 2, 25);
    display.println("Yash Bhushan Pandey");
    display.display();
    delay(1500);

    // Show QR
    generateAndDrawQR(qrText);
    delay(3000);
  }
  else if(tag == "1924310820"){
    Serial.println("Access Granted");
    display.setTextSize(1);
    display.setCursor((SCREEN_WIDTH - 90) / 2, 10);
    display.println("Access Granted");
    display.setCursor((SCREEN_WIDTH - 110) / 2, 25);
    display.println("Altamash Beg");
    display.display();
    delay(1500);
    generateAndDrawQR(qrText);
    display.clearDisplay();
    delay(3000);
  }
   else {
    Serial.println("Access Denied");
    display.setTextSize(2);
    display.setCursor((SCREEN_WIDTH - 80) / 2, 20);
    display.println("Access");
    display.setCursor((SCREEN_WIDTH - 80) / 2, 40);
    display.println("Denied");
    display.display();
    delay(2000);
  }

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}

// QR code drawing
void generateAndDrawQR(String data) {
  QRCode qrcode;
  uint8_t qrcodeData[qrcode_getBufferSize(3)];
  qrcode_initText(&qrcode, qrcodeData, 3, ECC_MEDIUM, data.c_str());

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
