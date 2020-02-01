#include "Shapes.h"
#include "UsefulShapes.h"
#include "DemoGraphicsRoutines.h"

void drawRoundedRectangle(int x, int y, int length, int width, int fill, int fillColour, int borderColour, int radius){
    int new_x = x + radius;
    int new_y = y + width - radius;
    int new_length = length - 2*radius;
    int new_width = width - 2*radius;

    HLine(new_x, y, new_length, borderColour);
    HLine(new_x, new_y, new_length, borderColour);

    VLine(x, y + radius, new_width, borderColour);
    VLine(x + length - radius, y + width - radius, new_width, borderColour);

    // draw edges
    int quad[] = {0, 0, 0, 1, 1, 0, 0, 0};
    drawArc(new_x, y + radius, 0, radius, borderColour, quad);
    
    quad[3] = 0;
    quad[4] = 0;
    quad[5] = 1;
    quad[2] = 1;
 
    drawArc(x + length - radius, y + radius, 0, radius, borderColour, quad);

    quad[5] = 0;
    quad[2] = 0;
    quad[1] = 1;
    quad[6] = 1;
 
    drawArc(x + width - radius, y + radius, 0, radius, borderColour, quad);

    quad[1] = 0;
    quad[6] = 0;
    quad[0] = 1;
    quad[7] = 1;

    drawArc(x + length - radius, y + radius, 0, radius, borderColour, quad);
    return;
}


// void drawMenu(){

// }

// void drawClock(){

// }

// void drawButton(){
    
// }

void drawBox(int x, int y, int Colour, int ShadowColour, int borderColour, int length, int width, int borderThickness, int border){
    drawRectangle(x + 3*borderThickness, y + 3*borderThickness, length, width, 1, ShadowColour, ShadowColour);
    if (border){
        drawRectangle(x, y, length, width, 1, Colour, Colour);
        drawRectangle(x - borderThickness, y - borderThickness, length - 2*borderThickness, width - 2*borderThickness, 1, Colour, Colour);
    } else{
        drawRectangle(x, y, length, width, 1, Colour, Colour);
    }
   return;
}