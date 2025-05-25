# 📄 Documentation Technique

**Projet** : Système de préhension motorisé (Grippers + moteur pas-à-pas)
**Auteur** : Lïam LOTTE – EI22
**Date de création** : 23/02/2025

---

## 🔧 Dépendances et Fichiers

Le projet dépend des fichiers suivants :

* `stepper_motor.hpp` : gestion du moteur pas-à-pas.
* `servo_pwm.hpp` : contrôle des servomoteurs via PWM.
* `test_bench.hpp` : (optionnel) permet de tester individuellement les composants.

---

## ⚙️ Matériel utilisé

| Composant         | Port          | Description                             |
| ----------------- | ------------- | --------------------------------------- |
| Moteur pas-à-pas  | D9, D10       | Commandé via Timer1                     |
| Servo de grippers | A5            | Ouvre et ferme les pinces               |
| Servo de poussée  | A2            | Position de repos et de poussée         |
| Raspberry Pi      | UART (Serial) | Communication série (entrées `u` / `d`) |

---

## 🔁 États de la machine à états (FSM)

Le système suit un cycle défini par une machine à états finis (FSM) avec les transitions contrôlées par l'utilisateur via le port série (`u` pour monter, `d` pour descendre).

### 🧠 États possibles

| État             | Description                                                            |
| ---------------- | ---------------------------------------------------------------------- |
| `IDLE_DOWN`      | Position initiale, grippers ouverts. Attente de la commande de montée. |
| `CLOSE_GRIPPERS` | Ferme les grippers.                                                    |
| `GO_UP`          | Monte l’objet avec le moteur pas-à-pas.                                |
| `IDLE_UP`        | En haut, en attente d'une commande de descente.                        |
| `GO_DOWN`        | Redescend à la position de départ.                                     |
| `OPEN_GRIPPERS`  | Ouvre les grippers.                                                    |

---

## 🔄 Logique de transition (fonction `FSM_transition`)

Les transitions dépendent du caractère reçu depuis le port série :

* Depuis `IDLE_DOWN`, si `'u'` est reçu → `CLOSE_GRIPPERS`
* Depuis `IDLE_UP`, si `'d'` est reçu → `GO_DOWN`
* Les autres transitions sont automatiques (sans entrée requise)

---

## ⚙️ Actions associées aux états (fonction `FSM_state`)

Chaque état déclenche une action spécifique :

* `CLOSE_GRIPPERS` : Positionne le servo des pinces à `CLOSED_POSITION` (0°).
* `GO_UP` : Fait tourner le moteur pas-à-pas vers le haut.
* `GO_DOWN` : Fait tourner le moteur pas-à-pas vers le bas.
* `OPEN_GRIPPERS` : Ouvre les grippers (`OPEN_POSITION` = 140°).
* `IDLE_*` : Aucune action.

---

## 🔧 Initialisation (fonction `setup`)

1. Exécute `test_bench()` si `TEST_BENCH_ENABLE == 1`.
2. Initialise la communication série (`Serial.begin(9600)`).
3. Positionne les servos à leurs positions initiales :

   * Grippers fermés puis ouverts.
   * Servo de poussée à `PUSHED_POSITION`, puis en `IDLE_POSITION`.
4. Envoie `"Initialized"` à la Raspberry Pi.

---

## 🔁 Boucle principale (fonction `loop`)

1. Lit un caractère depuis le port série.
2. Exécute une transition d'état avec `FSM_transition()`.
3. Affiche l'état courant sur le port série.
4. Exécute l’action associée à l’état courant via `FSM_state()`.

---

## 💬 Communication série

* Entrée :

  * `'u'` : démarre le cycle de montée.
  * `'d'` : déclenche la descente.
* Sortie :

  * Envoie l'état courant sous forme d'entier (`(int)current_state`).
  * Envoie `"Initialized"` à la fin du `setup()`.

---

## 📌 Définitions importantes

| Macro                  | Valeur | Rôle                                         |
| ---------------------- | ------ | -------------------------------------------- |
| `NUMBER_OF_STEP_TO_DO` | 1200   | Nombre de pas pour chaque déplacement moteur |
| `MOTOR_SPEED`          | 2      | Vitesse du moteur                            |
| `CLOSED_POSITION`      | 0      | Position des grippers fermés                 |
| `OPEN_POSITION`        | 140    | Position des grippers ouverts                |
| `PUSHED_POSITION`      | 30     | Position active du servo-poussoir            |
| `IDLE_POSITION`        | 140    | Position de repos du servo-poussoir          |

---

## ✅ Comportement attendu

1. **Au démarrage** :

   * Les grippers s’ouvrent et les servos sont calibrés.
   * Le message `Initialized` est envoyé.

2. **Commande `'u'` reçue** :

   * Ferme les grippers → Monte → Attente en haut.

3. **Commande `'d'` reçue** :

   * Descend → Ouvre les grippers → Retour à l’état initial.
