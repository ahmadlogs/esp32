/************************************************************************************
 *  Created By: Tauseef Ahmad
 *  Created On: October 23, 2021
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
 
//----------------------------
#include <WiFi.h>
#include <HTTPClient.h>
//----------------------------

//--------------------------------------------
#include <TinyGPS++.h>
TinyGPSPlus gps;

#define RXD2 16
#define TXD2 17
HardwareSerial neogps(2);
//--------------------------------------------

//---------------------------------------------------------------------
//
//
//ENTER_GOOGLE_DEPLOYMENT_ID
const char * ssid = "ENTER_YOUR_WIFI_SSID";
const char * password = "ENTER_YOUR_WIFI_PASSWORD";
String GOOGLE_SCRIPT_ID = "ENTER_GOOGLE_DEPLOYMENT_ID";
//---------------------------------------------------------------------

const int sendInterval = 2000;

/************************************************************************************
 *  setup function starts
 **********************************************************************************/
 void setup() {
  //--------------------------------------------
  Serial.begin(115200);
  //start serial communication with Serial Monitor
  //--------------------------------------------
  //start serial communication with Neo6mGPS
  neogps.begin(9600, SERIAL_8N1, RXD2, TXD2);
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
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM



/************************************************************************************
 *  loop function starts
 **********************************************************************************/
void loop() {
    
  boolean newData = false;
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (neogps.available())
    {
      if (gps.encode(neogps.read()))
        {newData = true;}
    }
  }

  //If newData is true
  if(newData == true)
  {
    newData = false;
    Serial.println(gps.satellites.value());
    print_speed();
  }
  else
  {
    Serial.println("No new data is received.");
  }  

  //delay(sendInterval);
}
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM




/************************************************************************************
 *  loop function starts
 **********************************************************************************/
void print_speed()
{
  if (gps.location.isValid() == 1)
  {
    //String gps_speed = String(gps.speed.kmph());
    Serial.println(gps.location.lat(),6);
    Serial.println(gps.location.lng(),6);
    Serial.println(gps.speed.kmph());
    Serial.println(gps.satellites.value());
    Serial.println(gps.altitude.meters(), 0);

    String param;
    param  = "latitude="+String(gps.location.lat(), 6);
    param += "&longitude="+String(gps.location.lng(), 6);
    param += "&speed="+String(gps.speed.kmph());
    param += "&satellites="+String(gps.satellites.value());
    param += "&altitude="+String(gps.altitude.meters());
    param += "&gps_time="+String(gps.time.value());
    param += "&gps_date="+String(gps.date.value());

    Serial.println(param);
    write_to_google_sheet(param);
  }
  else
  {
    Serial.println("No any valid GPS data.");
  }  

}
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM




/************************************************************************************
 *  loop function starts
 **********************************************************************************/
void write_to_google_sheet(String params) {
   HTTPClient http;
   String url="https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID+"/exec?"+params;
   //Serial.print(url);
    Serial.println("Postring GPS data to Google Sheet");
    //---------------------------------------------------------------------
    //starts posting data to google sheet
    http.begin(url.c_str());
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    int httpCode = http.GET();  
    Serial.print("HTTP Status Code: ");
    Serial.println(httpCode);
    //---------------------------------------------------------------------
    //getting response from google sheet
    String payload;
    if (httpCode > 0) {
        payload = http.getString();
        Serial.println("Payload: "+payload);     
    }
    //---------------------------------------------------------------------
    http.end();
}
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
