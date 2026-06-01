(versions 2x May)


/*
Dit is voor cycli, dus terugkerende gebeurtenissen. Niet voor mijn doel: een eenmalige wink. Maar voor wiggles kan het misschien juist heel goed. 
"hoe laat is het modulo de cyclus".
 millis() — geeft het aantal milliseconden sinds de Arduino aan is gegaan.
 < winkDuration — true zolang de modulo tussen 1 en 150 zit: de eerste 150ms in een cyclus.

  millis() % 5000:  0   150   151   ...   4999   0   150
  winkActive:       T   T     F           F      T   T
                    ←150ms→                      ←150ms→
*/



/*
OP weg naar scheduler
 if (millis() % 5000 < 150)  winkRight();
  if (millis() % 7000 < 300)  raiseEyebrow();
  if (millis() % 11000 < 100) twitchMouth();
*/