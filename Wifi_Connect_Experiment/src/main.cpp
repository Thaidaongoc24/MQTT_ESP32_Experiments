#include <Arduino.h>
#include "WiFiFSM.h"
#include "LED.h"
#include "secrets/wifi.h" // namespace WiFiSecrets

namespace
{
    // const char *WF_SSID = "Wokwi-GUEST";
    // const char *WF_PASS = "";
    const char *WF_SSID = WiFiSecrets::ssid;
    const char *WF_PASS = WiFiSecrets::pass;
}

LED led(LED_PIN, LED_ACT);
WiFiFSM wifi(WF_SSID, WF_PASS, led);

void setup()
{
    Serial.begin(115200);
    delay(100);
    wifi.on();
}

void loop()
{
    wifi.loop();
}

/** Thí nghiệm giải quyết vấn đề kết nối lại khi mất wifi
 * - Test bằng WF hotspot trên điện thoại --> tăng tốc thí nghiệm
 * - Kết quả mới: WF AP bật sau khi khởi động ESP32 --> chờ vài phút vẫn tự kết nối vào được !!!
 *
 * Vấn đề: Chỉ kết nối thành công mỗi hai lần bấm reset.
 * - Đang kết nối --> bấm hard-reset --> mãi không thành công
 * - ... --> bấm hard-reset lần nữa --> thành công.
 * - Lý do: https://github.com/espressif/arduino-esp32/pull/6359
 * - Giải pháp: PIO menu --> Platforms --> Updates Espressif 32 to 6.3.2 --> Solved!
 *
 * Tiêu thụ điện kết nối WiFi:
 * - Connecting: 150mA (Đặc biệt tại thời điểm DHCP có thể lên tới 400, 500mA)
 * - Kết nối rồi: 60-70mA
 *
 * Vấn đề: Tắt --> Bật Router --> Không thể kết nối lại (mãi Connecting):
 * - Giải quyết sau khi update Espressif 32 Platform lên bản 6.3.2
 */