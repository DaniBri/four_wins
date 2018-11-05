#include "manager.h"

Manager::Manager(Gamefield *field)
{
    this->field = field;
    // Seed for random generator
    qsrand(QTime::currentTime().msec());
    initGame();
}

void Manager::playNTournaments(int nbrOfTourns)
{
    // Ready timer
    QElapsedTimer timer;
    timer.start();

    // Play the tournaments and store winner
    for (int i = 0; i < nbrOfTourns; ++i) {
        playTournament();
        qDebug()<< "Tournaments "<< i + 1 <<"/"<<nbrOfTourns<<"done";
    }
    qDebug() << "The tournament took" << timer.elapsed()/1000 << "seconds";

}

void Manager::playGame()
{
    int timeoutCounter = 0;
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

void Manager::playTournament()
{
    // Create the AI in vector that play. 25 AI split in pool of 5. first is clone of original AI. 2-4th are tweaked ones.
    // and 5th AI is completely randomly generated.
    QVector<NN_AI*> tournametsAI = initTournametsAI();

    // In each pool every single AI plays N games against every other one. AI with most wins goes to next stage.
    QVector<NN_AI*> winners;
    for (int i = 0; i < POOL_PER_TOURNAMENT; ++i) {
        winners.append(poolFight(tournametsAI,i*POOL_SIZE));
    }

    // Do one pool out of the winners and let them play.
    // Print out winner of final pool as winner AI.
    poolFight(winners,0)->printNetwork(AI_WINNER_NAME);


    // Delete all the AI's created for tournament
    clearTourAI(tournametsAI);
}



// Start is the position in vector where First AI of a pool is
NN_AI *Manager::poolFight(QVector<NN_AI*> ais, int start)
{
    // Check if enough AI's available for full pool
    if(ais.size() < start+POOL_SIZE){
        qDebug()<<"ERROR 010: Vector of AIs is to small";
        return NULL;
    }

    QVector<NN_AI*> poolAIs;
    // Copy the AIs from pool to new vector, making it easier to work with
    for (int i = 0; i < POOL_SIZE; ++i) {
        poolAIs.append(ais.at(i+start));
    }


    // Make every AI play each other 1 time every single other one and mark winner
    int winnCounter[POOL_SIZE] = {0};

    // Go past full pool setting ai1 to first AI in vector
    for (int i = 0; i < POOL_SIZE; ++i) {
        this->ai1 = poolAIs.at(i);

        // make AI selected from vector play against every single AI stored next in vector
        // to do so store AI in AI2
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
                    // AI shouldn't be punished for draws
                    winnCounter[i]++;
                    winnCounter[j]++;
                    break;
                }
                // Switch who plays first
                aiSwitchSides();
                playGame();
                // Same switch as before with small changes according to switch
                switch (lastWinner) {
                case 1:
                    winnCounter[j]++;
                    break;
                case 2:
                    winnCounter[i]++;
                    break;
                default:
                    // AI shouldn't be punished for draws
                    winnCounter[i]++;
                    winnCounter[j]++;
                    break;
                }
                // Switch AI back into position for next match
                aiSwitchSides();
            }
        }
    }

    // Check counter for winner
    int poolWinner = 0;
    for (int i = 1; i < POOL_SIZE; ++i) {
        if(winnCounter[poolWinner] < winnCounter[i])
        {
            poolWinner = i;
        }
    }

    // Send back winner
    //qDebug()<<"Pool:"<<start/POOL_PER_TOURNAMENT<<" Winner at"<<poolWinner;
    return poolAIs.at(poolWinner);
}

void Manager::aiSwitchSides()
{
    NN_AI* tempAI = this->ai1;
    this->ai1 = this->ai2;
    this->ai2 = tempAI;
}

void Manager::clearTourAI(QVector<NN_AI *> tournametsAI)
{
    // Delete all AIs after tournament and empty ai vector
    while(!tournametsAI.empty())
    {
        delete tournametsAI.first();
        tournametsAI.removeFirst();
    }
}

// Makes AIs play a move on board switching between players.
// AI board or AI needs to check if it is really corresponding turn.
bool Manager::playOneMove()
{
    switch (gameState) {
    case -1:
        //qDebug()<<"Draw";
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
        lastWinner = 1;
        initGame();
        return false;
    case 2:
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


QVector<NN_AI*> Manager::initTournametsAI()
{
    QVector<NN_AI*> result;
    // load some given AI if in folder with winner name
    QFile file1(QDir::currentPath() + "/"+AI_WINNER_NAME+".txt");
    if (!file1.exists()) {
        NN_AI temp(field);
        temp.printNetwork(AI_WINNER_NAME);
    }

    NN_AI* aiPTR;
    for (int i = 0; i < POOL_PER_TOURNAMENT; ++i) {

        // Copy main AI
        aiPTR = new NN_AI(field);
        aiPTR->importNetwork(AI_WINNER_NAME);
        result.append(aiPTR);

        // check if there is a winner from a previous tournament
        QFile file2(QDir::currentPath() + "/" + AI_OLD_WINNER_NAME + ".txt");
        if(!file2.exists()) {
            qDebug()<<"No old Winner existing";

            // Create random AI because first turn
            // Concluded cause no old winner existing
            for (int j = 0; j < POOL_SIZE-1; ++j) {
                aiPTR = new NN_AI(field);
                result.append(aiPTR);
            }
        }else
        {

            // Create N new random child from old gen
            for (int j = 0; j < POOL_RAND_AI; ++j) {
                aiPTR = new NN_AI(field);
                aiPTR->importNetworkPartialy(AI_OLD_WINNER_NAME);
                result.append(aiPTR);
            }

            // Create children
            for (int j = 0; j < POOL_CHILD_AI; ++j) {
                aiPTR = new NN_AI(field);
                aiPTR->creatChild(AI_WINNER_NAME,AI_OLD_WINNER_NAME);
                result.append(aiPTR);
            }

            // Create mutated children
            for (int j = 0; j < POOL_MUTANT_AI; ++j) {
                aiPTR = new NN_AI(field);
                aiPTR->creatChild(AI_WINNER_NAME,AI_OLD_WINNER_NAME);
                aiPTR->tweakNetwork();
                result.append(aiPTR);
            }
        }
    }
    aiPTR = new NN_AI(field);
    aiPTR->importNetwork(AI_WINNER_NAME);
    aiPTR->printNetwork(AI_OLD_WINNER_NAME);
    delete aiPTR;
    return result;
}
