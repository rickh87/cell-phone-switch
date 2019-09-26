
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
String txtMessage5 = "Incorrect command format: nnnn c<enter>";
String txtMessage6 = " is not a recognized command";

int BUTTON = 0;
int RELAY = 1;
int FLAG = 0;
int numMessages = 0;

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
  Serial1.println("in readSerial");
  delay(300);
  while(Serial1.available() > 0){
        char c = Serial1.read();
        txtMessage += c;
     }
  Serial1.println(txtMessage);
  Serial1.println("out of while loop");    
}

void sendTxt(String txtMsg){
    if(numMessages < 100){
       Serial1.println(txtMsg);
       //sms.beginSMS(senderNumber);
       //sms.print(txtMsg);
       //sms.endSMS();
       numMessages++;
    }    
}

void setup() {
  // initialize serial communications
  Serial1.begin(2400);
  while(!Serial1);
  Serial1.println("Debug port started");
  Serial1.setTimeout(5000);
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
  readFlush();
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
  outMessage = "";
  readSerial();
  Serial1.print("numBytes after readSerial = ");
  Serial1.println(txtMessage.length());
  if( txtMessage.length() == 7){
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
//     if ((txtMessage[0] != '9') != (txtMessage[1] != '5') || (txtMessage[2] != '3') || (txtMessage[3] != '6')){
      Serial1.println(txtMessage.substring(0,4));
      if (txtMessage.substring(0,4) != "9536"){
         sendTxt(txtMessage4);
      }
      else{
          t = txtMessage[5];
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
          if(t == 'r'){
             numMessages = 0;
          }
          if((t != 'n') && (t != 'f') && (t !='s') && (t != 'r')){
             outMessage = txtMessage + txtMessage6;
             sendTxt(outMessage);
          }
      }  
   }
   else{
      if(txtMessage.length() !=0 ){
         sendTxt(txtMessage5);
      }
   }    
// Delete message from modem memory
   // sms.flush();
   txtMessage = "";
   delay(5000);
}
