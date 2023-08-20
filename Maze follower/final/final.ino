#define MOTOR_RIGHT_FORWARD 10
#define MOTOR_RIGHT_BACKWARD 11
#define MOTOR_RIGHT_PWM 6

#define MOTOR_LEFT_FORWARD 8
#define MOTOR_LEFT_BACKWARD 9
#define MOTOR_LEFT_PWM 5

// #define FRONT_LEFT_IR A3
#define FRONT_RIGHT_IR 4
#define FRONT_CENTER_IR 3

#define EXTREME_LEFT_IR A5
#define EXTREME_RIGHT_IR 12

int sensors[] = { A0, A1, A2, A3, A4 };
int sensor_max[] = {700,650,550,650,600};
int sensor_min[] = {40,40,40,40,40};
int original_readings[5];
int mapped_readings[5];

int rotation_threshold = 200;


//low speed
// double P, I, D, Kp = .875, Ki =0.002 , Kd = 40;
// // double P, I, D, Kp = .875, Ki =0.010 , Kd = 40;
// int basespeed_l = 55, basespeed_r = 47;
// int maxspeed_l = 75, maxspeed_r = 70;

// //high speed
// double P, I, D, Kp = 1.1, Ki = 0.0017, Kd = 45;
// // int basespeed_l = 65, basespeed_r = 57;
// int basespeed_l = 75, basespeed_r = 67;
// int maxspeed_l = 85, maxspeed_r = 80;
// int maxspeed_l = 95, maxspeed_r = 90;


//CURRENTLY WORKING


double P, I, D, Kp = 2, Ki =0.0010 , Kd = 30;
int basespeed_l = 70, basespeed_r = 68;
int maxspeed_l = 90, maxspeed_r = 88;

double last_error = 0;
int error_value;


int ext_left;
int ext_right;
int front_right_ir;
int front_center_ir;
/*
############################################
############################################PID FUNCTIONS BEGIN HERE
############################################
*/
void take_readings() {
  int temp;
  for (int i = 0; i < 5; i++) {
    temp = constrain(analogRead(sensors[i]), sensor_min[i], sensor_max[i]);
    mapped_readings[i] = map(temp, sensor_min[i], sensor_max[i], 0, 200);
    original_readings[i] = temp;
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
  int motorspeeda = 75 + motorspeed;  //for high speed
  int motorspeedb = 68 - motorspeed;  //for high speed

  if (motorspeeda > maxspeed_l) {
    motorspeeda = maxspeed_l;
  }
  if (motorspeedb > maxspeed_r) {
    motorspeedb = maxspeed_r;
  }
  // if (motorspeeda >0 && motorspeeda <basespeed_l) {
  // statements
  // }

  // if (last_error == 0 && error_value == 0) {
  //   I = 0;
  //   D = 0;
  // }
  forward_pid(motorspeeda, motorspeedb);
}



// void forward_pid(int speed_l, int speed_r) {
//   if (speed_l < 0) {
//     if (-speed_l > maxspeed_l) {
//       speed_l = -maxspeed_l;
//     }
//     analogWrite(MOTOR_LEFT_PWM, -speed_l);
//     digitalWrite(MOTOR_LEFT_FORWARD, 0);
//     digitalWrite(MOTOR_LEFT_BACKWARD, 1);
//   } else {
//     analogWrite(MOTOR_LEFT_PWM, speed_l);
//     digitalWrite(MOTOR_LEFT_BACKWARD, 0);
//     digitalWrite(MOTOR_LEFT_FORWARD, 1);
//   }
//   if (speed_r < 0) {
//     if (-speed_r > maxspeed_r) {
//       speed_r = -maxspeed_r;
//     }
//     analogWrite(MOTOR_RIGHT_PWM, -speed_r);
//     digitalWrite(MOTOR_RIGHT_FORWARD, 0);
//     digitalWrite(MOTOR_RIGHT_BACKWARD, 1);
//   } else {
//     analogWrite(MOTOR_RIGHT_PWM, speed_r);
//     digitalWrite(MOTOR_RIGHT_BACKWARD, 0);
//     digitalWrite(MOTOR_RIGHT_FORWARD, 1);
//   }
// }
// void forward_pid(int speed_l, int speed_r) {
//   if (speed_l < 0) {
//     if (-speed_l > maxspeed_l) {
//       speed_l = -maxspeed_l;
//       analogWrite(MOTOR_LEFT_PWM, -speed_l);
//       digitalWrite(MOTOR_LEFT_FORWARD, 0);
//       digitalWrite(MOTOR_LEFT_BACKWARD, 1);
//     } else {
//       analogWrite(MOTOR_LEFT_PWM, speed_l);
//       digitalWrite(MOTOR_LEFT_FORWARD, 0);
//       digitalWrite(MOTOR_LEFT_BACKWARD, 1);
//     }
//   }
//   //  else if (speed_l < 20) {
//   //   speed_l = 20;
//   //   analogWrite(MOTOR_LEFT_PWM, speed_l);
//   //   digitalWrite(MOTOR_LEFT_BACKWARD, 0);
//   //   digitalWrite(MOTOR_LEFT_FORWARD, 1);
//   // }
//    else {

//     analogWrite(MOTOR_LEFT_PWM, speed_l);
//     digitalWrite(MOTOR_LEFT_BACKWARD, 0);
//     digitalWrite(MOTOR_LEFT_FORWARD, 1);
//   }
//   if (speed_r < 0) {
//     if (-speed_r > maxspeed_r) {
//       speed_r = -maxspeed_r;
//       analogWrite(MOTOR_RIGHT_PWM, speed_r);
//       digitalWrite(MOTOR_RIGHT_BACKWARD, 1);
//       digitalWrite(MOTOR_RIGHT_FORWARD, 0);

//     } else {
//       analogWrite(MOTOR_RIGHT_PWM, speed_r);
//       digitalWrite(MOTOR_RIGHT_BACKWARD, 1);
//       digitalWrite(MOTOR_RIGHT_FORWARD, 0);
//     }
//   }

//   // else if (speed_r < 20) {
//   //   speed_r = 20;
//   //   analogWrite(MOTOR_RIGHT_PWM, speed_r);
//   //   digitalWrite(MOTOR_RIGHT_BACKWARD, 0);
//   //   digitalWrite(MOTOR_RIGHT_FORWARD, 1);
//   // }
//   else{
//   analogWrite(MOTOR_RIGHT_PWM, speed_r);
//   digitalWrite(MOTOR_RIGHT_BACKWARD, 0);
//   digitalWrite(MOTOR_RIGHT_FORWARD, 1);
// }
// }


int limit_l = 50;
int limit_r = 50;
void forward_pid(int speed_l, int speed_r) {
  // speed_l = (speed_l < limit_l && speed_l > 0) ? limit_l : speed_l;
  // speed_l = (-speed_l < limit_l && speed_l < 0) ? -limit_l : speed_l;

  // speed_r = (speed_r < limit_r && speed_r > 0) ? limit_r : speed_r;
  // speed_r = (-speed_r < limit_r && speed_r < 0) ? -limit_r : speed_r;

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
############################################
#############################################FUNCTIONS FOR SHARP TURNS
############################################

*/

void u_turn() {
  // delay(500);
  while (original_readings[1] < rotation_threshold && original_readings[2] < rotation_threshold && original_readings[3] < rotation_threshold) {
    // update_line_keepers();
    take_readings();
    digitalWrite(MOTOR_LEFT_FORWARD, 1);
    digitalWrite(MOTOR_RIGHT_FORWARD, 0);
    digitalWrite(MOTOR_LEFT_BACKWARD, 0);
    digitalWrite(MOTOR_RIGHT_BACKWARD, 1);
  }
}

void _straight() {
  digitalWrite(MOTOR_LEFT_FORWARD, 1);
  digitalWrite(MOTOR_RIGHT_FORWARD, 1);
  digitalWrite(MOTOR_LEFT_BACKWARD, 0);
  digitalWrite(MOTOR_RIGHT_BACKWARD, 0);
}
void _backward() {
  digitalWrite(MOTOR_LEFT_FORWARD, 0);
  digitalWrite(MOTOR_RIGHT_FORWARD, 0);
  digitalWrite(MOTOR_LEFT_BACKWARD, 1);
  digitalWrite(MOTOR_RIGHT_BACKWARD, 1);
}
void _left() {
  // analogWrite(MOTOR_LEFT_PWM, 85);
  // analogWrite(MOTOR_RIGHT_PWM, 85);
  digitalWrite(MOTOR_LEFT_FORWARD, 0);
  digitalWrite(MOTOR_RIGHT_FORWARD, 1);
  digitalWrite(MOTOR_LEFT_BACKWARD, 1);
  digitalWrite(MOTOR_RIGHT_BACKWARD, 0);
}
void _right() {
  //
  // analogWrite(MOTOR_LEFT_PWM, 85);
  // analogWrite(MOTOR_RIGHT_PWM, 85);
  digitalWrite(MOTOR_LEFT_FORWARD, 1);
  digitalWrite(MOTOR_RIGHT_FORWARD, 0);
  digitalWrite(MOTOR_LEFT_BACKWARD, 0);
  digitalWrite(MOTOR_RIGHT_BACKWARD, 1);
}
void _stop() {
  digitalWrite(MOTOR_LEFT_FORWARD, 0);
  digitalWrite(MOTOR_RIGHT_FORWARD, 0);
  digitalWrite(MOTOR_LEFT_BACKWARD, 0);
  digitalWrite(MOTOR_RIGHT_BACKWARD, 0);
}

void _rotate_right() {
  _right();
  delay(200);
  while (original_readings[2] < rotation_threshold && original_readings[3] < rotation_threshold && original_readings[4] < rotation_threshold) {
    take_readings();
    _right();
  }
}
void _rotate_left() {
  _left();
  delay(300);
  while (original_readings[0] < rotation_threshold && original_readings[1] < rotation_threshold && original_readings[2] < rotation_threshold) {
    take_readings();
    _left();
  }
}

/*
################################################
################################################extreme and front ir sensors code
################################################
*/

void take_extreme_ir_readings() {
  ext_left = digitalRead(EXTREME_LEFT_IR);
  ext_right = digitalRead(EXTREME_RIGHT_IR);
}

void take_front_ir_readings() {
  front_right_ir = digitalRead(FRONT_RIGHT_IR);
  front_center_ir = digitalRead(FRONT_CENTER_IR);
}

/*
#############################################
############################################# Detection functions for turns and deadends
#############################################
*/


bool T_point_found() {
  take_extreme_ir_readings();
  // if (ext_left == 1 && ext_right == 1)
  //   return true;
  // else
  //   return false;
  return ext_left && ext_right;
}
bool straight_found() {

  take_extreme_ir_readings();

  if (ext_left == 0 && ext_right == 0 && (original_readings[0] > rotation_threshold || original_readings[1] > rotation_threshold || original_readings[2] > rotation_threshold || original_readings[3] > rotation_threshold || original_readings[4] > rotation_threshold))
    return true;
  else
    return false;
}
bool right_found() {
  take_extreme_ir_readings();

  if (ext_right == 1 && (original_readings[2] > rotation_threshold || original_readings[3] > rotation_threshold || original_readings[4] > rotation_threshold))
    return true;
  else
    return false;
}
bool left_found() {
  take_extreme_ir_readings();

  if (ext_left == 1 && ext_right == 0 && (original_readings[0] > rotation_threshold || original_readings[1] > rotation_threshold || original_readings[2] > rotation_threshold))
    return true;
  else
    return false;
}

bool deadend_found() {
  take_extreme_ir_readings();

  if (ext_left == 0 && ext_right == 0 && (original_readings[0] < rotation_threshold && original_readings[1] < rotation_threshold && original_readings[2] < rotation_threshold && original_readings[3] < rotation_threshold && original_readings[4] < rotation_threshold))
    return true;
  else
    return false;
}


/*
##############################################
############################################## SETUP AND LOOP FUNCTIONS
##############################################


*/
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

  pinMode(EXTREME_LEFT_IR, INPUT);
  pinMode(EXTREME_RIGHT_IR, INPUT);
  pinMode(FRONT_RIGHT_IR, INPUT);
  pinMode(FRONT_CENTER_IR, INPUT);
}

void loop() {
  take_readings();
  take_extreme_ir_readings();
  take_front_ir_readings();

  // put your main code here, to run repeatedly
  // if (1)
  if (ext_left == 0 && ext_right == 0)
  // if (ext_left == 0 && ext_right == 0 && (original_readings[0] > rotation_threshold || original_readings[1] > rotation_threshold || original_readings[2] > rotation_threshold || original_readings[3] > rotation_threshold || original_readings[4] > rotation_threshold))
  // if(1)
  {

    PID_control();
  }


  else {
    delay(150);
    // _stop();
    // delay(2000);
    
    analogWrite(MOTOR_LEFT_PWM, 60);
    analogWrite(MOTOR_RIGHT_PWM, 58);
    // delay(40);

    take_extreme_ir_readings();
    take_front_ir_readings();
    /*****************************
******************************/
    //T point is encountered
    front_right_ir = 0;
    if (T_point_found()) {
      // delay(100);
      if (front_center_ir && front_right_ir) {  //later add

        delay(200);
        _stop();
      } else {
        delay(150);
        _rotate_left();
      }
    }
    //***********************************

    //if the right is encountered first
    else if (right_found()) {

      if (front_center_ir == 1) {
        //Straight found after Right
        _straight();
        // delay(200);

      } else if (front_center_ir == 0 && front_right_ir == 0) {  //Deadend found after right
        // delay(150);
        _rotate_right();
      }
    }
    //***********************************
    //Left Found first
    else if (left_found()) {
      // delay(150);
      _rotate_left();
    }
    //***********************************
    //Deadend Found
    else if (deadend_found()) {
      //Uturn after deadend
      u_turn();
    }
    //***********************************
  }
}

