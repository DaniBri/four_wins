#ifndef MANAGER_H
#define MANAGER_H
#include "gamefield.h"
#include "nn_ai.h"
class Manager
{
public:
    Manager();
    void initRelations(Gamefield* field);
    void ai_place_token(int i);

private:
    NN_AI ai1;
    NN_AI ai2;
    Gamefield* field;
};

#endif // MANAGER_H
