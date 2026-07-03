#ifndef BUTTONS_H
#define BUTTONS_H

#include <Arduino.h>

struct Button {
  Button(uint8_t pin);
  bool update();  // geeft true terug op het moment van indrukken
  bool read() const { return _pressed; }

  uint8_t  _pin;
  bool     _lastRaw;
  bool     _pressed;
  uint32_t _lastTime;
};

#endif
