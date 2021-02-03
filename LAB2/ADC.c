#include "ADC.h"
void ADC_CHS_CLKS (uint8_t C, uint8_t S){
     switch (C) {
            case 0:
                ADCON0bits.CHS3=0;
                ADCON0bits.CHS2=0;
                ADCON0bits.CHS1=0;
                ADCON0bits.CHS0=0;
                break;
            case 1:
                ADCON0bits.CHS3=0;
                ADCON0bits.CHS2=0;
                ADCON0bits.CHS1=0;
                ADCON0bits.CHS0=1;
                break;
            case 2:
                ADCON0bits.CHS3=0;
                ADCON0bits.CHS2=0;
                ADCON0bits.CHS1=1;
                ADCON0bits.CHS0=0;
                break;
            case 3:
                ADCON0bits.CHS3=0;
                ADCON0bits.CHS2=0;
                ADCON0bits.CHS1=1;
                ADCON0bits.CHS0=1;
                break;
            case 4:
                ADCON0bits.CHS3=0;
                ADCON0bits.CHS2=1;
                ADCON0bits.CHS1=0;
                ADCON0bits.CHS0=0;
                break;
            case 5:
                ADCON0bits.CHS3=0;
                ADCON0bits.CHS2=1;
                ADCON0bits.CHS1=0;
                ADCON0bits.CHS0=1;                
                break;
            case 6:
                ADCON0bits.CHS3=0;
                ADCON0bits.CHS2=1;
                ADCON0bits.CHS1=1;
                ADCON0bits.CHS0=0;
                break;
            case 7:
                ADCON0bits.CHS3=0;
                ADCON0bits.CHS2=1;
                ADCON0bits.CHS1=1;
                ADCON0bits.CHS0=1;
                break;
            case 8:
                ADCON0bits.CHS3=1;
                ADCON0bits.CHS2=0;
                ADCON0bits.CHS1=0;
                ADCON0bits.CHS0=0;
                break;
            case 9:
                ADCON0bits.CHS3=1;
                ADCON0bits.CHS2=0;
                ADCON0bits.CHS1=0;
                ADCON0bits.CHS0=1;
                break;
            case 10:
                ADCON0bits.CHS3=1;
                ADCON0bits.CHS2=0;
                ADCON0bits.CHS1=1;
                ADCON0bits.CHS0=0;
                break;
            case 11:
                ADCON0bits.CHS3=1;
                ADCON0bits.CHS2=0;
                ADCON0bits.CHS1=1;
                ADCON0bits.CHS0=1;
                break;
            case 12:
                ADCON0bits.CHS3=1;
                ADCON0bits.CHS2=1;
                ADCON0bits.CHS1=0;
                ADCON0bits.CHS0=0;
                break;
            case 13:
                ADCON0bits.CHS3=1;
                ADCON0bits.CHS2=1;
                ADCON0bits.CHS1=0;
                ADCON0bits.CHS0=1;
                break;
            case 14: //CVref
                ADCON0bits.CHS3=1;
                ADCON0bits.CHS2=1;
                ADCON0bits.CHS1=1;
                ADCON0bits.CHS0=0;
                break;
            default://fixed Ref (0.6v fixed voltage refernce)
                ADCON0bits.CHS3=1;
                ADCON0bits.CHS2=1;
                ADCON0bits.CHS1=1;
                ADCON0bits.CHS0=1;
                break;
        }
     switch (S) {
            case 0://FOSC/2
                ADCON0bits.ADCS1=0;
                ADCON0bits.ADCS0=0;                
                break;
            case 1://FOSC/8
                ADCON0bits.ADCS1=0;
                ADCON0bits.ADCS0=1;  
                break;
            case 2://FOSC/32
                ADCON0bits.ADCS1=1;
                ADCON0bits.ADCS0=0;  
                break;
            default://FRC (clock derived from a dedicated internal oscillator = 500 kHz max)
                ADCON0bits.ADCS1=1;
                ADCON0bits.ADCS0=1;
            break;
                
     ADCON0bits.ADON=1;
     ADCON0bits.GO=1;
     ADCON1=0;
    }
}
