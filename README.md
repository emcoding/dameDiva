# Components

## HC-SR501 PIR Bewegingssensor infrarood

Upside down, wiring at other side: 2 potmeters: left : distance, right: time
Afstand: 3-7 m
Time delay: how long it stays triggered/on

Jumper: L/H L = timer stops looking, until TIDE delay finished, H: timer restarts with every motion

Works: Both potmeters fully counterclockwise, flat sides to the left, seen from front, dome up.
Left is time (vertraging, hoe lang het aan blijft staan), right is sensitivity.

## Motor 28bYj-48, with UNL2003 driver
See arduino-experiments repo for the driver test (running leds)

stepsPerRevolution = 2048; for this motor; for other motors its listed in the datasheet or the Internet knows
each step ≈ 11.25º
360º / 11.25 = 32 steps per revolution
64 : 1 gear reduction in this motor => 32 steps * 64 = 2048

Learned from Rachel de Barros Stepper motor vid + https://youtu.be/PEg4bMFyHeM?si=3zoFUsg9fZ5GFgZG&t=642 for servo. 
