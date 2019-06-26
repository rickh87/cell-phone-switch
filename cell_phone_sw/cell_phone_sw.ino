
/*
 SMS sender

 This sketch, for the MKR GSM 1400 board,sends an SMS message
 you enter in the serial monitor. Connect your Arduino with the
 GSM shield and SIM card, open the serial monitor, and wait for
 the "READY" message to appear in the monitor. Next, type a
 message to send and press "return". Make sure the serial
 monitor is set to send a newline when you press return.

 Circuit:
 * MKR GSM 1400 board
 * Antenna
 * SIM card that can send SMS

 created 25 Feb 2012
 by Tom Igoe
*/

// Include the GSM library
#include <MKRGSM.h>
#include <string.h>

// #include "arduino_secrets.h" 
// Please enter your sensitive data in the Secret tab or arduino_secrets.h
// PIN Number: NOTE: the Ting SIM card does not have a PIN number
// const char PINNUMBER[] = SECRET_PINNUMBER;
#define PINNUMBER ""
const char senderNumber[] = "6143163988\n";
char remoteNum[20];  // telephone number to send sms
char txtMsg[200];
bool debug = true;
char txtMessage[200] = {};
char txtMessage1[] = "Relay is on.";
char txtMessage2[] = "Relay is off.";
char txtMessage3[] = "Front panel button pressed. ";

int BUTTON = 0;
int RELAY = 1;
int FLAG = 0;

// initialize the library instance
GSM gsmAccess;
GSM_SMS sms;

/*
  Read input serial
 */
int readSerial(char result[]) {
  int i = 0;
  while (1) {
    while (Serial.available() > 0) {
      char inChar = Serial.read();
      if (inChar == '\n') {
        result[i] = '\0';
        Serial.flush();
        return 0;
      }
      if (inChar != '\r') {
        result[i] = inChar;
        i++;
      }
    }
  }
}

void sendTxt(char txtMsg[]){
   if(debug){
      Serial.println(txtMsg);
   }
   else{
      sms.beginSMS(senderNumber);
      sms.print(txtMsg);
      sms.endSMS();
   } 
}

void setup() {
  // initialize serial communications and wait for port to open:
  Serial.begin(1200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // connection state
  bool connected = false;

  // Start GSM shield
  // If your SIM has PIN, pass it as a parameter of begin() in quotes
  while (!connected) {
    if (gsmAccess.begin(PINNUMBER) == GSM_READY) {
      connected = true;
    } else {
      Serial.println("Not connected");
      delay(1000);
    }
  }

  Serial.println("GSM initialized");
  pinMode(RELAY, OUTPUT);
  pinMode(BUTTON, INPUT);
  digitalWrite(RELAY, LOW);
}

void loop() {
  int c;
  int i;

  // put your main code here, to run repeatedly:
  // check to see if the button is pressed
  if (digitalRead(BUTTON) == 0){
    Serial.println("In button test loop");
    if(FLAG == 0){
       FLAG = 1;
       digitalWrite(RELAY, HIGH);
//     txtMessage = txtMessage3 + txtMessage1;
       strcat(txtMessage, txtMessage3);
       strcat(txtMessage, txtMessage1);
       sendTxt(txtMessage);
    }
    else{
      FLAG = 0;
      digitalWrite(RELAY, LOW);
//    txtMessage = txtMessage3 + txtMessage2;
      strcat(txtMessage, txtMessage3);
      strcat(txtMessage, txtMessage1);
      sendTxt(txtMessage);
      }
  }   
  // If there are any SMSs available()
  if (sms.available()) {
    Serial.println("Message received from:");
    
    // An example of message disposal
    // Any messages starting with # should be discarded
    if (sms.peek() == '#') {
      Serial.println("Discarded SMS");
      sms.flush();
    }

    // Read message bytes and print them if in debug mode
    while ((c = sms.read()) != -1) {
      if(debug){
         Serial.print((char)c);
      }
    }
    
    Serial.println(" ");
    if(FLAG == 0){
       FLAG = 1;
       digitalWrite(RELAY, HIGH);    
       sendTxt(txtMessage1);
       }
    else{
       FLAG = 0;
       digitalWrite(RELAY, LOW);
       sendTxt(txtMessage2);
       }

    // Delete message from modem memory
    sms.flush();
  }
  delay(1000);
}
