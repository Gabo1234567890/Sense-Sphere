#include <Wire.h>          
#include "RTClib.h"        
#include <DHT.h>           

#define DHTPIN 2           
#define DHTTYPE DHT11      
#define RELAY_PIN 8        
#define MOISTURE_PIN A0    
#define TRIG_PIN 9         
#define ECHO_PIN 10        

DHT dht(DHTPIN, DHTTYPE); 
RTC_DS3231 rtc;            

void setup() {
  Serial.begin(9600);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  dht.begin();
  Wire.begin();

  if (!rtc.begin()) {
    Serial.println("DS3231 not found!");
    while (1);
  }
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, setting time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void loop() {
  
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  
  int moisture = analogRead(MOISTURE_PIN);


  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH);
  int distance = duration * 0.034 / 2;

  
  DateTime now = rtc.now();

  
  Serial.print("Temp: "); Serial.print(temperature); Serial.print("°C | ");
  Serial.print("Humidity: "); Serial.print(humidity); Serial.print("% | ");
  Serial.print("Moisture: "); Serial.print(moisture); Serial.print(" | ");
  Serial.print("Distance: "); Serial.print(distance); Serial.print("cm | ");
  Serial.print("Time: "); Serial.print(now.hour()); Serial.print(":");
  Serial.print(now.minute()); Serial.print(":"); Serial.println(now.second());


  if (moisture < 500) {  
    digitalWrite(RELAY_PIN, LOW);  
    delay(2000);                  
    digitalWrite(RELAY_PIN, HIGH); 
  }

  delay(2000);  
}