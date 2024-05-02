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

#include <Arduino.h>
#include <pins.h>
#include <display.h>
#include <gps.h>
#include <buttons.h>
#include <thermistor.h>
#include <memory.h>
#include <saveTrackToSD.h>
#include <webserver.h>
#include <ElegantOTA.h>


unsigned long previousMillis1s = 0;
unsigned long previousMillisSaveInterval = 0;
unsigned long currentMillis = 0;

void setup(void)
{
  // Serial port for debugging
  Serial.begin(115200);

  // Initialize GPS module
  initializeGps();

  // Try to load config
  loadConfig(); // Config is loaded before drawing the splash screen to check if flipped 180º

  // Initialize graphic library
  initializeDisplay();

  // Draw splash screen
  drawSplashScreen();

  initButtons();

  initializeSDCard();

  initializeWifi();

  initializeWebserver();

  delay(2000);
  state.currentScreen = SCREEN_ODOMETER;
}

void loop(void)
{
  ElegantOTA.loop();

  u8g2->clearBuffer();

  drawScreen(state.currentScreen);
  u8g2->sendBuffer();

  checkButtons();
  pollGpsModule();

  // Tasks that occur every 1 second
  currentMillis = millis();
  if (currentMillis - previousMillis1s > 1000)
  {
    previousMillis1s = currentMillis;
    updateGpsValues();
    // updateTemperature();
    // Serial.print(F("GPS: "));
    // Serial.println(state.gpsSatellites);

    saveConfig(); // Save all to FRAM / FLASH
  }

  // Tasks that occur every memory.saveInterval seconds
  if (currentMillis - previousMillisSaveInterval > memory.config.saveInterval * 1000)
  {
    previousMillisSaveInterval = currentMillis;
    // save gps Data to SD Card
    saveTrackToSD();
    saveTrackToGPX();
  }
}
