/*
 * bsensor.h
 *
 *  Created on: Feb 6, 2019
 *      Author: nico
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


// C:/intelFPGA_lite/18.1/quartus/bin64/quartus_hps --cable="DE-SoC [USB-1]" -o GDBSERVER --gdbport0=3335 -preloader=C:/intelFPGA_lite/18.1/University_Program/Monitor_Program/arm_tools/u-boot-spl.de1-soc.srec -preloaderaddr=0xffff13a0
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

void Init_GPS(void);
int putcharGPS(int c);
char getcharGPS( void );
int GPSTestForReceivedData(void);
void GPSFlush(void);
void Init_BioSensor(void);
char getcharBioSensor( void );
int BioSensorTestForReceivedData(void);
void BioSensorFlush(void);
void Init_Wifi(void);
int putcharWifi(int);
int putString(char *);
int getcharWifi( void );
int WifiTestForReceivedData(void);
void WifiFlush(void);
char* get_GPS_Data(void);
char* get_Bio_Data(void);
void send_Message(void);
int num7seg(char);
void print_All(void);


