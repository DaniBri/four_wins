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
    ~NN_AI();
    int calcOut(QVector<double> boardInput);          // send out answer
    void tweekNetwork();    // tweek Values of Network
    void randomizeNetwork(); // randomize Values of Network
    void printNetwork(QString filename);    // exporting Netwrok
    void importNetwork(QString filename);     // importing Network
    void creatChild(QString mother,QString father);
    void initRelations(Gamefield* field);

private:
    QVector<Layer*> layerVec;
    Gamefield* field;
    void initNetwork();     // randomize Value of Network
    QString networkToString();
    bool checkColumnFull(int x);
    void importNetworkPartialy(QString filename);

};

#endif // NN_AI_H
