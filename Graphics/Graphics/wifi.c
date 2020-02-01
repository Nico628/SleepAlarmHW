/*
* wifi.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bsensor.h"
#include "wifi.h"
#include <time.h>
#include "cJSON.h"

#define num_headlines 10
#define num_events 10

//function to run lua commands on Wifi module
int runlua(char * str, char * res) {
	int i;
	int bytes_received = 0;
	printf("executing %s as a Lua command\n", str);
	while (*str) {
		for(i = 0; i < 1000; i++) {;}
		putcharWifi(*str);
		str += 1;
		for(i = 0; i < 5000; i++) {;}
		if(WifiTestForReceivedData()) {
			res[bytes_received++] = (char) getcharWifi();
		}
	}
	putcharWifi('\n');
    for(i = 0; i < 9000000; i++) {
        if(WifiTestForReceivedData()) {
        	res[bytes_received++] = (char) getcharWifi();
            i = 0 ; // reset timer if we got something back
        }
    }
	printf("Got %u bytes\n", bytes_received);
	res[bytes_received] = '\0';
	return bytes_received;
}

/* Purpose: send message saying the person is running late
*/
void send_message(void) {
	//Init_Wifi();
	WifiFlush();
	char buf[1024] = "";

	//printf("Initializing WiFi Module\n");
	//int res = runlua("dofile(\"combined_lua.lua\")", buf);
	//printf("%s\n", buf);

	int res = runlua("send_sms(\"(226) 796-5201\",\"(250) 858-9326\",\"I'm running late!\")", buf);
	printf("%s\n", buf);
}

/*	Purpose: Get current weather for Vancouver
 */
void get_weather(Weather *weather) {

	//Init_Wifi();
	WifiFlush();
	char* buf = malloc(1024*sizeof(char));

	//printf("Initializing WiFi Module\n");
	//int res = runlua("dofile(\"combined_lua.lua\")", buf);
	//printf("%s\n", buf);

	//hardcoded vancouver co-ordinates
	int res = runlua("get_weather()", buf);
	printf("%s\n", buf);


	while(*buf != '{')
		buf++;

	//	get Weather object with all the fields we need
	parse_weather_output(weather, buf);
}

/*	Purpose: Get current canadian news
 */
void get_news(News* news) {

	//Init_Wifi();
	WifiFlush();
	char* buf = malloc(1024*sizeof(char));

	//printf("Initializing WiFi Module\n");
	//int res = runlua("dofile(\"combined_lua.lua\")", buf);
	//printf("%s\n", buf);

	//hardcoded vancouver co-ordinates
	int res = runlua("get_headlines()", buf);
	printf("%s\n", buf);

	//get Weather object with all the fields we need
	get_headlines(news, buf);

}

/*	Purpose: Get today's events
 */
void get_calendar_data(Event* events) {

	Init_Wifi();
	WifiFlush();
	char* buf = malloc(1024*sizeof(char));

	printf("Initializing WiFi Module\n");
	int res = runlua("dofile(\"combined_lua.lua\")", buf);
	printf("%s\n", buf);

	//hardcoded vancouver co-ordinates
	res = runlua("get_calendar_data()", buf);
	printf("%s\n", buf);

	while(*buf != '{' && *buf != ']')
		buf++;

	if(*buf == '{') {
		//get Event object with all the fields we need
		get_events(events, buf);
	}

	else {
		events[0].last = 1;
		events[0].title = "NO EVENTS TODAY!";
	}
}


/*Purpose: send sleep data from biomed sensor to the database through the ESP8266
*/
void send_sleep_data(int heart_rate, int oxygen_level, int hour, int minute) {
	//Init_Wifi();
	WifiFlush();
	char buf[1024] = "";

	//printf("Initializing WiFi Module\n");
	//int res = runlua("dofile(\"combined_lua.lua\")", buf);
	//printf("%s\n", buf);

	//calculate time/date and parse into string
    time_t t = time(NULL);
    struct tm *tm_struct = localtime(&t);

	int year = tm_struct->tm_year + 1900;      /* The number of years since 1900   */
	int month = tm_struct->tm_mon + 1;         /* month, range 0 to 11             */
	int day = tm_struct->tm_mday;              /* day of the month, range 1 to 31  */

	//final strings for date and command
	char date[2048];
	char command[2048];

	//create strings with correct date and time
	char year_str[64];
	char month_str[64];
	char day_str[64];
	char hour_str[64];
	char minute_str[64];
	char oxygen_str[64];
	char heart_str[64];

    sprintf(year_str, "%d", year);
    sprintf(month_str, "%d", month);
    sprintf(day_str, "%d", day);
    sprintf(hour_str, "%d", hour);
    sprintf(minute_str, "%d", minute);
    sprintf(heart_str, "%d", heart_rate);
    sprintf(oxygen_str, "%d", oxygen_level);

    //create new string with all fields in right place
    //2019-05-09T15:46:34-08:00
    strcpy(date, year_str);
    strcat(date, "-");
    strcat(date, month_str);
    strcat(date, "-");
    strcat(date, day_str);
    strcat(date, "T");
    strcat(date, hour_str);
    strcat(date, ":");
    strcat(date, minute_str);

    //create string to call with runlua
    //oxygen level, date, heart rate
    strcpy(command, "send_sleep_data(");
    strcat(command, oxygen_str);
    strcat(command, ", \"");
    strcat(command, date);
    strcat(command, "\", ");
    strcat(command, heart_str);
    strcat(command, ")");

	//hardcoded vancouver co-ordinates
	int res = runlua(command, buf);
}


/* Do all the wiFi functions at once!
*/
void all_wifi(void) {

	char buf[1024] = "";

	//Init_Wifi();
	WifiFlush();
	//printf("Initializing WiFi Module\n");
	//int res = runlua("dofile(\"combined_lua.lua\")", buf);
	//printf("%s\n", buf);

	//hardcoded vancouver co-ordinates
	int res = runlua("get_headlines()", buf);
	printf("%s\n", buf);

	char *temp = (char *) buf;

	while(*temp != '{')
		temp++;

	News news[num_headlines];

	//get Weather object with all the fields we need
	get_headlines(news, buf);

	//***** Add the stuff to news whether through graphics here*****//

	//hardcoded vancouver co-ordinates
	res = runlua("get_weather()", buf);
	printf("%s\n", buf);

	temp = (char *) buf;

	while(*temp != '{')
		temp++;

//	get Weather object with all the fields we need
//	Weather weather = parse_weather_output(temp);

	//***** Add the stuff to display whether through graphics here*****//

	res = runlua("send_sms(\"(226) 796-5201\",\"(250) 858-9326\",\"I'm running late!\")", buf);
	printf("%s\n", buf);
}

//helper function for weather API
void parse_weather_output(Weather* weather, char *temp) {
	char* end;

	//parse JSON to fill weather fields
	//manually because I'm lazy
	temp++;
	while(*temp != '{') {temp++;}
	temp++;
	while(*temp != '{') {temp++;}
	while(*temp != ',') {temp++;}
	while(*temp != ':') {temp++;}
	temp+=2;
	end = temp;

	while(*end != '\"'){end++;}
	*end = '\0';

	//set the weather description
	strcpy(weather->description, temp);
	//weather.description = temp;
	printf("%c", weather->description);

	//set the temperature
	while(*temp != '}') {temp++;}
	while(*temp != '{') {temp++;}
	while(*temp != ':') {temp++;}
	temp++;
	end=temp;

	while(*end != ',') {end++;}
	*end = '\0';

	//weather->temperature = atoi(temp);


	//set the pressure
	while(*temp != ':') {temp++;}
	temp++;
	end=temp;

	while(*end != ',') {end++;}
	*end = '\0';
	weather->pressure = atoi(temp);


	//set the humidity percentage
	while(*temp != ':') {temp++;}
	temp++;
	end=temp;

	while(*end != ',') {end++;}
	*end = '\0';
	weather->humidity_percent = atoi(temp);

	//set wind speed
	while(*temp != '}') {temp++;}
	while(*temp != '{') {temp++;}
	while(*temp != ':') {temp++;}
	temp++;
	end=temp;

	while(*end != ',') {end++;}
	*end = '\0';
	weather->temperature = atoi(temp);
}

//helper function to process news output and create news object
void get_headlines(News *news, char *buf) {

	while(*buf != '{') {buf++;}
	buf++;

	for(int i=0;i<num_headlines;i++) {
		//given a pointer to json and current news object, put all fields 
		//in place and increment pointer accordingly
		buf = get_one_headline(buf, &news[i]);
	}
}

//gets all events for current day
void get_events(Event* events, char *buf) {

	for(int i=0;i<num_events;i++) {
		//given a pointer to json and current news object, put all fields 
		//in place and increment pointer accordingly
		buf = get_one_event(buf, &events[i]);
		if(events[i].last == 1) {
			break;
		}
	}	
}

//get one event and put it in an object
char *get_one_event(char *temp, Event *event) {
	char *end;

	//starts just after the source bracket for each article
	//find the source
	while(*temp != ':') {temp++;}
	temp++;
	while(*temp != ':') {temp++;}
	temp+=2;
	end=temp;

	while(*end != '\"') {end++;}
	*end = '\0';
	event->title = temp;

	//find the start time
	while(*temp != ':') {temp++;}
	while(*temp != 'T') {temp++;}
	temp++;
	end=temp;

	end +=5;
	*end = '\0';
	event->start = temp;

	//find the end time
	while(*temp != ',') {temp++;}
	while(*temp != ':') {temp++;}
	while(*temp != 'T') {temp++;}
	temp++;
	end=temp;

	end +=5;
	*end = '\0';
	event->end = temp;

	//prep for next event
	while(*temp != '{' && *temp != ']')
		temp++;

	event->last = 0;

	if(*temp == ']') {
		//get Weather object with all the fields we need
		event->last = 1;
	}

	return temp;
}

//given pointer to json object, fill in fields for one object
char *get_one_headline(char *temp, News *news) {
	char *end;

	//starts just after the source bracket for each article
	//find the source
	while(*temp != '{') {temp++;}
	temp++;
	while(*temp != ',') {temp++;}
	while(*temp != ':') {temp++;}
	temp+=2;
	end=temp;

	while(*end != '\"') {end++;}
	*end = '\0';
	news->source = temp;

	//find the headline
	while(*temp != ':') {temp++;}
	temp++;
	while(*temp != ':') {temp++;}
	temp+=2;
	end=temp;

	while(*end != '\"') {end++;}
	*end = '\0';
	news->headline = temp;

	//prep for next headline
	while(*temp != '{') {temp++;}
	temp++;

	return temp;
}


/*	Purpose: Get alarms for user 2
 */
void updateAlarmList(Alarm* alarmObj) {

	char* weekdays[7];
	weekdays[0]= "Sunday";
	weekdays[1]= "Monday";
	weekdays[2]= "Tuesday";
	weekdays[3]= "Wednesday";
	weekdays[4]= "Thursday";
	weekdays[5]= "Friday";
	weekdays[6]= "Saturday";

	int index = 0;
	char* buf = malloc(1024*sizeof(char));

	//Init_Wifi();
	WifiFlush();
	//printf("Initializing WiFi Module\n");
	//int res = runlua("dofile(\"combined_lua.lua\")", buf);
	//printf("%s\n", buf);

	int res = runlua("get_alarm_data()", buf);
	printf("%s\n", buf);

	while(*buf != '{')
		buf++;

	//now parse thru the buffer and get alarms
	cJSON *json = cJSON_Parse(buf);
	cJSON *alarms = NULL;
	cJSON *alarm = NULL;

	if (json == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        goto end;
    }

    alarms = cJSON_GetObjectItemCaseSensitive(json, "alarms");
    cJSON_ArrayForEach(alarm, alarms)
    {

    	//get json fields
    	cJSON *alarm_time = cJSON_GetObjectItemCaseSensitive(alarm, "alarm_time"); //use for day, month, year, hour, min, date, date_len, day_len
        cJSON *active = cJSON_GetObjectItemCaseSensitive(alarm, "active"); //use for on

		//calculate time/date and parse into string
        const char *dateStr = cJSON_GetStringValue(alarm_time);
		int year,month,day,hour,min;
		float s;
		sscanf(dateStr, "%d-%d-%dT%d:%d:%fZ", &year, &month, &day, &hour, &min, &s);

		//create a tm object
		struct tm time;
		time.tm_year = year - 1900; // Year since 1900
		time.tm_mon = month - 1;     // 0-11
		time.tm_mday = day;        // 1-31
		time.tm_hour =hour;        // 0-23
		time.tm_min = min;         // 0-59
		time.tm_sec = (int)s;    // 0-61 (0-60 in C++11) 

		time_t t = mktime(&time);
      	int wday_num = localtime(&t)->tm_wday; // Sunday=0, Monday=1, etc.
    	
    	//calculate day of the week
    	char* date = weekdays[wday_num];
    	int valid = 1;
    	
    	int on;
    	if(cJSON_IsTrue(active))
    		on = 1;
    	else
    		on = 0;

    	//convert the weird fields to char arrays

    	/*
    	char day[20];
    	char hour[20];
    	char min[20];

    	snprintf(day, 20,"%d",nday);
    	snprintf(hour, 20,"%d",nhour);
    	snprintf(min, 20,"%d",nmin);

    	*/

    	//fill in our temp object
    	alarmObj[index].day = day+'\0';
    	alarmObj[index].month = month;
    	alarmObj[index].year = year;
    	alarmObj[index].hour = hour+'\0';
    	alarmObj[index].min = min+'\0';
    	alarmObj[index].date = date;
    	alarmObj[index].date_len = -1; //don't need these rn
    	alarmObj[index].day_len = -1; //don't need these rn
    	alarmObj[index].valid = valid;
    	alarmObj[index].on = on;

    	index++;
    }

    end:
    cJSON_Delete(json);

    //add on invalid alarms at the end, if we didn't add 5 already

    while(index < 5) {
    	alarmObj[index].valid = 0;
    	index++;
    }

}


