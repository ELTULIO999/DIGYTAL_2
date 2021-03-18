#include"EUSART.h"
#include <xc.h>
void SET_TXR (void) {
    TXSTAbits.SYNC=0;
    TXSTAbits.TXEN=1;
    TXSTAbits.BRGH=1;
    TXSTAbits.TX9=0;
    BAUDCTLbits.BRG16=0;
    SPBRG=25;
    SPBRGH=0;
}
void SET_RXT (void) {
    RCSTAbits.SPEN=1;
    RCSTAbits.CREN=1;
   // RCSTAbits.FERR=0;
   //RCSTAbits.OERR=0;
    RCSTAbits.RX9=0;
    PIE1bits.RCIE=1;
}

