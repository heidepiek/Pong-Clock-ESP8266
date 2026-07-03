# Pong-Clock-ESP8266

A Pong Clock altered to use with an ESP8266 (Wemos D1 Mini) with NTP WiFi via ezTime.
Replaced Wunderground with ezTime for automatic timezone and DST support.
Added Pac-Man clockface and WiFiManager for easy WiFi setup.

Based on ESP8266 version by KiwiBushwalker (Thingiverse thing:2763302).
Original Pong Clock by 0miker0 & mic159 (https://github.com/mike-rankin/Pong-Clock).

## Hardware
- Wemos D1 Mini (ESP8266)
- 0.96" I2C OLED display (SSD1306 or SH1106, 128x64)
- 2x push buttons

## Wiring
| Component | D1 Mini |
|-----------|---------|
| OLED VCC  | 3.3V    |
| OLED GND  | GND     |
| OLED SDA  | D2      |
| OLED SCL  | D1      |
| Button NEXT (switch face) | D5 |
| Button SELECT (menu)      | D6 |

Buttons are wired to GND on the other side (INPUT_PULLUP).

## Required Libraries
Install via Arduino IDE Library Manager:
- ezTime by Rop Gonggrijp
- WiFiManager by tzapu
- Adafruit SSD1306 (for SSD1306 OLED)
- Adafruit SH110X (for SH1106 OLED)
- Adafruit GFX Library

## Arduino IDE Settings
- Board: LOLIN(WEMOS) D1 mini
- Upload via USB

## Configuration
Open `PongClock_ezTime.ino` and set your timezone:
```cpp
#define TIMEZONE "Europe/Amsterdam"
```

## WiFi Setup
This project uses WiFiManager - no need to hardcode your WiFi credentials!

1. On first boot, the clock creates a WiFi access point called **"PongClock"**
2. Connect to "PongClock" with your phone or computer
3. Go to **192.168.4.1** in your browser
4. Select your WiFi network and enter your password
5. The clock will remember your credentials from now on

## Clockfaces
Switch between clockfaces using button D5:
- Pong
- Digital
- Pac-Man
- Tetris

## Menu
Press button D6 to open the settings menu:
- Select clockfaces on/off
- 12/24 hour mode
- Brightness
- Set time and date manually (when no WiFi available)
