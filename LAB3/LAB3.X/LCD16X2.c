// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdint.h>
//#include <stdio.h>
#define _XTAL_FREQ 4000000
#include "SETLCD.h"  
#include "ADC.h"  
#include "Oscilador.h"  
#include "EUSART.h"  
//******************************************************************************
// Variables
//******************************************************************************
#define PB0 PORTBbits.RB0  
#define PB1 PORTBbits.RB1
char z,GET;
uint8_t S, L,l,Q,H,h,K,k,r,tem1,tem2,W,w,cont;
uint8_t POT1_U,POT2_U, POT1_H,POT2_H, POT1_T,POT2_T;
uint8_t POT1_Uas,POT2_Uas, POT1_Has,POT2_Has, POT1_Tas,POT2_Tas;
uint8_t cont_1,cont_2,cont_3;



//******************************************************************************
// Prototipos de funciones
//******************************************************************************
void Setup   (void);
void pull    (void);
void L_ADC   (void);
void CONVET  (void);
void send    (void);
void push    (void);
void CONVET_cont (void);
//******************************************************************************
//                            interuption 
//******************************************************************************
void __interrupt ( ) isr(void){
    if (ADIF==1){ //revisamos la bandera del adc
        PIR1bits.ADIF=0;} // //reset la bandera
    
    if (PIR1bits.RCIF==1){ 
        GET=RCREG;}
        
    if (PIR1bits.TXIF == 1){
        z++;
        send();
        PIE1bits.TXIE = 0;
        if (Q==1){Q=0;}
        else {Q=1;}
    }
    if (INTCONbits.TMR0IF==1){//este if esta revisando la bandera del timer0
        TMR0=236;//le cargamos un valor al timer0
        INTCONbits.TMR0IF=0; //reset la bandera
        r++;
       }
     
}
//******************************************************************************

void main(void) {
    Setup(); // general set up
    SET_RXT(); //set up to send data 
    SET_TXR(); //set up to recive  
    LCD_IN(); //start up of lcd
    LCD_CL();// start up of lcd
    LCD_POINT(0,1,"ADC1 ADC2 CONT");//Mensaje inicia    
    LCDVAL1 (1, 16); //esto es para poner los puntos entre los voltajes 
    LCDVAL1 (6, 16); //esto es para poner los puntos entre los voltajes

    
    //**************************************************************************
    // Loop principal
    //**************************************************************************
    while(1){
    L_ADC ();
    pull();
    LCDVAL1 (0,POT1_U);// sets the 1st variable in the fisrt bit of the displau 
    LCDVAL1 (2,POT1_T);// sets the 2nd variable in the fisrt bit of the displau
    LCDVAL1 (3,POT1_H);// sets the 3rd variable in the fisrt bit of the displau
    LCDVAL1 (5,POT2_U);// sets the 4rd variable in the fisrt bit of the displau
    LCDVAL1 (7,POT2_T);// sets the 5th variable in the fisrt bit of the displau    
    LCDVAL1 (8,POT2_H);// sets the 6st variable in the fisrt bit of the displau 
    push();
    CONVET_cont();
    LCDVAL1 (10,cont_1);
    LCDVAL1 (11,cont_2);
    LCDVAL1 (12,cont_3);
    
 
    }   
}
void Setup(void){
    initOsc(6); //reloj interno 
    INTCONbits.PEIE = 1;
    PIE1bits.ADIE = 1;
    PIR1bits.ADIF = 0;
    INTCONbits.GIE = 1;
    ADCON0bits.GO_DONE = 1;
    INTCONbits.TMR0IE=1;//on timer 0
    INTCONbits.TMR0IF=0;// clean flags
//puertos on clear 
    PORTA =  0; //PORTA EN 0
    PORTB =  0; //PORTB EN 0
    PORTC =  0; //PORTC EN 0
    PORTD =  0; //PORTD EN 0
    PORTE =  0; //PORTE EN 0
// inputs y otputs
    TRISA =  0B00000011; //INPUT EN porta
    TRISB =  0B00000000; //INPUT EN portb
    TRISC =  0B10000000; //INPUT EN portc
    TRISD =  0B00000000; //INPUT EN portd
    TRISE = 0 ; //INPUT EN porte
// analog inputs 
    ANSEL =  0B00000011; // solo estamos usando el RA0 como analogico 
    ANSELH = 0B00000000;
} 
void pull(void){   
    if (Q==1){
        tem1=ADRESH; // pasamos el contenido de adresh a unavariable
        ADC_CHS_CLKS (1,2);
        CONVET();
    }
    else {
        tem2=ADRESH; // pasamos el contenido de adresh a unavariable
        ADC_CHS_CLKS (0,2);
        CONVET();
    }
}// esta funcion hace el pull de los 7seg y swa
void L_ADC (void){
    if (r > 20){
        r=0;
        ADCON0bits.GO_nDONE=1;}}
void CONVET (void){
    L=tem1;//cambio temporal para mapeo 
    l=tem2;
    POT1_U=(L/51);//mapeo de primera var
    POT1_T=((L*100/51)-(POT1_U*100))/10;//mapeo de segunda var
    POT1_H=((L*100/51)-(POT1_U*100)-(POT1_T*10));//mapeo de tersero  var
    POT2_U=(l/51);
    POT2_T=(((l*100)/51)-(POT2_U*100))/10;
    POT2_H=(((l*100)/51)-(POT2_U*100)-(POT2_T*10));
    
    //cambio para asscci
    POT1_Uas=(POT1_U+0x30);
    POT1_Tas=(POT1_T+0x30);
    POT1_Has=(POT1_H+0x30);
    POT2_Uas=(POT2_U+0x30);
    POT2_Tas=(POT2_T+0x30);
    POT2_Has=(POT2_H+0x30);
    }
void send (void){
    switch (z){
        case 0:
            TXREG = POT1_Uas;
            break;
        case 1:
            TXREG = POT1_Uas;
            break;
        case 2:
            TXREG = 0x2E;
            break;
        case 3:
            TXREG = POT1_Tas;
            break;
        case 4:
            TXREG = POT1_Has;
            break;
        case 5:
            TXREG = 0x20;
            break;
        case 6:
            TXREG = POT2_Uas;
            break;
        case 7:
            TXREG = 0x2E;
            break;
        case 8:
            TXREG = POT2_Tas;
            break;
        case 9:
            TXREG = POT2_Has;
            break;
        default:
             TXREG = 0x0D;
             z=0;
            break;
 /*sending of data 
  its can only send 8bits therefor we have a switch that incremente on each 
  change of the interuption */
    }
}
void push ( void ){ 
    if(GET==43){//we wait to get the asscci + that is 43
        W=1;}
    if(GET==13 && W==1 ){//we wait for the enter and the flag w
            W=0;
            cont++;}
    if (GET==45){//we wait to get the asscci + that is 45
        w=1;}
    if(GET==13 && w==1){//we wait for the enter and the flag w
            W=0;
            cont--;}
}
void CONVET_cont (void){
    cont_1=(cont/100);
    cont_2=((cont)-(cont_1*100))/10;
    cont_3=((cont)-(cont_1*100)-(cont_2*10));   
    }