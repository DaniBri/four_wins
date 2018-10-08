#ifndef GAMEFIELD_H
#define GAMEFIELD_H
#include "globaldefiner.h"

class Display;
class Gamefield
{
public:
    Gamefield();
    ~Gamefield();

    bool placeToken(int column, int player);
    void initRelations(Display* disp);
    int getBoardAt(int x, int y);

private:
    void winCondition(int player);
    int board[NBR_HOLE_HOR][NBR_HOLE_VER];
    void displayBoard();
    Display* display;

};

#endif // GAMEFIELD_H
