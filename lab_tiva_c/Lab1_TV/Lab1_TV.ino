//**********************************************************************************************************
//                                  define
//**********************************************************************************************************
// defining the rgb leds and the push of the tiva core 
#define LEDR RED_LED
#define LEDG GREEN_LED
#define LEDB BLUE_LED
#define P1 PUSH1
#define P2 PUSH2

//i am fixinf an arry with ports so the cabling is more manageble 
 int port1[]={PB_0,PB_1,PE_4,PE_5,PB_4,PA_5,PA_6,PA_7};
 int port2[]={PD_0,PD_1,PD_2,PD_3,PE_1,PE_2,PE_3,PB_5};
 // defenition of variables 
 int cont1 ;
 int cont2 ;
 int E;
 int W;
 int w;
//**********************************************************************************************************
//                            Prototipos de funciones
//**********************************************************************************************************
void start  (unsigned char n,unsigned char N);
void pushb1 (unsigned char n);
void pushb2 (unsigned char n);
void PLY1  (void);
void PLY2  (void);
//**********************************************************************************************************
//                            General setup 
//**********************************************************************************************************
void setup() {                
  pinMode(P1, INPUT_PULLUP);
  pinMode(P2, INPUT_PULLUP);
  pinMode(LEDR, OUTPUT); 
  pinMode(LEDG, OUTPUT);  
  pinMode(LEDB, OUTPUT);
//def of port1
  pinMode(PB_0, OUTPUT);
  pinMode(PB_1, OUTPUT);
  pinMode(PE_4, OUTPUT);
  pinMode(PE_5, OUTPUT);
  pinMode(PB_4, OUTPUT);
  pinMode(PA_5, OUTPUT);
  pinMode(PA_6, OUTPUT);
  pinMode(PA_7, OUTPUT);
//def of port2
  pinMode(PD_0, OUTPUT);
  pinMode(PD_1, OUTPUT);
  pinMode(PD_2, OUTPUT);
  pinMode(PD_3, OUTPUT);
  pinMode(PE_1, OUTPUT);
  pinMode(PE_2, OUTPUT);
  pinMode(PE_3, OUTPUT);
  pinMode(PB_5, OUTPUT);
  E=0;
  
}
//********************************************************************************************************
//                             Loop principal
//********************************************************************************************************
void loop() {
//use to rest leds (all leds off)  
  PLY1();
  PLY2();
//wait for both players to be ready and sequence of read light 
  int val_P1 = digitalRead(P1);
  int val_P2 = digitalRead(P2);
  start(val_P1,val_P2);
// this while is use for the actual playing 
  while (E==1){
    int val_P1 = digitalRead(P1);
    int val_P2 = digitalRead(P2);
    pushb1 (val_P1);
    PLY1();
    pushb2 (val_P2);
    PLY2();

  }   
}
//********************************************************************************************************
//                            funciones 
//********************************************************************************************************

void start  (unsigned char n,unsigned char N){//sequence of read light 
     if(n==0 && N==0){
          digitalWrite(LEDR, HIGH);  
          delay(300);               
          digitalWrite(LEDR, LOW);    
          delay(300);               
          digitalWrite(LEDR, HIGH);   
          digitalWrite(LEDG, HIGH);   
          delay(300);               
          digitalWrite(LEDR, LOW);  
          digitalWrite(LEDG, LOW);   
          delay(300);
          digitalWrite(LEDG, HIGH);   
          delay(300);               
          digitalWrite(LEDG, LOW);    
          delay(300);
          E=1;}
          }
void pushb1 (unsigned char n){//debouce  
    if (n==0){
        W=1;}
    else {
        if(W==1){
        W=0;
        cont1++;
        }
        }
}//antirebote 
void pushb2 (unsigned char n){//debouce 
    if (n==1){
        w=1;}
    else {
        if(w==1){
        w=0;
       cont2++;
        }
        }
}//antirebote 
void PLY1  (void ){//player one sequence
       switch (cont1) {
            case 0:
            digitalWrite(port1[0], LOW);
            digitalWrite(port1[1], LOW);
            digitalWrite(port1[2], LOW);
            digitalWrite(port1[3], LOW);
            digitalWrite(port1[4], LOW);
            digitalWrite(port1[5], LOW);
            digitalWrite(port1[6], LOW);
            digitalWrite(port1[7], LOW);
                break;
            case 1:
            digitalWrite(port1[0], HIGH);
                break;
            case 2:
            digitalWrite(port1[1], HIGH);
            digitalWrite(port1[0], LOW);
                break;
            case 3:
            digitalWrite(port1[2], HIGH);
            digitalWrite(port1[1], LOW);
                break;
            case 4:
            digitalWrite(port1[3], HIGH);
            digitalWrite(port1[2], LOW);
                break;
            case 5:
            digitalWrite(port1[4], HIGH);
            digitalWrite(port1[3], LOW);
                break;
            case 6:
            digitalWrite(port1[5], HIGH);
            digitalWrite(port1[4], LOW);
                break;
            case 7:
            digitalWrite(port1[6], HIGH);
            digitalWrite(port1[5], LOW);
                break;
            case 8:
            digitalWrite(port1[7], HIGH);
            digitalWrite(port1[6], LOW);
                break;
            case 9:
            digitalWrite(LEDB, HIGH);
            delay(300);
            digitalWrite(LEDB, LOW);
            delay(300);
            digitalWrite(LEDB, HIGH);
            delay(300);
            digitalWrite(LEDB, LOW);
            E=0;
            cont1=0; //reset the var of the player 1
                break;
        }
}
void PLY2  ( void ){//player two sequence
         switch (cont2) {
            case 0:
            digitalWrite(port2[0], LOW);
            digitalWrite(port2[1], LOW);
            digitalWrite(port2[2], LOW);
            digitalWrite(port2[3], LOW);
            digitalWrite(port2[4], LOW);
            digitalWrite(port2[5], LOW);
            digitalWrite(port2[6], LOW);
            digitalWrite(port2[7], LOW);
                break;
            case 1:
            digitalWrite(port2[0], HIGH);
                break;
            case 2:
            digitalWrite(port2[1], HIGH);
            digitalWrite(port2[0], LOW);
                break;
            case 3:
            digitalWrite(port2[2], HIGH);
            digitalWrite(port2[1], LOW);
                break;
            case 4:
            digitalWrite(port2[3], HIGH);
            digitalWrite(port2[2], LOW);
                break;
            case 5:
            digitalWrite(port2[4], HIGH);
            digitalWrite(port2[3], LOW);
                break;
            case 6:
            digitalWrite(port2[5], HIGH);
            digitalWrite(port2[4], LOW);
                break;
            case 7:
            digitalWrite(port2[6], HIGH);
            digitalWrite(port2[5], LOW);
                break;
            case 8:
            digitalWrite(port2[7], HIGH);
            digitalWrite(port2[6], LOW);
                break;
            case 9:
            digitalWrite(LEDR, HIGH);
            delay(300);
            digitalWrite(LEDR, LOW);
            delay(300);
            digitalWrite(LEDR, HIGH);
            delay(300);
            digitalWrite(LEDR, LOW);
            E=0;
            cont2=0; //reset the var of the player 1
                break;
        }
}
