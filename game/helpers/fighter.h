#ifndef FIGHTER_H
#define FIGHTER_H
#include "ai/nn_ai.h"
#include "QString"
#include "basic/gamefield.h"


class Fighter
{
public:
    Fighter(int stateToPlay);
    int aiPlayOnce();
    void loadAI(QString aiName);
    void initRelations(Gamefield* field);
private:
    int stateToPlay;
    NN_AI ai;
    Gamefield* field;
};

#endif // FIGHTER_H
