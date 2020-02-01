/*
 * bluetooth.c
 *
 *  Created on: Apr 2, 2019
 *      Author: nico
 */

#include <stdio.h>
#include <string.h>

#define BT_ReceiverFifo             			(*(volatile unsigned char *)(0xFF210200))
#define BT_TransmitterFifo          			(*(volatile unsigned char *)(0xFF210200))
#define BT_InterruptEnableReg      			(*(volatile unsigned char *)(0xFF210202))
#define BT_InterruptIdentificationReg        (*(volatile unsigned char *)(0xFF210204))
#define BT_FifoControlReg                    (*(volatile unsigned char *)(0xFF210204))
#define BT_LineControlReg                    (*(volatile unsigned char *)(0xFF210206))
#define BT_ModemControlReg                   (*(volatile unsigned char *)(0xFF210208))
#define BT_LineStatusReg                     (*(volatile unsigned char *)(0xFF21020A))
#define BT_ModemStatusReg                    (*(volatile unsigned char *)(0xFF21020C))
#define BT_ScratchReg                        (*(volatile unsigned char *)(0xFF21020E))

#define BT_DivisorLatchLSB                   (*(volatile unsigned char *)(0xFF210200))
#define BT_DivisorLatchMSB                   (*(volatile unsigned char *)(0xFF210202))


void Init_BT(void) {
// set bit 7 of Line Control Register to 1, to gain access to the baud rate registers
BT_LineControlReg = BT_LineControlReg | 0x80;
// set Divisor latch (LSB and MSB) with correct value for required baud rate
BT_DivisorLatchLSB = 0x1B;
BT_DivisorLatchMSB = 0x00;
// set bit 7 of Line control register back to 0 and
BT_LineControlReg = BT_LineControlReg & 0x7F;
// program other bits in that reg for 8 bit data, 1 stop bit, no parity etc
BT_LineControlReg = 0x03;
// Reset the Fifo’s in the FiFo Control Reg by setting bits 1 & 2
BT_FifoControlReg = 0x06;
// Now Clear all bits in the FiFo control registers
BT_FifoControlReg = 0x0;
}

int putcharBT(int c) {
// wait for Transmitter Holding Register bit (5) of line status register to be '1'
// indicating we can write to the device
while((BT_LineStatusReg & 0x20) != 0x20);
// write character to Transmitter fifo register

printf("What we transmitted %d and char %c \n", c);
BT_TransmitterFifo = c;
// return the character we printed
return c;
}

int getcharBT( void ) {
// wait for Data Ready bit (0) of line status register to be '1'
//while((BT_LineStatusReg & 0x01) != 0x01);
// read new character from ReceiverFiFo register
int ans = BT_ReceiverFifo;

// return new character
return ans;
}

// the following function polls the UART to determine if any character
// has been received. It doesn't wait for one, or read it, it simply tests
// to see if one is available to read from the FIFO

int BTTestForReceivedData(void) {
// if BT_LineStatusReg bit 0 is set to 1
//return TRUE, otherwise return FALSE
if((BT_LineStatusReg & 0x01) == 0x01)
 return 1;
return 0;
}

//
// Remove/flush the UART receiver buffer by removing any unread characters
//

void BTFlush(void) { // read til nothing
// while bit 0 of Line Status Register == ‘1’
//    read unwanted char out of fifo receiver buffer
    // return; // no more characters so return
while((BT_LineStatusReg & 0x01) == 0x01) {
 int temp = BT_ReceiverFifo;
}
return;
}

void BTOutMessage(char *message){

for(int i = 0; message[i] != '\0'; i++) {

 putcharBT(message[i]);

}

printf("\n");
}


int main(void)
{
Init_BT();

// wait for 1 second between command
// enter these commands in upper case
// $$$ enter command mode
// SF,1 factory reset
// SN,Device1 set device name to “Device1”
// SP,1234 set 4 digit pin to “1234”
// R,1<CR> reboot BT controller
char c, Message[100] ;
int i;
while(1){
 printf("\r\nEnter Message for Bluetooth Controller:") ;
 gets(Message); // get command string from user keyboard
 BTOutMessage(Message) ; // write string to BT device
 // if the command string was NOT "$$$" send \r\n
 if(strcmp(Message, "$$$") != 0) { // $$$ puts BT module into command mode
  putcharBT('\r') ;
  putcharBT('\n') ;
 }
 // now read back acknowledge string from device and display on console,
 // will timeout after no communication for about 2 seconds
 for(i = 0; i < 2000000; i ++) {


 if(BTTestForReceivedData() == 1) {
  c = getcharBT() ;



  printf(" decimal %d char %c \n",c,c );
  i=0 ; // reset timer if we got something back
  }
 }

}
return 0;
}

