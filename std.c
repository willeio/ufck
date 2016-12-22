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


void _memset(uint8_t* ptr, uint8_t val, uint8_t length)
{
  for (uint8_t i = 0; i < length; i++)
  {
    ptr[i] = val;
  }
}


void _strcpy(char* destination, const char* source)
{
  for (int i = 0; i < (UINT8_MAX - 1); i++)
  {
    destination[i] = source[i]; // copy, even the 0
    
    if (destination[i] == 0)
      return;
  }
  
  destination[UINT8_MAX] = 0; // sec
}


size_t _strlen(const char* txt)
{
  size_t s = 0;
  
  for (uint8_t i = 0; i < 255; i++)
  {
    if (txt[i] == 0)
      return s;
    
    s++;
  }
  
  return s;
}


void panic(const char id)
{
  println("");
  print(">> panic ");
  write(id);
  println("!");
  while(1){}
}
