#include "layer.h"

Layer::Layer(int nodesNBR, int inputsNBR)
{
    // seed for randomizer generator
    qsrand(QTime::currentTime().msec());
    initLayer(nodesNBR,inputsNBR);
}

void Layer::calcValues(QVector<double> inputs)
{
    for (int i = 0; i < nodes.size(); ++i) {
        this->nodes.at(i)->value = fastSigmoid(calcInputWeights(inputs,i)-this->nodes.at(i)->bias);
        qDebug()<<"nodes at"<<i<<"value"<<this->nodes.at(i)->value;
    }
}

double Layer::fastSigmoid(double x)
{
    return x / (1 + qAbs(x));
}

void Layer::initLayer(int nodesNBR, int inputsNBR)
{
    Nodes* tempNode;
    for (int i = 0; i < nodesNBR; ++i) {

        // init new node
        tempNode = new Nodes;
        tempNode->bias = randRange(-NN_BIASES_RANGE, NN_BIASES_RANGE);
        for (int i = 0; i < inputsNBR; ++i) {
            tempNode->weights.append(randRange(-NN_WEIGHTS_RANGE, NN_WEIGHTS_RANGE));
        }
        this->nodes.append(tempNode);
    }
}

double Layer::randRange(int min, int max)
{
    return  qrand() % ((max + 1) - min) + min;
}

double Layer::calcInputWeights(QVector<double> inputs,int currentNode)
{
    double result = 0;
    // checking if every connection between input and node has a weight
    if(inputs.size() == this->nodes.at(0)->weights.size())
    {
        for (int i = 0; i < inputs.size(); ++i) {
            result += inputs.at(i)*this->nodes.at(currentNode)->weights.at(i);
        }
    }else{
        qDebug()<<"Nodes connection not matching weight number";
    }
    return result;
}

void Layer::tweekLayer()
{
    //TODO
}

QVector<double> Layer::getNodesValues()
{
    QVector<double> result;
    for (int i = 0; i < this->nodes.size(); ++i) {
        result.append(this->nodes.at(i)->value);
    }
    return result;
}
