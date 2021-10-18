/************************************************************************************
 *  Created By: Tauseef Ahmad
 *  Created On: 16 October, 2021
 *  
 *  YouTube Video: https://youtu.be/DshR6Y9aTSs
 *  My Channel: https://www.youtube.com/channel/UCOXYfOHgu-C-UfGyDcu5sYw/
 *  
 *  *********************************************************************************
 *  Preferences--> Aditional boards Manager URLs : 
 *  For ESP32:
 *  https://dl.espressif.com/dl/package_esp32_index.json
 *  
 *  *********************************************************************************
 *  Download Blynk Library (Version 1.0.1):
 *  https://github.com/blynkkk/blynk-library
 **********************************************************************************/


//------------------------------------------------------------------------------
// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID "TMPLb-x84_kk"
#define BLYNK_DEVICE_NAME "ESP32 Home Automation"
#define BLYNK_AUTH_TOKEN "U-hWVOCBHFWZhXK-GRDJDfx-lwL9_b8D"
//------------------------------------------------------------------------------
// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial
//------------------------------------------------
#include "BluetoothSerial.h"
BluetoothSerial SerialBT;
//------------------------------------------------
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;
//------------------------------------------------
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "OPPO A54";
char pass[] = "cebfa248e1a0";
//------------------------------------------------

BlynkTimer timer;

#define PIN_BUTTON1 26
#define PIN_BUTTON2 25
#define PIN_BUTTON3 33
#define PIN_BUTTON4 32

#define PIN_RELAY1 13
#define PIN_RELAY2 12
#define PIN_RELAY3 14
#define PIN_RELAY4 27

int state_relay1 = 0;
int state_relay2 = 0;
int state_relay3 = 0;
int state_relay4 = 0;


//Change the virtual pins according the rooms
#define VPIN_BUTTON1    V1
#define VPIN_BUTTON2    V2
#define VPIN_BUTTON3    V3 
#define VPIN_BUTTON4    V4

//------------------------------------------------------------------------------
// This function is called every time the device is connected to the Blynk.Cloud
// Request the latest state from the server
BLYNK_CONNECTED() {
  Blynk.syncVirtual(VPIN_BUTTON1);
  Blynk.syncVirtual(VPIN_BUTTON2);
  Blynk.syncVirtual(VPIN_BUTTON3);
  Blynk.syncVirtual(VPIN_BUTTON4);
}

//--------------------------------------------------------------------------
// This function is called every time the Virtual Pin state change
//i.e when you push switch from Blynk App or Web Dashboard
BLYNK_WRITE(VPIN_BUTTON1) {
  state_relay1 = param.asInt();
  digitalWrite(PIN_RELAY1, state_relay1);
  Serial.println("BLYNK_WRITE: Relay1 State = "+String(state_relay1));
}
//--------------------------------------------------------------------------
BLYNK_WRITE(VPIN_BUTTON2) {
  state_relay2 = param.asInt();
  digitalWrite(PIN_RELAY2, state_relay2);
}
//--------------------------------------------------------------------------
BLYNK_WRITE(VPIN_BUTTON3) {
  state_relay3 = param.asInt();
  digitalWrite(PIN_RELAY3, state_relay3);
}
//--------------------------------------------------------------------------
BLYNK_WRITE(VPIN_BUTTON4) {
  state_relay4 = param.asInt();
  digitalWrite(PIN_RELAY4, state_relay4);
}
//--------------------------------------------------------------------------

/*MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
 * setup() function
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM*/
void setup()
{
  // Debug console
  Serial.begin(115200);
  //--------------------------------------------------------------------
  pinMode(PIN_BUTTON1, INPUT_PULLUP);
  pinMode(PIN_BUTTON2, INPUT_PULLUP);
  pinMode(PIN_BUTTON3, INPUT_PULLUP);
  pinMode(PIN_BUTTON4, INPUT_PULLUP);  
  //--------------------------------------------------------------------
  pinMode(PIN_RELAY1, OUTPUT);
  pinMode(PIN_RELAY2, OUTPUT);
  pinMode(PIN_RELAY3, OUTPUT);
  pinMode(PIN_RELAY4, OUTPUT);
  //--------------------------------------------------------------------
  //During Starting all Relays should TURN OFF
  digitalWrite(PIN_RELAY1, HIGH);
  digitalWrite(PIN_RELAY2, HIGH);
  digitalWrite(PIN_RELAY3, HIGH);
  digitalWrite(PIN_RELAY4, HIGH);
  //--------------------------------------------------------------------
  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
  //--------------------------------------------------------------------
  Blynk.virtualWrite(VPIN_BUTTON1, state_relay1);
  Blynk.virtualWrite(VPIN_BUTTON2, state_relay2);
  Blynk.virtualWrite(VPIN_BUTTON3, state_relay3);
  Blynk.virtualWrite(VPIN_BUTTON4, state_relay4);
  //--------------------------------------------------------------------
  if(!SerialBT.begin("ESP32")){
    Serial.println("An error occurred initializing Bluetooth");
  }else{
    Serial.println("Bluetooth initialized");
  }
  //--------------------------------------------------------------------
}


/*MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
 * loop() function
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM*/
void loop()
{
  Blynk.run();
  timer.run();
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
  
  listen_push_buttons();

  handle_bt();
}

/*MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
 * listen_push_buttons() function
 * checks if a push button is pressed or not, and control the relay according to the
 * button state
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM*/
void listen_push_buttons(){
    //--------------------------------------------------------------------------
    if(digitalRead(PIN_BUTTON1) == LOW){
      delay(200);
      control_relay(1);
      Blynk.virtualWrite(VPIN_BUTTON1, state_relay1); //update button state
    }
    //--------------------------------------------------------------------------
    else if (digitalRead(PIN_BUTTON2) == LOW){
      delay(200);
      control_relay(2);
      Blynk.virtualWrite(VPIN_BUTTON2, state_relay2); //update button state
    }
    //--------------------------------------------------------------------------
    else if (digitalRead(PIN_BUTTON3) == LOW){
      delay(200);
      control_relay(3);
      Blynk.virtualWrite(VPIN_BUTTON3, state_relay3); //update button state
    }
    //--------------------------------------------------------------------------
    else if (digitalRead(PIN_BUTTON4) == LOW){
      delay(200);
      control_relay(4);
      Blynk.virtualWrite(VPIN_BUTTON4, state_relay4); //update button state
    }
    //--------------------------------------------------------------------------
}

//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM




/*MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
 * handle_bt() function
 * handles incomming bluetooth data to control relays
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM*/
void handle_bt(){
  //------------------------------------
  char command;
  if(SerialBT.available()){
    command = SerialBT.read();
    Serial.println(command);
  }
  //------------------------------------
  
  switch(command)
  {
     //******************************************************************************
     //Turn ON - Relay1
     case 'A': 
      digitalWrite(PIN_RELAY1, HIGH);  
      state_relay1 = 1; 
      Serial.println("handle_bt: Relay1 State = "+String(state_relay1)); 
      Blynk.virtualWrite(VPIN_BUTTON1, state_relay1);
      break;
      
    //Turn OFF - Relay1
    case 'a': 
      digitalWrite(PIN_RELAY1, LOW); 
      state_relay1 = 0; 
      Blynk.virtualWrite(VPIN_BUTTON1, state_relay1);
      break;
    //******************************************************************************
    //Turn ON - Relay2
    case 'B': 
      digitalWrite(PIN_RELAY2, HIGH);  
      state_relay2 = 1; 
      Blynk.virtualWrite(VPIN_BUTTON2, state_relay2);
      break;
      
    //Turn OFF - Relay2
    case 'b': 
      digitalWrite(PIN_RELAY2, LOW); 
      state_relay2 = 0; 
      Blynk.virtualWrite(VPIN_BUTTON2, state_relay2);
      break;
    //******************************************************************************
    //Turn ON - Relay3
    case 'C': 
      digitalWrite(PIN_RELAY3, HIGH);  
      state_relay3 = 1; 
      Blynk.virtualWrite(VPIN_BUTTON3, state_relay3);
      break;
      
    //Turn OFF - Relay3
    case 'c': 
      digitalWrite(PIN_RELAY3, LOW); 
      state_relay3 = 0; 
      Blynk.virtualWrite(VPIN_BUTTON3, state_relay3);
      break;
    //******************************************************************************
    //Turn ON - Relay4
    case 'D': 
      digitalWrite(PIN_RELAY4, HIGH);  
      state_relay4 = 1; 
      Blynk.virtualWrite(VPIN_BUTTON4, state_relay4);
      break;
      
    //Turn OFF - Relay4
    case 'd': 
      digitalWrite(PIN_RELAY4, LOW); 
      state_relay4 = 0; 
      Blynk.virtualWrite(VPIN_BUTTON4, state_relay4);
      break;
   //******************************************************************************
   }
}

//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM




/*MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
 * control_relay() function
 * turn on or off the relays
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM*/
void control_relay(int relay){
  //------------------------------------------------
  if(relay == 1){
    state_relay1 = !state_relay1;
    digitalWrite(PIN_RELAY1, state_relay1);
    Serial.println("control_relay: Relay1 State = "+String(state_relay1));
    delay(50);
  }
  //------------------------------------------------
  else if(relay == 2){
    state_relay2 = !state_relay2;
    digitalWrite(PIN_RELAY2, state_relay2);
    delay(50);
  }
  //------------------------------------------------
  else if(relay == 3){
    state_relay3 = !state_relay3;
    digitalWrite(PIN_RELAY3, state_relay3);
    delay(50);
  }
  //------------------------------------------------
  else if(relay == 4){
    state_relay4 = !state_relay4;
    digitalWrite(PIN_RELAY4, state_relay4);
    delay(50);
  }
  //------------------------------------------------
}
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
