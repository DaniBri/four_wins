#include "layer.h"

Layer::Layer(int nodesNBR, int inputsNBR)
{
    initLayer(nodesNBR,inputsNBR);
}

void Layer::calcValues(QVector<double> inputs)
{
    for (int i = 0; i < nodes.size(); ++i) {
        this->nodes.at(i)->value = fastSigmoid(calcInputWeights(inputs,i)-this->nodes.at(i)->bias);
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

double Layer::tweekValue()
{
    return randRange(-4,4)/10+randRange(-9,9)/100+randRange(-9,9)/1000;
}

// makes summ of all given node weights*inputs
double Layer::calcInputWeights(QVector<double> inputs,int currentNode)
{
    double result = 0;

    // checking if every connection between input and node has a weight
    if(inputs.size() == this->nodes.at(0)->weights.size())
    {
        for (int i = 0; i < inputs.size(); ++i) {
            result += inputs.at(i)*this->nodes.at(currentNode)->weights.at(i);
        }
    }else
    {
        qDebug()<<"Nodes connection not matching weight numberkkkkkkk";
    }
    return result;
}

void Layer::tweekLayer()
{
    double temp;
    // tweek weight and bias
    for (int i = 0; i < nodes.size(); ++i) {
        for (int j = 0; j < this->nodes.at(i)->weights.size(); ++j) {
            temp = this->nodes.at(i)->weights.at(j) + tweekValue();
            this->nodes.at(i)->weights.replace(j,temp);
        }
        this->nodes.at(i)->bias += tweekValue();
    }
}

void Layer::randomizeLayer()
{
    for (int i = 0; i < nodes.size(); ++i) {
        for (int j = 0; j < this->nodes.at(i)->weights.size(); ++j) {
            this->nodes.at(i)->weights.replace(j,randRange(-NN_WEIGHTS_RANGE, NN_WEIGHTS_RANGE));
        }
        this->nodes.at(i)->bias = randRange(-NN_BIASES_RANGE, NN_BIASES_RANGE) + tweekValue();
    }
}

QVector<double> Layer::getNodesValues()
{
    QVector<double> result;
    for (int i = 0; i < this->nodes.size(); ++i) {
        result.append(this->nodes.at(i)->value);
    }
    return result;
}


QString Layer::getWeightsAndBias()
{
    QString result;
    // get weights
    for (int i = 0; i < this->nodes.size(); ++i) {
        for (int j = 0; j < this->nodes.at(i)->weights.size()-1; ++j) {
            result += QString::number(this->nodes.at(i)->weights.at(j)) + ",";
        }
        result += QString::number(this->nodes.at(i)->weights.at(this->nodes.at(i)->weights.size()-1))+"\r\n";
    }

    //get biases
    for (int i = 0; i < this->nodes.size()-1; ++i) {
        result += QString::number(this->nodes.at(i)->bias)+",";
    }
    result += QString::number(this->nodes.at(this->nodes.size()-1)->bias)+"\r\n";
    return result;
}

// give weights for nodes of this layer
bool ok = false;
void Layer::setWeights(QVector<QStringList> strings)
{
    QStringList templist;
    if(strings.size() == nodes.size()){
        //go past every node / full vector(list)
        for (int i = 0; i < strings.size(); ++i) {
            templist = strings.at(i);
            //go past every weight / stringlist value
            for (int j = 0; j < strings.at(i).size(); ++j) {
                this->nodes.at(i)->weights.replace(j,templist.at(j).toDouble(&ok));

            }
        }
    }else
    {
        qDebug()<<"Amount of nodes and values do not match: Weight";
    }
}

// give biases for nodes of this layer
void Layer::setBiases(QStringList strings)
{
    if(strings.size() == nodes.size()){
        for (int i = 0; i < nodes.size(); ++i) {
            this->nodes.at(i)->bias = strings.at(i).toDouble(&ok);
        }
    }else
    {
        qDebug()<<"Amount of nodes and values do not match: Bias";
    }
}
