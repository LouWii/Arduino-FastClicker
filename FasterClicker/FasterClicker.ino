
const int debounceInterval = 50;

const int pinLed = LED_BUILTIN;

const int pinButtonStart = 2;
const int pinButtonAction = 3;

unsigned long timeButtonStart = 0;
unsigned long timeButtonAction = 0;

int lastStateButtonStart = 0;
int lastStateButtonAction = 0;

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

    pinMode(pinButtonStart, INPUT_PULLUP);
    pinMode(pinButtonAction, INPUT_PULLUP);

    pinMode(pinLed, OUTPUT);
}

void loop() {
    unsigned long currentTime = millis();

    int stateButtonStart = digitalRead(pinButtonStart);
    int stateButtonAction = digitalRead(pinButtonAction);

    if (needsToStart && currentTime >= timeToStart) {
        hasStarted = true;
        timeStarted = currentTime;
        digitalWrite(pinLed, HIGH);
        needsToStart = false;
        Serial.println("GO!");
    }

    if (currentTime - timeButtonStart >= debounceInterval
        && stateButtonStart != lastStateButtonStart) {
        timeButtonStart = currentTime;
        lastStateButtonStart = stateButtonStart;

        // If we want to be able to reset the startDelay, or not
        //if (stateButtonStart == LOW && !needsToStart) {
        if (stateButtonStart == LOW) {
            // Start the game

            long startDelay = random(500, 2500);
            timeToStart = currentTime + startDelay;
            needsToStart = true;
            Serial.println("Get ready...");
        }
    }

    if (currentTime - timeButtonAction >= debounceInterval
        && stateButtonAction != lastStateButtonAction) {
        timeButtonAction = currentTime;
        lastStateButtonAction = stateButtonAction;
        
        if (stateButtonAction == LOW) {
            if (hasStarted) {
                timeStopped = currentTime;
                int pushDelay = timeStopped - timeStarted;
                reset();

                Serial.println("Well done!");
                Serial.println(pushDelay);
            } else if (needsToStart) {
                Serial.println("Hmm... Are you trying to cheat?");
                Serial.println("Game has been reset");
                reset();
            }
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
