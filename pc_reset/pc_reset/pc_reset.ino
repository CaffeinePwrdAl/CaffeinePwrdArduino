#define RESET_MOMENTARY (1000)/*ms*/
#define ACPI_POWER_HOLD (20000)/*ms*/
#define DOWNTIME (5000) /*ms*/

#define RESET_PIN (2)
#define POWER_PIN (3)

void setup() {
#if defined(DEBUG)
  // Debug
  Serial.begin(9600);
#endif

  pinMode(RESET_PIN, OUTPUT);
  pinMode(POWER_PIN, OUTPUT);
}

void power_cycle_pc()
{
  digitalWrite(POWER_PIN, HIGH);
  delay(ACPI_POWER_HOLD);
  digitalWrite(POWER_PIN, LOW);
  delay(DOWNTIME);
  digitalWrite(POWER_PIN, HIGH);
  delay(RESET_MOMENTARY);
  digitalWrite(POWER_PIN, LOW);
}

void reset_pc()
{
  digitalWrite(RESET_PIN, HIGH);
  delay(RESET_MOMENTARY);
  digitalWrite(RESET_PIN, LOW);
}

void update_state()
{
  reset_pc();
  delay(1000);
  power_cycle_pc();
  
  delay(DOWNTIME);
}

void loop() {
  update_state();
}
