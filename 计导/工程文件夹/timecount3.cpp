#include "myLib1.h"

extern clock_t startTime ;
extern clock_t endTime ;
extern const int LEFT_RIGHT_MARGIN;
extern const int BLOCK_SIZE;

void timeCount(int width) {
    char t[100];
    int minutes = 0, seconds = 0,duration = (endTime - startTime)/1000;
    minutes = duration / 60;
    seconds = duration % 60;
    settextcolor(BLACK);
    settextstyle(20, 0, "Consolas");
    sprintf_s(t, "%02d:%02d", minutes, seconds);
    outtextxy(width-LEFT_RIGHT_MARGIN-2*BLOCK_SIZE, 25, t);
}

