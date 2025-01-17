#include "TV_PICKIT2_SHIFT_1.c"
#include "TV_PICKIT2_SHIFT_KEY4X4_138.c"
#include "18F4550.h"

signed int8 mp;
int8 pattern = 0;
int8 patternD = 0xFF;
unsigned int32 y=0;
int8 flag = 1;
int8 flag1 = 1;
unsigned int16 y1 = 0,y2 = 0;
int8 stat = 1;
int32 stack,stack1,head1,head;


void blink_32led() {
   if (stat == 1){
      xuat_32led_don_2word(0xffff,0xffff);
      delay_ms(50);
      xuat_32led_don_2word(0,0);
      delay_ms(50);
   }
}
void left_2_right_decay() {
      if( stat == 3) {
         if ((y == 0xffffffff) ||  (y == 0)) { flag = ~flag; }
         if (flag & 0x1) {
            y = (y << 1) | 1; }
         else {
            y = y<<1; }
         xuat_32led_don_2word(y>>16,y);
         delay_ms(30);
      }
}
void right_2_left_decay() {
   
   if( stat == 2 ) {
         if ((y == 0xffffffff) ||  (y == 0)) { flag1 = ~flag1;}
         if (flag1 & 0x1) {
            y = (y >> 1) | 0x80000000; }
         else {
            y = y>>1; }
         xuat_32led_don_2word(y>>16,y);
         delay_ms(30);
   }
}
void out_2_in(){
   if( stat == 4 ) { 
      if(y1 == 0xffff && y2 == 0xffff) { y1 = 0,y2 = 0;}
      y1 = (y1<<1) | 1;
      y2 = (y2>>1) | 0x8000;
      xuat_32led_don_2word(y2,y1);
      delay_ms(50);
   }
}
void in_2_out(){
   if( stat == 5 ) {
      if(y1 == 0xffff && y2 == 0xffff) { y1 = 0,y2 = 0 ;}
      y1 = (y1>>1) | 0x8000;
      y2 = (y2<<1) | 1;
      xuat_32led_don_2word(y2,y1);
      delay_ms(50);
   }
}
void left_2_right_incr(){
   if(stat == 6) { 
      if (y==0xffffffff) {y = 0; stack = 0x80000000; head = 1;} 
      xuat_32led_don_2word((y | stack)>>17,y | stack);
      if(stack == head) {stack = 0x80000000;   y |= head; head = head<<1; }
      else stack = stack>>1 ;
      delay_ms(5);
   }
}
void right_2_left_incr(){
   if(stat == 7) { 
      if(stack1 == head1) {stack1 = 1;   y |= head1; head1 = head1>>1; }
      xuat_32led_don_2word((y | stack1)>>16,y | stack1);
      if (y==0xffffffff) {y = 0; stack1 = 1; head1 = 0x80000000;}
      else stack1 = stack1<<1 ;
      delay_ms(10);
   }
}
void main()
{
   set_up_port_ic_chot();
   set_tris_b(0x3c);
   xuat_32led_don_4byte(0,0,0,0);
   
   while(TRUE)
   {
       mp = key_4x4_dw();
        if(mp!=0xff) {
            stat = mp;
            y = 0;
            y1 = 0; y2 =0; head = 1; head1 = 0x80000000; stack = 0x80000000; stack1 = 1;
        }
      blink_32led();
      left_2_right_decay();
      right_2_left_decay();
      out_2_in();
      in_2_out();
      left_2_right_incr();
      right_2_left_incr();
   }
}

