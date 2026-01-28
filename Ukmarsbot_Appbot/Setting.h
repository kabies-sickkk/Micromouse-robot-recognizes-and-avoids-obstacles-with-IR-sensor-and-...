#ifndef SETTING_H
#define SETTING_H

#include <Arduino.h>

class Setting {
public:
  // ==== Các tham số điều chỉnh toàn hệ thống ====
  int pwmBase;           // Mức PWM mặc định
  float Kp, Ki, Kd;      // Thông số PID
  int IR_Threshold;      // Ngưỡng cảm biến IR
  int turnLeft90_PWM;    // PWM quay trái 90 độ
  int turnRight90_PWM;   // PWM quay phải 90 độ

  // ==== Khởi tạo ====
  Setting() {
    pwmBase = 100;
    Kp = 1.0;
    Ki = 0.0;
    Kd = 0.0;
    IR_Threshold = 500;
    turnLeft90_PWM = 90;
    turnRight90_PWM = 90;
  }

  // ==== Hàm khởi động ====
  void begin() {
    Serial.println("Cau hinh he thong da san sang.");
  }

  // ==== In hướng dẫn ====
  void printHelp() {
    Serial.println(F("===== HUONG DAN LENH SETTING ====="));
    Serial.println(F("SetPWM <value>           : Dat lai PWM mac dinh"));
    Serial.println(F("SetPID <Kp> <Ki> <Kd>    : Dat lai thong so PID"));
    Serial.println(F("SetIR <threshold>        : Dat nguong cam bien IR"));
    Serial.println(F("SetTurnL <pwm>           : PWM quay trai 90 do"));
    Serial.println(F("SetTurnR <pwm>           : PWM quay phai 90 do"));
    Serial.println(F("ShowConfig               : Hien thi toan bo thong so"));
    Serial.println(F("================================="));
  }

  // ==== Cập nhật thông số từ lệnh Serial ====
  void updateSetting(String cmd) {
    cmd.trim();
    if (cmd.startsWith("SetPWM")) {
      pwmBase = cmd.substring(7).toInt();
      Serial.print(F("Da cap nhat PWM mac dinh = ")); Serial.println(pwmBase);
    }
    else if (cmd.startsWith("SetPID")) {
      float p, i, d;
      sscanf(cmd.c_str(), "SetPID %f %f %f", &p, &i, &d);
      Kp = p; Ki = i; Kd = d;
      Serial.print(F("Da cap nhat PID: Kp=")); Serial.print(Kp);
      Serial.print(F(", Ki=")); Serial.print(Ki);
      Serial.print(F(", Kd=")); Serial.println(Kd);
    }
    else if (cmd.startsWith("SetIR")) {
      IR_Threshold = cmd.substring(6).toInt();
      Serial.print(F("Da cap nhat nguong IR = ")); Serial.println(IR_Threshold);
    }
    else if (cmd.startsWith("SetTurnL")) {
      turnLeft90_PWM = cmd.substring(9).toInt();
      Serial.print(F("PWM quay trai 90 do = ")); Serial.println(turnLeft90_PWM);
    }
    else if (cmd.startsWith("SetTurnR")) {
      turnRight90_PWM = cmd.substring(9).toInt();
      Serial.print(F("PWM quay phai 90 do = ")); Serial.println(turnRight90_PWM);
    }
    else if (cmd == "ShowConfig") {
      printConfig();
    }
    else {
      Serial.println(F("Lenh khong hop le. Goi 'ShowConfig' de xem thong so hoac '?' de xem huong dan."));
    }
  }

  // ==== Hiển thị toàn bộ thông số hiện tại ====
  void printConfig() {
    Serial.println(F("===== CAU HINH HIEN TAI ====="));
    Serial.print(F("PWM mac dinh: ")); Serial.println(pwmBase);
    Serial.print(F("PID: Kp=")); Serial.print(Kp);
    Serial.print(F(", Ki=")); Serial.print(Ki);
    Serial.print(F(", Kd=")); Serial.println(Kd);
    Serial.print(F("Nguong IR: ")); Serial.println(IR_Threshold);
    Serial.print(F("PWM quay trai 90: ")); Serial.println(turnLeft90_PWM);
    Serial.print(F("PWM quay phai 90: ")); Serial.println(turnRight90_PWM);
    Serial.println(F("================================="));
  }
};

#endif
