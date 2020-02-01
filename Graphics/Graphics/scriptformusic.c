// /**************************************************************************
// /* Subroutine to initialise the Music Port by writing some data
// ** to the internal registers.
// ** Call this function at the start of the program before you attempt
// ** to read or write to data via the Music port
// **
// ** Refer to UART data sheet for details of registers and programming
// ***************************************************************************/
// void Init_Music(void)
// {
//  // set bit 7 of Line Control Register to 1, to gain access to the baud rate registers
//  // set Divisor latch (LSB and MSB) with correct value for required baud rate
//  // set bit 7 of Line control register back to 0 and
//  // program other bits in that reg for 8 bit data, 1 stop bit, no parity etc
//  // Reset the Fifo’s in the FiFo Control Reg by setting bits 1 & 2
//  // Now Clear all bits in the FiFo control registers
// }
// int putcharMusic(int c)
// {
//  // wait for Transmitter Holding Register bit (5) of line status register to be '1'
//  // indicating we can write to the device
//  // write character to Transmitter fifo register
//  // return the character we printed
// }
// int getcharMusic( void )
// {
//  // wait for Data Ready bit (0) of line status register to be '1'
//  // read new character from ReceiverFiFo register
//  // return new character
// }
// // the following function polls the UART to determine if any character
// // has been received. It doesn't wait for one, or read it, it simply tests
// // to see if one is available to read from the FIFO
// int MusicTestForReceivedData(void)
// {
//  // if Music_LineStatusReg bit 0 is set to 1
//  //return TRUE, otherwise return FALSE
// }
// //
// // Remove/flush the UART receiver buffer by removing any unread characters
// //
// void MusicFlush(void)
// {
//  // while bit 0 of Line Status Register == ‘1’
//  // read unwanted char out of fifo receiver buffer
//  // return; // no more characters so return
// }

/*
 * serialport.c
 *
 *  Created on: Jan 21, 2019
 *      Author: nico
 */

#include <stdio.h>
   #include <string.h>
// C:/intelFPGA_lite/18.1/quartus/bin64/quartus_hps --cable="DE-SoC [USB-1]" -o GDBSERVER --gdbport0=3335 -preloader=C:/intelFPGA_lite/18.1/University_Program/Monitor_Program/arm_tools/u-boot-spl.de1-soc.srec -preloaderaddr=0xffff13a0
#define Music_ReceiverFifo         				(*(volatile unsigned char *)(0xFF210200))
#define Music_TransmitterFifo      				(*(volatile unsigned char *)(0xFF210200))
#define Music_InterruptEnableReg   				(*(volatile unsigned char *)(0xFF210202))
#define Music_InterruptIdentificationReg        (*(volatile unsigned char *)(0xFF210204))
#define Music_FifoControlReg                    (*(volatile unsigned char *)(0xFF210204))
#define Music_LineControlReg                    (*(volatile unsigned char *)(0xFF210206))
#define Music_ModemControlReg                   (*(volatile unsigned char *)(0xFF210208))
#define Music_LineStatusReg                     (*(volatile unsigned char *)(0xFF21020A))
#define Music_ModemStatusReg                    (*(volatile unsigned char *)(0xFF21020C))
#define Music_ScratchReg                        (*(volatile unsigned char *)(0xFF21020E))

#define Music_DivisorLatchLSB                   (*(volatile unsigned char *)(0xFF210200))
#define Music_DivisorLatchMSB                   (*(volatile unsigned char *)(0xFF210202))


void Init_Music(void) {
	// set bit 7 of Line Control Register to 1, to gain access to the baud rate registers
	Music_LineControlReg = Music_LineControlReg | 0x80;
	// set Divisor latch (LSB and MSB) with correct value for required baud rate
	Music_DivisorLatchLSB = 0x45;
	
	Music_DivisorLatchMSB = 0x01;

	//0x45 and 0x01 for 9600 some shit

	//0x1B and 0x00 for 115200
	// set bit 7 of Line control register back to 0 and
	Music_LineControlReg = Music_LineControlReg & 0x7F;
	// program other bits in that reg for 8 bit data, 1 stop bit, no parity etc
	Music_LineControlReg = 0x03;
	// Reset the Fifo’s in the FiFo Control Reg by setting bits 1 & 2
	Music_FifoControlReg = 0x06;
	// Now Clear all bits in the FiFo control registers
	Music_FifoControlReg = 0x0;
}

int putcharMusic(int c) {
	// wait for Transmitter Holding Register bit (5) of line status register to be '1'
	// indicating we can write to the device
	while((Music_LineStatusReg & 0x20) != 0x20);
	// write character to Transmitter fifo register

	printf("What we transmitted %d and char %c \n", c,c);
	Music_TransmitterFifo = c;
	// return the character we printed
	return c;
}

int getcharMusic( void ) {
	// wait for Data Ready bit (0) of line status register to be '1'
	//while((Music_LineStatusReg & 0x01) != 0x01);
	// read new character from ReceiverFiFo register
	int ans = Music_ReceiverFifo;

	// return new character
	return ans;
}

// the following function polls the UART to determine if any character
// has been received. It doesn't wait for one, or read it, it simply tests
// to see if one is available to read from the FIFO

int MusicTestForReceivedData(void) {
	// if Music_LineStatusReg bit 0 is set to 1
	//return TRUE, otherwise return FALSE
	if((Music_LineStatusReg & 0x01) == 0x01)
		return 1;
	return 0;
}

//
// Remove/flush the UART receiver buffer by removing any unread characters
//

void MusicFlush(void) { // read til nothing
	// while bit 0 of Line Status Register == ‘1’
	//    read unwanted char out of fifo receiver buffer
    // return; // no more characters so return
	while((Music_LineStatusReg & 0x01) == 0x01) {
		int temp = Music_ReceiverFifo;
	}
	return;
}

void BTOutMessage(char *message){

	for(int i = 0; message[i] != '\0'; i++) {

		putcharMusic(message[i]);

	    //printf("%c", message[i]);

	}

	printf("\n");
}

void SignalToPlay(){
		 Init_Music();
	 	 //BTOutMessage('trigger'); // write string to BT device
		 putcharMusic('y');
		 //putcharMusic('\n');
		 //putcharMusic('e');
		 //BTOutMessage('trigger) ; // write string to BT device
		 //putcharMusic('\n');

}
