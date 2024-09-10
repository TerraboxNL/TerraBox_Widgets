#include <TerraBox_Widgets.h>

bool touch = false;
bool prevTouch = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Screen.begin();

  Screen.setRotation(0);
  Screen.setTextSize(1);
  Screen.setCursor(30, 10);
  Screen.println("Tap the screen and watch the Serial Monitor");
  Screen.setCursor(70, 20);
  Screen.println("for raw Touch coordinates...");
}

void loop() {
  // put your main code here, to run repeatedly:
  XY xy;

  touch = getRawTouchData(&xy);

  if (!prevTouch && touch) {
    Serial.print(F("Touch: ")); Serial.print((touch ? F("True"): F("False")));
    Serial.print(F(" X:")); Serial.print(xy.x); 
    Serial.print(F(" Y:")); Serial.print(xy.y);
    Serial.print(F(" Pressure:"));Serial.println(xy.z);
    delay(500);
 }

  else if (prevTouch && !touch) {
    Serial.println(F("Untouch"));
  }

  prevTouch = touch;
 }
