
<style>
summary {
    font-size: 1.5em;
    font-weight: bold;
}
</style>

# **Projet Initial de Système Embarqué**

**Équipe**: 0314 </br>
**Auteurs**: Asmaa Sabouri, Sara Dakir, Imane Abdeljalil, Chaïmaa Mesbah </br>
**Nom du robot**: Moulay Sultan</br>

Ce projet conciste en le robot effectuant une épreuve sur un parcours. Le robot doit chercher une extrémité, traverser </br>
le parcours en trouvant les poteaux et afficher le rapport décrivant ses constats pour chaque mode. </br>
Ce document contient des informations suplémentaires concernant notre projet.



<details>
<summary>Comment utiliser notre code</summary>

### **1. Compiler la librairie**

- Compiler la librairie pour avoir accès aux classes implémentées.
- Utiliser la commande suivante dans le terminal au ***répertoire du fichier lib*** :


```
make
```

### **2. Compiler le code et programmer le robot**

- Brancher le robot à l'ordinateur par câble USB.
- Compiler et télécharger le code du fichier main avec la commande suivante dans le terminal au ***répertoire du fichier app*** :

```
make install
```
- Débrancher le robot de l'ordinateur afin qu'il soit indépendant

### **3.1 Mode Chercher Extrémité** 

- Placer le robot sur le **point A ou B** en l'alignant sur une des quatre lignes de l'intersection en X.
- Choisir le mode en appuyant sur le ***bouton reset*** lorsque la DEL est allumée en ***vert***.


### **3.2 Mode Traverser Parcours** 

- Placer le robot sur le **point C ou D** avec son nez pointant vers l'intersection en X.
- Placer un poteau sur le **point 1 ou 2**.
- Choisir le mode en appuyant sur le ***bouton reset*** lorsque la DEL est allumée en ***rouge***.

- Attendre que le clignotement en rouge se termine.
- Appuyer sur le ***bouton interrupt*** pour démarrer le robot.
- Lorsque le robot s'arrête devant le poteau, déplacer ce dernier au point 3 ou 4.

- Répéter les deux étapes précédentes pour le point 5 ou 6.

- Appuyer une dernière fois sur le ***bouton interrupt*** pour que le robot avance vers le point E.

### **3.3 Mode Afficher Rapport** 

- Brancher le robot à l'ordinateur par câble USB.
- Effectuer la commande suivante dans le terminal :


```
serieViaUSB -l
```
- Choisir le mode en appuyant sur le ***bouton reset*** lorsque la DEL est allumée en ***ambre***.
- Observer le rapport s'afficher sur l'écran.

</details>

<details>
<summary>Les fichiers principaux</summary>


## Fichier App

<p> Ce fichier contient le code principal du projet, c'est-à-dire le main qui programmera le robot lors de l'épreuve.</br>
Le code du main permet de choisir dans quel mode entrera le robot et dicte à ce dernier quoi faire pour chaque mode à effectuer.</br>
Voici la machine à états responsable de cela :

| **État actuel**        | **Entrée**                      | **État suivant**         | **Action**                                 |
|------------------------|---------------------------------|-------------------------|--------------------------------------------|
| **AUCUN_MODE**         | Bouton reset appuyé pendant DEL **verte** | `CHERCHER_EXTREMITE`    | Appel de fonction de cycle de sélection    |
| **AUCUN_MODE**         | Bouton reset appuyé pendant DEL **rouge** | `TRAVERSER_PARCOURS`    | Idem                                       |
| **AUCUN_MODE**         | Bouton reset appuyé pendant DEL **ambre** | `AFFICHER_RAPPORT`      | Idem                                       |
| **AUCUN_MODE**         | Bouton reset **non appuyé**              | `AUCUN_MODE`            | Idem                                       |
|------------------------|---------------------------------|-------------------------|--------------------------------------------|
| **CHERCHER_EXTREMITE** | Aucune                         | `AUCUN_MODE`            | Clignoter **vert**, démarrer recherche     |
| **TRAVERSER_PARCOURS** | Aucune                         | `AUCUN_MODE`            | Clignoter **rouge**, démarrer parcours     |
| **AFFICHER_RAPPORT**   | Aucune                         | `AUCUN_MODE`            | Clignoter **ambre**, envoyer rapport       |



## Fichier Lib 

<p>Ce fichier contient la librairie où toutes les classes nécessaires à l'implémentation du projet sont codées. Voici les algorithmes importants à comprendre.

### **Classe ChercherExtremite**

<p> Cette classe utilise l'information traitée et retournée par les méthodes de la classe CapteurLigne afin de permettre au robot de trouver une extrémité.</br> 
Voici l'algorithme qui constitue la méthode principale de cette classe :

### Switch Case : Trouver Extrémité


1. **SUIVRE_LIGNE**  
   - Si **3 capteurs allumés** → `VERIFIER_CAPTEURS_ALLUME`.  
   - Si **capteurs éteints + intersection** → Tourner à droite pour retrouver la ligne.  
   - Sinon, continuer à suivre la ligne.

2. **VERIFIER_CAPTEURS_ALLUME**  
   - Si **5 capteurs allumés + 0 éteints** → `VERIFIER_EXTREMITE_GAUCHE`.  
   - Si **5 capteurs allumés + ≤ 2 éteints** → Incrémenter l'intersection et retourner à `SUIVRE_LIGNE`.  
   - Si **2-4 capteurs allumés + capteur gauche/droit éteint** → Respectivement `CAPTEUR_GAUCHE_ETEINT` ou `CAPTEUR_DROIT_ETEINT`.  
   - Sinon, retourner à `SUIVRE_LIGNE`.

3. **VERIFIER_EXTREMITE_GAUCHE**  
   - Si **ligne trouvée à gauche** → Incrémenter le compteur et retourner à `SUIVRE_LIGNE`.  
   - Si **capteurs éteints + pas de ligne** → `VERIFIER_EXTREMITE_DROITE`.

4. **VERIFIER_EXTREMITE_DROITE**  
   - Si **ligne trouvée à droite** → Incrémenter le compteur et retourner à `SUIVRE_LIGNE`.  
   - Si **capteurs éteints + pas de ligne** → `EXTREMITE_TROUVER`.

5. **CAPTEUR_GAUCHE_ETEINT**  
   - Ajuster trajectoire à gauche.  
   - Incrémenter le compteur et retourner à `SUIVRE_LIGNE`.

6. **CAPTEUR_DROIT_ETEINT**  
   - Ajuster trajectoire à droite.  
   - Incrémenter le compteur et retourner à `SUIVRE_LIGNE`.

7. **EXTREMITE_TROUVER**  
   - Repositionner le robot correctement à l'extrémité.  
   - Marquer l'extrémité comme trouvée, terminer le cycle.


### **Classe TrouverPoteau**

<p>Cette classe utilise l'information traitée et retournée par les méthodes des classes CapteurLigne et CapteurDistance afin de traverser le parcours </br> 
tout en trouvant le poteau. Voici l'algorithme qui constitue la méthode principale de cette classe :

### Switch Case : Trouver Poteau

1. **SUIVRE_LIGNE**  
   - Si **poteau à 10 cm** → `POTEAU_DETECTER_DIX_CM`.  
   - Si **3 capteurs allumés** → `VERIFIER_CAPTEURS_ALLUME`.  
   - Sinon, continuer à suivre la ligne.

2. **POTEAU_DETECTER_DIX_CM**  
   - Exécuter des actions pour le poteau à 10 cm.  
   - Rester dans cet état jusqu'à un nouvel événement.

3. **VERIFIER_CAPTEURS_ALLUME**  
   - Si **< 3 capteurs allumés** → Retourner à `SUIVRE_LIGNE`.  
   - Si **intersection détectée** → `INTERSECTION_DETECTEE`.

4. **INTERSECTION_DETECTEE**  
   - Ajuster direction pour continuer sur la ligne.  
   - Incrémenter le compteur d'intersection.  
   - Transitionner à l'état suivant selon le compteur d'intersection.

5. **POTEAU_DETECTER_VINGT_CM**  
   - Exécuter des actions pour le poteau à 20 cm.  
   - Avancer vers le poteau si la ligne est détectée devant.

6. **ACTION_FINALISEE**  
   - Terminer ou retourner à `SUIVRE_LIGNE`.  
   - Toutes les actions nécessaires sont complétées.


</details>



