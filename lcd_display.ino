#include <LiquidCrystal.h>

#define rs 13
#define en 12
#define d4 14
#define d5 27
#define d6 26
#define d7 25

int tds = 425;
int temp = 77;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
void task3_monitoring(void *param);

void setup() {
  // put your setup code here, to run once:
  //Serial.begin(115200);
  pinMode(LED_BUILTIN , OUTPUT);
  lcd.begin(16,2);

  xTaskCreatePinnedToCore(
    task3_monitoring,   //call task1 function
    "Monitoring the Output results",
    10000,
    NULL,
    1,
    NULL,
    1); //Run on 1 CPU core

}
void task3_monitoring(void *param) {
  while(1) {
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("TDS & TEMP");
    lcd.setCursor(1, 1);
    lcd.print("Smart Aquarium");
    delay(2000);
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("TDS(PPM): ");
    lcd.print(tds);
    delay(2000);
    lcd.setCursor(2, 1);
    lcd.print("Temp(F): ");
    lcd.print(temp);
    delay(2000);

  }
}

void loop() {
}
