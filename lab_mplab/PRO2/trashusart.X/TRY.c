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
#include "EUSART.h"  
//******************************************************************************
// Variables
//******************************************************************************

uint8_t temp,r;




//******************************************************************************
// Prototipos de funciones
//******************************************************************************
void Setup   (void);
void REV    (void);
void REV2    (void);
//******************************************************************************
//                            interuption 
//******************************************************************************
void __interrupt ( ) isr(void){    
    if (PIR1bits.RCIF==1){ 
        temp=RCREG;}
        
    if (PIR1bits.TXIF == 1){
        PIE1bits.TXIE = 0;
        PIR1bits.TXIF = 0;}
    
    if (INTCONbits.TMR0IF==1){//este if esta revisando la bandera del timer0
        TMR0=236;//le cargamos un valor al timer0
        INTCONbits.TMR0IF=0; //reset la bandera
        r++;
        if(r==10){
            r=0;
            PIE1bits.TXIE = 1;}
        } 
 }
     

//******************************************************************************

void main(void) {
    Setup(); // general set up
    SET_RXT(); //set up to send data 
    SET_TXR(); //set up to recive  

    
    //**************************************************************************
    // Loop principal
    //**************************************************************************
    while(1){  
 
    }   
}
void Setup(void){
    PORTA =  0; //PORTA EN 0
    PORTB =  0; //PORTB EN 0
    PORTC =  0; //PORTC EN 0
    PORTD =  0; //PORTD EN 0
    PORTE =  0; //PORTE EN 0
// inputs y otputs
    TRISA =  0B00000000; //INPUT EN porta
    TRISB =  0B00000000; //INPUT EN portb
    TRISC =  0B00000000; //INPUT EN portc
    TRISD =  0B00000000; //INPUT EN portd
    TRISE =  0B0000;     //INPUT EN porte
// analog inputs 
    ANSEL =  0B00000000; // solo estamos usando el RA0 como analogico 
    ANSELH = 0B00000000;

// Configuración de timer0------------------------------------------------------
    INTCONbits.GIE=1; //on todas las interrupts global 
    INTCONbits.PEIE=1;//on periferal
    INTCONbits.TMR0IE=1;//on timer 0
    INTCONbits.TMR0IF=0;// clean flags
    PIE1bits.TXIE = 1;
    PIE1bits.RCIE = 1;
    OPTION_REGbits.T0CS=0; //internal instructuion cycle clock 
    OPTION_REGbits.T0SE=0; // edge selection 
    OPTION_REGbits.PSA=0; //
//SELECT pre_scaler -----------------------------------------------------------
    OPTION_REGbits.PS0=1;
    OPTION_REGbits.PS1=1;
    OPTION_REGbits.PS2=1;
} 
void REV(void){   
    if (temp==0X31){PORTAbits.RA1=0;}
    if (temp==0X30){PORTAbits.RA1=1;} 
    }
void REV2(void){
    
    if (temp==0X31){PORTAbits.RA0=0;}
    if (temp==0X30){PORTAbits.RA0=1;} 
    }

