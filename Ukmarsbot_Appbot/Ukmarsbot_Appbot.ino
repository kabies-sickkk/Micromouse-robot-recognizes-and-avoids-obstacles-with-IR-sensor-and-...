#include "ManualControl.h"
#include "ReadData.h"
#include "Setting.h"

// ==== Cấu hình chân ====
const uint8_t ENCODER_LEFT_CLK  = 2;
const uint8_t ENCODER_RIGHT_CLK = 3;
const uint8_t ENCODER_LEFT_B    = 4;
const uint8_t ENCODER_RIGHT_B   = 5;
const uint8_t LED_PIN           = 6;
const uint8_t MOTOR_LEFT_DIR    = 7;
const uint8_t MOTOR_RIGHT_DIR   = 8;
const uint8_t MOTOR_LEFT_PWM    = 9;
const uint8_t MOTOR_RIGHT_PWM   = 10;

#define FONT_IR     11
#define DIA_IR      12
#define GOC_PHAI    A0
#define TRUOC_PHAI  A1
#define GOC_TRAI    A2
#define TRUOC_TRAI  A3
const uint8_t SWITCHES_PIN = A6;
const uint8_t BATTERY_PIN  = A7;

// ==== Tạo đối tượng ====
ManualControl robot(
  MOTOR_LEFT_DIR, MOTOR_RIGHT_DIR,
  MOTOR_LEFT_PWM, MOTOR_RIGHT_PWM,
  FONT_IR, DIA_IR,
  GOC_PHAI, TRUOC_PHAI, GOC_TRAI, TRUOC_TRAI,
  LED_PIN
);

ReadData reader(
  MOTOR_LEFT_PWM, MOTOR_RIGHT_PWM,
  ENCODER_LEFT_CLK, ENCODER_RIGHT_CLK,
  ENCODER_LEFT_B, ENCODER_RIGHT_B,
  BATTERY_PIN,
  FONT_IR, DIA_IR,
  GOC_PHAI, TRUOC_PHAI, GOC_TRAI, TRUOC_TRAI
);

Setting config;   // Đối tượng cấu hình hệ thống

// ==============================================================
void setup() {
  Serial.begin(9600);
  delay(1000);

  Serial.println(F("\n===== KHOI DONG HE THONG UKMARSBOT ====="));
  robot.begin();
  reader.begin();
  config.begin();

  robot.printHelp();
  reader.printHelp();
  config.printHelp();

  Serial.println(F("=========================================="));
}

// ==============================================================

void loop() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();

    // ====== LỆNH ĐIỀU KHIỂN ROBOT ======
    if (cmd == "S") robot.readSensors();
    else if (cmd == "H") robot.goStraight();
    else if (cmd == "B") robot.goBackward();
    else if (cmd == "L") robot.turnLeft90();
    else if (cmd == "R") robot.turnRight90();
    else if (cmd == "SPL") robot.spinLeft();
    else if (cmd == "SPR") robot.spinRight();
    else if (cmd == "LED") robot.toggleLED();
    else if (cmd == "0") robot.stopMotors();
    else if (cmd == "?S") config.printHelp();
    else if (cmd == "?M") robot.printHelp();
    else if (cmd == "?D") reader.printHelp();
    else if (cmd == "?") {
      robot.printHelp();
      reader.printHelp();
      config.printHelp();
    }

    // ====== LỆNH ĐỌC DỮ LIỆU CẢM BIẾN & ENCODER ======
    else if (cmd == "PWML") reader.printPWML();
    else if (cmd == "PWMR") reader.printPWMR();
    else if (cmd == "ECDL") reader.printEncoderLeft();
    else if (cmd == "ECDR") reader.printEncoderRight();
    else if (cmd == "VL")   reader.printSpeedLeft();
    else if (cmd == "VR")   reader.printSpeedRight();
    else if (cmd == "PIN")  reader.printBattery();
    else if (cmd == "IR" || cmd == "S") robot.readSensors();

    // ====== LỆNH THÔNG TIN HỆ THỐNG ======
    else if (cmd == "INFO") {
      Serial.println(F("===== THONG TIN ROBOT ====="));
      reader.printBattery();
      reader.printEncoderLeft();
      reader.printEncoderRight();
      reader.printSpeedLeft();
      reader.printSpeedRight();
      reader.printPWML();
      reader.printPWMR();
      robot.readSensors();
      config.printConfig();
      Serial.println(F("============================"));
    }

    // ====== LỆNH CÀI ĐẶT (SETTING) ======
    else if (cmd.startsWith("Set") || cmd == "ShowConfig") {
      config.updateSetting(cmd);
    }

    // ====== LỆNH KHÔNG HỢP LỆ ======
    else {
      Serial.println(F("Lenh khong hop le. Goi '?' de xem huong dan."));
    }
  }

  // ====== CẬP NHẬT TỐC ĐỘ ENCODER MỖI CHU KỲ ======
  reader.updateEncoders();
}
