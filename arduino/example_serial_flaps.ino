char *strtok(char *str1, const char *str2);
char *receivedData = NULL;
const byte numChars = 60;
char receivedChars[numChars];
boolean newData = false;

// example code
unsigned long previousMillisLedFlaps = 0;
unsigned long previousMillisToggleFlaps = 0;
const long intervalLedFlaps = 1000;
const long intervalToggleFlaps = 3500;
const int LED_PIN_FLAPS = 2;
int currentFlapRatio = 0;

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN_FLAPS, OUTPUT);
}

void loop() {
    unsigned long currentMillis = millis();
    serialReceiver();

    // example code - XPlaneLedFlaps
    if (currentMillis - previousMillisLedFlaps >= intervalLedFlaps) {
      previousMillisLedFlaps = currentMillis;
      Serial.println("getDREF#sim/cockpit2/controls/flap_ratio");
    }
    // example code - XPlaneToggleFlaps [TODO: press button]
    if (currentMillis - previousMillisToggleFlaps >= intervalToggleFlaps) {
      previousMillisToggleFlaps = currentMillis;
      if(currentFlapRatio > 0){
        Serial.println("sendDREF#sim/cockpit2/controls/flap_ratio#0");
      }else{
        Serial.println("sendDREF#sim/cockpit2/controls/flap_ratio#1");
      }
    }
}

void XPlaneReceiverDREF(String receivedDREF, double receivedValue) {
  Serial.println("receivedDREF: " + receivedDREF);
  Serial.println("receivedValue: " + String(receivedValue));

  // example code - event XPlaneLedFlaps
  if(receivedDREF == "sim/cockpit2/controls/flap_ratio"){
    currentFlapRatio = receivedValue; // global
    if(currentFlapRatio > 0){
      // LED ON
      digitalWrite(LED_PIN_FLAPS, HIGH);
    }else{
      // LED OFF
      digitalWrite(LED_PIN_FLAPS, LOW);
    }
  }
}

void serialReceiver() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;

    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();
        if(rc == '\n' || rc == '\r')
        {
          continue;
        }
        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }
        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
    if (newData == true) {
        //Serial.flush();
        char delimiter[] = "#";
        char *receivedDREF = NULL;
        char *receivedValue = NULL;
        receivedData = strtok(receivedChars, delimiter);
        while (receivedData != NULL) {
          if(receivedDREF == NULL){
            receivedDREF = receivedData;
          }else if(receivedValue == NULL){
            receivedValue = receivedData;
          }
          receivedData = strtok(NULL, delimiter);
        }

        XPlaneReceiverDREF(String(receivedDREF), String(receivedValue).toDouble());

        newData = false;
    }
}
