
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
char txtMessage1[] = "Relay is off.";
char txtMessage2[] = "Relay is on.";
char txtMessage3[] = "Front panel button pressed. ";
char txtMessage4[] = "Wrong PIN number";
char txtMessage5[] = "Available commands are: T/t (toggle), N/n (on), F/f (off), S/s (status), C/c (Change PIN), H/h (Help)";
char txtMessage6[] = " is not a recognized command";
char txtMessage7[] = "PIN can only be changed by the Master";
char txtMessage8[] = "Enter new 4 digit PIN";
char txtMessage9[] = "Reenter 4 digit PIN";
char txtMessage10[] = "PINs do not match.";
char txtMessage11[] = "Cell/router switch connected";

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
      sms.beginSMS(senderNumber);
      sms.print(txtMsg);
      sms.endSMS();
      digitalWrite(RELAY, LOW);   
}

void setup() {
  bool connected = false;

  // Start GSM shield
  // If your SIM has PIN, pass it as a parameter of begin() in quotes
  digitalWrite(RELAY, HIGH);
  delay(2000);
  digitalWrite(RELAY, LOW);
  while (!connected){
     if (gsmAccess.begin(PINNUMBER) == GSM_READY) {
        connected = true;
        strcat(txtMessage, txtMessage11);
        sendTxt(txtMessage);
        digitalWrite(RELAY, HIGH);
        delay(2000);
      }   
     delay(1000);
  }

  pinMode(RELAY, OUTPUT);
  pinMode(BUTTON, INPUT);
  // The relay is normally closed. The realy should default to closed (off). 
  digitalWrite(RELAY, LOW);
}

void loop() {
  int c;
  int i;
  int t;
  // put your main code here, to run repeatedly:
  // check to see if the button is pressed
  if (digitalRead(BUTTON) == 0){
    if(FLAG == 0){
       FLAG = 1;
       digitalWrite(RELAY, HIGH);
       strcat(txtMessage, txtMessage3);
       strcat(txtMessage, txtMessage1);
       sendTxt(txtMessage);
    }
    else{
      FLAG = 0;
      digitalWrite(RELAY, LOW);
      strcat(txtMessage, txtMessage3);
      strcat(txtMessage, txtMessage2);
      sendTxt(txtMessage);
      }
  }  
  else{
      // If there are any SMSs available()
      if (sms.available()) {   
      // An example of message disposal
      // Any messages starting with # should be discarded
      if (sms.peek() == '#') {
         sms.flush();
         }
      }
      // Read message bytes and print them if in debug mode
      i = 0;
      while ((c = sms.read()) != -1){
        txtMessage[i] = c;
        i = i + 1;
      }
  }
    if ((txtMessage[0] != '9') || (txtMessage[1] != '5') || (txtMessage[2] != '3') || (txtMessage[3] != '6')){
       sendTxt(txtMessage4);
       }
    else{
       t = (int) txtMessage[4];
// if t is less than 97 it is ASCII upper case. Adding 34 will change it to lower case ASCII       
       if (t < 97){
          t = t + 32;
       }
// switch case statements don't work with characters. It will generate a duplicate case error when compiled. 
// charactes can be acessed as integers get placing single quotes around them.
       switch (t){
          case 't':
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
             break;
          case 'n':
             FLAG = 1;
             digitalWrite(RELAY, LOW);    
             sendTxt(txtMessage1);
             break;
          case 'f':
             FLAG = 0;
             digitalWrite(RELAY, HIGH);
             sendTxt(txtMessage2);
             break;
          case 's':
            if(FLAG == 0){
                 sendTxt(txtMessage2);
                 }
             else{
                 sendTxt(txtMessage1);
                 }
             break;
           case 'h':
             sendTxt(txtMessage5);
             break;
           case 'c':
             sendTxt(txtMessage8);
             sendTxt(txtMessage9);
             break;  
           default:
             strcat(txtMessage, txtMessage6);
             sendTxt(txtMessage);
             sendTxt(txtMessage5);
             break;
       }
     }
    // Delete message from modem memory
    sms.flush();
    delay(1000);
  }
