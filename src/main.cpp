#include <Arduino.h>
#include <SoftwareSerial.h>
const byte RX = 2;
const byte TX = 4;
SoftwareSerial mySerial=SoftwareSerial(RX,TX);
String inputString = "";         // a String to hold incoming data
bool stringComplete = false; 



long bien1 = 0;
long bien2 = 0;
int trangthaithietbi1 = 0;
int trangthaithietbi2 = 0;
long last = 0;
String SendEsp = "";
void setup() {
  // initialize serial:
  Serial.begin(9600);
  mySerial.begin(9600);
  
  inputString.reserve(200);
  pinMode(LED_BUILTIN, OUTPUT);
  last = millis();
}
void send_Data()
{
  SendEsp = "";
  //trangthaithietbi1 trang thaithietbi2 bien1 bien2
  SendEsp = "A" + String(trangthaithietbi1) + "B" + String(trangthaithietbi2) + "C" + String(bien1) + "D" + String(bien2) + "E";
  Serial.print("SendEsp: ");
  Serial.println(SendEsp);//GỬI CHUỖI CÓ THÊM KÍ TỰ \n
  mySerial.println(SendEsp);
  mySerial.flush();
  
}
                                               //HAM XỬ LÍ KHI NHẬN DỮ LIỆU TỪ MÁY TÍNH HOẶC ESP8266
void  XuLyOnOff(String inputString)
{
  if (inputString.indexOf("A0B") >= 0)
  {
    Serial.println("OFF thiết bị 1");
    trangthaithietbi1 = 0;
  }
  else if (inputString.indexOf("A1B") >= 0)
  {
    Serial.println("ON thiết bị 1");
    trangthaithietbi1 = 1;
  }
  if (inputString.indexOf("C0D") >= 0)
  {
    Serial.println("OFF thiết bị 2");
    trangthaithietbi2 = 0;
  }
  else if (inputString.indexOf("C1D") >= 0)
  {
    Serial.println("ON thiết bị 2");
    trangthaithietbi2 = 1;
  }
}
void Caidat(String inputString)
{
  /*
    Bước 1: Tìm vị trí kí tự trong chuỗi tổng E F GH
    Bước 2: Cắt chuỗi
    Bước 3: Gắn biến
  */
  int TimE, TimF, TimG, TimH = -1;
  TimE = inputString.indexOf("E");
  TimF = inputString.indexOf("F");
  TimG = inputString.indexOf("G");
  TimH = inputString.indexOf("H");
  if (TimE >= 0 && TimF >= 0)
  {
    String DuLieuEF = "";
    DuLieuEF = inputString.substring(TimE + 1, TimF);
    bien1 = DuLieuEF.toInt();
    Serial.print("Bien1: ");
    Serial.print(bien1);
  }
  if (TimG >= 0 && TimH >= 0)
  {
    String DuLieuGH = "";
    DuLieuGH = inputString.substring(TimG + 1, TimH);
    bien2 = DuLieuGH.toInt();
    Serial.print("Bien2: ");
    Serial.print(bien2);
  }
}
void read_UART()
{
  //Kiểm tra vđk có nhận dữ liệu hay không
  while (Serial.available()) //ABCD12345\n
  {
    // get the new byte:
    char inChar = (char)Serial.read();//Đọc từng byte một
    //inchar=ABCD12345\n
    inputString += inChar;
    //Điều kiện kết thức chuỗi dữ liệu
    if (inChar == '\n') {
      stringComplete = true;
    }


    //Xử lí dữ liệu sau khi đọc
    if (stringComplete) {
      Serial.println("Data nhận");
      Serial.println(inputString);
      if (inputString.indexOf("A") >= 0)//Tồn tại kí tự A trong chuỗi
        {
        digitalWrite(LED_BUILTIN, HIGH);
        }
        else
        {
        digitalWrite(LED_BUILTIN, LOW);
        }
      if(inputString.indexOf("LM")>=0)
      {
        send_Data();
      }
      XuLyOnOff(String(inputString));
      Caidat(String(inputString));
      inputString = "";
      stringComplete = false;
    }
  }
}

void loop() {

  read_UART();
  
  if (millis() - last >= 1000)
  {
    
    last = millis();
  }
}
                                                //HÀM SEND DATA ĐẾN ESP8266//
