const int debounceInterval = 20;

const int pinLed = LED_BUILTIN;

const int pinButtonStart = 2;
const int pinButtonAction = 3;

// Duration of the game in seconds
const int duration = 5;

unsigned long timeButtonStart = 0;
unsigned long timeButtonAction = 0;

int lastStateButtonStart = 0;
int lastStateButtonAction = 0;

bool needsToStart = false;
bool hasStarted = false;

long timeStartPressed = 0;
long timeStarted = 0;

int startStep = 0;

int numberActionPressed = 0;

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

    if (hasStarted && currentTime - timeStarted >= duration * 1000) {
        hasStarted = false;
        Serial.println("Finished!");
        Serial.print("You pressed the button ");
        Serial.print(numberActionPressed);
        Serial.println(" times. Nice!");
        startStep = 0;
        numberActionPressed = 0;
    }

    if (hasStarted) {
        if (currentTime - timeButtonAction >= debounceInterval
        && stateButtonAction != lastStateButtonAction) {
            timeButtonAction = currentTime;
            lastStateButtonAction = stateButtonAction;

            if (stateButtonAction == LOW) {
                numberActionPressed++;
                // Serial.println(numberActionPressed);
            }
        }
    }

    if (needsToStart) {
        
        if (!hasStarted) {
            if (currentTime - timeStartPressed == 4*1000 && startStep == 3) {
                Serial.println("Go!");
                startStep++;
                needsToStart = false;
                hasStarted = true;
                timeStarted = currentTime;
            } else if (currentTime - timeStartPressed == 3*1000 && startStep == 2) {
                Serial.print("1... ");
                startStep++;
            } else if (currentTime - timeStartPressed == 2*1000 && startStep == 1) {
                Serial.print("2... ");
                startStep++;
            } else if (currentTime - timeStartPressed == 1*1000 && startStep == 0) {
                Serial.print("3... ");
                startStep++;
            }
        }
    }

    if (currentTime - timeButtonStart >= debounceInterval
        && stateButtonStart != lastStateButtonStart) {
        timeButtonStart = currentTime;
        lastStateButtonStart = stateButtonStart;

        // If we want to be able to reset the startDelay, or not
        //if (stateButtonStart == LOW && !needsToStart) {
        if (stateButtonStart == LOW) {
            // Start the game
            needsToStart = true;
            timeStartPressed = currentTime;
            Serial.println("Get ready...");
        }
    }
}
