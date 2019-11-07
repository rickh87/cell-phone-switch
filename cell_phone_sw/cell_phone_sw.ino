
/*
 Cell phone switch. This program recieves commands to turn a relay on and 
 off via sms messages. The relay controls an AC outlet. The cell phone switch
 also send the status of the outlet (i.e. either it is on or off) via sms
 message. The cell phone switch can send up to 100 messages before it must be
 reset. The realy can also toggle state via a fron panel pushbuttom.
 This program is based on "SMS sender" by Tom Igoe
 SMS sender

 Circuit:
 * MKR GSM 1400 board
 * Antenna
 * SIM card that can send SMS
 * Crydom D1240_B normally closed relay

 created 27 Sept 2019
 by Rick Holmes
*/

// Include the GSM library
#include <MKRGSM.h>
#include <string.h>

// PIN Number: NOTE: the Ting SIM card does not have a PIN number
#define PINNUMBER ""
const char senderNumber[] = "6143163988";
char remoteNum[20];  // telephone number to send sms
String txtMessage = "";
String outMessage = "";
String txtMessage1 = "Relay is on.";
String txtMessage2 = "Relay is off.";
String txtMessage3 = "Front panel button pressed. ";
String txtMessage4 = "Wrong PIN number.";
String txtMessage5 = "Incorrect command format: nnnn c<enter>";
String txtMessage6 = " is not a recognized command";
boolean stringComplete = false;

int BUTTON = 0;
int RELAY = 1;
int FLAG = 0;
int numMessages = 0;

// initialize the library instance
GSM gsmAccess;
GSM_SMS sms;

void sendTxt(String txtMsg){
//    Serial.println(txtMsg);
  
//    if(numMessages < 100){
       sms.beginSMS(senderNumber);
       sms.print(txtMsg);
       sms.endSMS();
       numMessages++;
//    }        
}

void setup() {
  txtMessage.reserve(200);
// connection state
  bool connected = false;
/*  Serial.begin(9600);
  while (!Serial){
    ;
  }
*/
//  Serial.println("serial port started");
  // Start GSM shield
  // If your SIM has PIN, pass it as a parameter of begin() in quotes
  while (!connected) {
    if (gsmAccess.begin(PINNUMBER) == GSM_READY){ 
        connected = true;
        sendTxt("GSM started");
    }
    else{
//       Serial.println("Not connected");
       delay(1000);
    }
  }  
  pinMode(RELAY, OUTPUT);
  pinMode(BUTTON, INPUT);
  digitalWrite(RELAY, HIGH);
  delay(2000);
  digitalWrite(RELAY, LOW);
}

void loop() {
  int c;
  int i;
  char t;
//  Serial.println("in loop");
  // put your main code here, to run repeatedly:
  // check to see if the button is pressed
  if (digitalRead(BUTTON) == 0){
    if(FLAG == 1){
       FLAG = 0;
       digitalWrite(RELAY, LOW);
       outMessage = txtMessage + txtMessage3 + txtMessage1;
       sendTxt(outMessage);
       }
    else{
       FLAG = 1;
       digitalWrite(RELAY, HIGH);
       outMessage = txtMessage + txtMessage3 + txtMessage2;
       sendTxt(outMessage);
      } 
   }  
   // See if there are any SMSs available()
   if (sms.available()) {   
      // An example of message disposal
      // Any messages starting with # should be discarded
      if (sms.peek() == '#') {
         sms.flush();
      }
   }
   
   // Read message bytes
   txtMessage = "";
   while ((c = sms.read()) != -1){
       txtMessage += (char)c;
   }
//   Serial.println(txtMessage);
   if( txtMessage.length() == 6){
      if (txtMessage.substring(0,4) != "9536"){
         sendTxt(txtMessage4);
//         Serial.print("command is ");
//         Serial.println(txtMessage.charAt(5));
      }   
      else{
          t = txtMessage.charAt(5);
          if(t == 'f'){
             FLAG = 1;
             digitalWrite(RELAY, HIGH);    
             sendTxt(txtMessage2);
          }
          if(t =='n'){
             FLAG = 0;
             digitalWrite(RELAY, LOW);
             sendTxt(txtMessage1);
          }
          if(t == 's'){
            if(FLAG == 0){
                 outMessage = txtMessage1 + " number of messages sent: " + (String)numMessages;
                 sendTxt(outMessage);
                 }
             else{
                 outMessage = txtMessage2 + " number of messages sent: " + (String)numMessages;
                 sendTxt(outMessage);
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
   sms.flush();
   delay(500);
}
