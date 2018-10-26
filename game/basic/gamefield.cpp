#include "gamefield.h"
#include "display.h"


using namespace std;

Gamefield::Gamefield()
{
    resetField();
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
            // remove comments to see step by step in consol
            //displayBoard();
            return true;
        }
    }
    return false;
}

void Gamefield::initRelations(Display *disp)
{
    this->display = disp;
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


// -1 = draw, 0 = game ongoing, 1 = p1 won, 2 = p2 won
int Gamefield::action(int column)
{
    int result = 0;
    //trying to place token in fiven column
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
            result = 1;
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
            result = 2;
        }
        break;
    default:
        result = -1;
        break;
    }

    //in case of full board ends in draw
    bool boardfull = true;
    for (int i = 0; i < NBR_HOLE_VER; ++i) {
        for (int j = 0; j < NBR_HOLE_HOR; ++j) {
            if(this->board[j][i] == 0)
            {
                boardfull = false;
            }
        }
    }

    if(boardfull)
    {
        initBoard();
        gamestate = PLAYER1;
        result = -1;
    }
    this->display->repaint();
    return result;
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

    //if(won)
    //qDebug()<<"player"<< player << "won";
    return won;
}

void Gamefield::displayBoard()
{
    for(int i = 0; i < NBR_HOLE_VER; i++)  // loop for  lines
    {
        for(int j = 0; j < NBR_HOLE_HOR; j++)  // loop for the elements on the line
        {
            switch (this->board[j][i]) { // display the current element out of the array
            case 1:
                cout << 1;
                break;
            case -1:
                cout << 2;
                break;
            default:
                cout << 0;
                break;
            }
        }
        cout << endl;  // when the inner loop is done, go to a new line
    }
}

void Gamefield::resetField()
{
    this->gamestate = PLAYER1;
    initBoard();
}

int Gamefield::getState()
{
    return (int)this->gamestate;
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
        return result;
    }
    if (gamestate == PLAYER2) {
        for(int i = 0; i < NBR_HOLE_VER; i++)  // loop for  lines
        {
            for(int j = 0; j < NBR_HOLE_HOR; j++)  // loop for the elements on the line
            {
                result.append(-this->board[j][i]);
            }
        }
        return result;
    }

    return result;
}

