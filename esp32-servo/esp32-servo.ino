/* ------------------------------------------------------------------------
 * Created by: Tauseef Ahmad
 * Created on: 22 March, 2023
 *  
 * Tutorial: https://youtu.be/xqxB0rRrodU
 * ------------------------------------------------------------------------
 * Download Resources
 * ------------------------------------------------------------------------
 * Preferences--> Aditional boards Manager URLs : 
 * For ESP32 - Version 2.0.6
 * https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
 * ------------------------------------------------------------------------
 * Install ESP32Servo Library 0.12.1
 * ------------------------------------------------------------------------*/

//---------------------------------------------------
#include "RMaker.h"
#include "WiFi.h"
#include "WiFiProv.h"
//---------------------------------------------------
#include <Servo.h>

Servo myservo;
int servoPin = 13;
//---------------------------------------------------
const char *service_name = "Ahmad_BLE";
const char *pop = "12345678";
//---------------------------------------------------
static uint8_t WIFI_LED    = 2;   //D2
static uint8_t gpio_reset = 0;
//---------------------------------------------------
//Declaring Devices
//The framework provides some standard device types 
//like switch, lightbulb, fan, temperature sensor.
static Fan my_fan("Fan");
#define DEFAULT_DIMMER_LEVEL 0

/****************************************************************************************************
 * sysProvEvent Function
*****************************************************************************************************/
void sysProvEvent(arduino_event_t *sys_event)
{
    switch (sys_event->event_id) {      
        case ARDUINO_EVENT_PROV_START:
#if CONFIG_IDF_TARGET_ESP32
        Serial.printf("\nProvisioning Started with name \"%s\" and PoP \"%s\" on BLE\n", service_name, pop);
        printQR(service_name, pop, "ble");
#else
        Serial.printf("\nProvisioning Started with name \"%s\" and PoP \"%s\" on SoftAP\n", service_name, pop);
        printQR(service_name, pop, "softap");
#endif        
        break;
        case ARDUINO_EVENT_WIFI_STA_CONNECTED:
        Serial.printf("\nConnected to Wi-Fi!\n");
        digitalWrite(WIFI_LED, HIGH);
        break;
    }
}

/****************************************************************************************************
 * write_callback Function
*****************************************************************************************************/
void write_callback(Device *device, Param *param, const param_val_t val, void *priv_data, write_ctx_t *ctx)
{
    const char *device_name = device->getDeviceName();
    const char *param_name = param->getParamName();
    //----------------------------------------------------------------------------------
    if(strcmp(device_name, "Fan") == 0) {
      //Serial.printf("Fan = %s\n", val.val.b? "true" : "false");
      if(strcmp(param_name, "Power") == 0) {
        //int fan_on_off_value = val.val.b;
        //Serial.printf("\nReceived value = %d for %s - %s\n", fan_on_off_value, device_name, param_name);
        //param->updateAndReport(val);
      } else if (strcmp(param_name, "Level") == 0) {
        int pos = val.val.i;
        Serial.printf("\nReceived value = %d for %s - %s\n", pos, device_name, param_name);
        myservo.write(pos);
        delay(15); // waits 15ms for the servo to reach the position
        //param->updateAndReport(val);
      }
    }
    //---------------------------------------------------------------------------------- 
}

/****************************************************************************************************
 * setup Function
*****************************************************************************************************/
void setup(){
  //------------------------------------------------------------------------------
  uint32_t chipId = 0;
  Serial.begin(115200);
  //------------------------------------------------------------------------------
  myservo.attach(servoPin);
  //------------------------------------------------------------------------------
  pinMode(gpio_reset, INPUT);
  pinMode(WIFI_LED, OUTPUT);
  digitalWrite(WIFI_LED, LOW);
  //------------------------------------------------------------------------------
  Node my_node;    
  my_node = RMaker.initNode("Room 1");
  
  //Standard switch device. Add switch device to the node
  Param level_param("Level", "custom.param.level", value(DEFAULT_DIMMER_LEVEL), PROP_FLAG_READ | PROP_FLAG_WRITE);
  level_param.addBounds(value(0), value(180), value(1)); //sart_value, end_value, interval
  level_param.addUIType(ESP_RMAKER_UI_SLIDER);
  my_fan.addParam(level_param);
  
  my_fan.addCb(write_callback);
  
  my_node.addDevice(my_fan);
  my_fan.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, 0);
  
  //------------------------------------------------------------------------------
  //This is optional 
  RMaker.enableOTA(OTA_USING_PARAMS);
  //If you want to enable scheduling, set time zone for your region using setTimeZone(). 
  //The list of available values are provided here https://rainmaker.espressif.com/docs/time-service.html
  // RMaker.setTimeZone("Asia/Shanghai");
  // Alternatively, enable the Timezone service and let the phone apps set the appropriate timezone
  RMaker.enableTZService();
  RMaker.enableSchedule();
  //------------------------------------------------------------------------------
  //Service Name
  for(int i=0; i<17; i=i+8) {
    chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
  }

  Serial.printf("\nChip ID:  %d Service Name: %s\n", chipId, service_name);
  //------------------------------------------------------------------------------
  Serial.printf("\nStarting ESP-RainMaker\n");
  RMaker.start();
  //------------------------------------------------------------------------------
  WiFi.onEvent(sysProvEvent);
  #if CONFIG_IDF_TARGET_ESP32
    WiFiProv.beginProvision(WIFI_PROV_SCHEME_BLE, WIFI_PROV_SCHEME_HANDLER_FREE_BTDM, WIFI_PROV_SECURITY_1, pop, service_name);
  #else
    WiFiProv.beginProvision(WIFI_PROV_SCHEME_SOFTAP, WIFI_PROV_SCHEME_HANDLER_NONE, WIFI_PROV_SECURITY_1, pop, service_name);
  #endif
  //------------------------------------------------------------------------------
}

/****************************************************************************************************
 * loop Function
*****************************************************************************************************/
void loop()
{
  //------------------------------------------------------------------------------
  // Read GPIO0 (external button to reset device
  if(digitalRead(gpio_reset) == LOW) { //Push button pressed
    Serial.printf("Reset Button Pressed!\n");
    // Key debounce handling
    delay(100);
    int startTime = millis();
    while(digitalRead(gpio_reset) == LOW) delay(50);
    int endTime = millis();
    //_______________________________________________________________________
    if ((endTime - startTime) > 10000) {
      // If key pressed for more than 10secs, reset all
      Serial.printf("Reset to factory.\n");
      RMakerFactoryReset(2);
    } 
    //_______________________________________________________________________
    else if ((endTime - startTime) > 3000) {
      Serial.printf("Reset Wi-Fi.\n");
      // If key pressed for more than 3secs, but less than 10, reset Wi-Fi
      RMakerWiFiReset(2);
    }
    //_______________________________________________________________________
  }
  //------------------------------------------------------------------------------
  delay(100);
  
  if (WiFi.status() != WL_CONNECTED){
    //Serial.println("WiFi Not Connected");
    digitalWrite(WIFI_LED, LOW);
  }
  else{
    //Serial.println("WiFi Connected");
    digitalWrite(WIFI_LED, HIGH);
  }
  //------------------------------------------------------------------------------
}
