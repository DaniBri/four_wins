#include "nn_ai.h"
#include "gamefield.h"


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


    // get 3 best choices according to NN

    int nbr1 = 0;
    int nbr2 = 1;
    int nbr3 = 2;

    for (int i = 1; i < this->layerOUT->getNodesValues().size(); ++i) {
        if((i != nbr2) && (i != nbr3))
        {
            if(this->layerOUT->getNodesValues().at(i) > this->layerOUT->getNodesValues().at(nbr1))
            {
                nbr1 = i;
            }
        }
    }

    for (int i = 1; i < this->layerOUT->getNodesValues().size(); ++i) {
        if((i != nbr1) && (i != nbr3))
        {
            if(this->layerOUT->getNodesValues().at(i) > this->layerOUT->getNodesValues().at(nbr2))
            {
                nbr2 = i;
            }
        }
    }

    for (int i = 1; i < this->layerOUT->getNodesValues().size(); ++i) {
        if((i != nbr1) && (i != nbr2))
        {
            if(this->layerOUT->getNodesValues().at(i) > this->layerOUT->getNodesValues().at(nbr3))
            {
                nbr3 = i;
            }
        }
    }

    // randomli send one of the 3 best choices
    switch (rand()%3) {
    case 0:
        result = nbr1;
        break;
    case 1:
        result = nbr2;
        break;
    case 2:
        result = nbr3;
        break;
    default:
        break;
    }

    // place at random if 3 fav are full
    if(check3Bussy(nbr1,nbr2,nbr3))
    {
        result = layer1->randRange(0,6);
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
    fullVector.removeLast(); // is just empty line
    file.close();

    // preping vector to cut int slices for different layers
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

    this->layer1->setWeights(vlayer1);
    this->layer2->setWeights(vlayer2);
    this->layerOUT->setWeights(vlayerOUT);
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

bool NN_AI::check3Bussy(int x, int y, int z)
{
    bool result = true;

    if(this->field->getBoardAt(x,5) == 0)
        result = false;
    if(this->field->getBoardAt(y,5) == 0)
        result = false;
    if(this->field->getBoardAt(z,5) == 0)
        result = false;

    return result;
}
