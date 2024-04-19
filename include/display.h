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


#pragma once

#include <U8g2lib.h>

// Display library
extern U8G2_SSD1309_128X64_NONAME0_F_4W_SW_SPI *u8g2; //note use of pointer, extern to prevent multiple definitions error


// Screens
#define SCREEN_SPLASH         0
#define SCREEN_ODOMETER       1
#define SCREEN_MENU           2
#define SCREEN_INPUT          3
#define SCREEN_LANGUAGE       4
#define SCREEN_GPS            5
#define SCREEN_TRACK          6
#define SCREEN_SPEED          7
#define SCREEN_TIME           8
#define SCREEN_HEADING        9
#define SCREEN_UPDATE         10

#define BACKLIGHT_MIN         0
#define BACKLIGHT_MAX         10
#define DISPLAY_HEIGHT        64
#define DISPLAY_WIDTH         128

void initializeDisplay();
void drawSplashScreen();
void drawScreen(int index);
void initBacklight();
void initDisplayRotation();
void setBacklight(int value);