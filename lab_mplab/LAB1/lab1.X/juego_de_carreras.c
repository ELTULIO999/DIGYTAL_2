/*
 * File:   juego_de_carreras.c
 * Author: julit
 *
 * Created on January 20, 2021, 5:44 PM
 */

//******************************************************************************
// Importación de Librerías
//******************************************************************************

#include <xc.h>

//******************************************************************************
// Palabra de configuración
//******************************************************************************
// CONFIG1
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator: Crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
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
//******************************************************************************
// Variables
//******************************************************************************
#define PB0 PORTAbits.RA0  
#define PB1 PORTAbits.RA1  
#define PB2 PORTAbits.RA2  
#define _XTAL_FREQ 8000000 
//******************************************************************************
// Prototipos de funciones
//******************************************************************************
void Setup  (void);
void delay  (char n);
void push_E (unsigned char n);
void led_E  (unsigned char n);
void push_C (unsigned char n);
void led_C  (char n );
void push_D (unsigned char n);
void led_D  (char n );

unsigned char L, l, M, m, W, w, E;


//******************************************************************************
// Ciclo principal
//******************************************************************************
void main(void) { 
    Setup();
    //**************************************************************************
    // Loop principal
    //**************************************************************************
    while(1){
         push_E (PB0);
         while (E==1){
             push_C (PB1);
             led_C  (L);
             push_D (PB2); 
             led_D  ( l );               
         }
    }
}
//******************************************************************************
// Configuración
//******************************************************************************
void Setup(void){
    PORTA =  0;
    PORTC =  0;
    PORTD =  0;
    PORTE =  0;
    ANSEL =  0;
    ANSELH = 0;
    TRISC =  0;
    TRISD =  0;
    TRISE =  0;
    TRISAbits.TRISA0 = 1;
    TRISAbits.TRISA1 = 1;
    TRISAbits.TRISA2 = 1;
    L=0;
    
}
//******************************************************************************
// Delay
//******************************************************************************
void delay(char n){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < 255; j++ ){     
        }
    } 
}
//******************************************************************************
// funciones 
//******************************************************************************
void push_E (unsigned char n){   
    if (n==1){
        m=1;}
    else {
        if(m==1){
            PORTE=0;
            PORTEbits.RE0 = 1;
            __delay_ms(50); 
            PORTE=0;
            PORTEbits.RE1 = 1;
            __delay_ms(50);
            PORTE=0;
            PORTEbits.RE2 = 1;
            __delay_ms(50);
            PORTE=0;
            E=1;}
    }
}//semaforo activado con push 
void push_C (unsigned char n){   
    if (n==1){
        W=1;}
    else {
        if(W==1){
        W=0;
        L++;
        }
        }
}//antirebote 
void push_D (unsigned char n){   
    if (n==1){
        w=1;}
    else {
        if(w==1){
        w=0;
        l++;
        }
        }
}//antirebote 
void led_C  (char n){
        switch (L) {
            case 0:
                PORTC=0;
                break;
            case 1:
                PORTC=0;
                PORTCbits.RC0 = 1;
                break;
            case 2:
                PORTC=0;
                PORTCbits.RC1 = 1;
                break;
            case 3:
                PORTC=0;
                PORTCbits.RC2 = 1;
                break;
            case 4:
                PORTC=0;
                PORTCbits.RC3 = 1;
                break;
            case 5:
                PORTC=0;
                PORTCbits.RC4 = 1;
                break;
            case 6:
                PORTC=0;
                PORTCbits.RC5 = 1;
                break;
            case 7:
                PORTC=0;
                PORTCbits.RC6 = 1;
                break;
            case 8:
                PORTC=0;
                PORTCbits.RC7 = 1;
                break;
            default:
                //PORTC = 0;
                L =0; // esto es para que se mantenga en un loop para jugar siempre 
                E=0;  // para que salga del while de juego 
                m=0;  // para resetiar el push button
                break;
        }
} // el jugador uno 
void led_D  (char n){
        switch (l) {
            case 0:
                PORTD=0;                
                break;
            case 1:
                PORTD=0;
                PORTDbits.RD0 = 1;
                break;
            case 2:
                PORTD=0;
                PORTDbits.RD1 = 1;
                break;
            case 3:
                PORTD=0;
                PORTDbits.RD2 = 1;
                break;
            case 4:
                PORTD=0;
                PORTDbits.RD3 = 1;
                break;
            case 5:
                PORTD=0;
                PORTDbits.RD4 = 1;
                break;
            case 6:
                PORTD=0;
                PORTDbits.RD5 = 1;
                break;
            case 7:
                PORTD=0;
                PORTDbits.RD6 = 1;
                break;
            case 8:
                PORTD=0;
                PORTDbits.RD3 = 1;
                break;
            default:
                //PORTD = 0;
                l=0; //esto es para que se mantenga 
                E=0; // para que salga del while de juego
                m=0; // para resetiar el push button
                break;
        }
} // el jugador dos 


    
