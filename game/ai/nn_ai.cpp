#include "nn_ai.h"
#include "basic/gamefield.h"


NN_AI::NN_AI()
{
    initNetwork();
}

NN_AI::~NN_AI()
{
    while(!this->layerVec.empty())
    {
        delete this->layerVec.first();
        this->layerVec.removeFirst();
    }
}

int NN_AI::calcOut(QVector<double> boardInput)
{
    int result = 0;

    this->layerVec.at(0)->calcValues(boardInput);
    for (int i = 1; i < layerVec.size(); ++i) {
        this->layerVec.at(i)->calcValues(this->layerVec.at(i-1)->getNodesValues());
    }

    // store results from nodes in vector
    QVector<double> nodeResult = this->layerVec.last()->getNodesValues();

    // creat ranking from vector of results
    QVector<int> ranking;
    int temp;
    for (int i = 0; i < nodeResult.size(); ++i) {
        temp = 0;
        // looking for node with higest value
        for (int j = 0; j < nodeResult.size(); ++j) {
            if(nodeResult.at(j) != -2)
            {
                if(nodeResult.at(temp) <= nodeResult.at(j))
                {
                    temp = j;
                }
            }
        }

        // store in ranking
        ranking.append(temp);

        // remove winner from vector
        // -2 can be used thanks to sigmoid
        nodeResult.replace(temp, -2);
    }

    // set result to free place according to ranking
    for (int i = 0; i < NBR_HOLE_HOR; ++i) {
        if(!checkColumnFull(ranking.first()))
        {
            result = ranking.first();
            break;
        }else
        {
            // if column full remove column from ranking
            ranking.removeFirst();
        }
    }

    return result;
}

void NN_AI::tweekNetwork()
{
    for (int i = 0; i < this->layerVec.size(); ++i) {
        this->layerVec.at(i)->tweekLayer();
    }
}

void NN_AI::randomizeNetwork()
{
    for (int i = 0; i < this->layerVec.size(); ++i) {
        this->layerVec.at(i)->randomizeLayer();
    }
}

void NN_AI::printNetwork(QString filename)
{
    QFile file(QDir::currentPath() + "/" + filename + ".txt");
    if(file.exists())
    {
        file.remove();
    }
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        stream << networkToString() << endl;
    }
    file.close();
}

void NN_AI::importNetwork(QString filename)
{
    QFile file(QDir::currentPath() + "/" + filename + ".txt");
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug()<<"No such file in dir";
    }

    QTextStream in(&file);
    QVector<QStringList> fullVector;

    // loop file and store in list
    while(!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(",");
        fullVector.append(fields);
    }
    file.close();
    fullVector.removeLast();
    // preping vector to cut int slices for different layers
    if(fullVector.size() == NN_SIZE_OF_SAVE)// stored data has right dim
    {
        // do all layers
        QVector<QStringList> templayer;
        for (int i = 0; i < this->layerVec.size()-1; ++i) {

            // Get weights
            for (int j = 0; j < NN_LAYER_NODES; ++j) {
                templayer.append(fullVector.at(0));
                fullVector.removeFirst();
            }
            //set weight
            this->layerVec.at(i)->setWeights(templayer);
            templayer.clear();

            // read biases on next line nad set them
            this->layerVec.at(i)->setBiases(fullVector.at(0));
            fullVector.removeFirst();
        }

        // do output layer
        for (int i = 0; i < NBR_HOLE_HOR; ++i) {
            templayer.append(fullVector.at(0));
            fullVector.removeFirst();
        }
        this->layerVec.last()->setBiases(fullVector.at(0));
        this->layerVec.last()->setWeights(templayer);
    }else{
        qDebug()<<"file to load corupted";
    }
}

void NN_AI::creatChild(QString mother, QString father)
{
    this->importNetwork(mother);
    this->importNetworkPartialy(father);
}

void NN_AI::initRelations(Gamefield *field)
{
    this->field = field;
}

void NN_AI::initNetwork()
{
    this->layerVec.append(new Layer(NN_LAYER_NODES,NBR_HOLE_VER*NBR_HOLE_HOR));
    for (int i = 0; i < NN_LAYER_NBR-1; ++i) {
        this->layerVec.append(new Layer(NN_LAYER_NODES,NN_LAYER_NODES));
    }
    this->layerVec.append(new Layer(NBR_HOLE_HOR,NN_LAYER_NODES));
}

QString NN_AI::networkToString()
{
    QString result = "";
    for (int i = 0; i < this->layerVec.size(); ++i) {
        result += this->layerVec.at(i)->getWeightsAndBias();
    }
    return result;
}

bool NN_AI::checkColumnFull(int x)
{
    if(this->field->getBoardAt(x,0) == 0)
        return false;
    return true;
}

void NN_AI::importNetworkPartialy(QString filename)
{
    QFile file(QDir::currentPath() + "/" + filename + ".txt");
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug()<<"No such file in dir";
    }

    QTextStream in(&file);
    QVector<QStringList> fullVector;

    // loop file and store in list
    while(!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(",");
        fullVector.append(fields);
    }
    file.close();
    fullVector.removeLast();
    // preping vector to cut int slices for different layers
    if(fullVector.size() == NN_SIZE_OF_SAVE)// stored data has right dim
    {
        // do all layers
        QVector<QStringList> templayer;
        for (int i = 0; i < this->layerVec.size()-1; ++i) {

            // Get weights
            for (int j = 0; j < NN_LAYER_NODES; ++j) {
                templayer.append(fullVector.at(0));
                fullVector.removeFirst();
            }
            //set weight
            this->layerVec.at(i)->setWeightsPartialy(templayer);
            templayer.clear();

            // read biases on next line nad set them
            this->layerVec.at(i)->setBiasesPartialy(fullVector.at(0));
            fullVector.removeFirst();
        }

        // do output layer
        for (int i = 0; i < NBR_HOLE_HOR; ++i) {
            templayer.append(fullVector.at(0));
            fullVector.removeFirst();
        }
        this->layerVec.last()->setBiasesPartialy(fullVector.at(0));
        this->layerVec.last()->setWeightsPartialy(templayer);
    }else{
        qDebug()<<"file to load corupted";
    }
}
