#include <Arduino.h>
#include <Adafruit_GFX.h>
#include "Menu_Clockface.h"
#include "Menu.h"
#include "State.h"
#include "Clockface_Pong.h"
#include "Clockface_Digital.h"
#include "Clockface_Pacman.h"
#include "Clockface_Tetris.h"

ClockfaceMenu::ClockfaceMenu()
: Menu(MENU_CLOCK), faceType(0), face(NULL)
{
  faceType = state.current_face;
  if (faceType >= FACE_MAX) faceType = 0;

  // Zorg dat startface ook echt enabled is
  for (uint8_t i = 0; i < FACE_MAX; i++) {
    if (state.enabled_faces & _BV(faceType)) break;
    faceType = (faceType + 1) % FACE_MAX;
  }
  changeMenu();
}

ClockfaceMenu::~ClockfaceMenu() {
  delete face;
}

bool ClockfaceMenu::update() {
  uint8_t hour = state.now.hour();
  if (!state.mode24h && hour > 12) hour -= 12;
  face->update(hour, state.now.minute());
  return true;
}

void ClockfaceMenu::draw(Adafruit_GFX* display) const {
  face->draw(display);
}

void ClockfaceMenu::button1() {
  // Stap door faces, maximaal FACE_MAX pogingen zodat we nooit vastlopen
  for (uint8_t i = 0; i < FACE_MAX; i++) {
    faceType = (faceType + 1) % FACE_MAX;
    if (state.enabled_faces & _BV(faceType)) break;
  }
  changeMenu();
}

void ClockfaceMenu::changeMenu() {
  delete face;
  face = NULL;

  switch (faceType) {
    case FACE_PONG:    face = new ClockfacePong();    break;
    case FACE_DIGITAL: face = new ClockfaceDigital(); break;
    case FACE_PACMAN:  face = new ClockfacePacman();  break;
    case FACE_TETRIS:  face = new ClockfaceTetris();  break;
    default:           face = new ClockfacePong();    break;
  }

  uint8_t hour = state.now.hour();
  if (!state.mode24h && hour > 12) hour -= 12;
  face->begin(hour, state.now.minute());

  state.current_face = faceType;
  // Geen state.save() hier - EEPROM schrijven bij elke wissel
  // triggert de watchdog timer op ESP8266
}

void ClockfaceMenu::button2() {
  switchMenu(MENU_SETTINGS);
}
