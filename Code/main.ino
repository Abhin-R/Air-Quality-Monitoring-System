
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#define MQ135_PIN 34
#define BUZZER    25
#define LED1      26
#define LED2      27

int sensorValue;
int ppm;
int aqi;


String getAQILevel(int value)
{
  if (value <= 50)
    return "GOOD";
  else if (value <= 100)
    return "MOD";
  else if (value <= 200)
    return "POOR";
  else if (value <= 300)
    return "V.POOR";
  else
    return "SEVERE";
}

void setup()
{
  Serial.begin(115200);

  lcd.init();
  lcd.backlight();

  pinMode(BUZZER, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

  digitalWrite(BUZZER, HIGH);   // Active LOW buzzer (5V connection)
}

void loop()
{
  // Read MQ135 Analog Output
  sensorValue = analogRead(MQ135_PIN);

  Serial.print("Sensor Value : ");
  Serial.println(sensorValue);

  // Estimate PPM
  ppm = map(sensorValue, 250, 1800, 50, 500);
  ppm = constrain(ppm, 50, 500);

  // Estimate AQI
  aqi = map(ppm, 50, 500, 0, 500);

  // Display values
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("AQI:");
  lcd.print(aqi);
  lcd.print(" ");
  lcd.print(getAQILevel(aqi));

  lcd.setCursor(0, 1);
  lcd.print("PPM:");
  lcd.print(ppm);

  alertSystem(aqi);

  delay(1000);
}

void alertSystem(int value)
{
  if (value <= 50)
  {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, LOW);

    digitalWrite(BUZZER, LOW);
    delay(80);
    digitalWrite(BUZZER, HIGH);
  }

  else if (value <= 100)
  {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);

    for (int i = 0; i < 2; i++)
    {
      digitalWrite(BUZZER, LOW);
      delay(120);
      digitalWrite(BUZZER, HIGH);
      delay(120);
    }
  }

  else if (value <= 200)
  {
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, HIGH);

    for (int i = 0; i < 3; i++)
    {
      digitalWrite(BUZZER, LOW);
      delay(100);
      digitalWrite(BUZZER, HIGH);
      delay(100);
    }
  }

  else
  {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);

    for (int i = 0; i < 5; i++)
    {
      digitalWrite(BUZZER, LOW);
      delay(80);
      digitalWrite(BUZZER, HIGH);
      delay(80);
    }
  }
}
