/*************************************************************************************
 *  Created By: Tauseef Ahmad
 *  Created On: 28 July, 2022
 *  
 *  YouTube Video: https://youtu.be/inVhQaKLdlk
 *  My Channel: https://www.youtube.com/channel/UCOXYfOHgu-C-UfGyDcu5sYw/
 *  
 *  *********************************************************************************
 *  Preferences--> Aditional boards Manager URLs : 
 *  For ESP32:
 *  https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
 *  Download Board ESP32 (2.0.3) : 
 *  *********************************************************************************
 *  Install the following libraries :
 *  1. ezButton
 *  2. IRremote
 ***********************************************************************************/

#include <IRremote.h>
const byte IR_RECEIVE_PIN = 14;

void setup(){
  Serial.begin(115200);
  IrReceiver.begin(IR_RECEIVE_PIN); // Start the IR receiver
}
void loop(){
  if (IrReceiver.decode()) {
  String ir_code = String(IrReceiver.decodedIRData.command, HEX);
  if(ir_code.equals("0")) {IrReceiver.resume();return; }
  
  Serial.println(ir_code);
  IrReceiver.resume();
  }
  delay(1000);
}
