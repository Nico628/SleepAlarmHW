#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "time.h"

#include "DemoGraphicsRoutines.h"
#include "Shapes.h"
#include "UsefulShapes.h"
#include "OutGraphicsCharFont1.h"
#include "touchscreen1.h"
#include "bsensor.h"
#include "list_struct.h"
#include "wifi.h"



/*******************************************************************************************
** This macro pauses until the graphics chip status register indicates that it is idle
*******************************************************************************************/

//Predefined Colour Values
//Use the symbolic constants below as the values to write to the Colour Register
//When you ask the graphics chip to draw something. These values are also returned
//By the read pixel command

// the header file "Colours.h" contains symbolic names for all 64colours e.g. RED
// while the source file ColourPaletteData.c contains the 24 bit RGB data
// that is pre-programmed into each of the 64 palettes


void main()
{
	// all events
	Event* event_list = malloc(10*sizeof(Event));
	get_calendar_data(event_list);

	// news
	News* news = malloc(10*sizeof(News));
	get_news(news);

	char* phone_buffer = malloc(10*sizeof(char));
	for(int i = 0; i < 10; i++) {
		*(phone_buffer+i) = '?';
	}

	char* alarm_buffer = malloc(4*sizeof(char));
	for(int j = 0; j < 4; j++) {
		*(alarm_buffer+j) = '?';
	}

	int* date = malloc(sizeof(int));
	*(date) = 8;

	Weather *weather = malloc(sizeof(Weather));
	Weather a;
	*weather = a;
	get_weather(weather);

	int* alarm_number_tracker = malloc(sizeof(int));
	*(alarm_number_tracker) = 4;

	int* which_message = malloc(sizeof(int));
	*(which_message) = 4;

	loadDefaultPalette();

//	// displayKeyboard();

	double* timer = malloc(sizeof(double));
	*timer = clock();

	SleepButton* sleep_button = malloc(sizeof(SleepButton));
	SleepButton but;
	*sleep_button = but;
	but.on = 0;

	Init_Touch();

	int i=0;
	int* screen_number = malloc(sizeof(int));
	*screen_number = 20;

	// alarm list
	Alarm* alarm_list = malloc(5*sizeof(Alarm));
	for(int k = 0; k < 5; k++) {
		Alarm a;
		a.valid = 0;
		*(alarm_list+k) = a;
	}
	updateAlarmList(alarm_list);
	drawMainScreen(20, alarm_list);


  while(1) {
	  WaitForTouch(timer, sleep_button, alarm_list, screen_number, weather, news, event_list);
	  int data=getcharRS232();

	  //touch
	  if(data==0x81 && i==0){
		*screen_number = GetPress(screen_number, alarm_buffer, phone_buffer, alarm_list, date, alarm_number_tracker, which_message, weather,news, sleep_button, event_list);
		i=1;
	  }
	  //release
	  else if(data==0x80){
		Point point2=GetRelease();
		i=0;
	  }
	}
}
