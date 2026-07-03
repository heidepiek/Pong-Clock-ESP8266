#ifndef MENU_CLOCKFACE_H
#define MENU_CLOCKFACE_H

#include <Arduino.h>
#include "Menu.h"
#include "Clockface.h"

enum FACE {
  FACE_PONG    = 0,
  FACE_DIGITAL = 1,
  FACE_PACMAN  = 2,
  FACE_TETRIS  = 3,
  FACE_MAX     = 4
};

class ClockfaceMenu : public Menu {
public:
  ClockfaceMenu();
  ~ClockfaceMenu();
  bool update();
  void draw(Adafruit_GFX* display) const;
  void button1();
  void button2();
private:
  void changeMenu();
  Clockface* face;
  mutable uint8_t faceType;
};

#endif
