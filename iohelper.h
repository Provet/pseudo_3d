#pragma once

#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <vector>
#include <ctype.h>

using namespace std;

struct PlayerInfo{
    double PosX;
    double PosY;
    double DirX;
    double DirY;
};

void IOInit() {
    termios t;
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag &= ~ICANON; 
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

char GetKey() {
    return tolower(getchar());
}

class ScreenManager{
public:
    ScreenManager(int w, int h) 
        : winW(w)
        , winH(h)
    {
        screen.resize(winH, vector<int>(winW, 0));
    }

    void RenderFrame(const PlayerInfo& pi) const {
        printf("\x1b[%dA", winH+3);  // clear console
        for (int i = 0; i != winH; ++i) {
            for (int j = 0; j != winW; ++j) {
                switch(screen[i][j]) {
                    case 0: printf(" "); break;
                    case 1: printf("."); break;
                    case 2: printf("\u2588"); break;
                    case 3: printf("\u2593"); break;
                    case 4: printf("\u2592"); break;
                    case 5: printf("\u2591"); break;
                    case 6: printf("|"); break;
                    case 7: printf("\u2587"); break;
                    case 8: printf("="); break;
                    case 10: printf("#"); break;
                    case 11: printf("x"); break;
                    case 12: printf("~"); break;
                    case 13: printf("-"); break;
                }
            }
            printf("\n");
        }
        printf("X: %f;  Y: %f;  DirX: %f;  DirY: %f\n", pi.PosX, pi.PosY, pi.DirX, pi.DirY);
    }

    void DrawColumn(int wallH, int col, bool isEdge) {
        int row = 0;

        for (int j = 0; j < (winH - wallH) / 2 + wallH % 2; ++j) { // draw sky
            screen[row++][col] = 0;
        }

        int wallCode = 2;
        if (isEdge) {
            wallCode = 6;
        } else if (wallH < 5) {
            wallCode = 8;
        } else if (wallH < 10) {
            wallCode = 5;
        } else if (wallH < 15) {
            wallCode = 4;
        } else if (wallH < 20) {
            wallCode = 3;
        }        
        for (int j = 0; j < wallH; ++j) { // draw wall
            screen[row++][col] = wallCode;
        }

        for (int j = 0; j < (winH - wallH) / 2; ++j) { // draw floor
            if (row == winH) break;
            int floorCode;
            if (row < 43) {
                floorCode = 13;
            } else if (row < 48) {
                floorCode = 12;
            } else if (row < 54) {
                floorCode = 11;
            } else {
                floorCode = 10;
            }
            screen[row++][col] = floorCode;
        }
    }

private:
    vector<vector<int>> screen;
    int winW;
    int winH;
};

