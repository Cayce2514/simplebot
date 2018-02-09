
// Set the pin for Echo and Trigger:
const int triggerPin = 8;
const int echoPin = 9;

volatile float duration, distanceCm;

void setup() {
  // serial setup so that we can watch in the serial monitor
  Serial.begin(9600);
  
  // ultrasonic sensor setup
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);

}

void loop() {
  // call the checkDistance function

}

void checkDistance() {
  digitalWrite(triggerPin, LOW);  // to generate a clean pulse we put LOW 4 microsec
  delayMicroseconds(4);
  digitalWrite(triggerPin, HIGH);  // generate Trigger (shot) of 10 microsec
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  duration = pulseIn(echoPin, HIGH);  // we measure the time between pulses, in microseconds
  distanceCm = duration * 10 / 292 / 2;  // we convert to distance, in cm
  Serial.print("Distance: ");
  Serial.print(distanceCm);
  Serial.println(" cm");
}

