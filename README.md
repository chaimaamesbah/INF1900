# **Projet Initial de Système Embarqué**

### **Informations Générales**
- **Équipe** : 0314  
- **Auteurs** : Asmaa Sabouri, Sara Dakir, Imane Abdeljalil, Chaïmaa Mesbah  
- **Nom du robot** : Moulay Sultan  

Ce projet consiste à programmer un robot autonome capable de réaliser les tâches suivantes :
1. Chercher une extrémité.  
2. Traverser un parcours en détectant les poteaux.  
3. Générer un rapport détaillant les constats pour chaque mode.  

Le robot utilise une machine à états pour gérer les différentes phases et transitions entre les modes.

---

## **Guide d'Utilisation**

### **1. Compiler la Librairie**
Avant d’utiliser le projet, compilez la librairie contenant toutes les classes nécessaires :  
- Accédez au répertoire `lib`.  
- Exécutez la commande suivante dans le terminal :  
  ```bash
  make
  ```

### **2. Compiler le Code Principal et Programmer le Robot**
- Connectez le robot à l’ordinateur via un câble USB.  
- Compilez et téléchargez le code principal situé dans le répertoire `app` avec la commande suivante :  
  ```bash
  make install
  ```
- Débranchez le robot pour qu'il puisse fonctionner de manière autonome.

### **3. Modes Disponibles**

#### **Mode 1 : Chercher Extrémité**
- Positionnez le robot sur le **point A ou B**, aligné sur l'une des lignes de l'intersection en X.  
- Sélectionnez le mode en appuyant sur le bouton ***reset*** lorsque la DEL est allumée en ***vert***.  

#### **Mode 2 : Traverser Parcours**
- Placez le robot sur le **point C ou D**, orienté vers l'intersection en X.  
- Ajoutez un poteau au **point 1 ou 2**.  
- Appuyez sur le bouton ***reset*** lorsque la DEL est allumée en ***rouge***.  
- Une fois que le robot s'arrête devant un poteau, déplacez celui-ci vers le prochain point désigné (3, 4, 5 ou 6).  
- Appuyez à nouveau sur le bouton ***interrupt*** pour poursuivre l’épreuve.  

#### **Mode 3 : Afficher Rapport**
- Connectez le robot à l’ordinateur via un câble USB.  
- Exécutez la commande suivante dans le terminal pour afficher les données :  
  ```bash
  serieViaUSB -l
  ```
- Sélectionnez ce mode en appuyant sur le bouton ***reset*** lorsque la DEL est allumée en ***ambre***.

---

## **Structure du Projet**

### **Fichier `app`**
Ce fichier contient le programme principal (`main`) qui :  
- Gère la sélection du mode à l’aide d’une machine à états.  
- Définit les comportements du robot pour chaque mode sélectionné.  

| **État actuel**        | **Entrée**                      | **État suivant**         | **Action**                                  |
|------------------------|---------------------------------|-------------------------|--------------------------------------------|
| **AUCUN_MODE**         | Bouton reset appuyé (DEL verte) | `CHERCHER_EXTREMITE`    | Activation du mode de recherche            |
| **AUCUN_MODE**         | Bouton reset appuyé (DEL rouge) | `TRAVERSER_PARCOURS`    | Activation du mode parcours                |
| **AUCUN_MODE**         | Bouton reset appuyé (DEL ambre) | `AFFICHER_RAPPORT`      | Activation du mode rapport                 |
| **CHERCHER_EXTREMITE** | Aucune                         | `AUCUN_MODE`            | Recherche d’extrémité                      |
| **TRAVERSER_PARCOURS** | Aucune                         | `AUCUN_MODE`            | Traversée du parcours                      |
| **AFFICHER_RAPPORT**   | Aucune                         | `AUCUN_MODE`            | Génération et affichage du rapport         |

### **Fichier `lib`**
Ce répertoire contient toutes les classes nécessaires pour le fonctionnement du robot, notamment :  

#### **Classe `ChercherExtremite`**
- Permet au robot de suivre les lignes et de détecter les extrémités à l’aide des capteurs de ligne.  

#### **Classe `TrouverPoteau`**
- Utilise les capteurs de ligne et de distance pour détecter les poteaux et gérer les intersections.  

---

## **Algorithmes Importants**

### **Algorithme de Recherche d’Extrémité**
1. **SUIVRE_LIGNE**  
   - Suivre la ligne jusqu’à une intersection.  
   - Ajuster la trajectoire si des capteurs latéraux sont éteints.  

2. **VERIFIER_CAPTEURS_ALLUME**  
   - Si tous les capteurs sont allumés, vérifier la présence d’une extrémité.  
   - Sinon, continuer à suivre la ligne.  

3. **EXTREMITE_TROUVEE**  
   - Lorsque l’extrémité est trouvée, repositionner le robot et terminer la tâche.  

### **Algorithme de Détection de Poteaux**
1. **SUIVRE_LIGNE**  
   - Suivre la ligne jusqu’à détecter un poteau à une distance spécifique.  

2. **POTEAU_DETECTE**  
   - Effectuer les actions nécessaires pour gérer le poteau détecté.  

3. **INTERSECTION_DETECTEE**  
   - Identifier les intersections et ajuster la direction pour poursuivre la tâche.  

---

## **Crédits**
- Ce projet a été réalisé dans le cadre du cours **INF1900** à Polytechnique Montréal.  
- Supervision par l’équipe pédagogique de Polytechnique Montréal.  

---
