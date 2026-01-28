#ifndef READDATA_H
#define READDATA_H

#include <Arduino.h>

class ReadData {
private:
  // ==== Cấu hình phần cứng ====
  uint8_t pwmLeftPin, pwmRightPin;
  uint8_t encLeftA, encRightA;
  uint8_t encLeftB, encRightB;
  uint8_t batteryPin;
  uint8_t irFront, irBottom;
  uint8_t irRightAngle, irRightFront, irLeftAngle, irLeftFront;

  // ==== Encoder ====
  volatile long pulseCountLeft = 0;
  volatile long pulseCountRight = 0;
  const int PULSES_PER_REV = 20;
  const int GEAR_RATIO = 30;
  const float WHEEL_DIAMETER = 0.065;
  const float WHEEL_CIRCUMFERENCE = PI * WHEEL_DIAMETER;
  unsigned long lastSpeedTime = 0;
  long lastPulseLeft = 0, lastPulseRight = 0;
  float speedLeft = 0, speedRight = 0;

  // ==== PWM hiện tại ====
  int pwmLeft = 0;
  int pwmRight = 0;

  // ==== Battery ====
  float readBatteryVoltage() {
    int adcValue = analogRead(batteryPin);
    float voltage = adcValue * (5.0 / 1023.0) * 4.0; // chia áp 30k-10k
    return voltage;
  }

  int batteryPercent(float voltage) {
    int percent = map((int)(voltage * 100), 650, 840, 0, 100);
    return constrain(percent, 0, 100);
  }

  // ==== IR ====
  int readIR(uint8_t pin) {
    return analogRead(pin);
  }

public:
  ReadData(uint8_t pwmL, uint8_t pwmR,
           uint8_t encLA, uint8_t encRA,
           uint8_t encLB, uint8_t encRB,
           uint8_t batPin,
           uint8_t irF, uint8_t irB,
           uint8_t irRA, uint8_t irRF, uint8_t irLA, uint8_t irLF)
    : pwmLeftPin(pwmL), pwmRightPin(pwmR),
      encLeftA(encLA), encRightA(encRA),
      encLeftB(encLB), encRightB(encRB),
      batteryPin(batPin),
      irFront(irF), irBottom(irB),
      irRightAngle(irRA), irRightFront(irRF),
      irLeftAngle(irLA), irLeftFront(irLF) {}

  // ==== Khởi tạo ====
  void begin() {
    pinMode(encLeftA, INPUT_PULLUP);
    pinMode(encRightA, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(encLeftA), [](){ instance->pulseCountLeft++; }, RISING);
    attachInterrupt(digitalPinToInterrupt(encRightA), [](){ instance->pulseCountRight++; }, RISING);
    instance = this;
  }

  // ==== Cập nhật encoder ====
  void updateEncoders() {
    unsigned long now = millis();
    if (now - lastSpeedTime >= 500) {
      long dL = pulseCountLeft - lastPulseLeft;
      long dR = pulseCountRight - lastPulseRight;

      float revL = (float)dL / (PULSES_PER_REV * GEAR_RATIO);
      float revR = (float)dR / (PULSES_PER_REV * GEAR_RATIO);

      speedLeft = (revL * WHEEL_CIRCUMFERENCE) / ((now - lastSpeedTime) / 1000.0);
      speedRight = (revR * WHEEL_CIRCUMFERENCE) / ((now - lastSpeedTime) / 1000.0);

      lastPulseLeft = pulseCountLeft;
      lastPulseRight = pulseCountRight;
      lastSpeedTime = now;
    }
  }

  // ==== Lệnh in thông tin ====
  void printPWML() {
    Serial.print("PWM trai hien tai: ");
    Serial.println(pwmLeft);
  }

  void printPWMR() {
    Serial.print("PWM phai hien tai: ");
    Serial.println(pwmRight);
  }

  void setPWML(int val) {
    pwmLeft = constrain(val, 0, 255);
    analogWrite(pwmLeftPin, pwmLeft);
  }

  void setPWMR(int val) {
    pwmRight = constrain(val, 0, 255);
    analogWrite(pwmRightPin, pwmRight);
  }

  void printEncoderLeft() {
    Serial.print("So vong quay trai: ");
    Serial.println((float)pulseCountLeft / (PULSES_PER_REV * GEAR_RATIO), 3);
  }

  void printEncoderRight() {
    Serial.print("So vong quay phai: ");
    Serial.println((float)pulseCountRight / (PULSES_PER_REV * GEAR_RATIO), 3);
  }

  void printSpeedLeft() {
    Serial.print("Van toc trai: ");
    Serial.print(speedLeft, 3);
    Serial.println(" m/s");
  }

  void printSpeedRight() {
    Serial.print("Van toc phai: ");
    Serial.print(speedRight, 3);
    Serial.println(" m/s");
  }

  void printBattery() {
    float v = readBatteryVoltage();
    Serial.print("Pin: ");
    Serial.print(v, 2);
    Serial.print("V | ");
    Serial.print(batteryPercent(v));
    Serial.println("%");
  }

  void printIRSensors() {
    Serial.print("IR-GocPhai: "); Serial.print(readIR(irRightAngle));
    Serial.print(" | TruocPhai: "); Serial.print(readIR(irRightFront));
    Serial.print(" | GocTrai: "); Serial.print(readIR(irLeftAngle));
    Serial.print(" | TruocTrai: "); Serial.println(readIR(irLeftFront));
  }

  void printHelp() {
    Serial.println(F("=== LENH READ DATA ==="));
    Serial.println(F("PWML [x] - doc/ghi PWM trai"));
    Serial.println(F("PWMR [x] - doc/ghi PWM phai"));
    Serial.println(F("ECDL - so vong encoder trai"));
    Serial.println(F("ECDR - so vong encoder phai"));
    Serial.println(F("VL - toc do trai (m/s)"));
    Serial.println(F("VR - toc do phai (m/s)"));
    Serial.println(F("PIN - doc dien ap pin (%)"));
    Serial.println(F("S - doc 4 cam bien IR"));
    Serial.println(F("========================"));
  }

  // ==== Đảm bảo ISR hoạt động đúng ====
  static ReadData* instance;
};
ReadData* ReadData::instance = nullptr;

#endif
