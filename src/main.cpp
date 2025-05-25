#include <Wire.h>
#include <VL53L0X.h>

// Définition des broches XSHUT
#define XSHUT1 13
#define XSHUT2 23
#define XSHUT3 15

// Seuil de détection obstacle en mm
#define OBSTACLE_THRESHOLD 400  // à adapter selon ta scène

// LED ou relais de sortie
#define ALERT_PIN 2

// Objets capteurs
VL53L0X sensor1;
VL53L0X sensor2;
VL53L0X sensor3;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  // Config des broches XSHUT
  pinMode(XSHUT1, OUTPUT);
  pinMode(XSHUT2, OUTPUT);
  pinMode(XSHUT3, OUTPUT);
  // Broche de signal
  pinMode(ALERT_PIN, OUTPUT);
  digitalWrite(ALERT_PIN, LOW);

  // Désactiver tous les capteurs
  digitalWrite(XSHUT1, LOW);
  digitalWrite(XSHUT2, LOW);
  digitalWrite(XSHUT3, LOW);
  delay(20);
  digitalWrite(XSHUT1, HIGH);
  digitalWrite(XSHUT2, HIGH);
  digitalWrite(XSHUT3, HIGH);
  delay(20);

  digitalWrite(XSHUT2, LOW);
  digitalWrite(XSHUT3, LOW);
  delay(300);
  // Initialiser chaque capteur un par un avec une adresse différente
  sensor1.init();
  sensor1.setAddress(0x31);
  Serial.println("Capteur 1 initialisé à l'adresse 0x30");
  sensor1.startContinuous();
  int d1 = sensor1.readRangeContinuousMillimeters();
  Serial.print("Distance Capteur 1: ");
  Serial.println(d1);
  digitalWrite(XSHUT2, HIGH);
  delay(500);


  sensor2.init();
  sensor2.setAddress(0x32);
    Serial.println("Capteur 2 initialisé à l'adresse 0x31");

  
  sensor2.startContinuous();
  int d2 = sensor2.readRangeContinuousMillimeters();
  Serial.print("Distance Capteur 2: ");
  Serial.println(d2);

  digitalWrite(XSHUT3, HIGH);
  delay(500);
  sensor3.init();
  sensor3.setAddress(0x33);
    Serial.println("Capteur 2 initialisé à l'adresse 0x32");

  /*sensor3.startContinuous();
  int d3 = sensor3.readRangeContinuousMillimeters();

  Serial.print("Distance Capteur 3: ");
  Serial.println(d3);*/
  // Démarrer les capteurs en mode continu
  sensor1.startContinuous();
  sensor2.startContinuous();
  sensor3.startContinuous();
  Serial.println("Initialisation terminée. En attente des capteurs dégagés...");
}

bool allClear() {
  Serial.println("cap1");
  int d1 = sensor1.readRangeContinuousMillimeters();
    Serial.println("cap2");
   int d2 = sensor2.readRangeContinuousMillimeters();
    Serial.println("cap3");
  int d3 = sensor3.readRangeContinuousMillimeters();

  return (d1 > OBSTACLE_THRESHOLD &&
          d2 > OBSTACLE_THRESHOLD &&
          d3 > OBSTACLE_THRESHOLD);
}

void loop() {
  // Étape 1 : Attendre que les 3 capteurs soient dégagés
  while (!allClear()) {
    Serial.println("Attente que les capteurs soient dégagés...");
    delay(100);  // évite de surcharger le bus
  }

  Serial.println("Capteurs dégagés, début de la boucle de surveillance.");

  // Étape 2 : Boucle principale
  while (true) {
    if (allClear()) {
      // Tout est dégagé
      digitalWrite(ALERT_PIN, LOW);
    } else {
      // Obstacle détecté → activer le signal
      digitalWrite(ALERT_PIN, HIGH);

      // Attendre que les capteurs redeviennent tous dégagés
      while (!allClear()) {
        delay(50);
      }

      // Repassé à l'état normal
      digitalWrite(ALERT_PIN, LOW);
    }

    delay(1000);  // pause entre les vérifications
  }
}
