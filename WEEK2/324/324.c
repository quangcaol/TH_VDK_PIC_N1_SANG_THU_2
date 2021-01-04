#include "TV_PICKIT2_SHIFT_1.c"
#include "TV_PICKIT2_SHIFT_KEY4X4_138.c"
unsigned int16 y = 0;



void main()
{
   set_up_port_ic_chot();
   set_tris_b(0x3c);
   xuat_32led_don_4byte(0,0,0,0);
   
   while(true)
   {
      if(!input(UP)) {
         delay_ms(20);
         if(!input(UP)) {
            y = (y<<1) | 1 ;
            while(!input(UP));
         }
      }
      if(!input(DW)) {
         delay_ms(20);
         if(!input(DW)) {
            y = (y>>1) ;
            while(!input(DW));
         }
      }
      if(!input(CLR)) {
         y = 0;
      }
      xuat_32led_don_2word(0,y);
   }
}


