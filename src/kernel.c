#include "rootUtils.h"

#define SCRWIDTH  80
#define SCRHEIGHT 24

char *TEXTBUFFER = (char *) 0x000B8000;
short terminalX = 0;
short terminalY = 0;

void drawChar(char drawable, unsigned short x, unsigned short y){
  unsigned int offset = 2 * (SCRWIDTH * y + x);
  TEXTBUFFER[offset] = drawable;
}

void moveCursor(unsigned short x, unsigned short y){
  unsigned pos = y * SCRWIDTH + x;
  outb(0x3D4, 14); //command_port, command
  outb(0x3D5, ((pos >> 8) & 0x00FF));   //data_port, data
  outb(0x3D4, 15);
  outb(0x3D5, (pos & 0x00FF));
}

void terminalWrite(char* string){
  while(*string){
    if(*string == '\n'){
      terminalX = 0;
      terminalY++;
      if(terminalY == SCRHEIGHT){
        for(unsigned short i = 2*SCRWIDTH; i<2*SCRWIDTH*SCRHEIGHT; i++)
          TEXTBUFFER[i - 2*SCRWIDTH] = TEXTBUFFER[i];
        for(unsigned short i = 0; i<SCRWIDTH; i++){
          TEXTBUFFER[2*SCRWIDTH*(SCRHEIGHT - 1) + 2*i] = ' ';
          TEXTBUFFER[2*SCRWIDTH*(SCRHEIGHT - 1) + 2*i + 1] = 0;
        }
      }
    } else {
      unsigned char myChar = *string;
      if(myChar>31 && myChar<127)
        TEXTBUFFER[2*(SCRWIDTH * terminalY + terminalX)] = myChar;
      else
        TEXTBUFFER[2*(SCRWIDTH * terminalY + terminalX)] = '?';
      terminalX++;
      if(terminalX==SCRWIDTH){
        terminalX=0;
        terminalY++;
        if(terminalY==SCRHEIGHT){
          for(unsigned short i = 2*SCRWIDTH; i<2*SCRWIDTH*SCRHEIGHT; i++)
            TEXTBUFFER[i - 2*SCRWIDTH] = TEXTBUFFER[i];
          for(unsigned short i = 0; i<SCRWIDTH; i++){
            TEXTBUFFER[2*SCRWIDTH*(SCRHEIGHT - 1) + 2*i] = ' ';
            TEXTBUFFER[2*SCRWIDTH*(SCRHEIGHT - 1) + 2*i + 1] = 0;
          }
        }
      }
    }
    string = string+1;
  }
  moveCursor(terminalX, terminalY);
}

void clearScreen(){
  for(short i=0; i<SCRWIDTH;i++)
    for(short j=0; j<SCRHEIGHT;j++)
      drawChar(' ', i, j);
  terminalX = 0;
  terminalY = 0;

}

void kernel_main(){
  clearScreen();
  terminalWrite("ShitOS\n");
  terminalWrite("version 0.0.1\n");
  while(1){
  }
}
