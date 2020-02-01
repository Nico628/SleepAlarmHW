/*
* wifi functions.h
*/

//weather and news objects
#include "list_struct.h"

//function stubs
void parse_weather_output(Weather*, char *);
int runlua(char * str, char * res);
void get_headlines(News *news, char *buf);
char *get_one_headline(char *temp, News *cur);
void get_events(Event* events, char *buf);
char *get_one_event(char *temp, Event *cur);

void send_sleep_data(int heart_rate, int oxygen_level, int hour, int minute);
void send_message(void);
void get_news(News*);
void get_weather(Weather*);
void get_calendar_data(Event* events);
void get_alarm_data(Alarm* alarms);
void all_wifi(void);
void updateAlarmList(Alarm*);
