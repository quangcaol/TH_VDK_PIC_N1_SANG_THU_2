#include "TV_PICKIT2_SHIFT_1.c"
#include "TV_PICKIT2_SHIFT_KEY4X4_138.c"
#include "TV_PICKIT2_SHIFT_32LED_DON.c"
#include "TV_PICKIT2_SHIFT_LCD.c"
#include "TV_PICKIT2_SHIFT_GLCD128X64.c"
#include "TV_PICKIT2_SHIFT_DS18B20.c"
#include "18F4550.h"
#include <math.h>
#include <graphics.c>


const unsigned char hang2[] = {0x11,0xE,0,0,0,0,0,0x1F,0x1F,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x1F,0x1F,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1F};

unsigned int8 you = 33;
signed int8 giay,bdn,phut,gio;
int8 dem = 0;
int16 lm35a = 0,lm35b = 0;
int1 ttqn = 0;
signed int8 max = 100,D,C,E;
int8 maxT = 40;
unsigned int8 count = 0,countT = 0,temp;
unsigned int8 donvi = 0, j;
unsigned int16 kqadc;
float k_cach, clk_cach, kcs;

/*#int_timer1
void interrupt_timer1(){
   bdn++;
   set_timer1(3036); //3036
}*/


void BaoChay(){
   if (ds18a > 35){
      triac_2_off();
      xuat_4led_7doan_4so(ma7doan[3],ma7doan[5],ma7doan[temp/10],ma7doan[temp%10]);
      you = 35;
   }
   if (ds18a <= 33){
      triac_2_on();
      you = 33;
      xuat_4led_7doan_4so(ma7doan[3],ma7doan[3],ma7doan[temp/10],ma7doan[temp%10]);
   }
}

void LCD_SO_LON(signed int8 lcd_so,x1,y1){
   lcd_goto_xy(x1,y1);
   for (int i = 0; i<6;i++){
      if(i==3) lcd_goto_xy(x1+1,y1);
      lcd_data(lcd_so_x[lcd_so][i]);
   }
}

void giai_ma(){
   led_7dq[0] = ma7doan[giay%10];
   led_7dq[1] = ma7doan[giay/10];
   led_7dq[3] = ma7doan[phut%10];
   led_7dq[4] = ma7doan[phut/10];
   led_7dq[6] = ma7doan[gio%10];
   led_7dq[7] = ma7doan[gio/10];
   lcd_command(lcd_addr_line1);
   printf(lcd_data,"NHIET DO LM35B: %ld",lm35a);
   lcd_command(lcd_addr_line2);
   printf(lcd_data,"NHIET DO LM35A: %ld",lm35b);
}

/*void LCD_DISPLAY_COUNTER(){
   unsigned int8 DM = (unsigned int8)kcs;
   D = DM/100;
   C = DM/10%10;
   E = DM%10;
   LCD_SO_LON(D,2,0);
   LCD_SO_LON(C,2,3);
   LCD_SO_LON(E,2,7);
}*/

void LCD_DISPLAY_COUNTER(){
   lcd_command(lcd_addr_line3);
   printf(lcd_data,"DEM SAN PHAM: %d",count);
}

void read_temp(){
   set_adc_channel(0); 
   lm35a = read_adc()/2.046;
   set_adc_channel(1);
   lm35b = read_adc()/2.046;
}

void phim_up() {
   if (!input(up)) {
      delay_ms(5);
      if (!input(up))
      {
         maxT++;
         xuat_4led_7doan_4so(ma7doan[maxT/10],ma7doan[maxT%10],ma7doan[lm35a/10],ma7doan[lm35a%10]);
         while(!input(up));
      }
   }
}
void phim_dw() {
   if (!input(dw)) {
      delay_ms(5);
      if (!input(dw))
      {
         maxT--;
         xuat_4led_7doan_4so(ma7doan[maxT/10],ma7doan[maxT%10],ma7doan[lm35a/10],ma7doan[lm35a%10]);
         while (!input(dw));
      }
   }
}

void dispDistance(float tam){

}

void readSR_04() {
   set_timer1(0);
   delay_ms(10);
   output_high(trigger);
   delay_ms(15);
   output_low(trigger);

   while (!(input(echo)));
   setup_timer_1(T1_INTERNAL|T1_DIV_BY_4);

   while(input(echo));
   kcs = get_timer1();
   set_timer1(T1_DISABLED);
   set_timer1(0);
   kcs = kcs*0.8;
   kcs = (kcs/58);

   dispDistance(kcs);
}

void readGP2D12() {
   set_adc_channel(2);
   kqadc = 0;
   for (j = 0; j<30; j++){
      kqadc = kqadc + read_adc();
      delay_quet_8led(1);
   }
   kqadc = kqadc /30;
   k_cach = pow(4277/kqadc,1.106);
   if (k_cach >80) k_cach = 80;
}

void readDS() {
      if(touch_present()) {doc_giatri_ds18b20();}
      if (ds18al != ds18a_tam) {
         ds18a_tam = ds18al;
         ds18a = ds18a>>4;
         ds18a = ds18a & 0x0ff;
         temp = ds18a;
         xuat_4led_7doan_4so(ma7doan[temp/10],ma7doan[temp%10],ma7doan[count/10],ma7doan[count%10]);
      }
}


void main()
{
   set_up_port_ic_chot();
   set_tris_b(0x3c);
   khoi_tao_ds18b20();
   ds18a_tam = 0;
   xuat_4led_7doan_giaima_xoa_so0(0);
   setup_lcd();
   setup_glcd(glcd_graphic_mode);



   lcd_command(0x40);
   for (int i = 0; i<64;i++){
      lcd_data(lcd_ma_8doan[i]);
   }


   setup_adc(ADC_CLOCK_DIV_32);
   setup_adc_ports(AN0_TO_AN2|VSS_VDD);
   set_adc_channel(2);
   setup_timer_0(T0_EXT_L_TO_H|T0_DIV_1);
   set_timer0(0);   
   setup_timer_1(T1_DISABLED);
   //set_timer1(3036); // 3036
   enable_interrupts(GLOBAL);
   enable_interrupts(INT_TIMER1);


   giay = 0;
   bdn = 0;
   phut = 29;
   gio = 15;
   giai_ma();
   while(TRUE)
   {
      if (bdn<10) {
         hien_thi_8led_7doan_quet_all();     
         count = get_timer0();
         if (count != countT) {
            countT = count;
            LCD_DISPLAY_COUNTER();
            xuat_4led_7doan_4so(ma7doan[temp/10],ma7doan[temp%10],ma7doan[count/10],ma7doan[count%10]);         
            if (count >= max+1) {set_timer0(1);}   
         }
         //readGP2D12();
         //readSR_04();
         readDS();
         read_temp();
         //BaoChay();
         phim_up();
         phim_dw();
         if(ttct_td==1) sang_tat_32led(5,1);
         if(ttct_td==2) sang_tat_dan_ngoai_vao_32led(5,1);
         if(ttct_td==3) sang_don_pst_32led(5,1);
         if(ttct_td==4) sang_tat_dan_pst_32led(5,1);
         if(ttct_td==5) sang_tat_dan_phai_sang_trai_2x16led(5,1);
         if(ttct_td==6) sang_don_pst_32led(5,1);
         if(ttct_td==7) sang_don_pst_32led(5,1);
         if(ttct_td==8) sang_tat_32led(5,1);
         if(ttct_td==9) sang_tat_dan_ngoai_vao_32led(5,1);
         if(ttct_td==10) sang_don_pst_32led(5,1);
         if(ttct_td==11) sang_tat_dan_pst_32led(5,1);
         if(ttct_td==12) sang_tat_dan_phai_sang_trai_2x16led(5,1);
         if(ttct_td==13) sang_tat_dan_pst_32led(5,1);
         if(ttct_td==14) sang_tat_dan_tsp_32led(5,1);
         if(ttct_td==15) sang_don_pst_32led(5,1);
         if(ttct_td>15) ttct_td =1;
         bdn++;      
      }
      else 
      {
         bdn = bdn -10;
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

