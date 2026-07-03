#include <Arduino.h>
#include "Buttons.h"

#define DEBOUNCE_MS 80

Button::Button(uint8_t pin)
: _pin(pin), _lastRaw(HIGH), _pressed(false), _lastTime(0)
{
  pinMode(_pin, INPUT_PULLUP);
}

bool Button::update() {
  bool raw = digitalRead(_pin);
  uint32_t now = millis();

  if (raw == _lastRaw || (now - _lastTime) < DEBOUNCE_MS) {
    _lastRaw = raw;
    return false;
  }

  _lastTime = now;
  _lastRaw  = raw;

  if (raw == LOW) {
    _pressed = true;
    return true;
  }

  _pressed = false;
  return false;
}
