#include "gamefield.h"
#include "display.h"


using namespace std;

Gamefield::Gamefield()
{
    this->gamestate = PLAYER1;
    initBoard();
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
            return true;
        }
    }
    qDebug()<<"ERROR 006: Column Full";
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

void Gamefield::initBoard()
{
    // init board
    for (int i = 0; i < NBR_HOLE_HOR; ++i) {
        for (int j = 0; j < NBR_HOLE_VER; ++j) {
            this->board[i][j] = 0;
        }
    }
}

void Gamefield::dispAction(int column)
{
    switch (this->gamestate) {
    case PLAYER1:
        if(placeToken( column,  1))
        {
            gamestate = PLAYER2;
        }
        if(winCondition(1))
        {
            initBoard();
            gamestate = PLAYER1;
        }
        break;
    case PLAYER2:
        if(placeToken( column,  -1))
        {
            gamestate = PLAYER1;
        }
        if(winCondition(-1))
        {
            initBoard();
            gamestate = PLAYER1;
        }
        break;
    default:
        break;
    }
    this->display->repaint();
}

bool Gamefield::winCondition(int player)
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
        qDebug()<<"PLAYER"<<player<<"WINS!!!";
    }
    return won;
}

void Gamefield::displayBoard()
{
    for(int i = 0; i < NBR_HOLE_VER; i++)  // loop for  lines
       {
           for(int j = 0; j < NBR_HOLE_HOR; j++)  // loop for the elements on the line
           {
               cout << this->board[j][i];  // display the current element out of the array
           }
       cout << endl;  // when the inner loop is done, go to a new line
       }
}

QVector<double> Gamefield::getInputs()
{
    QVector<double> result;

    if (gamestate == PLAYER1) {
        for(int i = 0; i < NBR_HOLE_VER; i++)  // loop for  lines
           {
               for(int j = 0; j < NBR_HOLE_HOR; j++)  // loop for the elements on the line
               {
                   result.append(this->board[j][i]);
               }
           }
    }else if (gamestate == PLAYER1) {
        for(int i = 0; i < NBR_HOLE_VER; i++)  // loop for  lines
           {
               for(int j = 0; j < NBR_HOLE_HOR; j++)  // loop for the elements on the line
               {
                   result.append(-this->board[j][i]);
               }
           }
    }

    return result;
}

