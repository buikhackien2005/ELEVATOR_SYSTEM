#include "HardwareSim.h"

HardwareSim::HardwareSim() {
    currentVirtualFloor = 1;
    isMoving = false;
    currentDir = DIR_STOP;
    lastMoveTime = 0;
    floorReachedFlag = false;
}

void HardwareSim::init() {
    // Khai báo Output
    pinMode(PIN_LED_F1, OUTPUT);
    pinMode(PIN_LED_F2, OUTPUT);
    pinMode(PIN_LED_F3, OUTPUT);
    pinMode(PIN_LED_UP, OUTPUT);
    pinMode(PIN_LED_DOWN, OUTPUT);
    pinMode(PIN_LED_DOOR, OUTPUT);
    pinMode(PIN_BUZZER, OUTPUT);

    closeDoor();
    updateDirectionLEDs();
    updateFloorLEDs();
}

void HardwareSim::update() {
    if (isMoving) {
        // Đếm timer ảo: Đã chạy đủ 3 giây chưa?
        if (millis() - lastMoveTime >= TIME_BETWEEN_FLOORS) {
            lastMoveTime = millis();
            
            if (currentDir == DIR_UP) currentVirtualFloor++;
            else if (currentDir == DIR_DOWN) currentVirtualFloor--;

            // Chặn trên chặn dưới an toàn
            if (currentVirtualFloor > 3) currentVirtualFloor = 3;
            if (currentVirtualFloor < 1) currentVirtualFloor = 1;

            updateFloorLEDs();
            floorReachedFlag = true; // Báo cho Lõi trung tâm biết
        }
    }
}

void HardwareSim::updateFloorLEDs() {
    digitalWrite(PIN_LED_F1, (currentVirtualFloor == 1) ? HIGH : LOW);
    digitalWrite(PIN_LED_F2, (currentVirtualFloor == 2) ? HIGH : LOW);
    digitalWrite(PIN_LED_F3, (currentVirtualFloor == 3) ? HIGH : LOW);
}

void HardwareSim::updateDirectionLEDs() {
    digitalWrite(PIN_LED_UP, (currentDir == DIR_UP) ? HIGH : LOW);
    digitalWrite(PIN_LED_DOWN, (currentDir == DIR_DOWN) ? HIGH : LOW);
}

void HardwareSim::moveUp() {
    isMoving = true;
    currentDir = DIR_UP;
    updateDirectionLEDs();
    lastMoveTime = millis();
}

void HardwareSim::moveDown() {
    isMoving = true;
    currentDir = DIR_DOWN;
    updateDirectionLEDs();
    lastMoveTime = millis();
}

void HardwareSim::stop() {
    isMoving = false;
    currentDir = DIR_STOP;
    updateDirectionLEDs();
}

void HardwareSim::openDoor() {
    digitalWrite(PIN_LED_DOOR, HIGH); // Bật LED Đỏ
    
    // Buzzer kêu tít 1 tiếng báo hiệu
    digitalWrite(PIN_BUZZER, HIGH);
    delay(100); // UI delay an toàn
    digitalWrite(PIN_BUZZER, LOW);
}

void HardwareSim::closeDoor() {
    digitalWrite(PIN_LED_DOOR, LOW); // Tắt LED Đỏ
}

bool HardwareSim::checkFloorReached() {
    if (floorReachedFlag) {
        floorReachedFlag = false;
        return true;
    }
    return false;
}

int HardwareSim::getCurrentFloor() {
    return currentVirtualFloor;
}