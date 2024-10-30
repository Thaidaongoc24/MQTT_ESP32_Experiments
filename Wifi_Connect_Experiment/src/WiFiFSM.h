/**
 * Tested on ESP32 / arduino-core / espressif-idf (6.3.2@PIO)
 * When router off/on --> auto-reconnecting by the core library.
 * Very simple interface to use. For usage see comment in the end of this file.
 * Written by Nguyen Anh Tuan (June 2023)
 *
 * July 2023:
 * - added `int getState()` function that returns WiFiFMS::States
 */

#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include "LED.h"

class WiFiFSM
{
private:
    LED &_led;
    static constexpr int LED_DURATION = 500;
    const char *_ssid;
    const char *_pass;
    enum States
    {
        DISCONNECTED,
        CONNECTING,
        CONNECTED,
    } state = DISCONNECTED;

public:
    WiFiFSM(const char *SSID, const char *PASS, LED &led) : _ssid(SSID), _pass(PASS), _led(led){};
    void on();
    void off();
    void loop();
    int getState() { return state; }
};

void WiFiFSM::on()
{
    if (state == DISCONNECTED)
    {
        WiFi.disconnect(false); // turn Wi-Fi radio on
        WiFi.mode(WIFI_STA);
        WiFi.begin(_ssid, _pass);
        state = CONNECTING;
        _led.blink(LED_DURATION);
    }
}

void WiFiFSM::off()
{
    WiFi.disconnect(true); // turn Wi-Fi radio off
    WiFi.mode(WIFI_OFF);
    state = DISCONNECTED; // Confident to change the state!
    _led.off();
}

void WiFiFSM::loop()
{
    switch (state)
    {
    case DISCONNECTED:
    case CONNECTING:
        if (WiFi.status() == WL_CONNECTED)
        {
            state = CONNECTED;
            _led.on();
            Serial.println("Connected to Wi-Fi");
        }
        _led.loop();
        break;

    case CONNECTED:
        if (WiFi.status() != WL_CONNECTED)
        {
            state = CONNECTING; // auto-reconnecting ...
            _led.blink(LED_DURATION);
            Serial.println("Lost Wi-Fi connection");
        }
        break;
    }
}

/*** Usage ***
#include <Arduino.h>
#include "utils/WiFiFSM.h"
#include "utils/LED.h"

LED led(LED_PIN, LED_ACT);
WiFiFSM wifi(WF_SSID, WF_PASS, led);

void setup()
{
    wifi.on();
}

void loop()
{
    wifi.loop();
}
*/

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