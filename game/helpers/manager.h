#ifndef MANAGER_H
#define MANAGER_H
#include "basic/gamefield.h"
#include "ai/nn_ai.h"
#include "QVector"
#include "QDir"
#include "QElapsedTimer"

class Manager
{
public:
    Manager(Gamefield* field);
    void playNTournaments(int nbrOfTourns);


private:
    NN_AI* ai1;
    NN_AI* ai2;
    Gamefield* field;
    bool player;
    int gameState;
    int lastWinner;
    bool playOneMove();
    int ai_place_token(int i);
    void initGame();
    QVector<NN_AI *> initTournametsAI();
    void playGame();
    void playTournament();
    NN_AI* poolFight(QVector<NN_AI*> ais, int start);
    void aiSwitchSides();
    void clearTourAI(QVector<NN_AI*> tournametsAI);
};

#endif // MANAGER_H
