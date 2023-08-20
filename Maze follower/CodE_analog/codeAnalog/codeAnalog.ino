#define MOTOR_LEFT_FORWARD 8
#define MOTOR_LEFT_BACKWARD 9
#define MOTOR_RIGHT_FORWARD 10
#define MOTOR_RIGHT_BACKWARD 11
#define MOTOR_LEFT_PWM 5
#define MOTOR_RIGHT_PWM 6

#define LEFT_SENSOR A0
#define RIGHT_SENSOR A4

double El, Er, SpeedL, SpeedR;
double LeftMax = 660, RightMax = 660;
double LeftMin = 45, RightMin = 45;
double LeftCurrent, RightCurrent;
double LeftMaxSpeed = 50, RightMaxSpeed = 50;
double err;


// Define a custom function to constrain a value between a min and max
double Constraint(double value, double minVal, double maxVal) {
  return min(max(value, minVal), maxVal);
}
void UpdateSensors() {
  // Read current sensor values and constrain them to the min/max range
  LeftCurrent = Constraint(analogRead(LEFT_SENSOR), LeftMin, LeftMax);
  RightCurrent = Constraint(analogRead(RIGHT_SENSOR), RightMin, RightMax);
}

void setup() {


  // Set pin modes for motor control
  pinMode(MOTOR_LEFT_FORWARD, OUTPUT);
  pinMode(MOTOR_LEFT_BACKWARD, OUTPUT);
  pinMode(MOTOR_RIGHT_FORWARD, OUTPUT);
  pinMode(MOTOR_RIGHT_BACKWARD, OUTPUT);
  pinMode(MOTOR_LEFT_PWM, OUTPUT);
  pinMode(MOTOR_RIGHT_PWM, OUTPUT);

  pinMode(LEFT_SENSOR, INPUT);
  pinMode(RIGHT_SENSOR, INPUT);
  // Set initial speed for motors to 0
  analogWrite(MOTOR_LEFT_PWM, 0);
  analogWrite(MOTOR_RIGHT_PWM, 0);
}

void loop() {
  UpdateSensors();

  El = ((LeftCurrent - LeftMin) / (LeftMax - LeftMin)) * 2;
  Er = ((RightMax - RightCurrent) / (RightMax - RightMin)) * 2;
  err = (El+Er)/2;
  
  SpeedL = LeftMaxSpeed * (1 - err);   // Scale El to the PWM range (0-255)
  SpeedR = RightMaxSpeed * (1 + err);  // Scale Er to the PWM range (0-255)

  if (SpeedL < 0) {
    // digitalWrite(MOTOR_LEFT_BACKWARD, HIGH);
    // digitalWrite(MOTOR_LEFT_FORWARD, LOW);
    // analogWrite(MOTOR_LEFT_PWM, abs(SpeedL));
    digitalWrite(MOTOR_LEFT_BACKWARD, HIGH);
    digitalWrite(MOTOR_LEFT_FORWARD, LOW);
    analogWrite(MOTOR_LEFT_PWM, abs(SpeedL));
  } else {
    // digitalWrite(MOTOR_LEFT_BACKWARD, LOW);
    // digitalWrite(MOTOR_LEFT_FORWARD, HIGH);
    // analogWrite(MOTOR_LEFT_PWM, SpeedL);
    digitalWrite(MOTOR_LEFT_BACKWARD, LOW);
    digitalWrite(MOTOR_LEFT_FORWARD, HIGH);
    analogWrite(MOTOR_LEFT_PWM, SpeedL);
  }

  if (SpeedR < 0) {
    // digitalWrite(MOTOR_RIGHT_BACKWARD, HIGH);
    // digitalWrite(MOTOR_RIGHT_FORWARD, LOW);
    // analogWrite(MOTOR_RIGHT_PWM, abs(SpeedR));
    digitalWrite(MOTOR_RIGHT_BACKWARD, HIGH);
    digitalWrite(MOTOR_RIGHT_FORWARD, LOW);
    analogWrite(MOTOR_RIGHT_PWM, abs(SpeedR));
  } else {
    // digitalWrite(MOTOR_RIGHT_BACKWARD, LOW);
    // digitalWrite(MOTOR_RIGHT_FORWARD, HIGH);
    // analogWrite(MOTOR_RIGHT_PWM, SpeedR);
    digitalWrite(MOTOR_RIGHT_BACKWARD, LOW);
    digitalWrite(MOTOR_RIGHT_FORWARD, HIGH);
    analogWrite(MOTOR_RIGHT_PWM, SpeedR);
  }
  // analogWrite(MOTOR_LEFT_PWM, 60);
  // digitalWrite(MOTOR_LEFT_BACKWARD, HIGH);
}
