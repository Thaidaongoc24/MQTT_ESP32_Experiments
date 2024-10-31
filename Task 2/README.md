## Bố trí thí nghiệm
1. Cài đặt:
- Cài đặt Mosquitto broker từ https://mosquitto.org/ và phần mềm MQTTX trên máy tính để thực hiện các thử nghiệm.
2. Thiết lập kết nối với localhost:
- Mở Command Prompt dưới quyền Administrator.
- Sử dụng lệnh net start mosquitto để khởi động Mosquitto broker.
- Sử dụng lệnh mosquitto -v để theo dõi chi tiết quá trình kết nối, các sự kiện publish và subscribe từ các client.
3. Cấu hình Mosquitto broker:
- Thiết lập các listener cho cổng 1883 và 8883.
- Đặt cấu hình username và password cho localhost.
- Đối với listener trên cổng 8883, tạo self-signed certificate để bảo mật kết nối.
4. Kiểm tra kết nối:
- Sử dụng phần mềm MQTTX để kết nối và kiểm tra các cổng 1883 và 8883 của localhost.

## Kịch bản thí nghiệm
1. Kiểm tra trạng thái kết nối:
- Thiết lập và xác nhận kết nối thành công giữa các client và broker trên các cổng 1883 (không bảo mật) và 8883 (bảo mật bằng chứng chỉ tự ký).
2. Gửi tin nhắn thử nghiệm:
- Thiết lập một client để publish tin nhắn lên topic esp32/echo_test.
3. Theo dõi tin nhắn trong thời gian thực:
- Sử dụng MQTTX để subscribe vào topic esp32/echo_test và kiểm tra rằng các tin nhắn được gửi và nhận ngay lập tức, phản ánh thời gian thực.
4. Kiểm tra với các topic khác:
- Thử nghiệm với nhiều topic khác nhau để xác nhận rằng broker xử lý các tin nhắn một cách ổn định.
- Kiểm tra khả năng hoạt động ổn định của broker trên cả hai cổng 1883 và 8883 để đảm bảo tính tương thích và bảo mật của kết nối.

## Mục tiêu
1. Đánh giá tính ổn định và hiệu suất của Mosquitto broker trên các cổng.
  Thí nghiệm này nhằm xác định khả năng xử lý của Mosquitto broker khi nhận và xử lý tin nhắn từ nhiều cổng khác nhau. Điều này sẽ giúp đánh giá khả năng hoạt động của broker trong các điều kiện khác nhau.
2. Xác nhận tính chính xác của dữ liệu truyền qua MQTT.
  Mục tiêu là đảm bảo rằng các tin nhắn được gửi và nhận một cách chính xác, không bị mất mát trong quá trình truyền tải. Chúng tôi sẽ kiểm tra nội dung của các tin nhắn để đảm bảo rằng chúng không bị thay đổi và đúng như mong đợi.

## Kết quả
1. Kết nối với cổng 1883:
![Hình 1](https://github.com/user-attachments/assets/2672dbbe-c10a-4a6f-ac6b-494791193efd)
 **Hình 1**
- Cổng 1883 là cổng mặc định của MQTT, không yêu cầu mã hóa.
- Kết nối đến MQTT broker Mosquitto đang chạy trên máy chủ cục bộ (localhost) tại cổng 1883.
- Đăng ký nhận tin từ topic mqtt1883.
- Ưu điểm và nhược điểm:
    Dễ dàng thiết lập và sử dụng.
    Không yêu cầu cấu hình chứng chỉ.
    Ít bảo mật hơn so với cổng 8883.
2. Kết nối với cổng 8883:
  ![Hình 2](https://github.com/user-attachments/assets/29138f10-f093-4491-a88b-34d82d6f7d16)
  **Hình 2**
-Cổng 8883 sử dụng giao thức MQTT over SSL/TLS, đảm bảo tính bảo mật cho dữ liệu truyền tải. Thường được sử dụng trong các ứng dụng yêu cầu bảo mật cao.
- Ở **Hình 2** ta setup các thông tin cơ bản: Name, Host, Port, Username, Password
- Và setup các chứng chỉ
- Ưu điểm:
  Bảo mật cao: Dữ liệu được mã hóa, khó bị tấn công và xâm nhập.
  Phù hợp: Với các ứng dụng yêu cầu bảo mật cao như IoT công nghiệp, hệ thống giám sát, nhà thông minh.
![Hình 3](https://github.com/user-attachments/assets/47380dde-3fc5-4806-b69a-436ebaf92c34)
 **Hình 3**
- Kết nối thành công và đã gửi được thông tin

## Kết luận
  Kết quả thử nghiệm bằng MQTTX để kiểm tra khả năng kết nối, gửi và nhận tin nhắn cho thấy Mosquitto broker đáp ứng tốt các yêu cầu về độ tin cậy và hiệu suất, kể cả khi tích hợp các cấu hình bảo mật như TLS. Qua quá trình cấu hình và kiểm thử trên localhost, chúng ta thấy Mosquitto mang lại tính bảo mật và độ chính xác cao khi truyền thông qua các cổng khác nhau.
