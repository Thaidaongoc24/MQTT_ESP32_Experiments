## Mô phỏng phần cứng ESP32 sử dụng Wokwi trên VS code

1. Cài đặt Wokwi extension cho VS Code: https://marketplace.visualstudio.com/items?itemName=wokwi.wokwi-vscode 
2. Kích hoạt tài khoản (tạo tài khoản Wokwi trên web và login trên web trước): https://docs.wokwi.com/vscode/getting-started/#installation 
3. Tạo một dự án ESP32 mới (New Project --> ESP32/Arduino Template) trên [Wokwi Web](https://wokwi.com/projects/new).
4. Copy file diagram.json về thư mục chủ của dự án pio (xem ví dụ trong repo này). 
5. Biên soạn file wokwi.toml: các đường dẫn relative tới file firmware và file elf: xem file `wokwi.toml` mẫu trong dự án này. 
6. Biên dịch bằng: pio --> build --> (tạo ra file .bin và .elf như trong .pio/)
7. Bấm vào file diagram.json: extension Wokwi trên vs code sẽ tự động hiển thị sơ đồ phần cứng mô phỏng, Bấm nút tam giác xanh (start simulation) --> Chạy mô phỏng, nó sẽ tự động load firmware đã biên dịch trong .pio và chạy, ở bên dưới màn hình sẽ tự động hiện lên Wokwi terminal mô phỏng giao tiếp Serial với máy tính y như phần cứng thật. 
8. Mô phỏng kết nối mạng trên Wokwi: Wokwi trên vs code sẽ tận dụng mạng trên chính máy tính của bạn để mô phỏng kết nối internet cho con ESP32. Bạn cần sử dụng wifi SSID là "Wokwi-GUEST" với Password là trống "". Mã kết nối trên Arduino: `WiFi.begin("Wokwi-GUEST", "")`. Tài liệu: https://docs.wokwi.com/guides/esp32-wifi/ 

Nhận xét: 
- Tài khoản miễn phí: sau khi kích hoạt như bước 2 ở trên sẽ được một tài khoản "community" dùng wokwi miễn phí, khi nào cần thì nó sẽ đòi kích hoạt lại và tiếp tục dùng (1 tháng). 
- Dùng Wokwi trên vs code sẽ tiện hơn trên web vì có thể viết mã trên VS Code với tất cả các tiện ích gợi ý mã, và công cụ trên PIO. Cái này sẽ phù hợp với việc chia nhỏ mã nguồn thành các module, trên web làm việc này rất khó khăn. Thêm một lợi ích nữa là việc cài đặt thư viện cộng đồng qua PIO sẽ đầy đủ và dễ dàng hơn trên Wokwi web.
- Mô phỏng kết nối mạng Wokwi trên VS code: bạn có thể cho con ESP32 kết nối với mạng quay chính máy tính của bạn, không như trên web nó sẽ kết nối mạng qua server của Wokwi (khá chậm) và phải trả phí. 
- Lợi ích của Wokwi trên VS code: Giúp bạn test mã nhanh hơn, không mất thời gian bố trí phần cứng, cắm cap, và thời gian nạp mã vào phần cứng thật. Điều này rút ngắn quá trình thí nghiệm/thử nghiệm ý tưởng, và tăng tốc quá trình học tập code rất nhiều (rút ngắn thời gian phản hồi trong quá trình học). Tuy nhiên đừng có lạm dụng quá, vẫn cần phải tương tác với phần cứng thật ở một bước nào đó sẽ giúp bạn giỏi lên nhiều hơn là dùng mô phỏng (bạn nên sẵn sàng làm hỏng phần cứng trong trường hợp xấu) --> điều này giúp các bạn tích lũy được nhiều kinh nghiệm quý báu với phần cứng. 
- Hạn chế hiện nay: Giấy phép miễn phí không edit được diagram trực tiếp trên VS code. Cách khắc phục: edit trên web rồi copy nội dung của file diagram.json vào file đó trên VS code (cần phải chuột phải vào file `digram.json` trên VS code sau đó chọn `Open With` và chọn default editor).

- Thực chất: các bạn có thể học được khá nhiều thông qua các project mẫu có sẵn trên Wokwi. Nên bỏ thời gian xem mã và tự chạy demo của riêng bạn --> thấu hiểu từng dự án từ dễ đến khó, từ cơ bản đến phức tạp, hãy bắt đầu bằng những dự án đơn giản nhất. Dự kiến của tôi: sẽ tạo ra một danh sách theo thứ tự các dự án mẫu cần tìm hiểu cho người mới học trên Wokwi.
