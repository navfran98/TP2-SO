#ifndef _SCREEN_DRIVER_
#define _SCREEN_DRIVER_

void drawCharInSpecificSpot(int x, int y, char character, int fontColor, int backgroundColor);
void drawStringInSpecificSpot(int x, int y, char* string, int fontColor, int backgroundColor);
void drawNumberInSpecificSpot(int x, int y, int number, int fontColor, int backgroundColor);

// hace falta que este drawChar este aca????
void drawChar(char character, int fontColor, int backgroundColor);
void drawString(char* string);

void init_screen_driver(unsigned int screen_width, unsigned int screen_height);

#endif