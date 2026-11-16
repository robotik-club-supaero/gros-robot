# Memo
Pas besoin de ```Serial.begin(9600);```

Pour pouvoir utiliser un interrupteur avec la résistance interne de la teensy au lieu d'en rajouter un pour qu'un 0 soit bien lu comme un 0.
```pinMode(8, INPUT_PULLUP);```
> Usually when using the pullup resistor, you will turn it on with INPUT_PULLUP and leave it on. However, you can turn it on and off. When the pin is in input mode, you can turn the pullup resistor on and off by using digitalWrite. That isn't very intuitive, writing to a pin which is input mode, but that is how it works. INPUT_PULLUP is a Teensy extension. On regular Arduino boards, digitalWrite the only way to access the pullup resistor.

# Changelog
J'ai mis old_synth_test.txt qui est l'ancien main, et old_platformio.ini.ini qui est l'ancien platformio.ini pour éviter de devoir recompiler la librairie audio et ILI9341_t3 à chaque fois et enlever le build flag de la teensy. 