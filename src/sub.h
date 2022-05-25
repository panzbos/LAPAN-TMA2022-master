#include <Arduino.h>
#include "RTClib.h"
#include <Wire.h>
#include <SPI.h>
#include <LiquidCrystal_I2C.h>

long duration;
uint8_t distance;
String tgl, jam;
LiquidCrystal_I2C lcd(0x27, 20, 4);
RTC_DS1307 rtc;
#define trigPin 15
#define echoPin 4
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
byte customChar[] = {
    B00000,
    B00000,
    B00000,
    B00001,
    B00011,
    B10110,
    B11100,
    B01000};

void BacaJarak()
{
    digitalWrite(trigPin, LOW);
    delayMicroseconds(5);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = duration * 0.034 / 2;
}
void printDate()
{
    DateTime now = rtc.now();
    tgl = String(now.day()) + "/" + String(now.month()) + "/" + String(now.year());
    jam = String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());
    Serial.print(tgl);
    Serial.print('\t');
    Serial.println(jam);
}
void LCDDate()
{
    DateTime now = rtc.now();
    lcd.setCursor(0, 0);
    lcd.print(now.hour());
    lcd.print(':');
    if (now.minute() < 10)
    {
        lcd.print("0");
        lcd.print(now.minute());
    }
    else
    {
        lcd.print(now.minute());
    }
    lcd.setCursor(8, 0);
    lcd.print(now.day());
    lcd.print('/');
    if (now.month() < 10)
    {
        lcd.print("0");
        lcd.print(now.month());
    }
    else
    {
        lcd.print(now.month());
    }
    lcd.print('/');
    lcd.print(now.year());
    lcd.setCursor(0, 1);
    lcd.print(F("jarak: "));
    lcd.setCursor(7, 1);
    lcd.print(distance);
    lcd.setCursor(10, 1);
    lcd.print(F("cm"));
}
