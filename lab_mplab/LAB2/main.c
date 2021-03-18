/*
 * File:   Interrupciones y Librerías.c
 * Author: juli0
 *
 * Created on January 27, 2021, 12:57 PM
 */
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
#include "Oscilador.h" // 
#include "LEDSCAT.h"   //
#include "ADC.h"       //

//******************************************************************************
// Variables
//******************************************************************************
#define PB0 PORTBbits.RB0  
#define PB1 PORTBbits.RB1  
//******************************************************************************
// Prototipos de funciones
//******************************************************************************
void Setup   (void);
void pull    (void);
void push_0  (void);
void push_1  (void);
void ADCG    (void);
uint8_t W,w,Q,H,h,L,ADCGO;
//******************************************************************************
//******************************************************************************
//                            interuption 
//******************************************************************************
void __interrupt ( ) isr(void){
    if (INTCONbits.TMR0IF==1){//este if esta revisando la bandera del timer0
        TMR0=236;//le cargamos un valor al timer0
        INTCONbits.TMR0IF=0; //reset la bandera
        pull();
        ADCGO++;
    } // la funcion del toggle para los 7seg
    if (RBIF==1){ //revisamos la bandera del ioc 
         push_0 ();// pushbutton de suma 
         push_1 ();// pushbutton de resta
         RBIF=0;} //reset la bandera
    if (ADIF==1){ //revisamos la bandera del adc
        L=ADRESH; // pasamos el contenido de adresh a unavariable 
        PIR1bits.ADIF=0; // //reset la bandera
        ADCON0bits.GO=1;} //  ponemos esta en on para que vuelva a 
}
//******************************************************************************
// Ciclo principal
//******************************************************************************
void main(void) { 
    Setup();
    //**************************************************************************
    // Loop principal
    //**************************************************************************
    while(1){
     ADCG();
     if (PORTD <= L){
         PORTEbits.RE2=1;}
     else {PORTEbits.RE2=0;}
    }
}
//******************************************************************************
// Configuración
//******************************************************************************
void Setup(void){
    ADC_CHS_CLKS (0,2);
    PIE1bits.ADIE=1;
    initOsc (2);
//puertos on clear 
    PORTA =  0; //PORTA EN 0
    PORTB =  0; //PORTB EN 0
    PORTC =  0; //PORTC EN 0
    PORTD =  0; //PORTD EN 0
    PORTE =  0; //PORTE EN 0
// inputs y otputs
    TRISA =  0B00000001; //INPUT EN porta
    TRISB =  0B00000011; //INPUT EN portb
    TRISC =  0B00000000; //INPUT EN portc
    TRISD =  0B00000000; //INPUT EN portd
    TRISE = 0 ; //INPUT EN porte
// analog inputs 
    ANSEL =  0B00000001; // solo estamos usando el RA0 como analogico 
    ANSELH = 0B00000000;
// portB modo pullup
    OPTION_REGbits.nRBPU=0;
// Configuración de timer0------------------------------------------------------
    INTCONbits.GIE=1; //on todas las interrupts global 
    INTCONbits.PEIE=1;//on periferal
    INTCONbits.TMR0IE=1;//on timer 0
    INTCONbits.TMR0IF=0;// clean flags
// CONFIG INT ON CHANGE 
    INTCONbits.RBIE=1; //PORTB Change Interrupt Enable bit
    IOCB=3;
    ei();
//config timer0----------------------------------------------------------------
    OPTION_REGbits.T0CS=0; //internal instructuion cycle clock 
    OPTION_REGbits.T0SE=0; // edge selection 
    OPTION_REGbits.PSA=0; //
//SELECT pre_scaler -----------------------------------------------------------
    OPTION_REGbits.PS0=1;
    OPTION_REGbits.PS1=1;
    OPTION_REGbits.PS2=1;
    
    PIR1bits.ADIF=0; // //reset la bandera
}
//******************************************************************************
// funciones 
//******************************************************************************
void push_0 (void){   
    if (PB0==0){ //check if the button was press
        W=1;}    // if so it will turn an a flag 
    else {
        if(W==1){ //when is not press and it was press in the past it will check the flag 
        W=0;
        PORTD++;//adds 1 to the porta 
        }
        }
} //antirebote 
void push_1 (void){   
    if (PB1==0){//check if the button was press
        w=1;} // if so it will turn an a flag
    else {
        if(w==1){ //when is not press and it was press in the past it will check the flag
        w=0;
        PORTD--;//subtracts 1 to the porta 
        }
        }
}//antirebote 
void pull(void){   
    if (Q==1){
        Q=0;
        PORTEbits.RE1 = 0;//esto prende un 7seg y apaga el otro 
        PORTEbits.RE0 = 1;
        H=((L & 0b11110000)>>4); //esto quita el nibble inferior
        TABLA_L (H); //llama la funcion de los 7seg 
    }
    else {
        Q=1;
        PORTEbits.RE1 = 1; //esto prende un 7seg y apaga el otro 
        PORTEbits.RE0 = 0; 
        h =(L&0b00001111) ; //esto quita el nibble superios solo pasa los primeros 4 bits 
         TABLA_L (h);      //llama la funcion de los 7seg 
        }
}// esta funcion hace el pull de los 7seg y swap

void ADCG(void){//GENERO UN DELAY DE ADQUISICIÓN EL CUAL FUNCIONA DE LA SIGUIENTE MANERA
    if(ADCGO > 20){ //CUANDO ADCGO SEA MÁS GRANDE QUE 20 YA QUE ESTE VA A ESTAR SUMANDOSE CONSTANTEMENTE EN LA INTERRUPCIÓN
        ADCGO = 0; //SE SETEA EN 0 NUEVAMENTE
        ADCON0bits.GO_nDONE = 1; //SE HABILITA EL GO DEL ADC PARA QUE LA CONFIGURACIÓN ADC FUNCIONE CORRECTAMENTE 
    }                            //DE ESTA MANERA PUEDE VOLVER A COMENZAR NUEVAMENTE SIN PROBLEMAS
}
