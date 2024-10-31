# librairie

# Utilisation de `Del`

### **Inclure la bibliothèque**

Pour utiliser la classe `Del`, tu dois inclure l'en-tête `del.h` dans ton fichier principal :

```cpp
#include "del.h"
```

### **Initialiser un objet de type `Del`**

Une fois que tu as inclus la bibliothèque, tu peux créer un objet de la classe `Del`. Le constructeur va automatiquement configurer les broches **PB0** et **PB1** comme sorties.

```cpp
Del del;  // Création de l'objet
```

### **Allumer la DEL dans une couleur spécifique**

Tu peux allumer la DEL avec une des couleurs définies (ROUGE, VERT, AMBRE) et spécifier un délai en millisecondes si nécessaire :

```cpp
del.allumer(ROUGE);    // Allume la DEL en rouge sans délai
del.allumer(VERT, 500); // Allume la DEL en vert pendant 500 ms

```

### **Éteindre la DEL**

Utilise la méthode `eteindre()` pour éteindre la DEL.

```cpp
del.eteindre();  // Éteint la DEL

```

### **Faire clignoter la DEL**

La méthode `clignoter()` permet de faire clignoter la DEL un certain nombre de fois. Il faut spécifier la couleur, le nombre de clignotements, et le délai entre chaque allumage/extinction.

```cpp
del.clignoter(AMBRE, 5, 300);  // La DEL clignote en ambre 5 fois avec un délai de 300 ms

```

### **Exemple d'utilisation complète**

Voici un exemple simple qui illustre comment utiliser la classe `Del` pour allumer, éteindre, et faire clignoter la DEL :

```cpp
#include "del.h"

int main() {
    Del del;  // Création de l'objet DEL

    // Allumer la DEL en rouge pendant 1 seconde
    del.allumer(ROUGE, 1000);

    // Éteindre la DEL
    del.eteindre();

    // Faire clignoter la DEL en vert 3 fois avec un délai de 500 ms
    del.clignoter(VERT, 3, 500);

    return 0;
}

```

**Résumé des fonctions :**

- **`Del()`** : Initialise la DEL.
- **`allumer(Couleur couleur, uint16_t delai = 0)`** : Allume la DEL avec la couleur spécifiée et un délai facultatif.
- **`eteindre()`** : Éteint la DEL.
- **`clignoter(Couleur couleur, uint8_t nombreClignotements, uint16_t delai)`** : Fait clignoter la DEL avec la couleur et le délai spécifiés.

---

# Classe `Bouton`

La classe `Bouton` permet de gérer les interactions avec des boutons poussoirs connectés aux broches du microcontrôleur. Elle inclut des fonctions pour vérifier si un bouton est appuyé ou relâché, et elle gère le comportement des boutons en tenant compte que, pour le **bouton blanc**, lorsque le bouton est appuyé, la lecture donne `0` et lorsqu'il est relâché, elle donne `1`.

### Utilisation

### Inclure la bibliothèque

Pour utiliser la classe `Bouton`, inclure le fichier `bouton.h` dans ton programme principal :

```cpp
#include "bouton.h"

```

### Branchement des boutons

- **Bouton noir** : Connecté à la broche **PD2**.
- **Bouton blanc** : Connecté à la broche **PD3** (comportement déjà pris en compte où appuyer = 0 et relâcher = 1).

### Créer un objet `Bouton`

Créer un objet de la classe `Bouton` en spécifiant la broche à laquelle le bouton est connecté (**PD2** pour le bouton noir, **PD3** pour le bouton blanc) :

```cpp
Bouton boutonNoir(PD2);  // Bouton noir sur PD2
Bouton boutonBlanc(PD3); // Bouton blanc sur PD3
```

### Fonctions principales

### `estAppuyer()`

- **Description** : Vérifie si le bouton est enfoncé.
- **Utilisation** : Utilisé pour détecter si un bouton est appuyé.

**Exemple** :

```cpp
if (boutonNoir.estAppuyer()) {
    // Action si le bouton noir est appuyé
}

```

### `estRelacher()`

- **Description** : Vérifie si le bouton est relâché.
- **Utilisation** : Utilisé pour détecter si un bouton est relâché.

**Exemple** :

```cpp
if (boutonBlanc.estRelacher()) {
    // Action si le bouton blanc est relâché
}

```

### `desactiverInteruption()`

- **Description** : Désactive l'interruption associée au bouton. Utile pour éviter des déclenchements intempestifs lorsque l'interruption n'est plus nécessaire.

**Exemple** :

```cpp
boutonNoir.desactiverInteruption();  // Désactiver l'interruption pour le bouton noir

```

### Exemple d'utilisation dans `main()`

Voici un exemple de programme qui utilise les fonctions `estAppuyer()`, `estRelacher()` et `desactiverInteruption()` pour interagir avec les boutons noir et blanc :

```cpp
#include "bouton.h"
#include "del.h"

int main() {
    // Créer les objets Bouton pour le bouton noir et le bouton blanc
    Bouton boutonNoir(PD2);
    Bouton boutonBlanc(PD3);
    Del del;

    while (true) {
        // Si le bouton noir est appuyé, allumer la DEL en rouge
        if (boutonNoir.estAppuyer()) {
            del.allumer(ROUGE);
        }
        // Si le bouton noir est relâché, allumer la DEL en vert
        else if (boutonNoir.estRelacher()) {
            del.allumer(VERT);
        }
    }
    
    while (true) {
        // Si le bouton blanc est appuyé, allumer la DEL en rouge
        if (boutonBlanc.estAppuyer()) {
            del.allumer(ROUGE);
        }
        // Si le bouton blanc est relâché, allumer la DEL en vert
        else if (boutonBlanc.estRelacher()) {
            del.allumer(VERT);
        }
    }
    return 0;
}

```

### **Amélioration avec interruptions**

Si tu préfères utiliser les interruptions pour une gestion asynchrone, voici comment procéder :

```cpp
ISR(INT0_vect) {
    if (bouton1.estRelacher()) {
        del.allumer(ROUGE);
    }
}

ISR(INT1_vect) {
    if (bouton2.estAppuyer()) {
        del.allumer(VERT);
    }
}

int main() {
    // Initialisation des interruptions
    sei();

    while (true) {
        // Le programme peut faire autre chose pendant que les interruptions gèrent les boutons.
    }
}
```

### Explication des fonctions :

- **Initialisation** : Les boutons sont initialisés sur **PD2** (noir) et **PD3** (blanc).
- **Détection d'appui** : Lorsque le bouton noir ou blanc est appuyé, la DEL s'allume en rouge. Lorsqu'il est relâché, elle s'allume en vert.
- **Désactivation d'interruption** : Si nécessaire, tu peux désactiver l'interruption associée à un bouton en appelant `desactiverInteruption()`.

---

# Classe `can` : Convertisseur Analogique / Numérique (CAN)

La classe `can` permet de lire des valeurs analogiques à partir des broches du port A (PA0 à PA7) et de les convertir en valeurs numériques. Le résultat est automatiquement décalé pour garder les 8 bits les plus significatifs des 10 bits de conversion.

### Inclure la bibliothèque

Inclure le fichier `can.h` dans ton programme :

```cpp
#include "can.h"
```

### Créer un objet `can`

Créer un objet de la classe `can` pour initialiser le convertisseur :

```cpp
can convertisseur;  // Initialise le CAN
```

### Lire une valeur analogique sur PA0

Utiliser la méthode `lecture()` pour lire une valeur analogique sur la broche **PA0**. Le résultat est un entier de 8 bits.

```cpp
uint8_t valeur = convertisseur.lecture(PA0);  // Lecture sur PA0
```

### Détails techniques

- **Résultat sur 8 bits** : Le CAN retourne une valeur sur 10 bits, mais seuls les 8 bits les plus significatifs sont conservés grâce à un décalage.

---

# Classe `Minuterie`

La classe `Minuterie` permet de gérer un minuteur basé sur le **Timer 1** du microcontrôleur AVR en mode CTC (Clear Timer on Compare Match). Elle inclut des fonctionnalités pour démarrer, arrêter et réinitialiser la minuterie avec différents prescalers.

### Inclure la bibliothèque

Inclure le fichier `minuterie.h` dans ton programme :

```cpp
#include "minuterie.h"

```

### Créer un objet `Minuterie`

Créer un objet de la classe `Minuterie` pour initialiser le minuteur :

```cpp
Minuterie minuterie;  // Initialise le Timer 1 en mode CTC

```

### Démarrer la minuterie avec une période et un prescaler

La méthode `demarrer()` permet de lancer la minuterie avec une période en millisecondes, et tu peux spécifier directement le **prescaler**. Le prescaler détermine la fréquence de comptage du minuteur.

- **Période** : La durée en millisecondes après laquelle une interruption sera générée.
- **Prescaler** : La valeur du prescaler pour ajuster la vitesse du minuteur. Tu peux choisir entre les valeurs suivantes :
    - **AUCUN** (1)
    - **PRESCALER_8** (8)
    - **PRESCALER_64** (64)
    - **PRESCALER_256** (256)
    - **PRESCALER_1024** (1024) (par défaut si non spécifié)

Exemple d'utilisation :

```cpp
minuterie.demarrer(1000, PRESCALER_64);  // Démarre la minuterie avec une période de 1000 ms et un prescaler de 64

```

Dans cet exemple, la minuterie démarre avec une période de 1000 millisecondes (1 seconde) et utilise un prescaler de 64.

Si tu veux utiliser le prescaler par défaut (1024), tu peux simplement omettre le deuxième argument :

```cpp
minuterie.demarrer(500);  // Démarre la minuterie avec une période de 500 ms et le prescaler par défaut (1024)

```

### Arrêter la minuterie

La méthode `arreter()` désactive la minuterie, stoppe les interruptions, et remet le compteur à zéro.

```cpp
minuterie.arreter();  // Arrête la minuterie

```

### Réinitialiser la minuterie

La méthode `reinitialiser()` remet le compteur et les registres de contrôle à zéro sans redémarrer la minuterie.

```cpp
minuterie.reinitialiser();  // Réinitialise la minuterie

```

**Détails techniques**

- **Mode CTC** : Le Timer 1 est configuré en mode CTC (Clear Timer on Compare Match), ce qui génère des interruptions à des intervalles réguliers.
- **Prescalers** : Les valeurs de prescaler modifient la vitesse à laquelle le minuteur compte les cycles d'horloge. Plus le prescaler est grand, plus le minuteur compte lentement.

---

# Classe `Roue`

La classe `Roue` permet de contrôler indépendamment ou ensemble les roues gauche et droite d’un robot, en utilisant les fonctions pour avancer, reculer, tourner et ajuster la vitesse des roues. Il est important de bien gérer l'arrêt entre les changements de direction (par exemple, entre avancer et ralentir).

### Inclure la bibliothèque

Pour utiliser la classe `Roue`, tu dois inclure le fichier `roue.h` dans ton programme principal :

```cpp
#include "roue.h"
```

### Branchement des roues

- **Roue gauche** : Connectée à la broche **PD6** pour le contrôle de la vitesse (PWM) et **PD4** pour la direction.
- **Roue droite** : Connectée à la broche **PD7** pour la vitesse et **PD5** pour la direction.

### Créer un objet `Roue`

Créer un objet de la classe `Roue` pour initialiser le contrôle des deux roues :

```cpp
Roue roue;  // Initialise les roues gauche et droite
```

### Contrôler une roue individuellement

- **Description** : Contrôle une seule roue en spécifiant laquelle (GAUCHE ou DROITE), la direction (AVANCER ou RECULER), et la vitesse (en pourcentage).

**Exemple** :

```cpp
roue.controlerRoue(GAUCHE, AVANCER, 50);  // La roue gauche avance à 50% de la vitesse
roue.controlerRoue(DROITE, RECULER, 30);  // La roue droite recule à 30% de la vitesse

```

### Avancer les deux roues ensemble

- **Description** : Fait avancer les deux roues à la même vitesse.

**Exemple** :

```cpp
roue.avancer(75);  // Le robot avance à 75% de la vitesse

```

### Reculer les deux roues ensemble :

- **Description** : Fait reculer les deux roues à la même vitesse.

**Exemple** :

```cpp
roue.reculer(50);  // Le robot recule à 50% de la vitesse

```

### Tourner le robot

- **Tourner à gauche** : La roue gauche ralentit et la droite avance plus vite.
- **Tourner à droite** : La roue droite ralentit et la gauche avance plus vite.

**Exemples** :

```cpp
roue.tournerGauche(80);  // Le robot tourne à gauche
roue.tournerDroite(80);  // Le robot tourne à droite

```

### Tourner sur place

- **Description** : Fait tourner le robot sur place en avançant une roue et en reculant l'autre.

**Exemple** :

```cpp
roue.tournerSurPlace(60);  // Le robot tourne sur place à 60% de la vitesse

```

### Arrêter les roues

- **Description** : Arrête immédiatement les deux roues.

**Exemple** :

```cpp
roue.arreter();  // Arrête les roue
```

### Accélérer une roue

- **Description** : Augmente progressivement la vitesse d'une roue jusqu'à la vitesse cible, en utilisant un incrément pour la progression.

**Exemple** :

```cpp
roue.accelerer(GAUCHE, 80, 10);  // La roue gauche accélère progressivement jusqu'à 80% de la vitesse

```

### Ralentir une roue

- **Description** : Diminue progressivement la vitesse d'une roue jusqu'à la vitesse cible.

**Exemple** :

```cpp
roue.ralentir(DROITE, 30, 5);  // La roue droite ralentit progressivement jusqu'à 30% de la vitesse

```

### Important pour le changement de direction

**Passer de `avancer()` à `ralentir()` :**

Toujours utiliser `arreter()` entre deux commandes pour éviter des comportements inattendus.

**Exemple** :

```cpp
roue.avancer(60);   // Le robot avance à 60%
roue.arreter();     // Arrête les roues avant de ralentir
roue.ralentir(GAUCHE, 20, 5);  // La roue gauche ralentit progressivement
```

---

# Classe `Usart`

La classe `Usart` permet de configurer et d'utiliser l'interface UART pour la communication série entre le microcontrôleur AVR et un PC (ou un autre dispositif). Elle permet d'envoyer des données ou des messages texte via une connexion série.

### Inclure la bibliothèque

Pour utiliser la classe `Usart`, inclure le fichier `usart.h` dans ton programme :

```cpp
#include "usart.h"
```

### Créer un objet `Usart`

Créer un objet de la classe `Usart` pour initialiser la communication série :

```cpp
Usart usart;  // Initialise l'USART avec 2400 bauds
```

### Fonctions principales

### `transmettre(uint8_t donnee)`

- **Description** : Cette fonction permet d'envoyer un octet de données via l'interface série.
- **Utilisation** : Elle attend que le buffer de transmission soit vide avant d'envoyer la donnée.

**Exemple** :

```cpp
usart.transmettre('A');  // Envoie la lettre 'A' au PC via l'interface série
```

### `envoyerMessage(const char *message)`

- **Description** : Cette fonction permet d'envoyer une chaîne de caractères (message) via la liaison série. Elle envoie chaque caractère un par un en utilisant la fonction `transmettre()`.

**Exemple** :

```cpp
usart.envoyerMessage("Bonjour!");  // Envoie le message "Bonjour!" au PC

```

### Détails techniques

- **Baud rate** : La communication est initialisée à **2400 bauds** (UBRR0L = 0xCF).
- **Transmission et réception** : Activée pour envoyer et recevoir des données série via les broches TX/RX.
- **Format des trames** : 8 bits de données, 1 bit de stop, sans parité.

---

### Exemple de programme `main()`

```cpp
#include "usart.h"

int main() {
    // Créer un objet Usart pour initialiser la communication série
    Usart usart;

    // Envoyer un message au démarrage
    usart.envoyerMessage("Initialisation de l'USART...\\n");

    // Boucle principale
    while (true) {
        // Envoyer un caractère unique
        usart.transmettre('A');  // Envoie la lettre 'A'
        _delay_ms(1000);         // Attendre 1 seconde

        // Envoyer un autre message
        usart.envoyerMessage("Hello, World!\\n");

        _delay_ms(1000);         // Attendre 1 seconde avant de répéter
    }

    return 0;
}

```

### Explication

- **Initialisation** : Le programme commence par initialiser l'USART en créant un objet `Usart`.
- **Envoyer des données** :
    - Il envoie d'abord un message d'initialisation avec `envoyerMessage()`.
    - Ensuite, dans la boucle `while`, il envoie un caractère 'A' toutes les secondes.
    - Après cela, il envoie un message "Hello, World!".