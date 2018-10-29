#include "fighter.h"


Fighter::Fighter(int stateToPlay, Gamefield *field)
{
    this->stateToPlay = stateToPlay;
    this->field = field;
    this->ai = new NN_AI(field);
}

// 0 = nothing, 1 = p1 won, 2 = p2 won
int Fighter::aiPlayOnce()
{
    if(field->getState() == this->stateToPlay )
    {
        int temp = field->action( ai->calcOut(field->getInputs()));
        return temp;
    }
    return 0;
}

void Fighter::loadAI(QString aiName)
{
    // preload a given AI file already existing in build path
    QFile file(QDir::currentPath() + "/" + aiName + ".txt");
    if (file.exists()) {
        this->ai->importNetwork(aiName);
    }else
    {
        qDebug()<<"file doesn't exist";
    }
}
