#include "move.h"

long int total_Steps_R = 0;
long int total_Steps_L = 0;

float teta_actuelle = 0;
float x_position= 1900 ;
float y_position= 100;

//float x_goal_position= 1850;
//float y_goal_position= 650;

BasicStepperDriver stepperR(MOTOR_STEPS, DIR_X, STEP_X);
BasicStepperDriver stepperL(MOTOR_STEPS, DIR_Y, STEP_Y);
SyncDriver controller(stepperR, stepperL);

void configureMotors(){ 
stepperR.begin(MOTOR_X_RPM, MICROSTEPS);
stepperL.begin(MOTOR_Y_RPM, MICROSTEPS);
stepperR.setSpeedProfile(stepperR.LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL);
stepperL.setSpeedProfile(stepperL.LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL);
}

void straight(float distance_){
  if(distance_ < 1) { 
  Serial.print("erreur distance < 0");
  return ;
  }
  float mm = (MOTOR_STEPS * MICROSTEPS * COEF_STRAIGHT) / 195 ;// 200*MICROSTEPS = périmètre de la roue (diamètre = 62)* M_PI = 195 mm
  float distance = distance_ * mm  ;
  Serial.print("distance à parcourir");
  Serial.println(distance_);
  Serial.print("nombre de step à parcourir");
  Serial.println(distance);
  controller.move(distance, distance);
  Serial.println("la fin du straight");
  position();
}

void rotate (float angle){
  float angl= angle * 468 / 360 ;
  Serial.print(" angle");
  Serial.println(angle);
  controller.rotate(angl, -angl);
  teta_actuelle += angle;

  if (teta_actuelle >= 360.0f) {
    teta_actuelle -= 360.0f;
  } else if (teta_actuelle < 0.0f) {
    teta_actuelle += 360.0f;
  }
  Serial.print(" teta_actuelle + angle:");
  Serial.println(teta_actuelle);
  Serial.println("fin du rotate");
}

void stop(){
  stepperR.stop(); stepperL.stop();
}

bool moving(){ 
  if (controller.isRunning() == true){
    return true ;
  } else{
    return false ;
  }
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

  float distance = sqrt(to_do_x*to_do_x + to_do_y*to_do_y );
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
  Serial.println("fin du go_to");
  return;
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
  rotate(60);
  straight(130);
  //rotate(-60);
}

void evitement_gauche(){
  rotate(-60);
  straight(130);
}

void position(){
  int current_StepsR = stepperR.getStepsCompleted();
  int current_StepsL = stepperL.getStepsCompleted();
  total_Steps_R += current_StepsR ;
  total_Steps_L += current_StepsL ;
  float distance_x ;
  float distance_y ; 
 
  //  il faut verifier que les rotation n'ont pas d'influence car on utilise que la variable "current_StepsR"
  float distance = current_StepsR / ((MOTOR_STEPS * MICROSTEPS * COEF_STRAIGHT) / 195); // mm
  //Serial.print("current_StepsR");
  //Serial.println(current_StepsR);
  Serial.print(" distance parcouru:");
  Serial.println(distance);
// le changement du teta s'actualise dans la fonction rotate. 

  //float displacement_x = distance_traveled * cos(teta_actuelle * M_PI / 180.0f); // M_PI / 180.0f pour la convertion en radian
  //float displacement_y = distance_traveled * sin(teta_actuelle * M_PI / 180.0f);
  float teta_actuelle_ = fmod(teta_actuelle, 90.0f);
  Serial.print("fmod(teta_actuelle, 90.0f);");
  Serial.println(teta_actuelle_);

  if (0 <= teta_actuelle && teta_actuelle <= 90){
    distance_y = distance * cos(teta_actuelle_ * M_PI / 180.0f); // M_PI / 180.0f pour la convertion en radian
    distance_x = -distance * sin(teta_actuelle_ * M_PI / 180.0f);
    //distance_x = - distance_x;
  }
  if( 90 < teta_actuelle && teta_actuelle <= 180 ){
    distance_x = -distance * cos(teta_actuelle_ * M_PI / 180.0f); // M_PI / 180.0f pour la convertion en radian
    distance_y = -distance * sin(teta_actuelle_ * M_PI / 180.0f);
  }
    if( 180 < teta_actuelle && teta_actuelle <= 270 ){
    distance_y = -distance * cos(teta_actuelle_ * M_PI / 180.0f); // M_PI / 180.0f pour la convertion en radian
    distance_x = distance * sin(teta_actuelle_ * M_PI / 180.0f);
  }
    if( 270 < teta_actuelle && teta_actuelle <= 360 ){
    Serial.print("270 < teta_actuelle && (teta_actuelle <= 360 || teta_actuelle == 0)");
    distance_x = distance * cos(teta_actuelle_ * M_PI / 180.0f); // M_PI / 180.0f pour la convertion en radian
    distance_y = distance * sin(teta_actuelle_ * M_PI / 180.0f);
  }

  Serial.print("distance_x");
  Serial.print(distance_x);
  Serial.print(" distance_y");
  Serial.println(distance_y);

  x_position += distance_x;
  y_position += distance_y;

  Serial.print(" position x ");
  Serial.print(x_position);
  Serial.print(" position y ");
  Serial.println(y_position);
}
