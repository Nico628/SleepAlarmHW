#define WIDTH                    800
#define HEIGHT                   480
#define IMG_HEIGHT               450

//typedef struct alarm Alarm;
#include "list_struct.h"
#include "wifi.h"

void display_on_off_button(int y, int on, int Colour, int SquareColour);
void displayAlarmBox(int Colour, Alarm a, int sequenceNum);
void displayAlarmList(Alarm*);
void decodeWeather(int start_x, int start_y, int);
void decodePlane(int start_x, int start_y);
void decodeBackButton(int start_x, int start_y);
void keyPad();
void drawEventScreen(Event*);
void drawNewsScreen();
void drawMessageScreen(Weather*);
void defaultMessages(void);

void decodeCloudSun(int start_x, int start_y);
void decodeImage(int start_x, int start_y, char* image_xpm, int width, int height);
void displayKeyboard();
void loadDefaultPalette();
void clearScreen();
void fillBackground(int Colour);
void drawSideBar(Weather* weather, int tempInt);
void drawMainScreen(int x, Alarm*, Weather*, News*, Event*);
void drawCreateAlarm();
void drawAlarmDetails();
void drawAlarmDate(int start_x, int start_y);
void decodeSpotify(int start_x, int start_y);
void loadDefaultPalette(void);

Alarm * defaultAlarmList();


