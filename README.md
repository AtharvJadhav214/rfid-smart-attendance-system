# 📡 RFID-Based Smart Attendance System

An automated, proxy-free attendance tracking solution built on the **ESP32 microcontroller**, integrating RFID hardware, I2C LCD, and cellular GSM telemetry for real-time attendance marking and SMS alerts.

---

## 🚀 Key Features
- **Contactless Authentication:** Uses 13.56 MHz MFRC522 RFID reader for UID verification.
- **Cellular Telemetry:** Automated parent/admin SMS notifications using SIM800L GSM modem via UART AT commands.
- **Local User Feedback:** 16x2 LCD interfaced over I2C displaying student identity and attendance status.
- **High Efficiency:** Eliminates paper rolls and manual proxy marking, saving over 90% tracking time.

---

## 🛠️ Hardware Stack
| Component | Interface | Description |
| :--- | :--- | :--- |
| **ESP32 DevKit** | Master MCU | 32-bit Dual Core, handles logic & comms |
| **MFRC522 RFID Module** | SPI | 13.56 MHz RFID transceiver |
| **SIM800L GSM Module** | UART (HardwareSerial2) | Cellular SMS transmission |
| **16x2 LCD Display** | I2C (PCF8574) | Visual display interface (Address: `0x27`) |
| **Piezo Buzzer** | GPIO | Audio feedback upon successful scan |

---

## 📋 Pin Mapping
- **MFRC522 $\rightarrow$ ESP32:** SS/CS (`GPIO 5`), SCK (`GPIO 18`), MOSI (`GPIO 23`), MISO (`GPIO 19`), RST (`GPIO 22`)
- **SIM800L $\rightarrow$ ESP32:** TX $\rightarrow$ RX2 (`GPIO 16`), RX $\rightarrow$ TX2 (`GPIO 17`)
- **I2C LCD $\rightarrow$ ESP32:** SDA (`GPIO 21`), SCL (`GPIO 22`)

---

## 👨‍💻 Author
**Atharv Anil Jadhav**  
*Electronics & Telecommunication Engineering*  
[LinkedIn Profile](https://www.linkedin.com/in/atharvajadhav214)
