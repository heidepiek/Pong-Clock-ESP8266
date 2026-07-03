#include <Arduino.h>
#include <Adafruit_GFX.h>
//#include <Adafruit_SSD1306.h>   //oled SSD1306
#include <Adafruit_SH110X.h>    //oled SH1106
#include "Menu_Settings_Brightness.h"
#include "State.h"

// Some graphics constants
#ifndef BLACK
#define BLACK 0
#endif
#define WHITE 1
#define WIDTH 128
#define HEIGHT 64

//extern Adafruit_SSD1306 display;    //oled SSD1306
extern Adafruit_SH1106G display;    //oled SH1106  

enum MenuItems {
  ITEM_BRIGHT,
  ITEM_DIM,
  ITEM_BACK,
  ITEM_MAX
};

SettingsBrightnessMenu::SettingsBrightnessMenu()
: Menu(MENU_SETTINGS_BRIGHTNESS)
, selection(0)
{}

bool SettingsBrightnessMenu::update() {
  return state.timeMinuteUpdated;
}

void SettingsBrightnessMenu::button1() {
  selection = (selection + 1) % ITEM_MAX;
}

void SettingsBrightnessMenu::button2() {
  if (selection == ITEM_BACK) {
    switchMenu(MENU_SETTINGS);
  } else if (selection == ITEM_BRIGHT) {
    //display.dim(false);       //oled SSD1306
    display.setContrast(255);   //oled SH1106
    state.dim = false;
    state.save();
  } else if (selection == ITEM_DIM) {
    //display.dim(true);        //oled SSD1306
    display.setContrast(0);     //oled SH1106  
    state.dim = true;
    state.save();
  }
}

void SettingsBrightnessMenu::draw(Adafruit_GFX* display) const {
  drawLayout(display, F("Brightness"));

  // Menu Items
  for (uint8_t i = 0; i < ITEM_MAX; ++i) {
    display->setCursor(5, 13 + (i * 10));
    if (selection == i) {
      display->setTextColor(BLACK, WHITE);
      display->fillRect(
        0    , 12 + (i * 10),
        WIDTH, 9,
        WHITE);
    } else {
      display->setTextColor(WHITE);
    }
    switch (i) {
    case ITEM_BRIGHT:
      if (!state.dim) display->print(F("* "));
      display->print(F("Bright"));
      break;
    case ITEM_DIM:
      if (state.dim) display->print(F("* "));
      display->print(F("Dim"));
      break;
    case ITEM_BACK:
      display->print(F("Back"));
      break;
    }
  }
}