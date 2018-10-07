#include "display.h"
#include "QPainter"
#include "globaldefiner.h"
#include "QDebug"
#include "QMouseEvent"
#include "gamefield.h"
#include <cmath>

#define HOLE_SIZE 32
#define DISPLAY_WIDTH 320
#define DISPLAY_HEIGHT 240

#define OFFSET_HOR 0.5*(DISPLAY_WIDTH/NBR_HOLE_HOR-HOLE_SIZE)
#define OFFSET_VER 0.5*(DISPLAY_HEIGHT/NBR_HOLE_VER-HOLE_SIZE)

Display::Display(QWidget *parent) :
    QWidget(parent)
{
    //Background color palett
    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::black);

    //Window setup
    this->setFixedHeight(DISPLAY_HEIGHT);
    this->setFixedWidth(DISPLAY_WIDTH);
    this->setWindowTitle("Four Wins");
    this->setAutoFillBackground(true);
    this->setPalette(pal);

    //player starting
    this->player = 1;

}

Display::~Display()
{
}

void Display::paintToken(int column, int row, QColor color)
{
    // drawing basic lines of board
    QPainter* painter = new QPainter(this);

    // how to get filled ellipse
    painter->setBrush(color);

    // painting circle
    painter->drawEllipse(column*DISPLAY_WIDTH/NBR_HOLE_HOR+OFFSET_HOR,row*DISPLAY_HEIGHT/NBR_HOLE_VER+OFFSET_VER,HOLE_SIZE,HOLE_SIZE);
}

void Display::initRelations(Gamefield *field)
{
    this->field=field;
}

void Display::drawBoard()
{
    // drawing basic lines of board
    QPainter* painter = new QPainter(this);

    // how to get filled ellipse
    painter->setBrush(Qt::white);

    // draw holes 7x6
    for (int i = 0; i < NBR_HOLE_HOR; ++i) {
        for (int j = 0; j < NBR_HOLE_VER; ++j) {
            switch (this->field->getBoardAt(i,j)) {
            case 0:
                paintToken(i,j,Qt::white);
                break;
            case 1:
                paintToken(i,j,Qt::yellow);
                break;
            case 2:
                paintToken(i,j,Qt::red);
                break;
            default:
                qDebug()<<"ERROR 001: No Valide data on Board";
                break;
            }
        }
    }
    delete painter;
}

void Display::paintEvent(QPaintEvent*)
{
    qDebug()<<"drawing";
    drawBoard();
}

void Display::mouseReleaseEvent(QMouseEvent *e)
{
    qDebug()<<"click at: "<<e->x();
    int column = round(e->x()*NBR_HOLE_HOR/DISPLAY_WIDTH);
    this->field->placeToken(column,player);
    if(this->player == 1)
        player++;
    else
        player--;
}