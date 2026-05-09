# ifndef SUPERSTAR_HH
 # define SUPERSTAR_HH
 
 # include "move.h"
 
 #include <freertos/FreeRTOS.h>
 
 # define TRANSLATION_DISTANCE_AFTER_EDGE 20
 # define APPROACH_SPEED 30
 # define APPROACH_ACCEL 700
 # define APPROACH_DECEL 5000
 # define APPROACH_SPEED_ROTATE 20
 
 
 # define SUDDEN_CHANGE_THRESHOLD 15
 
 void MoveToEdge();

 # endif