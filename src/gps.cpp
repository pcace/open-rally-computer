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

// GPS library
TinyGPSPlus gps;

// Serial port for GPS
HardwareSerial SerialGPS(1);

int previousSeconds = 0;
bool timeAlreadySet = false;

void initializeGps()
{
  // Disable Bluetooth to avoid antenna interferences
  btStop();
  Serial.println("Bluetooth stopped to avoid antenna interferences");

  // gps = new TinyGPSPlus();
  SerialGPS.begin(115200, SERIAL_8N1, PIN_GPS_RX, PIN_GPS_TX);
  Serial.println("GPS Serial started");
}


void adjustTimeZone()
{
  // set the Time to the latest GPS reading
  setTime(gps.time.hour(), gps.time.minute(), gps.time.second(), gps.date.day(), gps.date.month(), gps.date.year());
  adjustTime(memory.config.timeZone * SECS_PER_HOUR);
  Serial.println("Time zone adjusted");
}

void updateDistance()
{
  if (state.currentLatitude != state.previousLatitude || state.currentLongitude != state.previousLongitude)
  {
    // Position has changed. Let's calculate the distance between points.
    double distanceMts =
        gps.distanceBetween(
            state.previousLatitude,
            state.previousLongitude,
            state.currentLatitude,
            state.currentLongitude);

    double distanceKms = distanceMts / 1000.0;

    // Update distances
    if (state.previousLatitude != 0)
    { // This fixes a big jump in the first update
      if (state.currentSpeed > 2 && state.gpsPrecision < 500 && state.gpsFix)
      { // This fixes the GPS jitter
        // Update the distance only if I have a decent signal and speed is higher than 5 km/h
        if (memory.config.units == UNITS_METRIC)
        {
          memory.config.tripPartial += distanceKms;
          memory.config.tripTotal += distanceKms;
        }
        else
        {
          memory.config.tripPartial += distanceKms * MILES_PER_KM;
          memory.config.tripTotal += distanceKms * MILES_PER_KM;
        }

        state.isMoving = true;
        state.alreadySaved = false; // Indicate that changes has been made
        state.secsStopped = 0;
        Serial.println("Position changed and distance updated");
      }
      else
      {
        // Not moving or bad signal.
        state.isMoving = false;
        Serial.println("Not moving or bad signal");
      }
    }

    // Update previous data
    state.previousLatitude = state.currentLatitude;
    state.previousLongitude = state.currentLongitude;
  }
}

void pollGpsModule()
{
  while (SerialGPS.available())
  {
    gps.encode(SerialGPS.read());

    //DEBUG GPS RAW Data
    // char c = SerialGPS.read();
    // gps.encode(c);
    // Serial.print(c); // Log the raw GPS data character
  }
}

void updateGpsValues()
{

  if (!timeAlreadySet && state.gpsFix && gps.date.isValid() && gps.time.isValid())
  {
    adjustTimeZone();
    timeAlreadySet = true;
    Serial.println("Time zone adjusted");
  }

  if (gps.location.isValid())
  {
    state.currentLatitude = gps.location.lat();
    state.currentLongitude = gps.location.lng();
    updateDistance();

    Serial.println("GPS location is valid and distance updated");
  }
  else
  {
    state.currentLatitude = 0;
    state.currentLongitude = 0;
    Serial.println("GPS location is not valid");
  }

  if (gps.satellites.isValid())
  {
    state.gpsSatellites = gps.satellites.value();
  }
  else
  {
    state.gpsSatellites = 0;
  }

  if (gps.location.isValid())
  {
    if (memory.config.units == UNITS_METRIC)
    {
      state.currentAltitude = gps.altitude.meters();
    }
    else
    {
      state.currentAltitude = gps.altitude.meters() * FEETS_PER_METER;
    }
  }
  else
  {
    state.currentAltitude = 0;
  }

  state.timeHours = hour();
  state.timeMinutes = minute();
  state.timeSeconds = second();

  state.dateYear = year();
  state.dateMonth = month();
  state.dateDay = day();

  // Count trip time
  if (previousSeconds != state.timeSeconds)
  {
    previousSeconds = state.timeSeconds;
    memory.config.tripTime++;
  }

  if (gps.speed.isValid() && state.gpsPrecision < 500 && state.gpsFix)
  {
    // Set speed only when we have a good signal
    if (memory.config.units == UNITS_METRIC)
    {
      state.currentSpeed = gps.speed.kmph(); // Truncate speed
    }
    else
    {
      state.currentSpeed = gps.speed.kmph() * MILES_PER_KM;
    }

    // Add data to calculate the average
    memory.config.avgAccumulator += state.currentSpeed;
    memory.config.avgSamples++;
    state.average = round(memory.config.avgAccumulator / memory.config.avgSamples);

    // Calculate max speed
    if (state.currentSpeed > memory.config.maxSpeed)
      memory.config.maxSpeed = state.currentSpeed;
  }
  else
  {
    state.currentSpeed = 0;
  }

  if (gps.hdop.isValid())
  {
    state.gpsPrecision = gps.hdop.value();
  }
  else
  {
    state.gpsPrecision = 1000;
  }

  state.gpsDataAge = gps.location.age();

  if (gps.course.isValid())
  {
    state.currentHeading = gps.course.deg();
  }
  else
  {
    state.currentHeading = 0;
  }

  if (gps.location.age() > 3000 || !gps.location.isValid())
  {
    state.gpsFix = false;
  }
  else
  {
    state.gpsFix = true;
  }

  // Update status bar disc position
  if (gps.charsProcessed() > 10)
  {
    if (gps.charsProcessed() > state.gpsCharsProcessed && state.timeSeconds != state.gpsLastSecond)
    {
      state.gpsDiscPos++;
      state.gpsCharsProcessed = gps.charsProcessed();
      state.gpsLastSecond = state.timeSeconds;
    }

    if (state.gpsDiscPos >= 4)
    {
      state.gpsDiscPos = 0;
    }
  }
}
