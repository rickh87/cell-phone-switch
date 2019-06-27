
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
char txtMessage4[] = "Wrong PIN number DUMBASS!";
char txtMessage5[] = "Available commands are: T/t (toggle), N/n (on), F/f (off), S/s (status), C/c (Change PIN), H/h (Help)";
char txtMessage6[] = " is not a recognized command, DIPSHIT!";
char txtMessage7[] = "PIN can only be changed by the Master";
char txtMessage8[] = "Enter new 4 digit PIN";
char txtMessage9[] = "Reenter 4 digit PIN";
char txtMessage10[] = "PINs do not match. Go Away!";

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
  Serial.begin(9600);
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
        if (debug){
           Serial.println("Cnnected");
        }   
    } else {
        if (debug){
            Serial.println("Not connected");
        }
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
  int t;
  // put your main code here, to run repeatedly:
  // check to see if the button is pressed
  if (digitalRead(BUTTON) == 0){
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
      strcat(txtMessage, txtMessage2);
      sendTxt(txtMessage);
      }
  }  
   
  if (debug){
      Serial.print("Now enter test message content: ");
      readSerial(txtMessage);
      sendTxt(txtMessage);
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
    while ((c = sms.read()) != -1) {
      if(debug){
         Serial.print((char)c);
      }
    }
    
    if ((txtMessage[0] != '9') || (txtMessage[1] != '5') || (txtMessage[2] != '3') || (txtMessage[3] != '6')){
       sendTxt(txtMessage4);
       }
    else{
       t = (int) txtMessage[4];
       switch (t){
//          case ('T' || 't'): 
            case 'T':
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
//          case ('N' || 'n'):
            case 'N':
             FLAG = 1;
             digitalWrite(RELAY, HIGH);    
             sendTxt(txtMessage1);
             break;
//          case ('F' || 'f');
            case 'F':
             FLAG = 0;
             digitalWrite(RELAY, LOW);
             sendTxt(txtMessage2);
             break;
//          case ('S' || 's'): 
            case 'S':
            if(FLAG == 0){
                 sendTxt(txtMessage2);
                 }
             else{
                 sendTxt(txtMessage1);
                 }
             break;
//           case ('H' || 'h' || '?'):
             case 'H':
             sendTxt(txtMessage5);
             break;
//           case ('C' || 'c'):
             case 'C':
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
  }
  delay(1000);
}
