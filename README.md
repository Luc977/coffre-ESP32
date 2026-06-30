# 🔒 Coffre Fort ESP32 AVEC CLAVIER 4X4

Projet complet : Schéma KiCad + Code Arduino + Démo.

### 1. Hardware
Dans `/Hardware` : Fichiers KiCad V6. Ouvre `cofre_v1_kicard.pdf` pour voir le schéma sans KiCad.

### 2. Code 
Dans `/Code` : `sketch_cofre_fort_v1` à ouvrir avec Arduino IDE.

### 3. Démo
  OUVRIR media photo+video pour voir image du montage et video demo

### Composants
ESP32-WROOM-32, LCD I2C, Servo SG90, Buzzer, 2 LED(rouge,vert) , 2 Résistance, un clavier 4X4 une plaquette d'éssaie et des fils de connexion

### FONTIONNEMENT
 ## 1. 
    A l'allumage du dispositif le lcd affiche code et l'utilisateur entre le code a 4 chiffres(1234 initiale du programme)
    et aussi a chaque touche appuyer le buzzer fait bip
  
 ## 2.
     Le code correct entrer la led vert s'allume pour affirmer que le code es correct et le servo tourne pour debloauer la serrure sinon 
     led rouge s'allume et lcd affiche bad cod pour affirmer que c'est un mauvais code le servo reste inactif
 
 ## 3.
     Apres trois tentatifs erroné le dispiositif signale trop d'erreur et se bloque pendant une minute
 
## 4. 
    LA touche # permet de valider les codes a chaque fois et la touche A permet de changer le code pin
 
## 5. 
    Pour changer le code pin il suffit d'appuyer sur la touche A et le lcd affiche entrer l'ancien code une fois l'ancien code entrer
    correctement il te demande d'entrer ton nouveau code et apres le nouveau es bien enregistrer et c'est avec ce nouveau code Que tu         peut    débloqué encor le servo
