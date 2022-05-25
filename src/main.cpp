#include <Arduino.h>
#include "sub.h"
#include "SDcardconfig.h"
#include <WiFi.h>
#include <WebServer.h>
#include <WiFiManager.h>

unsigned long lastTime = 0;
unsigned long timerDelay = 1000;

String dataMessage;
unsigned long tambah = 0;

void setup()
{
  Serial.begin(115200);

  WiFiManager WifiManager;
  WifiManager.autoConnect("TMA LAPAN");
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  initSDCard();

  lcd.init();
  lcd.backlight();
  //====================================================
  lcd.createChar(1, customChar); // LOOOK
  //====================================================
  lcd.setCursor(6, 0);
  lcd.print(F("LAPAN"));
  lcd.setCursor(0, 1);
  lcd.print(F("Smkn 2 Pontianak"));
  delay(2000);
  lcd.clear();

#ifndef ESP8266
  while (!Serial)
    ; // wait for serial port to connect. Needed for native USB
#endif

  if (!rtc.begin())
  {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1)
      delay(10);
  }

  if (!rtc.isrunning())
  {
    Serial.println("RTC is NOT running, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  File file = SD.open("/data.txt");
  if (!file)
  {
    Serial.println("File doesn't exist");
    Serial.println("Creating file...");
    writeFile(SD, "/data.txt", "Date, Time, TMA\r\n");
  }
  else
  {
    Serial.println("File already exists");
  }
  file.close();
  ftpSrv.begin("esp32", "esp32");

  lcd.setCursor(0, 0);
  lcd.print(F("IP: "));
  lcd.print(WiFi.localIP());
  lcd.setCursor(0, 1);
  lcd.print(F("FTPname : esp32"));
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("IP: "));
  lcd.print(WiFi.localIP());
  lcd.setCursor(0, 1);
  lcd.print(F("FTPpass : esp32"));
  delay(2000);
}
void loop()
{

  ftpSrv.handleFTP();

  if ((millis() - lastTime) > timerDelay)
  {
    tambah++;
    lcd.clear();
    BacaJarak();
    printDate();
    LCDDate();
    if (tambah == 56)
    {
      dataMessage = String(tgl) + "," + String(jam) + "," + String(distance) + "\r\n";
      Serial.print("Saving data: ");
      Serial.println(dataMessage);

      // Append the data to file
      appendFile(SD, "/data.txt", dataMessage.c_str());
      tambah = 0;
    }
    lastTime = millis();
  }
}
