
#include <Keypad.h>
#include <LiquidCrystal.h>

// Input lenght control
#define MAX_PASSWORD 16
#define MAX_REGISTRATION 10

// Keypad pins
#define ROWS 4
#define COLS 4

// LCD display pins
#define RS_PIN 31
#define E_PIN 30
#define D4_PIN 34
#define D5_PIN 35
#define D6_PIN 32
#define D7_PIN 33
#define VO_PIN 2


// HARDWARE CONFIGURATION

// Keypad configuration
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

// LCD display configuration
LiquidCrystal lcd(
  RS_PIN,
  E_PIN,
  D4_PIN,
  D5_PIN,
  D6_PIN,
  D7_PIN
);


// PAGES

byte activeTutors = 0;

void defaultPage() {
  lcd.noBlink();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("hh:mm ");
  lcd.setCursor(0, 1);
  lcd.print("Tutoring: ");
  lcd.print(activeTutors);
}

void titlePage(const char* title) {
  if (title.length() <= MAX_REGISTRATION) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(title);
    lcd.setCursor(0, 1);
  }
}

void adminPage() {
  lcd.noBlink();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("1.clock 2.clear");
  lcd.setCursor(0, 1);
  lcd.print("3.+reg  4.-reg ");
}


// CONFIG FUNCTIONS

// 0 : Operational
// 1 : Admin auth
// 2 : Admin
// 3 : Set clock

byte mode = 0;
void mode() {
  switch (mode)
  {
  case 0:
    mode = 1;
    titlePage("ADMIN PASSWORD");
    break;
    
  default:
    mode = 0;
    defaultPage();
    break;
  }
}

void tutors() {

}

void language() {

}


// INPUT FUNCTIONS 

char inputDigits[MAXIN + 1] = "";
byte nextIndex = 0;

void cancel() {
  nextIndex = 0;
  inputDigits = "";
  defaultPage();
}

void backspace() {
  if (nextIndex > 0) {
    // internal storage managing
    nextIndex--;
    inputDigits[nextIndex] = "\0";

    //lcd magaging
    lcd.setCursor(nextIndex, 1);
    lcd.print(' ');
    lcd.setCursor(nextIndex, 1);
  }
}

void confirm() {

  lcd.noBlink();

  switch (mode)
  {
  case 0: // operational
    if (checkRegistration()) {

      titlePage("SMILE");
      registerPhoto(); //TO DO
      delay(5000);

      if (checkActive()) {
        activeTutors--;
        titlePage("WELL DONE!");
      }
      else {
        activeTutors++;
        titlePage("GOOD TOTURING,");
        lcd.print(inputDigits);
      }
    }
    else {
      titlePage("NOT FOUND");
    }
    delay(3000); //3s to read text

    defaultPage();
    break;

  case 1: // admin auth
    if (checkPassword()) {
      mode = 2;

      titlePage("WELCOME ADMIN");
      delay(2000);

      adminPage(); // TO DO
    }
    else {
      titlePage("WRONG PASSWORD");
      delay(3000);

      mode = 0;
      defaultPage();
    }
    break;

  case 2: // admin
    addRegistration();
    break;

  default:
    defaultPage();
    break;
  }
}

void digit(const byte digit) {
  switch (mode)
  {
  case 0:
    if (nextIndex <= MAX_REGISTRATION) {
      if (nextIndex == 0) {
        titlePage("REGISTRATION:");
      }

      //internal managing
      inputDigits[nextIndex] = digit;
      nextIndex++;
      inputDigits[nextIndex] = '\0';

      //lcd managing
      lcd.print(digit);
    }
    break;

  case 1:
    if (nextIndex <= MAX_PASSWORD) {
      if (nextIndex == 0) {
        titlePage("PASSWORD:");
      }

      //internal managing
      inputDigits[nextIndex] = digit;
      nextIndex++;
      inputDigits[nextIndex] = '\0';

      //lcd managing
      lcd.print('*');
    }
    break;

  case 2:
    // TO DO
  
  default:
    break;
  }
}


// SETUP

void setup() {
  Serial.begin(9600;);
  
  // TODO REMOVE VO_PIN AND USE 2k resistor instead
  pinMode(VO_PIN, OUTPUT);
  analogWrite(VO_PIN, 60);

  lcd.begin(16, 2);
  defaultPage();
}


// MAIN LOOP

//time management
unsigned long lastMillis = millis();

void loop() {
  char key = keypad.getKey();

  if (key) {
    lastMillis = millis();

    switch (key) {
      case 'A':
        mode();
        break;
      case 'B':
        tutors();
        break;
      case 'C':
        language();
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
  else if (millis() - lastMillis > 120000) {
    if (random(100) % 7 == 0) {
      // TO DO scrt
    }
    else {
      defaultPage();
    }
  }
}
