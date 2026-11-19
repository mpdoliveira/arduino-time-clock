
#include <Keypad.h>
#include <LiquidCrystal.h>

#define MAXIN 10

#define ROWS 4
#define COLS 4

#define RS_PIN 31
#define E_PIN 30
#define D4_PIN 7
#define D5_PIN 6
#define D6_PIN 5
#define D7_PIN 4
#define VO_PIN 2
#define BLA_PIN 3


LiquidCrystal lcd(
  RS_PIN,
  E_PIN,
  D4_PIN,
  D5_PIN,
  D6_PIN,
  D7_PIN
);

char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

const byte rowPins[ROWS] = {22, 24, 26, 28};
const byte colPins[COLS] = {23, 25, 27, 29};

Keypad keypad = Keypad(
  makeKeymap(hexaKeys),
  rowPins,
  colPins,
  ROWS,
  COLS
);

char inputWord[MAXIN + 1] = "";
byte currIndex = 0;

void setup() {
  Serial.begin(9600);
  pinMode(VO_PIN, OUTPUT);
  pinMode(BLA_PIN, OUTPUT);
  analogWrite(VO_PIN, 60);
  analogWrite(BLA_PIN, 60);
  lcd.begin(16, 2);
  lcd.print("Trabalha,Karina!");
}

void resetWord() {
  currIndex = 0;
  inputWord[0] = '\0';
}

void deleteKey() {
  if (currIndex > 0) {
    currIndex--;
    inputWord[currIndex] = '\0';
  }
}

void addKey(char key) {
  if (currIndex < MAXIN) {
    inputWord[currIndex] = key;
    currIndex++;
    inputWord[currIndex] = '\0';
  }
}

byte vo_light = 60;
byte bla_light = 60;

void light_set(byte pin, byte value) {
  if (pin == VO_PIN) {
    vo_light += value;
    analogWrite(VO_PIN, vo_light / 4);
  }
  else if (pin == BLA_PIN) {
    bla_light += value;
    analogWrite(BLA_PIN, bla_light / 4);
  }
}

void loop() {
  char key = keypad.getKey();

  if (key) {
    switch (key) {
      case '*':
        deleteKey();
        break;
      case '#':
        resetWord();
        break;
      case 'A':
        light_set(VO_PIN, 10);
        break;
      case 'B':
        light_set(VO_PIN, -10);
        break;
      case 'C':
        light_set(BLA_PIN, 10);
        break;
      case 'D':
        light_set(BLA_PIN, -10);
        break;
    }
  }
  else if (key) {
    addKey(key);
  }
  
}
