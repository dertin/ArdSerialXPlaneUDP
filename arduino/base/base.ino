char *strtok(char *str1, const char *str2);
char *receivedData = NULL;
const byte numChars = 60;
char receivedChars[numChars];
boolean newData = false;

void setup() {
  Serial.begin(9600);
}

void loop() {
    unsigned long currentMillis = millis();
    serialReceiver();

    //Serial.println("getDREF#sim/cockpit2/controls/flap_ratio");
    //Serial.println("sendDREF#sim/cockpit2/controls/flap_ratio#1");

}

void XPlaneReceiverDREF(String receivedDREF, double receivedValue) {
  //Serial.println("receivedDREF: " + receivedDREF);
  //Serial.println("receivedValue: " + String(receivedValue));
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
