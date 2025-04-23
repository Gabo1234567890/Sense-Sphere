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
  float weight = scale.get_units(5);  // Get the weight (average over 5 readings)
  
  Serial.print("Weight: ");
  Serial.print(weight);
  Serial.println(" g");  // Display the weight in grams
  
  // Condition 1: Check if the bowl is empty, dispense food for custom time (e.g., 5 seconds)
  if (weight < 1.0) {  // Allow small fluctuations around 0g
    Serial.println("Bowl is empty. Turning motor for custom time...");

    // Turn on the motor for custom time (e.g., 5 seconds)
    digitalWrite(motorPin1, HIGH);
    digitalWrite(motorPin2, LOW);
    delay(5000);  // Motor runs for 5 seconds
    
    // Stop the motor
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, LOW);

    delay(3000);  // Wait for 3 seconds before checking again
  }

  // Condition 2: Distribute food/water regardless of weight for custom time
  // If you want to dispense food or water at specific intervals regardless of the weight:
  static unsigned long lastWaterDispenseTime = 0;
  unsigned long currentMillis = millis();
  if (currentMillis - lastWaterDispenseTime >= 5000) { // Every 5 seconds or 5 sec after feed
    Serial.println("Dispensing water for custom time...");
    
    // Open the solenoid valve for custom time (e.g., 5 seconds)
    digitalWrite(relayPin, HIGH);
    delay(5000);  // Custom time for dispensing water/food
    
    digitalWrite(relayPin, LOW);  // Close the valve
    lastWaterDispenseTime = currentMillis;  // Update the last dispense time
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

