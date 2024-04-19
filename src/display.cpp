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


#include <display.h>
#include <screens/splash.h>
#include <screens/odometer.h>
#include <screens/speed.h>
#include <screens/time.h>
#include <screens/heading.h>
#include <screens/settings.h>
#include <screens/input.h>
#include <screens/language.h>
#include <screens/gps.h>
#include <screens/track.h>
#include <screens/update.h>
#include <pins.h>
#include <memory.h>

// Initialize display
U8G2_SSD1309_128X64_NONAME0_F_4W_SW_SPI *u8g2 = new U8G2_SSD1309_128X64_NONAME0_F_4W_SW_SPI(U8G2_R0, /* clock=*/PIN_DISPLAY_SCL, /* data=*/PIN_DISPLAY_SDA, /* cs=*/PIN_DISPLAY_CS, /* dc=*/PIN_DISPLAY_DC, /* reset=*/PIN_DISPLAY_RESET);

// Adjust VCOM deselect level
void setSSD1309VcomDeselect(uint8_t v)
{
  u8g2->sendF("ca", 0x0db, v << 4);
}
// Adjust pre-charge period
void setSSD1309PreChargePeriod(uint8_t p1, uint8_t p2)
{
  u8g2->sendF("ca", 0x0d9, (p2 << 4) | p1);
}
void initializeDisplay()
{
  initDisplayRotation();
  initBacklight();

  u8g2->begin(PIN_BUTTON_SELECT, PIN_BUTTON_PREV, PIN_BUTTON_NEXT);
  u8g2->setContrast(255);

  // Adjust the display settings for initial backlight/brightness
  setBacklight(5); // Example usage, adjust the initial value as needed
}

void drawSplashScreen()
{
  u8g2->clearBuffer();
  drawScreen(SCREEN_SPLASH);
  u8g2->sendBuffer();
}

void drawScreen(int index)
{
  switch (index)
  {
  case SCREEN_SPLASH:
    drawSplashLayout();
    break;
  case SCREEN_ODOMETER:
    drawOdometerLayout();
    break;
  case SCREEN_SPEED:
    drawSpeedLayout();
    break;
  case SCREEN_TIME:
    drawTimeLayout();
    break;
  case SCREEN_HEADING:
    drawHeadingLayout();
    break;
  case SCREEN_MENU:
    drawSettingsLayout();
    break;
  case SCREEN_INPUT:
    drawInputLayout();
    break;
  case SCREEN_LANGUAGE:
    drawLanguageLayout();
    break;
  case SCREEN_GPS:
    drawGpsLayout();
    break;
  case SCREEN_TRACK:
    drawTrackLayout();
    break;
  case SCREEN_UPDATE:
    drawUpdateLayout();
    break;
  default:
    drawOdometerLayout();
    break;
  }
}

void initBacklight()
{
  ledcSetup(0, 10000, 8); // LED channel, frequency, resolution
  ledcAttachPin(PIN_BACKLIGHT, 0);

  setBacklight(memory.config.backlight);
}

void initDisplayRotation()
{
  if (memory.config.flipScreen)
  {
    u8g2->setDisplayRotation(U8G2_R2); // 180 degrees clockwise rotation
  }
  else
  {
    u8g2->setDisplayRotation(U8G2_R0); // No rotation, landscape
  }
}

void setBacklight(int value)
{
  // Map the value from 0-10 to appropriate VCOM deselect and pre-charge period values
  uint8_t vcom = map(value, 0, 10, 0, 7);
  uint8_t preChargeP1 = map(value, 0, 10, 15, 1);
  uint8_t preChargeP2 = map(value, 0, 10, 1, 15);

  Serial.print("VCOM Deselect Level: ");
  Serial.println(vcom);
  Serial.print("Pre-Charge Period P2: ");
  Serial.println(preChargeP2);
  Serial.print("Pre-Charge ChargeP1: ");
  Serial.println(preChargeP1);

  if (preChargeP2 > 15)
  {
    preChargeP2 = 15;
  }

  if (preChargeP1 < 1)
  {
    preChargeP1 = 1;
  }

  setSSD1309VcomDeselect(vcom);
  setSSD1309PreChargePeriod(preChargeP1, preChargeP2);
}
