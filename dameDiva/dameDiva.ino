#include <Arduino.h>

// HC-SR501 PIR Bewegingssensor infrarood
// Upside down, wiring at other side: 2 potmeters: left : distance, right: time
// Afstand: 3-7 m
// Time delay: how long stays triggered
// Jumper: L/H L = timer stops looking, until TIDE delay finished, H: timer restarts with every motion

// Works now, both potmeters fully counterclockwise, flat sides to the left, seen from front, dome up.
// PROBABLY left is time (vertraging) , right is sensitivity OR the other way around

// Version 20 May 2026: add led parallel, second eye
// Version 21 May 2026 😍 : add a wink
// Version 22 May

// Led eyes
constexpr byte rightEyePin = 13;
constexpr byte leftEyePin = 8;
constexpr byte eyes[] = {
  rightEyePin, leftEyePin
};

// pir: passive infrared sensor, detects movement
constexpr byte pirSensorPin = 12;

constexpr unsigned long winkPeriod   = 5000;  // ms tussen winks
constexpr unsigned long winkDuration = 150;   // ms dat rechteroog knipoogt

void setup() {
  pinMode(pirSensorPin, INPUT); // read the pir value: movement == ON

  for (byte pin : eyes) {
    pinMode(pin, OUTPUT);       // LED's on/off in sync with pir value
    digitalWrite(pin, LOW);     // Start with leds off
  }
}


// De sluiting van de ogen was niet sync te krijgen met `if pirValue` varianten (ook met millis ipv delay). Het knipogende oog ging
// altijd eerder uit. Deze oplossing gebruikt een tijdcyclus: millis() % 5000 , en vergelijkt dat met de
// winkPeriod van 150 ms = "Phase based timing", stateless timing.
bool winking() {
  return (millis() % winkPeriod) < winkDuration;
}

void loop() {
  bool pirValue = digitalRead(pirSensorPin);
  for (byte pin : eyes) digitalWrite(pin, pirValue);   // basis: beide ogen samen
  if (pirValue && winking()) digitalWrite(rightEyePin, LOW);  // overlay: rechteroog knipt
}
