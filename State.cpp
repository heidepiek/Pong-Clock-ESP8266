/*
 * State.cpp - ezTime editie voor Wemos D1 Mini
 */
#include <Arduino.h>
#include <EEPROM.h>
#include <ezTime.h>
#include "State.h"

extern Timezone myTZ;

#define EEPROM_VERSION 0x50
#define FACE_MAX 4   // Pong, Digital, Pacman, Tetris

State::State()
: now(2024, 1, 1, 0, 0, 0)
, timeLastUpdated(0)
, timeUpdated(false)
, timeMinuteUpdated(false)
, temperature(0)
, mode24h(false)
, dim(false)
, current_face(0)
, enabled_faces(0xFF)
, tetris_highscore(0)
{
  EEPROM.begin(16);
  byte check = EEPROM.read(0);

  if (check == EEPROM_VERSION) {
    // Versie klopt - instellingen laden
    byte flags       = EEPROM.read(1);
    mode24h          = flags & _BV(0);
    dim              = flags & _BV(1);
    current_face     = EEPROM.read(2);
    enabled_faces    = EEPROM.read(3);
    tetris_highscore = EEPROM.read(4) | (EEPROM.read(5) << 8);

    // Veiligheidscheck: current_face mag nooit >= FACE_MAX zijn
    if (current_face >= FACE_MAX) current_face = 0;

    // Veiligheidscheck: alle bits voor bestaande faces moeten gedekt zijn
    // Als een bit 0 is voor een face die er vroeger niet was -> reset naar alles aan
    uint8_t allFacesMask = (1 << FACE_MAX) - 1;
    if ((enabled_faces & allFacesMask) == 0) {
      enabled_faces = 0xFF;
    }
  } else {
    // Nieuwe versie of lege EEPROM: sla defaults op
    save();
  }
}

void State::save() {
  byte flags = 0;
  if (mode24h) flags |= _BV(0);
  if (dim)     flags |= _BV(1);
  EEPROM.write(0, EEPROM_VERSION);
  EEPROM.write(1, flags);
  EEPROM.write(2, current_face);
  EEPROM.write(3, enabled_faces);
  EEPROM.write(4, tetris_highscore & 0xFF);
  EEPROM.write(5, tetris_highscore >> 8);
  EEPROM.commit();
}

void State::update() {
  if (millis() - timeLastUpdated > 1000 || timeLastUpdated == 0) {
    uint8_t prevMinute = now.minute();
    now = DateTime(
      myTZ.year(), myTZ.month(),  myTZ.day(),
      myTZ.hour(), myTZ.minute(), myTZ.second()
    );
    timeLastUpdated   = millis();
    timeMinuteUpdated = (now.minute() != prevMinute);
    timeUpdated       = true;
  } else {
    timeMinuteUpdated = false;
    timeUpdated       = false;
  }
}

const __FlashStringHelper* State::getMonthStr(uint8_t m) const {
  switch (m) {
    case 1:  return F("Jan"); case 2:  return F("Feb");
    case 3:  return F("Mar"); case 4:  return F("Apr");
    case 5:  return F("May"); case 6:  return F("Jun");
    case 7:  return F("Jul"); case 8:  return F("Aug");
    case 9:  return F("Sep"); case 10: return F("Oct");
    case 11: return F("Nov"); default: return F("Dec");
  }
}

const __FlashStringHelper* State::getDayStr(uint8_t d) const {
  switch (d) {
    case 1:  return F("Mon"); case 2: return F("Tue");
    case 3:  return F("Wed"); case 4: return F("Thu");
    case 5:  return F("Fri"); case 6: return F("Sat");
    default: return F("Sun");
  }
}
