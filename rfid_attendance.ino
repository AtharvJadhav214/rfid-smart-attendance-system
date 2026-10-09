/**
 * @file rfid_attendance.ino
 * @brief RFID-Based Smart Attendance System with GSM/Wi-Fi logging
 * @author Atharv Anil Jadhav
 * Hardware: ESP32, MFRC522 RFID Reader, SIM800L GSM, 16x2 I2C LCD
 */

#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <HardwareSerial.h>

// Pin Configuration for ESP32
#define SS_PIN    5   // SPI CS
#define RST_PIN   22  // RFID Reset
#define GSM_RX    16  // ESP32 RX2 connects to SIM800L TX
#define GSM_TX    17  // ESP32 TX2 connects to SIM800L RX
#define BUZZER_PIN 4

MFRC522 mfrc522(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2);
HardwareSerial gsmSerial(2);

// Whitelisted Student/Employee Tags
const String AUTHORIZED_TAG_1 = "A3 F4 21 09";
const String STUDENT_NAME_1   = "Student 1";
const String PARENT_PHONE_1   = "+91XXXXXXXXXX";

void setup() {
  Serial.begin(115200);
  gsmSerial.begin(9600, SERIAL_8N1, GSM_RX, GSM_TX);
  
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  // Initialize LCD & SPI
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Smart Attendance");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");

  SPI.begin();
  mfrc522.PCD_Init();
  delay(1000);

  // Initialize GSM Modem
  sendATCommand("AT", 1000);
  sendATCommand("AT+CMGF=1", 1000); // Set SMS to text mode

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Scan RFID Tag...");
}

void loop() {
  // Look for new RFID card
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  String scannedUID = getUIDString();
  Serial.print("Card Scanned: ");
  Serial.println(scannedUID);

  // Process Card
  processAttendance(scannedUID);

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
  delay(2000);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Scan RFID Tag...");
}

String getUIDString() {
  String uid = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    uid.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    uid.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  uid.toUpperCase();
  return uid.substring(1);
}

void processAttendance(String uid) {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(150);
  digitalWrite(BUZZER_PIN, LOW);

  if (uid == AUTHORIZED_TAG_1) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Welcome!");
    lcd.setCursor(0, 1);
    lcd.print(STUDENT_NAME_1);

    sendSMS(PARENT_PHONE_1, STUDENT_NAME_1 + " marked PRESENT at " + String(millis() / 1000) + "s.");
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Invalid Card!");
    lcd.setCursor(0, 1);
    lcd.print("Access Denied");
  }
}

void sendATCommand(String cmd, int timeout) {
  gsmSerial.println(cmd);
  long int time = millis();
  while ((time + timeout) > millis()) {
    while (gsmSerial.available()) {
      Serial.write(gsmSerial.read());
    }
  }
}

void sendSMS(String phone, String message) {
  gsmSerial.println("AT+CMGS=\"" + phone + "\"");
  delay(500);
  gsmSerial.print(message);
  delay(500);
  gsmSerial.write(26); // ASCII code of CTRL+Z to send
  delay(2000);
}
