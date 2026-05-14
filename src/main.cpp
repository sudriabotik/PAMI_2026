#include "move.h"
#include "config_robots.hh"
#include "Superstar.hh"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/timers.h>  
#include "ESP32Servo.h"

#define EVITEMENT

uint waypointIndex = 0;

volatile unsigned long Time1; // Variable pour stocker le temps de départ
volatile bool SuperStarTime = false; // Variable pour indiquer si la superstar est active
unsigned long elapsedTime;

int capteur(int sensorPin); 
float symetrie_x(float coordonne);
float symetrie_angle(float angle);
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
	delay(2000); 

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
	myservo.setPeriodHertz(50);    // standard 50 hz servo
	myservo.attach(servoPin, 500, 10000);
	myservo.write(180);
	pinMode(ENABLE, OUTPUT);
	digitalWrite(ENABLE, LOW); // pour que le enable des  drive soit à low. 
	pinMode(tirette, INPUT_PULLUP);
	pinMode(bouton_equipe, INPUT_PULLUP);
	Serial.println("setup done");
}

void AvoidanceChecksNormal(int sensor_M, int sensor_L, int sensor_R)
{
	if ((evitement == 0) && moving())
	{
		if (sensor_M > 5 && sensor_M < 200 && avoidance[waypointIndex] >= AVOID_MIDDLE)
		{
			stop();
			Serial.println("obstacle Middle");
			if (sensor_L < sensor_R) evitement = 1; // evitement par la droite
			else evitement = 2; //evitement par la gauche
			vTaskDelay(10/portTICK_PERIOD_MS);
		}
		else if (sensor_L > 5 && sensor_L  < 100 && avoidance[waypointIndex] >= AVOID_ALL)
		{
			stop();
			Serial.println("obstacle Left");
			evitement = 1;
			vTaskDelay(10/portTICK_PERIOD_MS);
		}
		else if (sensor_R > 5 && sensor_R < 100 && avoidance[waypointIndex] >= AVOID_ALL)
		{
			stop();
			Serial.println("obstacle Right");
			evitement = 2;
			vTaskDelay(10/portTICK_PERIOD_MS);
		}
	}
}


void Task1code( void * pvParameters ){
	vTaskDelay(3000);
	
	while ((digitalRead(tirette))) {vTaskDelay(20);}
	while (!(digitalRead(tirette))){
		vTaskDelay(20);
		Serial.println("wait la tirette task 1");
	}
	myservo.write(180); // to be REALLY redundant
	

	for(;;){

		static int depart = 1;
		if (depart == 1){
			depart ++;
			Serial.print("debut du compte à rebourt");
			Time1 = millis();
		}
		

		#ifdef EVITEMENT
		int sensor_M = capteur(sensorPinMidel);
		int sensor_R = 0;
		int sensor_L = capteur(sensorPinLeft);
		// int sensor_R = capteur(sensorPinRight);
		#endif

		# ifdef PRINT_DISTANCES
	//Serial.print(" evitement:");
	//Serial.print(evitement);
		Serial.print("elapsedTime:");
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
		//Serial.print(" stepperR:");
		//Serial.print(stepperR.getStepsCompleted());
		//Serial.print(" total_Steps_R:");
		//Serial.print(total_Steps_R); 
		Serial.print(" position x:");
		Serial.print(x_position);
		Serial.print(" position y:");
		Serial.print(y_position);
		Serial.print(" evitement:");
		Serial.println(evitement);
		# endif
		
		#ifdef EVITEMENT
		AvoidanceChecksNormal(sensor_M, sensor_L, sensor_R);
		# endif

		elapsedTime = millis() - Time1;

		// fin des 15s : on stoppe les moteurs et on fait danser le pami
		if (elapsedTime > MATCH_TIME) {
			stop();
			digitalWrite(ENABLE, HIGH);
			Serial.println("FIN du temps des 15sec");
			while(1){
				myservo.write(0);
				vTaskDelay(1000);
				myservo.write(180);
				vTaskDelay(1000);
			}
		}

	vTaskDelay(1/portTICK_PERIOD_MS);
	}
}

void Task2code( void * pvParameters ){
	vTaskDelay(3000);

	while ((digitalRead(tirette))) {vTaskDelay(20);}

	vTaskDelay(500);
	
	while (!(digitalRead(tirette))){
		vTaskDelay(20);
		Serial.print("equi_coul:");
		Serial.print(!digitalRead(bouton_equipe));
		Serial.println("wait la tirette task 2");
	} 
	
	/////////
	//// DEBUT DU CODE POUR FAIRE L'ODOMETRIE
	////////

	/// on fait trouner le robot sur lui meme et
	// on corrige COEF_ROTATE situer dans le lib/config_robots/config_robots.h 

	//current_rotation_rpm = 40;
	//current_translation_rpm = MOTOR_RPM;
	//rotation(360*10);
	//straight(1000);

	// on fait avancer le robot et il y a 2 parametre à corriger 
	// le diametre de la roue: DIAMETRE_ROUE situer dans le lib/config_robots/config_robots.h 
	// le parametre DIAMETRE_ROUE affecte tous les pamis
	// pour corriger chaque pamis individuelment il y a le parametre COEF_STRAIGHT  situer dans le lib/config_robots/config_robots.h 

	//straight (1000); //1000 mm
	//straight (-1000); // reculer de 1000 mm

	
	// test 1 enchaine deux commandes
	//straight_continue(500);
	//straight_extend(500);

	// test2 arc de cercle
	//arc(500, 90);         // quart de cercle à droite, rayon 200 mm
	//arc(150, 360);         // cercle complet — doit revenir au point de départ
	//arc_continue(300, 45); arc_extend(300, 45);  // 90° sans pause sonore au milieu

	// test3 arc de cercle + straight
	// arc(500, 90);              // arc bloquant, robot s'arrête à la fin
	// straight(300);             // straight bloquant

	

	/////
	// DEBUT DU CODE POUR FAIRE DES GO_TO
	////

	//Serial.print("ap_whil:");
	//Serial.print(!digitalRead(bouton_equipe));

	

	current_rotation_rpm = ROTATION_RPM;
	current_translation_rpm = MOTOR_RPM;

	for (int i = 0; i < numPoints; i++)
	{
		unsigned long toremove = WAIT_SUBSTRACTION;
		if (toremove > timereq[i]) toremove = timereq[i]; // we make sure we don't underflow the timereq by substracting more than its value
		timereq[i] -= toremove; // we substract the WAIT_SUBSTRACTION to the time requirement of each waypoint to take into account the global wait at the beginning of the match.
	}

	if (!digitalRead(bouton_equipe)){ // si on est équipe jaune 

		for (int i = 0; i < numPoints; i++)
		{
			waypoints[i].x = symetrie_x(waypoints[i].x);
		}
		x_position = symetrie_x(x_position);
		teta_actuelle = symetrie_angle(teta_actuelle);
		//teta_actuelle = teta_actuelle + 180;
		Serial.println("bouton_jaune");
		equipe = 'J';
	} 
	Serial.print(" equipe_couleur :");
	Serial.println(equipe);
	Serial.println("wait start...");
	vTaskDelay(GLOBAL_WAIT); 
	Serial.println("extrat wait...");
	vTaskDelay(ADD_DELAY_START);
	Serial.println("end extrat wait...");
	
	while (true)
	{
		Serial.println("start boucle while core 2");

		stepperL.setRPM(rpms[waypointIndex]);
		stepperR.setRPM(rpms[waypointIndex]);
		current_translation_rpm = rpms[waypointIndex];

		go_to(waypoints[waypointIndex].x, waypoints[waypointIndex].y);

		while (evitement != 0){  
			//evitement_droit();
			//stop();
			//digitalWrite(ENABLE, HIGH);
			//vTaskDelay(100);
			evitement = 0;
			// Serial.println("evitement fini");
			vTaskDelay(200);
			
		}
		

		if( (abs(x_position - waypoints[waypointIndex].x)<150) && (abs(y_position - waypoints[waypointIndex].y)<150) ){ // une fois que l'on est proche de la zone on quitte le while 
			
			waypointIndex ++;
			if (waypointIndex >= numPoints) break; // we reached the end of the waypoints 
			Serial.print("Next Waypoint, x:");
			Serial.print(waypoints[waypointIndex].x);
			Serial.print(" y:");
			Serial.println(waypoints[waypointIndex].y);

			
			while (millis() - Time1 < timereq[waypointIndex]) { // we wait until the time requirement for this waypoint is met
				Serial.println(millis() - Time1);
				Serial.println(timereq[waypointIndex]);
				vTaskDelay(100);
			}
		}
			
	}
	
	vTaskDelay(10);
	// prevent the loop from going on
	while (true) {
		Serial.println("FIN du programme du core 2");
		//digitalWrite(ENABLE, HIGH);
		vTaskDelay(500);
	}
	

	
		
}


//////////////////////////////////////////////////////////////////////////////

void loop() {
	vTaskDelete(NULL);
	Serial.println("dans le loop");
}



float symetrie_x(float coordonne) {
	coordonne = 1500 - (coordonne - 1500);
	return coordonne ;
}

float symetrie_angle(float angle)
{
	angle = 180 - angle;

	if (angle >= 180.0f){
	angle = angle - 360.0f;
	} 
	else if (angle < -180.0f){
	angle = angle + 360.0f;

	}
	return angle;
}


int capteur(int sensorPin){
	int16_t d ;
	int16_t t = pulseIn(sensorPin, HIGH);
// Serial.print(" t:");
// Serial.print(t);
// Serial.print("  ");
	if ( t==0)
	{
		Serial.print("Timeout, ERREUR capteur numero pin :  ");
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
