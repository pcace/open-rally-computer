// Open Rally Computer
// Copyright (C) 2021  Matías Godoy

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#include <gps.h>
#include <TinyGPS++.h>
#include <TimeLib.h>
#include <pins.h>
#include <languages.h>
#include <units.h>
#include <constants.h>
#include <memory.h>
#include <SD.h>
#include <SPI.h>

// Initialize SPI2 for SD card
SPIClass SPI2(VSPI);
void listDir(fs::FS &fs, const char *dirname, uint8_t levels)
{
  Serial.printf("Listing directory: %s\n", dirname);

  File root = fs.open(dirname);
  if (!root)
  {
    Serial.println("Failed to open directory");
    return;
  }
  if (!root.isDirectory())
  {
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file)
  {
    if (file.isDirectory())
    {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if (levels)
      {
        listDir(fs, file.name(), levels - 1);
      }
    }
    else
    {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());
      // Optionally print the file content
      Serial.println("  CONTENT:");
      while (file.available())
      {
        Serial.write(file.read());
      }
      Serial.println(); // Ensure there's a newline after file content
    }
    file = root.openNextFile();
  }
}

void initializeSDCard()
{
  // Initialize SPI2 with custom pins
  SPI2.begin(SD_CLK_PIN, SD_MISO_PIN, SD_MOSI_PIN, SD_CS_PIN);

  // Initialize SD card with SPI2 settings
  if (!SD.begin(SD_CS_PIN, SPI2))
  {
    Serial.println("Card Mount Failed");
    return;
  }
  Serial.println("Card Mount Successful");

  // List all files and their content after successful mount
  listDir(SD, "/", 0); // Set levels to 0 if you don't want to list files in subdirectories recursively
}

void saveTrackToSD()
{
  bool gpsFixExists = state.gpsFix == true;
  bool dateIsValid = state.dateYear != 0 && state.dateMonth != 0 && state.dateDay != 0;
  bool timeIsValid = state.timeHours != 0 && state.timeMinutes != 0 && state.timeSeconds != 0;

  if (gpsFixExists && dateIsValid && timeIsValid)
  {
    if (state.currentTrackFile == "")
    {
      String filename = "/" + String(state.dateYear) + String(state.dateMonth) + String(state.dateDay) + "_" + String(state.timeHours) + String(state.timeMinutes) + String(state.timeSeconds) + ".csv";
      state.currentTrackFile = filename.c_str();
      File dataFile = SD.open(state.currentTrackFile.c_str(), FILE_WRITE);
      dataFile.print("lat");
        dataFile.print(";");
        dataFile.print("lon");
        dataFile.print(";");
        dataFile.print("alt");
        dataFile.print(";");
        dataFile.println("speed");
    }

    File dataFile = SD.open(state.currentTrackFile.c_str(), FILE_WRITE);
    if (dataFile)
    {
      for (int i = 0; i < state.currentTrackDataIndex; i++)
      {
        dataFile.print(state.currentTrackData[i].lat, 6);
        dataFile.print(";");
        dataFile.print(state.currentTrackData[i].lon, 6);
        dataFile.print(";");
        dataFile.print(state.currentTrackData[i].alt, 2);
        dataFile.print(";");
        dataFile.println(state.currentTrackData[i].speed, 2);
      }
      dataFile.close();
    }
  }
}