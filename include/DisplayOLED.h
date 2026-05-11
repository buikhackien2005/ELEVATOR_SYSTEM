#ifndef DISPLAY_OLED_H
#define DISPLAY_OLED_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Config.h"

class DisplayOLED {
private:
    Adafruit_SSD1306 display;

public:
    // Khởi tạo màn hình với kích thước chuẩn 128x64
    DisplayOLED() : display(128, 64, &Wire, -1) {}

    void init();
    void updateFloor(int floor);
    void updateDirection(String dir);
    void showDoorState(String state); // Hiển thị chữ "OPENING..." hoặc "CLOSED"
};

#endif