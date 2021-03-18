/************************** Configuration ***********************************/
#define RXD2 16//set up of usart com
#define TXD2 17//set up of usart com
#include "config.h"

/************************ varriables  *******************************/
char segundos[2];
char minutos[2];
char horas [2];
/************************ Example Starts Here *******************************/
// this int will hold the current count for our sketch

#define IO_LOOP_DELAY 5000
unsigned long lastUpdate = 0;
/************************ set up of feed *******************************/
AdafruitIO_Feed *hora = io.feed("hora"); //set up of feed 
AdafruitIO_Feed *minu = io.feed("minu");
AdafruitIO_Feed *seg = io.feed("seg");
AdafruitIO_Feed *push = io.feed("push");
AdafruitIO_Feed *push2 = io.feed("push2");
/************************ General set up *******************************/
void setup() {
   // start the serial connection
  Serial.begin(9600);
  Serial2.begin(9600);
  Serial.print("Connecting to Adafruit IO");
    // connect to io.adafruit.com
  io.connect(); 
  // set up a message handler for the count feed.
  // the handleMessage function (defined below)
  // will be called whenever a message is
  // received from adafruit io.
  push ->onMessage(handleMessage);// setting up of feed as on message 
  push2->onMessage(handleMessage2);// setting up of feed as on message 
  
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
   // we are connected
  Serial.println();
  Serial.println(io.statusText());
  push->get();
  push2->get();
}
void loop() {
  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();
  Serial2.write(0X34);
  //revcive segundos from pic 
  Serial2.readBytesUntil(0x3A,segundos,3);//the data that is send has a safety character
  delay (2200);
  seg -> save(segundos);//send to wifi moduel
  Serial.print("sending_s -> ");
  Serial.println(segundos);
  //revcive minuto from pic 
  Serial2.write(0X35);
  Serial2.readBytesUntil(0x3A,minutos,3);//the data that is send has a safety character
  delay (2200);
  minu -> save(minutos);
  Serial.print("sending_m -> ");
  Serial.println(minutos);//send to wifi moduel
  //revcive hora from pic 
  Serial2.write(0X36);
  Serial2.readBytesUntil(0x3A,horas,3);//the data that is send has a safety character
  delay (2200);
  hora -> save(horas);//send to wifi moduel
  Serial.print("sending_h -> ");
  Serial.println(horas);
}
// this function is called whenever a 'counter' message
// is received from Adafruit IO. it was attached to
// the counter feed in the setup() function above.
void handleMessage(AdafruitIO_Data *data) {
   if (data -> toString()== "On"){Serial2.write(0X31);}
   if (data -> toString()== "OFF"){Serial2.write(0X30);}
    Serial.print("received <- ");
    Serial.println(data->value());
}
void handleMessage2(AdafruitIO_Data *data) { 
   if (data -> toString()== "ON"){Serial2.write(0X33);}
   if (data -> toString()== "Off"){Serial2.write(0X32);}
  Serial.print("received2 <- ");
  Serial.println(data->value());
}
