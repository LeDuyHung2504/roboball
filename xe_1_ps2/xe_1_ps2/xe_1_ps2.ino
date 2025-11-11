#include <PS2X_lib.h>

// Chân điều khiển PS2
#define PS2_CLK 32
#define PS2_ATT 33
#define PS2_CMD 22
#define PS2_DAT 23

int tg_phanh = 0;
int tang = 0;
int max_tang = 50;
int phanh = 0;
int ban = 0;

int LPWM1 = 2, RPWM1 = 3;
int LPWM2 = 4, RPWM2 = 5;
int LPWM3 = 7, RPWM3 = 8;
int LPWM4 = 10, RPWM4 = 9;

int Attach1 = 24;

PS2X ps2x;  // Create an object of the PS2X class



unsigned long lastRead = 0; // Biến lưu trữ thời gian lần đọc cuối cùng
const int readDelay = 20; // Thời gian chờ giữa các lần đọc (ms)

void setup() {
  Serial.begin(9600);

  // Khởi động tay cầm PS2
  int error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_ATT, PS2_DAT);

  if (error == 0) {
    Serial.println("Tay cầm PS2 đã được kết nối thành công.");
  } else {
    Serial.print("Không thể kết nối tay cầm PS2. Lỗi: ");
    Serial.println(error);
  }
}

void loop() {
  unsigned long currentMillis = millis();
  
  // Kiểm tra thời gian đọc
  if (currentMillis - lastRead >= readDelay) {
    lastRead = currentMillis;

    ps2x.read_gamepad(); // Đọc dữ liệu từ tay cầm PS2

    if (ps2x.ButtonPressed(PSB_START)) {
    }

    // Check if SELECT button is pressed
    if (ps2x.ButtonPressed(PSB_SELECT)) {
    }   

  // if (ps2x.Button(PSB_PAD_UP)) {
  //   //giubong();
  // }
  // if (ps2x.Button(PSB_PAD_DOWN)) {
  //   ban();
  //   ban = 1;
  // }
  // else{
  //   ban = 0;
  // }
  // if (ps2x.Button(PSB_PAD_LEFT)) {
  //   //koban();
  // }
  // if (ps2x.Button(PSB_PAD_RIGHT)) {
  //   //banmanh();
  // }

if (ps2x.Button(PSB_CROSS)) { 
  ban_bong();
  ban = 1;

}
  // Code for when X button is pressed } 
// if (ps2x.Button(PSB_CIRCLE)) { 
//   // Code for when O button is pressed } 
// if (ps2x.Button(PSB_TRIANGLE)) { 
//   // Code for when triangle button is pressed }
// if (ps2x.Button(PSB_SQUARE))


    // Đọc giá trị từ joystick trái
    int joystickY = ps2x.Analog(PSS_LY);
    int joystickX = ps2x.Analog(PSS_LX);
    // Đọc giá trị từ joystick phải
    int joystickRY = ps2x.Analog(PSS_RY);
    int joystickRX = ps2x.Analog(PSS_RX);

    if (ps2x.Button(PSB_L1)) {
      tang += 10; // Tăng tốc độ lên 10 đơn vị
      if (tang > max_tang) tang = max_tang; // Giới hạn tốc độ tối đa
    }
    if (ps2x.Button(PSB_L2)) {tang -= 10; // Giảm tốc độ xuống 10 đơn vị
      if (tang < 0) tang = 0; // Giới hạn tốc độ tối thiểu
    }

    // Chuyển đổi giá trị joystick (0-255) thành tốc độ động cơ (-128 đến 127)
    int centeredJoystickY = joystickY - 128;
    int centeredJoystickX = joystickX - 128;
    int centeredJoystickRY = joystickRY - 128;
    int centeredJoystickRX = joystickRX - 128;
    Serial.print(centeredJoystickX);
    Serial.print("      ");

    Serial.print(centeredJoystickY);
    Serial.print("      ");
    Serial.print(centeredJoystickRY);
    Serial.print("      ");
    Serial.println(centeredJoystickRX);

    // int speedLY = map(centeredJoystickY, -128, 127, -speed, speed);
    // int speedLX = map(centeredJoystickX, -128, 127, -speed, speed);
    // int speedRY = map(centeredJoystickRY, -128, 127, -speed, speed);
    // int speedRX = map(centeredJoystickRX, -128, 127, -speed, speed);


    if (abs(centeredJoystickY) > 125) {
      if (centeredJoystickY > 125) {
        moveBackward();
        phanh = 1;
        tg_phanh +=1;
        if(tg_phanh == 200) tg_phanh = 200;
      } else if (centeredJoystickY < -125) {
        moveForward();
        phanh = 2;
        tg_phanh +=1;
        if(tg_phanh == 200) tg_phanh = 200;
      }
    } else if (abs(centeredJoystickX) > 125) { // Điều khiển động cơ để quay
      if (centeredJoystickX > 125) {
        sang_phai();
        phanh = 3;
        tg_phanh +=1;
        if(tg_phanh == 100) tg_phanh = 100;
      } else if (centeredJoystickX < -125) {
        sang_trai();
        phanh = 4;
        tg_phanh +=1;
        if(tg_phanh == 100) tg_phanh = 100;
      }
    } else if (abs(centeredJoystickRY) > 125) {
      if (centeredJoystickRY > 125) {
        //ngangphai(speedRY);
      } else if (centeredJoystickRY < -125){
       // ngangtrai(-speedRY);
      }
    } else if (abs(centeredJoystickRX) > 125) {
      if (centeredJoystickRX > 125) {
       // cheophai(speedRX);
       xoay_phai();
       phanh = 5;
       tg_phanh +=1;
        if(tg_phanh == 100) tg_phanh = 100;
      } else if (centeredJoystickRX < -125){
       // cheotrai(-speedRX);
       xoay_trai();
       phanh = 6;
       tg_phanh +=1;
        if(tg_phanh == 100) tg_phanh = 100;
      }
    } else {
      moveStop(); // Dừng động cơ
    }
  }
  delay(50);
}

// void resetProgram() {
//   // Reset specific states of the program
//   speed = 100;
// }

void moveStop() {
    Serial.println("moveStop");
    analogWrite(LPWM1, 0); analogWrite(RPWM1, 0);
    analogWrite(LPWM2, 0); analogWrite(RPWM2, 0);
    analogWrite(LPWM3, 0); analogWrite(RPWM3, 0);
    analogWrite(LPWM4, 0); analogWrite(RPWM4, 0);
    Phanh(phanh);
}

void moveForward() {
    int speed = 160 + tang;
    analogWrite(LPWM1, 0); analogWrite(RPWM1, speed);
    analogWrite(LPWM2, 0); analogWrite(RPWM2, speed +20);
    analogWrite(LPWM3, 0); analogWrite(RPWM3, speed);
    analogWrite(LPWM4, 0); analogWrite(RPWM4, speed + 10);
}

void moveBackward() {
    //int speed = map(dc0, 0, 126, 160, 50);
    int speed = 160 + tang;
    Serial.println("moveBackward");
    analogWrite(LPWM1, speed); analogWrite(RPWM1, 0);
    analogWrite(LPWM2, speed + 20); analogWrite(RPWM2, 0);
    analogWrite(LPWM3, speed); analogWrite(RPWM3, 0);
    analogWrite(LPWM4, speed + 10); analogWrite(RPWM4, 0);
}

void sang_phai() {
    //int speed = map(dc1, 128, 254, 50, 150);
    Serial.println("sang_phai");
    int speed  = 120 + tang;
    analogWrite(LPWM1, 0); analogWrite(RPWM1, speed + 5);
    analogWrite(LPWM2, speed + 20); analogWrite(RPWM2, 0);
    analogWrite(LPWM3, speed); analogWrite(RPWM3, 0);
    analogWrite(LPWM4, 0); analogWrite(RPWM4, speed + 10);
}

void sang_trai() {
    //int speed = map(dc1, 0, 126, 150, 50);
    Serial.println("sang_trai");
    int speed = 120 + tang;
    analogWrite(LPWM1, speed + 5); analogWrite(RPWM1, 0);
    analogWrite(LPWM2, 0); analogWrite(RPWM2, speed);
    analogWrite(LPWM3, 0); analogWrite(RPWM3, speed);
    analogWrite(LPWM4, speed + 20); analogWrite(RPWM4, 0);
}

void xoay_phai() {
    //int speed = map(dc3, 0, 125, 80, 30);
    Serial.println("xoay_phai");
    int speed = 70 + tang;
    analogWrite(LPWM1, 0); analogWrite(RPWM1, speed);
    analogWrite(LPWM2, speed + 20); analogWrite(RPWM2, 0);
    analogWrite(LPWM3, 0); analogWrite(RPWM3, speed);
    analogWrite(LPWM4, speed + 10); analogWrite(RPWM4, 0);
}

void xoay_trai() {
    //int speed = map(dc3, 127, 254, 30, 80);
     Serial.println("xoay_trai");
    int speed = 70 + tang;
    analogWrite(LPWM1, speed); analogWrite(RPWM1, 0);
    analogWrite(LPWM2, 0); analogWrite(RPWM2, speed +20);
    analogWrite(LPWM3, speed); analogWrite(RPWM3, 0);
    analogWrite(LPWM4, 0); analogWrite(RPWM4, speed + 10);
}

void Phanh(int a)
{
  if(a == 1)
  {
    gt_moveBackward();
  }
  else if(a == 2)
  {
    gt_moveForward();
  }
  else if(a == 3)
  {
    gt_sang_phai();
  }
  else if(a == 4)
  {
    gt_sang_trai();
  }
  else if(a == 5)
  {
    gt_xoay_phai();
  }
  else if(a == 6)
  {
    gt_xoay_trai();
  }
}


//////////////////// giam toc khi dung ////////////////////


void gt_moveBackward() {
    //int speed = map(dc0, 128, 254, 50, 150);
    int speed = 50;
    Serial.print(speed); Serial.println(" moveForward");

    analogWrite(LPWM1, 0); analogWrite(RPWM1, speed);
    analogWrite(LPWM2, 0); analogWrite(RPWM2, speed);
    analogWrite(LPWM3, 0); analogWrite(RPWM3, speed);
    analogWrite(LPWM4, 0); analogWrite(RPWM4, speed);
    delay(tg_phanh);
    phanh = 0;
    tg_phanh = 0;
}

void gt_moveForward() {
    //int speed = map(dc0, 0, 126, 150, 50);
    int speed = 50;
    Serial.println("moveBackward");

    analogWrite(LPWM1, speed); analogWrite(RPWM1, 0);
    analogWrite(LPWM2, speed); analogWrite(RPWM2, 0);
    analogWrite(LPWM3, speed); analogWrite(RPWM3, 0);
    analogWrite(LPWM4, speed); analogWrite(RPWM4, 0);
    delay(tg_phanh);
    phanh = 0;
    tg_phanh = 0;
}

void gt_sang_trai() {
    //int speed = map(dc1, 128, 254, 50, 100);
    int speed = 30;
    Serial.println("sang_phai");

    analogWrite(LPWM1, 0); analogWrite(RPWM1, speed);
    analogWrite(LPWM2, speed); analogWrite(RPWM2, 0);
    analogWrite(LPWM3, speed); analogWrite(RPWM3, 0);
    analogWrite(LPWM4, 0); analogWrite(RPWM4, speed);
    delay(tg_phanh);
    phanh = 0;
    tg_phanh = 0;
}

void gt_sang_phai() {
    //int speed = map(dc1, 0, 126, 100, 50);
    int speed = 30;
    Serial.println("sang_trai");

    analogWrite(LPWM1, speed); analogWrite(RPWM1, 0);
    analogWrite(LPWM2, 0); analogWrite(RPWM2, speed);
    analogWrite(LPWM3, 0); analogWrite(RPWM3, speed);
    analogWrite(LPWM4, speed); analogWrite(RPWM4, 0);
    delay(tg_phanh);
    phanh = 0;
    tg_phanh = 0;
}
void gt_xoay_trai() {
    //int speed = map(dc3, 0, 125, 100, 0);
    int speed = 40;
    Serial.println("xoay_phai");

    analogWrite(LPWM1, 0); analogWrite(RPWM1, speed);
    analogWrite(LPWM2, speed); analogWrite(RPWM2, 0);
    analogWrite(LPWM3, 0); analogWrite(RPWM3, speed);
    analogWrite(LPWM4, speed); analogWrite(RPWM4, 0);
    delay(tg_phanh);
    phanh = 0;
    tg_phanh = 0;
}

void gt_xoay_phai() {
    //int speed = map(dc3, 127, 254, 10, 50);
    int speed = 40;
    Serial.println("xoay_trai");

    analogWrite(LPWM1, speed); analogWrite(RPWM1, 0);
    analogWrite(LPWM2, 0); analogWrite(RPWM2, speed);
    analogWrite(LPWM3, speed); analogWrite(RPWM3, 0);
    analogWrite(LPWM4, 0); analogWrite(RPWM4, speed);
    delay(tg_phanh);
    phanh = 0;
    tg_phanh = 0;
}


///////////////////// bắn ///////////////////////////

void ban_bong()
{
if(ban == 1)
{
 digitalWrite(Attach1, HIGH);
 delay(160);
 digitalWrite(Attach1, LOW);
 ban = 0;
}
}