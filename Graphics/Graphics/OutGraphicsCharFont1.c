/*************************************************************************************************
** This function draws a single ASCII character at the coord and colour specified
** it optionally ERASES the background colour pixels to the background colour
** This means you can use this to erase characters
**
** e.g. writing a space character with Erase set to true will set all pixels in the
** character to the background colour
**
*************************************************************************************************/

// Constants ideally put this in a header file and #include it
// #define XRES 800
// #define YRES 480
// #define TRUE 1

#include "OutGraphicsCharFont1.h"
#include "DemoGraphicsRoutines.h"

// // declaration of the external Array, ideally put this in a header file and #include it

void OutGraphicsStringFont1(int x, int y, int stringColour, int backgroundColour, char string[], int Erase)
{
	register int currX = x, currY = y;
	for (int i = 0; string[i] != 0; i++){

		// Reset to new line if too far right and end of word
		if (currX > 790 && string[i] == ' '){
			currY = currY + 20;
			currX = x - 8;
		}

		OutGraphicsCharFont1(currX, currY, stringColour, backgroundColour, string[i], Erase);
		currX += 6;
	}
	return;
}

void OutGraphicsStringAlarm(int x, int y, int stringColour, int backgroundColour, char string[], int Erase, int len){
	register int currX = x, currY = y;
	for (int i = 0; i < len; i++){

		// Reset to new line if too far right and end of word
		if (currX > 790 && string[i] == ' '){
			currY = currY + 20;
			currX = x - 8;
		}

		OutGraphicsCharFont1(currX, currY, stringColour, backgroundColour, string[i], Erase);
		currX += 6;
	}
	return;	
}

void OutGraphicsStringAlarm5(int len, int x, int y, int stringColour, int backgroundColour, char string[], int Erase)
{
	register int currX = x, currY = y;
	for (int i = 0; i < len; i++){
		OutGraphicsCharFont5(currX, currY, stringColour, backgroundColour, string[i], Erase);
		currX += 38;
	}
	return;
}


void OutGraphicsStringFont4(int x, int y, int stringColour, int backgroundColour, char string[], int Erase)
{
	register int currX = x, currY = y;
	for (int i = 0; string[i] != 0; i++){
		OutGraphicsCharFont4(currX, currY, stringColour, backgroundColour, string[i], Erase);
		currX += 22;
	}
	return;
}

void OutGraphicsStringFont3(int x, int y, int stringColour, int backgroundColour, char string[], int Erase)
{
	register int currX = x, currY = y;
	for (int i = 0; string[i] != 0; i++){
		OutGraphicsCharFont3(currX, currY, stringColour, backgroundColour, string[i], Erase);
		currX += 16;
	}
	return;
}


void OutGraphicsStringFont5(int x, int y, int stringColour, int backgroundColour, char string[], int Erase)
{
	register int currX = x, currY = y;
	for (int i = 0; string[i] != 0; i++){
		OutGraphicsCharFont5(currX, currY, stringColour, backgroundColour, string[i], Erase);
		currX += 38;
	}
	return;
}



void OutGraphicsMenuFont1(int stringColour, int y, int backgroundColour, char string[], int Erase){
	register int length = strlen(string);
	printf("length is %d", length);

	register int currX = 50, currY = y;

	for (int i = 0; string[i] != 0; i++){
		OutGraphicsCharFont1(currX, currY, stringColour, backgroundColour, string[i], Erase);
		currX += 5;
	}
	return;
}


void OutGraphicsMenuFont4(int stringColour, int backgroundColour, char string[], int Erase){
	register int currX = 50, currY = 50;

	int end = XRES/4;

	for (int i = 0; string[i] != 0; i++){
		OutGraphicsCharFont4(currX, currY, stringColour, backgroundColour, string[i], Erase);
		currX += 20;
	}

	return;
}


void OutGraphicsCharFont1(int x, int y, int fontcolour, int backgroundcolour, int c, int Erase)
{
// using register variables (as opposed to stack based ones) may make execution faster
// depends on compiler and CPU

	register int row, column, theX = x, theY = y ;
	register int pixels ;
	register char theColour = fontcolour  ;
	register int BitMask, theC = c ;

// if x,y coord off edge of screen don't bother

    if(((short)(x) > (short)(XRES-1)) || ((short)(y) > (short)(YRES-1)))
        return ;


// if printable character subtract hex 20
	if(((short)(theC) >= (short)(' ')) && ((short)(theC) <= (short)('~'))) {
		theC = theC - 0x20 ;

		for(row = 0; (char)(row) < (char)(7); row ++)	{

// get the bit pattern for row 0 of the character from the software font
			pixels = Font5x7[theC][row] ;
			BitMask = 16 ;

			for(column = 0; (char)(column) < (char)(5); column ++)	{

// if a pixel in the character display it
				if((pixels & BitMask))
					WriteAPixel(theX+column, theY+row, theColour) ;

				else {
					if(Erase == TRUE)

// if pixel is part of background (not part of character)
// erase the background to value of variable BackGroundColour

						WriteAPixel(theX+column, theY+row, backgroundcolour) ;
				}
				BitMask = BitMask >> 1 ;
			}
		}
	}
}

void OutGraphicsCharFont4(int x, int y, int fontcolour, int backgroundcolour, int c, int Erase)
{
// using register variables (as opposed to stack based ones) may make execution faster
// depends on compiler and CPU

	register int row, column, theX = x, theY = y ;
	register int pixels ;
	register char theColour = fontcolour  ;
	register int BitMask, theC = c ;

// if x,y coord off edge of screen don't bother

    if(((short)(x) > (short)(XRES-1)) || ((short)(y) > (short)(YRES-1)))
        return ;

// if printable character subtract hex 20
	if(((short)(theC) >= (short)(' ')) && ((short)(theC) <= (short)('~'))) {
		theC = theC - 0x20 ;

		for(row = 0; (char)(row) < (char)(40); row ++)	{

			for (int part = 0; part < 3; part++){
		// get the bit pattern for row 0 of the character from the software font
				pixels = Font22x40[theC*40*3 + row*3 + part] ;
				BitMask = 128;

				for(column = 0; (char)(column) < (char)(8); column ++)	{

	// if a pixel in the character display it
					if((pixels & BitMask))
						WriteAPixel(theX+(8*part) + column, theY+row, theColour) ;

					else {
						if(Erase == TRUE)

	// if pixel is part of background (not part of character)
	// erase the background to value of variable BackGroundColour

							WriteAPixel(theX+(8*part) + column, theY+row, backgroundcolour) ;
					}
					BitMask = BitMask >> 1 ;
				}
			}

		}
	}
}

void OutGraphicsCharFont3(int x, int y, int fontcolour, int backgroundcolour, int c, int Erase)
{
// using register variables (as opposed to stack based ones) may make execution faster
// depends on compiler and CPU

	register int row, column, theX = x, theY = y ;
	register int pixels ;
	register char theColour = fontcolour  ;
	register int BitMask, theC = c ;

// if x,y coord off edge of screen don't bother

    if(((short)(x) > (short)(XRES-1)) || ((short)(y) > (short)(YRES-1)))
        return ;

// if printable character subtract hex 20
	if(((short)(theC) >= (short)(' ')) && ((short)(theC) <= (short)('~'))) {
		theC = theC - 0x20 ;

		for(row = 0; (char)(row) < (char)(27); row ++)	{

			for (int part = 0; part < 2; part++){
		// get the bit pattern for row 0 of the character from the software font
				pixels = Font16x27[theC*27*2 + row*2 + part] ;
				BitMask = 128;

				for(column = 0; (char)(column) < (char)(8); column ++)	{

	// if a pixel in the character display it
					if((pixels & BitMask))
						WriteAPixel(theX+(8*part) + column, theY+row, theColour) ;

					else {
						if(Erase == TRUE)

	// if pixel is part of background (not part of character)
	// erase the background to value of variable BackGroundColour

							WriteAPixel(theX+(8*part) + column, theY+row, backgroundcolour) ;
					}
					BitMask = BitMask >> 1 ;
				}
			}

		}
	}
}

void OutGraphicsCharFont5(int x, int y, int fontcolour, int backgroundcolour, int c, int Erase)
{
// using register variables (as opposed to stack based ones) may make execution faster
// depends on compiler and CPU

	register int row, column, theX = x, theY = y ;
	register int pixels ;
	register char theColour = fontcolour  ;
	register int BitMask;
	register int theC = c ;

// if x,y coord off edge of screen don't bother

    if(((short)(x) > (short)(XRES-1)) || ((short)(y) > (short)(YRES-1)))
        return ;

// if printable character subtract hex 20
	if(((short)(theC) >= (short)(' ')) && ((short)(theC) <= (short)('~'))) {
		theC = theC - 0x20 ;

		for(row = 0; (char)(row) < (char)(59); row ++)	{

			for (int part = 0; part < 4; part ++){
		// get the bit pattern for row 0 of the character from the software font
				pixels = Font38x59[theC*59*4 + row*4 + part] ;
				BitMask = 128;

				for(column = 0; (char)(column) < (char)(8); column ++)	{

	// if a pixel in the character display it
					if((pixels & BitMask))
						WriteAPixel(theX+(8*part) + column, theY+row, theColour) ;

					else {
						if(Erase == TRUE)

	// if pixel is part of background (not part of character)
	// erase the background to value of variable BackGroundColour

							WriteAPixel(theX+(8*part) + column, theY+row, backgroundcolour) ;
					}
					BitMask = BitMask >> 1 ;
				}
			}

		}
	}
}
