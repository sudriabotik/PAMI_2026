#ifndef MOVE_H
#define MOVE_H
#include"Arduino.h"
//#include <freertos/FreeRTOS.h>
//#include <freertos/task.h>
//#include <freertos/timers.h>  // Si vous utilisez des timers FreeRTOS
#include "BasicStepperDriver.h"
#include "MultiDriver.h"
#include "SyncDriver.h"
#include <math.h>
#include <cmath>
#include <string>

#define MOTOR_STEPS 200
#define MOTOR_X_RPM 170
#define MOTOR_Y_RPM 170
#define DIR_X 2
#define STEP_X 5
#define DIR_Y 3
#define STEP_Y 6
#define MICROSTEPS 16
#define MOTOR_ACCEL 1000
#define MOTOR_DECEL 1000

#define COEF_STRAIGHT 1.006 // coefficient pour compenser la perte d'aderence pour la fonction straight. 
#define COEF_ROTATE 1

extern BasicStepperDriver stepperR;
extern BasicStepperDriver stepperL;
//extern SyncDriver controller(stepperR, stepperL);

void straight(float distance_);
void rotate (float angle);
void go_to(float go_x, float go_y);
void configureMotors();
void debug_position();
void evitement_droit();
void evitement_gauche();
void position();
bool moving();
void stop();

extern float x_position ;
extern float y_position ;
extern float teta_actuelle;

extern long int total_Steps_R;
extern long int total_Steps_L;

//extern float x_goal_position ;
//extern float y_goal_position ;

#endif