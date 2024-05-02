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

int intPosition = 0;

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
      // Serial.println("  CONTENT:");
      // while (file.available())
      // {
      //   Serial.write(file.read());
      // }
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
      char dateYear[5], dateMonth[3], dateDay[3], timeHours[3], timeMinutes[3], timeSeconds[3];
      sprintf(dateYear, "%04d", state.dateYear);
      sprintf(dateMonth, "%02d", state.dateMonth);
      sprintf(dateDay, "%02d", state.dateDay);
      sprintf(timeHours, "%02d", state.timeHours);
      sprintf(timeMinutes, "%02d", state.timeMinutes);
      sprintf(timeSeconds, "%02d", state.timeSeconds);

      String filename = "/" + String(dateYear) + String(dateMonth) + String(dateDay) + "_" + String(timeHours) + String(timeMinutes) + String(timeSeconds) + ".csv";
      state.currentTrackFile = filename.c_str();
      File dataFile = SD.open(state.currentTrackFile.c_str(), FILE_WRITE);
      dataFile.print("date");
      dataFile.print(";");
      dataFile.print("time");
      dataFile.print(";");
      dataFile.print("lat");
      dataFile.print(";");
      dataFile.print("lon");
      dataFile.print(";");
      dataFile.print("alt");
      dataFile.print(";");
      dataFile.println("speed");
      dataFile.close();
    }
    File dataFile = SD.open(state.currentTrackFile.c_str(), FILE_APPEND);
    if (dataFile)
    {
      dataFile.print(String(state.dateYear) + "-" + String(state.dateMonth) + "-" + String(state.dateDay));
      dataFile.print(";");
      dataFile.print(String(state.timeHours) + ":" + String(state.timeMinutes) + ":" + String(state.timeSeconds));
      dataFile.print(";");
      dataFile.print(state.currentLatitude, 6);
      dataFile.print(";");
      dataFile.print(state.currentLongitude, 6);
      dataFile.print(";");
      dataFile.print(state.currentAltitude, 2);
      dataFile.print(";");
      dataFile.println(state.currentSpeed, 2);
    }
    dataFile.close();
  }
}

void saveTrackToGPX()
{
  bool gpsFixExists = state.gpsFix == true;
  bool dateIsValid = state.dateYear != 0 && state.dateMonth != 0 && state.dateDay != 0;
  bool timeIsValid = state.timeHours != 0 && state.timeMinutes != 0 && state.timeSeconds != 0;

  if (gpsFixExists && dateIsValid && timeIsValid)
  {
    if (state.currentGPXFile == "")
    {
      char dateYear[5], dateMonth[3], dateDay[3], timeHours[3], timeMinutes[3], timeSeconds[3];
      sprintf(dateYear, "%04d", state.dateYear);
      sprintf(dateMonth, "%02d", state.dateMonth);
      sprintf(dateDay, "%02d", state.dateDay);
      sprintf(timeHours, "%02d", state.timeHours);
      sprintf(timeMinutes, "%02d", state.timeMinutes);
      sprintf(timeSeconds, "%02d", state.timeSeconds);
      String filename = "/" + String(dateYear) + String(dateMonth) + String(dateDay) + "_" + String(timeHours) + String(timeMinutes) + String(timeSeconds) + ".gpx";
      state.currentGPXFile = filename.c_str();
      File gpxFile = SD.open(state.currentGPXFile.c_str(), FILE_WRITE);
      // Write GPX header and metadata
      gpxFile.println("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>");
      gpxFile.println("<gpx version=\"1.1\" creator=\"Open Rally Computer - https://github.com/pcace/open-rally-computer\">");
      gpxFile.println("<metadata>");
      gpxFile.println("<name>" + filename + "</name>");
      gpxFile.println("<desc>Track log generated by Open Rally Computer</desc>");
      gpxFile.println("</metadata>");
      gpxFile.println("<trk><name>Track Log</name><trkseg>");
      intPosition = gpxFile.position();
      gpxFile.println("</trkseg></trk></gpx>");
      gpxFile.close();
    }

    // Open the file to append data
    File gpxFile = SD.open(state.currentGPXFile.c_str(), FILE_WRITE);
    if (gpxFile)
    {
      // Seek to the saved position to overwrite the closing tags
      gpxFile.seek(intPosition);
      // Write the new track point
      gpxFile.println("<trkpt lat=\"" + String(state.currentLatitude, 6) + "\" lon=\"" + String(state.currentLongitude, 6) + "\">");
      gpxFile.println("<ele>" + String(state.currentAltitude, 2) + "</ele>");
      gpxFile.println("<time>" + String(state.dateYear) + "-" + String(state.dateMonth) + "-" + String(state.dateDay) + "T" + String(state.timeHours) + ":" + String(state.timeMinutes) + ":" + String(state.timeSeconds) + "Z</time>");
      gpxFile.println("</trkpt>");
      intPosition = gpxFile.position();
      // Re-append the closing tags
      gpxFile.println("</trkseg></trk></gpx>");
      gpxFile.close();
    }
  }
}