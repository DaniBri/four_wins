#include "manager.h"

Manager::Manager()
{
    // seed for randomizer generator
    qsrand(QTime::currentTime().msec());
    initGame();
}

void Manager::initRelations(Gamefield *field)
{
    this->field = field;
    this->ai1.initRelations(field);
    this->ai2.initRelations(field);
}

void Manager::playNGame(int gamesNBR, bool preloadAI)
{
    bool timedOut = false;
    // preload a given AI file already existing in build path
    QFile file(QDir::currentPath() + "/winnerAI.txt");
    if (preloadAI && file.exists()) {
        ai1.importNetwork("winnerAI");
    }
    for (int i = 0; i < gamesNBR; ++i) {
        int timeoutCounter = 0;
        while(playOneMove())
        {
            timeoutCounter++;
            if(timeoutCounter == MOVES_TIMEOUT)
            {
                timedOut = true;
                break;
            }
        }

        // make shure both AIs are the same
        ai1.importNetwork("winnerAI");
        ai2.importNetwork("winnerAI");

        // tweek AI1
        ai1.tweekNetwork();

        // randomize AI1 if it timed out
        if(timedOut)
        {
            qDebug()<<"timed out multiple tweeks";
            timedOut = false;
            ai1.tweekNetwork();
            ai1.tweekNetwork();
            field->resetField();
        }
    }
}

bool Manager::playOneMove()
{

    switch (gameState) {
    case -1:
        qDebug()<<"Ended in Draw or ERROR";
        initGame();
        return false;
    case 0:
        if(player)
        {
            gameState = ai_place_token(1);
        }else
        {
            gameState = ai_place_token(2);
        }
        player = !player;
        return true;
    case 1:
        ai1.printNetwork("winnerAI");
        initGame();
        return false;
    case 2:
        ai2.printNetwork("winnerAI");
        initGame();
        return false;
    default:
        qDebug()<<"ERROR unknown gamestate";
        initGame();
        return false;
    }
}

// -1 = draw, 0 = game ongoing, 1 = p1 won, 2 = p2 won
int Manager::ai_place_token(int i)
{
    int column_to_play;
    if(i == 1) {
        column_to_play = ai1.calcOut(field->getInputs());
        return field->action(column_to_play);
    }
    if(i == 2) {
        column_to_play =  ai2.calcOut(field->getInputs());
        return field->action(column_to_play);
    }
    return -1; //ERROR OR DRAW
}

void Manager::initGame()
{
    player = true;
    gameState = 0;
}
