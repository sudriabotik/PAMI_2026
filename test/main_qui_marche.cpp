
#include"Arduino.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/timers.h> 

TaskHandle_t Task1;
TaskHandle_t Task2;

// LED pins
const int stepperL = 5;
const int stepperR = 6;

void Task1code(void *pvParameters);
void Task2code(void *pvParameters);

void setup() {
  Serial.begin(9600); 
  pinMode(stepperL, OUTPUT);
  pinMode(stepperR, OUTPUT);

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
for (;;) {
  digitalWrite(stepperL, HIGH);
  digitalWrite(stepperR, HIGH);
  vTaskDelay(2 / portTICK_PERIOD_MS);  // 0,5 ms
  digitalWrite(stepperL, LOW);
  digitalWrite(stepperR, LOW);
  vTaskDelay(2 / portTICK_PERIOD_MS);  // 0,5 ms
  }
}
//Task2code: blinks an LED every 700 ms
void Task2code( void * pvParameters ){

  for(;;){
    digitalWrite(stepperR, HIGH);
    //digitalWrite(led2, LOW);
    Serial.println("core 2");
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

void loop() {
  vTaskDelete(NULL);
  Serial.println("dans le loop");
}
