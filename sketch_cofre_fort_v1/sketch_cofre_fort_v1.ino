#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <ESP32Servo.h>
#include <Preferences.h> // AJOUT 1

LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo myservo;
Preferences prefs; // AJOUT 2

// Clavier 4x4
const byte ROWS = 4; const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[] = {13, 12, 14, 27};
byte colPins[] = {26, 25, 33, 32};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

#define SERVO_PIN 15
#define LED_ROUGE 4
#define LED_VERTE 2
#define BUZZER 16

String codeSecret;
String codeSaisi = "";

int essaisRestants = 3; // Compteur essais
bool bloque = false;    // État blocage
unsigned long tempsBlocage = 0; // Timer blocage
void bip(int duree) {
  digitalWrite(BUZZER, HIGH);
  delay(duree);
  digitalWrite(BUZZER, LOW);
}

void setup() {
  prefs.begin("coffre", false); // AJOUT 3 : Ouvre mémoire
  codeSecret = prefs.getString("pin", "1234"); // Lit code sauvé, sinon 1234 par défaut
  
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Coffre V1");
  
  myservo.attach(SERVO_PIN);
  myservo.write(0);
  
  pinMode(LED_ROUGE, OUTPUT);
  pinMode(LED_VERTE, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  
  delay(2000);
  lcd.clear();
  lcd.print("Code: ");
}

void loop() {
  gererBlocage(); // Vérifie blocage à chaque tour
  
  //char key = keypad.getKey();
  char key = keypad.getKey();
  
  if (key) {
    bip(100);
    
    if (key == '#') { // Valider
  if(bloque) {
    bip(300); bip(300); // Bip erreur si bloqué
  }
  else if (codeSaisi == codeSecret) {
    lcd.clear();
    lcd.print("Code OK! OPEN");
    digitalWrite(LED_VERTE, HIGH);
    myservo.write(90);
    delay(3000);
    myservo.write(0);
    digitalWrite(LED_VERTE, LOW);
    essaisRestants = 3; // Reset essais si bon code
  } else {
    essaisRestants--;
    lcd.clear();
    lcd.print("BAD COD!");
    lcd.setCursor(0,1);
    lcd.print("Essais: " + String(essaisRestants));
    digitalWrite(LED_ROUGE, HIGH);
    bip(300); delay(100); bip(300);
    digitalWrite(LED_ROUGE, LOW);
    
    // Si 0 essais restants → blocage 10s
    if(essaisRestants <= 0) {
      bloque = true;
      tempsBlocage = millis();
      lcd.clear();
      lcd.print("Trop erreurs!");
      
    }
  }
  codeSaisi = "";
  delay(5000);
  lcd.clear();
  lcd.print("Code: ");
} else if (key == '*') { // Effacer
      codeSaisi = "";
      lcd.setCursor(6,0);
      lcd.print("    ");
      lcd.setCursor(6,0);
      
    } else if (key == 'A') { // CHANGER CODE avec touche A
      changerCode();
      
    } else if (codeSaisi.length() < 4) {
      codeSaisi += key;
      lcd.print("*");
    }
  }
}
void gererBlocage() {
  if(bloque) {
    lcd.setCursor(0,1);
    lcd.print("Bloque 1MIN...   ");
    
    // Compte à rebours 10 secondes
    if(millis() - tempsBlocage >= 60000) {
      bloque = false;
      essaisRestants = 3;
      lcd.clear();
      lcd.print("Code: ");
    }
  }
}

void changerCode() {
  if(bloque) return; // Pas de changement si bloqué
  
  lcd.clear();
  lcd.print("Ancien code:");
  String ancien = lireCode(4);
  
  if(ancien != codeSecret) {
    lcd.clear(); 
    lcd.print("Code faux!"); 
    bip(300); 
    delay(1000);
    lcd.clear(); 
    lcd.print("Code: ");
    codeSaisi = "";
    return;
  }
  
  lcd.clear(); 
  lcd.print("Nouv. code:");
  String newCode = lireCode(4);
  
  prefs.putString("pin", newCode);
  codeSecret = newCode;
  
  lcd.clear(); 
  lcd.print("Code change!"); 
  bip(300); bip(300);
  delay(1500);
  lcd.clear(); 
  lcd.print("Code: ");
  codeSaisi = "";
}

String lireCode(int taille) {
  String code = "";
  while(code.length() < taille) {
    char k = keypad.getKey();
    if(k >= '0' && k <= '9') {
      code += k;
      lcd.print("*");
      bip(100);
    }
  }
  return code;
}