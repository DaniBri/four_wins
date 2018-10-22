#ifndef NN_AI_H
#define NN_AI_H
#include "globaldefiner.h"
#include "QVector"
#include "layer.h"
#include <algorithm>

class Gamefield;
class NN_AI
{
public:
    NN_AI();
    int calcOut(QVector<double> boardInput);          // send out answer
    void tweekNetwork();    // tweek Values of Nework
    void printNetwork();    // exporting Netwrok
    void readNetwork();     // importing Network


private:
    Layer* layer1;
    Layer* layer2;
    Layer* layerOUT;
    void initNetwork();     // randomize Value of Network

};

#endif // NN_AI_H
