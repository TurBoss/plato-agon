/**
 * PLATOTerm - A PLATO Terminal for the Agon Light
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 * Ported to AgonLight by TurBoss 2025
 * 
 * keyboard.h - Keyboard functions
 */

#ifndef KEYBOARD_H
#define KEYBOARD_H

/**
 * A simple key press feedback.
 */
void keyboard_click(void);

/**
 * keyboard_out - If platoKey < 0x7f, pass off to protocol
 * directly. Otherwise, platoKey is an access key, and the
 * ACCESS key must be sent, followed by the particular
 * access key from PTAT_ACCESS.
 */
void keyboard_out(unsigned char platoKey);

/**
 * keyboard_main - Handle the keyboard presses
 */
void keyboard_main(void);

/**
 * keyboard_clear() - Clear the keyboard buffer
 */
void keyboard_clear(void);

/**
 * keyboard_out_tty - keyboard output to serial I/O in TTY mode
 */
void keyboard_out_tty(char ch);

#endif
