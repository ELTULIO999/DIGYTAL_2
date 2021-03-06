/************************** Configuration ***********************************/
#define RXD2 16
#define TXD2 17
#include "config.h"
char segundos[2];
char minutos[2];
char horas [2];

int flag;

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
  Serial2.write(0X34);
  Serial2.readBytesUntil(0x3A,segundos,3);
  delay (2200);
  seg -> save(segundos);
  Serial.print("sending_s -> ");
  Serial.println(segundos);

  Serial2.write(0X35);
    Serial2.readBytesUntil(0x3A,minutos,3);
  delay (2200);
  minu -> save(minutos);
  Serial.print("sending_m -> ");
  Serial.println(minutos);
  
  Serial2.write(0X36);
  Serial2.readBytesUntil(0x3A,horas,3);
  delay (2200);
  hora -> save(horas);
  Serial.print("sending_h -> ");
  Serial.println(horas);
    

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
  Serial.print("received2 <- ");
  Serial.println(data->value());
}
