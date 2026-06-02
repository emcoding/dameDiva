#include <Arduino.h>
#include <Stepper.h> // Lib for the stepper motor

/*
Following Rachel De Barros youtube: pir + motor
Instead of hooking events to the HIGH state, do it on state changes

Version 1 25 May: detect and activate on state change only
Version 2 25 May: activate a blinking LED - not what I expect. First (next version)  do what Rachel does: low, blink, low
Version 3 26 May: take what we learned earlier, now add Rachel's state change.BUT first, we set the EYES on with state, not state change
✔ Checked with Upload
Version 4 26 May: change winking eye to pwd pin, use fade to make the wink more natural. Extract wink() method.
Version 5 27 May: add motor 28bYj-48 with UNL2003 driver
https://youtu.be/PEg4bMFyHeM?si=3zoFUsg9fZ5GFgZG&t=642 for servo.
First go to Stepper motor vid to install that one.

>>> wink like a human <<<<<
Close fast: 100ms
Short hold: ~20-50 ms
Open slowly: ~150-200 ms
total 300-400 ms
Considered easing curve instead of lineair: not doing, the winking period is probs too short to notice the diff

>>>>>> 26 mei, finetuning the wink: <<<<<
* close : steps -15, delay 5 ~85 ms
* closed: 150. (200, 400 is too long)
* Open: steps + 3, delay 6; ~ 500 ms


>>>> Motor 28bYj-48, with UNL2003 driver <<<<<<<<
each step ≈ 11.25º
360º / 11.25 = 32 steps per revolution
64 : 1 gear reduction in this motor => 32 steps * 64 = 2048

>>>> Current behaviour <<<<
No motion: sleep;
Motion detected: eyes open (== Leds on);
after 2 secs, wink once:
close fast, hold, open slower, both eyes open.
Motion stopped : eyes close
*/

/* PINS */
constexpr byte pirSensorPin = 12;

constexpr byte rightEyePin = 9; // pwd, for fading/analogWrite
constexpr byte leftEyePin = 8;
const byte eyes[] = {
    rightEyePin, leftEyePin};

/* MOTOR */
// stepper motor
constexpr int stepsPerRevolution = 2048; // motor type dependent; read data sheet; 2048 for this very common 28bYj-48
constexpr byte rpm = 10;                 // speed

// Specific order: IN1 ,IN3, IN2, IN4
Stepper wiggleStepper(stepsPerRevolution, A0, A2, A1, A3); // Motor driver INx pins to analog pins on Arduino board

void setup()
{
  pinMode(pirSensorPin, INPUT);

  wiggleStepper.setSpeed(rpm);

  for (byte pin : eyes)
  {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }

  Serial.begin(9600);
  delay(1000); // Give the PIR sensor time to calibrate
  Serial.println("Dance the night away, lights on");
}

void loop()
{
  static byte pirState = LOW; // to track the state changes and act on the change, not the state of motionStatus
  int motionStatus = digitalRead(pirSensorPin);

  if (motionStatus == HIGH)
  {
    // Option 1 to do things (activate components) here; it will repeat over and over

    Serial.print("Motion detector state hi?: ");
    Serial.println(digitalRead(pirSensorPin));

    for (byte pin : eyes)
    {
      digitalWrite(pin, HIGH);
    }

    wiggle(); // To do: it should wiggle periodically as long as motion is detected (and when duration is longer)

    //.. instead activate them on state change only <3
    if (pirState == LOW)
    {
      Serial.println("Motion detected...wait for it... in 2 sec...");
      delay(2500);

      wink(); // once,

      pirState = HIGH; // Flip state, so this loop is not hit after the first time

      // option 2 to activate things
    }
  }

  else
  {

    // option 1

    // Don't activate
    if (pirState == HIGH)
    {
      Serial.println("Motion ended");

      // Both eyes closed
      for (byte pin : eyes)
      {
        digitalWrite(pin, LOW);
      }

      pirState = LOW; // reset state indicator
    }
  }
}

// PM the lineair fade is so short, it doesn't make sense to make it an easing curve.
// Steps and delay are finetuned by trial and error.
void wink()
{

  // Close eye, fast:  <100 ms (255/15 = 17 stappen * 5ms = 85 ms)
  for (int brightness = 255; brightness >= 0; brightness -= 15)
  {
    analogWrite(rightEyePin, brightness); // 25:37
    delay(5);
  }
  // keep closed
  delay(150);

  // open slowly; ~500 ms
  for (int brightness = 0; brightness <= 255; brightness += 3)
  {
    analogWrite(rightEyePin, brightness); // 25:37
    delay(6);
  }

  Serial.println("Winked");
}

void wiggle()
{
  Serial.println("... and wiggle");
  wiggleStepper.step(stepsPerRevolution);
  delay(500);
  wiggleStepper.step(-stepsPerRevolution);
  delay(500);
}
