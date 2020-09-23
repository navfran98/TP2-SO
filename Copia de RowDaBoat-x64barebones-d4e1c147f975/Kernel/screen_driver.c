#include <lib.h>    // ya que necesita a mi strlen
#include <font.h>   // ya que necesita a CHR_HGTH y a CHR_WDTH
#include <video_driver.h>  // ya que necesita el drawPixel, el clearVideoDisplay

static void draw_char_in_current_writing_position(char character, int fontColor, int backgroundColor);
static void move_screen_upwards(int fontColor, int backgroundColor);
static void drawCharInSpecificSpot(int x, int y, char character, int fontColor, int backgroundColor);
static void clearChar(int x, int y);
// BORRAR ESTE SIGUIENTE DESPUESSSSS
// y tambien borrar abajo de todo el "drawStringInSpeficiposition"
void drawNumberInSpecificSpot(int x, int y, int number, int fontColor, int backgroundColor);

// la pantalla tiene 48 filas por 128 columnas
// necesitamos almacenar todas las lineas, para asi poder moverlas todas hacia abajo
static char screen[48][128];

static unsigned int screen_width_in_characters;
static unsigned int screen_height_in_characters;

// we start writing at the top of the screen
static int writing_line_width = 0;
static int writing_line_height = 0;

// this is to prevent the '\b' from deleting things that are already on the screen beforehand
static int characters_already_written = 0;

// default colors in this screen
static const int FONT_COLOR = 0xFFFFFF;
static const int BACKGROUND_COLOR = 0x000000;

void init_screen_driver(unsigned int screen_width_in_pixels, unsigned int screen_height_in_pixels) {
    screen_height_in_characters = screen_height_in_pixels / CHAR_HEIGHT;
    screen_width_in_characters = screen_width_in_pixels / CHAR_WIDTH;
    // drawStringInSpecificSpot(300, 300, "estoy en init_screen_driver", FONT_COLOR, BACKGROUND_COLOR);
}

void drawChar(char character, int fontColor, int backgroundColor) {
    if(character == '\n'){
        if(writing_line_height == screen_height_in_characters-1) {
            // we have reached end of screen
            move_screen_upwards(fontColor, backgroundColor);
        }
        else{
            writing_line_height += 1;
        }
        writing_line_width = 0;
        characters_already_written = 0;
    }
    else if(character == '\b') {
        if(characters_already_written - 2 > 0) {
            if(writing_line_width > 0){
                writing_line_width -= 1;
            }
            else {
                writing_line_height -= 1;
                writing_line_width = screen_width_in_characters - 1;
            }
            screen[writing_line_width][writing_line_height] = '\0';
            clearChar(writing_line_width, writing_line_height);
            characters_already_written -= 1;
        }
    }
    else{ // it is a normal character
        if(writing_line_width == screen_width_in_characters) {
            // we have reached end of line
            writing_line_width = 0;
            if(writing_line_height == screen_height_in_characters - 1) {
                // we have reached end of screen
                move_screen_upwards(fontColor, backgroundColor);
            }
            else{
                writing_line_height += 1;
            }
        }
        screen[writing_line_height][writing_line_width] = character;
        draw_char_in_current_writing_position(character, fontColor, backgroundColor);
        characters_already_written += 1;
    }
}

static void draw_char_in_current_writing_position(char character, int fontColor, int backgroundColor) {
    int aux_x = writing_line_width*CHAR_WIDTH;
    int aux_y = writing_line_height*CHAR_HEIGHT;
    char bitsIsPresent;
    unsigned char* toDraw = charBitmap(character);
    for(int i=0; i<CHAR_HEIGHT; i++) {
        for(int j=0; j<CHAR_WIDTH; j++) {
            bitsIsPresent = (1 << (CHAR_WIDTH - j)) & toDraw[i];
            if(bitsIsPresent)
                drawPixel(aux_x, aux_y, fontColor);
            else
                drawPixel(aux_x, aux_y, backgroundColor);
            aux_x += 1;
        }
        aux_x = writing_line_width*CHAR_WIDTH;
        aux_y += 1;
    }
    // once written, we move one character to the right
    writing_line_width += 1;
}

static void move_screen_upwards(int fontColor, int backgroundColor) {
    for(int i=1; i<screen_height_in_characters; i++) {
        char* line_to_copy = screen[i];
        for(int j=0; j<screen_width_in_characters; j++) {
            // we update the matrix
            screen[i-1][j] = line_to_copy[j];
            // then we update the screen
            if(line_to_copy[j] == '\0') 
                clearChar(j, i-1);
            else
                drawCharInSpecificSpot(j, i-1, line_to_copy[j], FONT_COLOR, BACKGROUND_COLOR);
        }
    }
    // then we clear the last line
    for(int i=0; i<screen_width_in_characters; i++) {
        screen[screen_height_in_characters-1][i] = '\0';
        clearChar(i, screen_height_in_characters-1);
    }
}

static void clearChar(int x, int y) {
    int aux_x = x*CHAR_WIDTH;
    int aux_y = y*CHAR_HEIGHT;
    int black_color = 0x000000;
    for(int i=0; i<CHAR_HEIGHT; i++) {
        for(int j=0; j<CHAR_WIDTH; j++) {
            drawPixel(aux_x, aux_y, black_color);
            aux_x += 1;
        }
        aux_x = x*CHAR_WIDTH;
        aux_y += 1;
    }
}

void drawString(char* string) {
    for(int i=0; i<strlen(string); i++){
        drawChar(string[i], FONT_COLOR, BACKGROUND_COLOR);
    }
}

void drawNumber(int number, int fontColor, int backgroundColor) {
    int is_negative_number = 0;
    if(number < 0) {
        is_negative_number = 1;
        number *= (-1);
    }

    int number_length = getNumberLength(number);
    char* number_converted = num_to_string(number);

    if(is_negative_number) {
        drawChar('-', fontColor, backgroundColor);
    }

    for(int i=0; i<number_length; i++) {
        drawChar(number_converted[i], fontColor, backgroundColor);
    }
}

static void drawCharInSpecificSpot(int x, int y, char character, int fontColor, int backgroundColor) {
    int aux_x = x*CHAR_WIDTH;
    int aux_y = y*CHAR_HEIGHT;
    char bitsIsPresent;
    unsigned char* toDraw = charBitmap(character);
    for(int i = 0; i<CHAR_HEIGHT; i++){
        for(int j=0; j<CHAR_WIDTH; j++){
            bitsIsPresent = (1 << (CHAR_WIDTH - j)) & toDraw[i];
            if(bitsIsPresent)
                drawPixel(aux_x, aux_y, fontColor);
            else
                drawPixel(aux_x, aux_y, backgroundColor);
            aux_x += 1;
        }
        aux_x = x*CHAR_WIDTH;
        aux_y += 1;
    }
}

void drawStringInSpecificSpot(int x, int y, char* string, int fontColor, int backgroundColor){
    int aux_x = x;
    for(int i=0; i<strlen(string); i++){
        drawCharInSpecificSpot(aux_x, y, string[i], fontColor, backgroundColor);
        aux_x += CHAR_WIDTH;
    }
}

void drawNumberInSpecificSpot(int x, int y, int number, int fontColor, int backgroundColor) {
    int number_length = getNumberLength(number);
    char* number_converted = num_to_string(number);
    int aux_x = x;
    for(int i=0; i<number_length; i++) {
        drawCharInSpecificSpot(aux_x, y, number_converted[i], fontColor, backgroundColor);
        aux_x += CHAR_WIDTH;
    }
}

void clearScreen() {
    clearVideoDisplay();
}