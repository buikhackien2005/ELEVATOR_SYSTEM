#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

class Button {
private:
    uint8_t pin;
    bool state;
    bool lastPhysicalState;
    uint32_t lastDebounceTime;
    const uint32_t debounceDelay = 50; // Chống nhiễu 50ms
    bool isPressedFlag; // Bẫy cờ trạng thái

public:
    Button(uint8_t p);
    void init();
    void update();      // Hàm chạy ngầm để quét trạng thái
    bool isPressed();   // Trả về true nếu nút vừa bị nhấn
};

#endif