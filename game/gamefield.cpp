#include "gamefield.h"
#include "display.h"
#include "QDebug"
Gamefield::Gamefield()
{
    // init board
    for (int i = 0; i < NBR_HOLE_HOR; ++i) {
        for (int j = 0; j < NBR_HOLE_VER; ++j) {
            this->board[i][j] = 0;
        }
    }

}

Gamefield::~Gamefield()
{

}

// placing token in top of column and drawing int on board
bool Gamefield::placeToken(int column, int player)
{
    for (int i = NBR_HOLE_VER-1; i >= 0; --i) {
        if(this->board[column][i] == 0)
        {
            this->board[column][i] = player;

            // update display if not know send msg error
            if(this->display != NULL)
            {
                this->display->repaint();
                return true;
            }else
            {
                // MSG ERROR no display in relation
            }
        }
    }
    return false;
}

void Gamefield::initRelations(Display *disp)
{
    this->display=disp;
}

int Gamefield::getBoardAt(int x, int y)
{
    return this->board[x][y];
}
