/************************************************************************************
 *  Created By: Tauseef Ahmad
 *  Created On: 10 November, 2021
 *  
 *  YouTube Video: 
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
 *  *********************************************************************************
 *  Download TinyGPS++ Library (Version 1.0.2b):
 *  https://github.com/mikalhart/TinyGPSPlus
 **********************************************************************************/


//------------------------------------------------------------------------------
// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID "ENTER_TEMPLATE_ID"
#define BLYNK_DEVICE_NAME "ENTER_DEVICE_NAME"
#define BLYNK_AUTH_TOKEN "ENTER_AUTH_TOKEN"
//------------------------------------------------------------------------------

char auth[] = BLYNK_AUTH_TOKEN;
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "ENTER_WIFI_SSID";
char pass[] = "ENTER_WIFI_PASSWORD";
//------------------------------------------------------------------------------
// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial

//------------------------------------------------------------------------------
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

//------------------------------------------------------------------------------

//GPS Module Settings
//GPS Module RX pin to ESP32 17
//GPS Module TX pin to ESP32 16
#include <TinyGPS++.h> 
#define RXD2 16
#define TXD2 17
HardwareSerial neogps(2);
TinyGPSPlus gps;
//------------------------------------------------------------------------------

BlynkTimer timer;

#define INTERVAL 1000L
/************************************************************************************
 *  This function sends Arduino's up time every second to Virtual Pin.
 *  In the app, Widget's reading frequency should be set to PUSH. This means
 *  that you define how often to send data to Blynk App.
 **********************************************************************************/
void sendGps()
{
  //-----------------------------------------------------------
  while(neogps.available())
  {
    if (gps.encode(neogps.read()))
    {
      break;
    }
  }
  //-----------------------------------------------------------
  if (!gps.location.isValid())
  {
    Serial.println("Failed to read from GPS Module!");
    return;
  }
  //-----------------------------------------------------------
  //get latitude and longitude
  float latitude = gps.location.lat();
  float longitude = gps.location.lng();
  float speed = gps.speed.kmph();
  //-----------------------------------------------------------
  //comment out this block of code to save space
  //used for debugging in serial monitor
  Serial.print("Latitude:  ");
  Serial.println(latitude, 6);
  Serial.print("Longitude: ");
  Serial.println(longitude, 6);
  Serial.print("Speed: ");
  Serial.println(speed, 6);
  //-----------------------------------------------------------
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V1, String(latitude, 6));
  Blynk.virtualWrite(V2, String(longitude, 6));
  Blynk.virtualWrite(V0, String(speed));
  //-----------------------------------------------------------
}



/************************************************************************************
 *  setup() function
 **********************************************************************************/
void setup()
{
  //-----------------------------------------------------------
  //Debug console (Serial Monitor)
  Serial.begin(115200);
  //-----------------------------------------------------------
  Blynk.begin(auth, ssid, pass);
  //You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
  //-----------------------------------------------------------
  //Set GPS module baud rate
  neogps.begin(9600, SERIAL_8N1, RXD2, TXD2);
  Serial.println("neogps serial initialize");
  delay(10);
  //-----------------------------------------------------------
  // Setup a function to be called every second
  timer.setInterval(INTERVAL, sendGps);
  //-----------------------------------------------------------
}


/************************************************************************************
 *  loop() function
 **********************************************************************************/
void loop()
{
  Blynk.run();
  timer.run();
}
