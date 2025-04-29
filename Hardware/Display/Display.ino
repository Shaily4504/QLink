 #include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "qrcode.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(115200);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // OLED I2C addr = 0x3C
    Serial.println(F("SSD1306 allocation failed"));
    while (true);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  // The text you want to encode in the QR code
  String qrText = "https://shorturl.at/gJdzq"; // Change to your link
  generateAndDrawQR(qrText);
}

void loop() {
  // Nothing here
}

void generateAndDrawQR(String data) {
  QRCode qrcode;
  uint8_t qrcodeData[qrcode_getBufferSize(3)];

  // Version 3, Error Correction Level M
  qrcode_initText(&qrcode, qrcodeData, 3, ECC_MEDIUM, data.c_str());

  int scale = 2; // Pixel size of each QR square
  int offsetX = (SCREEN_WIDTH - qrcode.size * scale) / 2;
  int offsetY = (SCREEN_HEIGHT - qrcode.size * scale) / 2;

  display.clearDisplay();

  // Draw QR Code on OLED
  for (uint8_t y = 0; y < qrcode.size; y++) {
    for (uint8_t x = 0; x < qrcode.size; x++) {
      if (qrcode_getModule(&qrcode, x, y)) {
        display.fillRect(offsetX + x * scale, offsetY + y * scale, scale, scale, SSD1306_WHITE);
      }
    }
  }

  display.display();
}
