#include <Servo.h>

// digital write will receive the full 5v signal.  This may be too fast.
// use analog write to send a PWM signal with a value of 0 - 255.
// works const int left_wheel = 3;       // Green wire, IN1, pin 3
// works const int left_wheel_back = 6;  // Yellow wire, IN2, pin 6 
// works const int right_wheel = 11;       // Blue wire, IN3, pin 11  
// works const int right_wheel_back = 5;   // Purple wire, IN4, pin 5

int speed = 125; // how fast do you want your motors to go? 0-255


void setup() {
  // serial
  Serial.begin(9600);

  // motors
  pinMode(left_wheel, OUTPUT);
//  pinMode(left_wheel_back, OUTPUT);
//  pinMode(right_wheel, OUTPUT);
//  pinMode(right_wheel_back, OUTPUT);

}
void loop() {
  
//  analogWrite(left_wheel_back, 125);
//  analogWrite(right_wheel_back, 125);
  analogWrite(left_wheel, 125);
//  analogWrite(right_wheel, 125);
  
}



