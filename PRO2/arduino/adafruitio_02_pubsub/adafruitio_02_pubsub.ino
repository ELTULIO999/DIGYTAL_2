/************************** Configuration ***********************************/
#define RXD2 16
#define TXD2 17
#include "config.h"
char timedata[7];
/************************ Example Starts Here *******************************/
// this int will hold the current count for our sketch

#define IO_LOOP_DELAY 5000
unsigned long lastUpdate = 0;
AdafruitIO_Feed *totaldata = io.feed("totaldata");
AdafruitIO_Feed *push = io.feed("push");
AdafruitIO_Feed *push2 = io.feed("push2");

void setup() {
  // start the serial connection
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  Serial.print("Connecting to Adafruit IO");
  io.connect();
  push->onMessage(handleMessage);
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println(io.statusText());
  //push2->get();
}
void loop() {
  io.run();
    Serial.print("sending -> ");
    Serial.println(timedata);
    while (Serial2.available()) {
      Serial.print(char(Serial2.read()));
      
   }   
   delay (2000);
   totaldata-> save(timedata);
}
void handleMessage(AdafruitIO_Data *data) {

  Serial.print("received <- ");
  Serial.println(data->value());

}
