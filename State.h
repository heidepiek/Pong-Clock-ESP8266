#ifndef STATE_H
#define STATE_H

#include <Arduino.h>

struct DateTime {
  DateTime(uint16_t yr=2024, uint8_t mo=1, uint8_t d=1,
           uint8_t h=0, uint8_t mi=0, uint8_t s=0)
  : _year(yr), _month(mo), _day(d), _hour(h), _minute(mi), _second(s) {}

  uint16_t year()   const { return _year; }
  uint8_t  month()  const { return _month; }
  uint8_t  day()    const { return _day; }
  uint8_t  hour()   const { return _hour; }
  uint8_t  minute() const { return _minute; }
  uint8_t  second() const { return _second; }

  // weekDay() i.p.v. dayOfWeek() - vermijdt conflict met ezTime macro
  uint8_t weekDay() const {
    static const int t[] = {0,3,2,5,0,3,5,1,4,6,2,4};
    uint16_t y = _year - (_month < 3);
    return (y + y/4 - y/100 + y/400 + t[_month-1] + _day) % 7;
  }

private:
  uint16_t _year;
  uint8_t  _month, _day, _hour, _minute, _second;
};

struct State {
  State();
  void update();
  void save();

  DateTime now;
  bool timeUpdated;
  bool timeMinuteUpdated;

  const __FlashStringHelper* getMonthStr(uint8_t m) const;
  const __FlashStringHelper* getDayStr(uint8_t d) const;

  float    temperature;
  bool     mode24h;
  bool     dim;
  uint8_t  current_face;
  uint8_t  enabled_faces;
  uint16_t tetris_highscore;

private:
  unsigned long timeLastUpdated;
};

extern State state;

#endif
