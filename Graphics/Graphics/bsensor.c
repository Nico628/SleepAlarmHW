
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "wifi.h"

#include "Shapes.h"
#include "UsefulGraphics.h"

//C:/intelFPGA_lite/18.1/quartus/bin64/quartus_hps --cable="DE-SoC [USB-1]" -o GDBSERVER --gdbport0=3335 -preloader=C:/intelFPGA_lite/18.1/University_Program/Monitor_Program/arm_tools/u-boot-spl.de1-soc.srec -preloaderaddr=0xffff13a0
#define GPS_ReceiverFifo         			  (*(volatile unsigned char *)(0xFF210210))
#define GPS_TransmitterFifo      			  (*(volatile unsigned char *)(0xFF210210))
#define GPS_InterruptEnableReg   			  (*(volatile unsigned char *)(0xFF210212))
#define GPS_InterruptIdentificationReg        (*(volatile unsigned char *)(0xFF210214))
#define GPS_FifoControlReg                    (*(volatile unsigned char *)(0xFF210214))
#define GPS_LineControlReg                    (*(volatile unsigned char *)(0xFF210216))
#define GPS_ModemControlReg                   (*(volatile unsigned char *)(0xFF210218))
#define GPS_LineStatusReg                     (*(volatile unsigned char *)(0xFF21021A))
#define GPS_ModemStatusReg                    (*(volatile unsigned char *)(0xFF21021C))
#define GPS_ScratchReg                        (*(volatile unsigned char *)(0xFF21021E))

#define GPS_DivisorLatchLSB                   (*(volatile unsigned char *)(0xFF210210))
#define GPS_DivisorLatchMSB                   (*(volatile unsigned char *)(0xFF210212))

#define BioSensor_ReceiverFifo         			  (*(volatile unsigned char *)(0xFF210240))
#define BioSensor_TransmitterFifo      			  (*(volatile unsigned char *)(0xFF210240))
#define BioSensor_InterruptEnableReg   			  (*(volatile unsigned char *)(0xFF210242))
#define BioSensor_InterruptIdentificationReg        (*(volatile unsigned char *)(0xFF210244))
#define BioSensor_FifoControlReg                    (*(volatile unsigned char *)(0xFF210244))
#define BioSensor_LineControlReg                    (*(volatile unsigned char *)(0xFF210246))
#define BioSensor_ModemControlReg                   (*(volatile unsigned char *)(0xFF210248))
#define BioSensor_LineStatusReg                     (*(volatile unsigned char *)(0xFF21024A))
#define BioSensor_ModemStatusReg                    (*(volatile unsigned char *)(0xFF21024C))
#define BioSensor_ScratchReg                        (*(volatile unsigned char *)(0xFF21024E))

#define BioSensor_DivisorLatchLSB                   (*(volatile unsigned char *)(0xFF210240))
#define BioSensor_DivisorLatchMSB                   (*(volatile unsigned char *)(0xFF210242))

#define Wifi_ReceiverFifo         				(*(volatile unsigned char *)(0xFF210250))
#define Wifi_TransmitterFifo      				(*(volatile unsigned char *)(0xFF210250))
#define Wifi_InterruptEnableReg   				(*(volatile unsigned char *)(0xFF210252))
#define Wifi_InterruptIdentificationReg        (*(volatile unsigned char *)(0xFF210254))
#define Wifi_FifoControlReg                    (*(volatile unsigned char *)(0xFF210254))
#define Wifi_LineControlReg                    (*(volatile unsigned char *)(0xFF210256))
#define Wifi_ModemControlReg                   (*(volatile unsigned char *)(0xFF210258))
#define Wifi_LineStatusReg                     (*(volatile unsigned char *)(0xFF21025A))
#define Wifi_ModemStatusReg                    (*(volatile unsigned char *)(0xFF21025C))
#define Wifi_ScratchReg                        (*(volatile unsigned char *)(0xFF21025E))

#define Wifi_DivisorLatchLSB                   (*(volatile unsigned char *)(0xFF210250))
#define Wifi_DivisorLatchMSB                   (*(volatile unsigned char *)(0xFF210252))

#define SWITCHES    (volatile unsigned int *)(0xFF200000)
#define PUSHBUTTONS (volatile unsigned int *)(0xFF200010)

#define LEDS        (volatile unsigned int *)(0xFF200020)
#define HEX0_1      (volatile unsigned int *)(0xFF200030)
#define HEX2_3      (volatile unsigned int *)(0xFF200040)
#define HEX4_5      (volatile unsigned int *)(0xFF200050)

int runlua(char *, char *);

void Init_GPS(void) {
	// set bit 7 of Line Control Register to 1, to gain access to the baud rate registers
	GPS_LineControlReg = GPS_LineControlReg | 0x80;
	// set Divisor latch (LSB and MSB) with correct value for required baud rate
	GPS_DivisorLatchLSB = 0x45;
	GPS_DivisorLatchMSB = 0x01;
	// set bit 7 of Line control register back to 0 and
	GPS_LineControlReg = GPS_LineControlReg & 0x7F;
	// program other bits in that reg for 8 bit data, 1 stop bit, no parity etc
	GPS_LineControlReg = 0x03;
	// Reset the Fifo�s in the FiFo Control Reg by setting bits 1 & 2
	GPS_FifoControlReg = 0x06;
	// Now Clear all bits in the FiFo control registers
	GPS_FifoControlReg = 0x0;
}

int putcharGPS(int c) {
	// wait for Transmitter Holding Register bit (5) of line status register to be '1'
	// indicating we can write to the device
	while((GPS_LineStatusReg & 0x20) != 0x20);
	// write character to Transmitter fifo register
	GPS_TransmitterFifo = c;
	// return the character we printed
	return c;
}

char getcharGPS( void ) {
	// wait for Data Ready bit (0) of line status register to be '1'
	while((GPS_LineStatusReg & 0x01) != 0x01);
	// read new character from ReceiverFiFo register
	char ans = GPS_ReceiverFifo;
	// return new character
	return ans;
}

// the following function polls the UART to determine if any character
// has been received. It doesn't wait for one, or read it, it simply tests
// to see if one is available to read from the FIFO

int GPSTestForReceivedData(void) {
	// if GPS_LineStatusReg bit 0 is set to 1
	//return TRUE, otherwise return FALSE
	if((GPS_LineStatusReg & 0x01) == 0x01)
		return 1;
	return 0;
}

//
// Remove/flush the UART receiver buffer by removing any unread characters
//

void GPSFlush(void) { // read til nothing
	// while bit 0 of Line Status Register == �1�
	//    read unwanted char out of fifo receiver buffer
    // return; // no more characters so return
	while((GPS_LineStatusReg & 0x01) == 0x01) {
		int temp = GPS_ReceiverFifo;
	}
	return;
}

void Init_BioSensor(void) {
	// set bit 7 of Line Control Register to 1, to gain access to the baud rate registers
	BioSensor_LineControlReg = BioSensor_LineControlReg | 0x80;
	// set Divisor latch (LSB and MSB) with correct value for required baud rate
	BioSensor_DivisorLatchLSB = 0x45;
	BioSensor_DivisorLatchMSB = 0x01;
	// set bit 7 of Line control register back to 0 and
	BioSensor_LineControlReg = BioSensor_LineControlReg & 0x7F;
	// program other bits in that reg for 8 bit data, 1 stop bit, no parity etc
	BioSensor_LineControlReg = 0x03;
	// Reset the Fifo�s in the FiFo Control Reg by setting bits 1 & 2
	BioSensor_FifoControlReg = 0x06;
	// Now Clear all bits in the FiFo control registers
	BioSensor_FifoControlReg = 0x0;
}

char getcharBioSensor( void ) {
	// wait for Data Ready bit (0) of line status register to be '1'
	while((BioSensor_LineStatusReg & 0x01) != 0x01);
	// read new character from ReceiverFiFo register
	char ans = BioSensor_ReceiverFifo;
	// return new character
	return ans;
}

// the following function polls the UART to determine if any character
// has been received. It doesn't wait for one, or read it, it simply tests
// to see if one is available to read from the FIFO

int BioSensorTestForReceivedData(void) {
	// if BioSensor_LineStatusReg bit 0 is set to 1
	//return TRUE, otherwise return FALSE
	if((BioSensor_LineStatusReg & 0x01) == 0x01)
		return 1;
	return 0;
}

//
// Remove/flush the UART receiver buffer by removing any unread characters
//

void BioSensorFlush(void) { // read til nothing
	// while bit 0 of Line Status Register == �1�
	//    read unwanted char out of fifo receiver buffer
    // return; // no more characters so return
	while((BioSensor_LineStatusReg & 0x01) == 0x01) {
		int temp = BioSensor_ReceiverFifo;
	}
	return;
}

void Init_Wifi(void) {
	// set bit 7 of Line Control Register to 1, to gain access to the baud rate registers
	Wifi_LineControlReg = Wifi_LineControlReg | 0x80;
	// set Divisor latch (LSB and MSB) with correct value for required baud rate
	Wifi_DivisorLatchLSB = 0x1B;//0x45;
	Wifi_DivisorLatchMSB = 0x00;//0x01;
	// set bit 7 of Line control register back to 0 and
	Wifi_LineControlReg = Wifi_LineControlReg & 0x7F;
	// program other bits in that reg for 8 bit data, 1 stop bit, no parity etc
	Wifi_LineControlReg = 0x03;
	// Reset the Fifo�s in the FiFo Control Reg by setting bits 1 & 2
	Wifi_FifoControlReg = 0x06;
	// Now Clear all bits in the FiFo control registers
	Wifi_FifoControlReg = 0x0;
}

int putcharWifi(int c) {
	// wait for Transmitter Holding Register bit (5) of line status register to be '1'
	// indicating we can write to the device
	while((Wifi_LineStatusReg & 0x20) != 0x20);
	// write character to Transmitter fifo register
	Wifi_TransmitterFifo = c;
	// return the character we printed
	return c;
}

int putString(char *str) {
	while(*str != NULL) {
		putcharWifi(*str);
		str++;
	}

	return 0;
}

int getcharWifi( void ) {
	// wait for Data Ready bit (0) of line status register to be '1'
	while((Wifi_LineStatusReg & 0x01) != 0x01);
	// read new character from ReceiverFiFo register
	int ans = Wifi_ReceiverFifo;
	// return new character
	return ans;
}

// the following function polls the UART to determine if any character
// has been received. It doesn't wait for one, or read it, it simply tests
// to see if one is available to read from the FIFO

int WifiTestForReceivedData(void) {
	// if Wifi_LineStatusReg bit 0 is set to 1
	//return TRUE, otherwise return FALSE
	if((Wifi_LineStatusReg & 0x01) == 0x01)
		return 1;
	return 0;
}


//
// Remove/flush the UART receiver buffer by removing any unread characters
//

void WifiFlush(void) { // read til nothing
	// while bit 0 of Line Status Register == �1�
	//    read unwanted char out of fifo receiver buffer
   // return; // no more characters so return
	while((Wifi_LineStatusReg & 0x01) == 0x01) {
		int temp = Wifi_ReceiverFifo;
	}
	return;
}

/*	Purpose: Get Time, Longitude, Latitude from GPS Chip.
 * 	Return: 37 characters (need to process later)
 */

char* get_GPS_Data(void) {
	Init_GPS();
	GPSFlush();
	while(GPSTestForReceivedData() != 1);
	char trial = getcharGPS();
	char *ret = malloc(37);
	if(!ret)
		return NULL;
	Repeat:
		while(trial != '$') {
			// repeat when trial is not $ sign
			trial = getcharGPS();
		}
		// if it is $ sign
		trial = getcharGPS();
		if(trial != 'G') {
			trial = getcharGPS();
			goto Repeat;
		}
		trial = getcharGPS();
		if(trial != 'P') {
			trial = getcharGPS();
			goto Repeat;
		}
		trial = getcharGPS();
		if(trial != 'G') {
			trial = getcharGPS();
			goto Repeat;
		}
		trial = getcharGPS();
		if(trial != 'G') {
			trial = getcharGPS();
			goto Repeat;
		}
		trial = getcharGPS();
		if(trial != 'A') {
			trial = getcharGPS();
			goto Repeat;
		}

	for(int i = 0; i < 37; i++) {
		ret[i] = getcharGPS();
	}

	return ret;
}

/*	Purpose: Get Pulse Rate, Oxygen level in blood
 * 	Return: 3 characters (need to process later)
 */
char* get_Bio_Data(void) {
	Init_BioSensor();
	while(BioSensorTestForReceivedData() != 1);
	char *ret = malloc(3);
	if(!ret)
		return NULL;

	char first_byte;
	char second_byte;
	char third_byte;
	char forth_byte;
	int pulse_rate;
	Repeat:
			first_byte = getcharBioSensor();
			if((first_byte & 0x80) != 0x80) {
				goto Repeat;
			}
			if((first_byte & 0x40) == 0x40) {
				goto Repeat;
			}
			if((first_byte & 0x20) == 0x20) {
				goto Repeat;
			}
			if((first_byte & 0x10) == 0x10) {
				goto Repeat;
			}
			if((first_byte & 0x08) == 0x08) {
				goto Repeat;
			}
			if((first_byte & 0x04) == 0x04) {
				goto Repeat;
			}

			second_byte = getcharBioSensor();
			if((second_byte & 0x80) == 0x80) {
				goto Repeat;
			}

			third_byte = getcharBioSensor();
			if((third_byte & 0x80) == 0x80) {
				goto Repeat;
			}

			forth_byte = getcharBioSensor();
			int SNSA = forth_byte & 0x08;

			if(SNSA) {
				goto Repeat;
			}

			pulse_rate =  (int)((first_byte & 0x03) << 7) + (int)(second_byte & 0x7F);

			if(pulse_rate == 511 || third_byte == 127) {
				goto Repeat;
			}
	ret[0] = first_byte;
	ret[1] = second_byte;
	ret[2] = third_byte;
	return ret;
}

/*	Purpose: Send message
 */

void send_Message(char* number, int* which_message) {
	Init_Wifi();
	WifiFlush();

	//text routine
	char buf[1024] = "";

	printf("Starting send text routine\n");
	int res = runlua("dofile(\"combined_lua.lua\")", buf);
	printf("%s\n", buf);

	if(*which_message == 0) {
		char msg1[18] = "I am running late.";
		char first_3[7] = {'(',number[0], number[1], number[2], ')', ' ', '\0'};
		char second_3[5] = {number[3], number[4], number[5],'-', '\0'};
		char last_4[5] = {number[6], number[7], number[8], number[9], '\0'};


		char product[80];
		strcpy(product, "send_sms(\"(226) 796-5201\",\"");
		strcat(product, first_3);
		strcat(product, second_3);
		strcat(product, last_4);
		strcat(product,"\",\"");
		strcat(product,msg1);
		strcat(product,"\")");
		res = runlua(product , buf);
	} else if(*which_message == 1) {
		char msg2[13] = "See you soon!";
		char first_3[7] = {'(',number[0], number[1], number[2], ')', ' ', '\0'};
		char second_3[5] = {number[3], number[4], number[5],'-', '\0'};
		char last_4[5] = {number[6], number[7], number[8], number[9], '\0'};


		char product[80];
		strcpy(product, "send_sms(\"(226) 796-5201\",\"");
		strcat(product, first_3);
		strcat(product, second_3);
		strcat(product, last_4);
		strcat(product,"\",\"");
		strcat(product,msg2);
		strcat(product,"\")");
		res = runlua(product , buf);
	} else if(*which_message == 2) {
		char msg3[18] = "I won't be coming.";
		char first_3[7] = {'(',number[0], number[1], number[2], ')', ' ', '\0'};
		char second_3[5] = {number[3], number[4], number[5],'-', '\0'};
		char last_4[5] = {number[6], number[7], number[8], number[9], '\0'};


		char product[80];
		strcpy(product, "send_sms(\"(226) 796-5201\",\"");
		strcat(product, first_3);
		strcat(product, second_3);
		strcat(product, last_4);
		strcat(product,"\",\"");
		strcat(product,msg3);
		strcat(product,"\")");
		res = runlua(product , buf);
	}  else if(*which_message == 3) {
		char msg4[17] = "Have a great day!";
		char first_3[7] = {'(',number[0], number[1], number[2], ')', ' ', '\0'};
		char second_3[5] = {number[3], number[4], number[5],'-', '\0'};
		char last_4[5] = {number[6], number[7], number[8], number[9], '\0'};


		char product[80];
		strcpy(product, "send_sms(\"(226) 796-5201\",\"");
		strcat(product, first_3);
		strcat(product, second_3);
		strcat(product, last_4);
		strcat(product,"\",\"");
		strcat(product,msg4);
		strcat(product,"\")");
		res = runlua(product , buf);
	}
	printf("%s\n", buf);
}

void print_All(void)
{
	// GPS processing
	char* gps_data = get_GPS_Data();
	char time_string[2] = {gps_data[1],gps_data[2]};
	int time_convert = atol(time_string);

	if(time_convert >= 8) {
		time_convert -= 8;
	} else {
		time_convert += 16;
	}
	// time
	char time_print[5] = {time_convert/10+'0',time_convert%10+'0',':',gps_data[3],gps_data[4]};

	for(int i = 0; i < 5; i++) {
		printf("%c", time_print[i]);
	}
	printf("\n");
	// latitude
	char lat_string[6] = {gps_data[14], gps_data[15], gps_data[17], gps_data[18], gps_data[19], gps_data[20]};
	long lat_decimal = atol(lat_string);
	lat_decimal = lat_decimal / 60;
	for(int i = 0; i < 10; i++) {
		if(lat_decimal/10 == 0)
			break;
		else
			lat_decimal = lat_decimal/10;
	}
	char lat_print[6] = {gps_data[12], gps_data[13],'.',((int)lat_decimal)+'0',' ',gps_data[22]};

	for(int i = 0; i < 6; i++) {
		printf("%c", lat_print[i]);
	}
	printf("\n");

	// longitude
	char long_string[6] = {gps_data[27], gps_data[28], gps_data[30], gps_data[31], gps_data[32], gps_data[33]};
	long long_decimal = atol(long_string);
	long_decimal = long_decimal / 60;
	for(int i = 0; i < 10; i++) {
		if(long_decimal/10 == 0)
			break;
		else
			long_decimal = long_decimal/10;
	}
	char long_print[7] = {gps_data[24], gps_data[25], gps_data[26],'.',((int)long_decimal)+'0',' ',gps_data[35]};

	for(int i = 0; i < 7; i++) {
		printf("%c", long_print[i]);
	}
	printf("\n");

	free(gps_data);

	// BioSensor Processing
	char* bio_data = get_Bio_Data();
	int pulse_rate =  (int)((bio_data[0] & 0x03) << 7) + (int)(bio_data[1] & 0x7F);

	if(pulse_rate/100 == 0) {
		char pulse_print[6] = {pulse_rate/10+'0', pulse_rate%10+'0', ' ', 'B','P','M'};
		for(int i = 0; i < 6; i++) {
			printf("%c", pulse_print[i]);
		}
		
		OutGraphicsStringFont2(60, 300, WHITE, WHITE, pulse_print, 0);
		printf("\n");
	} else {
		char pulse_print[7] = {pulse_rate/100+'0', pulse_rate/10+'0', pulse_rate%10+'0', ' ', 'B','P','M'};
		for(int i = 0; i < 7; i++) {
			printf("%c", pulse_print[i]);
		}
		
		OutGraphicsStringFont2(60, 300, WHITE, WHITE, pulse_print, 0);
		printf("\n");
	}

	if(((int)bio_data[2])/100 == 0) {
		char osr_print[3] = {((int) bio_data[2])/10+'0',((int) bio_data[2])%10+'0', '%'};
		for(int i = 0; i < 3; i++) {
			printf("%c", osr_print[i]);
		}
		
		OutGraphicsStringFont2(60, 330, WHITE, WHITE, osr_print, 0);

		printf("\n");
	} else {
		char osr_print[4] = {((int) bio_data[2])/100+'0' ,((int) bio_data[2])/10+'0',((int) bio_data[2])%10+'0', '%'};
		for(int i = 0; i < 4; i++) {
			printf("%c", osr_print[i]);
		}
		
		OutGraphicsStringFont2(60, 330, WHITE, WHITE, osr_print, 0);

		printf("\n");
	}

	free(bio_data);

	OutGraphicsStringFont4(50, 50, WHITE, WHITE, time_print, 0);
	OutGraphicsStringFont2(60, 220, WHITE, WHITE, lat_print, 0);
	OutGraphicsStringFont2(60, 250, WHITE, WHITE, long_print, 0);
	//OutGraphicsStringFont2(60, 300, WHITE, WHITE, pulse_print, 0);
}

int num7seg(char input) {
	int output;
	switch(input) {
	case '0': output = 0x0; /* 0 */
			break;
	case '1': output = 0x1; /* 1 */
			break;
	case '2': output = 0x2; /* 2 */
			break;
	case '3': output = 0x3; /* 3 */
			break;
	case '4': output = 0x4; /* 4 */
			break;
	case '5': output = 0x5; /* 5 */
			break;
	case '6': output = 0x6; /* 6 */
			break;
	case '7': output = 0x7; /* 7 */
			break;
	case '8': output = 0x8; /* 8 */
			break;
	case '9': output = 0x9; /* 9 */
			break;
	}

	return output;
}

void print_time(void) {
	// GPS processing
	char* gps_data = get_GPS_Data();
	char time_string[2] = {gps_data[1],gps_data[2]};
	int time_convert = (gps_data[1]-'0')*10 + (gps_data[2]-'0');

	if(time_convert >= 8) {
			time_convert -= 9;
		} else {
			time_convert += 17;
	}
	// time
//	char time_print[5] = {'1','3',':',gps_data[3],gps_data[4]};
	char time_print[5] = {time_convert/10+'0',time_convert%10+'0',':',gps_data[3],gps_data[4]};
	OutGraphicsStringFont4(50, 25, WHITE, WHITE, time_print, 0);
}

void send_biomed(void)
{
	// GPS processing
	char* gps_data = get_GPS_Data();

	char time_string[2] = {gps_data[1],gps_data[2]};
	int time_convert = (gps_data[1]-'0')*10 + (gps_data[2]-'0');

	if(time_convert >= 8) {
			time_convert -= 9;
		} else {
			time_convert += 17;
	}

	// BioSensor Processing
	char* bio_data = get_Bio_Data();
	BioSensorFlush();
	int pulse_rate =  (int)((bio_data[0] & 0x03) << 7) + (int)(bio_data[1] & 0x7F);

	int min = (gps_data[3]-'0') * 10 +( gps_data[4]-'0');

	send_sleep_data(pulse_rate, bio_data[2], time_convert, min);

	// display bpm
	drawRectangle(300, 25, 300, 60, 1, BLACK, BLACK);
	if(pulse_rate > 99) {
		char pulsePrint[7] = {pulse_rate/100+'0', (pulse_rate/10)%10+'0', pulse_rate%10+'0',' ','B','P','M'};
		OutGraphicsStringFont4(310, 25, WHITE, WHITE, pulsePrint, 0);
	} else {
		char pulsePrint[7] = {pulse_rate/10+'0', pulse_rate%10+'0',' ','B','P','M','\0'};
		OutGraphicsStringFont4(310, 25, WHITE, WHITE, pulsePrint, 0);
	}

	//display %
	drawRectangle(600, 25, 200, 60, 1, BLACK, BLACK);
	char OLPrint[5] = {bio_data[2]/10+'0', bio_data[2]%10+'0',' ','%','\0'};
	OutGraphicsStringFont4(610, 25, WHITE, WHITE, OLPrint, 0);






	free(bio_data);
	free(gps_data);
}
