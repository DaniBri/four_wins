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
    Fighter opponent1(0);
    Fighter opponent2(1);
    g.initRelations(&w);
    w.initRelations(&g);
    w.show();

    // true to creat ai with given generation number else ai fighting time!!!
    // Note if training is done after training it is possible to make a 2 player mod
    // if ai fighting each other is chosen it is posible to do player agains ai afterwards

    bool training = 1;
    bool player_vs_ai = true;
    int trainings = 1000;
    int fights = 200;
    // train AI else make them fight
    if(training)
    {
        aiManager.playNTournaments(trainings);
    }else
    {
        // load and Fight AI

        opponent1.initRelations(&g);
        w.initRelationAI1(&opponent1);
        opponent1.loadAI("winnerAI");


        opponent2.initRelations(&g);
        w.initRelationAI2(&opponent2);
        opponent2.loadAI("winnerAI");

        // test run to compare opponents
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
