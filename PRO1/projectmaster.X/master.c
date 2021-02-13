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
void CONVET_cont(void);
void send (void);
void CONV_AS (void);

uint8_t L,l,Z,z;
uint8_t POT1_U,POT1_H, POT1_T;
uint8_t TEM_U,TEM_T;
uint8_t TEM_UAS,TEM_TAS;
uint8_t cont_1,cont_2,cont_3,cont;
uint8_t cont_1AS,cont_2AS,cont_3AS;
uint8_t POT1_Uas,POT1_Has, POT1_Tas;

//******************************************************************************
//******************************************************************************
//                            interuption 
//******************************************************************************
        
//******************************************************************************
// Ciclo principal
//******************************************************************************
void main(void) {
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_END, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    Setup(); // general set up
    LCD_IN(); //start up of lcd
    LCD_CL();// start up of lcd
    LCD_POINT(0,1,"ADC TEMP CONT");//Mensaje inicia
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
                
                spiWrite(0x00);
                __delay_ms(1);
                cont=spiRead(); 
                PORTA=cont;
                CONVET_cont();
                __delay_ms(1);
                
                LCDVAL1 (10,cont_1);
                LCDVAL1 (11,cont_2);
                LCDVAL1 (12,cont_3);
                Z++;
                break;
            case 1:
                PORTEbits.RE0=1;
                PORTEbits.RE1=0;
                PORTEbits.RE2=1;
                
                spiWrite(0x00);
                __delay_ms(1);
                L=spiRead();
                
                CONV();
                __delay_ms(1);
                
                LCDVAL1 (2,POT1_U);
                LCDVAL1 (3,16);
                LCDVAL1 (4,POT1_T);
                LCDVAL1 (5,POT1_H);
                
                
                Z=0;
                break;
            default:
                PORTEbits.RE0=1;
                PORTEbits.RE1=1;
                PORTEbits.RE2=0;
                break;
        }
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
    //TRISC =  0B00101000; //INPUT EN portc
    TRISD =  0B00000000; //INPUT EN portd
    TRISE =  0 ; //INPUT EN porte
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
void CONV_AS (void){
    POT1_Uas=(POT1_U+0x30);
    POT1_Tas=(POT1_T+0x30);
    POT1_Has=(POT1_H+0x30);  
    cont_1AS=(cont_1+0X30);
    cont_2AS=(cont_2+0X30);
    cont_3AS=(cont_3+0X30);
    TEM_UAS=(TEM_U+0X30);
    TEM_TAS=(TEM_T+0X30);
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
            TXREG = cont_1AS;
            break;
        case 7:
            TXREG = cont_2AS;
            break;
        case 8:
            TXREG = cont_3AS;
            break;
        case 9:
            TXREG = 0x20;
            break;
        case 10:
            TXREG = TEM_UAS;
            break;
        case 11:
            TXREG = TEM_TAS;
            break;
        default:
             TXREG = 0x0D;
             z=0;
            break;
    }}
