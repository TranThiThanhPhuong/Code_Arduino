// HC-05 của Minh ADDR: 0022:12:018E86 (Master)
// HC-05 của Chung ADDR: 98D3:41:F6571A (Slave)

/*
 ***Các lệnh cơ bản
 *
  AT: Kiểm tra kết nối
  AT+ORGL: Reset lại cài đặt mặc định
  AT+RMAAD: Xóa mọi thiết bị đã ghép nối
  AT+UART?
  AT+UART=9600,0,0
  AT+NAME?
  AT+PSWD?
  
 ***Cài đặt cho SLAVE***
  AT: Lệnh test, nó sẽ trả về OK nếu module đã hoạt động ở Command Mode
  AT+RMAAD: Xóa mọi thiết bị đã ghép nối
  AT+UART=38400,0,0 ( thiết lập baudrate Bluetooth 34800 ; baud của SLAVE và MASTER phải giống nhau)
  AT+PSWD=1234 (đặt mật khẩu cho Slave là 1234, MK của SLAVE và MASTER phải giống nhau)
  AT+ROLE=0: Đặt là chế độ SLAVE
  AT+ADDR?: Hiển thị địa chỉ của SLAVE. Ví dụ địa chỉ là 123:34:232421 thì đổi dấu ":" thành dấu "," để nhập vào MASTER

 ***Cài đặt cho MASTER***
  AT: Lệnh test, nó sẽ trả về OK nếu module đã hoạt động ở Command Mode
  AT+RMAAD : ngắt kết nối với các thiết bị đã ghép
  AT+UART=38400,0,0 ( thiết lập baudrate Bluetooth 9600 ; baud của SLAVE và MASTER phải giống nhau)
  AT+PSWD=1234 (đặt mật khẩu cho Slave là 1234, MK của SLAVE và MASTER phải giống nhau)
  AT+ROLE=1 : đặt là module ở MASTER
  AT+CMODE=0: Cho phép kết nối với bất kì địa chỉ nào
  AT+BIND=98D3,41,F6571A Ràng buộc Master với Slave
  AT+LINK=98D3,41,F6571A Kết nối Master với Slave
  
 
 */


// D10 => HC-05 TX
// D11 => HC-05 RX
// 5V  => HC-05 VCC
// GND => HC-05 GND
// D9  => HC-05 EN
#include <SoftwareSerial.h>
SoftwareSerial BTSerial(10, 11); // RX | TX

void setup()
{
  pinMode(9, OUTPUT);
  digitalWrite(9, HIGH);

  Serial.begin(38400);
  Serial.println("Enter AT commands");
  BTSerial.begin(38400);
}

void loop() 
{
  if (Serial.available())
    BTSerial.write(Serial.read());
  if (BTSerial.available()) 
    Serial.write(BTSerial.read());
}
