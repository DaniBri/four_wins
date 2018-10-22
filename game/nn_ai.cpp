#include "nn_ai.h"
#include "gamefield.h"


NN_AI::NN_AI()
{
    initNetwork();
}

int NN_AI::calcOut(QVector<double> boardInput)
{

    this->layer1->calcValues(boardInput);
    this->layer2->calcValues(this->layer1->getNodesValues());
    this->layerOUT->calcValues(this->layer2->getNodesValues());


    int result = 0;
    for (int i = 1; i < this->layerOUT->getNodesValues().size(); ++i) {
        if(this->layerOUT->getNodesValues().at(i) > this->layerOUT->getNodesValues().at(result))
        {
            result = i;
        }
    }
    qDebug()<<result;
    qDebug()<<this->layerOUT->getNodesValues();
    return result;
}

void NN_AI::tweekNetwork()
{

}

void NN_AI::printNetwork()
{

}

void NN_AI::readNetwork()
{

}

void NN_AI::initNetwork()
{
    this->layer1 = new Layer(NN_LAYER_NODES,NBR_HOLE_VER*NBR_HOLE_HOR);
    this->layer2 = new Layer(NN_LAYER_NODES,NN_LAYER_NODES);
    this->layerOUT = new Layer(NBR_HOLE_HOR,NN_LAYER_NODES);
}
