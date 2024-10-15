#include <PxMatrix.h>  // Bibliothèque pour contrôler l'affichage LED

// Déclaration des pins pour l'ESP32-C6
#define P_LAT 22
#define P_A 19
#define P_B 23
#define P_C 18
#define P_D 5
#define P_E 15
#define P_OE 16

// Dimensions de l'affichage
PxMatrix display(64, 32, P_LAT, P_OE, P_A, P_B, P_C, P_D, P_E);

void setup() {
  // Initialisation de l'affichage LED
  display.begin(8);  // Le paramètre 8 correspond à la fréquence de rafraîchissement
  display.setBrightness(150);  // Ajustez la luminosité
}

void loop() {
  // Effacer l'écran
  display.clearDisplay();

  // Afficher un chronomètre
  int minutes = 4;
  int seconds = 0;
  char timeString[6];  // Format: MM:SS
  sprintf(timeString, "%02d:%02d", minutes, seconds);

  display.setTextColor(display.color565(255, 0, 0));  // Rouge
  display.setCursor(2, 2);  // Position du texte
  display.print(timeString);

  display.showBuffer();  // Actualiser l'affichage
  delay(1000);  // Attendre 1 seconde (vous pourrez ajouter un compte à rebours ici)
}
