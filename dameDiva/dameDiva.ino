#include <Arduino.h>

/*
Following Rachel De Barros youtube: pir + motor
Instead of hooking events to the HIGH state, do it on state changes

Version 1 25 May: detect and activate on state change only 
Version 2 25 May: activate a blinking LED - not what I expect. First (next version)  do what Rachel does: low, blink, low 
Version 3 26 May: take what we learned earlier, now add Rachel's state change.BUT first, we set the 
*/


/* PINS */
constexpr byte pirSensorPin = 12;
constexpr byte rightEyePin = 8; 
constexpr byte leftEyePin = 9;
const byte eyes[] = {
  rightEyePin, leftEyePin
};


/* SENSORS */
int motionStatus = 0; // pir detecting motion or not

/* STATE CHANGE FLAGS*/
byte pirState = LOW; // to track the state changes and act on the change, not the state of motionStatus

void setup() {
  pinMode(pirSensorPin, INPUT);

  for (byte pin : eyes) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }
  
  Serial.begin(9600);
  delay(1000); // Give the PIR sensor time to calibrate
  Serial.println("Dance the night away, lights on");
}

void loop() {

  motionStatus = digitalRead(pirSensorPin);
  
  delay(1000);

  if (motionStatus == HIGH) {
  // Dont be tempted to do things (activate components) here; unless you want it to repeat over and over

  for (byte pin : eyes) {
    digitalWrite(pin, HIGH);
  }
  
  Serial.print("Motion detector state hi?: ");
  Serial.println(digitalRead(pirSensorPin));
  
  //.. instead activate them on state change only <3 
   if (pirState == LOW) {
     Serial.println("Motion detected...wait for it... in 2 sec...");  
     delay(2000);
    
     digitalWrite(rightEyePin, LOW);
     delay(150);
     digitalWrite(rightEyePin, HIGH);
     Serial.println("Winked");
     pirState = HIGH;
    }
  }
  
  else {
    // Don't activate
    if (pirState == HIGH) {
      Serial.println("Motion ended");

// Both eyes closed
      for(byte pin : eyes) {
        digitalWrite(pin, LOW);
      }

     pirState = LOW;
    }
  }
}