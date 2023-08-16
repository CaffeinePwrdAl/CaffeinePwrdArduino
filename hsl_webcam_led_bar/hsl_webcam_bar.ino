/*
	HSL Colour Controlled LED Webcam Bar

	Little project constructing a lighting bar consisting of a handful of white, red,
	green, and blue LEDs.

	Current bar consists of [W R W G W B W] LEDs routed to pwm capable pins on an
	Arduino nano.

	Each LED's output level can be weighted to try and normalise the perceived intensity,
	then the RGB components can be used to alter the hue/temperature of the lighting.

	Will possibly look at printing a little enclosure so the light bar can be fixed onto
	the top of the monitor bezel, and might create a pair that connect via ribbon cables
	to the arduino to increase overall brightness.

	Also runs a little start up sequence that runs through each LED
*/
enum LEDS {
  LED_W1 = 0,
  LED_RED,
  LED_W2,
  LED_GREEN,
  LED_W3,
  LED_BLUE,
  LED_W4,
  NUM_LEDS,
};

struct Pin {
  uint8_t pin;
  bool bPWM;
  uint8_t reduction;
};

const Pin g_asLed[NUM_LEDS] {
  { 12, 0, 0   }, // W1
  { 11, 1, 255 }, // R
  { 6,  1, 128 }, // W2
  { 10, 1, 80  }, // G
  { 3,  1, 128 }, // W3
  { 9,  1, 40  }, // B
  { 2,  0, 0   }, // W4
};

#define MIN(x, y) (((x)>(y))?(y):(x))
void SetPWMLED(uint8_t level, LEDS eLED)
{
  float fL = level / 255.0f;
  level = (uint8_t)((fL * fL) * 255.0f);
  uint32_t l = level;
  l *= g_asLed[eLED].reduction;
  l /= 255;

  printf("Level %3d - LED %1d\n", l, eLED);
  analogWrite(g_asLed[eLED].pin, l);
}

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

  for (uint8_t i = 0; i < NUM_LEDS; i++)
  {
    uint8_t pin = g_asLed[i].pin;
    pinMode(pin, OUTPUT);
  }
  for (uint8_t i = 0; i < NUM_LEDS; i++)
  {
    uint8_t pin = g_asLed[i].pin;
    if (g_asLed[i].bPWM)
    {
      for (uint8_t j = 0; j < 255; j+=5)
      {
        SetPWMLED(j, i);
        delay(10);
      }
    }
  }
  //delay(500);
  for (uint8_t i = 0; i < NUM_LEDS; i++)
  {
    uint8_t pin = g_asLed[i].pin;
    if (!g_asLed[i].bPWM)
    {
      digitalWrite(pin, 1);
      //delay(500);
    }
  }
}

uint32_t ColourFromHSL(const float& h, const float& s, const float& l, uint8_t &outR, uint8_t &outG, uint8_t &outB)
{
  const float C = (1.0f - fabs(2.0f * l - 1.0f)) * s;
  const float X = C * (1.0f - fabs(fmod(h / 60, 2.0f) - 1.0f));
  const float m = l - C / 2.0f;

  float r, g, b;
  if (h < 60)                             r = C, b = X, g = 0;
  else if (h < 120)               r = X, b = C, g = 0;
  else if (h < 180)               r = 0, b = C, g = X;
  else if (h < 240)               r = 0, b = X, g = C;
  else if (h < 300)               r = X, b = 0, g = C;
  else                                    r = C, b = 0, g = X;

  r += m; g += m; b += m;

  outR = ((uint8_t)(r * 255.0f)) & 0xFF;
  outG = ((uint8_t)(g * 255.0f)) & 0xFF;
  outB = ((uint8_t)(b * 255.0f)) & 0xFF;
}

void loop() {
  //static uint8_t flip = 0;
  static float t = 300.0f;
  uint8_t r, g, b;

  ColourFromHSL(t, 1.0f, 0.5f, r, g, b);
  //printf("RGB = %3d %3d %3d\n", r, g, b);

  SetPWMLED(r, LED_RED);
  SetPWMLED(g, LED_GREEN);
  SetPWMLED(b, LED_BLUE);

  //flip = (flip + 1) % 5;
  //digitalWrite(g_asLed[LED_W1].pin, flip==0);
  //digitalWrite(g_asLed[LED_W2].pin, (g&0x12) != 0);
  //digitalWrite(g_asLed[LED_W3].pin, (r&0x60) != 0);
  //digitalWrite(g_asLed[LED_W4].pin, flip==0);

  //t += 1.0;
  //if (t >= 360.0f) t = 0.0f;
  //delay(10);
}
