#ifndef GAMEFIELD_H
#define GAMEFIELD_H
#include "globaldefiner.h"
#include "QDebug"
#include "QVector"
#include <iostream>

class Display;

enum Phase{
    PLAYER1, PLAYER2
};

class Gamefield
{
public:
    Gamefield();
    ~Gamefield();

    void initRelations(Display* disp);
    int getBoardAt(int x, int y);
    void dispAction(int column);
    QVector<double> getInputs();
    bool placeToken(int column, int player);

private:
    Phase gamestate;
    int board[NBR_HOLE_HOR][NBR_HOLE_VER];
    Display* display;

    void initBoard();

    bool winCondition(int player);
    void displayBoard();


};

#endif // GAMEFIELD_H
