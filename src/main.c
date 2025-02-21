#include <stdbool.h>
#include <input.h>
#include <stdint.h>
#include <stdio.h>
#include "protocol.h"
#include "screen.h"
#include "terminal.h"
#include "splash.h"
#include "help.h"

#include <conio.h>

#include "io.h"
#include "keyboard.h"

#include "sound.h"	//Added sound for Keyboard and ready beep

unsigned char already_started=0;

void main(void)
{
  bpoke(0x5C3B, bpeek(0x5C3B) | 0x8); /*Alistair's workaround to autoboot key mode issue */
  char c; 
  // zx_border(INK_BLACK);  //Tidy up the borders on start up
  screen_init();
  terminal_init();
  ShowPLATO(splash,sizeof(splash));
  terminal_ready();
  terminal_initial_position();
  
  help_clear();
  cprintf("Enable graphics fill?(y/n)");
  while(1)
  {
    // in_Pause(1); // Slow this loop
    c = getch();
    if (c == 'y') {
      enable_fill = 1;
      break;
    }
    if (c == 'n')  {
      enable_fill = 0;
      break;
    }
  }

  io_init();

#ifndef NO_BIT
  bit_play("2A--");  //Ready beep
#endif

  for (;;)
    {  
      /* CHANGES TO THE MAIN LOOP
          We now only check the keyboard on NO RX data 
          or while drawing on the screen, 
          except during flood fills.
      */

      io_main();
    }
}

