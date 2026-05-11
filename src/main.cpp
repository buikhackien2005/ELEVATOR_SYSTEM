#include <Arduino.h>
#include "Config.h"
#include "Button.h"
#include "HardwareSim.h"
#include "DisplayOLED.h"
#include "ElevatorCore.h"

// 1. Khởi tạo Không gian đối tượng Toàn cục (Global Objects)
HardwareSim hardware;
DisplayOLED display;
ElevatorCore core(&hardware, &display); // Nhét tay chân vào Lõi

// Khởi tạo đối tượng 9 nút bấm
Button btnInF1(PIN_BTN_IN_F1);
Button btnInF2(PIN_BTN_IN_F2);
Button btnInF3(PIN_BTN_IN_F3);

Button btnOutF1Up(PIN_BTN_OUT_F1_UP);
Button btnOutF2Up(PIN_BTN_OUT_F2_UP);
Button btnOutF2Dn(PIN_BTN_OUT_F2_DN);
Button btnOutF3Dn(PIN_BTN_OUT_F3_DN);

Button btnOpen(PIN_BTN_OPEN);
Button btnClose(PIN_BTN_CLOSE);

void setup() {
    Serial.begin(115200);
    
    // Gọi phương thức init() của tất cả phần cứng đầu vào
    btnInF1.init(); btnInF2.init(); btnInF3.init();
    btnOutF1Up.init(); btnOutF2Up.init(); btnOutF2Dn.init(); btnOutF3Dn.init();
    btnOpen.init(); btnClose.init();
    
    // Khởi tạo Lõi trung tâm (Lõi sẽ tự động gọi init() của LED và OLED)
    core.init();
    Serial.println("Hệ thống Thang máy ESP32 Sẵn sàng!");
}

void loop() {
    // =================================================================
    // BƯỚC 1: QUÉT LIÊN TỤC TRẠNG THÁI NÚT BẤM VÀ BỘ ĐẾM THỜI GIAN
    // (Luôn chạy mượt mà, không bao giờ bị chặn lại)
    // =================================================================
    btnInF1.update(); btnInF2.update(); btnInF3.update();
    btnOutF1Up.update(); btnOutF2Up.update(); btnOutF2Dn.update(); btnOutF3Dn.update();
    btnOpen.update(); btnClose.update();
    
    hardware.update(); // Chạy ngầm mô phỏng Motor

    // =================================================================
    // BƯỚC 2: PHÂN LUỒNG TÍN HIỆU ĐẦU VÀO VÀO HÀNG ĐỢI FSM
    // =================================================================
    if (btnInF1.isPressed()) core.addRequestInside(1);
    if (btnInF2.isPressed()) core.addRequestInside(2);
    if (btnInF3.isPressed()) core.addRequestInside(3);

    if (btnOutF1Up.isPressed()) core.addRequestOutside(1, "UP");
    if (btnOutF2Up.isPressed()) core.addRequestOutside(2, "UP");
    if (btnOutF2Dn.isPressed()) core.addRequestOutside(2, "DOWN");
    if (btnOutF3Dn.isPressed()) core.addRequestOutside(3, "DOWN");

    if (btnOpen.isPressed()) core.triggerOpenDoor();
    if (btnClose.isPressed()) core.triggerCloseDoor();

    // =================================================================
    // BƯỚC 3: KÍCH HOẠT VÒNG QUAY MÁY TRẠNG THÁI (CORE LOGIC)
    // =================================================================
    core.processQueue();
}