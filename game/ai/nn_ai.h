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
    NN_AI(Gamefield* field);
    ~NN_AI();
    int calcOut(QVector<double> boardInput);        // Send out answer
    void tweakNetwork();                            // Tweak Values of Network
    void printNetwork(QString filename);            // Exporting Network
    void importNetwork(QString filename);           // Importing Network
    void creatChild(QString mother,QString father);
    void importNetworkPartialy(QString filename);

private:
    QVector<Layer*> layerVec;
    Gamefield* field;
    void initNetwork();                             // Initialized with randomize Value for layers
    QString networkToString();
    bool checkColumnFull(int x);


};

#endif // NN_AI_H
