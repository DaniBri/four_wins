#ifndef MANAGER_H
#define MANAGER_H
#include "gamefield.h"
#include "nn_ai.h"
#include "QDir"

class Manager
{
public:
    Manager();
    void initRelations(Gamefield* field);
    void playNGame(int gamesNBR, bool preloadAI);

private:
    NN_AI ai1;
    NN_AI ai2;
    Gamefield* field;
    bool player;
    int gameState;
    bool playOneMove();
    int ai_place_token(int i);
    void initGame();
};

#endif // MANAGER_H
