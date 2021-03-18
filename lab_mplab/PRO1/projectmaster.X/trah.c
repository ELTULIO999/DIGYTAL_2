
void send_hora (void){
    switch (z){
        case 0:
            TXREG = (hor_t+0x30);
 //           z++;
            break;
        case 1:
            TXREG = (hor_u+0x30);
  //          z++;
            break;
        case 2:
            TXREG = (0x3A);
  //          z++;
            break;
        case 3:
             TXREG = (min_t+0x30);
 //            z++;
            break;
        case 4:
             TXREG = (min_u +0x30);
 //           z++;
            break;
        case 5:
            TXREG = (0x3A);
//            z++;
            break;
        case 6:
              TXREG = (seg_t+0x30);
 //            z++;
            break;
        case 7:
              TXREG = (seg_u+0x30);
 //            z++;
            break;
        case 8:
             TXREG = (0x0A);
             z=0;
             C=0;
            break;
    }}  
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