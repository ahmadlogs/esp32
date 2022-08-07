 /************************************************************************************
 *  Created By: Tauseef Ahmad
 *  Created On: 4 June, 2022
 *  
 *  YouTube Video: https://youtu.be/pp_zKEZlCD8
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
#define BLYNK_TEMPLATE_ID "ENTER_TEMPLATE_ID"
#define BLYNK_DEVICE_NAME "ENTER_DEVICE_NAME"
#define BLYNK_AUTH_TOKEN "ENTER_AUTH_TOKEN"
//------------------------------------------------------------------------------
//--------------------------------------------------
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial

char auth[] = BLYNK_AUTH_TOKEN;
BlynkTimer timer;
//--------------------------------------------------
// Your WiFi Router's credentials. (or Mobile Hotspot)
// Set password to "" for open networks.
char ssid[] = "ENTER_WIFI_SSID";
char pass[] = "ENTER_WIFI_PASSWORD";
//--------------------------------------------------
#define BUTTON_1 22 
#define BUTTON_2 15
#define BUTTON_3 4
#define BUTTON_4 16
//--------------------------------------------------
#define RELAY_1 27
#define RELAY_2 26
#define RELAY_3 25
#define RELAY_4 33
//--------------------------------------------------
#define BUZZER  23
#define TRIG    12
#define ECHO    13
#define PIR     14 //pir security button pin
#define MQ2     34
#define DHT_PIN 19
#define FLAME   21
//--------------------------------------------------
int STATE_RELAY_1 = 0;
int STATE_RELAY_2 = 0;
int STATE_RELAY_3 = 0;
int STATE_RELAY_4 = 0;
//--------------------------------------------------
//Change the virtual pins according the rooms
#define VPIN_BUTTON_1     V0
#define VPIN_BUTTON_2     V1
#define VPIN_BUTTON_3     V2 
#define VPIN_BUTTON_4     V3
#define VPIN_PIR          V4
#define VPIN_TEMPERATURE  V5
#define VPIN_HUMIDITY     V6
#define VPIN_ULTRASONIC   V7
#define VPIN_MQ2          V8
#define VPIN_FLAME        V9
//------------------------------------------------------
boolean pir_new     = LOW;  // current state of pin
boolean pir_old     = LOW;  // previous state of pin

boolean flame_new   = LOW;  // current state of pin
boolean flame_old   = LOW;  // previous state of pin

int mq2_new         = LOW;  // current state of pin
int mq2_old         = LOW;  // previous state of pin

float temp_new      = LOW;  // current state of pin
float temp_old      = LOW;  // previous state of pin
//------------------------------------------------------
String fire_event = "ENTER_EVENT_CODE";
String gas_event  = "ENTER_EVENT_CODE";
String temp_event = "ENTER_EVENT_CODE";
String pir_event  = "ENTER_EVENT_CODE";
//String fire_event = "FIRE";
//String gas_event  = "GAS";
//String temp_event = "TEMPERATURE";
//String pir_event  = "MOTION";
//------------------------------------------------------
boolean buzzer_state = false;
unsigned long buzzer_timer = 0;
//------------------------------------------------------
bool PIR_BUTTON = 0;
int tank_height = 50; //in centimeter
int mq2_limit   = 55;
int temp_limit  = 50;
//------------------------------------------------------
#include <DHT.h>
#define DHTTYPE DHT11
DHT dht(DHT_PIN, DHTTYPE);
//------------------------------------------------------
#include <IRremote.h>
const byte IR_RECEIVE_PIN = 18;
//------------------------------------------------------
//--------------------------------------------------------------------------
// This function is called every time the device is connected 
//to the Blynk.Cloud Request the latest state from the server
BLYNK_CONNECTED() {
  Blynk.syncVirtual(VPIN_BUTTON_1);
  Blynk.syncVirtual(VPIN_BUTTON_2);
  Blynk.syncVirtual(VPIN_BUTTON_3);
  Blynk.syncVirtual(VPIN_BUTTON_4);
  Blynk.syncVirtual(VPIN_PIR);
  //Blynk.syncAll();
}
//--------------------------------------------------------------------------
// This function is called every time the Virtual Pin state change
//i.e when web push switch from Blynk App or Web Dashboard
BLYNK_WRITE(VPIN_BUTTON_1) {
  STATE_RELAY_1 = param.asInt();
  digitalWrite(RELAY_1, STATE_RELAY_1);
  Serial.print("Relay1 State = "); Serial.println(STATE_RELAY_1);
}
//--------------------------------------------------------------------------
BLYNK_WRITE(VPIN_BUTTON_2) {
  STATE_RELAY_2 = param.asInt();
  digitalWrite(RELAY_2, STATE_RELAY_2);
  Serial.print("Relay2 State = "); Serial.println(STATE_RELAY_2);
}
//--------------------------------------------------------------------------
BLYNK_WRITE(VPIN_BUTTON_3) {
  STATE_RELAY_3 = param.asInt();
  digitalWrite(RELAY_3, STATE_RELAY_3);
  Serial.print("Relay3 State = "); Serial.println(STATE_RELAY_3);
}
//--------------------------------------------------------------------------
BLYNK_WRITE(VPIN_BUTTON_4) {
  STATE_RELAY_4 = param.asInt();
  digitalWrite(RELAY_4, STATE_RELAY_4);
  Serial.print("Relay4 State = "); Serial.println(STATE_RELAY_4);
}
//--------------------------------------------------------------------------
BLYNK_WRITE(VPIN_PIR) {
  PIR_BUTTON = param.asInt();
  Serial.print("PIR Button = "); Serial.println(PIR_BUTTON);
}
//--------------------------------------------------------------------------

/****************************************************************************************************
 * setup() Function
*****************************************************************************************************/
void setup(){
  // Debug console
  Serial.begin(115200);
  //--------------------------------------------------------------------
  pinMode(BUTTON_1, INPUT_PULLUP);
  pinMode(BUTTON_2, INPUT_PULLUP);
  pinMode(BUTTON_3, INPUT_PULLUP);
  pinMode(BUTTON_4, INPUT_PULLUP);
  //--------------------------------------------------------------------
  pinMode(RELAY_1, OUTPUT);
  pinMode(RELAY_2, OUTPUT);
  pinMode(RELAY_3, OUTPUT);
  pinMode(RELAY_4, OUTPUT);
  //--------------------------------------------------------------------
  //During Starting all Relays should TURN OFF
  digitalWrite(RELAY_1, HIGH);
  digitalWrite(RELAY_2, HIGH);
  digitalWrite(RELAY_3, HIGH);
  digitalWrite(RELAY_4, HIGH);
  //--------------------------------------------------------------------
  pinMode(FLAME, INPUT);
  pinMode(PIR, INPUT);
  pinMode(ECHO, INPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  //--------------------------------------------------------------------
  dht.begin();
  //--------------------------------------------------------------------
  IrReceiver.begin(IR_RECEIVE_PIN); // Start the IR receiver
  //--------------------------------------------------------------------
  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
  //--------------------------------------------------------------------
  //Blynk.virtualWrite(VPIN_BUTTON_1, STATE_RELAY_1);
  //Blynk.virtualWrite(VPIN_BUTTON_2, STATE_RELAY_2);
  //Blynk.virtualWrite(VPIN_BUTTON_3, STATE_RELAY_3);
  //Blynk.virtualWrite(VPIN_BUTTON_4, STATE_RELAY_4);
  //--------------------------------------------------------------------
  timer.setInterval(100L, MQ2GasSensor);
  timer.setInterval(100L, DHT11Sensor);
  timer.setInterval(100L, PirSensor);
  timer.setInterval(100L, UltrasonicSensor);
  timer.setInterval(100L, FlameSensor);
}

/****************************************************************************************************
 * loop() Function
*****************************************************************************************************/
void loop() {
  Blynk.run();
  timer.run();
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
  
  listen_push_buttons();
  listen_ir();
  
  if (buzzer_state == true) {
    if (millis() - buzzer_timer > 5000) {
      digitalWrite(BUZZER, LOW);
      buzzer_state = false;
      buzzer_timer = 0;
    }
  }
  
}

/****************************************************************************************************
 * DHT11Sensor Function
*****************************************************************************************************/
void DHT11Sensor() {
  //-----------------------------------------------------------------------
  //Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  //-----------------------------------------------------------------------
  //Read temperature as Celsius (the default)
  temp_old = temp_new;
  temp_new = dht.readTemperature();
  //-----------------------------------------------------------------------
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(temp_new)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  //-----------------------------------------------------------------------
  //Serial.print("Temperature: "); Serial.println(temp_new);
  //Serial.print("Humidity: "); Serial.println(h);
  //Serial.println("------------------------------------------");
  //-----------------------------------------------------------------------
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(VPIN_TEMPERATURE, temp_new);
  Blynk.virtualWrite(VPIN_HUMIDITY, h);
  //-----------------------------------------------------------------------
  //Serial.print(String(temp_old)+"-");Serial.println(temp_new);
  if(temp_old < temp_limit && temp_new >= temp_limit) { //LOW to HIGH
    String text = "Temperature is very high";
    Serial.println(text);
    Blynk.logEvent(temp_event, text);
    digitalWrite(BUZZER, HIGH);
    buzzer_state = true;
    buzzer_timer = millis();
  }
}

/****************************************************************************************************
 * PirSensor Function
*****************************************************************************************************/
void PirSensor() {
  if (PIR_BUTTON == 1) {
    pir_old = pir_new; // store old state
    pir_new = digitalRead(PIR); //read new state
    //------------------------------------------------------------------------
    if(pir_old == LOW && pir_new == HIGH) {
      String text = "Motion is Detected.";
      Serial.println(text);
      Blynk.logEvent(pir_event, text);
      digitalWrite(BUZZER, HIGH);
      buzzer_state = true;
      buzzer_timer = millis();
    }
    //------------------------------------------------------------------------
    //else if(pir_old == HIGH && pir_new == LOW) { //HIGH to LOW
      //Serial.println("Motion stopped!");
      //digitalWrite(BUZZER, LOW);
    //}
    //------------------------------------------------------------------------
  }
}

/****************************************************************************************************
 * MQ2GasSensor Function
*****************************************************************************************************/
void MQ2GasSensor() {
  mq2_old = mq2_new; // store old state
  mq2_new = analogRead(MQ2); //read new state
  mq2_new = map(mq2_new, 0, 4095, 0, 100);
  //Serial.print("MQ2GasSensor Value: "); Serial.println(mq2_new);

  //--------------------------------------------------------------
  if(mq2_old < mq2_limit && mq2_new >= mq2_limit) { //LOW to HIGH
    String text = "Gas leakage is detected";
    Serial.println(text);
    Blynk.logEvent(gas_event, text);
    digitalWrite(BUZZER, HIGH);
    buzzer_state = true;
    buzzer_timer = millis();
  }
  //--------------------------------------------------------------
  //else if(mq2_old > mq2_limit && mq2_new <= mq2_limit) { //HIGH to LOW
    //Serial.println("Fire stopped!");
    //digitalWrite(BUZZER, LOW);
  //}
  //--------------------------------------------------------------
  Blynk.virtualWrite(VPIN_MQ2, mq2_new);
}

/****************************************************************************************************
 * FlameSensor Function
*****************************************************************************************************/
void FlameSensor() {
  //--------------------------------------------------------------
  flame_old = flame_new; // store old state
  flame_new = digitalRead(FLAME); //read new state
  //--------------------------------------------------------------
  if(flame_old == HIGH && flame_new == LOW) { //HIGH to LOW
    String text = "Fire is detected";
    Serial.println(text);
    Blynk.logEvent(fire_event, text);
    digitalWrite(BUZZER, HIGH);
    buzzer_state = true;
    buzzer_timer = millis();
  }
  //--------------------------------------------------------------
  //else if(flame_old == LOW && flame_new == HIGH) { //LOW to HIGH
    //Serial.println("Fire stopped!");
    //digitalWrite(BUZZER, LOW);
  //}
  //--------------------------------------------------------------
  Blynk.virtualWrite(VPIN_FLAME, !flame_new);
  //--------------------------------------------------------------
}

/****************************************************************************************************
 * UltrasonicSensor Function
*****************************************************************************************************/
void UltrasonicSensor() {
  //--------------------------------------------------
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  //--------------------------------------------------
  long t = pulseIn(ECHO, HIGH);
  long cm = (t * 0.034) / 2; //SOUND_SPEED = 0.034 i.e. 343m/s
  //--------------------------------------------------
  if(cm <= tank_height)
    {Blynk.virtualWrite(VPIN_ULTRASONIC, tank_height-cm);}
  //--------------------------------------------------
  //Serial.print("Water Level:"); Serial.println(cm);
  //--------------------------------------------------
}

/****************************************************************************************************
 * listen_push_buttons() function
*****************************************************************************************************/
void listen_push_buttons(){
    //-----------------------------------------------------
    if(digitalRead(BUTTON_1) == LOW)
      {ControlRelay(1, RELAY_1, STATE_RELAY_1, VPIN_BUTTON_1);}
    //-----------------------------------------------------
    else if (digitalRead(BUTTON_2) == LOW)
      {ControlRelay(2, RELAY_2, STATE_RELAY_2, VPIN_BUTTON_2);}
    //-----------------------------------------------------
    else if (digitalRead(BUTTON_3) == LOW)
      {ControlRelay(3, RELAY_3, STATE_RELAY_3, VPIN_BUTTON_3);}
    //-----------------------------------------------------
    else if (digitalRead(BUTTON_4) == LOW)
      {ControlRelay(4, RELAY_4, STATE_RELAY_4, VPIN_BUTTON_4);}
    //-----------------------------------------------------
}

/****************************************************************************************************
 * ControlRelay Function
*****************************************************************************************************/
void ControlRelay(int number, int relay_pin, int &status, int virtual_pin){
    delay(200);
    status = !status;
    digitalWrite(relay_pin, status);
    delay(50);
    Blynk.virtualWrite(virtual_pin, status); //update button state
    Serial.print("Relay"+String(number)+" State = "); Serial.println(status);
}

/****************************************************************************************************
 * listen_ir Function
*****************************************************************************************************/
void listen_ir()
{
   if (IrReceiver.decode()) {
      String ir_code = String(IrReceiver.decodedIRData.command, HEX);
      if(ir_code.equals("0")) {IrReceiver.resume();return; }
      
      Serial.println(ir_code);
      
      if     (ir_code == "c")
        {ControlRelay(1, RELAY_1, STATE_RELAY_1, VPIN_BUTTON_1);}
      else if(ir_code == "18")
        {ControlRelay(2, RELAY_2, STATE_RELAY_2, VPIN_BUTTON_2);}
      else if(ir_code == "5e")
        {ControlRelay(3, RELAY_3, STATE_RELAY_3, VPIN_BUTTON_3);}
      else if(ir_code == "8")
        {ControlRelay(4, RELAY_4, STATE_RELAY_4, VPIN_BUTTON_4);}
      else if(ir_code == "d"){
        PIR_BUTTON = !PIR_BUTTON;
        Blynk.virtualWrite(VPIN_PIR, PIR_BUTTON); //update button state
        Serial.print("Security Button State = "); Serial.println(PIR_BUTTON);
      }
        
      IrReceiver.resume();
   }
}
