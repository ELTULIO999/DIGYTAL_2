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
//#include "Oscilador.h" // 
//******************************************************************************
// Variables
//******************************************************************************
//******************************************************************************
// Prototipos de funciones
//******************************************************************************
void Setup(void);
void CONV (void);
void CONT (void);
uint8_t L,l,Z ;
uint8_t POT1_U,POT1_H, POT1_T;
uint8_t TEM_U,TEM_T;
uint8_t cont_1,cont_2,cont_3,cont;

//******************************************************************************
//******************************************************************************
//                            interuption 
//******************************************************************************

//******************************************************************************
// Ciclo principal
//******************************************************************************
void main(void) {
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    Setup(); // general set up
    LCD_IN(); //start up of lcd
    LCD_CL();// start up of lcd
    LCD_POINT(0,1,"ADC CONT TEMP");//Mensaje inicia
    Z=0;
    
    //**************************************************************************
    // Loop principal
    //**************************************************************************
    while (1){
        switch (Z){
            case 0:
                PORTEbits.RE0=0;
                PORTEbits.RE1=1;
                PORTEbits.RE2=1;
                spiWrite(cont);
                PORTA=cont;
                break;
            case 1:
                PORTEbits.RE0=1;
                PORTEbits.RE1=0;
                PORTEbits.RE2=1;
                break;
            default:
                PORTEbits.RE0=1;
                PORTEbits.RE1=1;
                PORTEbits.RE2=0;
                break;
        }
    LCDVAL1 (10,cont_1);
    LCDVAL1 (11,cont_2);
    LCDVAL1 (12,cont_3);
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
    TRISC =  0B00010000; //INPUT EN portc
    TRISD =  0B00000000; //INPUT EN portd
    TRISE =  1 ; //INPUT EN porte
// analog inputs 
    ANSEL =  0B00000000; // solo estamos usando el RA0 como analogico 
    ANSELH = 0B00000000;

// Configuración de timer0------------------------------------------------------
    INTCONbits.GIE=1; //on todas las interrupts global 
    INTCONbits.PEIE=1;//on periferal
    INTCONbits.RBIE=1; //PORTB Change Interrupt Enable bit
    
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
    //l=(l-0.5)*100
    TEM_U=(l/51);
    TEM_T=((l*100/51)-(TEM_U*100))/10;
    }
void CONVET_cont (void){
    cont_1=(cont/100);
    cont_2=((cont)-(cont_1*100))/10;
    cont_3=((cont)-(cont_1*100)-(cont_2*10));   
    }
   

