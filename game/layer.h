#ifndef LAYER_H
#define LAYER_H
#include "QVector"
#include "QTime"
#include "QDebug"


#include "globaldefiner.h"
struct Nodes{
    QVector<double> weights;
    double bias;
    double value;
};

class Layer
{
public:
    Layer(int nodesNBR, int inputsNBR);
    void calcValues(QVector<double> inputs);      // calculate all values of the nodes according to theyr weights and biases
    void tweekLayer();
    QVector<double> getNodesValues();
private:
    QVector<Nodes*> nodes;
    double calcInputWeights(QVector<double> inputs, int currentNode);
    double fastSigmoid(double x);
    void initLayer(int nodesNBR, int inputsNBR);
    double randRange(int min, int max);
};

#endif // LAYER_H
