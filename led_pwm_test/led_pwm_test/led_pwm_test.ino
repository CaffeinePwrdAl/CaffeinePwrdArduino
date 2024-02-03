/*
 * light_ws2812 example
 *
 * Created: 07.03.2014 12:49:30
 *  Author: Matthias Riegler
 */ 
#include <inttypes.h>
#include <WS2812.h>

WS2812 LED(6); // 19 LED
	
cRGB value;

#if !defined(MIN)
  #define MIN(x, y) (((x)<(y))?(x):(y))
#endif


int serial_putc(char c, FILE *) 
{
  Serial.write(c);
  return c;
}

void SetupSerial()
{
  Serial.begin(115200);
  fdevopen(&serial_putc, 0);
}

void setup() {
  SetupSerial();
  printf("Hello world!\n");


  pinMode(6, OUTPUT);
  digitalWrite(6, HIGH);
  delay(500);
  digitalWrite(6, LOW);
  delay(500);
  digitalWrite(6, HIGH);
  delay(500);

	LED.setOutput(6); // Digital Pin 9

  LED.setColorOrderRGBW();


  // r0 g0 b0
  // g1 w0 r1
  // w1 b1 g2
  // b2 r2 w2  - bit of w2 with b2?
  value = { 32, 48, 64, 20};
  //value = { 0, 0, 255, 0 };
  LED.set_crgb_at(0, value);
  //value = { 255, 0, 0, 0 };
  LED.set_crgb_at(1, value);
  //value = { 0, 255, 0, 0 };
  LED.set_crgb_at(2, value);
  //value = { 1, 0, 0 };
  LED.set_crgb_at(3, value);
  LED.set_crgb_at(4, value);
  LED.set_crgb_at(5, value);
  //LED.set_crgb_at(6, value);
  //LED.set_crgb_at(7, value);
  //LED.set_crgb_at(8, value);
}

void colour_chase() {
  static uint8_t frame = 0;
  cRGB colours[6] = {
    { 255,   0,   0, 0 },
    { 255, 255,   0, 0 },
    {   0, 255,   0, 0 },
    {   0, 255, 255, 0 },
    {   0,   0, 255, 0 },
    { 255,   0, 255, 0 },
  };

  LED.set_crgb_at(0, colours[(frame + 0) % 6]);
  LED.set_crgb_at(1, colours[(frame + 1) % 6]);
  LED.set_crgb_at(2, colours[(frame + 2) % 6]);
  LED.set_crgb_at(3, colours[(frame + 3) % 6]);
  LED.set_crgb_at(4, colours[(frame + 4) % 6]);
  LED.set_crgb_at(5, colours[(frame + 5) % 6]);

  frame = (frame + 1) % 36;

  LED.sync(); // Sends the value to the LED
	delay(150); // Wait 500 ms
}

void random_colours() {
  cRGB colours[8] = {
    { 255,   0,   0,   0 },
    { 255, 255,   0,   0 },
    {   0, 255,   0,   0 },
    {   0, 255, 255,   0 },
    {   0,   0, 255,   0 },
    { 255,   0, 255,   0 },
    {   0,   0,   0, 255 },
    { 255, 255, 255,   0 },
  };

  LED.set_crgb_at(0, colours[rand() % 8]);
  LED.set_crgb_at(1, colours[rand() % 8]);
  LED.set_crgb_at(2, colours[rand() % 8]);
  LED.set_crgb_at(3, colours[rand() % 8]);
  LED.set_crgb_at(4, colours[rand() % 8]);
  LED.set_crgb_at(5, colours[rand() % 8]);

  LED.sync(); // Sends the value to the LED
	delay(150); // Wait 500 ms
}

void ColourFromHSL(const float& h, const float& s, const float& l, uint8_t &outR, uint8_t &outG, uint8_t &outB, uint8_t &outW)
{
  const float C = (1.0f - fabs(2.0f * l - 1.0f)) * s;
  const float X = C * (1.0f - fabs(fmod(h / 60, 2.0f) - 1.0f));
  const float m = l - C / 2.0f;

  float r, g, b;
  if (h < 60)                     r = C, g = X, b = 0;
  else if (h < 120)               r = X, g = C, b = 0;
  else if (h < 180)               r = 0, g = C, b = X;
  else if (h < 240)               r = 0, g = X, b = C;
  else if (h < 300)               r = X, g = 0, b = C;
  else                            r = C, g = 0, b = X;

  r += m; g += m; b += m;

   b *= 0.7f;
   b = sqrt(b);
   g *= 0.5f;

  // float luma = MIN(r, MIN(g, b));
  // r -= luma;
  // g -= luma;
  // b -= luma;

  float w = 0.0f;
  // luma;
  // if (w < 0.0f) w = 0.0f;

  outR = ((uint8_t)(r * 255.0f)) & 0xFF;
  outG = ((uint8_t)(g * 255.0f)) & 0xFF;
  outB = ((uint8_t)(b * 255.0f)) & 0xFF;
  outW = ((uint8_t)(w * 255.0f)) & 0xFF;

  printf("Hue %"PRIu32" - r %"PRIu32" g %"PRIu32" b %"PRIu32"\n", (uint32_t)(r * 255.0f), (uint32_t)(r * 255.0f), (uint32_t)(g * 255.0f), (uint32_t)(b * 255.0f));
}

void hue_wheel()
{
  static float fHue = 0.0f;
  static float fLumaRads = 0.0f;

  cRGB colour { 0, 0, 0, 0 };

  float fLuma = 0.5f;//0.5f + 0.25f * sin(fLumaRads);

  for (uint8_t i = 0; i < 6; i++)
  {
    float fHueLocal = fmod(fHue + 5.0f * i, 360.0f);
    uint8_t r, g, b, w;
    ColourFromHSL(fHueLocal, 1.0f, fLuma, r, g, b, w);

    // Internal order of the structure is not rgb, but params written in this order since we've called setColorOrderRGBW
    colour = { r, g, b, w };

    LED.set_crgb_at(i, colour);
  }
  // LED.set_crgb_at(1, colour);
  // LED.set_crgb_at(2, colour);
  // LED.set_crgb_at(3, colour);
  // LED.set_crgb_at(4, colour);
  // LED.set_crgb_at(5, colour);

  fHue += 0.5f;
  if (fHue >= 360.0f) fHue -= 360.0f;

  fLumaRads += 0.007f;
  if (fLumaRads > 2.0f * PI) fLumaRads -= 2.0f * PI;

  LED.sync(); // Sends the value to the LED
	delay(5); // Wait 500 ms
}

uint8_t lerp8(uint8_t a, uint8_t b, float t)
{
  return (uint8_t)(a + ((b-a) * t));
}

uint8_t lookup(const uint8_t lut[5], uint8_t v)
{
  float fv = (v / 255.0f);
  if (fv < 0.25f) return lerp8(lut[0], lut[1], fv * 4.0f);
  else if (fv < 0.5f) return lerp8(lut[1], lut[2], (fv - 0.25f) * 4.0f);
  else if (fv < 0.75f) return lerp8(lut[2], lut[3], (fv - 0.5f) * 4.0f);
  else return lerp8(lut[3], lut[4], (fv - 0.75f) * 4.0f);
}

void gamma_check()
{
  /*
    255  100
    223  87.5
    210  75
    172  62.5    
    157 50
    100 25
    55 12.5
    31 6.25
    0 0
  */

// uint8_t rlut[] = {0, 31, 55, 103, 157, 172, 210, 223, 255};
// uint8_t glut[] = {0, 31, 55, 103, 157, 172, 210, 223, 255};
// uint8_t blut[] = {0, 31, 55, 103, 157, 172, 210, 223, 255};


  // 2x 50% has a green bias compared to a slight blue bias for 1x 100%

                //  0  25  50  75  100
  uint8_t rlut[] = {0, 60, 125, 168, 255};
  uint8_t glut[] = {0, 60, 128, 168, 255};
  uint8_t blut[] = {0, 65, 129, 168, 245};

  
  const uint8_t nScenarios = 1;
  const uint8_t scenarios[nScenarios][6] = {
    //{ 0, 5, 0, 2, 0, 2 }, // 100 vs. 50s
    //{ 0, 3, 0, 2, 0, 1 }, // 75 vs. 50/25
    //{ 0, 3, 0, 2, 0, 1 }, // 82.5 vs. 75/25
    //{ 0, 2, 0, 1, 0, 1 }, // 50 vs. 25/25
  };

  static uint32_t s = 0;

  uint8_t sn = (s/100) % nScenarios;

  // for (uint8_t l = 0; l < 6; l++)
  // {
  //   uint8_t id = scenarios[sn][l];

  //   if (s & 0x01) id = scenarios[sn][l%3];

  //   cRGB colour { rlut[id], rlut[id], rlut[id], 0 };
  //   LED.set_crgb_at(l, colour);
  // }

  uint8_t v = s % 256;
  cRGB colour { lookup(rlut, v), lookup(glut, v), lookup(blut, v), 0 };
  LED.set_crgb_at(1, colour);

  v = (s%256) / 2;
  colour = { lookup(rlut, v), lookup(glut, v), lookup(blut, v), 0 };
  LED.set_crgb_at(3, colour);
  LED.set_crgb_at(5, colour);

  colour = { 0, 0, 0, 0 };
  LED.set_crgb_at(0, colour);
  LED.set_crgb_at(2, colour);
  LED.set_crgb_at(4, colour);

  s ++;
  LED.sync(); // Sends the value to the LED
	delay(15); // Wait 500 ms
}

void loop() {
  //colour_chase();
  //random_colours();
  //hue_wheel();
  gamma_check();
}