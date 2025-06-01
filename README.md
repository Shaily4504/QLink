# QLink 🔗

QLink is a smart RFID-based document access system that generates and displays secure QR codes for viewing documents uploaded via an admin backend. It's designed for seamless, tap-to-view access to important PDFs like ID cards, reports, certificates, etc.

> ✅ Built for smart campuses, institutions, and workplaces looking to digitize document access.
> ![alt text](https://github.com/Neel123priyansh/Academo/blob/main/img/Screenshot%202025-01-24%20000714.png/?raw=true)

---

## 🔧 Features

- 📡 **RFID-based scanning**  
  Detects RFID tags (like ID cards) to identify the user instantly.

- 📄 **Auto-fetch latest document**  
  Connects with the QLink_Backend to fetch the most recent document for the scanned user.

- 🔐 **Secure access link**  
  Generates a short and secure QR code link using Bitly.

- 🖥️ **OLED Display + QR Code**  
  Shows the document name and a scannable QR code on a small OLED screen.

- ☁️ **Hosted on Render**  
  Backend API hosted on [Render](https://render.com) for public access.

---


## 📦 Hardware Used

- ESP8266 
- MFRC522 RFID Module
- SSD1306 OLED Display (I2C)
- Jumper wires, breadboard

---

## 🌐 Backend Repo

> Check out the backend powering QLink here:  
🔗 [QLink_Backend](https://github.com/Neel123priyansh/QLink_backend)
