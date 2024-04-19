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

void saveTrackToSD()
{
  bool gpsFixExists = state.gpsFix == true;
  bool dateIsValid = state.dateYear != 0 && state.dateMonth != 0 && state.dateDay != 0;
  bool timeIsValid = state.timeHours != 0 && state.timeMinutes != 0 && state.timeSeconds != 0;

  if (gpsFixExists && dateIsValid && timeIsValid)
  {
    // Push location data into state.currentTrackData
    LocationData data = {state.currentLatitude, state.currentLongitude, state.currentAltitude, state.currentSpeed};
    state.currentTrackData[state.currentTrackDataIndex++] = data;

    // If there are more then memory.saveInterval elements in state.currentTrackData, write them to the SD card
    // to save some read / writes...
    bool shouldSaveToSD = state.currentTrackDataIndex >= memory.config.saveInterval;
    if (shouldSaveToSD)
    {
      if (state.currentTrackFile == "")
      {
        String filename = String(state.dateYear) + String(state.dateMonth) + String(state.dateDay) + "_" + String(state.timeHours) + String(state.timeMinutes) + String(state.timeSeconds) + ".csv";
        state.currentTrackFile = filename.c_str();
      }

      File dataFile = SD.open(state.currentTrackFile.c_str(), FILE_WRITE);
      if (dataFile)
      {
        for (int i = 0; i < state.currentTrackDataIndex; i++)
        {
          dataFile.print(state.currentTrackData[i].lat, 6);
          dataFile.print(",");
          dataFile.print(state.currentTrackData[i].lon, 6);
          dataFile.print(",");
          dataFile.print(state.currentTrackData[i].alt, 2);
          dataFile.print(",");
          dataFile.println(state.currentTrackData[i].speed, 2);
        }
        dataFile.close();
      }

      // Reset state.currentTrackData
      state.currentTrackDataIndex = 0;
    }
  }
}