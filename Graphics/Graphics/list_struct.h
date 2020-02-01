/*
 * list_struct.h
 *
 *  Created on: Feb 22, 2019
 *      Author: nico
 */

#ifndef LIST_STRUCT_H_
#define LIST_STRUCT_H_

typedef struct Alarm
{
	int day;
	int month;
	int year;
	int hour;
	int min;
	char* date;
	int date_len;
	int day_len;
	int valid;
	int on;
} Alarm;

typedef struct
{
	char *description;
	double temperature;
	double humidity_percent;
	double wind_speed;
	double pressure;
	int valid;
} Weather;

typedef struct
{
	char *headline;
	char *source;

} News;

typedef struct
{
	double *timer;
	int *on;

} SleepButton;

typedef struct
{
	char *title;
	char *start;
	char *end;
	int last;
} Event;

#endif /* LIST_STRUCT_H_ */
