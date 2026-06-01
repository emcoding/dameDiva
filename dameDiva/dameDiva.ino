#include <Arduino.h>

/*
Following Rachel De Barros youtube: pir + motor
Instead of hooking events to the HIGH state, do it on state changes

Version 1 25 May: detect and activate on state change only 
Version 2 25 May: activate a blinking LED - not what I expect. First (next version)  do what Rachel does: low, blink, low 
Version 3 26 May: take what we learned earlier, now add Rachel's state change.BUT first, we set the EYES on with state, not state change
✔ Checked with Upload
Version 4 26 May: change winking eye to pwd pin, use fade to make the wink more natural. Extract wink() method.  

>>> wink like a human <<<<<
Close fast: 100ms
Short hold: ~20-50 ms
Open slowly: ~150-200 ms
total 300-400 ms
Option: easing curve instead of lineair. 

Finetuning the wink: 
* close : steps -15, delay 5 ~85 ms
* closed: 150. (200, 400 is too long)
* Open: steps + 3, delay 6; ~ 500 ms
  
>>>> Current behaviour <<<<
No motion: sleep;
Motion detected: eyes open (== Leds on); after 2 secs, wink once: 
close fast, hold, open slower, both eyes open.
Motion stopped : eyes close

*/

/* PINS */
constexpr byte pirSensorPin = 12;

constexpr byte rightEyePin = 9; // pwd, for fading/analogWrite
constexpr byte leftEyePin = 8;
const byte eyes[] = {
  rightEyePin, leftEyePin
};

// Fading the led in and out for natural winking
int brightness = 255; // start w eyes wide open // pwd values range : 0 .. 255

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
      delay(2500);

      wink(); // once, 
      
      pirState = HIGH; // Flip state, so this loop is not hit after the first time
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

     pirState = LOW; // reset state
    }
  }
}


// PM the lineair fade is so short, it doesn't make sense to make it an easing curve. 
// Steps and delay are finetuned by trial and error.
void wink() {
    
  // Close eye, fast:  <100 ms (255/15 = 17 stappen * 5ms = 85 ms)
  for (int brightness = 255; brightness >= 0; brightness -= 15) {
    analogWrite(rightEyePin, brightness); // 25:37
    delay(5);
  }
  // keep closed 
  delay(150);

  // open slowly; ~500 ms
  for (int brightness = 0; brightness <= 255; brightness += 3) {
    analogWrite(rightEyePin, brightness); // 25:37
    delay(6);
  }

  Serial.println("Winked");
}
