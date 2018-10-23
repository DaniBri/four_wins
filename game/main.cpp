#include "display.h"
#include "gamefield.h"
#include "manager.h"
#include <QApplication>
#include "QDebug"
#include "fighter.h"



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Display w;
    Gamefield g;
    Manager aiManager;
    aiManager.initRelations(&g);
    g.initRelations(&w);
    w.initRelations(&g);
    w.show();

    // true to creat ai with given generation number else ai fighting time!!!
    bool training = 0;
    int trainings = 10;
    int fights = 10000;
    // train AI else make them fight
    if(training)
    {
        aiManager.playNGame(trainings,true);
    }else
    {
        // load and Fight AI
        Fighter opponent1(0);
        opponent1.initRelations(&g);
        w.initRelationAI1(&opponent1);
        opponent1.loadAI("gen10");

        Fighter opponent2(1);
        opponent2.initRelations(&g);
        w.initRelationAI2(&opponent2);
        opponent2.loadAI("gen100k");

        // test run to compare opponents
        int ai1Vic = 0;
        int ai2Vic = 0;

        while(ai1Vic+ai2Vic != fights)
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
