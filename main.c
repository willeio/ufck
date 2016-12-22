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


int main()
{
  serial_init(9600);
  
  println("\r<µfck - wille.io>"); // return needed to hide startup-fuckup
  
  brainfck_go();
 
  return 0;
}