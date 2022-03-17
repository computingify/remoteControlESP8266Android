# Introduction
Very simple remote control to pilote an ESP8266 (arduino based) from an Android application.
It's currently use to pilote a 3D printed paddle boat.
Android application implement a joystick to pilote the boad

## ESP8266
Is an embedded board with a wifi module.
This ESP opened an wifi access point

## Android
An Android application to connect to the ESP8266 access point and send command to it.

# Installation
## ESP 8266
Download arduino ESP8266 library
1. Open the Arduino IDE.
1. Go to File > Preferences.
1. Paste the URL http://arduino.esp8266.com/stable/package_esp8266com_index.json into the Additional Board Manager URLs field. (You can add multiple URLs, separating them with commas.)
1. Go to Tools > Board > Board Manager and search for 'esp8266'. Select the newest version, and click install. (As of February 7th 2017, the latest stable version is 2.3.0.)

THAT'S a board Node MCU 1.0

Download wifi driver for windows (ESP8266): 
https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers

Then compil and load the ESP8266 program to the board

## Android
1. Open Android project into Android Developer Studio
1. Compil and load the Android application to your device

# Usage
1. Start the ESP8266 board
1. Connect Android phone (or tablet) to padddle access point (like you did to connect your phone to your home wifi)
Note: there is no wifi password
1. Start Android application 
=> Lets have fun
