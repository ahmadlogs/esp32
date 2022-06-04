#include <IRremote.h>
const byte IR_RECEIVE_PIN = 18;

void setup(){
  Serial.begin(115200);
  Serial.println("Press Remote keys to get Hex code...");
  IrReceiver.begin(IR_RECEIVE_PIN); // Start the IR receiver
}

void loop() { 
  if (IrReceiver.decode()) {
    String ir_code = String(IrReceiver.decodedIRData.command, HEX);
    if(ir_code.equals("0")) {IrReceiver.resume();return; }
    Serial.println(ir_code);
  }
  IrReceiver.resume();  delay(1000);
}
