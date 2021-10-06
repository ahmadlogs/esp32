/************************************************************************************
 *  Created By: Tauseef Ahmad
 *  Created On: 30 September, 2021
 *  
 *  Tutorial Video: https://youtu.be/yem5EysVloc
 *  My Channel: https://www.youtube.com/channel/UCOXYfOHgu-C-UfGyDcu5sYw/
 *  
 *  *********************************************************************************
 *  Preferences--> Aditional boards Manager URLs : 
 *  For ESP32:
 *  https://dl.espressif.com/dl/package_esp32_index.json
 *  
 **********************************************************************************/
 
 #include <WiFi.h>
#include <HTTPClient.h>

//---------------------------------------------------------------------
const char * ssid = "ENTER_YOUR_WIFI_SSID";
const char * password = "ENTER_YOUR_WIFI_PASSWORD";
String GOOGLE_SCRIPT_ID = "ENTER_GOOGLE_DEPLOYMENT_ID";
//---------------------------------------------------------------------

//-----------------------------
#define relay1_pin 13
#define relay2_pin 12
#define relay3_pin 14
#define relay4_pin 27
//-----------------------------

const int sendInterval = 2000;

WiFiClientSecure client;

//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
void setup() {
  //--------------------------------------------
  pinMode(relay1_pin, OUTPUT);
  pinMode(relay2_pin, OUTPUT);
  pinMode(relay3_pin, OUTPUT);
  pinMode(relay4_pin, OUTPUT);
  //--------------------------------------------
  Serial.begin(115200);
  delay(10);
  //--------------------------------------------
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("OK");
  //--------------------------------------------
}
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM





//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
void loop() {
  
  read_google_sheet();
  
  delay(sendInterval);
}
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM




//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
void read_google_sheet(void) {
   //-----------------------------------------------------------------------------------
   HTTPClient http;
   String url="https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID+"/exec?read";
  //Serial.print(url);
  Serial.println("Reading Data From Google Sheet.....");
  http.begin(url.c_str());
  //-----------------------------------------------------------------------------------
  //Remove this error "302 Moved Temporarily Error"
  http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
  //-----------------------------------------------------------------------------------
  //Get the returning HTTP status code
  int httpCode = http.GET();
  Serial.print("HTTP Status Code: ");
  Serial.println(httpCode);
  //-----------------------------------------------------------------------------------
  if(httpCode <= 0){Serial.println("Error on HTTP request"); http.end(); return;}
  //-----------------------------------------------------------------------------------
  //reading data comming from Google Sheet
  String payload = http.getString();
  Serial.println("Payload: "+payload);
  //-----------------------------------------------------------------------------------
  if(httpCode == 200){
    //=====================================================================
    //get relay number from payload variable
    String temp = payload.substring(0, 1);
    int relay_number = temp.toInt();
    //=====================================================================
    //get the command comming from Google Sheet
    //i.e ON or OFF
    payload.remove(0, 1);
    payload.toLowerCase();
    payload.trim();
    Serial.println("Payload - Command: "+payload);
    //=====================================================================
    if(payload != "on" && payload != "off")
      {Serial.println("Invalid Command"); http.end(); return;}
    //=====================================================================
    if(relay_number < 1 || relay_number > 4)
      {Serial.println("Invalid Relay Number"); http.end(); return;}
    //=====================================================================
    int relay_state = control_relay(relay_number, payload);
    write_google_sheet( "relay_number="+String(relay_number)+"&relay_state="+String(relay_state) );
    //=====================================================================
  }
  //-------------------------------------------------------------------------------------
  http.end();
}
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM





//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
void write_google_sheet(String params) {
   HTTPClient http;
   String url="https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID+"/exec?"+params;
   //Serial.print(url);
    Serial.println("Updating Status of Relay");
    http.begin(url.c_str());
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    int httpCode = http.GET();  
    Serial.print("HTTP Status Code: ");
    Serial.println(httpCode);
    
    String payload;
    if (httpCode > 0) {
        payload = http.getString();
        Serial.println("Payload: "+payload);     
    }
    http.end();
}
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM





//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
int control_relay(int relay, String command){
  switch (relay) {
    //------------------------------------------------
    case 1:
      if(command == "on"){
        digitalWrite(relay1_pin, HIGH);
        Serial.println("Relay 1 is ON");
        return 1;
      } else {
        digitalWrite(relay1_pin, LOW);
        Serial.println("Relay 1 is OFF");
        return 0;
      }
      break;
    //------------------------------------------------
    case 2:
      if(command == "on"){
        digitalWrite(relay2_pin, HIGH);
        return 1;
      } else {
        digitalWrite(relay2_pin, LOW);
        return 0;
      }
      break;
    //------------------------------------------------
    case 3:
      if(command == "on"){
        digitalWrite(relay3_pin, HIGH);
        return 1;
      } else {
        digitalWrite(relay3_pin, LOW);
        return 0;
      }
      break;
    //------------------------------------------------
    case 4:
      if(command == "on"){
        digitalWrite(relay4_pin, HIGH);
        return 1;
      } else {
        digitalWrite(relay4_pin, LOW);
        return 0;
      }
      break;
    //------------------------------------------------    
    default:
      return -1;
      break;
  }
}
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
