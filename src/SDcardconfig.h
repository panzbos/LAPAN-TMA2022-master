#include <Arduino.h>
#include <SD.h>
#include <FS.h>
#include "ESP32FtpServer.h"
FtpServer ftpSrv;
// Initialize SD card
void initSDCard()
{
    if (!SD.begin())
    {
        Serial.println("Card Mount Failed");

        return;
    }

    Serial.println("SD opened!");
    uint8_t cardType = SD.cardType();

    if (cardType == CARD_NONE)
    {
        Serial.println("No SD card attached");
        return;
    }
    Serial.print("SD Card Type: ");
    if (cardType == CARD_MMC)
    {
        Serial.println("MMC");
    }
    else if (cardType == CARD_SD)
    {
        Serial.println("SDSC");
    }
    else if (cardType == CARD_SDHC)
    {
        Serial.println("SDHC");
    }
    else
    {
        Serial.println("UNKNOWN");
    }
    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    Serial.printf("SD Card Size: %lluMB\n", cardSize);
}

// Write to the SD card
void writeFile(fs::FS &fs, const char *path, const char *message)
{
    Serial.printf("Writing file: %s\n", path);

    File file = fs.open(path, FILE_WRITE);
    if (!file)
    {
        Serial.println("Failed to open file for writing");
        lcd.setCursor(15, 1);
        lcd.print("X");
        return;
    }
    if (file.print(message))
    {
        Serial.println("File written");
        lcd.setCursor(15, 1);
        lcd.write(1);
    }
    else
    {
        Serial.println("Write failed");
        lcd.setCursor(15, 1);
        lcd.print("X");
    }
    file.close();
}

// Append data to the SD card
void appendFile(fs::FS &fs, const char *path, const char *message)
{
    Serial.printf("Appending to file: %s\n", path);

    lcd.setCursor(13, 1);
    lcd.write(1);
    lcd.write(1);
    lcd.write(1);
    File file = fs.open(path, FILE_APPEND);
    if (!file)
    {
        Serial.println("Failed to open file for appending");
        lcd.setCursor(13, 1);
        lcd.print("XXX");
        return;
    }
    if (file.print(message))
    {
        Serial.println("Message appended");
    }
    else
    {
        Serial.println("Append failed");
    }
    file.close();
}
