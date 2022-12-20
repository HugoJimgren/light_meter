float ljus = 0;
int potentiometer = 0;
float shutter[] = {0, -8, -4, -2, -1, 2, 4, 8, 15, 30, 60, 125, 250, 500, 1000, 2000};
int buttonState1 = 0;
int buttonState2 = 0;
float aperture[] = {0, 1.8, 2.8, 4, 5.6, 8, 11, 16, 22};
float apertureOut = 0;
float shutterOut = 0;
int iso = 400;
int potMap = 0;
int potMap1 = 0;
float k = 0;
float kPotMap = 0;
int intPotMap = 0;

#include <U8glib.h>

U8GLIB_SSD1306_128X64 oled(U8G_I2C_OPT_NONE);

// Definierar vilken pin som går till vad, startar serial monitor
void setup() {
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  Serial.begin(9600);
}

// Tar reda på de två knapparnas tillstånd och går sedan antingen in i funktionen AV eller TV
void loop() {
  buttonState1 = digitalRead(2);
  buttonState2 = digitalRead(3);
  UpdateOled();
  if (buttonState1 == HIGH) {
    AV();
  }
  if (buttonState2 == HIGH) {
    TV();
  }
}

// Aperture priority, tar in ett värde till bländaren (aperture) från potentiometern och beräknar ett värde för slutartid (shutter)
void AV() {
  potentiometer = analogRead(A1);
  potMap = map(potentiometer, 0, 1020, 1, 9);
  ljus = map(analogRead(A0), 0, 1024, 1024, 0);
  k = ljus / 1000;
  kPotMap = potMap / k;
  intPotMap = kPotMap;
  apertureOut = aperture[potMap];
  Serial.println(kPotMap);
  shutterOut = shutter[map(intPotMap, 50, 0, 1, 16) - 6];
  draw();
}

// Shutter priority, tar in ett värde till slutartid (shutter) från potentiometern och beräknar ett värde för bländaren (aperture)
void TV() {
  potentiometer = analogRead(A1);
  potMap = map(potentiometer, 0, 1020, 1, 16);
  potMap1 = map(potentiometer, 0, 1020, 1, 9);
  ljus = analogRead(A0);
  k = ljus / 1000;
  kPotMap = potMap1 / k;
  intPotMap = kPotMap;
  shutterOut = shutter[potMap];
  Serial.println(kPotMap);
  apertureOut = aperture[map(intPotMap, 50, 0, 1, 9) - 4];
  draw();
}

// Uppdaterar oled skärm
void UpdateOled() {
  oled.firstPage();
    do {
    draw();
  } while(oled.nextPage());
}

// Skriver ut alla värden på skärmen
void draw() {
  oled.setFont(u8g_font_helvB10);
  if (shutterOut > 0){
    oled.drawStr(35, 25, String("1/").c_str());
    oled.drawStr(50, 25, String(shutterOut).c_str());
  }
  else {
    oled.drawStr(40, 25, String(shutterOut * -1).c_str());
  }
  oled.drawStr(0, 25, String(apertureOut).c_str());
  oled.drawStr(100, 25, String(ljus).c_str());
  oled.drawStr(0, 50, String("iso:").c_str());
  oled.drawStr(40, 50, String(iso).c_str());
}
