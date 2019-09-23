
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
//#include <MKRGSM.h>
#include <string.h>

// PIN Number: NOTE: the Ting SIM card does not have a PIN number
#define PINNUMBER ""
const char senderNumber[] = "6143163988\n";
char remoteNum[20];  // telephone number to send sms
String txtMessage;
String outMessage;
String txtMessage1 = "Relay is on.";
String txtMessage2 = "Relay is off.";
String txtMessage3 = "Front panel button pressed. ";
String txtMessage4 = "Wrong PIN number.";
String txtMessage6 = " is not a recognized command";
String txtMessage10 = "PINs do not match.";

int BUTTON = 0;
int RELAY = 1;
int FLAG = 0;
int numBytes = 0;
char inChar;

// initialize the library instance
//GSM gsmAccess;
//GSM_SMS sms;

/*
  Read input serial
 */
void readFlush(){
  while (Serial1.available() > 0) {
     char charFlush = Serial1.read();
  }
}
 
int readSerial() {
  numBytes = 0;
  while(Serial1.available()){
      txtMessage = Serial1.readString();
      numBytes++;
   }    
}

void sendTxt(String txtMsg){
    Serial1.println(txtMsg);
    //sms.beginSMS(senderNumber);
    //sms.print(txtMsg);
    //sms.endSMS(); 
}

void setup() {
  // initialize serial communications
  Serial1.begin(9600);
  Serial1.println("Debug port started");
  Serial.setTimeout(5000);
  // connection state
  bool connected = false;

  // Start GSM shield
  /* If your SIM has PIN, pass it as a parameter of begin() in quotes
  while (!connected) {
    if (gsmAccess.begin(PINNUMBER) == GSM_READY) {
        }
    delay(1000);
    }
  */
  pinMode(RELAY, OUTPUT);
  pinMode(BUTTON, INPUT);
  digitalWrite(RELAY, LOW);
  Serial1.println("end of setup");
}

void loop() {
  int c;
  int i;
  char t;
  Serial1.println("in loop");
  // put your main code here, to run repeatedly:
  // check to see if the button is pressed
  if (digitalRead(BUTTON) == 0){
    if(FLAG == 0){
       FLAG = 1;
       digitalWrite(RELAY, HIGH);
       outMessage = txtMessage + txtMessage1;
       sendTxt(outMessage);
       }
    else{
       FLAG = 0;
       digitalWrite(RELAY, LOW);
       outMessage = txtMessage + txtMessage3 + txtMessage2;
       sendTxt(outMessage);
      } 
  }  
  numBytes = 0;
  txtMessage = "";
  outMessage = "";
  readSerial();
  if(numBytes > 0){
     readFlush();
     sendTxt(txtMessage);
     /* If there are any SMSs available()
         if (sms.available()) {   
         // An example of message disposal
         // Any messages starting with # should be discarded
            if (sms.peek() == '#') {
                sms.flush();
            }
         }
         // Read message bytes and print them if in debug mode
         while ((c = sms.read()) != -1) {
              Serial.print((char)c);
            }
          }
     }
*/ 
     if ((txtMessage[0] != '9') != (txtMessage[1] != '5') || (txtMessage[2] != '3') || (txtMessage[3] != '6')){
         sendTxt(txtMessage4);
      }
      else{
         t = txtMessage[5];
      }
      Serial1.print("command ");
      Serial1.println(t);
// switch case statements don't work with characters. It will generate a duplicate case error when compiled. 
// charactes can be acessed as integers get placing single quotes around them.
      
          if (t == 't'){
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
          if(t == 'n'){
             FLAG = 1;
             digitalWrite(RELAY, HIGH);    
             sendTxt(txtMessage1);
          }
          if(t =='f'){
             FLAG = 0;
             digitalWrite(RELAY, LOW);
             sendTxt(txtMessage2);
          }
          if(t == 's'){
            if(FLAG == 0){
                 sendTxt(txtMessage2);
                 }
             else{
                 sendTxt(txtMessage1);
                 }
          }
          if (t == 'h'){
             sendTxt(txtMessage5);
          }
          if(t == 'c'){
             sendTxt(txtMessage8);
             sendTxt(txtMessage9);
          }
          if((t != 't') && (t != 'n') && (t != 'f') && (t !='s') && (t !='h') && (t != 's') && (t !='h') && (t != 'c')){
             outMessage = txtMessage + txtMessage6;
             sendTxt(outMessage);
             sendTxt(txtMessage5);
          }
     }
    // Delete message from modem memory
    // sms.flush();
    readFlush();
    delay(1000);
  }
