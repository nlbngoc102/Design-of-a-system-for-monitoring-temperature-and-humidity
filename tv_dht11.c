#define DHT11_PIN PIN_B0

short Time_out = 0;
unsigned int8 T_ng, T_tp, RH_ng, RH_tp, checksum;

void start_signal()
{
   output_drive(DHT11_PIN);
   output_low(DHT11_PIN);
   delay_ms(25);
   output_high(DHT11_PIN);
   delay_us(25);
   output_float(DHT11_PIN);
}

short check_response()
{
   set_timer1(0);
   setup_timer_1(T1_INTERNAL | T1_DIV_BY_2);
   while(!input(DHT11_PIN) && get_timer1() < 100);
   if(get_timer1() > 99)
      return 0;
   else
   {
      set_timer1(0);
      while(input(DHT11_PIN) && get_timer1() < 100);
      if(get_timer1() > 99)
         return 0;
      else
         return 1;
   }
}

unsigned int8 Read_Data()
{
   unsigned int8 i, _data = 0;
   if(Time_out)
      break;
   for(i = 0; i < 8; i++)
   {
      set_timer1(0);
      while(!input(DHT11_PIN))
         if(get_timer1() > 100)
         {
            Time_out = 1;
            break;
         }
      set_timer1(0);
      while(input(DHT11_PIN))
         if(get_timer1() > 100)
         {
            Time_out = 1;
            break;
         }
         if(get_timer1() > 50)
            bit_set(_data, (7 - i));
   }
   return _data;
}


