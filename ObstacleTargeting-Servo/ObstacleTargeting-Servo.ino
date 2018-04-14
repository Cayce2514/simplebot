#include <Servo.h>

// Distance sensor connections
const int frontEchoPin = 11;
const int frontTriggerPin = 10;

Servo leftServo;
Servo rightServo;

const int leftServoPin = 9;        // continuous rotation servo
const int rightServoPin = 8;      // continuous rotation servo

// how fast do you want your motors to go? 0-89 forward, 90 stop, 91-180 reverse.
int forward = 180;
int back = 0;
int stop = 90;

volatile float maxFrontDistance = 10.00;
volatile float frontDuration, frontDistanceCm;

void setup() {
  // serial
  Serial.begin(9600);

  // ultrasonic
  pinMode(frontTriggerPin, OUTPUT);
  pinMode(frontEchoPin, INPUT);

  // servos
  leftServo.attach(leftServoPin);
  rightServo.attach(rightServoPin);

  
  delay(5000);
}
void loop() {
  
  // front distance check
  checkFrontDistance();
  if (frontDistanceCm < maxFrontDistance) {
    Serial.println("Attack!!");
    moveForward();
    //delay(1000);
    //moveBackward();
    //delay(1000);
    //moveLeft();
    delay(50);
 
  }
  else {
    Serial.println("OK");
    moveRight();
  }
}


void checkFrontDistance() {
  digitalWrite(frontTriggerPin, LOW);  
  delayMicroseconds(4);
  digitalWrite(frontTriggerPin, HIGH);  
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
  leftServo.write(forward);
  rightServo.write(back);
}

void moveForward() {
  Serial.println("Forward.");
  leftServo.write(forward);
  rightServo.write(back);
}

void moveLeft() {
  Serial.println("Left.");
  leftServo.write(stop);
  rightServo.write(forward);
}

void moveRight() {
  Serial.println("Right.");
  leftServo.write(forward);
  rightServo.write(stop);
}

void moveStop() {
  Serial.println("Stopping.");
  leftServo.write(stop);
  rightServo.write(stop);
}
