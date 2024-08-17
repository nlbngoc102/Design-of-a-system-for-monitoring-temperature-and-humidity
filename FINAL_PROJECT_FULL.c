#include <tv_16f887.c>
#use I2C (MASTER, I2C1, FAST = 400000, stream = SSD1306_STREAM)
#include <tv_oled.c>
#include <tv_ds1307.c>
#include <tv_lcd.c>
#include <tv_dht11.c>
#include <tv_eeprom.c>
#use fast_io(B)
#use fast_io(D)

float ao_T, ao_RH;
unsigned int8 oled_mode = 0, lcd_mode = 0, i = 0;
unsigned int8 RH_max = 70, T_max = 40;
unsigned int8 slot_write = 0, slot_read = 0;
unsigned int8 T_read[32], RH_read[32];
unsigned int8 hour_Ts = 0, minute_Ts = 0, second_Ts = 0;
unsigned int8 date_Ts = 0, month_Ts = 0, year_Ts = 0;
unsigned int8 hour_Te = 0, minute_Te = 0, second_Te = 0;
unsigned int8 date_Te = 0, month_Te = 0, year_Te = 0;
unsigned int8 hour_RHs = 0, minute_RHs = 0, second_RHs = 0;
unsigned int8 date_RHs = 0, month_RHs = 0, year_RHs = 0;
unsigned int8 hour_RHe = 0, minute_RHe = 0, second_RHe = 0;
unsigned int8 date_RHe = 0, month_RHe = 0, year_RHe = 0;

void delay(unsigned int16 t)
{
   for (unsigned int16 j=0; j<t; j++)
   {
      delay_ms(1);
      if ((!input(pin_b7))||(!input(pin_b6))||(!input(pin_b5))||(!input(pin_b4)))
         break;      
   }
}

void eeprom_write_data()
{
   if(second%3 == 0)
   {
      if(slot_write < 14)
      {
         eeprom_write(slot_write, T_ng);
         eeprom_write(slot_write+32, RH_ng);
         delay(100);
         slot_write++;
         if (slot_write == 14)   slot_write = 0;
      }
   }
}

void button_return_lcd()
{
   if (!input(pin_b4))
   {
      delay_us(10);
      if (!input(pin_b4))
      {
         lcd_mode = 0;
      }
   } while (!input(pin_b4));
}

void select_mode()
{
   if(!input(pin_b7))
   {
      delay_us(10);
      if(!input(pin_b7))
      {
         oled_mode++;
         if (oled_mode == 5)
            oled_mode = 0;
         oled_cleardisplay();   
      }
      while(!input(pin_b7));
   }
}

void oled_128x64_mode_0()
{   
   oled_goto_xy(8,1);
   oled_printf("PROJECT");
   oled_goto_xy(4,2);
   oled_printf("EMBEDDED SYSTEM");
   
   oled_goto_xy(1,4);
   oled_printf("- TEMPERATURE: ");  
   oled_goto_xy(17,4);
   printf(oled_printf, "%2u", T_ng);
   OLED_PutCustomC(degree);
   oled_printf("C");
   
   oled_goto_xy(1,5);
   oled_printf("- HUMIDITY: ");
   oled_goto_xy(17,5);
   printf(oled_printf, "%2u", RH_ng);
   oled_printf("%");
   
   oled_goto_xy(1,7);
   oled_printf("- TIME: ");
   printf(oled_printf, "%02u", hour);
   oled_printf(":");
   printf(oled_printf, "%02u", minute);
   oled_printf(":");
   printf(oled_printf, "%02u", second);
   
   oled_goto_xy(1,8);
   oled_printf("- DATE: ");
   printf(oled_printf, "%02u", date);
   oled_printf("/");
   printf(oled_printf, "%02u", month);
   oled_printf("/20");
   printf(oled_printf, "%02u", year);
}

void oled_128x64_mode_1()
{
   oled_goto_xy(1,1);
   oled_printf("TEMPERATURE MAX:");
   oled_goto_xy(1,3);
   oled_printf("------------->  ");
   printf(oled_printf, "%2u", T_max);
   OLED_PutCustomC(degree);
   oled_printf("C");
}

void setting_high_T_mode_1()
{
   if(!input(pin_b5))
   {
      delay_us(10);
      if(!input(pin_b5))
      {
         T_max++;
         if(T_max == 51)
            T_max = 30;
      }
      while(!input(pin_b5));
   }
   
   if(!input(pin_b6))
   {
      delay_us(10);
      if(!input(pin_b6))
      {
         T_max--;
         if(T_max == 29)
            T_max = 50;
      }
      while(!input(pin_b6));
   }
}

void oled_128x64_mode_2()
{
   oled_goto_xy(1,1);
   oled_printf("HUMIDITY MAX:");
   oled_goto_xy(1,3);
   oled_printf("------------->  ");
   printf(oled_printf, "%2u", RH_max);
   oled_printf("%");       
}

void setting_high_RH_mode_2()
{
   if(!input(pin_b5))
   {
      delay_us(10);
      if(!input(pin_b5))
      {
         RH_max++;
         if(RH_max == 81)
            RH_max = 60;
      }
      while(!input(pin_b5));
   }
   
   if(!input(pin_b6))
   {
      delay_us(10);
      if(!input(pin_b6))
      {
         RH_max--;
         if(RH_max == 59)
            RH_max = 80;
      }
      while(!input(pin_b6));
   }  
}

void mang_nhiet_do_oled_mode_3()
{   
   ao_T = 0;
   oled_goto_xy(2,1);
   oled_printf("TEMPERATURE UPDATED");
   for (i = 0; i<7; i++)
   {
      oled_goto_xy(3*i+1, 3);
      printf(oled_printf, "%u", T_read[i]);
      
      oled_goto_xy(3*i+1, 5);
      printf(oled_printf, "%u", T_read[i+7]);
      
      ao_T = ao_T + T_read[i] + T_read[i+7];
   }
   ao_T /= 14;
   oled_goto_xy(1, 7);
   oled_printf("-> Average: ");
   printf(oled_printf, "%.2f", ao_T);
   OLED_PutCustomC(degree);
   oled_printf("C");
}

void mang_do_am_oled_mode_4()
{
   ao_RH = 0;
   oled_goto_xy(3,1);
   oled_printf("HUMIDITY UPDATED");
   for (i = 0; i<7; i++)
   {
      oled_goto_xy(3*i+1, 3);
      printf(oled_printf, "%u", RH_read[i]);
      
      oled_goto_xy(3*i+1, 5);
      printf(oled_printf, "%u", RH_read[i+7]);
       
      ao_RH = ao_RH + RH_read[i] + RH_read[i+7];
   }
   ao_RH /= 14;
   oled_goto_xy(1, 7);
   oled_printf("-> Average: ");
   printf(oled_printf, "%.2f", ao_RH);
   oled_printf("%");
}

void dht11_read()
{
   RH_ng = read_data();
   RH_tp = read_data();         
   T_ng = read_data();
   T_tp  = read_data();       
   Checksum = read_data();
   setup_timer_1(T1_DISABLED);
         
   if(CheckSum == ((RH_ng + RH_tp + T_ng + T_tp) & 0xFF))
   {
      oled_128x64_mode_0();
   }
}

void nhiet_do_vuot_nguong()
{
   if (T_ng <= T_max)
   {
      eeprom_write(64, hour);
      eeprom_write(65, minute);
      eeprom_write(66, second);
      eeprom_write(67, date);
      eeprom_write(68, month);
      eeprom_write(69, year);
      
      hour_Te = eeprom_read(72);
      minute_Te = eeprom_read(73);
      second_Te = eeprom_read(74);
      date_Te = eeprom_read(75);
      month_Te = eeprom_read(76);
      year_Te = eeprom_read(77);
   }
   else
   {
      eeprom_write(72, hour);
      eeprom_write(73, minute);
      eeprom_write(74, second);
      eeprom_write(75, date);
      eeprom_write(76, month);
      eeprom_write(77, year);
      
      hour_Ts = eeprom_read(64);
      minute_Ts = eeprom_read(65);
      second_Ts = eeprom_read(66);
      date_Ts = eeprom_read(67);
      month_Ts = eeprom_read(68);
      year_Ts = eeprom_read(69);
   }
}

void do_am_vuot_nguong()
{
   if (RH_ng <= RH_max)
   {
      eeprom_write(80, hour);
      eeprom_write(81, minute);
      eeprom_write(82, second);
      eeprom_write(83, date);
      eeprom_write(84, month);
      eeprom_write(85, year);
      
      hour_RHe = eeprom_read(88);
      minute_RHe = eeprom_read(89);
      second_RHe = eeprom_read(90);
      date_RHe = eeprom_read(91);
      month_RHe = eeprom_read(92);
      year_RHe = eeprom_read(93);
   }
   else
   {
      eeprom_write(88, hour);
      eeprom_write(89, minute);
      eeprom_write(90, second);
      eeprom_write(91, date);
      eeprom_write(92, month);
      eeprom_write(93, year);
      
      hour_RHs = eeprom_read(80);
      minute_RHs = eeprom_read(81);
      second_RHs = eeprom_read(82);
      date_RHs = eeprom_read(83);
      month_RHs = eeprom_read(84);
      year_RHs = eeprom_read(85);
   }
}

void lcd_20x04_8bit_mode_0()
{
   lcd_command(0x80);
   lcd_data("   TEMPERATURE      ");
   lcd_command(0xc0);
   lcd_data("        HUMIDITY    ");
   lcd_command(0x94);
   lcd_data("  MONITORING SYSTEM ");
   lcd_command(0xd4);
   lcd_data("                    ");
}

void lcd_20x04_8bit_mode_1()
{
   lcd_command(0x96);
   lcd_data(hour_Ts/10+48);
   lcd_data(hour_Ts%10+48);
   lcd_data(":");
   lcd_data(minute_Ts/10+48);
   lcd_data(minute_Ts%10+48);
   lcd_data(":");
   lcd_data(second_Ts/10+48);
   lcd_data(second_Ts%10+48);
   lcd_data(", ");
   lcd_data(date_Ts/10+48);
   lcd_data(date_Ts%10+48);
   lcd_data("/");
   lcd_data(month_Ts/10+48);
   lcd_data(month_Ts%10+48);
   lcd_data("/");
   lcd_data(year_Ts/10+48);
   lcd_data(year_Ts%10+48);
   lcd_command(0xd4);
   lcd_data(0x7e);
   
   if (T_ng > T_max)
   {
      lcd_command(0x80);
      lcd_data("  HIGH TEMPERATURE  ");
      lcd_command(0xc0);
      lcd_data("                    ");
      lcd_command(0xd6);
      lcd_data("--:--:--, --/--/--");
      for (int m = 0; m < 7; m++)
      {
         lcd_command(0xc3+m);
         lcd_data(">");
         lcd_command(0xc0+16-m);
         lcd_data("<");
         delay(20);
      }
   }
      
   if (T_ng <= T_max)
   {
      lcd_command(0xd6);
      lcd_data(hour_Te/10+48);
      lcd_data(hour_Te%10+48);
      lcd_data(":");
      lcd_data(minute_Te/10+48);
      lcd_data(minute_Te%10+48);
      lcd_data(":");
      lcd_data(second_Te/10+48);
      lcd_data(second_Te%10+48);
      lcd_data(", ");
      lcd_data(date_Te/10+48);
      lcd_data(date_Te%10+48);
      lcd_data("/");
      lcd_data(month_Te/10+48);
      lcd_data(month_Te%10+48);
      lcd_data("/");
      lcd_data(year_Te/10+48);
      lcd_data(year_Te%10+48);
   }
}

void lcd_20x04_8bit_mode_2()
{
   lcd_command(0x96);
   lcd_data(hour_RHs/10+48);
   lcd_data(hour_RHs%10+48);
   lcd_data(":");
   lcd_data(minute_RHs/10+48);
   lcd_data(minute_RHs%10+48);
   lcd_data(":");
   lcd_data(second_RHs/10+48);
   lcd_data(second_RHs%10+48);
   lcd_data(", ");
   lcd_data(date_RHs/10+48);
   lcd_data(date_RHs%10+48);
   lcd_data("/");
   lcd_data(month_RHs/10+48);
   lcd_data(month_RHs%10+48);
   lcd_data("/");
   lcd_data(year_RHs/10+48);
   lcd_data(year_RHs%10+48);
   lcd_command(0xd4);
   lcd_data(0x7e);
   
   if (RH_ng > RH_max)
   {
      lcd_command(0x80);
      lcd_data("   HIGH HUMIDITY    ");
      lcd_command(0xc0);
      lcd_data("                    ");
      lcd_command(0xd6);
      lcd_data("--:--:--, --/--/--");
      for (int m = 0; m < 7; m++)
      {
         lcd_command(0xc0+(9-m));
         lcd_data("<");
         lcd_command(0xc0+10+m);
         lcd_data(">");
         delay(20);
      }
   }
   
   if (RH_ng <= RH_max)
   {
      lcd_command(0xd6);
      lcd_data(hour_RHe/10+48);
      lcd_data(hour_RHe%10+48);
      lcd_data(":");
      lcd_data(minute_RHe/10+48);
      lcd_data(minute_RHe%10+48);
      lcd_data(":");
      lcd_data(second_RHe/10+48);
      lcd_data(second_RHe%10+48);
      lcd_data(", ");
      lcd_data(date_RHe/10+48);
      lcd_data(date_RHe%10+48);
      lcd_data("/");
      lcd_data(month_RHe/10+48);
      lcd_data(month_RHe%10+48);
      lcd_data("/");
      lcd_data(year_RHe/10+48);
      lcd_data(year_RHe%10+48);
   }
}

void main()
{   
   setup_oscillator(OSC_8MHZ);
   setup_timer_1(t1_disabled);
   port_b_pullups(0xf0);
   oled_init(ssd1306_switchcapvcc, ssd1306_i2c_address);
   lcd_setup();

   for (int k = 72; k < 78; k++)
   {
      eeprom_write(k, 0);
      eeprom_write(k+16, 0);
   }
   
   while(TRUE)
   {  
      i2c_read_data();      
      convert_data();
      select_mode();
      button_return_lcd();

      if (T_ng > T_max)
         lcd_mode = 1;
      if (RH_ng > RH_max)
         lcd_mode = 2;
         
      if (lcd_mode == 0)
         lcd_20x04_8bit_mode_0();
      else if (lcd_mode == 1)
         lcd_20x04_8bit_mode_1();
      else if (lcd_mode == 2) 
         lcd_20x04_8bit_mode_2();
      if (oled_mode == 0)
      {
         time_out = 0;
         Start_signal();
         if(check_response())    
            dht11_read();
         eeprom_write_data();
         nhiet_do_vuot_nguong();
         do_am_vuot_nguong();
      }
      else if (oled_mode == 1)
      {
         setting_high_T_mode_1();
         oled_128x64_mode_1();
      }
      else if (oled_mode == 2)
      {
         setting_high_RH_mode_2();
         oled_128x64_mode_2();
      }
      else if (oled_mode == 3)
      { 
         mang_nhiet_do_oled_mode_3();
         if(slot_read < 14)
         {
            T_read[slot_read] = eeprom_read(slot_read);
            slot_read++;
            if(slot_read == 14)  slot_read = 0;
         }
      }
      else if (oled_mode == 4)
      {
         mang_do_am_oled_mode_4();
         if(slot_read < 14)
         {
            RH_read[slot_read] = eeprom_read(slot_read+32);
            slot_read++;
            if(slot_read == 14)  slot_read = 0;
         }
      }       
      if (T_ng > T_max) output_high(pin_c0);
      else output_low(pin_c0);
      if (RH_ng > RH_max)  
      {
         output_high(pin_c1); delay(50);
         output_low(pin_c1);  delay(50);
      }
      else 
      {  
         output_low(pin_c1);  delay(100);
      }
   }
}

