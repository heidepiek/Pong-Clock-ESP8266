#include <Arduino.h>
#include <Adafruit_GFX.h>
#include "Menu_Settings_Clockface.h"
#include "Menu_Clockface.h"
#include "State.h"

#ifndef BLACK
#define BLACK 0
#endif
#ifndef WHITE
#define WHITE 1
#endif

SettingsClockfaceMenu::SettingsClockfaceMenu()
: Menu(MENU_SETTINGS_CLOCKFACE)
, selection(0)
{
}

bool SettingsClockfaceMenu::update() {
  return false;
}

void SettingsClockfaceMenu::button1() {
  selection = (selection + 1) % (FACE_MAX + 1);
}

void SettingsClockfaceMenu::button2() {
  if (selection == FACE_MAX) {
    switchMenu(MENU_SETTINGS);
    return;
  }
  state.enabled_faces ^= _BV(selection);
  // Zorg dat altijd minstens 1 face aan staat
  if ((state.enabled_faces & ((1 << FACE_MAX) - 1)) == 0) {
    state.enabled_faces = _BV(selection);
  }
  state.save();
}

void SettingsClockfaceMenu::draw(Adafruit_GFX* display) const {
  // Header
  display->drawRect(0, 0, 128, 64, WHITE);
  display->setTextColor(WHITE);
  display->setTextSize(1);
  display->setCursor(2, 2);
  display->print(F("Clockfaces"));
  display->drawFastHLine(0, 10, 68, WHITE);
  display->drawFastVLine(68, 0, 10, WHITE);

  for (uint8_t i = 0; i <= FACE_MAX; i++) {
    display->setCursor(5, 13 + (i * 10));
    if (selection == i) {
      display->setTextColor(BLACK, WHITE);
      display->fillRect(0, 12 + (i * 10), 128, 10, WHITE);
    } else {
      display->setTextColor(WHITE);
    }

    if (i == FACE_MAX) {
      display->print(F("  Back"));
    } else {
      display->print((state.enabled_faces & _BV(i)) ? '*' : ' ');
      switch (i) {
        case FACE_PONG:    display->print(F(" Pong"));    break;
        case FACE_DIGITAL: display->print(F(" Digital")); break;
        case FACE_PACMAN:  display->print(F(" Pacman"));  break;
        case FACE_TETRIS:  display->print(F(" Tetris"));  break;
      }
    }
    display->setTextColor(WHITE);
  }
}
