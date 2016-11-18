#include <WaveHC.h>
#include <WaveUtil.h>


SdReader card;    // This object holds the information for the card
FatVolume vol;    // This holds the information for the partition on the card
FatReader root;   // This holds the information for the volumes root directory
WaveHC wave;      // This is the only wave (audio) object, since we will only play one at a time

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

void setup() {
  Serial.begin(9600);
  setupSdReader(card);
  setupFatVolume(vol);
  setupFatReader(root);
}

void loop() {

}
