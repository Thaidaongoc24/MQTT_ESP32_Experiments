# Các dự án thực hành với giao thức MQTT trên ESP32

Đầu tiên nên cài đặt một client trên PC như EMQX client (MQTTX) và thử kết nối với các dịch vụ MQTT miễn phí để test thử trên PC trước.
Sử dụng thư viện PubSubClient by Nick O’Leary trên ESP32/PIO.

Có 3 thư mục con trong Repo này:

- [Thí nghiệm thô sơ ban đầu](./QoS_Levels_Exp/): đây là lần thử đầu tiên, mã viết rất tùy tiện, mục đích chỉ là để thử cài đặt ban đầu và một số thử nghiệm ngẫu nhiên. Kết quả thí nghiệm không nhiều ý nghĩa vì nó khá giống với lý thuyết cơ bản. Các bạn có thể bỏ qua thí nghiệm này. Việc thiết lập và làm quen với MQTT Broker có thể thực hiện bằng các PC client như EMQX client mà không cần làm trên phần cứng ESP32. 
- [Thí nghiệm mất kết nối vật lý](./WiFi_Disconnect_QoS0_Publishing/): đây là thí nghiệm thành công và được mô tả khá chi tiết trong file README. Các bạn nên đọc và làm theo thí nghiệm này một cách kỹ lưỡng.
- [Thí nghiệm về cơ chế kết nối lại của thư viện lõi WiFi.h trên ESP32](./Wifi_Connect_Experiment/): Thí nghiệm này tôi tự làm, nên tôi không mô tả quá kỹ, các bạn tò mò có thể tự đọc mã nguồn, thật ra nó khá hay, chỉ là không liên quan tới mục đích chính của bài học nên tôi không viết mô tả kỹ.

## Bài tập 

- Hãy thực hiện các đề xuất trong phần đề xuất của thí nghiệm 2 kể trên. 
- Dựa trên mã mẫu của thí nghiệm 2 hãy thiết kế và làm các thí nghiệm để hiểu và ứng dụng khái niệm Last Will Testament (LWT) và KeepAlive.
