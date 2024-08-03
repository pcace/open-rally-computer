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

#include <screens/heading.h>
#include <screens/status_bar.h>
#include <Arduino.h>
#include <display.h>
#include <utils.h>
#include <str_index.h>
#include <memory.h>
#include <language.h>

void drawHeadingLayout()
{
  String heading = String(state.currentHeading);

  // Draw status bar
  drawStatusBar();

  // Draw title
  u8g2->setFont(u8g2_font_7x13B_mr);
  u8g2->drawStr(0, 21, get_string(memory.config.language, STR_HEADING));

  // Draw heading
  u8g2->setFont(u8g2_font_logisoso38_tn);
  int offset = 20;
  u8g2->drawStr(0, 63, heading.c_str());
  // u8g2->drawStr(getCenterPosition(heading.c_str()) - offset, 63, heading.c_str());
  int degXpos = u8g2->getStrWidth(heading.c_str()) + 8; // - offset to make some space for the 2d Compas
  // int degXpos = getCenterPosition(heading.c_str()) + u8g2->getStrWidth(heading.c_str()) + 8 - offset; // - offset to make some space for the 2d Compas
  u8g2->drawDisc(degXpos, 28, 3, U8G2_DRAW_ALL); // Degree symbol (the one in the font sucks)
  u8g2->setDrawColor(0);
  u8g2->drawDisc(degXpos, 28, 1, U8G2_DRAW_ALL);
  u8g2->setDrawColor(1);

  // Draw compass
  int centerX = 105;  // X position of compass center
  int centerY = 36;   // Y position of compass center
  int radius = 19;    // Radius of the compass
  int arrowSize = -8; // Size of the arrowhead

  // Draw the outer circle of the compass
  u8g2->drawCircle(centerX, centerY, radius, U8G2_DRAW_ALL);

  // Draw the thick heading line
  float headingAngle = state.currentHeading * PI / 180.0;                   // Convert degrees to radians
  int lineX1 = centerX + (radius + arrowSize) * cos(headingAngle - PI / 2); // Adjust for line thickness
  int lineY1 = centerY + (radius + arrowSize) * sin(headingAngle - PI / 2);
  int lineX2 = centerX + radius * cos(headingAngle - PI / 2); // Endpoint for arrowhead base
  int lineY2 = centerY + radius * sin(headingAngle - PI / 2);
  u8g2->drawLine(centerX, centerY, lineX1, lineY1);
  u8g2->drawLine(centerX + 1, centerY, lineX1 + 1, lineY1); // Make line thicker
  u8g2->drawLine(centerX - 1, centerY, lineX1 - 1, lineY1); // Make line thicker

  // Draw arrowhead
  int arrowX1 = lineX2 + arrowSize * cos(headingAngle - PI / 2 - PI / 6);
  int arrowY1 = lineY2 + arrowSize * sin(headingAngle - PI / 2 - PI / 6);
  int arrowX2 = lineX2 + arrowSize * cos(headingAngle - PI / 2 + PI / 6);
  int arrowY2 = lineY2 + arrowSize * sin(headingAngle - PI / 2 + PI / 6);

  u8g2->drawLine(lineX2, lineY2, arrowX1, arrowY1);   // Left side of arrowhead
  u8g2->drawLine(lineX2, lineY2, arrowX2, arrowY2);   // Right side of arrowhead
  u8g2->drawLine(arrowX1, arrowY1, arrowX2, arrowY2); // Base of arrowhead
}