#define TS_ReceiverFifo                (*(volatile unsigned char *)(0xFF210230))
#define TS_TransmitterFifo             (*(volatile unsigned char *)(0xFF210230))
#define TS_InterruptEnableReg          (*(volatile unsigned char *)(0xFF210232))
#define TS_InterruptIdentificationReg  (*(volatile unsigned char *)(0xFF210234))
#define TS_FifoControlReg              (*(volatile unsigned char *)(0xFF210234))
#define TS_LineControlReg              (*(volatile unsigned char *)(0xFF210236))
#define TS_ModemControlReg             (*(volatile unsigned char *)(0xFF210238))
#define TS_LineStatusReg               (*(volatile unsigned char *)(0xFF21023A))
#define TS_ModemStatusReg              (*(volatile unsigned char *)(0xFF21023C))
#define TS_ScratchReg                  (*(volatile unsigned char *)(0xFF21023E))

#define TS_DivisorLatchLSB             (*(volatile unsigned char *)(0xFF210230))
#define TS_DivisorLatchMSB             (*(volatile unsigned char *)(0xFF210232))

#define FACTOR_Y (double) .1953125
#define FACTOR_X (double) .1171875

typedef struct { int x, y; } Point ;
#include "list_struct.h"
#include "wifi.h"

int putcharPS232 (int c);
unsigned char getcharRS232 (void);

void Init_Touch (void);

int GetPress(int *, char*, char*, Alarm*, int*, int*, int*, Weather*, News*, SleepButton*, Event*);
Point GetRelease(void);
void displayAlarmDigits(char*);
void addDigitToAlarmBuffer(char* alarm_buffer,char ch);
void resetAlarmDigitsBuffer(char* alarm_buffer);
void resetPhoneDigitsBuffer(char* phone_buffer);
void addDigitToPhoneBuffer(char* phone_buffer, char ch);
void displayPhoneDigits(char* buf);

int ScreenTouched(void);
void WaitForTouch(double *, SleepButton *, Alarm*, int*, Weather*, News*, Event*);
