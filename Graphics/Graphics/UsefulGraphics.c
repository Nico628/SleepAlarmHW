#include "Shapes.h"
#include "UsefulShapes.h"

#include "rain.h"
#include "keyboard.h"
#include "blue_plane.h"
#include "back-button.h"
#include "cloudsun.h"
#include "spotify.h"
#include "play.h"
#include "add-circle.h"
#include "sun.h"
#include "snow.h"
#include "music_vector.h"

#include "Palette.h"
#include "OutGraphicsCharFont2.h"
#include "OutGraphicsCharFont1.h"

#include "bsensor.h"
#include "list_struct.h"
#include "wifi.h"

// on = 1
// off = 0
void display_on_off_button(int y, int on, int Colour, int SquareColour){
	drawRectangle(725, y, 50, 20, 1, Colour, Colour);
	if (on == 1){
		drawRectangle(725+28, y + 2, 20, 16, 1, SquareColour, SquareColour);
		drawRectangle(727, y + 2, 20, 16, 1, GREEN3, GREEN3);
	} else {
		drawRectangle(727, y + 2, 20, 16, 1, SquareColour, SquareColour);
		drawRectangle(725+28, y + 2, 20, 16, 1, RED3, RED3);
	}
}


void decodeCloudSun(int start_x, int start_y){
	char xpmColorKeys[64] = " .+@#$%&*=-;>,')!~{]^/(_:<[}|1234567890abcdefghijklmnopqrstuvwxy";

	int draw_y = start_y;

	for (int y = 0; y < 50; y++){
		char *line = cloudsun_xpm[y];
		char *t;
		int x = start_x;
		for (t = line; *t != '\0'; t++){
			for (int i = 0; i < 55; i++){
				if (*t == xpmColorKeys[i]){
					WriteAPixel(x, draw_y, i);
					break;
				}
			}
			x++;
		}
		draw_y++;
	}
}

void displayAlarmBox(int Colour, Alarm a, int sequenceNum){
	char *monthName[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
	drawRectangle(499,sequenceNum*90 + 30, 300, 90, 1, Colour, Colour);
	if(a.on == 1) {
		display_on_off_button(sequenceNum*90 + 35 + 30, 1, DARKGRAY2, LIGHTGRAY2);
	} else {
		display_on_off_button(sequenceNum*90 + 35 + 30, 0, DARKGRAY2, LIGHTGRAY2);
	}

    char time[6];
    time[0] = (a.hour / 10) + '0';
    time[1] = (a.hour % 10) + '0';

    time[2] = ':';
    time[3] = (a.min / 10) + '0';
    time[4] = (a.min % 10) + '0';
    time[5] = '\0';

	OutGraphicsStringFont4(85 + 499, 17+sequenceNum*90 + 30, WHITE, WHITE, time, 0);

    char date[50];

    int i = 0;
    for (i = 0; a.date[i] != 0; i++){
        date[i] = a.date[i];
    }

	date[i] = ' ';
	i++;

    for (int j = 0; monthName[a.month-1][j] != 0; j++){
        date[i] = monthName[a.month-1][j];
		i++;
    }

	date[i] = ' ';
	i++;

	date[i] = a.day + '0';
	i++;
	while(i < 50) {
		date[i] = '\0';
		i++;
	}

    // *******************************

	OutGraphicsStringFont1(85 + 499, sequenceNum*90 + 62 + 30, WHITE, WHITE, date, 0);
	decodeCloudSun(499+15, sequenceNum*90 + 15 + 30);
}


// User has clicked on item x
void displayAlarmList(Alarm* alarm_list){
	drawRectangle(499, 0, 300, 400, 1, BLACK,BLACK);

	int colour_list[4] = {PURPLE2, PURPLE3, PURPLE4, PURPLE5};

	// Draw header bar
	drawRectangle(499, 0, 300, 30, 1, BLACK,BLACK);
	char alarm[] = "MY ALARMS";
	OutGraphicsStringFont2(80+499,8,WHITE, WHITE, alarm, 0);

	for (int i = 0; i < 5; i++){
		if(alarm_list[i].valid != 0) {
			displayAlarmBox(colour_list[i], alarm_list[i], i);
		}
	}
}


/*
 * 0: rain
 * 1: sunny
 * 2: snow
 * 3: cloudsun
 */
void decodeWeather(int start_x, int start_y, int weatherType){
	char xpmColorKeys[64] = " .+@#$%&*=-;>,')!~{]^/(_:<[}|1234567890abcdefghijklmnopqrstuvwxy";

	int draw_y = start_y;

	if (weatherType == 0){
		for (int y = 0; y < 53; y++){
			char *line = rain_xpm[y];
			char *t;
			int x = start_x;
			for (t = line; *t != '\0'; t++){
				for (int i = 0; i < 63; i++){
					if (*t == xpmColorKeys[i]){
						WriteAPixel(x, draw_y, i);
						break;
					}
				}
				x++;
			}
			draw_y++;
		}
	} else if (weatherType == 1){
		for (int y = 0; y < 50; y++){
			char *line = sun_xpm[y];
			char *t;
			int x = start_x;
			for (t = line; *t != '\0'; t++){
				for (int i = 0; i < 63; i++){
					if (*t == xpmColorKeys[i]){
						WriteAPixel(x, draw_y, i);
						break;
					}
				}
				x++;
			}
			draw_y++;
		}
	} else if (weatherType == 2){
		for (int y = 0; y < 50; y++){
			char *line = snow_xpm[y];
			char *t;
			int x = start_x;
			for (t = line; *t != '\0'; t++){
				for (int i = 0; i < 63; i++){
					if (*t == xpmColorKeys[i]){
						WriteAPixel(x, draw_y, i);
						break;
					}
				}
				x++;
			}
			draw_y++;
		}
	} else{
		decodeCloudSun(start_x, start_y);
	}



}

void decodeSpotify(int start_x, int start_y){
	char xpmColorKeys[64] = " .+@#$%&*=-;>,')!~{]^/(_:<[}|1234567890abcdefghijklmnopqrstuvwxy";

	int draw_y = start_y;

	for (int y = 0; y < 60; y++){
		char *line = music_vector_xpm[y];
		char *t;
		int x = start_x;
		for (t = line; *t != '\0'; t++){
			for (int i = 0; i < 60; i++){
				if (*t == xpmColorKeys[i]){
					WriteAPixel(x, draw_y, i);
					break;
				}
			}
			x++;
		}
		draw_y++;
	}
}


void decodePlane(int start_x, int start_y){
	char xpmColorKeys[64] = " .+@#$%&*=-;>,')!~{]^/(_:<[}|1234567890abcdefghijklmnopqrstuvwxy";

	int draw_y = start_y;

	for (int y = 0; y < 477; y++){
		char *line = blue_plane_xpm[y];
		char *t;
		int x = start_x;
		for (t = line; *t != '\0'; t++){
			for (int i = 0; i < 63; i++){
				if (*t == xpmColorKeys[i]){
					WriteAPixel(x, draw_y, i);
					break;
				}
			}
			x++;
		}
		draw_y++;
	}
}

void decodePartPlane(int start_x, int start_y){
	char xpmColorKeys[64] = " .+@#$%&*=-;>,')!~{]^/(_:<[}|1234567890abcdefghijklmnopqrstuvwxy";

	int draw_y = start_y;

	for (int y = 0; y < 105; y++){
		char *line = blue_plane_xpm[y];
		char *t;
		int x = start_x;
		for (t = line; *t != '\0'; t++){
			for (int i = 0; i < 63; i++){
				if (*t == xpmColorKeys[i]){
					WriteAPixel(x, draw_y, i);
					break;
				}
			}
			x++;
		}
		draw_y++;
	}
}

void decodeBackButton(int start_x, int start_y){
	char xpmColorKeys[64] = " .+@#$%&*=-;>,')!~{]^/(_:<[}|1234567890abcdefghijklmnopqrstuvwxy";

	int draw_y = start_y;

	for (int y = 0; y < 55; y++){
		char *line = back_button_xpm[y];
		char *t;
		int x = start_x;
		for (t = line; *t != '\0'; t++){
			for (int i = 0; i < 55; i++){
				if (*t == xpmColorKeys[i]){
					WriteAPixel(x, draw_y, i);
					break;
				}
			}
			x++;
		}
		draw_y++;
	}
}


void decodePlayButton(int start_x, int start_y){
	char xpmColorKeys[64] = " .+@#$%&*=-;>,')!~{]^/(_:<[}|1234567890abcdefghijklmnopqrstuvwxy";

	int draw_y = start_y;

	for (int y = 0; y < 40; y++){
		char *line = play_xpm[y];
		char *t;
		int x = start_x;
		for (t = line; *t != '\0'; t++){
			for (int i = 0; i < 40; i++){
				if (*t == xpmColorKeys[i]){
					WriteAPixel(x, draw_y, i);
					break;
				}
			}
			x++;
		}
		draw_y++;
	}
}

// void decodeImage(int start_x, int start_y, char* image_xpm, int width, int height){
// 	char xpmColorKeys[64] = " .+@#$%&*=-;>,')!~{]^/(_:<[}|1234567890abcdefghijklmnopqrstuvwxy";

// 	int draw_y = start_y;

// 	for (int y = 0; y < height; y++){
// 		char *line = image_xpm[y];
// 		char *t;
// 		int x = start_x;
// 		for (t = line; *t != '\0'; t++){
// 			for (int i = 0; i < width; i++){
// 				if (*t == image_xpm[i]){
// 					WriteAPixel(x, draw_y, i);
// 					break;
// 				}
// 			}
// 			x++;
// 		}
// 		draw_y++;
// 	}
// }

void loadDefaultPalette(){
	for (int i = 0; i < 64; i++){
		ProgramPalette(i, Palette[i]);
	}
}

void clearScreen(){
	for (int i = 0; i < HEIGHT; i++){
		HLine(0, i, WIDTH, BLACK);
	}
	return;
}

// default start_x = 630
//         start_y = 10
void drawWeatherBox(int start_x, int start_y, Weather* weather){
	drawRectangle(start_x, start_y, 160, 75, 1, BLACKYELLOW, BLACKYELLOW);

	if(weather->valid == 0) {
			get_weather(weather);
			weather->valid = 1;
	}
				char num[2];
				if(1) {
					strcpy(num, "4");
				}
				else if((weather->temperature - 273.15) < 0) {
					strcpy(num, "0");
				} else {
					sprintf(num, "%d", (int) (weather->temperature - 273.15));
				}

				char description[128];

				strcpy(description,"Clouds");
				strcat(description, " ");
				strcat(description, num);
				strcat(description, " C");
				if(1) {
					decodeWeather(start_x + 20, start_y + 10, 3);
				}
				else if(strcmp(weather->description, "Clouds") == 0 || strcmp(weather->description, "Mist") == 0) {
					decodeWeather(start_x + 20, start_y + 10, 3);
				} else if(strcmp(weather->description, "Snow") == 0) {
					decodeWeather(start_x + 20, start_y + 10, 2);
				} else if(strcmp(weather->description, "Rain") == 0 || strcmp(weather->description, "Drizzle") == 0 ) {
					decodeWeather(start_x + 20, start_y + 10, 0);
				} else if(strcmp(weather->description, "Clear") == 0) {
					decodeWeather(start_x + 20, start_y + 10, 1);
				} else {
					decodeWeather(start_x + 20, start_y + 10, 3);
				}

	OutGraphicsStringFont1(start_x + 75, start_y + 25,LIGHTGRAY2, BLACK, description, 0);
	char Location[] = "Vancouver, BC";
	OutGraphicsStringFont1(start_x + 75, start_y + 40,LIGHTGRAY2, BLACK, Location, 0);
}

void fillBackground(int Colour){
	for (int i = 0; i < HEIGHT; i++){
		HLine(0, i, WIDTH, Colour);
	}
	return;
}

void drawSideBar(Weather* weather){
	drawBox(0, 0, BLACK, BLACK, RED1, WIDTH/4, HEIGHT, 0, 0);
	print_time();

	char currDate[] = "Thursday, April 4";
	OutGraphicsMenuFont1(WHITE, 110, WHITE, currDate, 0);
	drawWeatherBox(20, 125, weather); // &&&&&&&&&&&&&&&&&&&&&&&&

	// Home BOX
	drawRectangle(40, 275, 150, 50, 0, BLUE1, BLUE1);
	drawRectangle(41, 276, 150, 50, 0, BLUE1, BLUE1);
	drawRectangle(42, 277, 150, 50, 0, BLUE1, BLUE1);
	char home[] = "HOME";
	OutGraphicsStringFont2(70, 300, WHITE, WHITE, home, 0);

	// Event BOX
	drawRectangle(40, 325, 150, 50, 0, BLUE1, BLUE1);
	drawRectangle(41, 326, 150, 50, 0, BLUE1, BLUE1);
	drawRectangle(42, 327, 150, 50, 0, BLUE1, BLUE1);
	char event[] = "EVENT";
	OutGraphicsStringFont2(70, 350, WHITE, WHITE, event, 0);

	// News BOX
	drawRectangle(40, 375, 150, 50, 0, BLUE1, BLUE1);
	drawRectangle(41, 376, 150, 50, 0, BLUE1, BLUE1);
	drawRectangle(42, 377, 150, 50, 0, BLUE1, BLUE1);
	char news[] = "NEWS";
	OutGraphicsStringFont2(70, 400, WHITE, WHITE, news, 0);

	// MESSAGE BOX
	drawRectangle(40, 425, 150, 50, 0, BLUE1, BLUE1);
	drawRectangle(41, 426, 150, 50, 0, BLUE1, BLUE1);
	drawRectangle(42, 427, 150, 50, 0, BLUE1, BLUE1);
	char message[] = "MESSAGE";
	OutGraphicsStringFont2(70, 450, WHITE, WHITE, message, 0);


	//print_All();

}

void drawNewsScreen(News* news) {
	drawRectangle(201, 0, 599, 480, 1, PURPLE3, PURPLE3);
	decodeBackButton(211,15);

	char title[11] = "Local News:";
	OutGraphicsStringFont4(350, 40, WHITE, WHITE, title, 0);

	char location[19] = "Location: Vancouver";
	OutGraphicsStringFont4(270, 80, WHITE, WHITE, location, 0);

	OutGraphicsStringFont1(210, 120+30, WHITE, WHITE, (news+4)->headline, 0);
	OutGraphicsStringFont1(210, 170+30, WHITE, WHITE, (news+5)->headline, 0);
	OutGraphicsStringFont1(210, 230+30, WHITE, WHITE, (news+6)->headline, 0);
	OutGraphicsStringFont1(210, 280+30, WHITE, WHITE, (news+7)->headline, 0);
	OutGraphicsStringFont1(210, 330+30, WHITE, WHITE, (news+8)->headline, 0);
	OutGraphicsStringFont1(210, 380+30, WHITE, WHITE, (news+9)->headline, 0);

}

void drawEventScreen(Event* event_list) {
	drawRectangle(201, 0, 599, 480, 1, PURPLE3, PURPLE3);
	decodeBackButton(211,15);

	char event[5] = "EVENT";
	OutGraphicsStringFont4(280, 40, WHITE, WHITE, event, 0);

	char start[5] = "START";
	OutGraphicsStringFont4(470+70, 40, WHITE, WHITE, start, 0);

	char end[3] = "END";
	OutGraphicsStringFont4(700, 40, WHITE, WHITE, end, 0);

	for(int i = 0; i < 10; i++) {
		if(event_list[i].last == 0) {
			OutGraphicsStringFont3(280-70, 40+50*(i+1), WHITE, WHITE, event_list[i].title, 0);
			OutGraphicsStringFont3(470+70, 40+50*(i+1), WHITE, WHITE, event_list[i].start, 0);
			OutGraphicsStringFont3(700, 40+50*(i+1), WHITE, WHITE, event_list[i].end, 0);
		} else if(i == 0) {
			OutGraphicsStringFont3(280-70, 40+50*(i+1), WHITE, WHITE, event_list[i].title, 0);
			return;
		} else {
			OutGraphicsStringFont3(280-70, 40+50*(i+1), WHITE, WHITE, event_list[i].title, 0);
			OutGraphicsStringFont3(470+70, 40+50*(i+1), WHITE, WHITE, event_list[i].start, 0);
			OutGraphicsStringFont3(700, 40+50*(i+1), WHITE, WHITE, event_list[i].end, 0);
			return;
		}
	}
}

void loadHomeScreen(Alarm* alarm_list, Weather* weather){
	clearScreen();
	displayAlarmList(alarm_list);
	drawSideBar(weather);
	decodePlane(201, 3);
	drawRectangle(244, 345, 210, 70, 1, BLACK, BLUE1);
	char msg[12] = "Going To Bed";
	OutGraphicsStringFont3(255, 365, WHITE, BLACK, msg, 0);
}

void defaultMessages(void) {
	drawRectangle(480, 204, 280, 80, 1, PURPLE1, BLACK);
	char msg1[18] = "I am running late.";
    OutGraphicsStringFont2(490, 214, BLACK, WHITE, msg1, 0);

	drawRectangle(480, 274, 280, 80, 1, PURPLE1, BLACK);
	char msg2[13] = "See you soon!";
    OutGraphicsStringFont2(490, 284, BLACK, WHITE, msg2, 0);

	drawRectangle(480, 344, 280, 80, 1, PURPLE1, BLACK);
	char msg3[18] = "I won't be coming.";
    OutGraphicsStringFont2(490, 354, BLACK, WHITE, msg3, 0);

	drawRectangle(480, 414, 280, 80, 1, PURPLE1, BLACK);
	char msg4[17] = "Have a great day!";
    OutGraphicsStringFont2(490, 424, BLACK, WHITE, msg4, 0);
}

// keypad block
void keyPad() {
	// 1
	drawRectangle(249, 204, 70, 70, 1, DARKGRAY2, BLACK);
	char n1[1] = "1";
	OutGraphicsStringFont3(275, 230, WHITE, DARKGRAY2, n1, 0);
	// 2
	drawRectangle(325, 204, 70, 70, 1, DARKGRAY2, BLACK);
	char n2[1] = "2";
	OutGraphicsStringFont3(346, 230, WHITE, DARKGRAY2, n2, 0);
	// 3
	drawRectangle(400, 204, 70, 70, 1, DARKGRAY2, BLACK);
	char n3[1] = "3";
	OutGraphicsStringFont3(416, 230, WHITE, DARKGRAY2, n3, 0);
	// 4
	drawRectangle(249, 275, 70, 70, 1, DARKGRAY2, BLACK);
	char n4[1] = "4";
	OutGraphicsStringFont3(275, 300, WHITE, DARKGRAY2, n4, 0);
	// 5
	drawRectangle(325, 275, 70, 70, 1, DARKGRAY2, BLACK);
	char n5[1] = "5";
	OutGraphicsStringFont3(346, 300, WHITE, DARKGRAY2, n5, 0);
	// 6
	drawRectangle(400, 275, 70, 70, 1, DARKGRAY2, BLACK);
	char n6[1] = "6";
	OutGraphicsStringFont3(416, 300, WHITE, DARKGRAY2, n6, 0);
	// 7
	drawRectangle(249, 345, 70, 70, 1, DARKGRAY2, BLACK);
	char n7[1] = "7";
	OutGraphicsStringFont3(275, 375, WHITE, DARKGRAY2, n7, 0);
	// 8
	drawRectangle(325, 345, 70, 70, 1, DARKGRAY2, BLACK);
	char n8[1] = "8";
	OutGraphicsStringFont3(346, 375, WHITE, DARKGRAY2, n8, 0);
	// 9
	drawRectangle(400, 345, 70, 70, 1, DARKGRAY2, BLACK);
	char n9[1] = "9";
	OutGraphicsStringFont3(416, 375, WHITE, DARKGRAY2, n9, 0);
	// delete
	drawRectangle(249, 415, 70, 70, 1, DARKGRAY2, BLACK);
	char delete[6] = "Delete";
	OutGraphicsStringFont1(275, 450, WHITE, DARKGRAY2, delete, 0);
	// 0
	drawRectangle(325, 415, 70, 70, 1, DARKGRAY2, BLACK);
	char n0[1] = "0";
	OutGraphicsStringFont3(346, 440, WHITE, DARKGRAY2, n0, 0);
	// enter
	drawRectangle(400, 415, 70, 70, 1, DARKGRAY2, BLACK);
	char enter[5] = " Save";
	OutGraphicsStringFont1(416, 450, WHITE, DARKGRAY2, enter, 0);

	return;
}

void drawMessageScreen(Weather* weather) {
	drawRectangle(201, 0, 599, 480, 1, PURPLE3, PURPLE3);
	decodeBackButton(211,15);
	drawSideBar(weather);

	char message[15] = "Send a Message:" ;
	OutGraphicsStringFont4(350, 40, WHITE, WHITE, message, 0);

	char to[3] = "TO:";
	OutGraphicsStringFont4(225, 90, WHITE, WHITE, to, 0);

	// first 3 digits
    drawRectangle(300, 90, 100, 50, 1, PURPLE1, BLACK);
    char colon[1] = '-';
    OutGraphicsStringFont4(400, 90, WHITE, WHITE, colon, 0);

    // second 3 digits
    drawRectangle(420, 90, 100, 50, 1, PURPLE1, BLACK);
    colon[1] = '-';
    OutGraphicsStringFont4(520, 90, WHITE, WHITE, colon, 0);

    // last 4 digits
    drawRectangle(540, 90, 133, 50, 1, PURPLE1, BLACK);

    keyPad();
    defaultMessages();
}

void drawMainScreen(int x, Alarm* alarm_list, Weather* weather, News* news, Event* event_list){
	if (x == 20){
		loadHomeScreen(alarm_list, weather);
	}
    else if (x == 30) {
    	drawEventScreen(event_list);
    }
    else if (x == 40) {
    	drawNewsScreen(news);
    }
    else if (x == 50) {
    	drawMessageScreen(weather);
    }

}
