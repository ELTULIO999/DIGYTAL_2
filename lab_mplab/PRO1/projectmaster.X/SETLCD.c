#include <xc.h>
#include "SETLCD.h"
#include <stdint.h>
void LCD_IN (void){
    TRISRS   = 0;
    TRISEN   = 0;
    PORTRI   = 0;
    RS       = 0;
    E        = 0;
    POT      = 0;
    __delay_ms (50);
    LCD_ON(0x02);
    LCD_ON(0x38);
    LCD_ON(0x0C);
    LCD_ON(0x06);
}
void LCD_ON (char C){
    RS=0;
    POT=C;
    LCD_TIM();
    __delay_ms(2);
}
void LCD_TIM(void){
    E = 1;
    __delay_us (5);
    E = 0;
    __delay_us(5);}
void LCD_PLAY(char D){
    RS=1;
    POT=D;
    LCD_TIM();
    __delay_us(50);
}
void LCD_CL (void){
    LCD_ON(0x01);
}
void LCD_P (char *D){
    while (*D){
        LCD_PLAY(*D);
        D++;
    }
}
void LCD_POINT (int x, int y, char *D){
    char LOC;
    switch (y){
        case 1:
            LOC = 0x80 + x;
            break;
        case 2:
            LOC = 0xC0 + x;
            break;            
        default:
            LOC = 0x80 + x;
            break;
    }
    LCD_ON(LOC);
    LCD_P(D);   
}
void LCDVAL1 (int x, int Z){
    switch (Z){
        case 0:
            LCD_POINT(x,2,"0");
            break;
        case 1:
            LCD_POINT(x,2,"1");
            break;
        case 2:
            LCD_POINT(x,2,"2");
            break;
        case 3:
            LCD_POINT(x,2,"3");
            break;
        case 4:
            LCD_POINT(x,2,"4");
            break;
        case 5:
            LCD_POINT(x,2,"5");
            break;
        case 6:
            LCD_POINT(x,2,"6");
            break;
        case 7:
            LCD_POINT(x,2,"7");
            break;
        case 8:
            LCD_POINT(x,2,"8");
            break;
        case 9:
            LCD_POINT(x,2,"9");
            break;
            case 10:
            LCD_POINT (x,2,"A");
            break;
        case 11:
            LCD_POINT (x,2,"B");
            break;
        case 12:
            LCD_POINT (x,2,"C");
            break;
        case 13:
            LCD_POINT  (x,2,"D");
            break;
        case 14:
            LCD_POINT  (x,2,"E");
            break;
        case 15:
            LCD_POINT  (x,2,"F");
            break;
        case 16:
            LCD_POINT  (x,2,".");
            break;
        case 17:
            LCD_POINT  (x,2,"+");
            break;
        case 18:
            LCD_POINT  (x,2,"-");
            break;
        case 19:
            LCD_POINT  (x,2,"°");
            break;
        case 20:
            LCD_POINT  (x,2," ");
            break;
        default:
            LCD_POINT(x,2,"@");
            break;
    }
}