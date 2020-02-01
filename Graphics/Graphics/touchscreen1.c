/*
 * touchscreen.c
 *
 *  Created on: Jan 31, 2019
 *      Author: user
 */

#include <stdio.h>
#include <stdlib.h>
#include "time.h"
#include "touchscreen1.h"
#include "UsefulGraphics.h"
#include "Shapes.h"
#include "bsensor.h"
#include "wifi.h"
#include "list_struct.h"

/* a data type to hold a point/coord */
//typedef struct { int x, y; } Point ;

/*****************************************************************************
** Initialise touch screen controller
*****************************************************************************/
int putcharRS232(int c) {
// wait for Transmitter Holding Register bit (5) of line status register to be '1'
// indicating we can write to the device
	while(1){
  int THRbit = (TS_LineStatusReg & (1 << 5)) >> 5;
  if (THRbit)
    break;
}

// write character to Transmitter fifo register
	TS_TransmitterFifo = c;

// return the character we printed
	return c;

}

unsigned char getcharRS232( void ) {
// wait for Data Ready bit (0) of line status register to be '1'
while(1){
  int DRbit = (TS_LineStatusReg & 1);
  if (DRbit)
    break;
}
// read new character from ReceiverFiFo register
	unsigned char c = TS_ReceiverFifo;

// return new character

	return c;
}

void Init_Touch(void)
{
// Program serial port to communicate with touchscreen
// send touchscreen controller an "enable touch" command


	 TS_LineControlReg = TS_LineControlReg | 0x80;

	TS_DivisorLatchLSB = 0x45;
	TS_DivisorLatchMSB = 0x1;

// set bit 7 of Line control register back to 0 and
	TS_LineControlReg = TS_LineControlReg & 0x7F;

// program other bits in that reg for 8 bit data, 1 stop bit, no parity etc
	TS_LineControlReg = TS_LineControlReg | ((1 << 2) - 1); // set word length
	TS_LineControlReg = TS_LineControlReg & 0xFB; // set stop bit to 0
	TS_LineControlReg = TS_LineControlReg & 0xF7; // disable parity check
	TS_LineControlReg = TS_LineControlReg & 0xBF; // set brake to normal operation (not sure what this means)


// Reset the Fifo�s in the FiFo Control Reg by setting bits 1 & 2
	TS_FifoControlReg = TS_FifoControlReg | (((1 << 2) - 1) << 1);

// Now Clear all bits in the FiFo control registers
// I guess "clear all bits" just mean zeroing all of them
	TS_FifoControlReg = TS_FifoControlReg & 0x00;

	putcharRS232(0x55);
	putcharRS232(0x01);
	putcharRS232(0x12);

}

// add a digit to phone number buffer
void addDigitToPhoneBuffer(char* phone_buffer, char ch) {
	for(int j = 0; j < 10; j++) {
		if(*(phone_buffer+j) == '?') {
			*(phone_buffer+j) = ch;
			// display
			displayPhoneDigits(phone_buffer);
			return;
		}
	}
}

// reset phone digit buffer
void resetPhoneDigitsBuffer(char* phone_buffer) {
	for(int j = 0; j < 10; j++) {
		*(phone_buffer+j) = '?';
	}
}

// display all the phone digits in the phone digit buffer
void displayPhoneDigits(char* buf) {
	int count;
		for(count = 0; count < 10; count++) {
			if(*(buf+count) == '?')
				break;
		}
		if(count==0) {
			return;
		} else if(count==1) {
			char arr[1] = {*(buf)};
			OutGraphicsStringFont4(308, 102, WHITE, WHITE, arr, 0);
		} else if(count==2) {
			char arr1[1] = {*(buf)};
			char arr2[1] = {*(buf+1)};
			OutGraphicsStringFont4(308, 102, WHITE, WHITE, arr1, 0);
			OutGraphicsStringFont4(341, 102, WHITE, WHITE, arr2, 0);
		} else if(count==3) {
			char arr1[1] = {*(buf)};
			char arr2[1] = {*(buf+1)};
			char arr3[1] = {*(buf+2)};
			OutGraphicsStringFont4(308, 102, WHITE, WHITE, arr1, 0);
			OutGraphicsStringFont4(341, 102, WHITE, WHITE, arr2, 0);
			OutGraphicsStringFont4(374, 102, WHITE, WHITE, arr3, 0);
		} else if(count==4) {
			char arr1[1] = {*(buf)};
			char arr2[1] = {*(buf+1)};
			char arr3[1] = {*(buf+2)};
			char arr4[1] = {*(buf+3)};
			OutGraphicsStringFont4(308, 102, WHITE, WHITE, arr1, 0);
			OutGraphicsStringFont4(341, 102, WHITE, WHITE, arr2, 0);
			OutGraphicsStringFont4(374, 102, WHITE, WHITE, arr3, 0);

			OutGraphicsStringFont4(428, 102, WHITE, WHITE, arr4, 0);
		} else if(count==5) {
			char arr1[1] = {*(buf)};
			char arr2[1] = {*(buf+1)};
			char arr3[1] = {*(buf+2)};
			char arr4[1] = {*(buf+3)};
			char arr5[1] = {*(buf+4)};
			OutGraphicsStringFont4(308, 102, WHITE, WHITE, arr1, 0);
			OutGraphicsStringFont4(341, 102, WHITE, WHITE, arr2, 0);
			OutGraphicsStringFont4(374, 102, WHITE, WHITE, arr3, 0);

			OutGraphicsStringFont4(428, 102, WHITE, WHITE, arr4, 0);
			OutGraphicsStringFont4(461, 102, WHITE, WHITE, arr5, 0);
		} else if(count==6) {
			char arr1[1] = {*(buf)};
			char arr2[1] = {*(buf+1)};
			char arr3[1] = {*(buf+2)};
			char arr4[1] = {*(buf+3)};
			char arr5[1] = {*(buf+4)};
			char arr6[1] = {*(buf+5)};
			OutGraphicsStringFont4(308, 102, WHITE, WHITE, arr1, 0);
			OutGraphicsStringFont4(341, 102, WHITE, WHITE, arr2, 0);
			OutGraphicsStringFont4(374, 102, WHITE, WHITE, arr3, 0);

			OutGraphicsStringFont4(428, 102, WHITE, WHITE, arr4, 0);
			OutGraphicsStringFont4(461, 102, WHITE, WHITE, arr5, 0);
			OutGraphicsStringFont4(494, 102, WHITE, WHITE, arr6, 0);
		} else if(count==7) {
			char arr1[1] = {*(buf)};
			char arr2[1] = {*(buf+1)};
			char arr3[1] = {*(buf+2)};
			char arr4[1] = {*(buf+3)};
			char arr5[1] = {*(buf+4)};
			char arr6[1] = {*(buf+5)};
			char arr7[1] = {*(buf+6)};
			OutGraphicsStringFont4(308, 102, WHITE, WHITE, arr1, 0);
			OutGraphicsStringFont4(341, 102, WHITE, WHITE, arr2, 0);
			OutGraphicsStringFont4(374, 102, WHITE, WHITE, arr3, 0);

			OutGraphicsStringFont4(428, 102, WHITE, WHITE, arr4, 0);
			OutGraphicsStringFont4(461, 102, WHITE, WHITE, arr5, 0);
			OutGraphicsStringFont4(494, 102, WHITE, WHITE, arr6, 0);

			OutGraphicsStringFont4(548, 102, WHITE, WHITE, arr7, 0);
		} else if(count==8) {
			char arr1[1] = {*(buf)};
			char arr2[1] = {*(buf+1)};
			char arr3[1] = {*(buf+2)};
			char arr4[1] = {*(buf+3)};
			char arr5[1] = {*(buf+4)};
			char arr6[1] = {*(buf+5)};
			char arr7[1] = {*(buf+6)};
			char arr8[1] = {*(buf+7)};
			OutGraphicsStringFont4(308, 102, WHITE, WHITE, arr1, 0);
			OutGraphicsStringFont4(341, 102, WHITE, WHITE, arr2, 0);
			OutGraphicsStringFont4(374, 102, WHITE, WHITE, arr3, 0);

			OutGraphicsStringFont4(428, 102, WHITE, WHITE, arr4, 0);
			OutGraphicsStringFont4(461, 102, WHITE, WHITE, arr5, 0);
			OutGraphicsStringFont4(494, 102, WHITE, WHITE, arr6, 0);

			OutGraphicsStringFont4(548, 102, WHITE, WHITE, arr7, 0);
			OutGraphicsStringFont4(581, 102, WHITE, WHITE, arr8, 0);
		} else if(count==9) {
			char arr1[1] = {*(buf)};
			char arr2[1] = {*(buf+1)};
			char arr3[1] = {*(buf+2)};
			char arr4[1] = {*(buf+3)};
			char arr5[1] = {*(buf+4)};
			char arr6[1] = {*(buf+5)};
			char arr7[1] = {*(buf+6)};
			char arr8[1] = {*(buf+7)};
			char arr9[1] = {*(buf+8)};
			OutGraphicsStringFont4(308, 102, WHITE, WHITE, arr1, 0);
			OutGraphicsStringFont4(341, 102, WHITE, WHITE, arr2, 0);
			OutGraphicsStringFont4(374, 102, WHITE, WHITE, arr3, 0);

			OutGraphicsStringFont4(428, 102, WHITE, WHITE, arr4, 0);
			OutGraphicsStringFont4(461, 102, WHITE, WHITE, arr5, 0);
			OutGraphicsStringFont4(494, 102, WHITE, WHITE, arr6, 0);

			OutGraphicsStringFont4(548, 102, WHITE, WHITE, arr7, 0);
			OutGraphicsStringFont4(581, 102, WHITE, WHITE, arr8, 0);
			OutGraphicsStringFont4(614, 102, WHITE, WHITE, arr9, 0);
		} else {
			char arr1[1] = {*(buf)};
			char arr2[1] = {*(buf+1)};
			char arr3[1] = {*(buf+2)};
			char arr4[1] = {*(buf+3)};
			char arr5[1] = {*(buf+4)};
			char arr6[1] = {*(buf+5)};
			char arr7[1] = {*(buf+6)};
			char arr8[1] = {*(buf+7)};
			char arr9[1] = {*(buf+8)};
			char arr10[1] = {*(buf+9)};
			OutGraphicsStringFont4(308, 102, WHITE, WHITE, arr1, 0);
			OutGraphicsStringFont4(341, 102, WHITE, WHITE, arr2, 0);
			OutGraphicsStringFont4(374, 102, WHITE, WHITE, arr3, 0);

			OutGraphicsStringFont4(428, 102, WHITE, WHITE, arr4, 0);
			OutGraphicsStringFont4(461, 102, WHITE, WHITE, arr5, 0);
			OutGraphicsStringFont4(494, 102, WHITE, WHITE, arr6, 0);

			OutGraphicsStringFont4(548, 102, WHITE, WHITE, arr7, 0);
			OutGraphicsStringFont4(581, 102, WHITE, WHITE, arr8, 0);
			OutGraphicsStringFont4(614, 102, WHITE, WHITE, arr9, 0);
			OutGraphicsStringFont4(648, 102, WHITE, WHITE, arr10, 0);
		}
}

/*****************************************************************************
* This function waits for a touch screen press event and returns X,Y coord
*****************************************************************************/
int GetPress(int* input, char* alarm_buffer, char* phone_buffer, Alarm* alarm_list, int* date, int* alarm_number_tracker, int* which_message, Weather* weather, News* news, SleepButton* sleep_button, Event* event_list)
{
	Point p1;
// wait for a pen down command then return the X,Y coord of the point
// calibrated correctly so that it maps to a pixel on screen

	 unsigned char a=getcharRS232();

	 unsigned char a2=getcharRS232();

	 unsigned char b=getcharRS232();

	 unsigned char b2=getcharRS232();

	 int ta=(a2<<7|a);
	 int tb=(b2<<7|b);
	 p1.x=(ta*(799-80))/(4095-80);
	 p1.y=(tb*479)/4095;

	 // the point is valid:
	 if(p1.y<=479 && p1.x<=799){
		 if(*input==20) { // FUNCTION
			 	 	 	if(p1.x>244 && p1.x<454 && p1.y>345 && p1.y<415) { // if going to bed button pressed
			 				*sleep_button->on = 1;
			 				*sleep_button->timer = clock();
			 				clearScreen();
			 				char msg[42] = "Good Night, Press Me When You Wake Up.....";
			 				OutGraphicsStringFont3(35, 85, WHITE, WHITE, msg, 0);
			 				send_biomed();

							return 120;
			 			}
			 			// Sending message button
			 			else if (p1.x>40 && p1.x<190 && p1.y>425 && p1.y<475){
			 				drawMainScreen(50, alarm_list,weather,news,event_list);
			 				return 50;
			 			}
			 			// event button
			 			else if (p1.x>40 && p1.x<190 && p1.y>325 && p1.y<375) {
			 				drawMainScreen(30, alarm_list,weather,news,event_list);
			 				return 30;
			 			}
			 			// news button
			 			else if (p1.x>40 && p1.x<190 && p1.y>375 && p1.y<425) {
			 				drawMainScreen(40, alarm_list,weather,news,event_list);
			 				return 40;
			 			}
		 } else if(*input == 30) { // event page // FUNCTION
			 	// back button && home button
			 	if ((p1.x>180 && p1.x<240 && p1.y<90) || (p1.x>40 && p1.x<190 && p1.y>275 && p1.y<325)){
					drawMainScreen(20, alarm_list,weather,news,event_list);
					return 20;
			 	}
	 			// Sending message button
	 			else if (p1.x>40 && p1.x<190 && p1.y>425 && p1.y<475){
	 				drawMainScreen(50, alarm_list,weather,news,event_list);
	 				return 50;
	 			}
	 			// news button
	 			else if (p1.x>40 && p1.x<190 && p1.y>375 && p1.y<425) {
	 				drawMainScreen(40, alarm_list,weather,news,event_list);
	 				return 40;
	 			}
		 } else if(*input == 40) { // news screen // FUNCTION
			 	// back button && home button
			 	if ((p1.x>180 && p1.x<240 && p1.y<90) || (p1.x>40 && p1.x<190 && p1.y>275 && p1.y<325)){
					drawMainScreen(20, alarm_list,weather,news,event_list);
					return 20;
			 	}
	 			// Sending message button
	 			else if (p1.x>40 && p1.x<190 && p1.y>425 && p1.y<475){
	 				drawMainScreen(50, alarm_list,weather,news,event_list);
	 				return 50;
	 			}
	 			// event button
	 			else if (p1.x>40 && p1.x<190 && p1.y>325 && p1.y<375) {
	 				drawMainScreen(30, alarm_list,weather,news,event_list);
	 				return 30;
	 			}
		 } else if(*input == 50) { // message screen // FUNCTION
		 	 // phone digits entering
			 if(p1.x>480 && p1.x<750 && p1.y>204 && p1.y<274) { // first message selected
				 defaultMessages();
				 drawRectangle(480, 204, 280, 70, 1, DARKGRAY1, BLACK);
				 char msg1[18] = "I am running late.";
				 OutGraphicsStringFont2(490, 214, WHITE, WHITE, msg1, 0);
				 *which_message = 0;
			 } else if(p1.x>480 && p1.x<750 && p1.y>274 && p1.y<344) { // second message selected
				 defaultMessages();
				 drawRectangle(480, 274, 280, 70, 1, DARKGRAY1, BLACK);
				 char msg2[13] = "See you soon!";
				 OutGraphicsStringFont2(490, 284, WHITE, WHITE, msg2, 0);
				 *which_message = 1;
			 } else if(p1.x>480 && p1.x<750 && p1.y>344 && p1.y<414) { // third message selected
				 defaultMessages();
				 drawRectangle(480, 344, 280, 70, 1, DARKGRAY1, BLACK);
				 char msg3[18] = "I won't be coming.";
				 OutGraphicsStringFont2(490, 354, WHITE, WHITE, msg3, 0);
				 *which_message = 2;
			 } else if(p1.x>480 && p1.x<750 && p1.y>414 && p1.y<484) { // forth message selected
				 defaultMessages();
				 drawRectangle(480, 414, 280, 70, 1, DARKGRAY1, BLACK);
				 char msg4[17] = "Have a great day!";
				 OutGraphicsStringFont2(490, 424, WHITE, WHITE, msg4, 0);
				 *which_message = 3;
			 }
			 else if(p1.x>249-20 && p1.x<319-20 && p1.y>204 && p1.y<274) { // button 1
		 		 	addDigitToPhoneBuffer(phone_buffer, '1');
		 	 } else if(p1.x>325-20 && p1.x<395-20 && p1.y>204 && p1.y<274) { // button 2
			 		addDigitToPhoneBuffer(phone_buffer, '2');
		 	 } else if(p1.x>400-20 && p1.x<470-20 && p1.y>204 && p1.y<274) { // button 3
			 		addDigitToPhoneBuffer(phone_buffer, '3');
		 	 } else if(p1.x>249-20 && p1.x<319-20 && p1.y>275 && p1.y<319) { // button 4
			 		addDigitToPhoneBuffer(phone_buffer, '4');
		 	 } else if(p1.x>325-20 && p1.x<395-20 && p1.y>275 && p1.y<319) { // button 5
			 		addDigitToPhoneBuffer(phone_buffer, '5');
		 	 } else if(p1.x>400-20 && p1.x<470-20 && p1.y>275 && p1.y<319) { // button 6
			 		addDigitToPhoneBuffer(phone_buffer, '6');
		 	 } else if(p1.x>249-20 && p1.x<319-20 && p1.y>345 && p1.y<415) { // button 7
			 		addDigitToPhoneBuffer(phone_buffer, '7');
		 	 } else if(p1.x>325-20 && p1.x<395-20 && p1.y>345 && p1.y<415) { // button 8
			 		addDigitToPhoneBuffer(phone_buffer, '8');
		 	 } else if(p1.x>400-20 && p1.x<470-20 && p1.y>345 && p1.y<415) { // button 9
			 		addDigitToPhoneBuffer(phone_buffer, '9');
		 	 } else if(p1.x>249-20 && p1.x<319-20 && p1.y>415 && p1.y<485) { // button delete
		 		 for(int j = 9; j >= 0; j--) {
		 			 if(*(phone_buffer+j) != '?') {
		 					*(phone_buffer+j) = '?';
		 					// clear the digit spots
		 					// first 3 digits
		 				    drawRectangle(300, 90, 100, 50, 1, PURPLE1, BLACK);

		 				    // second 3 digits
		 				    drawRectangle(420, 90, 100, 50, 1, PURPLE1, BLACK);

		 				    // last 4 digits
		 				    drawRectangle(540, 90, 133, 50, 1, PURPLE1, BLACK);

		 					// display
		 					displayPhoneDigits(phone_buffer);
		 					return *input;
		 			 }
		 		 }
		 	 } else if(p1.x>325-20 && p1.x<395-20 && p1.y>415 && p1.y<485) { // button 0
			 		addDigitToPhoneBuffer(phone_buffer, '0');
		 	 }
			 	else if(p1.x>400-20 && p1.x<470-20 && p1.y>415 && p1.y<485) { // button enter
			 		// send message
			 		int flag = 0;
			 		for(int count = 0; count < 10; count++) {
			 			if(phone_buffer[count] == '?') {
			 				flag = 1;
			 				break;
			 			}
			 		}
			 		if(*which_message >= 0 && *which_message < 4 && flag == 0) { // a valid message
			 			drawRectangle(200, 120, 400, 240, 1, BLACK, WHITE);
			 			char sending[18] = "Message Sending...";
			 			OutGraphicsStringFont3(210, 130, WHITE, WHITE, sending, 0);
			 			send_Message(phone_buffer, which_message);
			 			resetPhoneDigitsBuffer(phone_buffer);
			 			*which_message = 4;
			 			clearScreen();
			 			char msg[15] = "Message Sent...";
			 			OutGraphicsStringFont3(35, 85, WHITE, WHITE, msg, 0);
			 			return 90;
			 		} else {
			 			resetPhoneDigitsBuffer(phone_buffer);
			 			*which_message = 4;
			 			clearScreen();
			 			char msg[30] = "Please Enter a Valid Number...";
			 			OutGraphicsStringFont3(35, 85, WHITE, WHITE, msg, 0);
			 			return 90;
			 		}

			 	}
			 	// back button && home button
			 	else if ((p1.x>180 && p1.x<240 && p1.y<90) || (p1.x>40 && p1.x<190 && p1.y>275 && p1.y<325)){
			 		resetPhoneDigitsBuffer(phone_buffer);
					drawMainScreen(20, alarm_list,weather,news,event_list);
					return 20;
			 	}
	 			// event button
	 			else if (p1.x>40 && p1.x<190 && p1.y>325 && p1.y<375) {
	 				resetPhoneDigitsBuffer(phone_buffer);
	 				drawMainScreen(30, alarm_list,weather,news,event_list);
	 				return 30;
	 			}
	 			// news button
	 			else if (p1.x>40 && p1.x<190 && p1.y>375 && p1.y<425) {
	 				resetPhoneDigitsBuffer(phone_buffer);
	 				drawMainScreen(40, alarm_list,weather,news,event_list);
	 				return 40;
	 			}
		 } else if(*input == 90) { // successfully sent message FUNCTION
			 drawMainScreen(50, alarm_list,weather,news,event_list);
			 return 50;
		 }
		 else if(*input == 120) { // Go to bed button Pressed FUNCTION
			 *sleep_button->on = 0;
			 drawMainScreen(20, alarm_list,weather,news,event_list);
			 return 20;
		 }
  	 }
	 return *input;
}

/*****************************************************************************
* This function waits for a touch screen release event and returns X,Y coord
*****************************************************************************/
Point GetRelease(void){

	Point p1;
// wait for a pen down command then return the X,Y coord of the point
// calibrated correctly so that it maps to a pixel on screen
	 unsigned char a=getcharRS232();

	 unsigned char a2=getcharRS232();

	 unsigned char b=getcharRS232();

	 unsigned char b2=getcharRS232();

	 int ta=(a2<<7|a);
	 int tb=(b2<<7|b);
	 p1.x=(ta*799)/4095;
	 p1.y=(tb*479)/4095;

	return p1;

}

/*****************************************************************************
** test if screen touched
*****************************************************************************/
int ScreenTouched( void )
{
// return TRUE if any data received from serial port connected to
// touch_screen or FALSE otherwise
 return (TS_LineStatusReg & 1);

}

/*****************************************************************************
** wait for screen to be touched
*****************************************************************************/
void WaitForTouch(double* timer, SleepButton* sleep_button, Alarm* alarm_list, int* screenNum, Weather* weather, News* news, Event* event_list)
{
	while(!ScreenTouched()) {
		// update time constantly
		if(((double)(clock() - *timer) / CLOCKS_PER_SEC) > 40) {
			*timer = clock();
			drawRectangle(50, 25, 150, 60, 1, BLACK, BLACK);
			char* gps_data = get_GPS_Data();
			char time_string[2] = {gps_data[1],gps_data[2]};

			int time_convert = (gps_data[1]-'0')*10 + (gps_data[2]-'0');

			if(time_convert >= 8) {
				time_convert -= 9;
			} else {
				time_convert += 17;
			}
			// time
			char time_print[5] = {time_convert/10+'0',time_convert%10+'0',':',gps_data[3],gps_data[4]};
			OutGraphicsStringFont4(50, 25, WHITE, WHITE, time_print, 0);




			// send data to database constantly
			if(*sleep_button->on == 1) {
				send_biomed();
			}
			// if user is at home page,
			if(*screenNum == 20) {
				updateAlarmList(alarm_list);
				displayAlarmList(alarm_list);
			}
		}
	}
}
