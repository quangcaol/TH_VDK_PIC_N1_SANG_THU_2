#include "TV_PICKIT2_SHIFT_1.c"
#include "TV_PICKIT2_SHIFT_KEY4X4_138.c"
#include "TV_PICKIT2_SHIFT_32LED_DON.c"
#include "TV_PICKIT2_SHIFT_LCD.c"
#include "TV_PICKIT2_SHIFT_GLCD128X64.c"
#include "TV_PICKIT2_SHIFT_DS18B20.c"
#include "TV_PICKIT2_SHIFT_DS1307_I2C.c"
//#include "18F4550.h"
#include <math.h>
#include <graphics.c>



unsigned char mode = 0;

void dispMode()
{
   lcd_command(0x8a);
   lcd_data(mode+0x30);
}


void phim_mode()
{
   if (!input(mod))
   {
      delay_ms(20);
      if(!input(mod)){
         if (mode < 3) mode++;
         else mode =0;
         dispMode();
         while(!input(mod));
      }
   }
}

unsigned int8 BCD_UP(unsigned int8 x)
{
   unsigned int8 y;
   y = x;
   x =x & 0x0f;
   if (x==0x0a) y = y+6;
   return y;
}

unsigned int8 BCD_DW(unsigned int8 x)
{
   unsigned int8 y;
   y = x;
   x =x & 0x0f;
   if (x==0x0a) y = y-6;
   return y;
}

void ds18_Save() {
   nap_thoi_gian_htai_vao_ds13b07();
   hien_thi_thoi_gian_ds13b07();
}
void phim_up()
{
   if (!input(up)){
      delay_ms(10);
      if (!input(up))
      {
         switch (mode)
         {
         case 1:
            if (giay_ds13 == 0x59) giay_ds13 = 0;
            else 
            {
               giay_ds13++;
               giay_ds13 = BCD_UP(giay_ds13);
            }
            ds18_Save();
            break;
         case 2:
            if (phut_ds13 == 0x59) phut_ds13 = 0;
            else 
            {
               phut_ds13++;
               phut_ds13 = BCD_UP(phut_ds13);
            }
            ds18_Save();
            break;
         case 3:
            if (giay_ds13 == 0x23) giay_ds13 = 0;
            else 
            {
               gio_ds13++;
               gio_ds13 = BCD_UP(gio_ds13);
            }
            ds18_Save();
            break;
         default:
            break;                           
         }
         while (!input(UP));
      }
   }
}

void phim_dw()
{
   if (!input(dw)){
      delay_ms(10);
      if (!input(dw))
      {
         switch (mode)
         {
         case 1:
            if (giay_ds13 == 0x0) giay_ds13 = 0x59;
            else 
            {
               giay_ds13--;
               giay_ds13 = BCD_DW(giay_ds13);
            }
            ds18_Save();
            break;
         case 2:
            if (phut_ds13 == 0x00) phut_ds13 = 0x59;
            else 
            {
               phut_ds13--;
               phut_ds13 = BCD_DW(phut_ds13);
            }
            ds18_Save();
            break;
         case 3:
            if (giay_ds13 == 0) giay_ds13 = 0x23;
            else 
            {
               gio_ds13--;
               gio_ds13 = BCD_DW(gio_ds13);
            }
            ds18_Save();
            break;   
         default:
            break;
         }
         while (!input(dw));
      }
   }
}

void main()
{
   set_up_port_ic_chot();
   setup_lcd();
   set_tris_b(0x3c);
   lcd_command(lcd_addr_line1);
   lcd_data("clock:");
   doc_thoi_gian_tu_realtime();
   xuat_32led_don_4byte(0xff,0,0,0xff);
   if (ma_ds13!=ma_ds)
   {
      thiet_lap_thoi_gian_hien_tai();
      nap_thoi_gian_htai_vao_ds13b07();
   }
   mode = 0;
   dispMode();
   doc_thoi_gian_tu_realtime();
   giaytam = giay_ds13;
   while(true)
   {
      doc_thoi_gian_tu_realtime();
      if (giaytam!=giay_ds13)
      {
         giaytam=giay_ds13;
         dispMode();
         hien_thi_thoi_gian_ds13b07();
      }
      for (int i =0 ; i<100; i ++) 
      {
         phim_mode();
         phim_dw();
         phim_up();
      } 
   }
          
}

