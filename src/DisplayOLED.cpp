#include "DisplayOLED.h"

// Các biến trạng thái nội bộ để giữ nội dung trên màn hình không bị nhấp nháy
static int currentDispFloor = 1;
static String currentDispDir = "IDLE";
static String currentDoorState = "CLOSED";

// Hàm helper tĩnh: Vẽ lại toàn bộ màn hình một cách đồng bộ
static void redrawScreen(Adafruit_SSD1306& disp) {
    disp.clearDisplay();
    
    // Viết Số tầng (Cỡ chữ siêu lớn)
    disp.setTextSize(5);
    disp.setTextColor(SSD1306_WHITE);
    disp.setCursor(10, 15);
    disp.print(currentDispFloor);
    
    // Viết Hướng di chuyển (Cỡ chữ thường)
    disp.setTextSize(1);
    disp.setCursor(60, 15);
    disp.print("DIR: ");
    disp.print(currentDispDir);
    
    // Viết Trạng thái Cửa
    disp.setCursor(60, 40);
    disp.print("DOOR:");
    disp.setCursor(60, 50);
    disp.print(currentDoorState);
    
    disp.display(); // Đẩy dữ liệu ra I2C
}

void DisplayOLED::init() {
    // Khởi tạo I2C với 2 chân đã quy hoạch trong Config.h
    Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);
    
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("Lỗi: Không tìm thấy OLED"));
        for(;;); // Khóa chết hệ thống nếu không có màn hình
    }
    
    redrawScreen(display);
}

void DisplayOLED::updateFloor(int floor) {
    currentDispFloor = floor;
    redrawScreen(display);
}

void DisplayOLED::updateDirection(String dir) {
    currentDispDir = dir;
    redrawScreen(display);
}

void DisplayOLED::showDoorState(String state) {
    currentDoorState = state;
    redrawScreen(display);
}