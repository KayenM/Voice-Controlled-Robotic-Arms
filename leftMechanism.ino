#include <Servo.h>

Servo gripper; 
Servo ax1;
Servo ax2;

//Variables to store the servo angle associated with the gripper open and closed positions
int gr_open;
int gr_close;

//Variables to store the servo angles associated with the home location
float ax1_home;
float ax2_home;

//Variables to store the servo angles associated with the current location
float ax1_current;
float ax2_current;

// What is the current target and next target position
  int currpos;
  int nextpos;
 

// Update the current position axes
  float ax1_curtar;
  float ax2_curtar;

// Update the next target position
  float ax1_nexttar;
  float ax2_nexttar;

//Variables to store the servo angles for target 1
float ax1_target1;
float ax2_target1;

//Variables to store the servo angles for target 2
float ax1_target2;
float ax2_target2;

//Variables to store the servo angles for target 3
float ax1_target3;
float ax2_target3;

//---------Communication Bus Start------------------
const int input1 = 7;
const int input2 = 8;
int output1;
int output2;
//---------Communication Bus End------------------


//function given to move to new angle
void moveTopos(float ax1s, float ax1e, float ax2s, float ax2e) {
  /*
   This function taskes the starting and ending angles for both axes and
   divides them into an even number of steps. This ensures that the movement
   from both motors occurs simultaneously. It also reduces the speed of
   movement to help protect the mechanism. This is a workable but sub-optimal
   way of moving the mechanism.

   ax1s -> Axis 1 Start Position
   ax1e -> Axis 1 End Position
   ax2s -> Axis 2 Start Position
   ax2e -> Axis 2 End Position
   */
  float num_steps_1 = ax1e - ax1s;
  float num_steps_2 = ax2e - ax2s;
  float num_steps = 0;

  if (abs(num_steps_1) >= abs(num_steps_2)){
    num_steps = abs(num_steps_1);
  }
  else {
     num_steps = abs(num_steps_2);
  }
  float inc1 = num_steps_1 / num_steps;
  float inc2 = num_steps_2 / num_steps;
  float stp = 0;
  for(stp = 0; stp<=num_steps; stp += 1){
    ax1_current += inc1;
    ax1.write(ax1_current);
    ax2_current += inc2;
    ax2.write(ax2_current);
    delay(25);
  }
  ax1_current = ax1e;
  ax1.write(ax1_current);
  ax2_current = ax2e;
  ax2.write(ax2_current);
  delay(25);
}


void setup() {
  //What pins are the servos attached to?
  gripper.attach(12); 
  ax1.attach(10);
  ax2.attach(11);

//---------Communication Bus Start------------------
  pinMode(input1, INPUT);
  pinMode(input2, INPUT);

  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
//---------Communication Bus END------------------

  //set home angle values
  ax1_home = 40;
  ax2_home = 40;

  
  //Setting arm to the home position for the start of function
  ax1_current = ax1_home;
  ax2_current = ax2_home;

  //target 1 motor angles defined with offset in mind
  ax1_target1 = 57;
  ax2_target1 = 96;

  //target 2 motor angles defined with offset in mind
  ax1_target2 = 77;
  ax2_target2 = 116;

  //target 3 motor angles defined with offset in mind
  ax1_target3 = 102;
  ax2_target3 = 146;

//---------Communication Bus Start------------------
  if ((digitalRead(input1) == HIGH) && (digitalRead(input2) == LOW)){
    ax1_curtar = ax1_target1;
    ax2_curtar = ax2_target1;
  }
  else if ((digitalRead(input1) == LOW) && (digitalRead(input2) == HIGH)) {
    ax1_curtar = ax1_target2;
    ax2_curtar = ax2_target2;
  }
  else if ((digitalRead(input1) == LOW) && (digitalRead(input2) == LOW)) {
    ax1_curtar = ax1_target3;
    ax2_curtar = ax2_target3;
  }
//---------Communication Bus End------------------


/*
//---------Comment out when using communication bus (Start)--------------
// Set the current target
  currpos = 2;

// if current is 1, update the current position of the motor to the specified angles
  if (currpos == 1){
    ax1_curtar = ax1_target1;
    ax2_curtar = ax2_target1;
  }
  // else do the same if the current is 2
  else if (currpos == 2) {
    ax1_curtar = ax1_target2;
    ax2_curtar = ax2_target2;
  }
  // else do the same if the current is 3
  else if (currpos == 3) {
    ax1_curtar = ax1_target3;
    ax2_curtar = ax2_target3;
  }
//---------Comment out when using communication bus (End)--------------
*/


 // Set the next target
  int target = 1;

  if (target == 1){
    ax1_nexttar = ax1_target1;
    ax2_nexttar = ax2_target1;

//---------Communication Bus Start------------------
    digitalWrite(output1, HIGH);
    digitalWrite(output2, LOW);
//---------Communication Bus End------------------
  }
  else if (target == 2) {
    ax1_nexttar = ax1_target2;
    ax2_nexttar = ax2_target2;

//---------Communication Bus Start------------------
    digitalWrite(output1, LOW);
    digitalWrite(output2, HIGH);
//---------Communication Bus End------------------
  }
  else if (target == 3) {
    ax1_nexttar = ax1_target3;
    ax2_nexttar = ax2_target3;
//---------Communication Bus Start------------------
    digitalWrite(output1, LOW);
    digitalWrite(output2, LOW);
//---------Communication Bus End------------------
  } 
  //init to home at start of program
  ax1.write(ax1_current);
  ax2.write(ax2_current);

  //Setting the gripper to open and close position angles
   gr_open = 0;
   gr_close = 150;
}

void loop() {
  //currpos = Serial.parseInt();
  
  //open the gripper
  gripper.write(gr_open);
  delay(3000);

  //Move the mechanism to target to pick up the object
  moveTopos(ax1_home, ax1_curtar, ax2_home, ax2_curtar);
  delay(3000);


  //close the gripper
  gripper.write(gr_close);
  delay(3000);

  //Move the mechanism to the next position
   moveTopos(ax1_curtar, ax1_nexttar, ax2_curtar, ax2_nexttar);
   delay(3000);

  //open the gripper to drop the object
  gripper.write(gr_open);
  delay(3000);

  //Move the arm back home position
   moveTopos(ax1_nexttar, ax1_home, ax2_nexttar, ax2_home);
   delay(3000);

  //close the gripper
 // gripper.write(gr_close);
 // delay(3000);

//---------Communication Bus Start------------------
  //send the new state back to outputs
  digitalWrite(output1, digitalRead(input1));
  digitalWrite(output2, digitalRead(input2));
//---------Communication Bus End------------------
}
