//#include"Arduino.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/timers.h>  // Si vous utilisez des timers FreeRTOS
//#include "BasicStepperDriver.h"
//#include "MultiDriver.h"
//#include "SyncDriver.h"
#include <string>
#include "move.h"
/* 
#define MOTOR_STEPS 200
#define MOTOR_X_RPM 50
#define MOTOR_Y_RPM 50
#define DIR_X 2
#define STEP_X 5
#define DIR_Y 3
#define STEP_Y 6
#define MICROSTEPS 16
#define MOTOR_ACCEL 300
#define MOTOR_DECEL 300 

#define COEF_STRAIGHT 1.05 // coefficient pour compenser la perte d'aderence pour la fonction straight. 
#define COEF_ROTATE 1

BasicStepperDriver stepperR(MOTOR_STEPS, DIR_X, STEP_X);
BasicStepperDriver stepperL(MOTOR_STEPS, DIR_Y, STEP_Y);
SyncDriver controller(stepperR, stepperL);

float teta_actuelle = 225;
float x_position= 2250 ;
float y_position= 200;

float x_goal_position= 1850;
float y_goal_position= 650;
*/
const uint8_t sensorPinRight = A2;
const uint8_t sensorPinMidel = A6;
const uint8_t sensorPinLeft = A4;
int conteur = 0 ;
//long total_Steps_R = 0 ;
//long total_Steps_L = 0 ;
bool cancelGo_to = false ;

TaskHandle_t Task1;
TaskHandle_t Task2;

void Task1code(void *pvParameters);
void Task2code(void *pvParameters);
/* 
void straight(float distance_);
void rotate (float angle);
void go_to(float go_x, float go_y);*/
int capteur(int sensorPin); // Change to int
void evitement_droit();

void setup() {
  Serial.begin(9600); 
  configureMotors();
/* 
  stepperR.begin(MOTOR_X_RPM, MICROSTEPS);
  stepperL.begin(MOTOR_Y_RPM, MICROSTEPS);

  stepperR.setSpeedProfile(stepperR.LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL);
  stepperL.setSpeedProfile(stepperL.LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL);
*/
  //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
                    Task1code,   /* Task function. */
                    "Task1",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task1,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */                  
  delay(500); 

  //create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(
                    Task2code,   /* Task function. */
                    "Task2",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task2,      /* Task handle to keep track of created task */
                    1);          /* pin task to core 1 */
    delay(500); 
}

void Task1code( void * pvParameters ){
  for(;;){
    //Serial.println("core_2");
    //int sensor_M = capteur(sensorPinMidel);
    //int sensor_L = capteur(sensorPinLeft);
    //int sensor_R = capteur(sensorPinRight);

  //  Serial.print("R:");
   // Serial.println(sensor_R);
    //Serial.print(" M:");
    //Serial.print(sensor_M);
   // Serial.print(" L:");
   // Serial.println(sensor_L);
/* 
    if(sensor_M < 200){
      Serial.print("obstacle_midel");
      stepperR.stop(); stepperL.stop();
      //evitement_droit();
    }*/
    vTaskDelay(1/portTICK_PERIOD_MS);
  }
}
//Task2code: blinks an LED every 700 ms
void Task2code( void * pvParameters ){
  for (;;) {
    
    while(cancelGo_to){
     Serial.println("pas de go_to");
      vTaskDelay(50/portTICK_PERIOD_MS);
    }

    vTaskDelay(3000/portTICK_PERIOD_MS);

    if (conteur < 2){ 
    Serial.print("core_1");
    straight(1000);

    conteur ++;
    }
  }
}

void loop() {
  vTaskDelete(NULL);
  Serial.println("dans le loop");
}
/*
void straight(float distance_){
  int mm = MOTOR_STEPS * MICROSTEPS * COEF_STRAIGHT / 195 ;// 200*MICROSTEPS = périmètre de la roue (diamètre = 62)* M_PI = 195 mm
  int distance = distance_ * mm  ;
  Serial.print("distance");
  Serial.println(distance_);
 controller.move(distance, distance);
 total_Steps_R += stepperR.getStepsCompleted();
 total_Steps_L += stepperL.getStepsCompleted();
}

void rotate (float angle){
  float angl= angle * 480 / 360 ;
  Serial.print(" angle");
  Serial.println(angle);
  controller.rotate(angl, -angl);
  total_Steps_R += stepperR.getStepsCompleted();
  total_Steps_L += stepperL.getStepsCompleted();
}

void go_to(float go_x, float go_y){
  float teta_goal = 0;
  float teta_objectif = 0;
  float to_do_x = x_position - go_x ;
  float to_do_y = y_position - go_y ;
  Serial.print("to do_y");
  Serial.println(to_do_y);
  Serial.print("  to do_x ");
  Serial.println(to_do_x);

  int distance = sqrt(to_do_x*to_do_x + to_do_y*to_do_y );
  float teta_calcule = atan(abs(to_do_y) / abs(to_do_x));
  teta_calcule = abs(teta_calcule * 180 / M_PI);

    Serial.print("teta_calcule ");
    Serial.println(teta_calcule );

  if ((to_do_y > 0) && (to_do_x > 0)){
    teta_objectif = 90 + teta_calcule;
    Serial.println(" 90 + teta_calcule");
  }
  if ((to_do_y < 0) && (to_do_x > 0)){
    teta_objectif = 90 - teta_calcule;
    Serial.println(" 90 - teta_calcule");
  }
  if((to_do_y > 0) && (to_do_x < 0)){
  teta_objectif = 270 - teta_calcule ;
  Serial.println(" 270 - teta_calcule");
  }
  if((to_do_y < 0) && (to_do_x < 0)){
  teta_objectif = 270 + teta_calcule ;
  Serial.println(" 270 + teta_calcule");
  }

  Serial.print("teta_objectif ");
  Serial.println(teta_objectif);

teta_goal = teta_objectif - teta_actuelle;
if(teta_goal >= 180){
  teta_goal = teta_objectif - teta_actuelle - 360;
  Serial.println("teta_goal > 180");
} 

if (teta_goal < -180){
  teta_goal = teta_objectif - teta_actuelle + 360;
}
  Serial.print("teta_goal ");
  Serial.println(teta_goal);
  rotate(teta_goal );
  Serial.print("valeur distance  ");
  Serial.println(distance);
  straight(distance);
  Serial.print("fin du go_to");
  return;
}
*/
int capteur(int sensorPin){
  int16_t d ;
  int16_t t = pulseIn(sensorPin, HIGH);
// Serial.print(" t:");
// Serial.print(t);
// Serial.print("  ");
  if ((t > 1100) || (t==0))
  {
    Serial.print("obstacle trop loin > 450mm");
  }
  else if (t > 1850)
  {
    // No detection.
    Serial.println(-1);
  }
  else
  {
    // Valid pulse width reading. Convert pulse width in microseconds to distance in millimeters.
    d = (t - 992) * 4;
 
    // Limit minimum distance to 0.
    if (d < 0) { d = 0; } 
    //Serial.print(d);
    //Serial.println(" mm");
  }
  return d;
}

void debug_position(){
  Serial.print("stepperR.getStepsCompleted() ");
  Serial.print(stepperR.getStepsCompleted());
/* 
  total_Steps_R += stepperR.getStepsCompleted();
  total_Steps_L += stepperL.getStepsCompleted();
  Serial.print("total_Steps_R : ");
  Serial.print(total_Steps_R);
  Serial.print(" total_Steps_L : ");
  Serial.println(total_Steps_L);*/
}

void evitement_droit(){
  cancelGo_to = true;
  stepperR.stop(); stepperL.stop();
  //total_Steps_R += stepperR.getStepsCompleted();
  //total_Steps_L += stepperL.getStepsCompleted();
  rotate(45);
  straight(100);
  rotate(-45);
  cancelGo_to = false;
}

void save_position(){
 // total_Steps_R += stepperR.getStepsCompleted();
  //total_Steps_L += stepperL.getStepsCompleted();
}
