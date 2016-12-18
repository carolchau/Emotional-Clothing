/*
 * WaveHC and WaveUtil libraries are provided by Adafruit.
 * Source: https://github.com/adafruit/WaveHC
 *
 * RGB Color Sensor library (Adafruit_TCS34725) is also provided by Adafruit.
 * Source: https://github.com/adafruit/Adafruit_TCS34725
 */

#include <WaveHC.h>
#include <WaveUtil.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include <LiquidCrystal.h>
#include "ColorUtil.h"

LiquidCrystal lcd(A0, A1, 9, 8, 7, 6);

const int button = A3;

SdReader card;    // This object holds the information for the card
FatVolume vol;    // This holds the information for the partition on the card
FatReader root;   // This holds the information for the volumes root directory
WaveHC wave;      // This is the only wave (audio) object, since we will only play one at a time

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);

void setupSdReader(SdReader &card) {
  if (!card.init()) Serial.println("Card init failed");
  card.partialBlockRead(true); // Optimization. Disable if having issues
}

void setupFatVolume(FatVolume &vol) {
  uint8_t slot;
  // There are 5 slots to look at.
  for (slot = 0; slot < 5; slot++)
    if (vol.init(card, slot)) break;
  if (slot == 5) Serial.println("No valid FAT partition");
}

void setupFatReader(FatReader &root) {
  if (!root.openRoot(vol)) Serial.println("Can't open root dir");
}

void setupRGBSensor(Adafruit_TCS34725 &tcs) {
  if (!tcs.begin()) Serial.println("No TCS34725 found ... check your connections");
}

void displayDirections() {
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Hover clothing");
  lcd.setCursor(0, 1);
  lcd.print("over light.");
}

void setup() {
  Serial.begin(9600);
  setupSdReader(card);
  setupFatVolume(vol);
  setupFatReader(root);
  setupRGBSensor(tcs);
  displayDirections();
  pinMode(button, INPUT);
}

void play() {
  uint16_t clear, red, green, blue;

  tcs.setInterrupt(false);
  delay(60);
  tcs.getRawData(&red, &green, &blue, &clear);
  tcs.setInterrupt(true);

  uint32_t sum = clear;
  float r, g, b;
  r = red; r /= sum;
  g = green; g /= sum;
  b = blue; b /= sum;
  r *= 256; g *= 256; b *= 256;

  double hsl[3];
  getHSL(hsl, r, g, b);
//  Serial.print("r: "); Serial.print((int) r);
//  Serial.print(" g: "); Serial.print((int) g);
//  Serial.print(" b: "); Serial.print((int) b); Serial.println();
//  Serial.print("H: "); Serial.print(hsl[0]);
//  Serial.print(" S: "); Serial.print(hsl[1]);
//  Serial.print(" L: "); Serial.print(hsl[2]); Serial.println();

  int hue = hsl[0];
  double saturation = hsl[1];
  double light = hsl[2];

  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0); lcd.print('R'); lcd.print(r, 0);
  lcd.setCursor(5, 0); lcd.print('G'); lcd.print(g, 0);
  lcd.setCursor(10, 0); lcd.print('B'); lcd.print(b, 0);
  lcd.setCursor(0, 1); lcd.print('H'); lcd.print(hue);
  lcd.setCursor(5, 1); lcd.print('S'); lcd.print(saturation * 100, 0); lcd.print('%');
  lcd.setCursor(10, 1); lcd.print('L'); lcd.print(light * 100, 0); lcd.print('%');

  root.rewind();
  FatReader file;

  if (light < 0.2) {                // Black
    file.open(root, "gmechngd.wav");
  } else if (light > 0.8) {         // White
    file.open(root, "avemaria.wav");
  } else if (saturation < 0.05) {   // Gray
    file.open(root, "veridquo.wav");
  } else if (hue < 5) {            // Red
    file.open(root, "nitefate.wav");
  } else if (hue < 50) {            // Orange
    file.open(root, "mariothm.wav");
  } else if (hue < 110) {            // Yellow
    file.open(root, "wiishopc.wav");
  } else if (hue < 170) {           // Green
    file.open(root, "swedenmc.wav");
  } else if (hue < 230) {           // Blue
    file.open(root, "animcros.wav");
  } else if (hue < 360) {           // Magenta
    file.open(root, "wndrland.wav");
  }

  if (!wave.create(file)) Serial.println("Can't open the file");
  wave.play();
  while(wave.isplaying) delay(100);

  displayDirections();
}

void loop() {
  int state = digitalRead(button);
  bool pushed = (state == LOW);
  if (pushed) {
    play();
  }

  delay(50);
}
