#include <Arduino.h>
#include "BasicStepperDriver.h"
#include "MultiDriver.h"
#include "SyncDriver.h"


// Motor steps per revolution. Most steppers are 200 steps or 1.8 degrees/step
#define MOTOR_STEPS 200
// Target RPM for X axis motor
#define MOTOR_X_RPM 200
// Target RPM for Y axis motor
#define MOTOR_Y_RPM 200

// X motor
#define DIR_X 2
#define STEP_X 5

// Y motor
#define DIR_Y 3
#define STEP_Y 6
int alpha =0;
// 1=full step, 2=half step etc.
#define MICROSTEPS 16

BasicStepperDriver stepperX(MOTOR_STEPS, DIR_X, STEP_X);
BasicStepperDriver stepperY(MOTOR_STEPS, DIR_Y, STEP_Y);
// MultiDriver controller(stepperX, stepperY);
// OR
// synchronized move, trajectory is a straight line
SyncDriver controller(stepperX, stepperY);

#define MOTOR_ACCEL 300
#define MOTOR_DECEL 300 
void setup() {

     
//Set target motors RPM.
Serial.begin(9600);

stepperX.begin(MOTOR_X_RPM, MICROSTEPS);
stepperY.begin(MOTOR_Y_RPM, MICROSTEPS);

//    stepper.setSpeedProfile(stepper.LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL);
    stepperX.setSpeedProfile(stepperX.LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL);
    stepperY.setSpeedProfile(stepperY.LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL);
    controller.rotate(142.5, -142.5);
}

void loop() {
if (alpha==0){ 
  controller.rotate(5500, -5500);
  delay(3000);
  long stepsRemaining = stepperX.stop();
  //Serial.println(alpha);
  //delay(1000);
  //controller.rotate(-30000 , 30000);
  //delay(1000);
  alpha++;
  }
  //Serial.println("yeas");
//controller.rotate(100, -100);

  long stepsCompleted = stepperX.getStepsCompleted();
  Serial.print("Nombre de pas parcourus : ");
  Serial.println(stepsCompleted);
}