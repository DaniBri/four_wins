#include "display.h"
#include "gamefield.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Display w;
    Gamefield g;
    g.initRelations(&w);
    w.initRelations(&g);
    w.show();

    return a.exec();
}
