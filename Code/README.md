# Code Carte Actionneur - AREM CdR 2025  

Répertoire de travail du code de la carte STM32 des actionneurs pour la Coupe de France de Robotique 2025.  

## Documentation  

Chaque partie du code est découpée en bibliothèques. Chaque bibliothèque doit posséder sa propre documentation dans son répertoire sous la forme d’un fichier README situé dans l’arborescence :  
📂 *./lib/"nomDeBibliothèque"/README.md*  

## Test de code

Afin de tester des sous-parties de votre code, au lieu de manipuler directement le main possèdant possiblement du code important, n'hésitez pas à utiliser les fichiers de [test_bench](lib/test_bench/).

Pour activer la ```void bench_loop()``` et la ```void bench_setup()```, definissez la constante ```TEST_BENCH_ENABLE``` à 1 au lieu de 0 dans le fichier [test_bench.h](test_bench.h).

## Prérequis de compréhension  

Le code nécessite quelques notions de base afin d’être compris et maintenu sur la durée.  

### 🔹 Programmation orientée objet (POO)  

Le code étant écrit en **C++/framework Arduino**, il repose sur le paradigme de la programmation orientée objet. Connaître les bases des **classes C++** est donc un atout.  

👉 **En résumé :**  
La **programmation orientée objet (POO)** est une méthode d'organisation du code qui regroupe **les données** et **les comportements** associés sous forme d’objets.  

**Exemple :**  
Imaginons un objet `Voiture_t`.  

- Il possède des **attributs** (ou propriétés) comme :  
  - `nb_portes` (nombre de portes)  
  - `type_moteur` (électrique, essence, etc.)  
  - `kilometrage`  
- Il a aussi des **méthodes** (ou fonctions associées) comme :  
  - `demarrer()` (démarrer la voiture)  
  - `rouler(vitesse)` (avancer à une certaine vitesse)  
  - `arreter()` (arrêter la voiture)  

L'objectif de la POO est de **structurer le code**, **simplifier la maintenance** et **favoriser la réutilisation** en encapsulant les données et les comportements. Cela permet aussi à un développeur ne connaissant pas notre module de l’utiliser facilement.  

### 🔹 Développement embarqué  

Le code fonctionne en **baremetal** (sans système d’exploitation). Il est donc essentiel d’avoir des notions de base en **développement embarqué**. De manière générale, comme nous travaillons sur STM32 avec framework arduino, [**je vous invite très fortement à consulter la documentation ST pour exploiter tous les périphériques du microcontroleurs !**](https://github.com/stm32duino/Arduino_Core_STM32/wiki/API)



📌 Concepts clés à connaître pour le code de la carte :  
- **Interruption**  
- **Périphériques matériels courants** (notamment **UART** et **timers**)  

👉 **Les interruptions :**  
Une interruption est un événement, soit matériel (ex : signal d’un capteur), soit logiciel, qui **suspend temporairement l’exécution normale du programme**. Le microcontrôleur exécute alors un **traitement spécifique** avant de reprendre l’exécution là où il s’était arrêté.  

**Exemple concret :**  
Un capteur de fin de course envoie un signal lorsque le bras d’un robot atteint une position extrême. Ce signal génère une **interruption**, qui déclenche une action spécifique (ex : arrêt du moteur).  

👉 **L'UART :**  
Le protocole **UART** (*Universal Asynchronous Receiver Transmitter*) est un protocole de communication série asynchrone qui permet l'échange de données entre deux dispositifs électroniques. Il fonctionne sans horloge partagée, en envoyant les données bit par bit avec des bits de start, de stop et éventuellement un bit de parité pour la détection d'erreurs. Il est couramment utilisé pour la communication entre microcontrôleurs. 
**C'est par ailleurs par ce protocole que la carte d'actionneur devra communiquer avec la raspberry pi (le coeur de réfléxion du robot).**

👉 **Les Timers :**  
Les timers sont basiquement des compteus dans le microcontrôleur qui s'incrémente à chaque coup de clock qu'ils recoivent. En connaissant la fréquence de leurs incrémentations, nous pouvons déterminer des durées temporelles. Si leurs fonctionnement exact est vu en cours de SAM et ne sera pas abordé ici, la bibliothèque STM32 **HardwareTimer** fait un gros travail d'abstraction pour les rendres utilisable le plus facilement possible. C'est donc une ressource que je vous invite à consulter. 
**Important :** C'est à partir des timers que les signaux PWM du microcontreur sont générés. Derrière la fonction analogWrite(num_pin) c'est un timer qui est appelé. Méfiez vous, vérifiez bien que l'un des timers que vous utilisez n'est pas utilisé deux fois pour deux applications différentes.

## Pour plus de questions : Utilisez le discord AREM ! 