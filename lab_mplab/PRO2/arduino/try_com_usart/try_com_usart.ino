#define RXD2 16
#define TXD2 17
char timedata [7]; 

void setup() {
  // Note the format for setting a serial port is as follows: Serial2.begin(baud-rate, protocol, RX pin, TX pin);
  Serial.begin(9600);
  Serial2.begin(9600);
 
}

void loop() { //Choose Serial1 or Serial2 as required
  
   Serial2.write(0X31);

   Serial2.println(timedata);
   
  delay(500);
   Serial2.write(0X30);
    //while (Serial2.available()) {
   // Serial.print(char(Serial2.read()));
  //}
  delay(500);
    Serial2.write(0X34);
    Serial2.readBytesUntil(0X0A, timedata, 8);
}
