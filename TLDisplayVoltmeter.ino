#include <Wire.h>

#define MIRQ 2
#define R1 18000 //Резистор в верхнем плече
#define R2 4300 //Резистор в нижнем плече
#define Vref 3300

#define NO_HU

char displayText[] = "bat 127V";
unsigned long timing;

void sendStringToDisplay(char* str, bool dotPointEnable = false)
{
    Wire.beginTransmission(0x23);
    Wire.write(0x0F);
    Wire.write(0x90);
    Wire.write(0x7F);
    Wire.write(0x55);
    Wire.write(0xFF);
    Wire.write(0xFF);
    Wire.write((!dotPointEnable) << 6);
    Wire.write(0x01);
    Wire.write(str[0]);
    Wire.write(str[1]);
    Wire.write(str[2]);
    Wire.write(str[3]);
    Wire.write(str[4]);
  // put your setup code here, to run once:
    Wire.write(str[5]);
    Wire.write(str[6]);
    Wire.write(str[7]);
}

void setup() {
  Wire.begin();
  TWBR = 0xff;
  TWSR = 0x01;
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(MIRQ, INPUT);
  pinMode(A0, INPUT);
}

void loop() {

if (millis() - timing > 2000){ // Обновление данных на дисплее
  #ifdef NO_HU
  while(!digitalRead(MIRQ));
  if (digitalRead(MIRQ))
  {
    pinMode(MIRQ, OUTPUT);
    digitalWrite(MIRQ,LOW);
    delay(1);
    Wire.beginTransmission(0x23);
    Wire.write(0x01);
    Wire.write(0x11);
    Wire.endTransmission();
    pinMode(MIRQ, INPUT);
    delay(1);
  }
  #endif

  while(!digitalRead(MIRQ));
  if (digitalRead(MIRQ))
  {
    pinMode(MIRQ, OUTPUT);
    digitalWrite(MIRQ,LOW);
    //delay(20);
    char c[3];
    unsigned int voltageInmV = (((1.0 * Vref / 1024)*analogRead(A0)) / R2) * (R1 + R2);
    sprintf(c, "%03d", voltageInmV / 100);
    displayText[4] = c[0];
    displayText[5] = c[1];
    displayText[6] = c[2];
    sendStringToDisplay(displayText,true);
    Wire.endTransmission();
    pinMode(MIRQ, INPUT);
    delay(1);
  }
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  timing = millis(); 
 }
}
