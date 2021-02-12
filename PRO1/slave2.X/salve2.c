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
//#include "Oscilador.h" // 


//******************************************************************************
// Variables
//******************************************************************************
#define PB0 PORTBbits.RB0  
#define PB1 PORTBbits.RB1  
//******************************************************************************
// Prototipos de funciones
//******************************************************************************
void Setup   (void);
void push_0  (void);
void push_1  (void);
uint8_t W,w,Q,H,h,L,ADCGO;
//******************************************************************************
//******************************************************************************
//                            interuption 
//******************************************************************************
void __interrupt ( ) isr(void){
// la funcion del toggle para los 7seg
    if (RBIF==1){ //revisamos la bandera del ioc 
         push_0 ();// pushbutton de suma 
         push_1 ();// pushbutton de resta
         RBIF=0;} //reset la bandera
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
    TRISB =  0B00000011; //INPUT EN portb
    TRISC =  0B00000000; //INPUT EN portc
    TRISD =  0B00000000; //INPUT EN portd
    TRISE = 0 ; //INPUT EN porte
// analog inputs 
    ANSEL =  0B00000000; // solo estamos usando el RA0 como analogico 
    ANSELH = 0B00000000;
// portB modo pullup
    OPTION_REGbits.nRBPU=0;
// Configuración de timer0------------------------------------------------------
    INTCONbits.GIE=1; //on todas las interrupts global 
    INTCONbits.PEIE=1;//on periferal

// CONFIG INT ON CHANGE 
    INTCONbits.RBIE=1; //PORTB Change Interrupt Enable bit
    IOCB=3;
    ei();}

//******************************************************************************
// funciones 
//******************************************************************************
void push_0 (void) {  
    if (PB0==0){ //check if the button was press
        W=1;}    // if so it will turn an a flag 
    else {
        if(W==1){ //when is not press and it was press in the past it will check the flag 
        W=0;
        PORTA++;//adds 1 to the porta 
        }
        }
} //antirebote 
void push_1 (void) {   
    if (PB1==0){//check if the button was press
        w=1;} // if so it will turn an a flag
    else {
        if(w==1){ //when is not press and it was press in the past it will check the flag
        w=0;
        PORTA--;//subtracts 1 to the porta 
        }
        }
}//antirebote 