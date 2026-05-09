#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/timers.h>  // Si vous utilisez des timers FreeRTOS
#include "move.h"

const uint8_t sensorPinRight = A2;
const uint8_t sensorPinMidel = A6;
const uint8_t sensorPinLeft = A4;

const uint8_t tirettePin = 10;
const uint8_t teamButtonPin = 11;

const int pamiNumber = 0;

int x_goal = 600;
int y_goal = 800;
volatile bool go_to_1 = true ;
int x_goal_2 = 200;
int y_goal_2 = 950;

int blueTeamTargetPool[] = {600, 800, 200, 950,
                          800, 900, 1000, 2000};

int conteur = 0 ;
volatile int8_t evitement = 0 ;
volatile int8_t lifeStage = 0; // 0 is waiting before start, 1 is moving, 2 is stopped

TaskHandle_t Task1;
TaskHandle_t Task2;

TimerHandle_t startupTimer;
TimerHandle_t endTimer;

void Task1code(void *pvParameters);
void Task2code(void *pvParameters);

int capteur(int sensorPin); 
void evitement_droit();

void IRAM_ATTR Activation_ISR()
{
  lifeStage = 1;
}

void SelectTarget()
{
  if (digitalRead(teamButtonPin) == LOW)
  {
    x_goal = blueTeamTargetPool[pamiNumber*4];
    y_goal = blueTeamTargetPool[pamiNumber*4 + 1];
    x_goal_2 = blueTeamTargetPool[pamiNumber*4 + 2];
    y_goal_2 = blueTeamTargetPool[pamiNumber*4 + 3];
  }
  else
  {

  }
}

void StartupCallback(TimerHandle_t xTimer)
{
  xTimerStart(endTimer, 0);
  Serial.println("started");

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

void EndCallback(TimerHandle_t xTimer)
{
  Serial.println("stopped");
  vTaskDelete(Task1);
  vTaskDelete(Task2);
  stop();
}

void setup() {
  Serial.begin(9600);
  Serial.println("program begun");

  pinMode(teamButtonPin, INPUT);
  pinMode(tirettePin, INPUT);
  attachInterrupt(tirettePin, Activation_ISR, LOW);
  SelectTarget();
  configureMotors();

  startupTimer = xTimerCreate("startup", pdMS_TO_TICKS(90000), pdFALSE, (void *) 0, StartupCallback);
  endTimer = xTimerCreate("ending", pdMS_TO_TICKS(10000), pdFALSE, (void *) 0, EndCallback);

  Serial.println("Awaiting pull");
  while (lifeStage == 0) {}

  xTimerStart(startupTimer, 0);
  Serial.println("Launching in 90 seconds");
}


void Task1code( void * pvParameters ){
  for (;;) {
    //Serial.println("core_2");
    int sensor_M = abs(capteur(sensorPinMidel));
    int sensor_L = abs(capteur(sensorPinLeft));
    int sensor_R = abs(capteur(sensorPinRight));

   //Serial.print(" evitement:");
   //Serial.print(evitement);
  //  Serial.print("R:");
   // Serial.println(sensor_R);
    Serial.print(" M:");
    Serial.print(abs(sensor_M));
   // Serial.print(" L:");
   // Serial.println(sensor_L);
    //vTaskDelay(1000);
    //stepperR.stop(); stepperL.stop();
 
    Serial.print(" stepperR:");
    Serial.print(stepperR.getStepsCompleted());
    //Serial.print(" total_Steps_R:");
    //Serial.print(total_Steps_R); 
    Serial.print(" position x ");
    Serial.print(x_position);
    Serial.print(" position y ");
    Serial.println(y_position);

    // condition de detection pour les évitements
    if (evitement == 0 && moving())
    {
      if (sensor_M > 5 && sensor_M < 250 && go_to_1)
      {
        stop();
        if (sensor_L < sensor_R) evitement = 1; // evitement droit
        else evitement = 2; //evitement gauche
        vTaskDelay(10/portTICK_PERIOD_MS);
      }
      else if (sensor_L > 5 && sensor_L  < 250 && go_to_1)
      {
        stop();
        evitement = 1;
        vTaskDelay(10/portTICK_PERIOD_MS);
      }
      else if (sensor_R > 5 && sensor_R < 250 && go_to_1)
      {
        stop();
        evitement = 2;
        vTaskDelay(10/portTICK_PERIOD_MS);
      }
    }
    
    /*
    if((abs(sensor_M) > 5) && (abs(sensor_M) < 250)  && (evitement == true) && (moving()) && (stepperR.getStepsCompleted() > 10) && (go_to_1) ){
      Serial.println("obstacle_midel");
      //xSemaphoreGive(stateChangeSemaphore);
      stop();
      Serial.println("end of the break");
      evitement = false;
      vTaskDelay(10/portTICK_PERIOD_MS); //pour laisser le temps au corps 2 de refaire ça boucle. 
      //xSemaphoreGive(stateChangeSemaphore);

      //fonction qui permet de transmettre l'information à la fonctin task2code que l'état viens de changer 
    }
    */
    


    vTaskDelay(1/portTICK_PERIOD_MS);
  }
  stop();
}
//Task2code: blinks an LED every 700 ms
void Task2code( void * pvParameters ){
  //vTaskDelay(2000);
  for (;;) {
    //Serial.print("start core 1");
  /* 
  if (evitement == false){  // xSemaphoreTake(stateChangeSemaphore, SEMAPHORE_WAIT_TIME) == pdTRUE
    evitement_droit();
    evitement = true;
  }
  */ 
    evitement = -1; // empêche les évitements
    straight(100); // pour que le pami démarre meme s'il y a un obstacle devant lui. 
    evitement = 0; // réactive les évitements 
    //vTaskDelay(100000/portTICK_PERIOD_MS);
    while (!((x_position >= x_goal - 1 && x_position <= x_goal + 1) && (y_position >= y_goal - 1 && y_position <= y_goal + 1))) {
      Serial.println("debut du while");

      if (evitement == 1){  // xSemaphoreTake(stateChangeSemaphore, SEMAPHORE_WAIT_TIME) == pdTRUE
        evitement_droit();
        evitement = 0;
      }
      else if (evitement == 2){
        evitement_gauche();
        evitement = 0;
      }

      if( (abs(x_position - x_goal)<100) || (abs(y_position - y_goal)<100) ){ // une fois que l'on est proche de la zone on quitte le while 
        break; // sortir du wile 
      }
      go_to(x_goal,y_goal);
    }
  Serial.println("sortie du while");
  go_to_1 = false ; // pour désactiver l'evitement. 
  go_to(x_goal_2,y_goal_2);
  vTaskDelay(10000);

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

int capteur(int sensorPin){
  int16_t d ;
  int16_t t = pulseIn(sensorPin, HIGH);
// Serial.print(" t:");
// Serial.print(t);
// Serial.print("  ");
  if ( t==0)
  {
    t = 1;
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
