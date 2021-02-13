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
#include "SSSPI.h"
#include "ADC.h"      

//******************************************************************************
// Variables
//******************************************************************************
//******************************************************************************
// Prototipos de funciones
//******************************************************************************
void Setup   (void);
void ADCG    (void);
uint8_t ADCGO,L;
//******************************************************************************
//******************************************************************************
//                            interuption 
//******************************************************************************
void __interrupt ( ) isr(void){
    if (ADIF==1){ //revisamos la bandera del adc
        L=ADRESH; // pasamos el contenido de adresh a unavariable 
        PIR1bits.ADIF=0; // //reset la bandera
        ADCON0bits.GO=1;} //  ponemos esta en on para que vuelva a
    if(SSPIF == 1){
        PORTB=L;
        spiWrite(L);
        SSPIF = 0;
        }
}
//******************************************************************************
// Ciclo principal
//******************************************************************************
void main(void) { 
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_END, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    Setup();
    //**************************************************************************
    // Loop principal
    //**************************************************************************
    while(1){
     ADCG();
    }
}
//******************************************************************************
// Configuración
//******************************************************************************
void Setup(void){
    ADC_CHS_CLKS (0,2);
    PIE1bits.ADIE=1;
 //puertos on clear 
    PORTA =  0; //PORTA EN 0
    PORTB =  0; //PORTB EN 0
    PORTC =  0; //PORTC EN 0
    PORTD =  0; //PORTD EN 0
    PORTE =  0; //PORTE EN 0
// inputs y otputs
    TRISA =  0B00100001; //INPUT EN porta
    TRISB =  0B00000000; //INPUT EN portb
    TRISD =  0B00000000; //INPUT EN portd
    TRISE =  0B0000; //INPUT EN porte
// analog inputs 
    ANSEL =  0B00000001; // solo estamos usando el RA0 como analogico 

// Configuración de timer0------------------------------------------------------
    INTCONbits.GIE=1; //on todas las interrupts global 
    INTCONbits.PEIE=1;//on periferal
     PIR1bits.ADIF=0; // //reset la bandera
    PIR1bits.ADIF=0; // //reset la bandera
    PIR1bits.SSPIF = 0;// Borramos bandera interrupción MSSP
    PIE1bits.SSPIE = 1;// Habilitamos interrupción MSSP
}
//******************************************************************************
// funciones 
//******************************************************************************

void ADCG(void){//GENERO UN DELAY DE ADQUISICIÓN EL CUAL FUNCIONA DE LA SIGUIENTE MANERA
    if(ADCGO > 20){ //CUANDO ADCGO SEA MÁS GRANDE QUE 20 YA QUE ESTE VA A ESTAR SUMANDOSE CONSTANTEMENTE EN LA INTERRUPCIÓN
        ADCGO = 0; //SE SETEA EN 0 NUEVAMENTE
        ADCON0bits.GO_nDONE = 1; //SE HABILITA EL GO DEL ADC PARA QUE LA CONFIGURACIÓN ADC FUNCIONE CORRECTAMENTE 
    }                            //DE ESTA MANERA PUEDE VOLVER A COMENZAR NUEVAMENTE SIN PROBLEMAS
}
