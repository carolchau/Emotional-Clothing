#include <WaveHC.h>
#include <WaveUtil.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 9, 8, 7, 6);

String fileNames[4] = {"BUGS2.WAV", "DAFFY1.WAV", "BUGS1.WAV", "DAFFY2.WAV"};

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

void setup() {
  Serial.begin(9600);
  setupSdReader(card);
  setupFatVolume(vol);
  setupFatReader(root);
  setupRGBSensor(tcs);
  lcd.begin(16, 2);
  lcd.print("Before");
}

void loop() {
  root.rewind();
  FatReader file;

  for (int i = 0; i < 4; i++) {
    char fileName[fileNames[i].length() + 1];
    fileNames[i].toCharArray(fileName, fileNames[i].length() + 1);

    file.open(root, fileName);
    wave.create(file);
    wave.play();
    while (wave.isplaying) delay(100);
  }
}
