//***************************************************************************************************************************************
#include <stdint.h>
#include <stdbool.h>
#include <TM4C123GH6PM.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"

#include "bitmaps.h"
#include "font.h"
#include "lcd_registers.h"

#define LCD_RST PD_0
#define LCD_CS PD_1
#define LCD_RS PD_2
#define LCD_WR PD_3
#define LCD_RD PE_1
int DPINS[] = {PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7};  

#define P1 PA_7 //LEFT
#define P2 PE_3 //shoot
#define P3 PA_6  //RIGHT

#define P4 PD_7
#define P5 PD_6
#define P6 PF_4


//#define pb1 PUSH1
//#define pb2 PUSH2

int L,l,W, w,S,s,F, f;
int flg_1,flg_2;

int game_mode_flag;

int g1,g2,g3,g4,g5,g6,g7,g8,g9,g10;
int r1,r2,r3,r4,r5,r6,r7,r8,r9,r10;

int flag1 ,flag_1,P_cor_x,P_cor_y;
int flag2 ,flag_2,P2_cor_x,P2_cor_y;

int cont_index; 

int cont_ply1,cont_play2;

int contador_J1, contador_J2;


//***************************************************************************************************************************************
// Functions Prototypes
//***************************************************************************************************************************************
void LCD_Init(void);
void LCD_CMD(uint8_t cmd);
void LCD_DATA(uint8_t data);
void SetWindows(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
void LCD_Clear(unsigned int c);
void H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c);
void V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c);
void Rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c);
void FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c);
void LCD_Print(String text, int x, int y, int fontSize, int color, int background);
void LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]);
void LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[],int columns, int index, char flip, char offset);

void move_nave_1 (void);
void enemigo (int x,int y);
void shoot(unsigned char n);


void PLY1  (void);
void PLY2  (void);


//***************************************************************************************************************************************
// Inicialización
//***************************************************************************************************************************************
void setup() {
  SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
  Serial.begin(9600);
  GPIOPadConfigSet(GPIO_PORTB_BASE, 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);
  Serial.println("Inicio");
  LCD_Init();
  LCD_Clear(0x0F7D);

  
 pinMode(P1, INPUT_PULLUP);
 pinMode(P2, INPUT_PULLUP);
 pinMode(P3, INPUT_PULLUP);
 pinMode(P4, INPUT_PULLUP);
 pinMode(P5, INPUT_PULLUP);
 pinMode(P6, INPUT_PULLUP);


  
 P_cor_y=198;
 P2_cor_y=198;
 l=120;
 s=220;
 contador_J1=10;
 contador_J2=10;
 


}
//***************************************************************************************************************************************
// Loop Infinito
//***************************************************************************************************************************************
void loop() {
  /*int val_PB1 = digitalRead(pb1);
  int val_PB2 = digitalRead(pb2);
  if (val_PB1==HIGH&&val_PB2==LOW){game_mode_flag=1;}
  else if (val_PB1==LOW &&val_PB2==LOW){game_mode_flag=2;}
    
  while (game_mode_flag==1){//single player
    int val_P1 = digitalRead(P1);
    int val_P2 = digitalRead(P2); 
    int val_P3 = digitalRead(P3);
    shoot(val_P3);
    pushb1 (val_P1);
    pushb2 (val_P2);
    move_nave();
    enemigos (50,50,23,14,cont_index,enemy_b,&g1);
    enemigos (74,50,23,14,cont_index,enemy_b,&g2);
    enemigos (98,50,23,14,cont_index,enemy_b,&g3);
    enemigos (122,50,23,14,cont_index,enemy_b,&g4);
    enemigos (62,65,23,14,cont_index,enemy_b,&g5);
    enemigos (86,65,23,14,cont_index,enemy_b,&g6);
    enemigos (110,65,23,14,cont_index,enemy_b,&g7);
    enemigos (74,80,23,14,cont_index,enemy_b,&g8);
    enemigos (98,80,23,14,cont_index,enemy_b,&g9);
    enemigos (86,95,23,14,cont_index,enemy_b,&g10);

    enemigos (170,50,23,14,cont_index,enemy_c,&r1,);
    enemigos (170+24,50,23,14,cont_index,enemy_c,&r2);
    enemigos (170+48,50,23,14,cont_index,enemy_c,&r3);
    enemigos (170+72,50,23,14,cont_index,enemy_c,&r4);
    enemigos (182,65,23,14,cont_index,enemy_c,&r5);
    enemigos (182+24,65,23,14,cont_index,enemy_c,&r6);
    enemigos (182+48,65,23,14,cont_index,enemy_c,&r7);
    enemigos (194,50+30,23,14,cont_index,enemy_c,&r8);
    enemigos (194+24,50+30,23,14,cont_index,enemy_c,&r9);
    enemigos (206,50+45,23,14,cont_index,enemy_c,&r10);
    cont_index++;
   if (cont_index>30){cont_index=0;}
  }
  
  while (game_mode_flag==2){*/
    int val_P1 = digitalRead(P1);
    int val_P2 = digitalRead(P2); 
    int val_P3 = digitalRead(P3);
    shoot(val_P3);
    pushb1 (val_P1);
    pushb2 (val_P2);
    move_nave_1();
    
    int val_P4 = digitalRead(P4);
    int val_P5 = digitalRead(P5); 
    int val_P6 = digitalRead(P6);
    pushb3 (val_P4);
    pushb4 (val_P5);
    move_nave_2();  
    shoot_2(val_P6);
    
    enemigos2 (170,50,23,14,cont_index,enemy_c,&r1,&contador_J2);
    enemigos2 (170+24,50,23,14,cont_index,enemy_c,&r2,&contador_J2);
    enemigos2 (170+48,50,23,14,cont_index,enemy_c,&r3,&contador_J2);
    enemigos2 (170+72,50,23,14,cont_index,enemy_c,&r4,&contador_J2);
    enemigos2 (182,65,23,14,cont_index,enemy_c,&r5,&contador_J2);
    enemigos2 (182+24,65,23,14,cont_index,enemy_c,&r6,&contador_J2);
    enemigos2 (182+48,65,23,14,cont_index,enemy_c,&r7,&contador_J2);
    enemigos2 (194,50+30,23,14,cont_index,enemy_c,&r8,&contador_J2);
    enemigos2 (194+24,50+30,23,14,cont_index,enemy_c,&r9,&contador_J2);
    enemigos2 (206,50+45,23,14,cont_index,enemy_c,&r10,&contador_J2);
    
    if (contador_J2==0){FillRect(100,100,11,12,0x00);}
    if (contador_J1==0){FillRect(100,100,11,12,0x00);}
    if (cont_index>30){cont_index=0;}
    cont_index++;
  //}
}
//***************************************************************************************************************************************
// Función para inicializar LCD
//***************************************************************************************************************************************

void enemigos (int x,int y, int m, int q,int ind,unsigned char bla[],int* flagx){
  if (P_cor_x<=x && P_cor_x>=x-m && P_cor_y<=y && *flagx==0){
    (*flagx)=1;
    FillRect(x,y,23,14,0x00);
    FillRect(P_cor_x+5,P_cor_y+11,11,12,0x00);
    P_cor_y=198;
    flag1=0;
    flag_1=0;
    }
  else if ((*flagx)==0){
    if(ind>0&&ind<10){LCD_Sprite(x,y,23,14,bla,2, 0, 1,1);}
    else if (ind>10&&ind<20){LCD_Sprite(x,y,23,14,bla,2, 1, 1,1);}
    }
}
void enemigos1 (int x,int y, int m, int q,int ind,unsigned char bla[],int* flagx,int* flag){
  if (P_cor_x<=x && P_cor_x>=x-m && P_cor_y<=y && *flagx==0){
    (*flagx)=1;
    FillRect(x,y,23,14,0x00);
    FillRect(P_cor_x+5,P_cor_y+11,11,12,0x00);
    P_cor_y=198;
    flag1=0;
    flag_1=0;
    (*flag)--;
    }
  else if ((*flagx)==0){
    if(ind>0&&ind<10){LCD_Sprite(x,y,23,14,bla,2, 0, 1,1);}
    else if (ind>10&&ind<20){LCD_Sprite(x,y,23,14,bla,2, 1, 1,1);}
    }
}

void enemigos2 (int x,int y, int m, int q,int ind,unsigned char bla[],int* flagx, int* flag){
  if (P2_cor_x<=x && P2_cor_x>=x-m && P2_cor_y<=y && *flagx==0){
    (*flagx)=1;
    FillRect(x,y,23,14,0x00);
    FillRect(P2_cor_x+5,P2_cor_y+11,11,12,0x00);
    P2_cor_y=198;
    flag2=0;
    flag_2=0;
    (*flag)--;
    }
  else if ((*flagx)==0){
    if(ind>0&&ind<10){LCD_Sprite(x,y,23,14,bla,2, 0, 1,1);}
    else if (ind>10&&ind<20){LCD_Sprite(x,y,23,14,bla,2, 1, 1,1);}
    }
}
  
void shoot(unsigned char n){
  if (n == LOW){flag1=1;}
  else if (n == HIGH && flag_1==0 && flag1==1){
    flag_1=1;
    P_cor_x=L;}
  else if(P_cor_y<=201 && P_cor_y>0 && P_cor_y>=9 && flag1==1 && flag_1==1){
    LCD_Bitmap(P_cor_x+5,P_cor_y, 11,12, proyectil);
    FillRect(P_cor_x+5,P_cor_y+12,11,12,0x00);
    //delay(100);
    P_cor_y=P_cor_y-12;}
  else if(P_cor_y<=9){
    FillRect(P_cor_x+5,P_cor_y+12,11,12,0x00);
    P_cor_y=198; 
    flag1=0;
    flag_1=0;}
}
void shoot_2(unsigned char n){
  if (n == LOW){flag2=1;}
  else if (n == HIGH && flag_2==0 && flag2==1){
    flag_2=1;
    P2_cor_x=S;}
  else if(P2_cor_y<=201 && P2_cor_y>0 && P2_cor_y>=9 && flag2==1 && flag_2==1){
    LCD_Bitmap(P2_cor_x+5,P2_cor_y, 11,12, proyectil_2);
    FillRect(P2_cor_x+5,P2_cor_y+12,11,12,0x00);
    P2_cor_y=P2_cor_y-12;}
  else if(P2_cor_y<=9){
    FillRect(P2_cor_x+5,P2_cor_y+12,11,12,0x00);
    P2_cor_y=198; 
    flag2=0;
    flag_2=0;}
}
void move_nave (void){
  while( L != l){
    if (L < l){
      L=l;
      FillRect(L-19, 220,19,19,0x00);
      LCD_Bitmap(L,220, 19, 19, ship);
      if(L>301){
        L=0;
        l=0;
        FillRect(301, 220,19,19,0x00);
        LCD_Bitmap(L,220, 19, 19, ship);}
      }
    if (L > l){
      L=l;
      FillRect(L+19, 220,19,19,0x00);
      LCD_Bitmap(L,220, 19, 19, ship);
      if(L<0){
        L=301;
        l=301;
        FillRect(0, 220,19,19,0x00);
        LCD_Bitmap(L,220, 19, 19, ship);
      }
    }
  }
}
void move_nave_1 (void){
  while( L != l){
    if (L < l){
      L=l;
      if(L>140){
        L=0;
        l=0;
        FillRect(140, 220,19,19,0x00);
        LCD_Bitmap(L,220, 19, 19, ship);
        }
      FillRect(L-10, 220,19,19,0x00);
      LCD_Bitmap(L,220, 19, 19, ship);
      }
    if (L > l){
      L=l;
      if(L<0){
        L=140;
        l=140;
        FillRect(0, 220,19,19,0x00);
        LCD_Bitmap(L,220, 19, 19, ship);
        flg_1=1;}
      if (flg_1==0){FillRect(L+10, 220,19,19,0x00);}
      if (flg_1==1){flg_1=0;}
      LCD_Bitmap(L,220, 19, 19, ship);
      Serial.println("dis");
    }
  }
}
void move_nave_2 (void){
  while( S != s){
    if (S < s){
      S=s;
      if(S>301){
        S=160;
        s=160;
        FillRect(301, 220,18,18,0x00);
        LCD_Bitmap(S,220, 18, 18, ship_2);
        flg_2=1;}
      if (flg_2==0){FillRect(S-9, 220,18,18,0x00);}
      if (flg_2==1){flg_2=0;}
      LCD_Bitmap(S,220, 18, 18, ship_2);
      }
    if (S > s){
      S=s;
      if(S<160){
        S=301;
        s=301;
        FillRect(160, 220,18,18,0x00);
        LCD_Bitmap(S,220, 18, 18, ship_2);}
      FillRect(S+9, 220,18,18,0x00);
      LCD_Bitmap(S,220, 18, 18, ship_2);
    }
  }
}


void pushb1 (unsigned char n){//debouce  
    if (n==LOW){W=1;}
    else {
        if(W==1){
        W=0;
        l=l+10;}
        }} 
void pushb2 (unsigned char n){
    if (n==LOW){
        w=1;}
    else {
        if(w==1){
        w=0;
        l=l-10;}
        }}
void pushb3 (unsigned char n){//debouce  
    if (n==LOW){F=1;}
    else {
        if(F==1){
        F=0;
        s=s+9;}
        }} 
void pushb4 (unsigned char n){
    if (n==LOW){
        f=1;}
    else {
        if(f==1){
        f=0;
        s=s-9;}
        }}


void LCD_Init(void) {
  pinMode(LCD_RST, OUTPUT);
  pinMode(LCD_CS, OUTPUT);
  pinMode(LCD_RS, OUTPUT);
  pinMode(LCD_WR, OUTPUT);
  pinMode(LCD_RD, OUTPUT);
  for (uint8_t i = 0; i < 8; i++){
    pinMode(DPINS[i], OUTPUT);
  }
  //****************************************
  // Secuencia de Inicialización
  //****************************************
  digitalWrite(LCD_CS, HIGH);
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_WR, HIGH);
  digitalWrite(LCD_RD, HIGH);
  digitalWrite(LCD_RST, HIGH);
  delay(5);
  digitalWrite(LCD_RST, LOW);
  delay(20);
  digitalWrite(LCD_RST, HIGH);
  delay(150);
  digitalWrite(LCD_CS, LOW);
  //****************************************
  LCD_CMD(0xE9);  // SETPANELRELATED
  LCD_DATA(0x20);
  //****************************************
  LCD_CMD(0x11); // Exit Sleep SLEEP OUT (SLPOUT)
  delay(100);
  //****************************************
  LCD_CMD(0xD1);    // (SETVCOM)
  LCD_DATA(0x00);
  LCD_DATA(0x71);
  LCD_DATA(0x19);
  //****************************************
  LCD_CMD(0xD0);   // (SETPOWER) 
  LCD_DATA(0x07);
  LCD_DATA(0x01);
  LCD_DATA(0x08);
  //****************************************
  LCD_CMD(0x36);  // (MEMORYACCESS)
  LCD_DATA(0x40|0x80|0x20|0x08); // LCD_DATA(0x19);
  //****************************************
  LCD_CMD(0x3A); // Set_pixel_format (PIXELFORMAT)
  LCD_DATA(0x05); // color setings, 05h - 16bit pixel, 11h - 3bit pixel
  //****************************************
  LCD_CMD(0xC1);    // (POWERCONTROL2)
  LCD_DATA(0x10);
  LCD_DATA(0x10);
  LCD_DATA(0x02);
  LCD_DATA(0x02);
  //****************************************
  LCD_CMD(0xC0); // Set Default Gamma (POWERCONTROL1)
  LCD_DATA(0x00);
  LCD_DATA(0x35);
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0x02);
  //****************************************
  LCD_CMD(0xC5); // Set Frame Rate (VCOMCONTROL1)
  LCD_DATA(0x04); // 72Hz
  //****************************************
  LCD_CMD(0xD2); // Power Settings  (SETPWRNORMAL)
  LCD_DATA(0x01);
  LCD_DATA(0x44);
  //****************************************
  LCD_CMD(0xC8); //Set Gamma  (GAMMASET)
  LCD_DATA(0x04);
  LCD_DATA(0x67);
  LCD_DATA(0x35);
  LCD_DATA(0x04);
  LCD_DATA(0x08);
  LCD_DATA(0x06);
  LCD_DATA(0x24);
  LCD_DATA(0x01);
  LCD_DATA(0x37);
  LCD_DATA(0x40);
  LCD_DATA(0x03);
  LCD_DATA(0x10);
  LCD_DATA(0x08);
  LCD_DATA(0x80);
  LCD_DATA(0x00);
  //****************************************
  LCD_CMD(0x2A); // Set_column_address 320px (CASET)
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0x3F);
  //****************************************
  LCD_CMD(0x2B); // Set_page_address 480px (PASET)
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0xE0);
//  LCD_DATA(0x8F);
  LCD_CMD(0x29); //display on 
  LCD_CMD(0x2C); //display on

  LCD_CMD(ILI9341_INVOFF); //Invert Off
  delay(120);
  LCD_CMD(ILI9341_SLPOUT);    //Exit Sleep
  delay(120);
  LCD_CMD(ILI9341_DISPON);    //Display on
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para enviar comandos a la LCD - parámetro (comando)
//***************************************************************************************************************************************
void LCD_CMD(uint8_t cmd) {
  digitalWrite(LCD_RS, LOW);
  digitalWrite(LCD_WR, LOW);
  GPIO_PORTB_DATA_R = cmd;
  digitalWrite(LCD_WR, HIGH);
}
//***************************************************************************************************************************************
// Función para enviar datos a la LCD - parámetro (dato)
//***************************************************************************************************************************************
void LCD_DATA(uint8_t data) {
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_WR, LOW);
  GPIO_PORTB_DATA_R = data;
  digitalWrite(LCD_WR, HIGH);
}
//***************************************************************************************************************************************
// Función para definir rango de direcciones de memoria con las cuales se trabajara (se define una ventana)
//***************************************************************************************************************************************
void SetWindows(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {
  LCD_CMD(0x2a); // Set_column_address 4 parameters
  LCD_DATA(x1 >> 8);
  LCD_DATA(x1);   
  LCD_DATA(x2 >> 8);
  LCD_DATA(x2);   
  LCD_CMD(0x2b); // Set_page_address 4 parameters
  LCD_DATA(y1 >> 8);
  LCD_DATA(y1);   
  LCD_DATA(y2 >> 8);
  LCD_DATA(y2);   
  LCD_CMD(0x2c); // Write_memory_start
}
//***************************************************************************************************************************************
// Función para borrar la pantalla - parámetros (color)
//***************************************************************************************************************************************
void LCD_Clear(unsigned int c){  
  unsigned int x, y;
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);   
  SetWindows(0, 0, 319, 239); // 479, 319);
  for (x = 0; x < 320; x++)
    for (y = 0; y < 240; y++) {
      LCD_DATA(c >> 8); 
      LCD_DATA(c); 
    }
  digitalWrite(LCD_CS, HIGH);
} 
//***************************************************************************************************************************************
// Función para dibujar una línea horizontal - parámetros ( coordenada x, cordenada y, longitud, color)
//*************************************************************************************************************************************** 
void H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c) {  
  unsigned int i, j;
  LCD_CMD(0x02c); //write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);
  l = l + x;
  SetWindows(x, y, l, y);
  j = l;// * 2;
  for (i = 0; i < l; i++) {
      LCD_DATA(c >> 8); 
      LCD_DATA(c); 
  }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar una línea vertical - parámetros ( coordenada x, cordenada y, longitud, color)
//*************************************************************************************************************************************** 
void V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c) {  
  unsigned int i,j;
  LCD_CMD(0x02c); //write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);
  l = l + y;
  SetWindows(x, y, x, l);
  j = l; //* 2;
  for (i = 1; i <= j; i++) {
    LCD_DATA(c >> 8); 
    LCD_DATA(c);
  }
  digitalWrite(LCD_CS, HIGH);  
}
//***************************************************************************************************************************************
// Función para dibujar un rectángulo - parámetros ( coordenada x, cordenada y, ancho, alto, color)
//***************************************************************************************************************************************
void Rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c) {
  H_line(x  , y  , w, c);
  H_line(x  , y+h, w, c);
  V_line(x  , y  , h, c);
  V_line(x+w, y  , h, c);
}
//***************************************************************************************************************************************
// Función para dibujar un rectángulo relleno - parámetros ( coordenada x, cordenada y, ancho, alto, color)
//***************************************************************************************************************************************
void FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c) {
  unsigned int i;
  for (i = 0; i < h; i++) {
    H_line(x  , y  , w, c);
    H_line(x  , y+i, w, c);
  }
}
//***************************************************************************************************************************************
// Función para dibujar texto - parámetros ( texto, coordenada x, cordenada y, color, background) 
//***************************************************************************************************************************************
void LCD_Print(String text, int x, int y, int fontSize, int color, int background) {
  int fontXSize ;
  int fontYSize ;
  
  if(fontSize == 1){
    fontXSize = fontXSizeSmal ;
    fontYSize = fontYSizeSmal ;
  }
  if(fontSize == 2){
    fontXSize = fontXSizeBig ;
    fontYSize = fontYSizeBig ;
  }
  
  char charInput ;
  int cLength = text.length();
  Serial.println(cLength,DEC);
  int charDec ;
  int c ;
  int charHex ;
  char char_array[cLength+1];
  text.toCharArray(char_array, cLength+1) ;
  for (int i = 0; i < cLength ; i++) {
    charInput = char_array[i];
    Serial.println(char_array[i]);
    charDec = int(charInput);
    digitalWrite(LCD_CS, LOW);
    SetWindows(x + (i * fontXSize), y, x + (i * fontXSize) + fontXSize - 1, y + fontYSize );
    long charHex1 ;
    for ( int n = 0 ; n < fontYSize ; n++ ) {
      if (fontSize == 1){
        charHex1 = pgm_read_word_near(smallFont + ((charDec - 32) * fontYSize) + n);
      }
      if (fontSize == 2){
        charHex1 = pgm_read_word_near(bigFont + ((charDec - 32) * fontYSize) + n);
      }
      for (int t = 1; t < fontXSize + 1 ; t++) {
        if (( charHex1 & (1 << (fontXSize - t))) > 0 ) {
          c = color ;
        } else {
          c = background ;
        }
        LCD_DATA(c >> 8);
        LCD_DATA(c);
      }
    }
    digitalWrite(LCD_CS, HIGH);
  }
}
//***************************************************************************************************************************************
// Función para dibujar una imagen a partir de un arreglo de colores (Bitmap) Formato (Color 16bit R 5bits G 6bits B 5bits)
//***************************************************************************************************************************************
void LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]){  
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW); 
  
  unsigned int x2, y2;
  x2 = x+width;
  y2 = y+height;
  SetWindows(x, y, x2-1, y2-1);
  unsigned int k = 0;
  unsigned int i, j;

  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      LCD_DATA(bitmap[k]);
      LCD_DATA(bitmap[k+1]);
      //LCD_DATA(bitmap[k]);    
      k = k + 2;
     } 
  }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar una imagen sprite - los parámetros columns = número de imagenes en el sprite, index = cual desplegar, flip = darle vuelta
//***************************************************************************************************************************************
void LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[],int columns, int index, char flip, char offset){
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW); 

  unsigned int x2, y2;
  x2 =   x+width;
  y2=    y+height;
  SetWindows(x, y, x2-1, y2-1);
  int k = 0;
  int ancho = ((width*columns));
  if(flip){
  for (int j = 0; j < height; j++){
      k = (j*(ancho) + index*width -1 - offset)*2;
      k = k+width*2;
     for (int i = 0; i < width; i++){
      LCD_DATA(bitmap[k]);
      LCD_DATA(bitmap[k+1]);
      k = k - 2;
     } 
  }
  }else{
     for (int j = 0; j < height; j++){
      k = (j*(ancho) + index*width + 1 + offset)*2;
     for (int i = 0; i < width; i++){
      LCD_DATA(bitmap[k]);
      LCD_DATA(bitmap[k+1]);
      k = k + 2;
     } 
  }
    
    
    }
  digitalWrite(LCD_CS, HIGH);
}
