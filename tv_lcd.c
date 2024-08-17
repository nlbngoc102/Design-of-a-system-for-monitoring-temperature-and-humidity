#ifndef     lcd_rs
#define     lcd_rs         pin_e0                        //(1)
#endif

#ifndef     lcd_rw
#define     lcd_rw         pin_e1                        //(2)
#endif

#ifndef     lcd_e
#define     lcd_e          pin_e2                        //(3)
#endif

//co the viet cac dinh nghia tren ngan gon nhu sau:
// #define     lcd_rs      pin_e0                        <->(1)                   
// #define     lcd_rw      pin_e1                        <->(2)
// #define     lcd_e       pin_e2                        <->(3)


#ifndef     output_lcd  
#define     output_lcd        output_d
#endif

#define     lcd_function_set        0x38
#define     lcd_display_control     0x0c
#define     lcd_clear_display       0x01
#define     lcd_entry_mode          0x06
#define     lcd_shift_left          0x18
#define     lcd_shift_right         0x1c
#define     lcd_addr_line1          0x80
#define     lcd_addr_line2          0xc0

void lcd_out (unsigned char x)
{
   output_lcd(x);
   output_high(lcd_e);     delay_us(10);
   output_low(lcd_e);      delay_us(30);
}

void lcd_command (unsigned char mdk)
{
   output_low(lcd_rs);
   lcd_out(mdk);
}

void lcd_data (unsigned char mht)
{
   output_high(lcd_rs);
   lcd_out(mht);
}

void lcd_goto_xy (signed int8 x, signed int8 y)
{
   const unsigned int8 lcd_vitri[] = {0x80,0xc0,0x94,0xd4};
   lcd_command(lcd_vitri[x]+y);
}

void lcd_setup()
{
   output_low(lcd_e);
   output_low(lcd_rs);
   output_low(lcd_rw);
   lcd_command(lcd_function_set);
   lcd_command(lcd_display_control);
   lcd_command(lcd_clear_display);           delay_ms(2);
   lcd_command(lcd_entry_mode);
}



