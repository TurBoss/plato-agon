/**
 * PLATOTerm - A PLATO Terminal for the Agon Light
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 * Ported to AgonLight by TurBoss 2025
 * 
 * screen.h - Display output functions
 */

#ifndef SCREEN_H
#define SCREEN_H

#include "protocol.h"

extern char enable_fill;

/**
 * screen_init() - Set up the screen
 */
void screen_init(void);

/**
 * screen_wait(void) - Sleep for approx 16.67ms
 */
void screen_wait(void);

/**
 * screen_beep(void) - Beep the terminal
 */
void screen_beep(void);

/**
 * screen_clear - Clear the screen
 */
void screen_clear(void);

/**
 * screen_block_draw(Coord1, Coord2) - Perform a block fill from Coord1 to Coord2
 */
void screen_block_draw(padPt* Coord1, padPt* Coord2);

/**
 * screen_dot_draw(Coord) - Plot a mode 0 pixel
 */
void screen_dot_draw(padPt* Coord);

/**
 * screen_line_draw(Coord1, Coord2) - Draw a mode 1 line
 */
void screen_line_draw(padPt* Coord1, padPt* Coord2);

/**
 * screen_char_draw(Coord, ch, count) - Output buffer from ch* of length count as PLATO characters
 */
void screen_char_draw(padPt* Coord, unsigned char* ch, unsigned char count);

/**
 * screen_tty_char - Called to plot chars when in tty mode
 */
void screen_tty_char(padByte theChar);

/**
 * screen_foreground - Set foreground
 */
void screen_foreground(padRGB* theColor);

/**
 * screen_background - Set Background
 */
void screen_background(padRGB* theColor);

/**
 * screen_paint - Paint
 */
void screen_paint(padPt* Coord);

/**
 * screen_done()
 * Close down TGI
 */
void screen_done(void);

#endif /* SCREEN_H */
