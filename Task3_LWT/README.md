## Bố trí thí nghiệm 

- Dùng thư viện PubSubClient trên ESP32 kết nối với MQTT Broker HiveMQ.
- Sử dụng thư viện Ticker, một thư viện chuẩn trong Arduino để gọi hàm publish một cách đều đặn và bất đồng bộ, mỗi giây (1s) một lần:
    + Mã: `mqttPulishTicker.attach(1, mqttPublish)`
    + Tài liệu về Ticker: https://docs.arduino.cc/libraries/ticker/, https://github.com/espressif/arduino-esp32/blob/master/libraries/Ticker/src/Ticker.h 
- Subscribe tới topic `esp32/echo_test` ngay sau khi MQTT connect thành công
- Gọi hàm `mqttClient.loop()` trong main loop để handle các thông điệp nhận được từ broker (bất đồng bộ, event driven) bất kỳ lúc nào. 
- Phát hiện mất kết nối MQTT `if (!mqttClient.connected())` trong main loop để kết nối lại `mqttReconnect()` ngay khi phát hiện mất kết nối.
- Khi client bị mất kết nối, sau khoảng thời gian 20s sẽ thông báo "Lost Connection" về topic; khi client được kết nối sẽ có thông báo "online"

## Kịch bản thí nghiệm

- Sau khi ESP32 khởi động, sẽ kết nối WiFi vào một điểm phát AP đã định (ssid, và pass trong secrets/wifi.h) --> thành công
- Sẽ thấy MQTT Client kết nối đến broker thành công và bắt đầu gửi (publish) và nhận (subscribe) số đếm tăng dần trong `echo_topic` đều đặn
- Khi đó sẽ tiến hành ngắt điểm phát WiFi, tiện nhất là phát wifi từ điện thoại để bật ngắt nó nhanh chóng trong tầm tay
- Quan sát phản ứng của MQTT Client trong mã khi mất kết nối WiFi giữa chừng, 
- Sau đó bật lại điểm phát WiFi và quan sát khả năng khôi phục kết nối, và quan sát việc mất gói tin trong quá trình kết nối.

## Mục đích 
- Xem việc ngắt kết nối từ bên dưới chồng Internet Protocol có ảnh hưởng tới lớp trên không. Ở đây là lớp WiFi (link layer) bị ngắt --> lớp TCP/IP bị ngắt --> có ảnh hưởng tới lớp ứng dụng MQTT trên cùng hay không? ESP core lib sẽ in ra thông điệp lỗi như nào (có báo lỗi từ lớp dưới lên lớp bên trên hay không?)
- Quan sát sự bỏ mặc việc mất thông điệp trong QoS = 0. 
- Hiểu rõ hơn về cơ chế hoạt động của MQTT client bên trên tầng TCP/IP, nhất là cơ chế phát hiện mất kết nối và khôi phục kết nối ở lớp vật lý, rất hay xảy ra trong thực tế.
- Thực hiện thí nghiệm để hiểu rõ hơn về Last Will and Testament LWT

## Kết quả:
1. Thiết lập kết nối với HiveMQ:
- Sử dụng HiveMQ Web Client để quan sát dữ liệu được gửi đến topic 'esp32/echo_test'

![Hình 1](![image](https://github.com/user-attachments/assets/46012d87-bb58-45cb-a733-4a573d743347)
    **Hình 1**

- Sau khi kết nối, trạng thái của esp sẽ được hiển thị trước khi dữ liệu bắt đầu nhận:
  ![Hình 2](https://github.com/user-attachments/assets/c33bc2d3-4071-40bc-ba89-c6e6fa69c665) 
    **Hình 2**
    Trước khi kết nối thành công: Ngay cả khi MQTT chưa kết nối hoàn toàn, các thông điệp (0, 1, 2) đã được "publish" thành công. Điều này có thể là do hệ thống cho phép lưu các thông điệp vào hàng đợi tạm thời, hoặc thiết bị đã có khả năng gửi thông điệp trước khi kiểm tra trạng thái kết nối với broker.
    Kết nối WiFi: Quá trình kết nối WiFi diễn ra trước khi kết nối với MQTT. Sau khi WiFi kết nối thành công, thiết bị bắt đầu cố gắng thiết lập kết nối với MQTT broker.
    Thời gian thiết lập kết nối MQTT: Mất khoảng 3 giây để thiết lập kết nối MQTT sau khi WiFi đã được kết nối. Trong thời gian này, thiết bị gửi đi một thông điệp mỗi giây
2. Đột ngột gắt kết nối mạng của esp32 và cung cấp trở lại:
  ![Hình 3](https://github.com/user-attachments/assets/e8bea639-2d83-495e-9265-b881e7f8a8f6)
    **Hình 3**
- Gửi Last Will message "Lost Connection": Khi ESP32 mất kết nối mạng, MQTT broker sẽ tự động gửi Last Will message là "Lost Connection" đến topic echo_topic thông báo rằng thiết bị đã ngắt kết nối.
- Kết nối lại và gửi thông báo "online": Ngay khi kết nối mạng được khôi phục, ESP32 sẽ gửi thông điệp "online" đến topic echo_topic. Điều này giúp MQTT broker và các client khác biết rằng thiết bị đã trở lại trạng thái hoạt động.
- Tiếp tục gửi dữ liệu: Sau khi gửi thông báo "online", ESP32 sẽ bắt đầu tiếp tục gửi các thông điệp dữ liệu như thông thường. Các thông điệp này sẽ được publish lên echo_topic ngay sau thông báo "online".

## Kết luận
- Kết quả thí nghiệm cho thấy Broker HiveMQ hoạt động ổn định và đúng như mong đợi trong việc duy trì trạng thái và đảm bảo tính tin cậy của hệ thống MQTT, đáp ứng tốt nhu cầu giám sát trạng thái trong các ứng dụng IoT.
- Thí nghiệm đã chứng minh rằng tính năng Last Will and Testament (LWT) của HiveMQ có khả năng giám sát trạng thái của client một cách hiệu quả.
- Nhờ vào cấu hình keepAlive và socketTimeout, hệ thống có thể cập nhật chính xác trạng thái kết nối của ESP32 (online/offline) trên topic theo thời gian thực, mang lại độ chính xác và ổn định cao trong việc giám sát trạng thái thiết bị.
