#ifndef MANUAL_CONTROL_H
#define MANUAL_CONTROL_H

#include <Arduino.h>

class ManualControl {
private:
  uint8_t motorLeftDir, motorRightDir;
  uint8_t motorLeftPwm, motorRightPwm;
  uint8_t fontIr, diaIr;
  uint8_t gocPhai, truocPhai, gocTrai, truocTrai;
  uint8_t ledPin;
  bool ledState = false;

  const int NO_OBJECT_THRESHOLD = 50;
  const unsigned long TURN_LEFT_90_DEGREES = 130;
  const unsigned long TURN_RIGHT_90_DEGREES = 125;

  void checkAndPrintSensor(int value, const char* pos) {
    if (value < NO_OBJECT_THRESHOLD) {
      Serial.print(pos);
      Serial.print(": No object\t");
    } else {
      Serial.print(pos);
      Serial.print(": ");
      Serial.print(value);
      Serial.print(" \t");
    }
  }

public:
  ManualControl(uint8_t mLeftDir, uint8_t mRightDir,
                uint8_t mLeftPwm, uint8_t mRightPwm,
                uint8_t fIr, uint8_t dIr,
                uint8_t gPhai, uint8_t tPhai,
                uint8_t gTrai, uint8_t tTrai,
                uint8_t led)
      : motorLeftDir(mLeftDir), motorRightDir(mRightDir),
        motorLeftPwm(mLeftPwm), motorRightPwm(mRightPwm),
        fontIr(fIr), diaIr(dIr),
        gocPhai(gPhai), truocPhai(tPhai),
        gocTrai(gTrai), truocTrai(tTrai),
        ledPin(led) {}

  void begin() {
    pinMode(fontIr, OUTPUT);
    pinMode(diaIr, OUTPUT);
    pinMode(gocPhai, INPUT);
    pinMode(truocPhai, INPUT);
    pinMode(gocTrai, INPUT);
    pinMode(truocTrai, INPUT);

    pinMode(motorLeftDir, OUTPUT);
    pinMode(motorRightDir, OUTPUT);
    pinMode(motorLeftPwm, OUTPUT);
    pinMode(motorRightPwm, OUTPUT);

    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);
  }

  void printHelp() {
    Serial.println("===== DANH SACH LENH =====");
    Serial.println("H   : Di thang");
    Serial.println("B   : Di lui");
    Serial.println("L   : Quay trai 90 do");
    Serial.println("R   : Quay phai 90 do");
    Serial.println("SPL : Quay vong trai (Spin Left)");
    Serial.println("SPR : Quay vong phai (Spin Right)");
    Serial.println("LED : Bat/Tat den LED");
    Serial.println("0   : Dung dong co ngay lap tuc");
    Serial.println("?   : In lai huong dan");
    Serial.println("===========================");
  }

  void readSensors() {
    digitalWrite(fontIr, HIGH);
    digitalWrite(diaIr, HIGH);

    checkAndPrintSensor(analogRead(gocPhai), "Goc Phai");
    checkAndPrintSensor(analogRead(truocPhai), "Truoc Phai");
    checkAndPrintSensor(analogRead(gocTrai), "Goc Trai");
    checkAndPrintSensor(analogRead(truocTrai), "Truoc Trai");
    Serial.println();
  }

  void goStraight() {
    digitalWrite(motorLeftDir, LOW);
    analogWrite(motorLeftPwm, 200);
    digitalWrite(motorRightDir, HIGH);
    analogWrite(motorRightPwm, 205);
    Serial.println("Di thang");
  }

  void goBackward() {
    digitalWrite(motorLeftDir, HIGH);
    analogWrite(motorLeftPwm, 200);
    digitalWrite(motorRightDir, LOW);
    analogWrite(motorRightPwm, 201);
    Serial.println("Di lui");
  }

  void turnLeft90() {
    digitalWrite(motorLeftDir, HIGH);
    analogWrite(motorLeftPwm, 200);
    digitalWrite(motorRightDir, HIGH);
    analogWrite(motorRightPwm, 200);
    delay(TURN_LEFT_90_DEGREES);
    goStraight();
    Serial.println("Hoan thanh quay trai 90 do");
  }

  void turnRight90() {
    digitalWrite(motorLeftDir, LOW);
    analogWrite(motorLeftPwm, 200);
    digitalWrite(motorRightDir, LOW);
    analogWrite(motorRightPwm, 200);
    delay(TURN_RIGHT_90_DEGREES);
    goStraight();
    Serial.println("Hoan thanh quay phai 90 do");
  }

  void spinLeft() {
    digitalWrite(motorLeftDir, HIGH);
    analogWrite(motorLeftPwm, 200);
    digitalWrite(motorRightDir, HIGH);
    analogWrite(motorRightPwm, 200);
    Serial.println("Spin Left");
  }

  void spinRight() {
    digitalWrite(motorLeftDir, LOW);
    analogWrite(motorLeftPwm, 200);
    digitalWrite(motorRightDir, LOW);
    analogWrite(motorRightPwm, 200);
    Serial.println("Spin Right");
  }

  void stopMotors() {
    analogWrite(motorLeftPwm, 0);
    analogWrite(motorRightPwm, 0);
    Serial.println(">>> STOP <<<");
  }

  void toggleLED() {
    ledState = !ledState;
    digitalWrite(ledPin, ledState ? HIGH : LOW);
    Serial.println(ledState ? "LED ON" : "LED OFF");
  }
};

#endif
