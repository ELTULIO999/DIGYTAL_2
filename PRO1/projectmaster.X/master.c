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
#define _XTAL_FREQ 4000000
#include <xc.h>
#include <stdint.h>
#include "SETLCD.h" 
#include "EUSART.h"
#include "MSSP.h"
#include "Oscilador.h"

//******************************************************************************
// Variables
//******************************************************************************
//******************************************************************************
// Prototipos de funciones
//******************************************************************************
void Setup(void);
void CONV (void);
void CONT (void);
void CONVET_cont(void);
void send (void);
void CONV_AS (void);

uint8_t L,R,r,Z,z,C,c,sign,bro;
uint8_t POT1_U,POT1_H, POT1_T;
uint8_t TEM_U,TEM_T,TEM_H;
uint8_t TEM_UAS,TEM_TAS, TEM_HAS;
uint8_t cont_1,cont_2,cont_3,cont;
uint8_t cont_1AS,cont_2AS,cont_3AS;
uint8_t POT1_Uas,POT1_Has, POT1_Tas;

//******************************************************************************
//******************************************************************************
//                            interuption 
//******************************************************************************
void __interrupt ( ) isr(void){
    if (PIR1bits.TXIF == 1){
        z++;
        send();
        PIE1bits.TXIE = 0;}
    if (INTCONbits.TMR0IF==1){//este if esta revisando la bandera del timer0
        TMR0=250;//le cargamos un valor al timer0
        INTCONbits.TMR0IF=0; //reset la bandera
        r++;
        if(r==100){
            r=0;
            PIE1bits.TXIE = 1;}
       }

     
}
//******************************************************************************
// Ciclo principal
//******************************************************************************
void main(void) {
    SET_TXR(); //set up to recive     initOsc(6); //reloj interno
    SET_RXT();
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_END, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    Setup(); // general set up
    LCD_IN(); //start up of lcd
    LCD_CL();// start up of lcd
    LCD_POINT(0,1,"ADC   TEMP   CONT");//Mensaje inicia
    Z=0;
    z=0;
    
    //**************************************************************************
    // Loop principal
    //**************************************************************************
    while (1){
        switch (Z){
            case 0:
                PORTEbits.RE0=0;
                PORTEbits.RE1=1;
                PORTEbits.RE2=1;
                spiWrite(0x00);
                cont=spiRead(); 
                PORTA=cont;
                CONVET_cont();
                LCDVAL1 (13,cont_1);
                LCDVAL1 (14,cont_2);
                LCDVAL1 (15,cont_3);
                Z++;
                break;
            case 1:
                PORTEbits.RE0=1;
                PORTEbits.RE1=0;
                PORTEbits.RE2=1;
                spiWrite(0x00);
                L=spiRead();
                CONV();
                LCDVAL1 (0,POT1_U);
                LCDVAL1 (1,16);
                LCDVAL1 (2,POT1_T);
                LCDVAL1 (3,POT1_H);
                Z++;
                break;
            case 2:
                PORTEbits.RE0=1;
                PORTEbits.RE1=1;
                PORTEbits.RE2=0;

                spiWrite(0x00);
    
                C=spiRead();
                
                LCDVAL1 (6,bro);
                LCDVAL1 (7,TEM_U);
                LCDVAL1 (8,TEM_T);
                LCDVAL1 (9,TEM_H);


                CONT();
    
                Z=0;
                break;
        }
        CONV_AS();
    }
}
//******************************************************************************
// Configuración
//******************************************************************************
void Setup(void){
//puertos on clear 
    PORTA =  0; //PORTA EN 0
    PORTB =  0; //PORTB EN 0
    PORTC =  0; //PORTC EN 0
    PORTD =  0; //PORTD EN 0
    PORTE =  0; //PORTE EN 0
// inputs y otputs
    TRISA =  0B00000000; //INPUT EN porta
    TRISB =  0B00000000; //INPUT EN portb
    TRISC =  0B00010000; //INPUT EN portb
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
    OPTION_REGbits.T0CS=0; //internal instructuion cycle clock 
    OPTION_REGbits.T0SE=0; // edge selection 
    OPTION_REGbits.PSA=0; //
//SELECT pre_scaler -----------------------------------------------------------
    OPTION_REGbits.PS0=1;
    OPTION_REGbits.PS1=1;
    OPTION_REGbits.PS2=1;
    

}
//******************************************************************************
// funciones 
//******************************************************************************
void CONV (void){   
    
    POT1_U=(L/51);//mapeo de primera var
    POT1_T=((L*100/51)-(POT1_U*100))/10;//mapeo de segunda var
    POT1_H=((L*100/51)-(POT1_U*100)-(POT1_T*10));//mapeo de tersero  var
    }
void CONT (void){  
    R=C;
    if (R>=26){
        c =(((R-26)*150)/77);
        TEM_U=(c/100);
        TEM_T=(c-(TEM_U*100))/10;
        TEM_H=(c-(TEM_U*100)-(TEM_T*10));
        sign=0x20;
        bro=20;
    }
    else{
        c=-((c-26)*55)/27;
        TEM_U=(c/100);
        TEM_T=(c-(TEM_U*100))/10;
        TEM_H=(c-(TEM_U*100)-(TEM_T*10));
        sign=0x2D;
        bro=18;
    }
}
void CONVET_cont (void){
    cont_1=(cont/100);
    cont_2=((cont)-(cont_1*100))/10;
    cont_3=((cont)-(cont_1*100)-(cont_2*10));   
    }
void CONV_AS (void){
    POT1_Uas=(POT1_U+0x30);
    POT1_Tas=(POT1_T+0x30);
    POT1_Has=(POT1_H+0x30);  
    cont_1AS=(cont_1+0X30);
    cont_2AS=(cont_2+0X30);
    cont_3AS=(cont_3+0X30);
    TEM_UAS=(TEM_U+0X30);
    TEM_TAS=(TEM_T+0X30);
    TEM_HAS=(TEM_H+0X30);
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
            TXREG = sign;
            break;
        case 7:
            TXREG = TEM_UAS;
            break;
        case 8:
            TXREG = TEM_TAS;
            break;
        case 9:
            TXREG = TEM_HAS;
            break;
        case 10:
            TXREG = 0x20;
            break;
        case 11:
            TXREG = cont_1AS;
            break;
        case 12:
            TXREG = cont_2AS;
            break;
        case 13:
            TXREG = cont_3AS;
            break;
        default:
             TXREG = 0x0D;
             z=0;
            break;
    }}
