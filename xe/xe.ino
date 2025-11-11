#include <PS2X_lib.h>

// Chân điều khiển PS2
#define PS2_CLK 41
#define PS2_CMD 42
#define PS2_ATT 43
#define PS2_DAT 40

// L298_ 23    
// dco BANH2
#define ENA 3   // PWM pin
#define IN1 10  // Digital pin
#define IN2 4  // Digital pin

#define IN3 5  // Digital pin
#define IN4 6  // Digital pin
#define ENB 7   // PWM pin

// L298_14
#define ENA_1 8   // PWM pin=b
#define IN1_1 34  // Digital pin
#define IN2_1 35  // Digital pin
#define IN3_1 36  // Digital pin
#define IN4_1 37  // Digital pin
#define ENB_1 9   // PWM pin

// DCOban
//#define ENA_2 2   // PWM pin
#define IN1_2 11  // Digital pin
#define IN2_2 12  // Digital pin
// cuonb
#define IN3_2 32  // Digital pin
#define IN4_2 33  // Digital pin
#define ENB_2 13   // PWM pin

PS2X ps2x;  // Create an object of the PS2X class

// Biến lưu trữ tốc độ động cơ
int speed = 100; // Tốc độ ban đầu là 100
int speed1 = 165;
int speed2 = 180;
int speed3 = 165;
int speed4 = 210;
const int maxSpeed = 180; // Tốc độ tối đa

unsigned long lastRead = 0; // Biến lưu trữ thời gian lần đọc cuối cùng
const int readDelay = 20; // Thời gian chờ giữa các lần đọc (ms)

void setup() {
  Serial.begin(9600);

  // Thiết lập các chân L298 làm đầu ra
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

// L298_13
  pinMode(ENA_1, OUTPUT);
  pinMode(IN1_1, OUTPUT);
  pinMode(IN2_1, OUTPUT);
  pinMode(IN3_1, OUTPUT);
  pinMode(IN4_1, OUTPUT);
  pinMode(ENB_1, OUTPUT);

 // pinMode(ENA_2, OUTPUT);
  pinMode(IN1_2, OUTPUT);
  pinMode(IN2_2, OUTPUT);
  pinMode(IN3_2, OUTPUT);
  pinMode(IN4_2, OUTPUT);
  pinMode(ENB_2, OUTPUT);

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

  if (ps2x.Button(PSB_PAD_UP)) {
    giubong();
  }
  if (ps2x.Button(PSB_PAD_DOWN)) {
    ban();
  }
  if (ps2x.Button(PSB_PAD_LEFT)) {
    koban();
  }
  if (ps2x.Button(PSB_PAD_RIGHT)) {
    banmanh();
  }

    // Đọc giá trị từ joystick trái
    int joystickY = ps2x.Analog(PSS_LY);
    int joystickX = ps2x.Analog(PSS_LX);
    // Đọc giá trị từ joystick phải
    int joystickRY = ps2x.Analog(PSS_RY);
    int joystickRX = ps2x.Analog(PSS_RX);

  
    if (ps2x.Button(PSB_L1)) {
      speed += 10; // Tăng tốc độ lên 10 đơn vị
      speed1 = 175;
      speed2 = 188;
      speed3 = 180;
      speed4 = 240;
      if (speed > maxSpeed) speed = maxSpeed; // Giới hạn tốc độ tối đa
    }
    if (ps2x.Button(PSB_L2)) {speed -= 10; // Giảm tốc độ xuống 10 đơn vị
      if (speed < 0) speed = 100; // Giới hạn tốc độ tối thiểu
      speed1 = 145;
      speed2 = 155;
      speed3 = 150;
      speed4 = 210;
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

    int speedLY = map(centeredJoystickY, -128, 127, -speed, speed);
    int speedLX = map(centeredJoystickX, -128, 127, -speed, speed);
    int speedRY = map(centeredJoystickRY, -128, 127, -speed, speed);
    int speedRX = map(centeredJoystickRX, -128, 127, -speed, speed);


    if (abs(centeredJoystickY) > 125) {
      if (centeredJoystickY > 125) {
        moveForward(speedLY);
      } else if (centeredJoystickY < -125) {
        backward(-speedLY);
      }
    } else if (abs(centeredJoystickX) > 125) { // Điều khiển động cơ để quay
      if (centeredJoystickX > 125) {
        ngangphai(speedRY);
      } else if (centeredJoystickX < -125) {
        ngangtrai(-speedRY);
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
       xoayphai(speedRY);
      } else if (centeredJoystickRX < -125){
       // cheotrai(-speedRX);
       xoaytrai(-speedRY);
      }
    } else {
      Stop(); // Dừng động cơ
    }
  }
  delay(50);
}

void resetProgram() {
  // Reset specific states of the program
  speed = 100;
}

void backward(int speed) {
  analogWrite(ENA, speed2);
  analogWrite(ENB, speed3);
// dco 3
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW); 
// dco2
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH); 

  analogWrite(ENA_1, speed1);
  analogWrite(ENB_1, speed4);
// dco1
  digitalWrite(IN1_1, HIGH);
  digitalWrite(IN2_1, LOW);  
  
  digitalWrite(IN3_1, HIGH);
  digitalWrite(IN4_1, LOW);  
}

void moveForward(int speed) {
  analogWrite(ENA, speed2);
  analogWrite(ENB, speed3);
// dco 3
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH); 
// dco2
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW); 

  analogWrite(ENA_1, speed1);
  analogWrite(ENB_1, speed4);
// dco1
  digitalWrite(IN1_1, LOW);
  digitalWrite(IN2_1, HIGH);  
  
  digitalWrite(IN3_1, LOW);
  digitalWrite(IN4_1, HIGH); 
}

void ngangphai(int speed) {
  // NGANG TRAI
  analogWrite(ENA, speed2);
  analogWrite(ENB, speed3);
// dco 3
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW); 
// dco2
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW); 

  analogWrite(ENA_1, speed1);
  analogWrite(ENB_1, speed4);
// dco1
digitalWrite(IN1_1, LOW);
digitalWrite(IN2_1, HIGH);  
// dco4
  digitalWrite(IN3_1, HIGH);
  digitalWrite(IN4_1, LOW);
}

void ngangtrai(int speed) {
  analogWrite(ENA, speed2);
  analogWrite(ENB, speed3);
// dco 3
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH); 
// dco2
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH); 

  analogWrite(ENA_1, speed1);
  analogWrite(ENB_1, speed4);
// dco1
  digitalWrite(IN1_1, HIGH);
  digitalWrite(IN2_1, LOW);  
// dco4
  digitalWrite(IN3_1, LOW);
  digitalWrite(IN4_1, HIGH);
}

void xoayphai(int speed) {
  analogWrite(ENA, speed2 - 55);
  analogWrite(ENB, speed3 - 45);
// dco 3
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW); 
// dco2
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH); 

  analogWrite(ENA_1, speed1 - 45);
  analogWrite(ENB_1, speed4 - 55);
// dco1
  digitalWrite(IN1_1, LOW);
  digitalWrite(IN2_1, HIGH);  
// dco4
  digitalWrite(IN3_1, LOW);
  digitalWrite(IN4_1, HIGH);
  
}

void xoaytrai(int speed) {
  analogWrite(ENA, speed2 - 50);
  analogWrite(ENB, speed3 - 45);
// dco 3
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH); 
// dco2
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW); 

  analogWrite(ENA_1, speed1 - 45);
  analogWrite(ENB_1, speed4 - 55);
// dco1
  digitalWrite(IN1_1, HIGH);
  digitalWrite(IN2_1, LOW);  
// dco4
  digitalWrite(IN3_1, HIGH);
  digitalWrite(IN4_1, LOW); 
}

void Stop() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
// dco 3
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW); 
// dco2
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW); 

  analogWrite(ENA_1, 0);
  analogWrite(ENB_1, 0);
// dco1
  digitalWrite(IN1_1, LOW);
  digitalWrite(IN2_1, LOW);  
// dco4
  digitalWrite(IN3_1, LOW);
  digitalWrite(IN4_1, LOW); 
}

void ban() {
  // analogWrite(ENA_2, 100);
  analogWrite(ENB_2, 120);
  analogWrite(IN1_2, 35);
  analogWrite(IN2_2, 35);  
  digitalWrite(IN3_2, LOW);
  digitalWrite(IN4_2, HIGH); 
}

void giubong() {
  // analogWrite(ENA_2, 60);
  analogWrite(ENB_2, 80);
  analogWrite(IN1_2, 0);
  analogWrite(IN2_2, 0);  
  digitalWrite(IN3_2, LOW);
  digitalWrite(IN4_2, HIGH); 
}

void koban() {
  // analogWrite(ENA_2, 60);
  // analogWrite(ENB_2, 80);
  analogWrite(IN1_2, 0);
  analogWrite(IN2_2, 0);  
  digitalWrite(IN3_2, LOW);
  digitalWrite(IN4_2, LOW); 
}

void banmanh() {
  // analogWrite(ENA_2, 100);
  analogWrite(ENB_2, 120);
  analogWrite(IN1_2, 50);
  analogWrite(IN2_2, 50);  
  digitalWrite(IN3_2, LOW);
  digitalWrite(IN4_2, HIGH); 
}