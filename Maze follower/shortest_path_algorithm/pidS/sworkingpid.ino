
#define MOTOR_RIGHT_FORWARD 10
#define MOTOR_RIGHT_BACKWARD 11
#define MOTOR_RIGHT_PWM 6

#define MOTOR_LEFT_FORWARD 8
#define MOTOR_LEFT_BACKWARD 9
#define MOTOR_LEFT_PWM 5

int sensors[] = { A0, A1, A2, A3, A4 };
int sensor_max = 700;
int sensor_min = 70;
int mapped_readings[5];


double last_error = 0;
//low speed
// double P, I, D, Kp = .875, Ki =0.002 , Kd = 40;
// // double P, I, D, Kp = .875, Ki =0.015 , Kd = 40;
// int basespeed_l = 55, basespeed_r = 47;
// int maxspeed_l = 75, maxspeed_r = 70;

//high speed
double P, I, D, Kp = 1.1, Ki = 0.0017, Kd = 60;
int basespeed_l = 65, basespeed_r = 57;
int maxspeed_l = 85, maxspeed_r = 80;




void take_readings() {
  int temp;
  for (int i = 0; i < 5; i++) {
    temp = constrain(analogRead(sensors[i]), sensor_min, sensor_max);
    mapped_readings[i] = map(temp, sensor_min, sensor_max, 0, 200);
  }
}


double error(int data[]) {
  double positions[] = { -2, -1, 0, 1, 2 };  //for high speed

  double error_val = 0;
  for (int i = 0; i < 5; i++) {
    error_val += mapped_readings[i] * positions[i];
  }

  return error_val;
}
int error_value;
void PID_control() {
  take_readings();  // read the current position
  error_value = error(mapped_readings);

  P = error_value;
  I = I + error_value;
  D = error_value - last_error;
  last_error = error_value;
  int motorspeed = P * Kp + I * Ki + D * Kd;  // calculate the correction
                                              // needed to be applied to the speed

  // int motorspeeda = 125 - motorspeed; // for low speed
  // int motorspeedb = 125 + motorspeed; //for low speed
  int motorspeeda = basespeed_l + motorspeed;  //for high speed
  int motorspeedb = basespeed_r - motorspeed;  //for high speed

  if (motorspeeda > maxspeed_l) {
    motorspeeda = maxspeed_l;
  }
  if (motorspeedb > maxspeed_r) {
    motorspeedb = maxspeed_r;
  }
  // if (motorspeeda >0 && motorspeeda <basespeed_l) {
  // statements
  // }

  if (last_error == 0 && error_value == 0) {
    I = 0;
    D = 0;
  }
  forward_pid(motorspeeda, motorspeedb);
}



void forward_pid(int speed_l, int speed_r) {
  if (speed_l < 0) {
    if (-speed_l > maxspeed_l) {
      speed_l = -maxspeed_l;
    }
    analogWrite(MOTOR_LEFT_PWM, -speed_l);
    digitalWrite(MOTOR_LEFT_FORWARD, 0);
    digitalWrite(MOTOR_LEFT_BACKWARD, 1);
  } else {
    analogWrite(MOTOR_LEFT_PWM, speed_l);
    digitalWrite(MOTOR_LEFT_BACKWARD, 0);
    digitalWrite(MOTOR_LEFT_FORWARD, 1);
  }
  if (speed_r < 0) {
    if (-speed_r > maxspeed_r) {
      speed_r = -maxspeed_r;
    }
    analogWrite(MOTOR_RIGHT_PWM, -speed_r);
    digitalWrite(MOTOR_RIGHT_FORWARD, 0);
    digitalWrite(MOTOR_RIGHT_BACKWARD, 1);
  } else {
    analogWrite(MOTOR_RIGHT_PWM, speed_r);
    digitalWrite(MOTOR_RIGHT_BACKWARD, 0);
    digitalWrite(MOTOR_RIGHT_FORWARD, 1);
  }
}
void setup() {
  pinMode(MOTOR_LEFT_FORWARD, OUTPUT);
  pinMode(MOTOR_LEFT_BACKWARD, OUTPUT);
  pinMode(MOTOR_LEFT_PWM, OUTPUT);

  pinMode(MOTOR_RIGHT_FORWARD, OUTPUT);
  pinMode(MOTOR_RIGHT_PWM, OUTPUT);
  pinMode(MOTOR_RIGHT_BACKWARD, OUTPUT);
  // pinMode(GREEN_LED, OUTPUT);
  // pinMode(RED_LED, OUTPUT);

  //setting up the sensor pins
  for (auto pin : sensors)
    pinMode(pin, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly

  PID_control();
}
