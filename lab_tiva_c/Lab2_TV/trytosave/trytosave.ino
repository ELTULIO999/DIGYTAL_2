//**********************************************************************************************************
//                                          define                                                         *
//**********************************************************************************************************
#include <SPI.h>
#include <SD.h>
File myFile;
File root;
//**********************************************************************************************************
//*                                 Prototipos de funciones                                                *
//**********************************************************************************************************
void MENU (void);
void printDirectory(File dir, int numTabs);
//**********************************************************************************************************
//*                                     General setup                                                      *
//**********************************************************************************************************

void setup(){
  Serial.begin(9600); // Open serial communications and wait for port to open:
  SPI.setModule(0);   // waiting for the SD to start with the module spi 0
  Serial.print("WAITING...");
  pinMode(PA_3, OUTPUT); // setup of the pin for spi com 
  if (!SD.begin(PA_3)) {Serial.println("int failed!");
  return;}
  Serial.println(" DONE!!!!");
  root = SD.open("/");
  printDirectory(root,0);}
  
void loop(){
  int num =Serial.read(); //waitting for input from consul
  if ( num==49 || num==50 || num==51){MENU(num);} //check valio 
  else if (num == -1){num=0;}// ignore enters 
  else if (num != 10){ // if there is any othere input it lets you do it again and tells you to try again 
  Serial.print(num);
  Serial.println(" <--- eso no es una opcion");
  Serial.println(" TRY again!!!!");
  }
}
void MENU (unsigned char poke){ // prints  text depending on that you select 
  switch (poke){
            case 49:
            Serial.println();
            Serial.println();
            Serial.println();
            myFile = SD.open("picachu.txt");
            if (myFile) {
              Serial.println("YOU'VE CHOSEN:");
            //read from the file until there's nothing else in it:
              while (myFile.available()) {Serial.write(myFile.read());}
              // close the file:
              myFile.close();}
            else {Serial.println("error opening txt");}
              break;
            case 50:
            Serial.println();
            Serial.println();
            Serial.println();
            //open the file 
            myFile = SD.open("blast.txt");
            if (myFile) {
              Serial.println("YOU'VE CHOSEN:");
              while (myFile.available()) {Serial.write(myFile.read());}
              myFile.close();}
            else {Serial.println("error opening txt");}
              break;
            case 51:
            Serial.println();
            Serial.println();
            Serial.println();
            myFile = SD.open("fuego.txt");
            if (myFile) {            
              Serial.println("YOU'VE CHOSEN:");
              while (myFile.available()) {Serial.write(myFile.read());}
              myFile.close();}
            else {Serial.println("error opening txt");}
            break;
            default:
            Serial.println("error");
            break;
       }
}
void printDirectory(File dir, int numTabs){ // showes you what is inside of the SD
  dir.rewindDirectory();
  Serial.println("            Bien venido al laboratorio de del Profesor Oak, el no está en este momento pero dejo los");
  Serial.println("                        siguientes Pokémon para que empieces tu aventura Pokémon");
   while(true){
     File entry = dir.openNextFile();
     if (!entry){break;}
     for (uint8_t i=0; i<numTabs; i++){Serial.print('\t');}
     Serial.print(entry.name());
     Serial.println ("\t\t");
     entry.close();}
}
