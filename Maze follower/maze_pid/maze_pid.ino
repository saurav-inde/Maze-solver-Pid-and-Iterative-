#define MOTOR_RIGHT_FORWARD 11
#define MOTOR_RIGHT_BACKWARD 10
#define MOTOR_RIGHT_PWM 6

#define MOTOR_LEFT_FORWARD 8
#define MOTOR_LEFT_BACKWARD 9
#define MOTOR_LEFT_PWM 5

#define GREEN_LED 4
#define RED_LED 3

bool T = 0;
bool LEFT = 0;
bool RIGHT = 0;
bool PLUS = 0;
bool DEAD_END = 0;

uint8_t sensors[] = { A0, A1, A2, A3, A4, A5, 12, 13 };  //LTR
int sensor_data[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
uint8_t sensor_count = 8;

double last_error = 0;
double P, I, D, Kp = 5, Ki = 0, Kd = 4;
int basespeed_l = 90, basespeed_r = 90;
int maxspeed_l = 190, maxspeed_r = 190;  //for high speed
// int maxspeed_l = 150, maxspeed_r = 150; //for low speed
/*
HELPER UTILITY FUNCTIONS

*/

int sum(int a[]) {
  int sums = 0;
  for (int i = 0; i < 5; i++) {
    sums += a[i];
  }
  return sums;
}
/*
ERROR FUNCTION 
Gives the error in terms of the cm deviation from the center line``

*/
double error(int data[]) {
  double positions[] = { -15, -8, 0, 8, 15 };  //for high speed
  // double positions[] = { -13, -5.5, 0, 5.5, 13 }; //for low speed
  // double positions[] = { -10, -5.5, 0, 5.5, 10 };
  // double positions[] = { -6.4,-3.2,3.2,6.4 };
  int arr_data[5];
  // printf("\n");
  // int i;
  for (int k = 0, i = 0; i < 6; i++, k++) {
    if (i == 2) {
      arr_data[k] = max(data[i], data[i + 1]);
      i++;
    } else {
      arr_data[k] = data[i];
    }
  }
  int sums = sum(arr_data);


  double error_val = 0;
  for (int i = 0; i < 5; i++) {
    error_val += arr_data[i] * positions[i];
  }
  //   Serial.print("sums : ");
  // Serial.println(sums);

  // Serial.print("error : ");
  // Serial.println(error_val);
  // int sums = sum(arr_data);
  if (sums == 0)
    sums++;
  return error_val / (sums * 3);
}
/*FORWARD PID CONTROL FUNCITON : HELPER FUNCTION
USED BY THE PID FUNCTION TO ALTER MOTER SPEEDS SOLELY

*/
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

/*
PID CONTROL FUNCTION : PUBLIC FUNCTION 
IT MANAGES THE CAR ON LINE AND CURVES

*/
int error_value = 0;
void PID_control() {
  read_sensors();  // read the current position
  error_value = error(sensor_data);

  P = error_value;
  I = I + error_value;
  D = error_value - last_error;
  last_error = error_value;
  int motorspeed = P * Kp + I * Ki + D * Kd;  // calculate the correction
                                              // needed to be applied to the speed

  // int motorspeeda = 125 - motorspeed; // for low speed
  // int motorspeedb = 125 + motorspeed; //for low speed
  int motorspeeda = 140 - motorspeed;  //for high speed
  int motorspeedb = 140 + motorspeed;  //for high speed

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

/*
SESNOR READ FUNCTION:
It reads the input for each sensor and updates the data array

*/
void read_sensors() {
  for (int i = 0; i < sensor_count; i++) {
    sensor_data[i] = digitalRead(sensors[i]);
  }
}
/*
VOID SETUP functin
sets the initial conditions and the pins to be used
also sets the pwms
*/
void setup() {
  //setting up the motor pins
  pinMode(MOTOR_LEFT_FORWARD, OUTPUT);
  pinMode(MOTOR_LEFT_BACKWARD, OUTPUT);
  pinMode(MOTOR_LEFT_PWM, OUTPUT);

  pinMode(MOTOR_RIGHT_FORWARD, OUTPUT);
  pinMode(MOTOR_RIGHT_PWM, OUTPUT);
  pinMode(MOTOR_RIGHT_BACKWARD, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  //setting up the sensor pins
  for (auto pin : sensors)
    pinMode(pin, INPUT);

  read_sensors();
  TCCR0B = TCCR0B & B11111000 | B00000001;  // for PWM frequency of 7812.50 Hz
  Serial.begin(9600);
}

void loop() {

  PID_control();
  Detect();
  if (DEAD_END == true) {
    // turnRobot(180);
    U_Turn_timed();
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, HIGH);
    P = 0;
    I = 0;
    D = 0;
    error_value = 0;
  }
  PID_control();
  // read_sensors(sensor_data, sensors);
  // double err = error(sensor_data);
  // Serial.print("\nSesdsdsddsnsor read done\n");
  // for (auto i : sensor_data) {
  //   Serial.print(i);
  //   Serial.print(" ");
  // }
  //   Serial.println("");
  //   Serial.println(err);
  // Serial.println("");
  // delay(1000);
  // Serial.println("hi");
}
void Detect() {
  read_sensors();
  if (sensor_data[sensor_count - 2] == 1 && sensor_data[sensor_count - 1] == 0) {
    LEFT = true;
  } else if (sensor_data[sensor_count - 2] == 0 && sensor_data[sensor_count - 1] == 1) {
    RIGHT = true;
  } else if (sensor_data[sensor_count - 2] == 1 && sensor_data[sensor_count - 1] == 1) {
    T = true;
  } else if (sensor_data[0] == 0 && sensor_data[1] == 0 && sensor_data[2] == 0 && sensor_data[3] == 0 && sensor_data[4] == 0 && sensor_data[5] == 0
             && sensor_data[6] == 0 && sensor_data[7] == 0) {
    DEAD_END = true;
  }
}

void Stop() {
  analogWrite(MOTOR_LEFT_PWM, 0);
  analogWrite(MOTOR_RIGHT_PWM, 0);
  // analogWrite(MOTOR_RIGHT_B_PWM,0);
  digitalWrite(MOTOR_RIGHT_FORWARD, 0);
  digitalWrite(MOTOR_RIGHT_BACKWARD, 0);
  digitalWrite(MOTOR_LEFT_FORWARD, 0);
  digitalWrite(MOTOR_LEFT_BACKWARD, 0);
  // delay(3000);
}
int state[] = { 0, 0, 0, 0, 0, 0 };
void save_motor_state() {
  state[0] = digitalRead(MOTOR_LEFT_FORWARD);
  state[1] = digitalRead(MOTOR_LEFT_BACKWARD);
  state[2] = digitalRead(MOTOR_RIGHT_FORWARD);
  state[3] = digitalRead(MOTOR_RIGHT_BACKWARD);
  state[4] = analogRead(MOTOR_LEFT_PWM);
  state[5] = analogRead(MOTOR_RIGHT_PWM);
}

void reset_motor_state() {
  digitalWrite((MOTOR_LEFT_FORWARD), state[0]);
  digitalWrite((MOTOR_LEFT_BACKWARD), state[1]);
  digitalWrite((MOTOR_RIGHT_FORWARD), state[2]);
  digitalWrite((MOTOR_RIGHT_BACKWARD), state[3]);
  analogWrite(MOTOR_LEFT_PWM, state[4]);
  analogWrite(MOTOR_RIGHT_PWM, state[5]);
}
void turnRobot(int angle) {
  // Set the motor speeds and direction to turn the robot
  if (angle < 0) {
    // Turn left
    analogWrite(MOTOR_LEFT_FORWARD, 0);
    analogWrite(MOTOR_RIGHT_FORWARD, abs(angle));
    digitalWrite(MOTOR_LEFT_BACKWARD, abs(angle));
    digitalWrite(MOTOR_RIGHT_BACKWARD, 0);
  } else {
    // Turn right
    analogWrite(MOTOR_LEFT_FORWARD, abs(angle));
    analogWrite(MOTOR_RIGHT_FORWARD, 0);
    digitalWrite(MOTOR_LEFT_BACKWARD, 0);
    digitalWrite(MOTOR_RIGHT_BACKWARD, abs(angle));
  }
  // Wait for the turn to complete
  delay(500);
  // Stop the robot after the turn
  Stop();
}
void U_Turn_timed() {
  save_motor_state();
  analogWrite(MOTOR_LEFT_PWM, 125);
  analogWrite(MOTOR_RIGHT_PWM, 125);
  long time_start = millis();
  while (millis() - time_start < 100) {
    digitalWrite(MOTOR_RIGHT_BACKWARD, 0);
    digitalWrite(MOTOR_LEFT_FORWARD, 0);
    digitalWrite(MOTOR_RIGHT_FORWARD, 1);
    digitalWrite(MOTOR_LEFT_BACKWARD, 1);
  }
  Stop();

  // while (1) {
  //   digitalWrite(MOTOR_RIGHT_FORWARD, 1);
  //   digitalWrite(MOTOR_RIGHT_BACKWARD, 0);
  //   digitalWrite(MOTOR_LEFT_FORWARD, 0);
  //   digitalWrite(MOTOR_LEFT_BACKWARD, 1);
  //   analogWrite(MOTOR_LEFT_PWM, 130);
  //   analogWrite(MOTOR_RIGHT_PWM, 130);
  //   read_sensors();
  //   if (sensor_data[2] == 1 || sensor_data[3] == 1) {
  //     analogWrite(MOTOR_LEFT_PWM, 80);
  //     analogWrite(MOTOR_RIGHT_PWM, 80);
  //     digitalWrite(RED_LED, HIGH);
  //     Stop();
  //     delay(500);
  //     break;
  //   }
  // }
  P = 0;
  I = 0;
  D = 0;
  error_value = 0;
  PID_control();
}
void U_Turn() {
  save_motor_state();

  while (1) {
    digitalWrite(MOTOR_RIGHT_FORWARD, 1);
    digitalWrite(MOTOR_RIGHT_BACKWARD, 0);
    digitalWrite(MOTOR_LEFT_FORWARD, 0);
    digitalWrite(MOTOR_LEFT_BACKWARD, 1);
    analogWrite(MOTOR_LEFT_PWM, 120);
    analogWrite(MOTOR_RIGHT_PWM, 120);
    read_sensors();
    if ((sensor_data[1] == 1 || sensor_data[2] == 1 || sensor_data[3] == 1 || sensor_data[4] == 1 || sensor_data[0] == 1 || sensor_data[5] == 1))
    // { Stop();
    {
      // Stop();
      // analogWrite(MOTOR_LEFT_PWM, 40);
      // analogWrite(MOTOR_RIGHT_PWM, 40);
      digitalWrite(RED_LED, HIGH);
      // delay(100);
      long time_start = millis();
      int i;
      // while (millis() - time_start < 500) {
      //   i++;
      // }


      return;
    }
  }
  // while (1) {
  //   digitalWrite(MOTOR_RIGHT_FORWARD, 1);
  //   digitalWrite(MOTOR_RIGHT_BACKWARD, 0);
  //   digitalWrite(MOTOR_LEFT_FORWARD, 0);
  //   digitalWrite(MOTOR_LEFT_BACKWARD, 1);
  //   analogWrite(MOTOR_LEFT_PWM, 130);
  //   analogWrite(MOTOR_RIGHT_PWM, 130);
  //   read_sensors();
  //   if (sensor_data[2] == 1 || sensor_data[3] == 1) {
  //     analogWrite(MOTOR_LEFT_PWM, 80);
  //     analogWrite(MOTOR_RIGHT_PWM, 80);
  //     digitalWrite(RED_LED, HIGH);
  //     Stop();
  //     delay(500);
  //     break;
  //   }
  // }

  // PID_control();
}
void U_Turn_main() {
  int i = -1;
  // save_motor_state();
  // int i = -1;
  // while (sensors[2] == 1 && sensor_data[3] == 1 && sensor_data[6] == 0 && sensor_data[7] == 0) {
  //   U_Turn2(i);
  //   i *= -1;
  //   // U_Turn2();
  // }
  // reset_motor_state();
  // PID_control();

  save_motor_state();

  while (!((sensors[2] == 1 || sensor_data[3] == 1) && (sensor_data[6] == 0 && sensor_data[7] == 0)))

  {
    if (i == 1) {
      i = -1;
      while (1) {
        analogWrite(MOTOR_LEFT_PWM, 130);
        analogWrite(MOTOR_RIGHT_PWM, 130);
        digitalWrite(MOTOR_RIGHT_FORWARD, 1);
        digitalWrite(MOTOR_RIGHT_BACKWARD, 0);
        digitalWrite(MOTOR_LEFT_FORWARD, 0);
        digitalWrite(MOTOR_LEFT_BACKWARD, 1);
        read_sensors();
        if ((sensor_data[6] == 1 || sensor_data[7] == 1 || sensor_data[1] == 1 || sensor_data[2] == 1 || sensor_data[3] == 1 || sensor_data[4] == 1 || sensor_data[0] == 1 || sensor_data[5] == 1)) {
          analogWrite(MOTOR_LEFT_PWM, 90);
          analogWrite(MOTOR_RIGHT_PWM, 90);
          digitalWrite(GREEN_LED, HIGH);
          Stop();
          // delay(50);
          break;
        }
      }
    }
    if (i == -1) {
      i = 1;
      while (1) {
        analogWrite(MOTOR_LEFT_PWM, 130);
        analogWrite(MOTOR_RIGHT_PWM, 130);
        digitalWrite(MOTOR_RIGHT_FORWARD, 0);
        digitalWrite(MOTOR_LEFT_BACKWARD, 0);
        digitalWrite(MOTOR_RIGHT_BACKWARD, 1);
        digitalWrite(MOTOR_LEFT_FORWARD, 1);
        read_sensors();
        if ((sensor_data[6] == 1 || sensor_data[7] == 1 || sensor_data[1] == 1 || sensor_data[2] == 1 || sensor_data[3] == 1 || sensor_data[4] == 1 || sensor_data[0] == 1 || sensor_data[5] == 1))
        // { Stop();
        {
          // analogWrite(MOTOR_LEFT_PWM, 90);
          // analogWrite(MOTOR_RIGHT_PWM, 90);
          digitalWrite(RED_LED, HIGH);
          Stop();
          // delay(50);
          break;
        }
      }
    }
  }

  reset_motor_state();
}

void U_Turn2(int i) {
  // read_sensors() if (sensors[2] == 1 && sensor_data[3] == 1 && sensor_data[6] == 0 && sensor_data[7] == 0) return;
  // if (i == -1) {
  //   while (1) {
  //     digitalWrite(MOTOR_RIGHT_BACKWARD, 0);
  //     digitalWrite(MOTOR_LEFT_FORWARD, 0);
  //     digitalWrite(MOTOR_RIGHT_FORWARD, 1);
  //     digitalWrite(MOTOR_LEFT_BACKWARD, 1);
  //     analogWrite(MOTOR_LEFT_PWM, 125);
  //     analogWrite(MOTOR_RIGHT_PWM, 125);
  //     read_sensors();
  //     if ((sensor_data[6] == 1 || sensor_data[7] == 1 || sensor_data[1] == 1 || sensor_data[2] == 1 || sensor_data[3] == 1 || sensor_data[4] == 1 || sensor_data[0] == 1 || sensor_data[5] == 1))
  //     // { Stop();
  //     {
  //       analogWrite(MOTOR_LEFT_PWM, 0);
  //       analogWrite(MOTOR_RIGHT_PWM, 0);
  //       digitalWrite(RED_LED, HIGH);
  //       // delay(100);
  //       break;
  //     }
  //   }
  // }
  // if (i == 1) {
  //   while (1) {
  //     digitalWrite(MOTOR_RIGHT_FORWARD, 0);
  //     digitalWrite(MOTOR_LEFT_BACKWARD, 0);
  //     digitalWrite(MOTOR_RIGHT_BACKWARD, 1);
  //     digitalWrite(MOTOR_LEFT_FORWARD, 1);
  //     analogWrite(MOTOR_LEFT_PWM, 125);
  //     analogWrite(MOTOR_RIGHT_PWM, 125);
  //     read_sensors();
  //     if ((sensor_data[6] == 1 || sensor_data[7] == 1 || sensor_data[1] == 1 || sensor_data[2] == 1 || sensor_data[3] == 1 || sensor_data[4] == 1 || sensor_data[0] == 1 || sensor_data[5] == 1))
  //     // { Stop();
  //     {
  //       analogWrite(MOTOR_LEFT_PWM, 0);
  //       analogWrite(MOTOR_RIGHT_PWM, 0);
  //       digitalWrite(RED_LED, HIGH);
  //       // delay(100);
  //       break;
  //     }
  //   }
  // }
  save_motor_state();

  while (1) {
    digitalWrite(MOTOR_RIGHT_FORWARD, 1);
    digitalWrite(MOTOR_RIGHT_BACKWARD, 0);
    digitalWrite(MOTOR_LEFT_FORWARD, 0);
    digitalWrite(MOTOR_LEFT_BACKWARD, 1);
    analogWrite(MOTOR_LEFT_PWM, 125);
    analogWrite(MOTOR_RIGHT_PWM, 125);
    read_sensors();
    if ((sensor_data[6] == 1 || sensor_data[7] == 1 || sensor_data[1] == 1 || sensor_data[2] == 1 || sensor_data[3] == 1 || sensor_data[4] == 1 || sensor_data[0] == 1 || sensor_data[5] == 1))
    // { Stop();
    {
      analogWrite(MOTOR_LEFT_PWM, 90);
      analogWrite(MOTOR_RIGHT_PWM, 90);
      digitalWrite(RED_LED, HIGH);
      // delay(100);
      break;
    }
  }
}

void toggle_LED(int pin) {
  digitalWrite(pin, digitalRead(pin) ^ 1);
}
