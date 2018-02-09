
// Set the pin for left and right motors, forward and back:
const int left_wheel = 12;
const int left_wheel_back = 11;

const int right_wheel = 7;
const int right_wheel_back = 6;


void setup() {
  // serial setup so that we can watch in the serial monitor
  Serial.begin(9600);
  
  // wheel pin setup
  pinMode(left_wheel, OUTPUT);
  pinMode(left_wheel_back, OUTPUT);

  pinMode(right_wheel, OUTPUT);
  pinMode(right_wheel_back, OUTPUT);


}

void loop() {
  // call a move function
  moveForward();
  //moveBackward();

}

void moveForward() {
  Serial.println("Forward.");
  digitalWrite(left_wheel, HIGH);
  digitalWrite(left_wheel_back, LOW);
  digitalWrite(right_wheel, HIGH);
  digitalWrite(right_wheel_back, LOW);
}

void moveBackward() {
  Serial.println("Backward.");
  digitalWrite(left_wheel_back, HIGH);
  digitalWrite(left_wheel, LOW);
  digitalWrite(right_wheel_back, HIGH);
  digitalWrite(right_wheel, LOW);

}
