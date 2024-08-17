#include <16f887.h>
#device  ADC=10
#fuses nowdt, put, hs, noprotect, nolvp, nobrownout, nomclr
#use delay (clock = 20m)
const unsigned char ma7doan [16] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e};

void set_up_ports (unsigned int8 porta, unsigned int8 portb, unsigned int8 portc, unsigned int8 portd, unsigned int8 porte)
{
   set_tris_a (porta);
   set_tris_b (portb);
   set_tris_c (portc);
   set_tris_d (portd);
   set_tris_e (porte);
}












