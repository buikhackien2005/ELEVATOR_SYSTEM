#ifndef CONFIG_H
#define CONFIG_H

// ================= NÚT BẤM (INPUT_PULLUP) =================
// Nút bên trong buồng thang
#define PIN_BTN_IN_F1 26
#define PIN_BTN_IN_F2 27
#define PIN_BTN_IN_F3 14

// Nút ngoài hành lang
#define PIN_BTN_OUT_F1_UP 18
#define PIN_BTN_OUT_F2_UP 4
#define PIN_BTN_OUT_F2_DN 5
#define PIN_BTN_OUT_F3_DN 19

// Nút điều khiển cửa
#define PIN_BTN_OPEN 25
#define PIN_BTN_CLOSE 13

// ================= MÀN HÌNH OLED (I2C) =================
#define PIN_I2C_SDA 21
#define PIN_I2C_SCL 22

// ================= ĐẦU RA MÔ PHỎNG (OUTPUT) =================
// LED báo vị trí tầng (Xanh lá)
#define PIN_LED_F1 32
#define PIN_LED_F2 33
#define PIN_LED_F3 23

// LED báo trạng thái
#define PIN_LED_UP 2      // Xanh dương - Thang đi Lên
#define PIN_LED_DOWN 16   // Vàng - Thang đi Xuống
#define PIN_LED_DOOR 17   // Đỏ - Đang mở cửa

// Âm thanh
#define PIN_BUZZER 15

// ================= THÔNG SỐ HỆ THỐNG =================
#define TIME_BETWEEN_FLOORS 3000 // 3000ms = 3 giây/tầng
#define TIME_DOOR_OPEN 3000      // Thời gian mở cửa 3 giây

#endif