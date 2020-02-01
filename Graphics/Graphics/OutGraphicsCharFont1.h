#define XRES 800
#define YRES 480
#define TRUE 1

#define FONT3_XPIXELS 16
#define FONT3_YPIXELS 27

#define FONT4_XPIXEL 22
#define FONT4_YPIXEL 40

extern const unsigned char Font5x7[][7] ;
extern const unsigned char Font16x27[];
extern const unsigned char Font22x40[];
extern const unsigned char Font38x59[];

void OutGraphicsStringFont1(int, int, int, int, char[], int);
void OutGraphicsStringFont4(int x, int y, int stringColour, int backgroundColour, char string[], int Erase);
void OutGraphicsStringFont5(int x, int y, int stringColour, int backgroundColour, char string[], int Erase);
void OutGraphicsStringFont3(int x, int y, int stringColour, int backgroundColour, char string[], int Erase);
void OutGraphicsStringAlarm5(int len, int x, int y, int stringColour, int backgroundColour, char string[], int Erase);

void OutGraphicsStringAlarm(int x, int y, int stringColour, int backgroundColour, char string[], int Erase, int len);

void OutGraphicsCharFont1(int, int, int, int, int, int);

void OutGraphicsCharFont3(int x, int y, int fontcolour, int backgroundcolour, int c, int Erase);

void OutGraphicsCharFont4(int x, int y, int fontcolour, int backgroundcolour, int c, int Erase);

void OutGraphicsCharFont5(int x, int y, int fontcolour, int backgroundcolour, int c, int Erase);



void OutGraphicsMenuFont4(int stringColour, int backgroundColour, char string[], int Erase);

void OutGraphicsMenuFont1(int stringColour, int y, int backgroundColour, char string[], int Erase);
