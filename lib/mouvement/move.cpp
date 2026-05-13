#include "move.h"

long int total_Steps_R = 0;
long int total_Steps_L = 0;

float x_position = X_POSITION_START;
float y_position = Y_POSITION_START;
float teta_actuelle = TETA_POSITION_START;

int evitement = 0 ;

BasicStepperDriver stepperR(MOTOR_STEPS, DIR_X, STEP_X);
BasicStepperDriver stepperL(MOTOR_STEPS, DIR_Y, STEP_Y);
SyncDriver controller(stepperR, stepperL);

void configureMotors(){ 
stepperR.begin(MOTOR_RPM, MICROSTEPS);
stepperL.begin(MOTOR_RPM, MICROSTEPS);
stepperR.setSpeedProfile(stepperR.LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL);
stepperL.setSpeedProfile(stepperL.LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL);
}

void straight(float distance_){  // positive and negatif value allowed
  Serial.println("debut du straight");

  // COEF_DROIT biaise les pas de la roue droite pour compenser l'asymétrie physique des roues
  float steps_per_mm = (MOTOR_STEPS * MICROSTEPS) / (DIAMETRE_ROUE * M_PI);

  float steps_R = distance_ * steps_per_mm * COEF_DROIT;
  float steps_L = distance_ * steps_per_mm;

  Serial.print("distance à parcourir:");
  Serial.print(distance_);
  Serial.print(" steps R:");
  Serial.print(steps_R);
  Serial.print(" steps L:");
  Serial.println(steps_L);

  controller.move(steps_R, steps_L);
  position();
  Serial.println("end function straight");
}

// Démarre un straight non-bloquant, rend la main une fois en CRUISING.
// Doit être suivi par straight_extend(...) (un ou plusieurs appels) pour terminer.
void straight_continue(float distance_){
  Serial.println("debut du straight_continue");
  float steps_per_mm = (MOTOR_STEPS * MICROSTEPS) / (DIAMETRE_ROUE * M_PI);
  long steps_R = (long)(distance_ * steps_per_mm * COEF_DROIT);
  long steps_L = (long)(distance_ * steps_per_mm);

  controller.startMove(steps_R, steps_L);
  // pompe jusqu'à entrer en CRUISING ; si le segment est trop court on pompe jusqu'à la fin
  while (controller.isRunning() &&
         stepperR.getCurrentState() != BasicStepperDriver::CRUISING) {
    controller.nextAction();
  }
}

// Ajoute des pas au mouvement en cours sans toucher au profil de vitesse,
// puis pompe jusqu'à l'arrêt (décélération naturelle à la fin).
void straight_extend(float distance_){
  Serial.println("debut du straight_extend");
  float steps_per_mm = (MOTOR_STEPS * MICROSTEPS) / (DIAMETRE_ROUE * M_PI);
  long steps_R = (long)(distance_ * steps_per_mm * COEF_DROIT);
  long steps_L = (long)(distance_ * steps_per_mm);

  controller.alterMove(steps_R, steps_L);
  while (controller.isRunning()) controller.nextAction();
  position();
  Serial.println("end function straight_extend");
}

/**
 * @param angle the angle relative, in trigonometric direction , in degrees.
 */
void rotation (float angle){
  Serial.println("start fonction rotation");
  int sauvegarde_evitement;
  sauvegarde_evitement = evitement ;
  evitement = -1;

  stepperR.setSpeedProfile(stepperR.LINEAR_SPEED, MOTOR_ACCEL_DECEL_ROTATE, MOTOR_ACCEL_DECEL_ROTATE);
  stepperL.setSpeedProfile(stepperL.LINEAR_SPEED, MOTOR_ACCEL_DECEL_ROTATE, MOTOR_ACCEL_DECEL_ROTATE);

  float angl = angle  * (ENTRE_AXE / DIAMETRE_ROUE);

  Serial.print(" rotation_of ...:");
  Serial.println(angle);
  controller.rotate(-angl, angl);
  teta_actuelle += angle;

  if (teta_actuelle >= 360.0f) {
    teta_actuelle -= 360.0f;
  } else if (teta_actuelle < 0.0f) {
    teta_actuelle += 360.0f;
  }
  Serial.print(" nouvelle orientation");
  Serial.println(teta_actuelle);

  stepperR.setSpeedProfile(stepperR.LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL);
  stepperL.setSpeedProfile(stepperL.LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL);

  evitement = sauvegarde_evitement ;
  Serial.println("end fonction rotation");
}

/**
 * @param angle_absolue the absolute angle to reach, in trigonometric direction, in degrees (0-360).
 * Rotates the robot to face the specified absolute angle relative to the field coordinate system.
 */
void orientation(float angle_absolue){
  Serial.println("start fonction orientation (absolute)");
  Serial.print(" angle_absolue demandé:");
  Serial.println(angle_absolue);
  Serial.print(" teta_actuelle:");
  Serial.println(teta_actuelle);
  
  // Calculer l'angle relatif à tourner
  float angle_relatif = angle_absolue - teta_actuelle;
  
  // Normaliser l'angle pour prendre le chemin le plus court
  if (angle_relatif >= 180.0f){
    angle_relatif = angle_relatif - 360.0f;
  } 
  else if (angle_relatif < -180.0f){
    angle_relatif = angle_relatif + 360.0f;
  }
  
  Serial.print(" angle_relatif à faire:");
  Serial.println(angle_relatif);
  
  // Effectuer la rotation relative
  rotation(angle_relatif);
  
  Serial.println("end fonction orientation (absolute)");
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
  Serial.print("start fonction go_to, go_x:");
  Serial.print(go_x);
  Serial.print(" go_y:");
  Serial.println(go_y);
  float teta_to_do = 0;
  float teta_objectif = 0;
  float to_do_x = go_x - x_position;
  float to_do_y = go_y - y_position; // on rajoute un moins pour que l'axe y soit inverser. l'axe y sur le vinil de surdiabotique est inverser par rapport au cercle trigo. 
  Serial.print(" to do_y:");
  Serial.print(to_do_y);
  Serial.print(" to do_x:");
  Serial.print(to_do_x);

  float distance = sqrt(to_do_x*to_do_x + to_do_y*to_do_y );
  //float teta_calcule = atan(abs(to_do_y) / abs(to_do_x));
  float teta_calcule = atan2(to_do_y, to_do_x);
  teta_objectif = (teta_calcule * 180 / M_PI); // convert theta to degrees

  Serial.print(" teta_objectif:");
  Serial.print(teta_objectif);

  teta_to_do = teta_objectif - teta_actuelle;

  if(teta_to_do >= 180){
    teta_to_do = teta_to_do - 360;
    //Serial.print(" teta_goal > 180");
  } 

  if (teta_to_do < -180){
    teta_to_do = teta_to_do + 360;
  }
  Serial.print(" teta_to_do:");
  Serial.print(teta_to_do);
  rotation(teta_to_do);
  //evitement = 0;
  //Serial.print("evitement: ");
  //Serial.print(evitement);
  //Serial.print(" valeur distance:");
  //Serial.println(distance);
  straight(distance);
  Serial.println("end fonction go_to");
  return;
}

// Calcule steps_R/L et rpm_R/L pour un arc de rayon `rayon` (mm) et `angle_deg`.
// Signe de angle_deg : positif = horaire (roue L extérieure), négatif = anti-horaire.
// Renvoie le rayon effectivement utilisé (clamp si trop petit).
static float arc_compute(float rayon, float angle_deg,
                         long *steps_R, long *steps_L,
                         float *rpm_R,  float *rpm_L){
  float E   = ENTRE_AXE;
  float spm = (MOTOR_STEPS * MICROSTEPS) / (DIAMETRE_ROUE * M_PI);

  if (rayon <= E / 2.0f){
    Serial.print("arc: rayon trop petit (");
    Serial.print(rayon);
    Serial.print(" mm), clamp a ");
    rayon = E / 2.0f + 1.0f;
    Serial.println(rayon);
  }

  float theta_rad = fabsf(angle_deg) * (float)M_PI / 180.0f;
  int   dir       = (angle_deg >= 0.0f) ? 1 : -1;

  float len_outer = (rayon + E / 2.0f) * theta_rad;
  float len_inner = (rayon - E / 2.0f) * theta_rad;

  float rpm_outer = MOTOR_RPM * (rayon + E / 2.0f) / rayon;
  float rpm_inner = MOTOR_RPM * (rayon - E / 2.0f) / rayon;

  if (dir > 0){
    // Horaire : L = exterieur, R = interieur
    *steps_L = (long)(len_outer * spm);
    *steps_R = (long)(len_inner * spm * COEF_DROIT);
    *rpm_L   = rpm_outer;
    *rpm_R   = rpm_inner;
  } else {
    // Anti-horaire : R = exterieur, L = interieur
    *steps_R = (long)(len_outer * spm * COEF_DROIT);
    *steps_L = (long)(len_inner * spm);
    *rpm_R   = rpm_outer;
    *rpm_L   = rpm_inner;
  }
  return rayon;
}

// Met à jour x_position, y_position, teta_actuelle pour un arc déjà parcouru.
// Convention cos/sin(teta) cohérente avec position() (move.cpp:256-257).
static void arc_apply_odometry(float rayon, float angle_deg){
  float theta_rad = fabsf(angle_deg) * (float)M_PI / 180.0f;
  float chord     = 2.0f * rayon * sinf(theta_rad / 2.0f);
  float mid_teta  = teta_actuelle + angle_deg / 2.0f;
  x_position += chord * cosf(mid_teta * (float)M_PI / 180.0f);
  y_position += chord * sinf(mid_teta * (float)M_PI / 180.0f);
  teta_actuelle += angle_deg;
  if (teta_actuelle >= 360.0f) teta_actuelle -= 360.0f;
  else if (teta_actuelle < 0.0f) teta_actuelle += 360.0f;
}

// Arc de cercle bloquant. Centre du robot à vitesse v = pi*D*MOTOR_RPM/60 (idem straight).
// rayon : mm (positif), angle_deg : signe = direction (positif = horaire).
void arc(float rayon, float angle_deg){
  Serial.print("debut du arc, rayon:");
  Serial.print(rayon);
  Serial.print(" angle:");
  Serial.println(angle_deg);

  long  steps_R, steps_L;
  float rpm_R,   rpm_L;
  float r_eff = arc_compute(rayon, angle_deg, &steps_R, &steps_L, &rpm_R, &rpm_L);

  stepperR.setRPM(rpm_R);
  stepperL.setRPM(rpm_L);

  Serial.print(" steps R:");
  Serial.print(steps_R);
  Serial.print(" steps L:");
  Serial.print(steps_L);
  Serial.print(" rpm R:");
  Serial.print(rpm_R);
  Serial.print(" rpm L:");
  Serial.println(rpm_L);

  controller.move(steps_R, steps_L);
  arc_apply_odometry(r_eff, angle_deg);

  // Restaure le RPM nominal pour ne pas polluer les straight() suivants.
  stepperR.setRPM(MOTOR_RPM);
  stepperL.setRPM(MOTOR_RPM);

  Serial.print(" new x:");
  Serial.print(x_position);
  Serial.print(" y:");
  Serial.print(y_position);
  Serial.print(" teta:");
  Serial.println(teta_actuelle);
  Serial.println("end function arc");
}

// Démarre un arc non-bloquant, rend la main une fois en CRUISING.
// Doit être suivi par arc_extend(...) avec MÊME rayon et MÊME signe d'angle.
// Mélanger straight_continue/extend et arc_continue/extend dans la même chaîne
// n'est pas supporté (alterMove ne change pas le profil de vitesse).
void arc_continue(float rayon, float angle_deg){
  Serial.println("debut du arc_continue");
  long  steps_R, steps_L;
  float rpm_R,   rpm_L;
  float r_eff = arc_compute(rayon, angle_deg, &steps_R, &steps_L, &rpm_R, &rpm_L);

  stepperR.setRPM(rpm_R);
  stepperL.setRPM(rpm_L);

  controller.startMove(steps_R, steps_L);
  // Met à jour l'odométrie immédiatement (le mouvement est lancé, on suppose
  // qu'il se terminera). Comme ça straight_alter/ramp_to qui suivent calculent
  // dans le bon cap. La formule chord se décompose linéairement, donc
  // arc_continue(R, a) + arc_extend(R, b) donne le même résultat odométrique
  // que arc(R, a+b).
  arc_apply_odometry(r_eff, angle_deg);

  while (controller.isRunning() &&
         stepperR.getCurrentState() != BasicStepperDriver::CRUISING){
    controller.nextAction();
  }
}

// Ajoute des pas à l'arc en cours sans toucher au profil de vitesse,
// puis pompe jusqu'à l'arrêt. Met à jour l'odométrie pour SON segment uniquement.
void arc_extend(float rayon, float angle_deg){
  Serial.println("debut du arc_extend");
  long  steps_R, steps_L;
  float rpm_R,   rpm_L;
  float r_eff = arc_compute(rayon, angle_deg, &steps_R, &steps_L, &rpm_R, &rpm_L);

  controller.alterMove(steps_R, steps_L);
  while (controller.isRunning()) controller.nextAction();

  arc_apply_odometry(r_eff, angle_deg);

  stepperR.setRPM(MOTOR_RPM);
  stepperL.setRPM(MOTOR_RPM);

  Serial.print(" new x:");
  Serial.print(x_position);
  Serial.print(" y:");
  Serial.print(y_position);
  Serial.print(" teta:");
  Serial.println(teta_actuelle);
  Serial.println("end function arc_extend");
}

void debug_position(){
  Serial.print("stepperR.getStepsCompleted()");
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
  Serial.println("start evitement droit");
  rotation(70);
  straight(150);
  Serial.println("end evitement droit ");
}

void evitement_gauche(){
  Serial.println("start evitement gauche");
  rotation(-70);
  straight(150);
  Serial.println("end evitement gauche ");
}

void grand_evitement_droit()
{
  Serial.println("start grand evitement droit");
  rotation(140);
  straight(150);
}

void grand_evitement_gauche()
{
  Serial.println("start grand evitement gauche");
  rotation(-140);
  straight(150);
}

void position(){
  Serial.println("start fonction position");
  int current_StepsR = stepperR.getStepsCompleted();
  int current_StepsL = stepperL.getStepsCompleted();
  total_Steps_R += current_StepsR ;
  total_Steps_L += current_StepsL ;
  float distance_x ;
  float distance_y ;

  // La roue droite a un diamètre effectif différent (compensation COEF_DROIT) -> on divise sa distance
  float steps_per_mm = (MOTOR_STEPS * MICROSTEPS) / (DIAMETRE_ROUE * M_PI);
  float distance_R = (current_StepsR / steps_per_mm) / COEF_DROIT;
  float distance_L = current_StepsL / steps_per_mm;

  // Prendre la moyenne pour plus de précision
  float distance = (distance_R + distance_L) / 2.0f;

  Serial.print(" distance parcouru:");
  Serial.print(distance);

  distance_x = distance * cos(teta_actuelle * M_PI / 180.0f); // M_PI / 180.0f pour la convertion en radian
  distance_y = distance * sin(teta_actuelle * M_PI / 180.0f); // il y a un signe moins au debut car on conconsidère le cercle trigonométrique avec le 0 vers la cuisine, l'axe des x est correcte, mais l'axe des y est inversé par rapport au cercle trigo.

  Serial.print(" distance_x_ajouter:");
  Serial.print(distance_x);
  Serial.print(" distance_y_ajouter:");
  Serial.print(distance_y);

  x_position += distance_x;
  y_position += distance_y;

  Serial.print(" new position x:");
  Serial.print(x_position);
  Serial.print(" new position y:");
  Serial.println(y_position);
  Serial.println("end fonction position");
}


void SetRPM(int rpm)
{
  stepperL.setRPM(rpm);
  stepperR.setRPM(rpm);
}

void Set_Decelerate(int decel){
  stepperR.setSpeedProfile(stepperR.LINEAR_SPEED, MOTOR_ACCEL, decel);
  stepperL.setSpeedProfile(stepperL.LINEAR_SPEED, MOTOR_ACCEL, decel);
}
