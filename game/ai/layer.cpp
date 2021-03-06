#include "layer.h"

Layer::Layer(int nodesNBR, int inputsNBR)
{
    initLayer(nodesNBR,inputsNBR);
}

Layer::~Layer()
{
    while(!this->nodes.empty()){
        delete this->nodes.at(0);
        this->nodes.removeFirst();
    }
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

        // Initialize new node
        tempNode = new Nodes();
        tempNode->bias = randRange(-NN_BIASES_RANGE, NN_BIASES_RANGE);
        for (int i = 0; i < inputsNBR; ++i) {
            tempNode->weights.append(randRange(-NN_WEIGHTS_RANGE, NN_WEIGHTS_RANGE));
        }
        this->nodes.append(tempNode);
    }
}

double Layer::randRange(int min, int max)
{
    double f = (double)rand() / RAND_MAX;
    return min + f * (max - min);
}

double Layer::tweakValue()
{
    return randRange(-1,1);
}

// Makes sum of all given node weights*inputs
double Layer::calcInputWeights(QVector<double> inputs,int currentNode)
{
    double result = 0;

    // Checking if every connection between input and node has a weight
    if(inputs.size() == this->nodes.at(0)->weights.size())
    {
        for (int i = 0; i < inputs.size(); ++i) {
            result += inputs.at(i)*this->nodes.at(currentNode)->weights.at(i);
        }
    }else
    {
        qDebug()<<"Nodes connection not matching weight number";
    }
    return result;
}

void Layer::tweakLayer()
{
    double temp1;
    double temp2;
    // Tweak weight and bias
    for (int i = 0; i < nodes.size(); ++i) {
        for (int j = 0; j < this->nodes.at(i)->weights.size(); ++j) {
            // Changing weight and random
            temp1 = this->nodes.at(i)->weights.at(j) + tweakValue();
            this->nodes.at(i)->weights.replace(j,temp1);
        }
        // Changing bias at random but still needs to be smaller then sum of weight
        temp1 = this->nodes.at(i)->bias + tweakValue(); // representing new bias
        temp2 = 0;                                      // sum of weights
        for (int j = 0; j < this->nodes.at(i)->weights.size(); ++j) {
            // Get sum of weight from this node
            temp2 += this->nodes.at(i)->weights.at(j);
        }

        // Check if bias is valid
        if(temp2 > temp1)
        {
            this->nodes.at(i)->bias = temp1;
        }

        // If current bias bigger then weighted sum correct it
        if(this->nodes.at(i)->bias >= temp2)
        {
            this->nodes.at(i)->bias = temp2 - abs(tweakValue());
        }
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
    // Get weights
    for (int i = 0; i < this->nodes.size(); ++i) {
        for (int j = 0; j < this->nodes.at(i)->weights.size()-1; ++j) {
            result += QString::number(this->nodes.at(i)->weights.at(j)) + ",";
        }
        result += QString::number(this->nodes.at(i)->weights.at(this->nodes.at(i)->weights.size()-1))+"\r\n";
    }

    // Get biases
    for (int i = 0; i < this->nodes.size()-1; ++i) {
        result += QString::number(this->nodes.at(i)->bias)+",";
    }
    result += QString::number(this->nodes.at(this->nodes.size()-1)->bias)+"\r\n";
    return result;
}

// Give weights for nodes of this layer
bool ok = false;
void Layer::setWeights(QVector<QStringList> strings)
{
    QStringList templist;
    if(strings.size() == nodes.size()){
        // Go past every node / full vector(list)
        for (int i = 0; i < strings.size(); ++i) {
            templist = strings.at(i);
            // Go past every weight / string list value
            for (int j = 0; j < strings.at(i).size(); ++j) {
                this->nodes.at(i)->weights.replace(j,templist.at(j).toDouble(&ok));

            }
        }
    }else
    {
        qDebug()<<"Amount of nodes and values do not match: Weight";
    }
}

// Give biases for nodes of this layer
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

void Layer::setWeightsPartialy(QVector<QStringList> strings)
{
    QStringList templist;
    if(strings.size() == nodes.size()){
        // Go past every node / full vector(list)
        for (int i = 0; i < strings.size(); ++i) {
            templist = strings.at(i);
            // Go past every weight / string list value
            for (int j = 0; j < strings.at(i).size(); ++j) {
                if(rand()%HERITAGE_RATE == 1)
                    this->nodes.at(i)->weights.replace(j,templist.at(j).toDouble(&ok));

            }
        }
    }else
    {
        qDebug()<<"Amount of nodes and values do not match: Weight";
    }
}

void Layer::setBiasesPartialy(QStringList strings)
{
    if(strings.size() == nodes.size()){
        for (int i = 0; i < nodes.size(); ++i) {
            if(rand()%HERITAGE_RATE == 1)
                this->nodes.at(i)->bias = strings.at(i).toDouble(&ok);
        }
    }else
    {
        qDebug()<<"Amount of nodes and values do not match: Bias";
    }
}
