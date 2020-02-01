
#define PI 3.14159265

#define GraphicsCommandReg   		(*(volatile unsigned short int *)(0xFF210000))
#define GraphicsStatusReg   		(*(volatile unsigned short int *)(0xFF210000))
#define GraphicsX1Reg   		(*(volatile unsigned short int *)(0xFF210002))
#define GraphicsY1Reg   		(*(volatile unsigned short int *)(0xFF210004))
#define GraphicsX2Reg   		(*(volatile unsigned short int *)(0xFF210006))
#define GraphicsY2Reg			(*(volatile unsigned short int *)(0xFF210008))
#define GraphicsColourReg		(*(volatile unsigned short int *)(0xFF21000E))
#define GraphicsBackGroundColourReg   		(*(volatile unsigned short int *)(0xFF210010))

#define WIDTH                    800
#define HEIGHT                   480
#define IMG_HEIGHT               450

#define DrawHLine		1
#define DrawVLine		2
#define DrawLine		3
#define PutAPixel		0xA
#define GetAPixel		0xB
#define ProgramPaletteColour   	0x10

#define WAIT_FOR_GRAPHICS		while((GraphicsStatusReg & 0x0001) != 0x0001);
#define POLL_GRAPHICS           (GraphicsStatusReg & 0x0001);

// #defined constants representing values we write to the graphics 'command' register to get
// it to do something. You will add more values as you add hardware to the graphics chip
// Note DrawHLine, DrawVLine and DrawLine at the moment do nothing - you will modify these

 #define    BLACK             0 
 #define    DARKGRAY1         1 
 #define    LIGHTGRAY3        2 
 #define    LIGHTGRAY2        3 
 #define    DARKGRAY2         4 
 #define    LIGHTGRAY4        5 
 #define    LIGHTGREEN1       6 
 #define    DARKGRAY5         7 
 #define    DARKBROWN1        8 
 #define    DARKGREEN1        9 
 #define    DARKBLUE1         10
 #define    DARKPURPLE1       11
 #define    DARKFUSCHIA1      12
 #define    DARKGREEN2        13
 #define    DARKGREEN3        14
 #define    DARKBLUE2         15
 #define    GREEN1            16
 #define    GREEN2            17
 #define    GREEN3            18
 #define    GREEN4            19
 #define    GREEN5            20
 #define    TEAL1             21
 #define    TEAL2             22
 #define    TEAL3             23
 #define    TEAL4             24
 #define    TEAL5             25
 #define    YELLOW1           26
 #define    YELLOW2           27
 #define    YELLOW3           28
 #define    YELLOW4           29
 #define    YELLOW5           30
 #define    ORANGE1           31
 #define    ORANGE2           32
 #define    ORANGE3           33
 #define    ORANGE4           34
 #define    ORANGE5           35
 #define    RED1              36
 #define    RED2              37
 #define    RED3              38
 #define    RED4              39
 #define    RED5              40
 #define    PINK1             41
 #define    PINK2             42
 #define    PINK3             43
 #define    PINK4             44
 #define    BLUE1             45
 #define    BLUE2             46
 #define    BLUE3             47
 #define    BLUE4             48
 #define    BLUE5             49
 #define    PURPLE1           50
 #define    PURPLE2           51
 #define    PURPLE3           52
 #define    PURPLE4           53
 #define    PURPLE5           54
 #define    BLACKGREEN        55
 #define    BLACKTEAL         56
 #define    BLACKYELLOW       57
 #define    BLACKORANGE       58
 #define    BLACKRED          59
 #define    BLACKPINK         60
 #define    BLACKBLUE         61
 #define    BLACKPURPLE       62
 #define    WHITE             63

void WriteAPixel (int x, int y, int Colour);
int ReadAPixel (int x, int y);
void ProgramPalette(int PaletteNumber, int RGB);

void HLine(int x1, int y1, int length, int Colour);

void drawRectangle(int x, int y, int length, int width, int fill, int fillColour, int borderColour);
void drawCircle(int center_x, int center_y, int fill, int radius, int colour);
void drawTriangle(int length, int borderColour, int x, int y);
void drawCircleHelper(int x, int y, int x_a, int y_a, int colour, int fill);
void drawArc(int center_x, int center_y, int fill, int radius, int colour, int quad[]);
void drawArcHelper(int x, int y, int x_a, int y_a, int colour, int fill, int quad[]);
