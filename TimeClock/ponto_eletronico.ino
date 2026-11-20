
#include <Keypad.h>
#include <LiquidCrystal.h>

#define MAX_PASSWORD 16
#define MAX_REGISTRATION 10

#define ROWS 4
#define COLS 4

#define RS_PIN 31
#define E_PIN 30
#define D4_PIN 34
#define D5_PIN 35
#define D6_PIN 32
#define D7_PIN 33
#define VO_PIN 2


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

void defaultLCD() {
  lcd.noBlink();
  lcd.clear();
  lcd.print("hh:mm");
}

void setup() {
  Serial.begin(9600);
  inputWord[0] = '\0';
  pinMode(VO_PIN, OUTPUT);
  analogWrite(VO_PIN, 60);
  lcd.begin(16, 2);
  defaultLCD();
}

void setTitle(const char* title) {
  lcd.setCursor(0, 0);
  lcd.print(title);
  lcd.setCursor(0, 1);
  lcd.blink();
}

void resetWord() {
  currIndex = 0;
  inputWord[0] = '\0';
}

void eraseWord() {
  lcd.setCursor(0, 1);
  lcd.print("               ");
}

void deleteKey() {
  if (currIndex > 0) {
    currIndex--;
    inputWord[currIndex] = '\0';
  }
}

void eraseKey() {
  lcd.setCursor(currIndex, 1);
  lcd.print(' ');
  lcd.setCursor(currIndex, 1);
}

void addKey(char key) {
  if (currIndex == 0) {
    setTitle("NUM MATRICULA:");
  }
  if (currIndex < MAXIN) {
    inputWord[currIndex] = key;
    currIndex++;
    inputWord[currIndex] = '\0';
  }
}

void printKey(char key) {
  lcd.setCursor(currIndex - 1, 1);
  lcd.print(key);
}

// Mode managing

// 0 : Operational
// 1 : Admin auth
// 2 : Admin

byte mode = 0;

void mode() {
  switch (mode)
  {
  case 0:
    mode = 1;
    titlePage("ADMIN PASSWORD");
    break;
  case 1:
    mode = 0;
    defaultPage();
  case 2:
    mode = 0;
    defaultPage();
  default:
    break;
  }
}

void cancel() {

}

void backspace() {

}

void confirm() {

}

void digit(const byte digit) {

}

void loop() {
  char key = keypad.getKey();

  if (key) {

    switch (key) {
      case 'A':
        mode();
        break;
      case 'B':
        //functionless
        break;
      case 'C':
        //functionless
        break;
      case 'D':
        cancel();
        break;
      case '*':
        backspace();
        break;
      case '#':
        confirm();
        break;
      default:
        digit();
        break;
    }
  }
}
