#ifndef NN_AI_H
#define NN_AI_H
#include "globaldefiner.h"
#include "QVector"
#include "QString"
#include "layer.h"
#include <algorithm>
#include <QFile>
#include <QTextStream>
#include <QDir>

class Gamefield;
class NN_AI
{
public:
    NN_AI();
    int calcOut(QVector<double> boardInput);          // send out answer
    void tweekNetwork();    // tweek Values of Network
    void randomizeNetwork(); // randomize Values of Network
    void printNetwork(QString filename);    // exporting Netwrok
    void importNetwork(QString filename);     // importing Network
    void initRelations(Gamefield* field);

private:
    Layer* layer1;
    Layer* layer2;
    Layer* layerOUT;
    Gamefield* field;
    void initNetwork();     // randomize Value of Network
    QString networkToString();
    bool check3Bussy(int x, int y, int z);

};

#endif // NN_AI_H
