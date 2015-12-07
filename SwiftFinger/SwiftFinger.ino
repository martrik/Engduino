#include <EngduinoLEDs.h>
#include <EngduinoThermistor.h>
#include <math.h>

// Finger detect
float pastTemp;
int long tempCycle;
bool finger;

// General
int long cycleStart;
int state = 0;

// Lights
int nextRow = 0;
int direct = 1;
int counter;

int rows[5][4] = {
  {0, 15, 14, 13},
  {1, -1, -1, 12},
  {3, -1, -1, 11},
  {5, -1, -1, 10},
  {6, 7, 8, 9}
};

int road[16] = {0, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};

void setup() {
  EngduinoLEDs.begin();
  EngduinoThermistor.begin();
  cycleStart = millis();
}

void loop() {
  // Idle state
  if (state == 0) {
    moveLights();
    // Change state when finger ready
    if (analogRead(4) < 10) {
      state = 1;
      counter = 0;
      EngduinoLEDs.setAll(OFF);
    }
  }
  // Display gameboard
  else if (state == 1) {
    for (int i = 0; i < 10; i++) {
      Serial.println(road[8-i]);
      EngduinoLEDs.setLED(road[8-i], 0, 0, 10);
      delay(40);
    }
    
    // LED traffic light 
    EngduinoLEDs.setLED(3, 10, 0, 0);
    delay(500);
    EngduinoLEDs.setLED(3, 10, 2, 0);
    delay(500);
    EngduinoLEDs.setLED(3, 0, 10, 0);
    delay(500);
    EngduinoLEDs.setLED(3, OFF);

    cycleStart = millis();
    counter = 0;
    state = 2;    
  }
  else if (state == 2) {
    if (millis()-cycleStart > 20) {
      if (counter < 9) {
        EngduinoLEDs.setLED(road[counter], OFF);
        counter++;
        cycleStart = millis();
      } else if (counter < 16) {
        EngduinoLEDs.setLED(road[counter], 4, 0, 0);
        counter++;
        cycleStart = millis();
      }
      if ((analogRead(4) > 30 && analogRead(4)<500) || counter > 15) {
        state = 3;
        Serial.println(analogRead(4));
      }
    }    
  }

  // Display result
  else if (state == 3) {
    // Too early
    if (counter < 9) {
      for (int i = 0; i<2; i++) {
        for (int i = counter; i < 9; i++) {
          EngduinoLEDs.setLED(road[i], 4, 0, 0);
        }
        delay(500);
        for (int i = counter; i < 9; i++) {
          EngduinoLEDs.setLED(road[i], OFF);
        }        
        delay(500);
      }
      state = 0;
    }
    // Too late
    if (counter > 9) {
      delay(500);
      for (int i = 9; i < counter; i++) {
        EngduinoLEDs.setLED(road[i], OFF);
      }
      delay(500);
      for (int i = 9; i < counter; i++) {
        EngduinoLEDs.setLED(road[i], 4, 0, 0);
      }        
      delay(500);
      EngduinoLEDs.setAll(OFF);
      delay(500);
      state = 0;
    }
    // Sweet spot
    if (counter == 9) {
      EngduinoLEDs.setAll(GREEN);
      delay(500);
      EngduinoLEDs.setAll(OFF);
      delay(500);
      EngduinoLEDs.setAll(GREEN);
      delay(500);
      EngduinoLEDs.setAll(OFF);
      state = 0;
    }
  }    
}


void moveLights() {
  // Change lights each cycle
  if (millis() - cycleStart > 100) {
     lightUpRow(nextRow);
     nextRow += direct;
     cycleStart = millis();

     // Change light direction
     if (nextRow == 5) {
       direct = -1;
       nextRow = 3;
     } 
     else if (nextRow == -1) {
       shutDownRow(1);
       shutDownRow(2);
       for (int i = 0; i < 4; i++) {
         if (rows[1][i] != -1) EngduinoLEDs.setLED(rows[1][i], 0, 0, 1);
       }
       direct = 1;
       nextRow = 1;
     }  
   }
}

void lightUpRow(int row) {
  for (int i = 0; i < 4; i++) {
    if (rows[row][i] != -1) EngduinoLEDs.setLED(rows[row][i], 0, 0, 8);
    if (rows[row-direct][i] != -1) EngduinoLEDs.setLED(rows[row-direct][i], 0, 0, 1);
  }
  if (row != 0) shutDownRow(row-2*direct);
}

void shutDownRow(int row) {
  for (int i = 0; i < 4; i++) {
    if (rows[row][i] != -1) EngduinoLEDs.setLED(rows[row][i], OFF);
  }
}

/*void fadeLEDsToColorInTime(int r, int g, int b, int t) {
  int long initTime = millis();
  float rIncr = r/20.00, gIncr = g/20.00, bIncr = b/20.00;
  float currentR, currentG, currentB;
  
  while (initTime != 0) {
    if (millis()-initTime > t/20) {
      currentR += rIncr;
      currentG += gIncr;
      currentG += bIncr;
      Serial.println(currentR);
      Serial.println(currentG);
      Serial.println(currentB);
      EngduinoLEDs.setAll((uint8_t)round(currentR), (uint8_t)round(currentG), (uint8_t)round(currentB));
      initTime = millis();
    }
    if (currentR >= r && currentG >= g && currentB >= b) initTime = 0;
  }
}*/

