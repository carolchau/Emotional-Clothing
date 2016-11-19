#ifndef COLOR_UTIL_H
#define COLOR_UTIL_H

/**
 * Calculates the Hue.
 *
 * @param r Red prime.
 * @param g Green prime.
 * @param b Blue prime.
 * @param cMax Highest color value.
 * @param delta Difference between highest and lowest color value.
 * @return The Hue value.
 */
double getHue(double r, double g, double b, double cMax, double delta) {
  double hue;
  if (delta == 0)
    hue = 0;
  else if (cMax == r)
    hue = int((g - b)/delta) % 6;
  else if (cMax == g)
    hue = (b - r)/delta + 2;
  else
    hue = (r - g)/delta + 4;

  hue *= 60;
  if (hue < 0) hue += 360;
  return hue;
}

/**
 * Calculates the Saturation.
 *
 * @param delta Difference between highest and lowest color value.
 * @param light The lightness value of HSL.
 * @return The Saturation value.
 */
double getSaturation(double delta, double light) {
  double saturation;
  if (delta == 0)
    saturation = 0;
  else
    saturation = delta / (1 - abs(2 * light - 1));

  return saturation;
}

/**
 * Calculates the Lightness value.
 *
 * @param cMax Highest color value.
 * @param cMin Lowest color value.
 * @return The Lightness value.
 */
double getLightness(double cMax, double cMin) {
  return (cMax + cMin) / 2;
}

/**
 * Fills in the provided double array with HSL values mapped
 * from the given RGB values.
 *
 * @param hsl The double array to be filled.
 * @param R Red intensity.
 * @param G Green intensity.
 * @param B Blue intensity.
 */
 void getHSL(double hsl[3], int R, int G, int B) {
  double r = R/255.0;
  double g = G/255.0;
  double b = B/255.0;

  double cMax = max(max(r, g), b);
  double cMin = min(min(r, g), b);
  double delta = cMax - cMin;

  double lightness = getLightness(cMax, cMin);
  hsl[0] = getHue(r, g, b, cMax, delta);
  hsl[1] = getSaturation(delta, lightness);
  hsl[2] = lightness;
}

#endif
