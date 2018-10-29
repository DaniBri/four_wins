#ifndef LAYER_H
#define LAYER_H
#include "QVector"
#include "QTime"
#include "QDebug"
#include "QString"

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
    ~ Layer();
    void calcValues(QVector<double> inputs);      // calculate all values of the nodes according to theyr weights and biases
    void tweekLayer();
    void randomizeLayer();
    QVector<double> getNodesValues();
    QString getWeightsAndBias();
    void setWeights(QVector<QStringList> strings);
    void setBiases(QStringList strings);
    void setWeightsPartialy(QVector<QStringList> strings);
    void setBiasesPartialy(QStringList strings);
    double randRange(int min, int max);

private:
    QVector<Nodes*> nodes;
    double calcInputWeights(QVector<double> inputs, int currentNode);
    double fastSigmoid(double x);
    void initLayer(int nodesNBR, int inputsNBR);
    double tweekValue();
};

#endif // LAYER_H
