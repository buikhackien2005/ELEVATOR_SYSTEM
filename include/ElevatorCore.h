#ifndef ELEVATOR_CORE_H
#define ELEVATOR_CORE_H

#include <Arduino.h>
#include <set>             // Thư viện tập hợp của C++ để chống trùng lặp tầng
#include "HardwareSim.h"
#include "DisplayOLED.h"

// Định nghĩa 4 trạng thái độc lập của Máy trạng thái (FSM)
enum ElevatorState {
    IDLE,
    MOVING_UP,
    MOVING_DOWN,
    DOOR_OPERATING
};

class ElevatorCore {
private:
    int currentFloor;
    int targetFloor;
    ElevatorState currentState;
    
    // Hàng đợi thông minh: Tự động sắp xếp từ bé đến lớn và không chứa ID trùng nhau
    std::set<int> requestUp;
    std::set<int> requestDown;

    // Bộ đếm thời gian cho khối DOOR_OPERATING (Non-blocking)
    uint32_t doorStartTime;

    // Con trỏ lưu "địa chỉ" của phần cứng để Core ra lệnh (Quan hệ Composition)
    HardwareSim* hardware;
    DisplayOLED* display;

    // Hàm nội bộ: Tính toán đích đến tiếp theo
    void calculateNextTarget();

public:
    // Khởi tạo: Truyền địa chỉ phần cứng vào tay ElevatorCore
    ElevatorCore(HardwareSim* hw, DisplayOLED* disp);
    void init();
    
    // Hàm cốt lõi: Nơi chứa thuật toán Switch-Case điều phối thang máy
    void processQueue();

    // ================= CÁC API CHO NÚT BẤM (Giao tiếp Đầu vào) =================
    void addRequestInside(int floor);
    void addRequestOutside(int floor, String direction);
    void triggerOpenDoor();
    void triggerCloseDoor();
};

#endif