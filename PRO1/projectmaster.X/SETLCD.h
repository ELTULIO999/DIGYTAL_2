#ifndef SETLCD_H
#define	SETLCD_H
#include <xc.h> 

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 4000000
#endif

#define RS      PORTDbits.RD6
#define E       PORTDbits.RD7
#define TRISRS  TRISDbits.TRISD6
#define TRISEN  TRISDbits.TRISD7
#define POT     PORTB
#define PORTRI  TRISB

void LCD_IN (void);
void LCD_ON (char C);
void LCD_TIM  (void);
void LCD_PLAY(char D);
void LCD_CL   (void);
void LCD_P (char *D);
void LCD_POINT (int x, int y, char *D);
void LCDVAL1 (int x, int Z);


#endif	/* SETLCD_H */

