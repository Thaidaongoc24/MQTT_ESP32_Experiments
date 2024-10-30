Tôi để ý thấy mã kết nối wifi của tôi khi dùng WiFi.begin(ssid, pass) rồi check if connected thỉnh thoảng nó bị chạy vô hạn sau khi nạp code, cần phải hard reset lại (bấm nút trên board) thì mới kết nối được với wifi AP. 

Trước đây tôi đã gặp điều này vài năm trước và nhớ là nó được giải quyết trong lõi Arduino Core của ESP32 rồi, nghĩa là tôi tin rằng WiFi.begin(...) sẽ tự reconnect khi mất kết nối. 

Vậy cần phải làm một vài thí nghiệm về vấn đề này để tìm ra hướng giải quyết. Nếu không trên thực tế sẽ rất không tốt. 

