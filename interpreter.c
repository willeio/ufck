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
#include "interpreter.h"


#define MEMORY_SIZE 255


char tape[MEMORY_SIZE];
uint8_t mem[MEMORY_SIZE];
uint8_t ptr;
uint8_t pc;
uint8_t stack[UINT8_MAX];
uint8_t stack_pos;


uint8_t find_end()
{
  uint8_t brackets = 1; // this open '[' counts also, so it starts with 1
  
  for (uint8_t i = (pc+1 /* ignore current bracket */); i < UINT8_MAX; i++)
  {
    switch (mem[i])
    {
      case '[':
      {
        brackets++;
        break;
      }
      
      case ']':
      {
        brackets--;
        break;
      }
      
      default:
      {
        break;
      }
    }
    
    if (brackets == 0) // biased to zero? then the coherant brackets was found
      return i;
  }
  
  panic('B'); // not Biased

  return -1;
}


void stack_push(uint8_t val)
{
  if (stack_pos > (UINT8_MAX-1))
    panic('O'); // overflow
  
  stack[stack_pos++] = val;
}


uint8_t stack_last()
{
  return stack[stack_pos-1];
}


uint8_t stack_pop()
{
  if (stack_pos == 0)
    panic('U'); // underflow
  
  uint8_t val = stack_last();
  stack_pos--;
  
  return val;
}


void brainfck_go()
{
  _memset((uint8_t*)&mem[0], 0, MEMORY_SIZE);
  
  _memset((uint8_t*)&tape[0], 0, MEMORY_SIZE);
  const char* code = "-[------->+<]>-.-[->+++++<]>++.+++++++..+++.[--->+<]>----."; // "+++++>+++++<[-[-]]";
  _strcpy(&tape[0], code);
  
  pc = 0;
  ptr = 0;
  stack_pos = 0;
  
  while (1)
  {
    char cmd = tape[pc];
    
    switch (cmd)
    {
      case '>':
      {
        ptr++;        
        break;
      }
      
      case '<':
      {
        ptr--;
        break;
      }
      
      case '+':
      {
        mem[ptr]++;
        break;
      }
      
      case '-':
      {
        mem[ptr]--;
        break;
      }
      
      case '.':
      {
        write(mem[ptr]);
        break;
      }
      
      case ',':
      {
        mem[ptr] = read();
        break;
      }
      
      case '[':
      {
        // check if ptr != 0, so that the loop can start, otherwise jump to loop's end
        if (mem[ptr] == 0) // exit loop before it starts
          pc = find_end(); // after ']'
        else // enter while
          stack_push(pc+1); // add start of loop's code to stack
        
        break;
      }
      
      case ']':
      {
        // exit loop if ptr == 0, restart loop if not done yet
        if (mem[ptr] == 0) // exit
          stack_pop();
        else // restart
          pc = stack_last()-1;
        
        break;
      }
      
      case 0:
      {
        panic('E'); // end, don't panic :)
        break; // sec
      }
      
      default:
      {
        panic('I'); // invalid cmd
        return;
        break; // sec
      }
    }
    
    pc++;
  }
}