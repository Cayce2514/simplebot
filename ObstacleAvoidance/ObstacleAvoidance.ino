#include <Servo.h>
// this code doesn't work. -cayceb 1/9/18

// Distance sensor connections
const int frontEchoPin = 9;
const int frontTriggerPin = 8;

// Distance sensor servo
const int sensorServoPin = 10;
Servo sensorServo;

// digital write will receive the full 5v signal.  This may be too fast.
// use analog write to send a PWM signal with a value of 0 - 255.
const int left_wheel = 3;        // Green wire, IN1, pin 3
const int left_wheel_back = 6;   // Yellow wire, IN2, pin 6
const int right_wheel = 11;      // Blue wire, IN3, pin 11
const int right_wheel_back = 5;  // Purple wire, IN4, pin 5

int speed = 125; // how fast do you want your motors to go? 0-255

volatile float maxFrontDistance = 50.00;
volatile float frontDuration, frontDistanceCm;
volatile float maxLeftDistance, maxRightDistance = 20.00;
void setup() {
  // serial
  Serial.begin(9600);
  // ultrasonic
  sensorServo.attach(sensorServoPin);
  sensorServo.write(90);
  
  pinMode(frontTriggerPin, OUTPUT);
  pinMode(frontEchoPin, INPUT);

  // sensor servo
  pinMode(sensorServoPin, OUTPUT);

  // motors
  pinMode(left_wheel, OUTPUT);
  pinMode(left_wheel_back, OUTPUT);
  pinMode(right_wheel, OUTPUT);
  pinMode(right_wheel_back, OUTPUT);
  delay(5000);
}
void loop() {
  
  // front distance check
  checkFrontDistance();
  if (frontDistanceCm < maxFrontDistance) {
    Serial.println("Too close");
    moveStop();
    delay(1000);
    moveBackward();
    delay(1000);
    moveLeft();
    delay(1000);
 
  }
  else {
    Serial.println("OK");
    moveForward();
  }
}
void checkFrontDistance() {
  digitalWrite(frontTriggerPin, LOW);  //para generar un pulso limpio ponemos a LOW 4us
  delayMicroseconds(4);
  digitalWrite(frontTriggerPin, HIGH);  //generamos Trigger (disparo) de 10us
  delayMicroseconds(10);
  digitalWrite(frontTriggerPin, LOW);
  frontDuration = pulseIn(frontEchoPin, HIGH);  //medimos el tiempo entre pulsos, en microsegundos
  frontDistanceCm = frontDuration * 10 / 292 / 2;  //convertimos a distancia, en cm
  Serial.print("Distance: ");
  Serial.print(frontDistanceCm);
  Serial.println(" cm");
}

void moveBackward() {
  Serial.println("Backward.");
  analogWrite(left_wheel_back, speed);  // alogWrite values from 0 to 255
  analogWrite(right_wheel_back, speed);
  analogWrite(left_wheel, 0);
  analogWrite(right_wheel, 0);
}

void moveForward() {
  Serial.println("Forward.");
  analogWrite(left_wheel, speed);
  analogWrite(right_wheel, speed);
  analogWrite(left_wheel_back, 0);
  analogWrite(right_wheel_back, 0);
}

void moveLeft() {
  Serial.println("Left.");
  analogWrite(left_wheel, 0);
  analogWrite(right_wheel, speed);
  analogWrite(left_wheel_back, 0);
  analogWrite(right_wheel_back, 0);
}

void moveRight() {
  Serial.println("Right.");
  analogWrite(left_wheel, speed);
  analogWrite(right_wheel, 0);
  analogWrite(left_wheel_back, 0);
  analogWrite(right_wheel_back, 0);
}

void moveStop() {
  Serial.println("Stopping.");
  analogWrite(left_wheel_back, 0);
  analogWrite(right_wheel_back, 0);
  analogWrite(left_wheel, 0);
  analogWrite(right_wheel, 0);
}
