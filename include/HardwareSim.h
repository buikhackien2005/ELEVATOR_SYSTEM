#ifndef HARDWARE_SIM_H
#define HARDWARE_SIM_H

#include <Arduino.h>
#include "Config.h"

enum Direction { DIR_UP, DIR_DOWN, DIR_STOP };

class HardwareSim {
private:
    int currentVirtualFloor;
    bool isMoving;
    Direction currentDir;

    uint32_t lastMoveTime;
    bool floorReachedFlag;

    void updateFloorLEDs();
    void updateDirectionLEDs(); // Cập nhật đèn Xanh dương (Lên) và Vàng (Xuống)

public:
    HardwareSim();
    void init();
    void update(); // Chạy ngầm mô phỏng thời gian
    
    void moveUp();
    void moveDown();
    void stop();
    
    void openDoor();
    void closeDoor();
    
    bool checkFloorReached(); 
    int getCurrentFloor();
};

#endif