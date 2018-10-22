#include "display.h"
#include "gamefield.h"
#include "manager.h"
#include <QApplication>



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

    aiManager.ai_place_token(1);

    return a.exec();
}
