#include <stdbool.h>
#include <stdio.h>

#define MINI_PIXELATED
#define MINI_IMPLEMENTATION
#include "miniengine.h"

int main() {
    windowInit(400, 400, "Hello World!");
	
	while (1) {
        windowClear(colorRGBA(0x80, 0x90, 0xA0, 255));
        
        Vec2 mouse = mousePosition();
        
        if (mouseDown(MB_LEFT)) {
           printf("hello!\n");
        }
        
        int points[] = { 20, 20, 50, 50, 80, 100, 10, 60 };
        drawPolygonFilled(points, sizeof(points) / sizeof(points[0]), colorRGB(255, 0, 255));
        drawLine(0, 0, 400, 400, colorRGB(255, 255, 255));
        drawTriangleFilled(10, 10, 100, 10, 55, 100, colorRGB(255, 100, 100));
        drawRectangle(10, 40, 100, 100, colorRGB(255, 0, 0));
        windowUpdate();
        
	}
    windowClose();
    
	return 0;
}


