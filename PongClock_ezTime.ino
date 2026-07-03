/*
 * Pong Clock - ezTime editie voor Wemos D1 Mini
 * Originele code door 0miker0 & mic159 (github.com/mike-rankin/Pong-Clock)
 * Wunderground vervangen door ezTime - geen API key nodig
 *
 * Bedrading OLED (SSD1306 128x64 I2C):
 *   VCC -> 3.3V  |  GND -> GND  |  SDA -> D2  |  SCL -> D1
 *
 * Bedrading knoppen (andere kant naar GND):
 *   NEXT   (wissel face/omhoog) -> D5
 *   SELECT (menu/bevestig)      -> D6
 *
 * Arduino IDE: Board = LOLIN(WEMOS) D1 mini  |  Upload via USB
 *
 * ============================================================
 *  INSTELLINGEN - vul hier jouw gegevens in
 * ============================================================
 */
#define WIFI_SSID     ""
#define WIFI_PASSWORD ""
#define TIMEZONE      "Europe/Amsterdam"
// ============================================================

#include <ESP8266WiFi.h>
#include <ezTime.h>
#include <Wire.h>
#include <EEPROM.h>
#include <Adafruit_GFX.h>
//#include <Adafruit_SSD1306.h> //oled SSD1306
#include <Adafruit_SH110X.h>  //oled SH1106

#include "Buttons.h"
#include "Menu.h"
#include "Menu_Settings.h"
#include "Menu_Settings_24.h"
#include "Menu_Settings_Time.h"
#include "Menu_Settings_Date.h"
#include "Menu_Settings_Brightness.h"
#include "Menu_Settings_Clockface.h"
#include "Menu_Clockface.h"
#include "State.h"
#include <WiFiManager.h>    

//#define WHITE SSD1306_WHITE   //oled SSD1306
//#define BLACK SSD1306_BLACK   //oled SSD1306
#define WHITE SH110X_WHITE      //oled SH1106
#define BLACK SH110X_BLACK      //oled SH1106


#define NEXT_PIN   D5
#define SELECT_PIN D6

//Adafruit_SSD1306 display(128, 64, &Wire, -1);  //oled SSD1306
Adafruit_SH1106G display(128, 64, &Wire, -1);                     //oled SH1106
Button btnNext(NEXT_PIN);
Button btnSelect(SELECT_PIN);

Timezone myTZ;
State state;
Menu* menu = NULL;

void showSplash(const char* line1, const char* line2 = "") {
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 20);
  display.println(line1);
  display.println(line2);
  display.display();
}

void setup() {
  Serial.begin(115200);
  delay(100);

  Wire.begin();  // SDA=D2, SCL=D1 op D1 Mini

  //if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  //oled SSD1306
  if (!display.begin(0x3C, true)) {                   //oled SH1106
    // OLED niet gevonden: knippper LED
    pinMode(LED_BUILTIN, OUTPUT);
    while (true) {
      digitalWrite(LED_BUILTIN, LOW);  delay(200);
      digitalWrite(LED_BUILTIN, HIGH); delay(200);
    }
  }

  display.setRotation(2);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(5, 20);
  display.print(F("Pong Clock"));
  display.setTextSize(1);
  display.setCursor(66, 48);
  display.print(F("by 0miker0"));
  display.setCursor(78, 56);
  display.print(F("& mic159"));
  display.display();
  delay(1500);


//  oude manier om wifi verbinding te maken met vast ssid en wachtwoord
/*
  // WiFi verbinden
  showSplash("Verbinden...", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  uint8_t tries = 0;
  while (WiFi.status() != WL_CONNECTED && tries < 40) {
    delay(500);
    tries++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    myTZ.setLocation(F(TIMEZONE));
    showSplash("NTP sync...", "");
    waitForSync(10);
    char buf[10];
    snprintf(buf, 10, "%02d:%02d:%02d", myTZ.hour(), myTZ.minute(), myTZ.second());
    showSplash("Tijd OK!", buf);
    delay(800);
  } else {
    showSplash("Geen WiFi!", "Handmatig inst.");
    delay(2000);
  }
*/   

//  Nieuwe manier om met wifimanager verbinding te maken

// WiFi verbinden via WiFiManager
// Eerste keer: maakt access point "PongClock" aan
// Daarna: onthoudt de gegevens automatisch
WiFiManager wifiManager;
showSplash("WiFi...", "");

// Optioneel: reset WiFi-instellingen bij opstarten (voor testen)
// wifiManager.resetSettings();

wifiManager.setAPCallback([](WiFiManager* wm) {
  // Wordt aangeroepen als het access point gestart wordt
  showSplash("Geen WiFi!", "Verbind met:");
  display.setCursor(0, 40);
  display.print(F("PongClock"));
  display.display();
});

wifiManager.setConnectTimeout(30);  // 30 seconden proberen

if (wifiManager.autoConnect("PongClock")) {
  // Verbonden!
  myTZ.setLocation(F(TIMEZONE));
  showSplash("NTP sync...", "");
  waitForSync(10);
  char buf[10];
  snprintf(buf, 10, "%02d:%02d:%02d", myTZ.hour(), myTZ.minute(), myTZ.second());
  showSplash("Tijd OK!", buf);
  delay(800);
} else {
  showSplash("Geen WiFi!", "Klok loopt door");
  delay(2000);
}

  randomSeed(analogRead(A0));
  //display.dim(state.dim);   //oled SSD1306
  if (state.dim) display.setContrast(0);    //oled SH1106
  state.update();
  switchMenu(MENU_CLOCK);
}

void loop() {
  events();  // ezTime NTP hersynct op achtergrond

  bool draw = false;

  if (btnNext.update()) {
    menu->button1();
    draw = true;
  }
  if (btnSelect.update()) {
    menu->button2();
    draw = true;
  }

  updateMenuSelection();
  state.update();

  if (menu->update()) {
    draw = true;
  }

  if (draw) {
    display.clearDisplay();
    menu->draw(&display);
    display.display();
  }
}
