/*
# PROGRAM STRUCTURE
-> MACROS
-> GLOBAL DECLARATIONS
-> BASIC MOVEMENT FUNCTIONS
-> LINE SENSOR FUNCTIONS
-> SETUP FUNCTION
-> LOOP FUNCTION

*/
#define MOTOR_RIGHT_FORWARD 11
#define MOTOR_RIGHT_BACKWARD 10
#define MOTOR_RIGHT_PWM 5

#define MOTOR_LEFT_FORWARD 8
#define MOTOR_LEFT_BACKWARD 9
#define MOTOR_LEFT_PWM 6

// variables initialization for the sensors that follow the line
int line_keeper_sensors[] = { A0, A1, A2, A4, A5 };  //A3 not being used
int positions[] = { -2, -1, 0, 0, 2 };
bool line_keepers_detects_black[] = { 0, 0, 0, 0, 0 };
int line_keepers_sum;

/***************************************
#######################################################
INCLUDED:
  CHOOSE MOVEMENT
  STRAIGHT 
  BACKWARD
  LEFT
  RIGHT 
#######################################################
****************************************/
void choose_movement() {
  if (line_keepers_sum == 0) {
    _straight();
  } else if (line_keepers_sum < 0) {
    _left();
  } else {
    _right();
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
  digitalWrite(MOTOR_LEFT_FORWARD, 0);
  digitalWrite(MOTOR_RIGHT_FORWARD, 1);
  digitalWrite(MOTOR_LEFT_BACKWARD, 1);
  digitalWrite(MOTOR_RIGHT_BACKWARD, 0);
}
void _right() {
  digitalWrite(MOTOR_LEFT_FORWARD, 1);
  digitalWrite(MOTOR_RIGHT_FORWARD, 0);
  digitalWrite(MOTOR_LEFT_BACKWARD, 0);
  digitalWrite(MOTOR_RIGHT_BACKWARD, 1);
}
/***************************************
#######################################################
UPDATE SENSORS FOR LINE KEEPING AND ALL THE DATA RELATED TO THEM
 
#######################################################
****************************************/
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
/***************************************
#######################################################
SETUP FUNCTION
INTIALIZING THE PIN MODES AND INITAL CONDITIONS
#######################################################
****************************************/
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
  analogWrite(MOTOR_LEFT_PWM, 110);
  analogWrite(MOTOR_RIGHT_PWM, 110);
  Serial.begin(9600);
}

/***************************************
#######################################################
MAIN LOOP FUNCTION 

#######################################################
****************************************/

void loop() {
  update_line_keepers();
  
  choose_movement();
  // _left();
  // Serial.println(line_keepers_sum);
  // delay(1000);
  // digitalWrite(MOTOR_RIGHT_FORWARD, 1);
  // delay(1000);
  // digitalWrite(MOTOR_RIGHT_FORWARD, 0);

}