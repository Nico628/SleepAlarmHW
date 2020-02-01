// /**************************************************************************
// /* Subroutine to initialise the RS232 Port by writing some data
// ** to the internal registers.
// ** Call this function at the start of the program before you attempt
// ** to read or write to data via the RS232 port
// **
// ** Refer to UART data sheet for details of registers and programming
// ***************************************************************************/
// void Init_RS232(void)
// {
//  // set bit 7 of Line Control Register to 1, to gain access to the baud rate registers
//  // set Divisor latch (LSB and MSB) with correct value for required baud rate
//  // set bit 7 of Line control register back to 0 and
//  // program other bits in that reg for 8 bit data, 1 stop bit, no parity etc
//  // Reset the Fifo’s in the FiFo Control Reg by setting bits 1 & 2
//  // Now Clear all bits in the FiFo control registers
// }
// int putcharRS232(int c)
// {
//  // wait for Transmitter Holding Register bit (5) of line status register to be '1'
//  // indicating we can write to the device
//  // write character to Transmitter fifo register
//  // return the character we printed
// }
// int getcharRS232( void )
// {
//  // wait for Data Ready bit (0) of line status register to be '1'
//  // read new character from ReceiverFiFo register
//  // return new character
// }
// // the following function polls the UART to determine if any character
// // has been received. It doesn't wait for one, or read it, it simply tests
// // to see if one is available to read from the FIFO
// int RS232TestForReceivedData(void)
// {
//  // if RS232_LineStatusReg bit 0 is set to 1
//  //return TRUE, otherwise return FALSE
// }
// //
// // Remove/flush the UART receiver buffer by removing any unread characters
// //
// void RS232Flush(void)
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
// C:/intelFPGA_lite/18.1/quartus/bin64/quartus_hps --cable="DE-SoC [USB-1]" -o GDBSERVER --gdbport0=3335 -preloader=C:/intelFPGA_lite/18.1/University_Program/Monitor_Program/arm_tools/u-boot-spl.de1-soc.srec -preloaderaddr=0xffff13a0
#define RS232_ReceiverFifo         				(*(volatile unsigned char *)(0xFF210200))
#define RS232_TransmitterFifo      				(*(volatile unsigned char *)(0xFF210200))
#define RS232_InterruptEnableReg   				(*(volatile unsigned char *)(0xFF210202))
#define RS232_InterruptIdentificationReg        (*(volatile unsigned char *)(0xFF210204))
#define RS232_FifoControlReg                    (*(volatile unsigned char *)(0xFF210204))
#define RS232_LineControlReg                    (*(volatile unsigned char *)(0xFF210206))
#define RS232_ModemControlReg                   (*(volatile unsigned char *)(0xFF210208))
#define RS232_LineStatusReg                     (*(volatile unsigned char *)(0xFF21020A))
#define RS232_ModemStatusReg                    (*(volatile unsigned char *)(0xFF21020C))
#define RS232_ScratchReg                        (*(volatile unsigned char *)(0xFF21020E))

#define RS232_DivisorLatchLSB                   (*(volatile unsigned char *)(0xFF210200))
#define RS232_DivisorLatchMSB                   (*(volatile unsigned char *)(0xFF210202))


void Init_RS232(void) {
	// set bit 7 of Line Control Register to 1, to gain access to the baud rate registers
	RS232_LineControlReg = RS232_LineControlReg | 0x80;
	// set Divisor latch (LSB and MSB) with correct value for required baud rate
	RS232_DivisorLatchLSB = 0x45;
	RS232_DivisorLatchMSB = 0x01;
	// set bit 7 of Line control register back to 0 and
	RS232_LineControlReg = RS232_LineControlReg & 0x7F;
	// program other bits in that reg for 8 bit data, 1 stop bit, no parity etc
	RS232_LineControlReg = 0x03;
	// Reset the Fifo’s in the FiFo Control Reg by setting bits 1 & 2
	RS232_FifoControlReg = 0x06;
	// Now Clear all bits in the FiFo control registers
	RS232_FifoControlReg = 0x0;
}

int putcharRS232(int c) {
	// wait for Transmitter Holding Register bit (5) of line status register to be '1'
	// indicating we can write to the device
	while((RS232_LineStatusReg & 0x20) != 0x20);
	// write character to Transmitter fifo register
	RS232_TransmitterFifo = c;
	// return the character we printed
	return c;
}

int getcharRS232( void ) {
	// wait for Data Ready bit (0) of line status register to be '1'
	while((RS232_LineStatusReg & 0x01) != 0x01);
	// read new character from ReceiverFiFo register
	int ans = RS232_ReceiverFifo;
	// return new character
	return ans;
}

// the following function polls the UART to determine if any character
// has been received. It doesn't wait for one, or read it, it simply tests
// to see if one is available to read from the FIFO

int RS232TestForReceivedData(void) {
	// if RS232_LineStatusReg bit 0 is set to 1
	//return TRUE, otherwise return FALSE
	if((RS232_LineStatusReg & 0x01) == 0x01)
		return 1;
	return 0;
}

//
// Remove/flush the UART receiver buffer by removing any unread characters
//

void RS232Flush(void) { // read til nothing
	// while bit 0 of Line Status Register == ‘1’
	//    read unwanted char out of fifo receiver buffer
    // return; // no more characters so return
	while((RS232_LineStatusReg & 0x01) == 0x01) {
		int temp = RS232_ReceiverFifo;
	}
	return;
}


int main(void)
{
	Init_RS232();
	int num = putcharRS232(255);
	while(RS232TestForReceivedData() != 1);
	int ans = getcharRS232();
	if(num != ans)
		printf("Error, the words are not the same.");
	else
		printf("Success, words are the same.");
	RS232Flush();
	return 0;
}



