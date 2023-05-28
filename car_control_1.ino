#include Arduino.h
#include SPI.h
#include Wire.h
#include Adafruit_GFX.h
#include Adafruit_SSD1306.h
#include Wire.h
#include INA226.h
#define SCREEN_WIDTH 128  OLED display width, in pixels
#define SCREEN_HEIGHT 32  OLED display height, in pixels

#define OLED_RESET     28
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Define python input
String FaceRec;

Define sonar pins
#define echoPin1 22
#define trigPin1 24
#define echoPin2 52
#define trigPin2 53

 Define motor pins
#define PWMA 12    Motor A PWM
#define DIRA1 34
#define DIRA2 35  Motor A Direction
#define PWMB 8    Motor B PWM
#define DIRB1 37
#define DIRB2 36  Motor B Direction
#define PWMC 9   Motor C PWM
#define DIRC1 43
#define DIRC2 42  Motor C Direction
#define PWMD 5    Motor D PWM
#define DIRD1 A4  26  
#define DIRD2 A5  27  Motor D Direction

#define MOTORA_FORWARD(pwm)    do{digitalWrite(DIRA1,LOW); digitalWrite(DIRA2,HIGH);analogWrite(PWMA,pwm);}while(0)
#define MOTORA_STOP(x)         do{digitalWrite(DIRA1,LOW); digitalWrite(DIRA2,LOW); analogWrite(PWMA,0);}while(0)
#define MOTORA_BACKOFF(pwm)    do{digitalWrite(DIRA1,HIGH);digitalWrite(DIRA2,LOW); analogWrite(PWMA,pwm);}while(0)

#define MOTORB_FORWARD(pwm)    do{digitalWrite(DIRB1,LOW); digitalWrite(DIRB2,HIGH);analogWrite(PWMB,pwm);}while(0)
#define MOTORB_STOP(x)         do{digitalWrite(DIRB1,LOW); digitalWrite(DIRB2,LOW); analogWrite(PWMB,0);}while(0)
#define MOTORB_BACKOFF(pwm)    do{digitalWrite(DIRB1,HIGH);digitalWrite(DIRB2,LOW); analogWrite(PWMB,pwm);}while(0)

#define MOTORC_FORWARD(pwm)    do{digitalWrite(DIRC1,LOW); digitalWrite(DIRC2,HIGH);analogWrite(PWMC,pwm);}while(0)
#define MOTORC_STOP(x)         do{digitalWrite(DIRC1,LOW); digitalWrite(DIRC2,LOW); analogWrite(PWMC,0);}while(0)
#define MOTORC_BACKOFF(pwm)    do{digitalWrite(DIRC1,HIGH);digitalWrite(DIRC2,LOW); analogWrite(PWMC,pwm);}while(0)

#define MOTORD_FORWARD(pwm)    do{digitalWrite(DIRD1,LOW); digitalWrite(DIRD2,HIGH);analogWrite(PWMD,pwm);}while(0)
#define MOTORD_STOP(x)         do{digitalWrite(DIRD1,LOW); digitalWrite(DIRD2,LOW); analogWrite(PWMD,0);}while(0)
#define MOTORD_BACKOFF(pwm)    do{digitalWrite(DIRD1,HIGH);digitalWrite(DIRD2,LOW); analogWrite(PWMD,pwm);}while(0)

int Motor_PWM = 20;
int Motor_PWMA = Motor_PWM+3;
int Motor_PWMB = Motor_PWM+5;
int Motor_PWMC = Motor_PWM;
int Motor_PWMD = Motor_PWM-1;
long distance_in_cm_1;
long distance_in_cm_2;
long duration1;
long duration2;
long power_check;

void ADVANCE()
{
  MOTORA_FORWARD(Motor_PWMA); 
  MOTORB_BACKOFF(Motor_PWMB);
  MOTORC_FORWARD(Motor_PWMC); 
  MOTORD_BACKOFF(Motor_PWMD);
}

void BACK()
{
  MOTORA_BACKOFF(Motor_PWMA); 
  MOTORB_FORWARD(Motor_PWMB);
  MOTORC_BACKOFF(Motor_PWMC); 
  MOTORD_FORWARD(Motor_PWMD);
}

void RIGHT()
{
  MOTORA_FORWARD(Motor_PWM); 
  MOTORB_FORWARD(Motor_PWM);
  MOTORC_BACKOFF(Motor_PWM); 
  MOTORD_BACKOFF(Motor_PWM);
}

void LEFT()
{
  MOTORA_BACKOFF(Motor_PWM); 
  MOTORB_BACKOFF(Motor_PWM);
  MOTORC_FORWARD(Motor_PWM); 
  MOTORD_FORWARD(Motor_PWM);
}

void rotate_clockwise()
{
  MOTORA_BACKOFF(Motor_PWMA); 
  MOTORB_BACKOFF(Motor_PWMB);
  MOTORC_BACKOFF(Motor_PWMC); 
  MOTORD_BACKOFF(Motor_PWMD);
}

void rotate_anticlockwise()
{
  MOTORA_FORWARD(Motor_PWMA);
  MOTORB_FORWARD(Motor_PWMB);
  MOTORC_FORWARD(Motor_PWMC);
  MOTORD_FORWARD(Motor_PWMD);
}

void STOP()
{
  MOTORA_STOP(Motor_PWMA);
  MOTORB_STOP(Motor_PWMB);
  MOTORC_STOP(Motor_PWMC);
  MOTORD_STOP(Motor_PWMD);
}

void measure_distance_sonar_1() { 
  digitalWrite(trigPin1, LOW); 
  delayMicroseconds(2); 
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10); 
  digitalWrite(trigPin1, LOW); 
  duration1 = pulseIn(echoPin1, HIGH);
  distance_in_cm_1 = (duration2.0)  29.1;
}
void measure_distance_sonar_2() { 
  digitalWrite(trigPin2, LOW); 
  delayMicroseconds(2); 
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10); 
  digitalWrite(trigPin2, LOW); 
  duration2 = pulseIn(echoPin2, HIGH);
  distance_in_cm_2 = (duration2.0)  29.1;
}

void printing_power()
{
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print(Current );
  display.print(ina.readShuntCurrent(), 5);
  display.println( A);
  display.display();
  dealy(3000);

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(Bus voltage   );
  display.print(ina.readBusVoltage(), 5)
  display.println( V);
  display.display();
  dealy(3000);

  display.clearDisplay();
  display.setCursor(0, 0);
  display.print(Bus power     );
  display.print(ina.readBusPower(), 5);
  display.println( W);
  display.display();
  dealy(3000);
}

void setup()
{
  Serial.begin(9600);
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  servo_pan.attach(48);
  servo_tilt.attach(47);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  Address 0x3C for 128x32
    Serial.println(F(SSD1306 allocation failed));
  }
  display.clearDisplay();
  display.setTextSize(2);       Normal 11 pixel scale
  display.setTextColor(SSD1306_WHITE);  Draw white text
  display.cp437(true);          Use full 256 char 'Code Page 437' font
  display.setCursor(0, 0);      Start at top-left corner
  ina.begin();
  ina.configure(INA226_AVERAGES_16, INA226_BUS_CONV_TIME_2116US, INA226_SHUNT_CONV_TIME_2116US, INA226_MODE_SHUNT_BUS_CONT);
  ina.calibrate(0.002, 4);
}

void loop()
{
  display.clearDisplay();
  display.println(Clockwise...);
  display.display();
  rotate_clockwise();
  while(!Serial.available()){}
  STOP();
  display.clearDisplay();
  display.println(Stop...);
  display.display();
  delay(2000);
  
  display.clearDisplay();
  display.println(Forward...);
  display.display();
  ADVANCE();
  measure_distance_sonar_1();
  measure_distance_sonar_2();
  while(distance_in_cm_1  5 && distance_in_cm_2  5) {
    measure_distance_sonar_1();
    measure_distance_sonar_2();
  }
  STOP();  
  display.clearDisplay();
  display.println(Stop...);
  display.display();
  delay(2000);

  display.clearDisplay();
  display.println(Rotating...);
  display.display();
  if (distance_in_cm1  distance_in_cm_2) {
    while(distance_in_cm1 != distance_in_cm_2) {
      rotate_clockwise();
    }
    STOP();
    delay(1000);
    RIGHT();
    while(Serial.available()==0){}
    STOP();
  }
  else {
    while(distance_in_cm1 != distance_in_cm_2) {
      rotate_anticlockwise();
    }
    STOP();
    delay(1000);
    LEFT();
    while(Serial.available()==0){}
    STOP();
  }
  while(True) {
    printing_power();
  }
  display.clearDisplay();
  display.println(Finished...);
  display.display();
  delay(20000);
}