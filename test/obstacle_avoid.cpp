
# define TEST_MODE

# ifdef TEST_MODE
# define GLOBAL_WAIT 2000
# endif
# ifndef TEST_MODE
# define GLOBAL_WAIT 90000
# endif

#include "move.h"
# include "../lib/TeamSelect/TeamSelect.hh"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/timers.h>  // Si vous utilisez des timers FreeRTOS
#include "ESP32Servo.h"



const uint8_t sensorPinRight = A3;
const uint8_t sensorPinMidel = A2;
const uint8_t sensorPinLeft = A1;
const uint8_t servoPin = 17;
const uint8_t tirette = 11;
const uint8_t bouton_equipe = 10; // low = blue vers le haut, hight= jaune
char equipe = 'B'; // équipe bleu par default
//bool drapeau_task_1 = false ;

/*
float x_goal = 500; // 1=2500 2=500 3=2400  4=500
float y_goal = 1600; // 1=1400 2=1600 3=1000 4=600
volatile bool go_to_1 = true ;
float x_goal_2 = 200;// 1=3000 2=200 3=2800 4=0
float y_goal_2 = 1800; // 1=1400 2=1800 3=1000 4=600
int avancement_depart = 200 ; //1=1100 2=200 3=600 4=100
int time_start = 2500; //1 = 90000s 2 = 90500s 3 = 90000s 4 = 90500s
int depart = 0 ;

*/

volatile unsigned long Time1; // Variable pour stocker le temps de départ
unsigned long elapsedTime;

// extra variables for the code that prevent the pami from leaving the zone in case of obstacles
int initial_y;
unsigned long int block_time_mark;
bool obstacle_present;

int capteur(int sensorPin); 
float symetrie(float coordonne);
//volatile int8_t evitement = 0 ;

Servo myservo;

TaskHandle_t Task1;
TaskHandle_t Task2;
void Task1code(void *pvParameters);
void Task2code(void *pvParameters);

void setup() {
  Serial.begin(9600); 
  configureMotors();
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
  //delay(500); 
  myservo.setPeriodHertz(50);    // standard 50 hz servo
	myservo.attach(servoPin, 500, 10000);
  myservo.write(0);
  pinMode(8, OUTPUT);
  digitalWrite(8, LOW); // pour que le enable des  drive soit à low. 
  pinMode(tirette, INPUT_PULLUP);
  pinMode(bouton_equipe, INPUT_PULLUP);
  Serial.print("setup done");
}

void Task1code( void * pvParameters ){
  //vTaskDelay(1000);

  myservo.write(0);
  while (!(digitalRead(tirette))){
    vTaskDelay(20);
    Serial.println("wait la tirette task 1");
  }
  myservo.write(0); // to be REALLY redundant

  for(;;){

    
    if (depart == 1){
      depart ++;
      Serial.print("debut du compte à rebourt");
      Time1 = millis();
    }
    

    elapsedTime = millis() - Time1;

    if ((elapsedTime >10000) && (depart == 2)){
      stop();
      while(1){
        myservo.write(180);
        vTaskDelay(100);
        digitalWrite(8, HIGH);
        Serial.print("FIN");
  }
    }

    int sensor_M = capteur(sensorPinMidel);
    int sensor_L = capteur(sensorPinLeft);
    int sensor_R = capteur(sensorPinRight);
    
  //Serial.print(" evitement:");
  //Serial.print(evitement);
    Serial.print("time:");
    Serial.print(elapsedTime);
    Serial.print(" equipe:");
    Serial.print(equipe);
    Serial.print(" R:");
    Serial.print(sensor_R);
    Serial.print(" M:");
    Serial.print(abs(sensor_M));
    Serial.print(" L:");
    Serial.print(sensor_L);
    //vTaskDelay(1000);
    //stepperR.stop(); stepperL.stop();
    Serial.print(" teta_actuelle:");
    Serial.print(teta_actuelle);
    Serial.print(" stepperR:");
    Serial.print(stepperR.getStepsCompleted());
    //Serial.print(" total_Steps_R:");
    //Serial.print(total_Steps_R); 
    Serial.print(" position x ");
    Serial.print(x_position);
    Serial.print(" position y ");
    Serial.print(y_position);
    Serial.print(" evitement ");
    Serial.println(evitement);
    
    /* 
    
    if((abs(sensor_M) < 250)  && (evitement == true)  && (stepperR.getStepsCompleted() > 10) && (go_to_1) ){
      Serial.println("obstacle_midel");
      //xSemaphoreGive(stateChangeSemaphore);
      stop();
      Serial.println("end of the break");
      evitement = false;
      vTaskDelay(10/portTICK_PERIOD_MS); //pour laisser le temps au corps 2 de refaire ça boucle. 
      //xSemaphoreGive(stateChangeSemaphore);

    }
    */ 

    obstacle_present = (sensor_M > 5 && sensor_M < 100) || (sensor_L > 5 && sensor_L  < 100 ) || (sensor_R > 5 && sensor_R < 100 );

    if ((evitement == 0) && moving() && go_to_1)
    {
      if (sensor_M > 5 && sensor_M < 200)
      {
        stop();
        Serial.println("obstacle midel");
        if (sensor_L < sensor_R) evitement = 1; // evitement droit
        else evitement = 2; //evitement gauche
        vTaskDelay(10/portTICK_PERIOD_MS);
      }
      else if (sensor_L > 5 && sensor_L  < 100 )
      {
        stop();
        Serial.println("obstacle Left");
        evitement = 1;
        vTaskDelay(10/portTICK_PERIOD_MS);
      }
      else if (sensor_R > 5 && sensor_R < 100 )
      {
        stop();
        Serial.println("obstacle Right");
        evitement = 2;
        vTaskDelay(10/portTICK_PERIOD_MS);
      }
    }    
  vTaskDelay(1/portTICK_PERIOD_MS);
  }
}
//Task2code: blinks an LED every 700 ms
void Task2code( void * pvParameters ){
 
  while (!(digitalRead(tirette))){
    vTaskDelay(20);
    Serial.println("wait la tirette task 2");
  } 
    
   //vTaskDelay(200);
  if (digitalRead(bouton_equipe)){
    x_position= symetrie(x_position) ;
    x_goal = symetrie(x_goal);
    x_goal_2 = symetrie(x_goal_2);
    equipe = 'J';
  } 
  Serial.println("debut core 2");
  Serial.println(equipe);
  vTaskDelay(GLOBAL_WAIT); 
  
  //1 = 10s 2 = 12s 3 = 10s 4 = 12s
//straight(400); //1=400 2=200 3=400 4=200

  if(depart==0){
    depart ++;
    Serial.println("go_to depart");
  }

  vTaskDelay(time_start);
  initial_y = y_position;
  go_to(x_position, y_position + avancement_depart);

  if(evitement!=0)
  {
    if (abs(initial_y - y_position) < 20)
    {
      stop();
      block_time_mark = millis();
      // wait for 3 seconds to pass and for the path to be clear
      while (obstacle_present || (millis() - block_time_mark) < 3000)
      {
        // do absolutely nothing
      }
      go_to(x_position, initial_y + avancement_depart);
    }
    else
    {    
      Serial.println("evitement depart");
      vTaskDelay(time_evitement_depart);
      # ifndef PAMI_1
      # ifndef PAMI_2
      if (x_position < x_goal_2){
        evitement = 2;
      } else {
        evitement = 1;
      }
      # endif
      # ifdef PAMI_2
      if (equipe == 'B') evitement = 1;
      else evitement = 2;
      # endif
      # endif
    }
    //vTaskDelay(1000/portTICK_PERIOD_MS);
  }
  for (;;) {
  /* 

  if (evitement == false){  // xSemaphoreTake(stateChangeSemaphore, SEMAPHORE_WAIT_TIME) == pdTRUE
    evitement_droit();
    evitement = true;
  }

  */ 
    //evitement = false;
    //vTaskDelay(300000/portTICK_PERIOD_MS);
    //straight(20000); // pour que le pami démarre meme s'il y a un obstacle devant lui. 
    //evitement = true; // pour que le pami démarre meme s'il y a un obstacle devant lui. 
    //vTaskDelay(300000/portTICK_PERIOD_MS);
    while (!((x_position >= x_goal - 1 && x_position <= x_goal + 1) && (y_position >= y_goal - 1 && y_position <= y_goal + 1))) {
      Serial.println("debut du while");
/* 
      if (evitement == false){  // xSemaphoreTake(stateChangeSemaphore, SEMAPHORE_WAIT_TIME) == pdTRUE
        evitement_droit();
        evitement = true;
      }
*/

      if (evitement == 1){  // xSemaphoreTake(stateChangeSemaphore, SEMAPHORE_WAIT_TIME) == pdTRUE
        evitement_droit();
        //evitement = 0;
      }
      else if (evitement == 2){
        evitement_gauche();
        //evitement = 0;
      }

      if( (abs(x_position - x_goal)<150) && (abs(y_position - y_goal)<150) ){ // une fois que l'on est proche de la zone on quitte le while 
        Serial.println("break sortie du while");
        break; // sortir du wile 
      }
      go_to(x_goal,y_goal);
    }
  Serial.println("sortie du while");
  Serial.print("arrivé à la zone");
  //vTaskDelay(1000000);
  go_to_1 = false ; // pour désactiver l'evitement. 
  stepperR.setSpeedProfile(stepperR.LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL_FINISH);
  stepperL.setSpeedProfile(stepperL.LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL_FINISH);
  Serial.println("go_to_2");
  go_to(x_goal_2,y_goal_2);

  while(1){
    myservo.write(180);
    vTaskDelay(10);
    digitalWrite(8, HIGH);
    Serial.print("désactiver les moteurs fin");
  }

  while(1){
  Serial.println("Erreur");
  }
  /* 
  if (conteur < 2){ 
  straight(1000);
  //go_to(1500,700); // attention durant les rotations du go_to elle l'évitement peut s'activer
  conteur ++;
  }
  */ 
  }
}

//////////////////////////////////////////////////////////////////////////////

void loop() {
  vTaskDelete(NULL);
  Serial.println("dans le loop");
}

float symetrie(float coordonne) {
  coordonne = 1500 - (coordonne - 1500);
  return coordonne ;
}

int capteur(int sensorPin){
  int16_t d ;
  int16_t t = pulseIn(sensorPin, HIGH);
// Serial.print(" t:");
// Serial.print(t);
// Serial.print("  ");
  if ( t==0)
  {
    Serial.print("Timeout ");
    Serial.println(sensorPin);
    //t = 1;
  }
  else if (t > 1850)
  {
     //Serial.print("Pas de détection capteur ");// No detection.
  }
  else
  {
    // Valid pulse width reading. Convert pulse width in microseconds to distance in millimeters.
    d = (t - 1000) * 2;
 
    // Limit minimum distance to 0.
    if (d < 0) { d = 0; } 
    //Serial.print(d);
    //Serial.println(" mm");
  }
  return d;
}