#include "nn_ai.h"
#include "basic/gamefield.h"


NN_AI::NN_AI()
{
    initNetwork();
}

int NN_AI::calcOut(QVector<double> boardInput)
{
    int result = 0;

    this->layer1->calcValues(boardInput);
    this->layer2->calcValues(this->layer1->getNodesValues());
    this->layerOUT->calcValues(this->layer2->getNodesValues());


    QVector<int> ranking;


    // store results from nodes in vector
    QVector<double> nodeResult = this->layerOUT->getNodesValues();

    // creat ranking from vector of results
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
    this->layer1->tweekLayer();
    this->layer2->tweekLayer();
    this->layerOUT->tweekLayer();
}

void NN_AI::randomizeNetwork()
{
    this->layer1->randomizeLayer();
    this->layer2->randomizeLayer();
    this->layerOUT->randomizeLayer();
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

    // preping vector to cut int slices for different layers
    if(fullVector.size() >= (2*NN_LAYER_NODES+NBR_HOLE_HOR+3))// stored data has right dim
    {
        QVector<QStringList> vlayer1;
        for (int i = 0; i < NN_LAYER_NODES; ++i) {
            vlayer1.append(fullVector.at(0));
            fullVector.removeFirst();
        }

        // read biases on next line nad set them
        this->layer1->setBiases(fullVector.at(0));
        fullVector.removeFirst();

        //Repeat for each layer
        QVector<QStringList> vlayer2;
        for (int i = 0; i < NN_LAYER_NODES; ++i) {
            vlayer2.append(fullVector.at(0));
            fullVector.removeFirst();
        }

        this->layer2->setBiases(fullVector.at(0));
        fullVector.removeFirst();

        QVector<QStringList> vlayerOUT;
        for (int i = 0; i < NBR_HOLE_HOR; ++i) {
            vlayerOUT.append(fullVector.at(0));
            fullVector.removeFirst();
        }

        this->layerOUT->setBiases(fullVector.at(0));
        fullVector.removeFirst();

        // set weights for each layer
        this->layer1->setWeights(vlayer1);
        this->layer2->setWeights(vlayer2);
        this->layerOUT->setWeights(vlayerOUT);
    }else{
        qDebug()<<"file to load corupted";
    }
}

void NN_AI::initRelations(Gamefield *field)
{
    this->field = field;
}

void NN_AI::initNetwork()
{
    this->layer1 = new Layer(NN_LAYER_NODES,NBR_HOLE_VER*NBR_HOLE_HOR);
    this->layer2 = new Layer(NN_LAYER_NODES,NN_LAYER_NODES);
    this->layerOUT = new Layer(NBR_HOLE_HOR,NN_LAYER_NODES);
}

QString NN_AI::networkToString()
{
    QString result = layer1->getWeightsAndBias();
    result += layer2->getWeightsAndBias();
    result += layerOUT->getWeightsAndBias();
    return result;
}

bool NN_AI::checkColumnFull(int x)
{
    if(this->field->getBoardAt(x,0) == 0)
        return false;
    return true;
}
