#include <Keypad.h>
#include <LiquidCrystal.h>
#include <string.h>

// Input length control
#define MAX_PASSWORD 16
#define MAX_REGISTRATION 10
#define MAX_REGISTRATIONS 30
#define MAX_INPUT MAX_PASSWORD

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

// HARDWARE CONFIGURATION
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

const byte rowPins[ROWS] = {22, 24, 26, 28};
const byte colPins[COLS] = {23, 25, 27, 29};

Keypad keypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
LiquidCrystal lcd(RS_PIN, E_PIN, D4_PIN, D5_PIN, D6_PIN, D7_PIN);

// STATE & STORAGE
byte activeTutors = 0;

// 0 : Operational
// 1 : Admin auth
// 2 : Admin Menu
// 3 : Set clock
// 4 : Add registration
byte currMode = 0;

char inputDigits[MAX_INPUT + 1] = "";
byte nextIndex = 0;
unsigned long lastMillis = 0;

byte nextReg = 0;
char registrations[MAX_REGISTRATIONS][MAX_REGISTRATION + 1];

// PAGES
void defaultPage() {
  lcd.noBlink();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("hh:mm");
  lcd.setCursor(0, 1);
  lcd.print("Tutoring: ");
  lcd.print(activeTutors);
}

void titlePage(const char* title) {
  if (strlen(title) > 16) {
    return;
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(title);
  lcd.setCursor(0, 1);
}

void adminPage() {
  lcd.noBlink();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("1.relog 2.zerar");
  lcd.setCursor(0, 1);
  lcd.print("3.ad/rm monitor");
}

// STUBS FOR PENDING HARDWARE / LOGIC
void registerPhoto() {}
bool checkActive() { return false; }
void setClock() {}

// INPUT & REGISTRATION HELPERS
void eraseInput() {
  nextIndex = 0;
  inputDigits[0] = '\0';
}

void addRegistration() {
  if (nextReg < MAX_REGISTRATIONS && nextIndex > 0) {
    strlcpy(registrations[nextReg], inputDigits, sizeof(registrations[nextReg]));
    nextReg++;
    eraseInput();
    titlePage("CADASTRADO!");
    delay(1500);
  } else {
    eraseInput();
    titlePage("ERRO CADASTRO");
    delay(1500);
  }
  currMode = 2;
  adminPage();
}

void deleteRegistrations() {
  for (byte i = 0; i < nextReg; i++) {
    registrations[i][0] = '\0';
  }
  nextReg = 0;
}

bool checkRegistration() {
  for (byte i = 0; i < nextReg; i++) {
    if (strcmp(registrations[i], inputDigits) == 0) {
      return true;
    }
  }
  return false;
}

bool checkPassword() {
  return strcmp(inputDigits, "12345") == 0;
}

// CONFIG FUNCTIONS
void mode() {
  eraseInput();
  switch (currMode) {
    case 0:
      currMode = 1;
      titlePage("SENHA:");
      lcd.blink();
      break;
    default:
      currMode = 0;
      defaultPage();
      break;
  }
}

void tutors() {}
void language() {}

// INPUT FUNCTIONS
void cancel() {
  currMode = 0;
  eraseInput();
  defaultPage();
}

void backspace() {
  if (nextIndex > 0) {
    nextIndex--;
    inputDigits[nextIndex] = '\0';

    lcd.setCursor(nextIndex, 1);
    lcd.print(' ');
    lcd.setCursor(nextIndex, 1);
  }
}

void confirm() {
  lcd.noBlink();

  switch (currMode) {
    case 0: // operational
      if (checkRegistration()) {
        titlePage("SORRIA");
        registerPhoto();
        delay(5000);

        if (checkActive()) {
          if (activeTutors > 0) {
            activeTutors--;
          }
          titlePage("PRONTO!");
        } else {
          activeTutors++;
          titlePage("BOA MONITORIA!");
        }
        eraseInput();
      } else {
        titlePage("NAO ENCONTRADO");
      }
      delay(3000);
      cancel();
      break;

    case 1: // admin auth
      if (checkPassword()) {
        currMode = 2;
        eraseInput();
        titlePage("BEM VINDO");
        delay(2000);
        adminPage();
      } else {
        titlePage("SENHA INCORRETA");
        delay(3000);
        cancel();
      }
      break;

    case 2: // admin menu (no action on '#' alone)
      break;

    case 4: // confirm new registration
      addRegistration();
      break;

    default:
      eraseInput();
      defaultPage();
      break;
  }
}

void digit(char key) {
  switch (currMode) {
    case 0:
      if (nextIndex < MAX_REGISTRATION) {
        if (nextIndex == 0) {
          titlePage("MATRICULA:");
          lcd.blink();
        }
        inputDigits[nextIndex] = key;
        nextIndex++;
        inputDigits[nextIndex] = '\0';
        lcd.print(key);
      }
      break;

    case 1:
      if (nextIndex < MAX_PASSWORD) {
        if (nextIndex == 0) {
          titlePage("SENHA:");
          lcd.blink();
        }
        inputDigits[nextIndex] = key;
        nextIndex++;
        inputDigits[nextIndex] = '\0';
        lcd.print('*');
      }
      break;

    case 2:
      switch (key) {
        case '1':
          currMode = 3;
          titlePage("RELOGIO");
          setClock();
          break;
        case '2':
          deleteRegistrations();
          titlePage("ZERADO!");
          delay(1500);
          adminPage();
          break;
        case '3':
          currMode = 4;
          eraseInput();
          titlePage("MATRICULA:");
          lcd.blink();
          break;
      }
      break;

    case 4:
      if (nextIndex < MAX_REGISTRATION) {
        inputDigits[nextIndex] = key;
        nextIndex++;
        inputDigits[nextIndex] = '\0';
        lcd.print(key);
      }
      break;

    default:
      break;
  }
}

void setup() {
  Serial.begin(9600);

  lcd.begin(16, 2);
  defaultPage();
  lastMillis = millis();
}

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
        digit(key);
        break;
    }
  } else if (millis() - lastMillis > 120000) {
    lastMillis = millis();
    if (random(100) % 7 == 0) {
      // TODO scrt
    } else {
      defaultPage();
    }
  }
}