// Define the pins for the left and right IR sensors
int leftSensorPin = A1;
int rightSensorPin = A4;

// Define the threshold value for detecting the line
int threshold = 160;

// Define the motor control pins
int leftMotorPin1 = 8;
int leftMotorPin2 = 9;
int rightMotorPin1 = 10;
int rightMotorPin2 = 11;  

// Define the motor speed values
int leftMotorSpeed = 110;
int rightMotorSpeed = 45;

void setup() {
  // Initialize the sensor pins as input
  pinMode(leftSensorPin, INPUT);
  pinMode(rightSensorPin, INPUT);

  // Initialize the motor control pins as output
  pinMode(leftMotorPin1, OUTPUT);
  pinMode(leftMotorPin2, OUTPUT);
  pinMode(rightMotorPin1, OUTPUT);
  pinMode(rightMotorPin2, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  analogWrite(5, 58);
  analogWrite(6, 52);
}


void loop() {
  // Read the values from the left and right IR sensors
  int leftSensorValue = constrain(analogRead(leftSensorPin), 21,499);
  int rightSensorValue = constrain(analogRead(rightSensorPin), 21,499);

  // Calculate the motor speeds based on the sensor readings
  // int leftMotorSpeed = 60 - map(leftSensorValue, 20, 500, 0, 10);
  // int rightMotorSpeed = 50 - map(rightSensorValue, 20, 500, 0, 10);
  // int rightMotorSpeed = 50 - map(leftSensorValue, 70, 500, 0, 10);
  // int leftMotorSpeed = 60 - map(rightSensorValue, 70, 500, 0, 10);

  // Set the motor speeds
  // analogWrite(5, 60);
  // analogWrite(6, 50);

  // If both sensors detect the line, move forward
  if (leftSensorValue <threshold && rightSensorValue <threshold) {
    digitalWrite(leftMotorPin1, HIGH);
    digitalWrite(leftMotorPin2, LOW);
    digitalWrite(rightMotorPin1, HIGH);
    digitalWrite(rightMotorPin2, LOW);
  }
  // If only the left sensor detects the line, turn left
  else if (leftSensorValue > threshold) {
    digitalWrite(leftMotorPin1, LOW);
    digitalWrite(leftMotorPin2, HIGH);
    digitalWrite(rightMotorPin1, HIGH);
    digitalWrite(rightMotorPin2, LOW);
    // analogWrite(5, 50);
    // analogWrite(6, 50);
  }
  // If only the right sensor detects the line, turn right
  else if (rightSensorValue > threshold) {
    digitalWrite(leftMotorPin1, HIGH);
    digitalWrite(leftMotorPin2, LOW);
    digitalWrite(rightMotorPin1, LOW);
    digitalWrite(rightMotorPin2, HIGH);
  }
  // If neither sensor detects the line, stop
  else if(leftSensorValue >threshold && rightSensorValue >threshold){
    digitalWrite(leftMotorPin1, LOW);
    digitalWrite(leftMotorPin2, LOW);
    digitalWrite(rightMotorPin1, LOW);
    digitalWrite(rightMotorPin2, LOW);
  }
}



// void loop() {
//   // Read the values from the left and right IR sensors
//   int leftSensorValue = analogRead(leftSensorPin);
//   int rightSensorValue = analogRead(rightSensorPin);

//   // If both sensors detect the line, move forward
//   if (leftSensorValue <threshold && rightSensorValue <threshold) {
//     digitalWrite(leftMotorPin1, HIGH);
//     digitalWrite(leftMotorPin2, LOW);
//     digitalWrite(rightMotorPin1, HIGH);
//     digitalWrite(rightMotorPin2, LOW);
//   }
//   // If only the left sensor detects the line, turn left
//   else if (leftSensorValue > threshold) {
//     digitalWrite(leftMotorPin1, LOW);
//     digitalWrite(leftMotorPin2, HIGH);
//     digitalWrite(rightMotorPin1, HIGH);
//     digitalWrite(rightMotorPin2, LOW);
//     // analogWrite(5, 50);
//     // analogWrite(6, 50);
//   }
//   // If only the right sensor detects the line, turn right
//   else if (rightSensorValue > threshold) {
//     digitalWrite(leftMotorPin1, HIGH);
//     digitalWrite(leftMotorPin2, LOW);
//     digitalWrite(rightMotorPin1, LOW);
//     digitalWrite(rightMotorPin2, HIGH);
//   }
//   // If neither sensor detects the line, stop
//   else if(leftSensorValue >threshold && rightSensorValue >threshold){
//     digitalWrite(leftMotorPin1, LOW);
//     digitalWrite(leftMotorPin2, LOW);
//     digitalWrite(rightMotorPin1, LOW);
//     digitalWrite(rightMotorPin2, LOW);
//   }

//   // // Set the motor speeds
//   // analogWrite(leftMotorSpeed, leftMotorPin1);
//   // analogWrite(rightMotorSpeed, rightMotorPin1);
// // digitalWrite((leftMotorPin1), HIGH);
// // digitalWrite((rightMotorPin1), HIGH);

// }
