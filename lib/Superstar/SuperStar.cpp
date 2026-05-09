# include "Superstar.hh"
 
 void MoveToEdge()
{
     Serial.println("start function MoveToEdge");
     //float rpmStore = stepperL.getRPM();
     stepperR.setSpeedProfile(stepperR.LINEAR_SPEED, APPROACH_ACCEL, APPROACH_DECEL);
     stepperL.setSpeedProfile(stepperL.LINEAR_SPEED, APPROACH_ACCEL, APPROACH_DECEL); // accel et decel work ?
     stepperL.setRPM(APPROACH_SPEED);
     stepperR.setRPM(APPROACH_SPEED);
     while (true)
     {
         Serial.println("boucle approaching edge");
         // make sure avoidance is on
         evitement = 0;
         straight(1000);
 
         // when reaching this point, either the move is finished or an avoidance happened
        static short last_evitement = 0 ;

         if (evitement == 0)
         {
             Serial.println("error no edge detected");
             break;
         }
         if (evitement == 1){
            rotation(-3); 
            last_evitement = -2;
         } 
         if (evitement == 2) { 
            rotation(3);
            last_evitement = 2;
         }
         if (evitement == 3) {  // floor no more detected
            static int backword = 1;
            if (backword) {
                backword = 0;
                Serial.println("backward ");
                straight(-50);
                continue;
            } else {
            Serial.println("break while");
            break;
            }
        }
    }
 
    stop();
    Serial.println("rotation -1 before edge");
    rotation(-1);
    Serial.println("TRANSLATION_DISTANCE_AFTER_EDGE");
    straight(TRANSLATION_DISTANCE_AFTER_EDGE);

    //stepperL.setRPM(rpmStore);
    //stepperR.setRPM(rpmStore);

    // avoid the motors getting the power cut at the end of the program too soon
    vTaskDelay(3000 / portTICK_PERIOD_MS);
    Serial.println("end function MoveToEdge");
}
