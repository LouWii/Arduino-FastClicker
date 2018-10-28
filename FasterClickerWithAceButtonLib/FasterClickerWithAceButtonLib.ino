/*
 * This version of the game uses a button lib: https://github.com/bxparks/AceButton
 * This makes the code easier to read and way simpler, but uses a bit more memory
 * Version without lib: Program storage space: 5530 bytes (19%) - dynamic memory: 257 bytes (10%)
 * Version with lib: Program storage space: 6798 bytes (23%) - dynamic memory: 345 bytes (13%)
 * 
 * This lib uses a "lot" of memory, mainly because we need to create 2 ButtonConfig objects.
 */

#include <AceButton.h>
using namespace ace_button;

const int pinLed = LED_BUILTIN;

const int pinButtonStart = 2;
const int pinButtonAction = 3;

// As we use 2 buttons, we need to create separate button configs first
ButtonConfig startConfig;
AceButton startButton(&startConfig);
ButtonConfig actionConfig;
AceButton actionButton(&actionConfig);

bool needsToStart = false;
bool hasStarted = false;

// Time in ms when the count will start
long timeToStart = 0;
// Time in ms when we actually starts to count
long timeStarted = 0;
// Time when the action button was pushed
long timeStopped = 0;

void handleStartEvent(AceButton* /* button */, uint8_t eventType, uint8_t buttonState);
void handleActionEvent(AceButton* /* button */, uint8_t eventType, uint8_t buttonState);

void setup() {
    Serial.begin(9600);

    pinMode(pinLed, OUTPUT);

    startConfig.setEventHandler(handleStartEvent);
    startConfig.setFeature(ButtonConfig::kFeatureClick);
    pinMode(pinButtonStart, INPUT_PULLUP);
    startButton.init(pinButtonStart, HIGH, 0);

    actionConfig.setEventHandler(handleActionEvent);
    actionConfig.setFeature(ButtonConfig::kFeatureClick);
    pinMode(pinButtonAction, INPUT_PULLUP);
    actionButton.init(pinButtonAction, HIGH, 1);
}

void loop() {
    startButton.check();
    actionButton.check();

    unsigned long currentTime = millis();

    if (needsToStart && currentTime >= timeToStart) {
        hasStarted = true;
        timeStarted = currentTime;
        digitalWrite(pinLed, HIGH);
        needsToStart = false;
        Serial.println("GO!");
    }
}

void handleStartEvent(AceButton* /* button */, uint8_t eventType, uint8_t buttonState) {
    if (eventType == AceButton::kEventPressed) {
        unsigned long currentTime = millis();
        long startDelay = random(500, 2500);
        timeToStart = currentTime + startDelay;
        needsToStart = true;
        Serial.println("Get ready...");
    }
}

void handleActionEvent(AceButton* /* button */, uint8_t eventType, uint8_t buttonState) {
    if (eventType == AceButton::kEventPressed) {
        if (hasStarted) {
            unsigned long currentTime = millis();
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
