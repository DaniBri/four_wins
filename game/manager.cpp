#include "manager.h"

Manager::Manager()
{

}

void Manager::initRelations(Gamefield *field)
{
    this->field = field;
}

void Manager::ai_place_token(int i)
{
    int column_to_play;
    if(i == 1) {
        column_to_play = ai1.calcOut(field->getInputs());
        qDebug()<<column_to_play;
        field->placeToken(column_to_play,1);
    }
    if(i == 2) {
        column_to_play =  ai2.calcOut(field->getInputs());
        qDebug()<<column_to_play;
        field->placeToken(column_to_play,-1);
    }
}
