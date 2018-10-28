/*
 * This version of the game uses a button lib: https://github.com/rmorenojr/Button
 * This makes the code easier to read and way simpler, but uses a bit more memory
 * Version without lib: Program storage space: 5530 bytes (19%) - dynamic memory: 257 bytes (10%)
 * Version with lib: Program storage space: 5864 bytes (20%) - dynamic memory: 325 bytes (12%)
 */

#include <Button.h>

const int debounceInterval = 30;

const int pinLed = LED_BUILTIN;

const int pinButtonStart = 2;
const int pinButtonAction = 3;

Button startButton(pinButtonStart, BUTTON_PULLUP_INTERNAL, true, debounceInterval);
Button actionButton(pinButtonAction, BUTTON_PULLUP_INTERNAL, true, debounceInterval);

bool needsToStart = false;
bool hasStarted = false;

// Time in ms when the count will start
long timeToStart = 0;
// Time in ms when we actually starts to count
long timeStarted = 0;
// Time when the action button was pushed
long timeStopped = 0;

void setup() {
    Serial.begin(9600);

    pinMode(pinLed, OUTPUT);
}

void loop() {
    unsigned long currentTime = millis();

    if (needsToStart && currentTime >= timeToStart) {
        hasStarted = true;
        timeStarted = currentTime;
        digitalWrite(pinLed, HIGH);
        needsToStart = false;
        Serial.println("GO!");
    }

    if (startButton.uniquePress()) {
        long startDelay = random(500, 2500);
        timeToStart = currentTime + startDelay;
        needsToStart = true;
        Serial.println("Get ready...");
    }

    if (actionButton.uniquePress()) {
        if (hasStarted) {
            timeStopped = currentTime;
            int pushDelay = timeStopped - timeStarted;
            reset();

            Serial.print("Well done! Your reaction time: ");
            Serial.print(pushDelay);
            Serial.println("ms");
        } else if (needsToStart) {
            Serial.println("Hmm... Are you trying to cheat?");
            Serial.println("Game has been reset");
            reset();
        }
    }
}

void reset() {
    digitalWrite(pinLed, LOW);

    needsToStart = false;
    hasStarted = false;

    timeToStart = 0;
    timeStarted = 0;
    timeStopped = 0;
}
