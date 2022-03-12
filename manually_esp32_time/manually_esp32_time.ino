/*
   MIT License

  Copyright (c) 2021 Felix Biego

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

/*  This modified sketch will let you manually update the ESP32 Real Time Clock via the IDE Serial Monitor
 *  
 *  You enter Update Mode by pressing the BOOT/Flash button on an ESP Dev board
 *  I used the onboard LED to blink while the system is waiting for an update
 *  Only a "true" input will be accepted, no leading zeros
 *  The int format to enter is: day/month/year hour:minute:second IE: 10/7/2021 16:30:0
 *  
 *  Modified by: 	R McCleery macca448@gmail.com
 *  Date:   		July 10th 2021
 */

#include <ESP32Time.h>	    // https://github.com/fbiego/ESP32Time
#define UDT 0                                 // DEV board BOOT/FLASH button
#define LED 2                                 // Onboard LED on ESP32
#define DUTY_CYCLE 50                         // Single millis() duty cycle for the loop()

ESP32Time rtc;                                // Real Time Clock Object

long      currentTime = 0;

uint8_t   count = 0, blinkCount = 0, udtState,
          lastUDTstate = HIGH, ledState = LOW;

int       Sc, Mn, Hr, Dy, Mh, Yr;
          
bool      updateDateTime = false, ledUpdate = false,
          UDTpressed = false, blink = false, sPtime = true;

bool inRange(int val, int minimum, int maximum){
    return ((minimum <= val) && (val <= maximum));
}

void setup() {
    Serial.begin(115200);
    pinMode(UDT, INPUT);
    pinMode(LED, OUTPUT);
    digitalWrite(LED, ledState);
}

void loop() {
    udtState = digitalRead(UDT);
        if(udtState != lastUDTstate){
            UDTpressed = true;
    }

    if(millis() - currentTime > DUTY_CYCLE){
        currentTime = millis();
        count++;
        blinkCount++;
            if(UDTpressed){
                if(udtState == LOW){
                    Serial.println("**********   Update Time and Date   **********");
                    Serial.println("********** Fast Blinking LED = True **********");
                    Serial.println("**********     No Leading ZERO's    **********");
                    Serial.println("* Type in: dd/mm/yyyy hh:mm:ss to update RTC *");                
                    updateDateTime = true;
                    blink = true;
                    UDTpressed = false;
                }
            }
            
            if(sPtime){
                if(count >= 100){             
                    Serial.println(rtc.getTime("%A, %B %d %Y %H:%M:%S")); 
                    //formating options  http://www.cplusplus.com/reference/ctime/strftime/
                    count = 0;
                }               
            }

            if(blink){
                if(blinkCount > 5){
                    blinkCount = 0;
                    ledState = !ledState;
                    ledUpdate = true;
                }
            }
            
            if(ledUpdate){
                digitalWrite(LED, ledState);
                ledUpdate = false;
            } 
    }  // END millis() statements

    if(updateDateTime){
        sPtime = false;   
            if(Serial.available() > 0) {
                Dy = Serial.parseInt();
                Mh = Serial.parseInt();
                Yr = Serial.parseInt();
                Hr = Serial.parseInt();
                Mn = Serial.parseInt();
                Sc = Serial.parseInt();
                Serial.printf("I have understood %u/%u/%u %u:%u:%u\n", Dy, Mh, Yr, Hr, Mn, Sc);
                // Values being set ( Day(%d), Month(%m), Year(%Y), Hr(%H), Min(%M), Sec(%S) )   
                boolean validDate = (inRange(Dy, 1, 31) && inRange(Mh, 1, 12) && inRange(Yr, 2021, 2031));
                boolean validTime = (inRange(Hr, 0, 23) && inRange(Mn, 0, 59) && inRange(Sc, 0, 59));
                    if(validTime && validDate){     
                        rtc.setTime(Sc, Mn, Hr, Dy, Mh, Yr);
                        Serial.flush();
                        sPtime = true;        
                        blink = false;
                        ledState = LOW;
                        ledUpdate = true;
                        updateDateTime = false;
                    }
            }
    }
    lastUDTstate = udtState;        // Update Last button State
}
