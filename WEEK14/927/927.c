#include "18F4550.h"
#include "TV_PICKIT2_SHIFT_1.c"
#include "TV_PICKIT2_SHIFT_KEY4X4_138.c"
#include "TV_PICKIT2_SHIFT_32LED_DON.c"
//#include "TV_PICKIT2_SHIFT_LCD.c"
#include "TV_PICKIT2_SHIFT_STEP_MOTOR.c"
#include "TV_PICKIT2_SHIFT_DC_MOTOR.c"
#include "TV_PICKIT2_SHIFT_GLCD128X64.c"
#include "TV_PICKIT2_SHIFT_DS18B20.c"
#include "TV_PICKIT2_SHIFT_DS1307_I2C.c"

#include <math.h>



signed int8 pwm_capso,bdn;
unsigned int16 duty,t1,t1_tam;




#int_timer3
void interrupt_timer3()
{
   set_timer3(3036);
   bdn++;
   if(bdn<5)
   xuat_32led_don_4byte(0xff,0xff,0,0);
   if (bdn==10)
   {
      bdn = 0;
      t1 = get_timer1();
      set_timer1(0);
      t1 = t1/45; t1=t1*60;
      giai_ma_gan_cho_8led_quet_16_xoa(t1);
      xuat_32led_don_4byte(0,0,0xff,0xff);      
   }
}



void phim_up(){
   if (!input(up) && (duty<1000))
   {
         delay_quet_8led(100);
         if (!input(up)) {
         duty = duty + 10;
         set_pwm1_duty(duty);
         pwm_capso++;
         xuat_4led_7doan_giaima_xoa_so0(pwm_capso);
         delay_quet_8led(100);
      }
   }
}
void phim_dw()
{
   if (!input(dw) && (duty>0))
   {
         delay_quet_8led(100);
         if (!input(dw)) {
         duty = duty - 10;
         set_pwm1_duty(duty);
         pwm_capso--;
         xuat_4led_7doan_giaima_xoa_so0(pwm_capso);
         delay_quet_8led(100);
         }
   }
}


void main()
{
   set_up_port_ic_chot();
   dc_enable=1;
   output_low(pin_c1);
   xuat_buzzer_relay();
   setup_ccp1(CCP_PWM);
   setup_timer_2(T2_DIV_BY_1,249,1);

   duty = 0;
   pwm_capso = 0;
   xuat_4led_7doan_giaima_xoa_so0(pwm_capso);
   set_pwm1_duty(duty);

enable_interrupts(INT_TIMER3);
   enable_interrupts(GLOBAL);
   
   

   setup_timer_3(T3_INTERNAL|T3_DIV_BY_8);
   set_timer3(3036);


   setup_timer_1(T1_EXTERNAL_SYNC | T1_DIV_BY_1);
   set_timer1(0);




   bdn = 0;


   while (true)
   {
      phim_dw();
      phim_up();
      hien_thi_8led_7doan_quet();
      if(!input(stop))
      {
         duty = 0;
         pwm_capso = 0;
         set_timer1(0);
         xuat_4led_7doan_giaima_xoa_so0(pwm_capso);
         set_pwm1_duty(duty);
      }
   }
   
}

