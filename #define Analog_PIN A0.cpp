#define Analog_PIN A0
#define SET_BTN 
#define RESUME_BTN 
#define CANCEL_BTN
#define SET_BTN_TH 2700
#define RESUME_BTN_TH 800
#define CANCEL_BTN_TH 0
#define TOLERANCE 100
 
 void loop(){
 int analog_BTN = analogRead(Analog_PIN);
  if (analog_BTN > SET_BTN_TH - TOLERANCE && analog_BTN < SET_BTN_TH + TOLERANCE){
    buttons = SET_BTN;
  }
  if (analog_BTN > RESUME_BTN_TH - TOLERANCE && analog_BTN < RESUME_BTN_TH + TOLERANCE){
    buttons = RESUME_BTN;
  }
  if (analog_BTN > CANCEL_BTN_TH - TOLERANCE && analog_BTN < CANCEL_BTN_TH + TOLERANCE){
    button = CANCEL_BTN;
  }
  
  byte buttons;

  unsigned long buttonHeldMillis;
  if (buttons == SET_BTN || buttons == RESUME_BTN) {
    buttonHeldMillis = MUST_HOLD_BUTTON_TO_ACTIVATE;
  } else {
    buttonHeldMillis = MUST_HOLD_BUTTON_TO_CHANGE;
  }

  if (buttons != 0) {
    // Button pressed
    buttonState = HIGH;
    if (buttonState != lastButtonState) {
      lastButtonState = buttonState;
      startButtonPressed = millis();
    }
    if ((millis() - startButtonPressed > buttonHeldMillis) && ! buttonStateChangeProcessed) {
      buttonStateChangeProcessed = true;
      switch (buttons) {
        case SET_BTN:
          if (currentSpeed >= MIN_SPEED_TO_ACTIVATE) {
            targetSpeed = currentSpeed;
            Serial.println(F("ACK S1 pressed (activate/set cruise control)"));

            waitThrottleReleaseThenActivate();
          } else {
            releaseControlFeedback = true;
            releaseControl();
          }
          break;
        case RESUME_BTN:
          if (currentSpeed >= MIN_SPEED_TO_ACTIVATE && targetSpeed > 0) {
            Serial.println(F("ACK S2 pressed (reactivate/reset cruise control)"));

            waitThrottleReleaseThenActivate();
          } else {
            releaseControlFeedback = true;
            releaseControl();
          }
          break;
        case CANCEL_BTN:
          if (controlCode == 1) {
            Serial.println(F("ACK S3 pressed (increase target SPEED by 5)"));
            // tone(BUZZER_PIN, 1500, 50);
            delay(100);
            targetSpeed += 5;
          }
          break;
      }
    } else {
      // Button not pressed
      buttonState = LOW;
      lastButtonState = LOW;
      buttonStateChangeProcessed = false;
    }
  }
}