#ifndef WIDGET_H
#define WIDGET_H
#include "globaldefiner.h"
#include "QDebug"
#include "QMouseEvent"
#include <QWidget>
#include "QPainter"
#include "QColor"
#include <cmath>

class Gamefield;
class Fighter;
class Display : public QWidget
{
    Q_OBJECT

public:
    explicit Display(QWidget *parent = 0);
    ~Display();
    void initRelations(Gamefield* field);
    void initRelationAI1(Fighter* someAI);
    void initRelationAI2(Fighter* someAI);
    int simClick(); //function simulating click on screen to macke ai play
private:
    void drawBoard();
    void paintToken(int column, int row, QColor color);
    Gamefield* field;
    Fighter* opponent1; // opponent for player in case
    Fighter* opponent2; // opponent for player in case
protected:
    void paintEvent(QPaintEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
};

#endif // WIDGET_H
