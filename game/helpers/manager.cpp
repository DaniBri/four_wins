#include "manager.h"

Manager::Manager(Gamefield *field)
{
    this->field = field;
    winnerAI = NULL;
    // seed for randomizer generator
    qsrand(QTime::currentTime().msec());
    initGame();
}

void Manager::playNTournaments(int nbrOfTourns)
{
    // Prepare ai for generation creation
    // Check if there is a AI already in path to load from
    if(winnerAI == NULL) // mean it's gen1
    {
        winnerAI = new NN_AI();
    }

    // load some given ai if in folder with winner name
    QFile file(QDir::currentPath() + "/"+AI_WINNER_NAME+".txt");
    if (file.exists()) {
        winnerAI->importNetwork(AI_WINNER_NAME);
    }

    //ready timer and play
    QElapsedTimer timer;
    timer.start();

    //play the tournaments and store winner
    for (int i = 0; i < nbrOfTourns; ++i) {
        winnerAI = playTournament(this->winnerAI);
        qDebug()<< "Tournaments "<< i + 1 <<"/"<<nbrOfTourns<<"done";
    }
    qDebug() << "The tournamet took" << timer.elapsed()/1000 << "seconds";

    // save winner
    winnerAI->printNetwork(AI_WINNER_NAME);
    delete winnerAI;
}

void Manager::playGame()
{
    int timeoutCounter = 0;
    showFieldToAI();
    while(playOneMove())
    {
        timeoutCounter++;
        if(timeoutCounter == MOVES_TIMEOUT)
        {
            qDebug()<<"Game Timed out";
            field->resetField();
            break;
        }
    }
}

NN_AI* Manager::playTournament(NN_AI* templateAI)
{
    // Creat the ai in vector that play. 25 ai split in pool of 5. first is clone of original ai. 2-4th are tweeked ones.
    // and 5th ai is complitly randomly generated.
    initTournametsAI(templateAI);
    delete templateAI; // not needed anymore

    // in each pool every single ai playes 5 games against every other one. AI with most winns goes to next stage
    QVector<NN_AI*> winners;
    for (int i = 0; i < POOL_PER_TOURNAMENT; ++i) {
        winners.append(poolFight(this->tournametsAI,i*POOL_SIZE));
    }

    // do one pool out of the winners and let them play.
    // winner of this pool is tournament winner and this AI will be stored
    NN_AI* tournamentWinner = new NN_AI();
    poolFight(winners,0)->printNetwork(AI_WINNER_NAME);
    tournamentWinner->importNetwork(AI_WINNER_NAME);

    //delet all the AI's created for tournament
    clearTourAI();

    return tournamentWinner;
}

void Manager::showFieldToAI()
{
    this->ai1->initRelations(field);
    this->ai2->initRelations(field);
}

// start is the position in vector where First ai of a pool is
// Pool size min is defined in globaldefiner
NN_AI *Manager::poolFight(QVector<NN_AI*> ais, int start)
{
    // check if enouth ai's avaible for full pool
    if(ais.size() < start+POOL_SIZE){
        qDebug()<<"ERROR 010: Vector of AIs is to small";
        return NULL;
    }

    QVector<NN_AI*> poolAIs;
    // copie the ais from pool to new vector, making it easier to work with
    // for example if init vector hold all tournament players
    for (int i = 0; i < POOL_SIZE; ++i) {
        poolAIs.append(ais.at(i+start));
    }


    // make ais play each other 1 time every single one of them and mark winner
    int winnCounter[POOL_SIZE] = {0};

    // go pst full pool setting ai1 to first ai in vector
    for (int i = 0; i < POOL_SIZE; ++i) {
        this->ai1 = poolAIs.at(i);

        // make ai selected from vector play against every single ai sored next in vector
        // to do so store ai in AI2
        for (int j = i+1; j < POOL_SIZE; ++j) {
            this->ai2 = poolAIs.at(j);

            for (int x = 0; x < MATCHES_PER_AI_IN_POOL; ++x) {
                playGame();
                switch (lastWinner) {
                case 1:
                    winnCounter[i]++;
                    break;
                case 2:
                    winnCounter[j]++;
                    break;
                default:
                    // ai shouldn't be punished for draws
                    winnCounter[i]++;
                    winnCounter[j]++;
                    break;
                }
                //switch who playes wird
                aiSwitchSides();
                playGame();
                // same switch as befor with small changes according to switch
                switch (lastWinner) {
                case 1:
                    winnCounter[j]++;
                    break;
                case 2:
                    winnCounter[i]++;
                    break;
                default:
                    // ai shouldn't be punished for draws
                    winnCounter[i]++;
                    winnCounter[j]++;
                    break;
                }
                //switch ai back into position for next match
                aiSwitchSides();
            }
        }
    }

    //check counter for winner
    int poolWinner = 0;
    for (int i = 1; i < POOL_SIZE; ++i) {
        if(winnCounter[poolWinner] < winnCounter[i])
        {
            poolWinner = i;
        }
    }

    //send back winner
    qDebug()<<"pool Winner at"<<poolWinner;
    return poolAIs.at(poolWinner);
}

void Manager::aiSwitchSides()
{
    NN_AI* tempAI = this->ai1;
    this->ai1 = this->ai2;
    this->ai2 = tempAI;
}

void Manager::clearTourAI()
{
    // delete all AIs after tournament and empty ai vector
    while(!this->tournametsAI.empty())
    {
        delete this->tournametsAI.first();
        this->tournametsAI.removeFirst();
    }
}

// makes AIs play a move on board switching between players.
// ai board or ai needs to check if it realy theyr turn.
bool Manager::playOneMove()
{
    switch (gameState) {
    case -1:
        qDebug()<<"Draw";
        lastWinner = 0;
        initGame();
        return false;
    case 0:
        if(player)
        {
            gameState = ai_place_token(1);
        }else
        {
            gameState = ai_place_token(2);
        }
        player = !player;
        return true;
    case 1:
        ai1->printNetwork(AI_WINNER_NAME);
        lastWinner = 1;
        initGame();
        return false;
    case 2:
        ai2->printNetwork(AI_WINNER_NAME);
        lastWinner = 2;
        initGame();
        return false;
    default:
        qDebug()<<"ERROR unknown gamestate";
        initGame();
        return false;
    }
}

// -1 = draw, 0 = game ongoing, 1 = p1 won, 2 = p2 won
int Manager::ai_place_token(int i)
{
    int column_to_play;
    if(i == 1) {
        column_to_play = ai1->calcOut(field->getInputs());
        return field->action(column_to_play);
    }
    if(i == 2) {
        column_to_play =  ai2->calcOut(field->getInputs());
        return field->action(column_to_play);
    }
    return -1; //ERROR OR DRAW
}

void Manager::initGame()
{
    player = true;
    gameState = 0;
}


void Manager::initTournametsAI(NN_AI* model)
{
    model->printNetwork(AI_WINNER_NAME);

    NN_AI* aiPTR;
    this->tournametsAI.clear();
    for (int i = 0; i < POOL_PER_TOURNAMENT; ++i) {

        // copie main AI
        aiPTR = new NN_AI();
        aiPTR->importNetwork(AI_WINNER_NAME);
        this->tournametsAI.append(aiPTR);

        // creat 1 new random
        aiPTR = new NN_AI();
        this->tournametsAI.append(aiPTR);

        //creat variants
        for (int j = 0; j < POOL_SIZE-2; ++j) {
            aiPTR = new NN_AI();
            aiPTR->importNetwork(AI_WINNER_NAME);
            aiPTR->tweekNetwork();
            this->tournametsAI.append(aiPTR);
        }


    }
}