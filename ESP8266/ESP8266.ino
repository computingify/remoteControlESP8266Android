
/*
   Copyright (c) 2022, Computingify
   All rights reserved.

   Redistribution and use in source and binary forms, with or without modification,
   are permitted provided that the following conditions are met:

 * * Redistributions of source code must retain the above copyright notice, this
     list of conditions and the following disclaimer.

 * * Redistributions in binary form must reproduce the above copyright notice, this
     list of conditions and the following disclaimer in the documentation and/or
     other materials provided with the distribution.

 * * Neither the name of Majenko Technologies nor the names of its
     contributors may be used to endorse or promote products derived from
     this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
   ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
   ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
   (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
   ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <string>
#include <cstddef>         // std::size_t
#include <stdexcept>

/* Set these to your desired credentials. */
const char* ssid = "paddle";
const char* password = "";
const int port = 80;

int PWMA = 5;//Right side
int PWMB = 4;//Left side
int DA = 0;//Right reverse
int DB = 2;//Left reverse

WiFiServer wifiServer(port);

typedef enum {
  STOP = 0,
  FROWARD,
  BACKWARD,
  TURN_RIGHT,
  TURN_LEFT,
  FROWARD_TURN_RIGHT,
  FROWARD_TURN_LEFT,
  BACKWARD_TURN_RIGHT,
  BACKWARD_TURN_LEFT
}message;

void handleAction(std::string actionStr) {
  bool isReady = false;
  int action = 0;
  
  try{
    action = std::stoi(actionStr);
    isReady = true;
  }catch(std::invalid_argument& e) {
    Serial.printf("Unknown command: %s\n", actionStr.c_str());
  }
  
  if (isReady) {
    
    switch (action) {
      case FROWARD:
        digitalWrite(PWMA, HIGH);
        digitalWrite(DA, LOW);
  
        digitalWrite(PWMB, HIGH);
        digitalWrite(DB, LOW);
        Serial.println("Forward");
        break;
  
      case BACKWARD:
        digitalWrite(PWMA, HIGH);
        digitalWrite(DA, HIGH);
  
        digitalWrite(PWMB, HIGH);
        digitalWrite(DB, HIGH);
        Serial.println("Backward");
        break;
  
      case TURN_RIGHT:
        digitalWrite(PWMA, 450);
        digitalWrite(DA, LOW);
  
        digitalWrite(PWMB, 450);
        digitalWrite(DB, HIGH);
        Serial.println("Right turn");
        break;
        
      case TURN_LEFT:
        digitalWrite(PWMA, 450);
        digitalWrite(DA, HIGH);
  
        digitalWrite(PWMB, 450);
        digitalWrite(DB, LOW);
        Serial.println("Left Turn");
        break;
        
      case FROWARD_TURN_RIGHT:
        digitalWrite(PWMA, HIGH);
        digitalWrite(DA, LOW);
  
        digitalWrite(PWMB, LOW);
        digitalWrite(DB, LOW);
        Serial.println("Forward and Right");
        break;
        
      case FROWARD_TURN_LEFT:
        digitalWrite(PWMA, LOW);
        digitalWrite(DA, LOW);
  
        digitalWrite(PWMB, HIGH);
        digitalWrite(DB, LOW);
        Serial.println("Forward and Left");
        break;
        
      case BACKWARD_TURN_RIGHT:
        digitalWrite(PWMA, HIGH);
        digitalWrite(DA, HIGH);
  
        digitalWrite(PWMB, LOW);
        digitalWrite(DB, LOW);
        Serial.println("Backward and Right");
        break;
        
      case BACKWARD_TURN_LEFT:
        digitalWrite(PWMA, LOW);
        digitalWrite(DA, LOW);
  
        digitalWrite(PWMB, HIGH);
        digitalWrite(DB, HIGH);
        Serial.println("Backward and Left");
        break;
        
      default:
        digitalWrite(PWMA, LOW);
        digitalWrite(DA, LOW);
  
        digitalWrite(PWMB, LOW);
        digitalWrite(DB, LOW);
        Serial.println("Stop");
        break;
    }
  }
}

void setup() {
   Serial.begin(115200);

   Serial.println();
   Serial.println("Configuring access point...");
   boolean result = WiFi.softAP(ssid, password);
   if(result == true) {
      Serial.println("Ready");
   }else {
      Serial.println("Failed!");
   }
   
   // Set Boat Pin
   pinMode(PWMA, OUTPUT);
   pinMode(PWMB, OUTPUT);
   pinMode(DA, OUTPUT);
   pinMode(DB, OUTPUT);
   Serial.println("configuration ended");
   Serial.println("Waitting connection");
   int connectionNumber = 0;
   while(0 == connectionNumber) {
     connectionNumber = WiFi.softAPgetStationNum();
     Serial.print(".");
     delay(1000);
   }
  
   wifiServer.begin();
}

// All command should be ended by # char
void loop() {
   WiFiClient client = wifiServer.available();

   if (client) {
    Serial.println("Client present, launch receiver loop");
    while (client.connected()) {
      std::string actionStr;
      bool endOfLine = false;
      int numberOfByteToRead = client.available();
      if(0 < numberOfByteToRead) {
        while (0 < numberOfByteToRead) {
          char c = client.read();
          actionStr.push_back(c);
          numberOfByteToRead--;
        }
        // Search for the last end of line char "#"
        std::size_t pos = actionStr.find_last_of("#");
        if(std::string::npos != pos) {
          // Remove the end of line char
          actionStr = actionStr.substr(0, pos);
          // Search for the last end of line char "#" because just after that's the last requested user action
          pos = actionStr.find_last_of("#");
          if(std::string::npos != pos) {
            // Get all char after the end of line => that's the last action
            actionStr = actionStr.substr(pos + 1);
          } // even that's a commande with only one action
          handleAction(actionStr);
        }
      }
      // Add a delay to filter fast changing command and take only the lastest one
      delay(250);
    }
    client.stop();
    Serial.println("Client disconnected");

   }
   delay(500);
}
