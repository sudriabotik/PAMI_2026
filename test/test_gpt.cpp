#include "BasicStepperDriver.h"

// Déclarez et initialisez vos pilotes de moteur pas à pas
BasicStepperDriver stepper1(200, 2, 3); // Moteur 1 : 200 pas par tour, broches DIR sur 2 et STEP sur 3
BasicStepperDriver stepper2(200, 4, 5); // Moteur 2 : 200 pas par tour, broches DIR sur 4 et STEP sur 5

void setup() {
  // Initialisation des pilotes avec une vitesse de rotation de 60 tr/min et un pas micro de 1
  stepper1.begin(60, 1);
  stepper2.begin(60, 1);
}

void loop() {
  // Effectuez un mouvement pour chaque moteur (par exemple, 100 pas pour chaque moteur)
  stepper1.move(100);
  stepper2.move(100);

  // Attendez un moment pour simuler une certaine activité
  delay(2000);

  // Arrêtez les deux moteurs soudainement
  long stepsRemaining1 = stepper1.stop();
  long stepsRemaining2 = stepper2.stop();

  // Affichez le nombre de pas restants pour chaque moteur
  Serial.print("Mouvement arrêté pour moteur 1. Pas restants : ");
  Serial.println(stepsRemaining1);
  Serial.print("Mouvement arrêté pour moteur 2. Pas restants : ");
  Serial.println(stepsRemaining2);

  // Attendez un moment avant de recommencer
  delay(1000);
}
