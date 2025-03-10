#include <stdio.h>
#include <string.h>
#include <rs232.h>
#include <conio.h>
#include <time.h>

#include "keyboard.h"
#include "io.h"
#include "protocol.h"
#include "help.h"

#define rxbuffersize 2048
#define rxdelay 2 //Small delay after fetching the first character to try and get all the data in one

extern void uart_init();
extern void uart_put(unsigned char);
extern unsigned char uart_get(void);

static unsigned char inb;
char rxdata[rxbuffersize];
int rxbufferlimit = rxbuffersize;
static int bytes;
char io_initialized = 0;
uint_fast8_t relaxbuffer = 0;

void io_init(void) {

	// PUT A CRAPPY MENU ON RS232 IO INIT
	char host_name[80];
	char c;

	help_prompt_input("Modem dial? y/n: ");

	// rs232_params(RS_BAUD_9600|RS_STOP_1|RS_BITS_8,RS_PAR_NONE);  //  Bauds tested 1200[/] 2400[/] 4800[/] 9600[/] 19200[X] 38400[X] 57600[] 115200[]
	// rs232_params(RS_BAUD_19200|RS_STOP_1|RS_BITS_8,RS_PAR_NONE);  //  Bauds tested 1200[/] 2400[/] 4800[/] 9600[/] 19200[X] 38400[X] 57600[] 115200[]

	uart_init();

	io_initialized = 1;



	while (1) {
		// in_Pause(1); // Slow this loop
		c = getch();
		if (c == 'y') {
			help_clear();
			help_prompt_input("Enter Hostname or <ENTER> for IRATA.ONLINE: ");
			cgets(host_name);
			if (strcmp(host_name, "") == 0) {
				strcpy(host_name, "192.168.10.218:8005");
				//  strcpy(host_name, "IRATA.ONLINE:8005");
			}
			help_clear();

			io_send_byte('A');
			io_send_byte('T');
			io_send_byte('D');
			io_send_byte('T');
			io_send_byte(' ');

			//Push CTS/RTS on
			//io_send_byte('&');
			//io_send_byte('K');
			//io_send_byte('3');
			//io_send_byte(' ');

			//io_send_byte('D');
			//io_send_byte('T');
			//io_send_byte(' ');

			int i = 0;
			do {
				io_send_byte(host_name[i]);
			} while (++i < strlen(host_name));
			io_send_byte(0x0a);
			break;
		} else if (c == 'n') {
			help_clear();
			break;
		}
	}

}

void io_send_byte(unsigned char b) {
	if (io_initialized == 1) {
		uart_put(b);	//*IRQ-OFF (SENDING DATA)
	}
}

void io_main(void) { // NEW ADAPTIVE BUFFERED SERIAL CONNECTION !!

	//rs232_get(&inb);
	inb = uart_get();

	if (inb != RS_ERR_NO_DATA)  	// *IRQ-OFF (RECEIVING DATA)
	{ /* [RX - Display] */
		bytes = 0;
		rxdata[bytes] = inb;
		bytes++;

		msleep(rxdelay);  // Hold off to get a buffer load

		// Mark screen while RX in progress
		//	  gotoxy(0,0);
		//    if(rxbufferlimit==rxbuffersize)		printf("X");  // BIG buffer active
		//    else		                          printf("O");  // small buffer active

		while (inb != RS_ERR_NO_DATA && bytes < rxbufferlimit) {// LOOP until no data or Buffer is FULL
			keyboard_main(); // On the off chance some pushes a key and we have the IRQs enabled...

			//rs232_get(&inb);

			inb = uart_get();
			if (inb != RS_ERR_NO_DATA) {
				cprintf("GOT DATA \0");
				cprintf("0x%s\r\n\0",inb);
				rxdata[bytes] = inb;
				bytes++;
				msleep(rxdelay);  // Hold off to get a buffer load
			} else {
				//printf("GOT NO DATA\r\n\0");
				inb = RS_ERR_NO_DATA;
			}
		}

		//  ADAPTIVE BUFFERING -- Try to not partially load pages, but if we blast the buffer in roses stream using a small buffer
		if (inb != RS_ERR_NO_DATA || (bytes > 5 && bytes < 20)) { //  Buffer flooded OR Buffer starved, switch to small buffer
			rxbufferlimit = 20;
		} else {  //  Buffer not flooded, switch to big buffer
			rxbufferlimit = rxbuffersize;
		}

		// Clear the RX data mark
		//    gotoxy(0,0);    printf(" ");
		//printf("%s", rxdata);
		ShowPLATO(rxdata, bytes);
		//ShowPLATO(&inb,1);  // ORIGINAL

		// Display buffer usage for DEBUG
		/*
		 gotoxy(0,23);
		 printf("[%4d]",bytes);
		 //printf("[%4d][%4d]",bytes,rxbufferlimit);
		 */
	} else { /* [NO RX - KEY scan] */

		// RELAX the adaptive buffer on count of 3 no data passes
		if (relaxbuffer < 3) {
			relaxbuffer++;
		} else {
			relaxbuffer = 0;
			rxbufferlimit = rxbuffersize;
		}

		for (int Kscan = 0; Kscan < 10; Kscan++) {// THIS IS THE MAIN KEYBOARD SCAN WINDOW NOW
												  // in_Pause(1);
			keyboard_main();
		}

	}
}
