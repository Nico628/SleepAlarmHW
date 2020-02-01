#include <stdio.h>
#include "DemoGraphicsRoutines.h"
#include "Shapes.h"
#include "math.h"

/**********************************************************************
* This function writes a single pixel to the x,y coords specified in the specified colour
* Note colour is a palette number (0-63) not a 24 bit RGB value
**********************************************************************/
void WriteAPixel (int x, int y, int Colour)
{
	WAIT_FOR_GRAPHICS;			// is graphics ready for new command

	GraphicsX1Reg = x;			// write coords to x1, y1
	GraphicsY1Reg = y;
	GraphicsColourReg = Colour;		// set pixel colour with a palette number
	GraphicsCommandReg = PutAPixel;		// give graphics a "write pixel" command
}

/*****************************************************************************************
* This function read a single pixel from x,y coords specified and returns its colour
* Note returned colour is a palette number (0-63) not a 24 bit RGB value
******************************************************************************************/
int ReadAPixel (int x, int y)
{
	WAIT_FOR_GRAPHICS;			// is graphics ready for new command

	GraphicsX1Reg = x;			// write coords to x1, y1
	GraphicsY1Reg = y;
	GraphicsCommandReg = GetAPixel;		// give graphics a "get pixel" command

	WAIT_FOR_GRAPHICS;			// is graphics done reading pixel
	return (int)(GraphicsColourReg) ;		// return the palette number (colour)
}

/****************************************************************************************************
** subroutine to program a hardware (graphics chip) palette number with an RGB value
** e.g. ProgramPalette(RED, 0x00FF0000) ;
****************************************************************************************************/


void ProgramPalette(int PaletteNumber, int RGB)
{
    WAIT_FOR_GRAPHICS;
    GraphicsColourReg = PaletteNumber;
    GraphicsX1Reg = RGB >> 16   ;          // program red value in ls.8 bit of X1 reg
    GraphicsY1Reg = RGB ;                	 // program green and blue into 16 bit of Y1 reg
    GraphicsCommandReg = ProgramPaletteColour;	// issue command
}

/********************************************************************************************* This function draw a horizontal line, 1 pixel at a time starting at the x,y coords specified
*********************************************************************************************/

void HLine(int x1, int y1, int length, int Colour)
{
	if (x1 < 0 || x1 > 800 || y1 < 0 || y1 > 480){    // out of range
		return;
	}

	WAIT_FOR_GRAPHICS;			// is graphics ready for new command
	GraphicsX1Reg = x1;			// write coords to x1, y1
	GraphicsY1Reg = y1;
	GraphicsX2Reg = x1 + length;	
    GraphicsColourReg = Colour;			// set pixel colour		
	GraphicsCommandReg = DrawHLine;		// give graphics a "get pixel" command
}

/*
 * (x,y): starting position (topmost left)
 * length 
 * width 
 * rotation: rotation in degrees
 * fill: fill shape w/ colour?
 */

void drawRectangle(int x, int y, int length, int width, int fill, int fillColour, int borderColour){
    HLine(x, y, length, borderColour);
    HLine(x, y + width, length, borderColour);
    VLine(x, y, width, borderColour);
    VLine(x + length, y, width, borderColour);

    if (fill == 1){
        for (int i = x + 1; i < x + length; i++){
            VLine(i, y+1, width-2, fillColour);
        }
    }

    return;
}

/*
 * Function draws circle given radius, center coordinates, can be customized with fill and border colour.
 * radius
 * 
 * (center_x, center_y): center coordinates
 * fill: fills in Circle with fillColour
 * borderColour, fillColour
 */
void drawCircle(int center_x, int center_y, int fill, int radius, int colour){
    int x = 0; 
    int y = radius;
    int i = 3 - 2*radius;
    drawCircleHelper(center_x, center_y, x, y, colour, fill);

    while (y >= x){
        x++;

        if (i > 0){
            y--;
            i = i + 4*(x-y) + 10;
        }
        else i = i + 4*x + 6;
        drawCircleHelper(center_x, center_y, x, y, colour, fill);
    }
    return;
}

void drawCircleHelper(int x, int y, int x_a, int y_a, int colour, int fill){
    if (fill){
        Line(x + x_a, y + y_a, x, y, colour);
        Line(x - x_a, y_a + y, x, y, colour);
        Line(x + x_a, y - y_a, x, y, colour);
        Line(x - x_a, y - y_a, x, y, colour);

        Line(x - y_a, y - x_a, x, y, colour);
        Line(x + y_a, y - x_a, x, y, colour);
        Line(x - y_a, y + x_a, x, y, colour);
        Line(y_a + x, x_a + y, x, y, colour);
    }
    else {
        WriteAPixel(x + x_a, y + y_a, colour);
        WriteAPixel(x - x_a, y_a + y, colour);
        WriteAPixel(x + x_a, y - y_a, colour);
        WriteAPixel(x - x_a, y - y_a, colour);

        WriteAPixel(x - y_a, y - x_a, colour);
        WriteAPixel(x + y_a, y - x_a, colour);
        WriteAPixel(x - y_a, y + x_a, colour);
        WriteAPixel(y_a + x, x_a + y, colour);
    }

}

/*
 * Draws an equilateral triangle.
 * length: length of one side
 * (x, y): leftmost vertex.
 */
void drawTriangle(int length, int borderColour, int x, int y){

    HLine(x, y, length, borderColour);

    int x_change = cos((PI/180)*60)*length;
    int y_change = sin((PI/180)*60)*length;

    int top_x = x + x_change;
    int top_y = y - y_change;
    int right_x = x + length;
    
    Line(x, y, top_x, top_y, borderColour);
    Line(right_x, y, top_x, top_y, borderColour);

    return;
}

void drawArc(int center_x, int center_y, int fill, int radius, int colour, int quad[]){
    int x = 0; 
    int y = radius;
    int i = 3 - 2*radius;
    drawArcHelper(center_x, center_y, x, y, colour, fill, quad);

    while (y >= x){
        x++;

        if (i > 0){
            y--;
            i = i + 4*(x-y) + 10;
        }
        else i = i + 4*x + 6;
        drawArcHelper(center_x, center_y, x, y, colour, fill, quad);
    }
    return;
}

void drawArcHelper(int x, int y, int x_a, int y_a, int colour, int fill, int quad[]){
    if (fill){
        if (quad[0] == 1){
            Line(x + x_a, y + y_a, x, y, colour);
        }
        if (quad[1] == 1){
            Line(x - x_a, y_a + y, x, y, colour);
        }
        if (quad[2] == 1){
            Line(x + x_a, y - y_a, x, y, colour);
        }
        if (quad[3] == 1){
            Line(x - x_a, y - y_a, x, y, colour);
        }
        if (quad[4] == 1){
            Line(x - y_a, y - x_a, x, y, colour);
        }
        if (quad[5] == 1){
            Line(x + y_a, y - x_a, x, y, colour);
        }
        if (quad[6] == 1){
            Line(x - y_a, y + x_a, x, y, colour);
        }
        if (quad[7] == 1){
            Line(y_a + x, x_a + y, x, y, colour);
        }
        
    }
    else {
        if (quad[0] == 1){
            WriteAPixel(x + x_a, y + y_a, colour);
        }
        if (quad[1] == 1){
            WriteAPixel(x - x_a, y_a + y, colour);
        }
        if (quad[2] == 1){    
            WriteAPixel(x + x_a, y - y_a, colour);
        }
        if (quad[3] == 1){
            WriteAPixel(x - x_a, y - y_a, colour);
        }
        if (quad[4] == 1){
            WriteAPixel(x - y_a, y - x_a, colour);
        }
        if (quad[5] == 1){
            WriteAPixel(x + y_a, y - x_a, colour);
        }
        if (quad[6] == 1){
            WriteAPixel(x - y_a, y + x_a, colour);
        }
        if (quad[7] == 1){
            WriteAPixel(y_a + x, x_a + y, colour);
        }
        
    }
}


/********************************************************************************************* This function draw a vertical line, 1 pixel at a time starting at the x,y coords specified
*********************************************************************************************/

void VLine(int x1, int y1, int length, int Colour)
{
	if (x1 < 0 || x1 > 800 || y1 < 0 || y1 > 480){    // out of range
		return;
	}

	WAIT_FOR_GRAPHICS;			// is graphics ready for new command
	GraphicsX1Reg = x1;			// write coords to x1, y1
	GraphicsY1Reg = y1;
	GraphicsY2Reg = y1 + length;	
    GraphicsColourReg = Colour;			// set pixel colour		
	GraphicsCommandReg = DrawVLine;		// give graphics a "get pixel" command
}

/*******************************************************************************
** Implementation of Bresenhams line drawing algorithm
*******************************************************************************/

void Line(int x1, int y1, int x2, int y2, int Colour)
{
	if (x1 < 0 || x1 > 800 || y1 < 0 || y1 > 480 || x2 < 0 || x2 > 800 || y2 < 0 || y2 > 480){    // out of range
		return;
	}

    WAIT_FOR_GRAPHICS;				// is graphics ready for new command
	GraphicsX1Reg = x1;				// write coords to x1, y1
	GraphicsY1Reg = y1;
    GraphicsX2Reg = x2;
    GraphicsY2Reg = y2;
	GraphicsColourReg = Colour;			// set pixel colour
	GraphicsCommandReg = DrawLine;			// give graphics "write pixel" command
}

void Random(){
	int r_x, r_y, r_length, r_colour, r_x1, r_y1;

	for (int i = 0; i < 1000; i++){
		r_x = rand() % WIDTH; 
		r_y = rand() % HEIGHT;

		r_length = rand() % HEIGHT;
		r_colour = rand() % 8;
		
		VLine(r_x, r_y, r_length, r_colour);

		r_x = rand() % WIDTH; 
		r_y = rand() % HEIGHT;

		r_x1 = rand() % WIDTH; 
		r_y1 = rand() % HEIGHT;

		HLine(r_x, r_y, r_length, r_colour);

		r_x = rand() % WIDTH; 

		Line(r_x1, r_y1, r_x, r_y, r_colour);
	}

	return;
}
