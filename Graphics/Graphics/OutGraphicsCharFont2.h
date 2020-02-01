// Constants ideally put this in a header file and #include it
#define XRES 800
#define YRES 480
#define FONT2_XPIXELS	10				// width of Font2 characters in pixels (no spacing)
#define FONT2_YPIXELS	14				// height of Font2 characters in pixels (no spacing)
#define TRUE            1

extern const unsigned short int Font10x14[][14] ;

void OutGraphicsStringFont2(int, int, int, int, char[], int);
void OutGraphicsStringAlarm2(int len, int x, int y, int stringColour, int backgroundColour, char string[], int Erase);


void OutGraphicsCharFont2(int, int, int, int, int, int);

