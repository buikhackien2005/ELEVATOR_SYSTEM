#include "Button.h"

// 1. Hàm khởi tạo (Constructor): Thiết lập giá trị mặc định
Button::Button(uint8_t p) {
    pin = p;
    state = HIGH;             // Dùng trở kéo lên nội bộ nên mặc định là HIGH
    lastPhysicalState = HIGH; 
    lastDebounceTime = 0;
    isPressedFlag = false;    // Cờ báo hiệu chưa có ai bấm
}

// 2. Hàm cấu hình phần cứng
void Button::init() {
    pinMode(pin, INPUT_PULLUP);
}

// 3. Hàm Update (Thuật toán Non-blocking cốt lõi chạy trong loop)
void Button::update() {
    // Đọc trạng thái vật lý thực tế ngay lúc này
    bool currentPhysicalState = digitalRead(pin);

    // Nếu có sự dao động điện áp (tay người vừa chạm vào hoặc thả ra)
    if (currentPhysicalState != lastPhysicalState) {
        lastDebounceTime = millis(); // Reset lại đồng hồ chống nhiễu
    }

    // Nếu trạng thái đã đứng yên đủ lâu (vượt qua thời gian nhiễu 50ms)
    if ((millis() - lastDebounceTime) > debounceDelay) {
        
        // Nếu trạng thái ổn định này thực sự khác với trạng thái ta đang lưu
        if (currentPhysicalState != state) {
            state = currentPhysicalState; // Cập nhật trạng thái mới

            // CHỈ kích hoạt cờ báo hiệu khi nút bị KÉO XUỐNG GND (HIGH -> LOW)
            if (state == LOW) {
                isPressedFlag = true;
            }
        }
    }

    // Lưu lại trạng thái vật lý cho lần quét vòng lặp loop() tiếp theo
    lastPhysicalState = currentPhysicalState;
}

// 4. Hàm trích xuất dữ liệu (Giao tiếp với Lõi trung tâm)
bool Button::isPressed() {
    if (isPressedFlag) {
        // Lệnh bẫy chuột: Khi Lõi trung tâm hỏi và ta xác nhận "Có bấm",
        // phải lập tức xóa cờ để tránh việc Lõi nhận nhầm thành 2 lần bấm.
        isPressedFlag = false; 
        return true;
    }
    return false;
}