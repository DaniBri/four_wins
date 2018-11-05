#include "basic/display.h"
#include "basic/gamefield.h"
#include "helpers/manager.h"
#include <QApplication>
#include "QDebug"
#include "helpers/fighter.h"



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Display w;
    Gamefield g;
    Manager aiManager(&g);
    Fighter opponent1(0,&g);
    Fighter opponent2(1,&g);
    g.initRelations(&w);
    w.initRelations(&g);
    w.show();

    // True to create AI with given generation number else AI fighting time!!!
    // Note if training is done after training it is possible to make a 2 player mod
    // If AI fighting each other is chosen it is possible to do player against AI afterwards

    bool training = TRAINING;
    bool player_vs_ai = true;
    int tournaments = NBR_TOURN;
    int fights = 200;
    // train AI else make them fight
    if(training)
    {
        aiManager.playNTournaments(tournaments);
    }else
    {
        // Load and Fight AI
        w.initRelationAI1(&opponent1);
        opponent1.loadAI("winnerAI");

        w.initRelationAI2(&opponent2);
        opponent2.loadAI("winnerAI");

        // Test run to compare opponents
        int ai1Vic = 0;
        int ai2Vic = 0;

        while(ai1Vic+ai2Vic != fights && !player_vs_ai)
        {
            switch (w.simClick()) {
            case 1:
                ai1Vic++;
                break;
            case 2:
                ai2Vic++;
                break;
            default:
                break;
            }
        }

        qDebug()<<"AI1 Won:"<<ai1Vic*100/fights<<"%";
        qDebug()<<"AI2 Won:"<<ai2Vic*100/fights<<"%";
    }
    return a.exec();
}
