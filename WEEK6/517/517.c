#include "TV_PICKIT2_SHIFT_1.c"
#include "TV_PICKIT2_SHIFT_KEY4X4_138.c"
#include "TV_PICKIT2_SHIFT_32LED_DON.c"
#include "18F4550.h"

signed int8 giay,bdn,phut,gio,dem_tg_exit = 0;
int8 mode = 0;

#int_timer1
void interrupt_timer1(){
   bdn++;
   set_timer1(3036); //3036
}

void giai_ma(){
   led_7dq[0] = ma7doan[giay%10];
   led_7dq[1] = ma7doan[giay/10];
   led_7dq[3] = ma7doan[phut%10];
   led_7dq[4] = ma7doan[phut/10];
   led_7dq[6] = ma7doan[gio%10];
   led_7dq[7] = ma7doan[gio/10];
   if (mode == 1) led_7dq[0] = led_7dq[0] & 0x7f;
   else if (mode == 2) led_7dq[3] = led_7dq[3] & 0x7f;
   else if (mode == 3) led_7dq[6] = led_7dq[6] & 0x7f;
}

void phim_up(){
   if (phim_bt0(200)) {
         dem_tg_exit = 0;
         switch (mode)
         {
            case 1: if (giay==59) giay =0;
                    else giay++;
                    break;
            case 2: if (phut == 59) phut = 0;
                    else phut++;
                    break;
            case 3: if (gio == 23) gio = 0;
                    else gio ++;
                    break;
            default: break;
         }
         giai_ma();
   }
}

void phim_dw(){
   if (phim_bt1(200)) {
         switch (mode)
         {
            case 1: if (giay==0) giay =59;
                    else giay--;
                    break;
            case 2: if (phut == 0) phut = 59;
                    else phut--;
                    break;
            case 3: if (gio == 0) gio = 23;
                    else gio--;
                    break;
            default: break;        
         }
         giai_ma();
   }
}

void phim_mode(){
   if (phim_bt2(200)) {
         mode++;
         if (mode  >= 4){
            mode = 0;
         }
         dem_tg_exit = 0;
         giai_ma();
      }
}

void tat_2_led_chinh(){
   if (mode == 1) {led_7dq[0] &= 0x7f; led_7dq[1] &= 0x7f;}
   else if (mode == 2) {led_7dq[3] &= 0x7f; led_7dq[4] &= 0x7f;}
   else if (mode == 3) {led_7dq[6] &= 0x7f; led_7dq[7] &= 0x7f;}   
}

void main()
{
   set_up_port_ic_chot();
   set_tris_b(0x3c);
   setup_timer_1(T1_INTERNAL|T1_DIV_BY_8);
   set_timer1(3036); // 3036
   enable_interrupts(GLOBAL);
   enable_interrupts(INT_TIMER1);
   giay = 0;
   bdn = 0;
   phut = 0;
   gio = 0;
   giai_ma();
   while(TRUE)
   {
      if (bdn<10) {
         if (mode !=0)
         {
            if ((bdn = 0) && (input(bt0)) && (input(bt1)) && (input(bt2))) tat_2_led_chinh();
            else if (bdn == 5) giai_ma();
         }
         hien_thi_8led_7doan_quet_all();
         phim_up();
         phim_dw();
         phim_mode();
      }
      else 
      {
         bdn = bdn -10;
         dem_tg_exit++;
         if (dem_tg_exit == 20) mode = 0;
         giay++;
            if (giay==60) { giay = 0; phut++;
               if (phut == 60) { phut = 0; gio++; 
                  if (gio == 24) gio = 0;
               } 
            }
         giai_ma();
      }
   }           
}

