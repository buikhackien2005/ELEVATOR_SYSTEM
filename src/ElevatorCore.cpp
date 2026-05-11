#include "ElevatorCore.h"

// Biến tĩnh cục bộ: Giúp thang máy "nhớ" hướng đi cũ sau khi bị gián đoạn bởi việc mở cửa
static ElevatorState lastDirection = IDLE;

// ================== KHỞI TẠO ==================
ElevatorCore::ElevatorCore(HardwareSim* hw, DisplayOLED* disp) {
    this->hardware = hw;
    this->display = disp;
    currentFloor = 1;
    targetFloor = 1;
    currentState = IDLE;
    doorStartTime = 0;
}

void ElevatorCore::init() {
    hardware->init();
    display->init();
    display->updateFloor(currentFloor);
    display->updateDirection("IDLE");
}

// ================== THUẬT TOÁN FSM CỐT LÕI ==================
void ElevatorCore::processQueue() {
    switch (currentState) {
        
        case IDLE:
            if (!requestUp.empty() || !requestDown.empty()) {
                // VÁ LỖI KỊCH BẢN KHÁC: Nếu khách gọi đúng tại tầng thang đang đỗ, mở cửa ngay!
                if (requestUp.count(currentFloor) > 0 || requestDown.count(currentFloor) > 0) {
                    requestUp.erase(currentFloor);
                    requestDown.erase(currentFloor);
                    hardware->openDoor();
                    display->showDoorState("OPENING");
                    doorStartTime = millis();
                    currentState = DOOR_OPERATING;
                } else {
                    lastDirection = IDLE; // Bắt đầu lộ trình mới
                    calculateNextTarget();
                }
            }
            break;

        case MOVING_UP:
            if (hardware->checkFloorReached()) {
                currentFloor = hardware->getCurrentFloor();
                display->updateFloor(currentFloor);

                // VÁ LỖI KỊCH BẢN 4: Dừng nếu có người vẫy đi Lên dọc đường, HOẶC đã đến đích
                if (requestUp.count(currentFloor) > 0 || currentFloor == targetFloor) {
                    hardware->stop();
                    display->updateDirection("ARRIVED");
                    
                    requestUp.erase(currentFloor); // Luôn xóa yêu cầu đi Lên
                    
                    // VÁ LỖI KỊCH BẢN 2 & 3: Nếu là trạm cuối, xóa sạch bóng ma ở cả mảng DOWN
                    if (currentFloor == targetFloor) {
                        requestDown.erase(currentFloor);
                    }
                    
                    hardware->openDoor();
                    display->showDoorState("OPENING");
                    doorStartTime = millis();
                    lastDirection = MOVING_UP; // Lưu vào não bộ: "Vừa đi Lên xong nhé"
                    currentState = DOOR_OPERATING;
                }
            }
            break;

        case MOVING_DOWN:
            if (hardware->checkFloorReached()) {
                currentFloor = hardware->getCurrentFloor();
                display->updateFloor(currentFloor);

                // Tương tự cho chiều Xuống
                if (requestDown.count(currentFloor) > 0 || currentFloor == targetFloor) {
                    hardware->stop();
                    display->updateDirection("ARRIVED");
                    
                    requestDown.erase(currentFloor); 
                    if (currentFloor == targetFloor) {
                        requestUp.erase(currentFloor);
                    }
                    
                    hardware->openDoor();
                    display->showDoorState("OPENING");
                    doorStartTime = millis();
                    lastDirection = MOVING_DOWN;
                    currentState = DOOR_OPERATING;
                }
            }
            break;

        case DOOR_OPERATING:
            // Quét vòng lặp Non-blocking
            if (millis() - doorStartTime >= TIME_DOOR_OPEN) {
                hardware->closeDoor();
                display->showDoorState("CLOSED");
                
                // Dọn dẹp quét rác một lần nữa cho chắc chắn không kẹt bóng ma
                requestUp.erase(currentFloor);
                requestDown.erase(currentFloor);

                calculateNextTarget(); // Đóng cửa xong thì tính xem đi đâu tiếp
            }
            break;
    }
}

// ================== LOGIC ĐIỀU PHỐI (SMART ROUTING) ==================
void ElevatorCore::calculateNextTarget() {
    if (requestUp.empty() && requestDown.empty()) {
        currentState = IDLE;
        lastDirection = IDLE;
        display->updateDirection("IDLE");
        return;
    }

    // Nếu trước đó đang đi LÊN, hoặc đang ngủ đông: Quét mảng tìm khách phía TRÊN trước
    if (lastDirection == MOVING_UP || lastDirection == IDLE) {
        
        // 1. Tìm khách gọi đi LÊN ở các tầng cao hơn
        auto it = requestUp.upper_bound(currentFloor);
        if (it != requestUp.end()) {
            targetFloor = *it;
            currentState = MOVING_UP;
            hardware->moveUp();
            display->updateDirection("UP");
            return;
        }
        
        // 2. Không có khách đi LÊN, quay sang đón khách gọi đi XUỐNG ở tầng cao nhất
        if (!requestDown.empty()) {
            int highestDown = *requestDown.rbegin();
            if (highestDown > currentFloor) {
                targetFloor = highestDown;
                currentState = MOVING_UP;
                hardware->moveUp();
                display->updateDirection("UP");
                return;
            }
        }
        
        // 3. Nếu bên trên đầu hoàn toàn trống trơn, bẻ lái đổi hướng xuống!
        lastDirection = MOVING_DOWN; 
    }

    // Nếu trước đó đang đi XUỐNG: Quét mảng tìm khách phía DƯỚI trước
    if (lastDirection == MOVING_DOWN) {
        
        // 1. Tìm khách gọi đi XUỐNG ở các tầng thấp hơn
        int nextDown = -1;
        for (auto it = requestDown.rbegin(); it != requestDown.rend(); ++it) {
            if (*it < currentFloor) {
                nextDown = *it;
                break;
            }
        }
        if (nextDown != -1) {
            targetFloor = nextDown;
            currentState = MOVING_DOWN;
            hardware->moveDown();
            display->updateDirection("DOWN");
            return;
        }

        // 2. Không có khách đi XUỐNG, quay sang đón khách gọi đi LÊN ở tầng thấp nhất
        if (!requestUp.empty()) {
            int lowestUp = *requestUp.begin();
            if (lowestUp < currentFloor) {
                targetFloor = lowestUp;
                currentState = MOVING_DOWN;
                hardware->moveDown();
                display->updateDirection("DOWN");
                return;
            }
        }
        
        // 3. Nếu bên dưới chân hoàn toàn trống trơn, bẻ lái lên LÊN và đệ quy tìm lại
        lastDirection = MOVING_UP;
        calculateNextTarget();
    }
}

// ================== CÁC GIAO DIỆN NÚT BẤM ==================
void ElevatorCore::addRequestInside(int floor) {
    if (floor > currentFloor) requestUp.insert(floor);
    else if (floor < currentFloor) requestDown.insert(floor);
    else requestUp.insert(floor); // Ném vào mảng, FSM IDLE sẽ tự dọn rác và mở cửa
}

void ElevatorCore::addRequestOutside(int floor, String direction) {
    if (direction == "UP") requestUp.insert(floor);
    else if (direction == "DOWN") requestDown.insert(floor);
}

void ElevatorCore::triggerOpenDoor() {
    if (currentState == DOOR_OPERATING) doorStartTime = millis(); 
}

void ElevatorCore::triggerCloseDoor() {
    if (currentState == DOOR_OPERATING) doorStartTime = millis() - TIME_DOOR_OPEN; 
}