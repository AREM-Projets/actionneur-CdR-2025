# Servo PWM

## Objectif de la bibliothèque

L'objectif de cette bibliothèque est de fournir une alternative optimisée à la bibliothèque standard STM32duino `<Servo>`.
Bien que simple à utiliser, la bibliothèque standard repose sur une implémentation **logicielle (software)** du signal PWM, ce qui présente plusieurs inconvénients :

* Elle **n'exploite pas les périphériques matériels (timers PWM)** disponibles sur les microcontrôleurs STM32.
* Elle peut **limiter les performances**, surtout lorsque plusieurs servos ou d'autres périphériques temps-réel sont utilisés.
* Elle **masque l'utilisation réelle des ressources hardware**, ce qui peut poser problème pour des projets complexes.

## Fonctionnalités

* Initialisation d’un signal PWM sur n’importe quel timer/canal supporté.
* Possibilité de choisir un timer/canal précis ou de laisser la bibliothèque en allouer un automatiquement.
* Contrôle facile de la largeur d’impulsion (en microsecondes).
* Interface simple et compatible avec une utilisation bas niveau STM32.

## Exemple d'utilisation

```cpp
#include "Servo_pwm.h"

Servo_pwm monServo(PA8);  // Utilisation automatique d'un timer

void setup() {
    // Centrer le servo
    monServo.writeMicroseconds(1500);
}

void loop() {
    // Balayage du servo de 1000µs à 2000µs
    for (int us = 1000; us <= 2000; us += 10) {
        monServo.writeMicroseconds(us);
        delay(20);
    }
}
```
