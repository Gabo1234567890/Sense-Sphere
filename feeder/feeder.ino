#include "HX711.h"

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 3;  // Data pin of the load cell
const int LOADCELL_SCK_PIN = 2;   // Clock pin of the load cell
HX711 scale;

// L298N Motor Driver Pins
const int motorPin1 = 8;
const int motorPin2 = 9;

// Relay (Solenoid Valve) Pin
const int relayPin = 7;

void setup() {
  Serial.begin(9600);  // Start serial communication
  
  // Initialize the HX711 load cell
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(7050);  // Adjust this based on your load cell calibration
  scale.tare();  // Tare the scale (set current weight to 0)
  
  // Outputs for motor and relay
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(relayPin, OUTPUT);

  // Initial states for motor and relay
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(relayPin, LOW);
  
  Serial.println("Weight measurement starting...");
}

void loop() {
  float weight = scale.get_units(1);  // Get the weight (average over 5 readings)
  
  Serial.print("Weight: ");
  Serial.print(weight);
  Serial.println(" g");  // Display the weight in grams

  if (weight < 1.0) {  // Allow small fluctuations around 0g
    Serial.println("Bowl is empty. Feeding for custom time...");

    // Turn on motor (auger mechanism)
    digitalWrite(motorPin1, HIGH);
    digitalWrite(motorPin2, LOW);
    
    // Open solenoid valve
    digitalWrite(relayPin, HIGH);
    delay(5000); // Custom feed time for 0g

    // Stop feeding
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, LOW);
    digitalWrite(relayPin, LOW);

    delay(3000);  // Wait for 3 seconds before checking again
  }
  else if (weight < 50.0) {  // If the weight is low but not empty
    Serial.println("Weight low. Feeding...");

    // Turn on motor (auger mechanism)
    digitalWrite(motorPin1, HIGH);
    digitalWrite(motorPin2, LOW);
    
    // Open solenoid valve
    digitalWrite(relayPin, HIGH);
    delay(2000); // Default feed time for low weight

    // Stop feeding
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, LOW);
    digitalWrite(relayPin, LOW);

    delay(3000);  // Wait for 3 seconds before checking again
  }

  delay(1000);  // Wait 1 second before measuring again
}




// Weight code
// #include "HX711.h"

// // Pin definitions for HX711 load cell
// const int LOADCELL_DOUT_PIN = 3;  // Data pin of the load cell
// const int LOADCELL_SCK_PIN = 2;   // Clock pin of the load cell

// HX711 scale;

// void setup() {
//   Serial.begin(9600);  // Start serial communication
  
//   // Initialize the HX711 load cell
//   scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
//   scale.set_scale(7050); // Adjust this based on your load cell calibration
//   scale.tare();  // Tare the scale (set current weight to 0)
  
//   Serial.println("Weight measurement starting...");
// }

// void loop() {
//   float weight = scale.get_units(5);  // Get the weight (average over 5 readings)
  
//   Serial.print("Weight: ");
//   Serial.print(weight);
//   Serial.println(" g");  // Display the weight in grams
  
//   delay(1000);  // Wait for 1 second before the next reading
// }

