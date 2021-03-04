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
#include "EUSART.h"
#include "Oscilador.h"
#include "I2C.h"
#include "stdio.h"



//******************************************************************************
// Variables
//******************************************************************************
//******************************************************************************
// Prototipos de funciones
//******************************************************************************
void Setup  (void);
void send_hora  (void);
void send_dia  (void);
void CONVET (void);
void CARGA  (void);
void REV    (void);
void forced_send (void);
//******************************************************************************
// Variables
//******************************************************************************
uint8_t L,Z,z,r,C,q;
uint8_t GET,GET2,temp;
uint8_t mou,day,hor,min,seg,week,year;
uint8_t mou_u,day_u,hor_u,min_u,seg_u ;
uint8_t mou_t,day_t,hor_t,min_t,seg_t ;
//******************************************************************************

//******************************************************************************
//                            interuption 
//******************************************************************************
void __interrupt ( ) isr(void){
    //if (C==1){
        if (PIR1bits.TXIF ==1){
            send_hora();
            PIE1bits.TXIE = 0;
            PIR1bits.TXIF =0;}
   // }
    
    
    
  
    if (PIR1bits.RCIF==1){ 
        temp= RCREG ;
        forced_send();
        PIR1bits.RCIF=1;
    }
    if (INTCONbits.TMR0IF==1){//este if esta revisando la bandera del timer0
        TMR0=236;             //le cargamos un valor al timer0
        INTCONbits.TMR0IF=0;  //reset la bandera
        r++;
        if(r==10){
            r=0;
            PIE1bits.TXIE = 1;}
       }     
}
//******************************************************************************
// Ciclo principal
//******************************************************************************
void main(void) {
    Setup();   // general set up
    initOsc(6);
    SET_RXT(); //set up to send data 
    SET_TXR(); //set up to recive  
    I2C_Master_Init(100000);
    //**************************************************************************
    // Loop principal
    //**************************************************************************
    while (1){
        PORTAbits.RA0=0;
        I2C_Master_Start();
        I2C_Master_Write(0xD0);
        I2C_Master_Write(0x00);
        I2C_Master_RepeatedStart();
        I2C_Master_Write(0xD1);
        seg =  I2C_Master_Read(1);
        min =  I2C_Master_Read(1);
        hor =  I2C_Master_Read(1);
        week = I2C_Master_Read(1);
        day =  I2C_Master_Read(1);
        mou =  I2C_Master_Read(1);
        year=  I2C_Master_Read(0);
        I2C_Master_Stop();
        CONVET();
 
    }}
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
void CONVET (void){
    mou_t = ((mou & 0b00010000)>>4);
    mou_u = (mou  & 0b00001111);   
    day_t = ((day & 0b00110000)>>4); 
    day_u = (day  & 0b00001111);   
    hor_t = ((hor & 0b00110000)>>4);
    hor_u = (hor  & 0b00001111);
    min_t = ((min & 0b01110000)>>4);
    min_u = (min  & 0b00001111);
    seg_t = ((seg & 0b01110000)>>4);
    seg_u = (seg  & 0b00001111);
}
void CARGA (void){
    I2C_Master_Start();
    I2C_Master_Write(0xD0);
    I2C_Master_Write(0);
    I2C_Master_Write(0b00000000);
    I2C_Master_Write(0x35);//MIN
    I2C_Master_Write(0x01);//HORAS
    I2C_Master_Write(1);
    I2C_Master_Write(0x01);// día
    I2C_Master_Write(0x03);// mes
    I2C_Master_Write(0x21);// año
    I2C_Master_Stop();//finalizo comunicacion
}
void REV (void){
    switch (Z){
        case 0:
            GET=RCREG;
            if (GET==3){PORTAbits.RA0=1;}
            if (GET==4){PORTAbits.RA0=0;}
            Z++;
            PIR1bits.RCIF=0;
            break;
        case 1:
            GET2=RCREG;
            if (GET2==5){PORTAbits.RA1=1;}
            if (GET2==6){PORTAbits.RA1=0;}
            Z=0;
            PIR1bits.RCIF=0;
            break;
    }
}
void forced_send (void){
    if (temp == 1){C=1;
    PORTAbits.RA0=0;
    }
    
//    if (C==2){PIE1bits.TXIE=1;
//    PORTAbits.RA1=1;}
}
void send_hora (void){
    switch (z){
        case 0:
            TXREG = (hor_t+0x30);
            z++;
            break;
        case 1:
            TXREG = (hor_u+0x30);
            z++;
            break;
        case 2:
            TXREG = (0x3A);
            z++;
            break;
        case 3:
             TXREG = (min_t+0x30);
             z++;
            break;
        case 4:
             TXREG = (min_u +0x30);
            z++;
            break;
        case 5:
            TXREG = (0x3A);
            z++;
            break;
        case 6:
              TXREG = (seg_t+0x30);
             z++;
            break;
        case 7:
              TXREG = (seg_u+0x30);
             z++;
            break;
        case 8:
             TXREG = (0x0A);
             z=0;
             C=0;
            break;
    }}  
void send_dia (void){
    switch (q){
        case 0:
            TXREG = (mou_t+0x30);
            q++;
            break;
        case 1:
            TXREG = (mou_u+0x30);
            q++;
            break;
        case 2:
            TXREG = (0x2F);
            q++;
            break;
        case 3:
            TXREG = (day_t+0x30);
            q++;
            break;
        case 4:
            TXREG = (day_u+0x30 );
            q++;
            break;
        case 5:
            TXREG = (0x0A);
            q=0;
            break;
    }}
