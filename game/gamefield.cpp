#include "gamefield.h"
#include "display.h"
#include "QDebug"
#include <iostream>

using namespace std;

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
            displayBoard();
            // update display if not know send msg error
            if(this->display != NULL)
            {
                this->display->repaint();
                winCondition(player);
                return true;
            }else
            {
                // MSG ERROR no display in relation
                qDebug()<<"ERROR 002: No display known";
                return false;
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

void Gamefield::winCondition(int player)
{
    bool won = false;
    int counter = 0;

    // check rows
    for (int i = 0; i < NBR_HOLE_VER; ++i) {
        counter = 0;
        for (int j = 0; j < NBR_HOLE_HOR; ++j) {
            if(this->board[j][i] == player)
            {
                counter++;
            }else{
                counter = 0;
            }
            if(counter == 4)
            {
                won = true;
                break;
            }
        }
    }

    // check columns
    for (int i = 0; i < NBR_HOLE_HOR; ++i) {
        counter = 0;
        for (int j = 0; j < NBR_HOLE_VER; ++j) {
            if(this->board[i][j] == player)
            {
                counter++;
            }else{
                counter = 0;
            }
            if(counter == 4)
            {
                won = true;
                break;
            }
        }
    }

    // check diagonal left -> right && up -> down
    for (int i = 0; i < NBR_HOLE_HOR; ++i) {
        for (int j = 0; j < NBR_HOLE_VER; ++j) {
            counter = 0;
            for (int k = 0; k < 4; ++k) {
                if(i+k < NBR_HOLE_HOR && j+k < NBR_HOLE_VER)
                {
                    if(this->board[i+k][j+k] == player)
                    {
                        counter++;
                    }else{
                        counter = 0;
                    }
                    if(counter == 4)
                    {
                        won = true;
                        break;
                    }
                }
            }
        }
    }

    // check diagonal right -> left && up -> down
    for (int i = 0; i < NBR_HOLE_HOR; ++i) {
        for (int j = 0; j < NBR_HOLE_VER; ++j) {
            counter = 0;
            for (int k = 0; k < 4; ++k) {
                if(i-k >= 0 && j+k < NBR_HOLE_VER)
                {
                    if(this->board[i-k][j+k] == player)
                    {
                        counter++;
                    }else{
                        counter = 0;
                    }
                    if(counter == 4)
                    {
                        won = true;
                        break;
                    }
                }
            }
        }
    }


    // send out msg if winner
    if(won)
    {
        qDebug()<<"There is a winner!!!";
    }
}

void Gamefield::displayBoard()
{
    for(int i=0; i < NBR_HOLE_VER; i++)  // loop for  lines
       {
           for(int j=0; j < NBR_HOLE_HOR; j++)  // loop for the elements on the line
           {
               cout<<this->board[j][i];  // display the current element out of the array
           }
       cout<<endl;  // when the inner loop is done, go to a new line
       }
}
