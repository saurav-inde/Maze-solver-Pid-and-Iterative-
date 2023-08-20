#define MOTOR_RIGHT_FORWARD 10
#define MOTOR_RIGHT_BACKWARD 11
#define MOTOR_RIGHT_PWM 6

#define MOTOR_LEFT_FORWARD 8
#define MOTOR_LEFT_BACKWARD 9
#define MOTOR_LEFT_PWM 5

#define EXTREME_LEFT_IR A5
#define EXTREME_RIGHT_IR 12

// #define FRONT_LEFT_IR A3
#define FRONT_RIGHT_IR 3
#define FRONT_CENTER_IR 4

int line_keeper_sensors[] = { A0, A1, A2, A3, A4};  //A3 not being used
int positions[] = { -2, -1, 0, 0, 2 };
bool line_keepers_detects_black[] = { 0, 0, 0, 0, 0};
int line_keepers_sum;
int ext_left;
int ext_right;
// int front_left_ir;
int front_right_ir;
int front_center_ir;

void choose_movement() {
  // if (line_keepers_detects_black[0] == 0 && line_keepers_detects_black[1] == 0 && line_keepers_detects_black[2] == 0 && line_keepers_detects_black[3] == 0 && line_keepers_detects_black[4] == 0) {
  //   u_turn();
  // }
  if (line_keepers_sum == 0) {
    _straight();
  } else if (line_keepers_sum < 0) {
    _left();
  } else {
    _right();
  }
}

void u_turn() {
  while (!(line_keepers_detects_black[0] == 1 || line_keepers_detects_black[1] == 1 || line_keepers_detects_black[2] == 1 || line_keepers_detects_black[3] == 1 || line_keepers_detects_black[4] == 1)) {
    update_line_keepers();
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
  while (!(line_keepers_detects_black[1] == 1 || line_keepers_detects_black[2] == 1 || line_keepers_detects_black[3] == 1)) {
    update_line_keepers();
    _right();
  }
}
void _rotate_left() {
  _left();
  delay(200);
  while (!(line_keepers_detects_black[1] == 1 || line_keepers_detects_black[2] == 1 || line_keepers_detects_black[3] == 1)) {
    update_line_keepers();
    _left();
  }
}

void update_line_keepers() {
  for (int i = 0; i < 5; i++) {
    line_keepers_detects_black[i] = digitalRead(line_keeper_sensors[i]);
  }
  _weighted_sum_line_keepers();
}

void _weighted_sum_line_keepers() {
  line_keepers_sum = 0;
  for (int i = 0; i < 5; i++) {
    line_keepers_sum += line_keepers_detects_black[i] * positions[i];
  }
}   

void extreme_ir() {
  ext_left = digitalRead(EXTREME_LEFT_IR);
  ext_right = digitalRead(EXTREME_RIGHT_IR);
}

void front_ir(){
  // front_left_ir = digitalRead(FRONT_LEFT_IR);
  front_right_ir = digitalRead(FRONT_RIGHT_IR); 
  front_center_ir = digitalRead(FRONT_CENTER_IR); 
}
bool T_point_found() {
  extreme_ir();
  if (ext_left == 1 && ext_right == 1)
    return true;
  else
    return false;
}
bool straight_found() {
  extreme_ir();
  if (ext_left == 0 && ext_right == 0 && (line_keepers_detects_black[0] == 1 || line_keepers_detects_black[1] == 1 || line_keepers_detects_black[2] == 1 || line_keepers_detects_black[3] == 1 || line_keepers_detects_black[4] == 1))
    return true;
  else
    return false;
}
bool right_found() {
  extreme_ir();
  if (ext_right == 1 && ext_left == 0 && (line_keepers_detects_black[2] == 1 || line_keepers_detects_black[3] == 1 || line_keepers_detects_black[4] == 1))
    return true;
  else
    return false;
}
bool left_found() {
  extreme_ir();
  if (ext_left == 1 && ext_right == 0 && (line_keepers_detects_black[0] == 1 || line_keepers_detects_black[1] == 1 || line_keepers_detects_black[2] == 1))
    return true;
  else
    return false;
}

bool deadend_found() {
  extreme_ir();
  if (ext_left == 0 && ext_right == 0 && (line_keepers_detects_black[0] == 0 && line_keepers_detects_black[1] == 0 && line_keepers_detects_black[2] == 0 && line_keepers_detects_black[3] == 0 && line_keepers_detects_black[4] == 0))
    return true;
  else
    return false;
}

void setup() {
  //setting up the motor pins
  pinMode(MOTOR_LEFT_PWM, OUTPUT);
  pinMode(MOTOR_LEFT_FORWARD, OUTPUT);
  pinMode(MOTOR_LEFT_BACKWARD, OUTPUT);

  pinMode(MOTOR_RIGHT_PWM, OUTPUT);
  pinMode(MOTOR_RIGHT_FORWARD, OUTPUT);
  pinMode(MOTOR_RIGHT_BACKWARD, OUTPUT);


  //setting up the linee keeper sensors
  for (auto pin : line_keeper_sensors) {
    pinMode(pin, INPUT);
  }

  //initalizing the pwm values for the each of the motors
  analogWrite(MOTOR_LEFT_PWM, 55);
  analogWrite(MOTOR_RIGHT_PWM, 55);
  Serial.begin(9600);
}

void loop() {
  update_line_keepers();
  extreme_ir();
  if (ext_left == 0 && ext_right == 0 && (line_keepers_detects_black[0] == 1 || line_keepers_detects_black[1] == 1 || line_keepers_detects_black[2] == 1 || line_keepers_detects_black[3] == 1 || line_keepers_detects_black[4] == 1)) {
    choose_movement();
  } 
  else {
    if (T_point_found()) {
      // _straight();/
      // delay(200);
      front_ir();
      if(front_right_ir==1 && front_center_ir==1)      
      // digitalWrite(2, HIGH);
      // update_line_keepers();
      // extreme_ir();
      // if (T_point_found()) {
        _stop();
      // }
       else if (front_center_ir==1 && front_right_ir==0) {   // Straight found after T
        // _backward();
        // delay(200);
        _rotate_left();
      } 
      else if (front_center_ir==0 && front_right_ir==0) {    //Deadend found after T 
        // digitalWrite(2, LOW);
        // digitalWrite(3, HIGH);
        // _backward();
        // delay(200);
        _rotate_left();
      }
    } else if (right_found()) {
      // delay(200);
      front_ir();
      // _straight();
      // delay(50);
      // update_line_keepers();
      // extreme_ir();
      // digitalWrite(3, LOW);
      // digitalWrite(2, HIGH);
      if (front_center_ir==1 && front_right_ir==0) {   //Straight found after Right
        // digitalWrite(2, LOW);
        // digitalWrite(3, HIGH);
        // continue;
        _straight();   //Move Straight if straigh found after right
      } 
      else if (front_center_ir==0 && front_right_ir==0) {   //Deadend found after right
        // delay(100);
        // digitalWrite(3, LOW);
        // digitalWrite(2, HIGH);

        _rotate_right();    //Rotate right if deadend found
      }
    } else if (left_found()) {   //Left Found
      // delay(200);
      _rotate_left();
    } 
    else if (deadend_found()) {   //Deadend Found
      // digitalWrite(2, HIGH);
      // digitalWrite(3, HIGH);
      u_turn();  //Uturn after deadend
    }
  }
}