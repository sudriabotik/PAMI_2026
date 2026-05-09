#ifndef MOVE_H
#define MOVE_H
#include"Arduino.h"
#include "BasicStepperDriver.h"
#include "MultiDriver.h"
#include "SyncDriver.h"
#include "config_robots.hh"
#include <math.h>
#include <cmath>
#include <string>

#define MOTOR_STEPS 200
#define MOTOR_RPM MOTOR_RPM_ 
 

// changer cette valeur à 1 a l'air de casser le calcul de distance et la puissance des moteurs
#define MICROSTEPS 16

#define MOTOR_ACCEL MOTOR_ACCEL_
#define MOTOR_DECEL MOTOR_DECEL_
#define MOTOR_DECEL_FINISH MOTOR_DECEL_FINISH_ // à test 

#define MOTOR_ACCEL_DECEL_ROTATE MOTOR_ACCEL_DECEL_ROTATE_ // 700

#define X_POSITION_START X_POSITION_START_
#define Y_POSITION_START  Y_POSITION_START_
#define TETA_POSITION_START  TETA_POSITION_START_

extern BasicStepperDriver stepperR;
extern BasicStepperDriver stepperL;
//extern SyncDriver controller(stepperR, stepperL);

void straight(float distance_);
void rotation (float angle);
void orientation(float angle_absolue);
void go_to(float go_x, float go_y);
void configureMotors();
void debug_position();
void evitement_droit();
void evitement_gauche();
void grand_evitement_droit();
void grand_evitement_gauche();
void position();
bool moving();
void stop();

void SetRPM(int rpm);
void Set_Decelerate(int decel);

extern float x_position ;
extern float y_position ;
extern float teta_actuelle;

extern long int total_Steps_R;
extern long int total_Steps_L;

/*
-1 : no avoidance
0 : sensors check for avoidance
1 : have to do an avoidance turning to the right
2 : have to do an avoidance turning to the left
3 : have to do an avoidance turning twice as much to the right
4 : have to do an avoidance turning twice as much to the left
*/
extern int evitement;

//extern float x_goal_position ;
//extern float y_goal_position ;

#endif