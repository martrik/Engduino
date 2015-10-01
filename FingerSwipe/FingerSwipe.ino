#include <EngduinoLEDs.h>
#include <EngduinoLight.h>
#include <EngduinoButton.h>

/* This is a demo game using UCL Engduino based on user imput thanks to the
 * LDR and output through the LEDs. The game goes as follows: when the time starts
 * the player has to swipe his finger on top of the LDR as fast as possible
 * during 10 seconds. Once the time has finished the user is given a score
 * displayed by the LEDs.
 */

int state = 0;
int r = 0, g = 1, b = 2;
int long averageLight;
int lastClear = HIGH;
int long cycleStart;
int nextLEDoff;
int score = 0;

int rows[5][4] = {{0, 15, 14, 13},
  {1, -1, 12, -1},
  {3, 11, -1, -1},
  {5, -1, 10, -1},
  {6, 7, 8, 9}
};

void setup() {
  EngduinoLEDs.begin();
  EngduinoLight.begin();
  EngduinoButton.begin();
  cycleStart = millis();
}

void loop()
{
  // Keep running lights
  if (state == 0) {
    if (millis() - cycleStart > 400) {
      lightUpRow(nextLEDoff);
      nextLEDoff = (nextLEDoff + 1) % 5;
      cycleStart = millis();
    }
    if (EngduinoButton.wasPressed()) state = 1;
  }

  // Starting game LED sequence
  else if (state == 1) {
    nextLEDoff = 10;
    lastClear = HIGH;
    
    // Get average light
    EngduinoLEDs.setAll(OFF);
    for (int i = 0; i < 100; i++) {
      averageLight = (averageLight * i + analogRead(4)) / (i + 1);
    }
    EngduinoLEDs.setAll(RED);
    delay(500);
    EngduinoLEDs.setAll(15, 7, 0);
    delay(500);
    EngduinoLEDs.setAll(GREEN);
    delay(500);
    EngduinoLEDs.setAll(OFF);
    Serial.println(averageLight);

    cycleStart = millis();
    for (int i = 10; i < 17; i++) {
      if (i == 16) EngduinoLEDs.setLED(0, 2, 1, 0);
      else EngduinoLEDs.setLED(i, 2, 1, 0);
    }
    state = 2;
  }

  // Start swiping!
  else if (state == 2) {
    if (averageLight - analogRead(4) > averageLight*0.4) {
      if (lastClear) {
        lastClear = LOW;
        Serial.println(analogRead(4));
        Serial.println("Finger swiped");
        score++;
      }
    } else lastClear = HIGH;

    // Set light to show time left
    if (millis() - cycleStart > 1000) {
      EngduinoLEDs.setLED(nextLEDoff, OFF);
      nextLEDoff++;
      cycleStart = millis();
    }

    if (nextLEDoff == 17) {
      delay(250);
      EngduinoLEDs.setAll(7, 2, 0);
      delay(500);
      EngduinoLEDs.setAll(OFF);
      delay(400);
      EngduinoLEDs.setAll(7, 2, 0);
      delay(500);
      EngduinoLEDs.setAll(OFF);
      state = 3;
    }
  }

  // End game show score
  else if (state == 3) {
    Serial.println(score);
    cycleStart = millis();
    nextLEDoff = 0;
    score = 0;
    averageLight = 0;
    state = 0;
  }
}

void lightUpRow(int row) {
  if (row == 0) {
    EngduinoLEDs.setAll(OFF);
    delay(400);
  }
  for (int i = 0; i < 4; i++) {
    if (rows[row][i] != -1) EngduinoLEDs.setLED(rows[row][i], BLUE);
  }
}

