/************************** Configuration ***********************************/
#define RXD2 16
#define TXD2 17
#include "config.h"
char segundos[2];
char minutos[2];
char horas [2];

/************************ Example Starts Here *******************************/
// this int will hold the current count for our sketch

#define IO_LOOP_DELAY 5000
unsigned long lastUpdate = 0;
AdafruitIO_Feed *hora = io.feed("hora");
AdafruitIO_Feed *minu = io.feed("minu");
AdafruitIO_Feed *seg = io.feed("seg");
AdafruitIO_Feed *push = io.feed("push");
AdafruitIO_Feed *push2 = io.feed("push2");

void setup() {
  // start the serial connection
  Serial.begin(9600);
  Serial2.begin(9600);
  Serial.print("Connecting to Adafruit IO");
  io.connect();
  
  push->onMessage(handleMessage);
  push2->onMessage(handleMessage2);
  
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println(io.statusText());
  push->get();
  push2->get();
}
void loop() {
  io.run();
//  if(flag==1 && cont==1)
  Serial2.write(0X34);
  Serial2.readBytesUntil(0x3A,segundos,3);
  delay (3000);
  seg -> save(segundos);
  Serial.print("sending -> ");
  Serial.println(segundos);
    

}
void handleMessage(AdafruitIO_Data *data) {
   if (data -> toString()== "On"){Serial2.write(0X31);}
   if (data -> toString()== "OFF"){Serial2.write(0X30);}
    Serial.print("received <- ");
    Serial.println(data->value());
}
void handleMessage2(AdafruitIO_Data *data) { 
   
   if (data -> toString()== "On"){Serial2.write(0X33);}
   if (data -> toString()== "OFF"){Serial2.write(0X32);}
  Serial.print("received <- ");
  Serial.println(data->value());
}
