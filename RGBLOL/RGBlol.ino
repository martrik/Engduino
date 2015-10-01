#include <EngduinoLEDs.h>
int long pasttime;
int r = 15;
int g = 0;
int b = 0;

void setup() {
  // put your setup code here, to run once:
  
  EngduinoLEDs.begin();
}

void loop() {
 
  if (millis()-pasttime > 80) {
    pasttime = millis();

    if (g == 15) {
      if (b > 0) {
        b--;
      } else {
        r++;
      }
    }

    if (b == 15) {
      if (r > 0) {
        r--;
      } else {
        g++;
      }
    }

    if (r == 15) {
      if (g > 0) {
        g--;
      } else {
        b++;
      }
    }

    EngduinoLEDs.setAll(r, g, b);
  // put your main code here, to run repeatedly:
  }
}
