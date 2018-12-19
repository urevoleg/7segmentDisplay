#include <Wire.h>
#include <DS3231.h>

DS3231 clock;
RTCDateTime dt;
/*
  Serial.print(dt.year);   Serial.print("-");
  Serial.print(dt.month);  Serial.print("-");
  Serial.print(dt.day);    Serial.print(" ");
  Serial.print(dt.hour);   Serial.print(":");
  Serial.print(dt.minute); Serial.print(":");
  Serial.print(dt.second); Serial.println("");
*/

int raz0 = 9;
int raz1 = 10;
int raz2 = 11;
int raz3 = 12;

int a = 1;
int b = 2;
int c = 3;
int d = 4;
int e = 5;
int f = 6;
int g = 7;
int h = 8;
int dotPin = 13;

int showTimeDigit = 1;
unsigned long lastUpdateClock = 0;
unsigned long lastUpdateTemp = 0;
unsigned long lastUpdateDot = 0;
int digitMass[] = {0, 0, 0, 0, 2};
int dsHour = 0;
int dsMin = 0;
int arduinoTemp = 0;

void setup() {

  clock.begin();
  // Set sketch compiling time
  clock.setDateTime(__DATE__, __TIME__);
  // настраиваем выход разрядов
  pinMode(raz0, OUTPUT);
  pinMode(raz1, OUTPUT);
  pinMode(raz2, OUTPUT);
  pinMode(raz3, OUTPUT);

  // настраиваем выход сегментов
  pinMode(a, OUTPUT);
  pinMode(b, OUTPUT);
  pinMode(c, OUTPUT);
  pinMode(d, OUTPUT);
  pinMode(e, OUTPUT);
  pinMode(f, OUTPUT);
  pinMode(g, OUTPUT);
  pinMode(h, OUTPUT);

  pinMode(dotPin, OUTPUT);

  // выключить все разряды
  digitalWrite(raz0, HIGH);
  digitalWrite(raz1, HIGH);
  digitalWrite(raz2, HIGH);
  digitalWrite(raz3, HIGH);
  lastUpdateClock = millis();
  lastUpdateDot = millis();
}

void loop() {
  if (millis() - lastUpdateDot >= 1000) {
    digitalWrite(dotPin, !digitalRead(dotPin));
    dt = clock.getDateTime();
    dsHour = dt.hour;
    dsMin = dt.minute;
    digitMass[0] = dsHour / 10;
    digitMass[1] = dsHour % 10;
    digitMass[2] = dsMin / 10;
    digitMass[3] = dsMin % 10;

    lastUpdateDot = millis();
  }

  // цикл отрисовки чисeл из массива digitMass
  while (millis() - lastUpdateClock < 500) {
    for (int i = 0; i < 4; i++) {
      viborRaz(i);
      digit(digitMass[3 - i]);
      delay(showTimeDigit);
    }
  }
  lastUpdateClock = millis();

  if (millis() - lastUpdateTemp >= 10000) {
    arduinoTemp = readTemp();
    digitMass[0] = arduinoTemp / 1000;
    digitMass[1] = arduinoTemp % 1000 / 100;
    digitMass[2] = arduinoTemp % 1000 % 100 / 10;
    digitMass[3] = arduinoTemp % 1000 % 100 % 10;
    unsigned long lastUpdate = millis();
    // цикл отрисовки чисeл из массива digitMass
    while (millis() - lastUpdate < 2500) {
      for (int i = 0; i < 5; i++) {
        if (i != 4) {
          viborRaz(i);
          digit(digitMass[3 - i]);
        } else {
          viborRaz(2);
          digitalWrite(h, HIGH);
        }
        delay(showTimeDigit);
      }
    }
    lastUpdateTemp = millis();
  }
}

/*unsigned long var = millis() / 1000;

   while (millis() - lastUpdate < 500) {
   // Разбиваем цифру по разрядам индикатора
   viborRaz(3); digit(var / 1000);
   viborRaz(2); digit((var % 1000) / 100);
   viborRaz(1); digit((var % 100) / 10);
   viborRaz(0); digit(var % 10);
   }
   lastUpdate = millis();*/
// бегущая полоска
/*viborRaz(0);
  digitalWrite(a, 1);
  delay(showTimeDigit);
  digitalWrite(a, 0);
  digitalWrite(b, 1);
  delay(showTimeDigit);
  digitalWrite(b, 0);
  digitalWrite(c, 1);
  delay(showTimeDigit);
  digitalWrite(c, 0);
  digitalWrite(d, 1);
  delay(showTimeDigit);
  viborRaz(1);
  delay(showTimeDigit);
  viborRaz(2);
  delay(showTimeDigit);
  viborRaz(3);
  delay(showTimeDigit);
  digitalWrite(d, 0);
  digitalWrite(e, 1);
  delay(showTimeDigit);
  digitalWrite(e, 0);
  digitalWrite(f, 1);
  delay(showTimeDigit);
  digitalWrite(f, 0);
  digitalWrite(a, 1);
  delay(showTimeDigit);
  viborRaz(2);
  delay(showTimeDigit);
  viborRaz(1);
  delay(showTimeDigit);*/
int readTemp() {
  long result;
  // Read temperature sensor against 1.1V reference
  ADMUX = _BV(REFS1) | _BV(REFS0) | _BV(MUX3);
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA, ADSC));
  result = ADCL;
  result |= ADCH << 8;
  int TEMP_INTERNAL = int(100 * (result - 125) * 0.1075);
  return TEMP_INTERNAL;
}

void nul() {
  digitalWrite(a, LOW);
  digitalWrite(b, LOW);
  digitalWrite(c, LOW);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  digitalWrite(g, LOW);
}

void zero() {
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, LOW);
}

void one() {
  digitalWrite(a, LOW);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  digitalWrite(g, LOW);
}

void two() {
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, LOW);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, LOW);
  digitalWrite(g, HIGH);
}

void three() {
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  digitalWrite(g, HIGH);
}

void four() {
  digitalWrite(a, LOW);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
}

void five() {
  digitalWrite(a, HIGH);
  digitalWrite(b, LOW);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, LOW);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
}

void six() {
  digitalWrite(a, HIGH);
  digitalWrite(b, LOW);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
}

void seven() {
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  digitalWrite(g, LOW);
}

void eight() {
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
}

void nine() {
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, LOW);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
}

void digit(int d) {
  switch (d) {
    case 0:
      zero();
      break;
    case 1:
      one();
      break;
    case 2:
      two();
      break;
    case 3:
      three();
      break;
    case 4:
      four();
      break;
    case 5:
      five();
      break;
    case 6:
      six();
      break;
    case 7:
      seven();
      break;
    case 8:
      eight();
      break;
    case 9:
      nine();
      break;
    case 11:
      nul();
      break;
  }
  delay(showTimeDigit);
}

void viborRaz(int num) {
  switch (num) {
    case 0:
      digitalWrite(raz0, LOW);
      //analogWrite(raz0, 250);
      digitalWrite(raz1, HIGH);
      digitalWrite(raz2, HIGH);
      digitalWrite(raz3, HIGH);
      break;
    case 1:
      digitalWrite(raz0, HIGH);
      digitalWrite(raz1, LOW);
      digitalWrite(raz2, HIGH);
      digitalWrite(raz3, HIGH);
      break;
    case 2:
      digitalWrite(raz0, HIGH);
      digitalWrite(raz1, HIGH);
      digitalWrite(raz2, LOW);
      digitalWrite(raz3, HIGH);
      break;
    case 3:
      digitalWrite(raz0, HIGH);
      digitalWrite(raz1, HIGH);
      digitalWrite(raz2, HIGH);
      digitalWrite(raz3, LOW);
      break;
  }
}
