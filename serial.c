/*
  µfck
  Copyright (C) 2016 - Mike Wille <mike@wille.io>

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "std.h"
#include "serial.h"

#include <stddef.h>


#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define SERIAL_8N1 0x06


static const char* new_line = "\r\n";
volatile uint8_t *_ucsra;
volatile uint8_t *_ubrrh;
volatile uint8_t *_ubrrl;
volatile uint8_t *_ucsrb;
volatile uint8_t *_ucsrc;
volatile uint8_t *_udr;


void write(char c)
{
  loop_until_bit_is_set(UCSR0A, UDRE0); /* Wait until data register empty. */
  
  *_udr = (uint8_t)c;
  sbi(*_ucsra, TXC0);
}


// READ (TODO: don't use interrupts)
volatile char r;


char read()
{
  r = -1;
  
  while(r == -1)
  {}
  
  return r;
}


ISR(USART_RX_vect)
{
  r = (char)*_udr;
}
// READ END


void print(const char* txt)
{
  size_t s = _strlen(txt);
  
  for (uint8_t i = 0; i < s; i++)
  {
    write(txt[i]);
  }
}


void println(const char* txt)
{
  print(txt);
  print(new_line);
}


void serial_init(uint16_t baud)
{
  sei();
  
  // set timer 0 prescale factor to 64
  sbi(TCCR0B, CS01);
  sbi(TCCR0B, CS00);

  _ucsra = (uint8_t*)&UCSR0A;
  _ucsrb = (uint8_t*)&UCSR0B;
  _ucsrc = (uint8_t*)&UCSR0C;
  _ubrrh = (uint8_t*)&UBRR0H;
  _ubrrl = (uint8_t*)&UBRR0L;
  _udr   = (uint8_t*)&UDR0;
  
  uint16_t baud_setting = (F_CPU / 4 / baud - 1) / 2;
  *_ucsra = 1 << U2X0;

  // assign the baud_setting, a.k.a. ubrr (USART Baud Rate Register)
  *_ubrrh = baud_setting >> 8;
  *_ubrrl = baud_setting;

  //set the data bits, parity, and stop bits
  *_ucsrc = (SERIAL_8N1);
  
  sbi(*_ucsrb, RXEN0);
  sbi(*_ucsrb, TXEN0);
  sbi(*_ucsrb, RXCIE0);
  cbi(*_ucsrb, UDRIE0);
}