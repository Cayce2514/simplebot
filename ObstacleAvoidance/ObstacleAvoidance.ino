#include <Servo.h>
// this code doesn't work. -cayceb 1/9/18
const int frontEchoPin = 3;
const int frontTriggerPin = 2;
const int leftEchoPin = 11;


const int left_wheel = 11;
const int right_wheel = 10;
volatile float maxFrontDistance = 25.00;
volatile float frontDuration, frontDistanceCm;
volatile float maxLeftDistance, maxRightDistance = 20.00;
void setup() {
  // serial
  Serial.begin(9600);
  // ultrasonic
  pinMode(frontTriggerPin, OUTPUT);
  pinMode(frontEchoPin, INPUT);

  // motors
  pinMode(left_wheel, OUTPUT);
  pinMode(right_wheel, OUTPUT);
}
void loop() {
  // front distance check
  checkFrontDistance();
  if (frontDistanceCm < maxFrontDistance) {
    Serial.println("Too close");
    moveLeft();
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
  digitalWrite(left_wheel, HIGH);
  digitalWrite(right_wheel, HIGH);
}
void moveForward() {
  Serial.println("Forward.");
  digitalWrite(left_wheel, LOW);
  digitalWrite(right_wheel, LOW);
}
void moveLeft() {
  Serial.println("Left.");
  digitalWrite(left_wheel, LOW);
  digitalWrite(right_wheel, HIGH);
}
void moveRight() {
  Serial.println("Right.");
  digitalWrite(left_wheel, HIGH);
  digitalWrite(right_wheel, LOW);
}
