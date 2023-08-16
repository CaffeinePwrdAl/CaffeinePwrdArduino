/*
  trafficlights:

  An implementation of a standard UK traffic light junction with pedestrian cycle

  See g_uLEDPins & enum 'LEDNames' for LED Pin Mapping.

  Add a push button on pin 2 to enable 'dance mode' for a few seconds :)

  gnd        pin2        5v
   |          |     /    |
   |--\/\/\---o----o  o--|
      ~10k
      
*/
//#define DEBUG

// Button pin
const uint8_t PUSH_BUTTON = 2;

// List of LED's in use
enum LEDNames
{
  TRAFFIC_R = 0,
  TRAFFIC_A,
  TRAFFIC_G,
  PEEPS_R,
  PEEPS_G,
  NUM_LEDS,
};

// Mapping of LED Name to Arduino Pins
const uint8_t g_uLEDPins[NUM_LEDS] { 5, 6, 9, 10, 11 };

// Each of the states in the statemachine
enum TrafficState
{
  CAR_GO = 0,
  CAR_STOPPING,
  CAR_STOP,
  PEEPS_GO,
  PEEPS_GO_FASTER,
  QUICK_OLD_PEOPLE_RUN,
  NUM_NORMAL_MODES,
  
  DANCE_DANCE = NUM_NORMAL_MODES,
  NUM_STATES,
};

// Data for which led's to light for each state
uint8_t g_uFlashRate = 300 /*ms*/;
const uint8_t  LONG_INTERVAL = 21 /*flashes*/;
const uint8_t  MED_INTERVAL  = 11 /*flashes*/;
const uint8_t  SHORT_INTERVAL = 3 /*flashes*/;

struct StateInfo
{
  uint8_t interval;  // number of 'flashrate' 'ticks' until transition 
  uint8_t steady;    // bitfield of steadily lit LED's
  uint8_t flashslow;     // bitfield of flashing LED's
  uint8_t flashfast;     // bitfield of flashing LED's
};

#define LED_BIT(x) (1<<x)
const StateInfo g_auStateData[NUM_STATES] = {
  //  INTERVAL           STEADY LEDs                            FLASHING LEDs (slow, fast)
  {   LONG_INTERVAL,     LED_BIT(TRAFFIC_G) | LED_BIT(PEEPS_R), 0, 0 },
  {   MED_INTERVAL,      LED_BIT(TRAFFIC_A) | LED_BIT(PEEPS_R), 0, 0 },
  {   SHORT_INTERVAL,    LED_BIT(TRAFFIC_R) | LED_BIT(PEEPS_R), 0, 0 },
  {   LONG_INTERVAL,     LED_BIT(TRAFFIC_R) | LED_BIT(PEEPS_G), 0, 0 },
  // For these flashing ones, if you want the traffic and pedestrian led's to alternate, 
  // set both to flash, but also set one as steady as well - each flash just 'reverses' the
  // last state so if they start the same they'll flash together, if they start different
  // then they alternate.
  // Flash ped-green for hurry up
  {   
      MED_INTERVAL,    
      LED_BIT(TRAFFIC_R) | LED_BIT(PEEPS_G),
      LED_BIT(PEEPS_G),
      0
  },
  // Faster flashing ped-green, cars can go if clear
  {   
      MED_INTERVAL,    
      LED_BIT(PEEPS_G),
      LED_BIT(TRAFFIC_A),
      LED_BIT(PEEPS_G)   
  },
  // Dance!
  {   
      LONG_INTERVAL * 4,    
      LED_BIT(PEEPS_G) | LED_BIT(TRAFFIC_G) | LED_BIT(TRAFFIC_R) | LED_BIT(TRAFFIC_A) | LED_BIT(PEEPS_R),
      LED_BIT(TRAFFIC_A) | LED_BIT(TRAFFIC_R) | LED_BIT(PEEPS_R) | LED_BIT(TRAFFIC_G),
      LED_BIT(PEEPS_G) | LED_BIT(TRAFFIC_R) | LED_BIT(TRAFFIC_G)
  },
};

uint8_t g_eCurrentState = PEEPS_GO;
const StateInfo* g_psCurrentParams = &g_auStateData[g_eCurrentState];
uint8_t updateTicksTillChange = g_psCurrentParams->interval;
uint8_t currentLedBits = g_psCurrentParams->steady;

#if defined(DEBUG)
#define ENUM_TO_STRING(x) case x: return #x;
const char* GetStateName(uint8_t state)
{
  switch (state)
  {
    ENUM_TO_STRING(CAR_GO)
    ENUM_TO_STRING(CAR_STOPPING)
    ENUM_TO_STRING(CAR_STOP)
    ENUM_TO_STRING(PEEPS_GO)
    ENUM_TO_STRING(PEEPS_GO_FASTER)
    ENUM_TO_STRING(QUICK_OLD_PEOPLE_RUN)
    default: return "UNKNOWN";
  };
}
#endif

void SetStateTo(uint8_t state)
{
    g_eCurrentState = state;
    g_psCurrentParams = &g_auStateData[g_eCurrentState];
    currentLedBits = g_psCurrentParams->steady;
    updateTicksTillChange = g_psCurrentParams->interval;

#if defined(DEBUG)
    Serial.println("]");
    Serial.print(GetStateName(g_eCurrentState));
    Serial.print(" - Ticks = ");
    Serial.println( g_psCurrentParams->interval);
    Serial.print("[ ");
#endif
}

void UpdateState()
{
  if (digitalRead(PUSH_BUTTON))
  {
    SetStateTo(DANCE_DANCE);
    g_uFlashRate = 50;
  }
  
  // Cycle through the traffic light states if interval timer has elapsed
  if (updateTicksTillChange == 0)
  {
    SetStateTo((g_eCurrentState + 1) % NUM_NORMAL_MODES);
    g_uFlashRate = 100;
  }
  // Or update the current led bits for the current state
  else
  {
    currentLedBits ^= g_psCurrentParams->flashfast;
    if (updateTicksTillChange & 0x1)
    {
      currentLedBits ^= g_psCurrentParams->flashslow;
    }
    updateTicksTillChange--;

#if defined(DEBUG)
    Serial.print(updateTicksTillChange);
    Serial.print(" ");
#endif
  }

  // Set the led states from the bitmask
  for (uint8_t led = 0; led < NUM_LEDS; led++)
  {
    const uint8_t ledState = (currentLedBits & LED_BIT(led)) != 0;
    digitalWrite(g_uLEDPins[led], (ledState == 1) ? (HIGH) : (LOW));
  }

  delay(g_uFlashRate);
}

// the setup function runs once when you press reset or power the board
void setup()
{
#if defined(DEBUG)
  // Debug
  Serial.begin(9600);
#endif

  pinMode(PUSH_BUTTON, INPUT);
  
  // initialize digital pin LED_BUILTIN as an output.
  for (auto a : g_uLEDPins)
  {
    pinMode(a, OUTPUT);
    digitalWrite(a, HIGH);
    delay(150);
    digitalWrite(a, LOW);
  }
}

// the loop function runs over and over again forever
void loop()
{
  UpdateState();
}
