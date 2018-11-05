#include "display.h"
#include "gamefield.h"
#include "helpers/fighter.h"

#define HOLE_SIZE 32
#define DISPLAY_WIDTH 320
#define DISPLAY_HEIGHT 240

#define OFFSET_HOR 0.5*(DISPLAY_WIDTH/NBR_HOLE_HOR-HOLE_SIZE)
#define OFFSET_VER 0.5*(DISPLAY_HEIGHT/NBR_HOLE_VER-HOLE_SIZE)

Display::Display(QWidget *parent) :
    QWidget(parent)
{
    // Background colour palette
    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::black);

    // Window set-up
    this->setFixedHeight(DISPLAY_HEIGHT);
    this->setFixedWidth(DISPLAY_WIDTH);
    this->setWindowTitle("Four Wins");
    this->setAutoFillBackground(true);
    this->setPalette(pal);
}

Display::~Display()
{
}

void Display::paintToken(int column, int row, QColor color)
{
    // Drawing basic lines of board
    QPainter* painter = new QPainter(this);

    // How to get filled ellipse
    painter->setBrush(color);

    // Painting circle
    painter->drawEllipse(column*DISPLAY_WIDTH/NBR_HOLE_HOR+OFFSET_HOR,row*DISPLAY_HEIGHT/NBR_HOLE_VER+OFFSET_VER,HOLE_SIZE,HOLE_SIZE);
    delete painter;
}

void Display::initRelations(Gamefield *field)
{
    this->field=field;
}

void Display::initRelationAI1(Fighter *someAI)
{
    this->opponent1 = someAI;
}

void Display::initRelationAI2(Fighter *someAI)
{
    this->opponent2 = someAI;
}

// 0 = draw/ongoing, 1=p1 win, 2 = p2 win
int Display::simClick()
{
    int winner = 0;
    if(this->opponent1 != NULL){
        winner = this->opponent1->aiPlayOnce();
    }
    if(this->opponent2 != NULL){
        if(winner == 0)
            winner = this->opponent2->aiPlayOnce();
    }
    return winner;
}

void Display::drawBoard()
{
    // Drawing basic lines of board
    QPainter* painter = new QPainter(this);

    // How to get filled ellipse
    painter->setBrush(Qt::white);

    // Draw holes according to defined parameters in definer file
    for (int i = 0; i < NBR_HOLE_HOR; ++i) {
        for (int j = 0; j < NBR_HOLE_VER; ++j) {
            if(this->field != NULL){
                switch (this->field->getBoardAt(i,j)) {
                case 0:
                    paintToken(i,j,Qt::white);
                    break;
                case 1:
                    paintToken(i,j,Qt::yellow);
                    break;
                case -1:
                    paintToken(i,j,Qt::red);
                    break;
                default:
                    qDebug()<<"ERROR 001: No valid data on board";
                    break;
                }
            }else
            {
                qDebug()<<"ERROR 003: No gamefield known";
            }
        }
    }
    delete painter;
}

void Display::paintEvent(QPaintEvent*)
{
    drawBoard();
}

void Display::mouseReleaseEvent(QMouseEvent *e)
{
    int playerBefore = this->field->getState();
	
    // Managing what AI to play on click
    if(this->opponent1 != NULL){
        this->opponent1->aiPlayOnce();
        if(playerBefore != this->field->getState())
            return;
    }

    // If AI is missing user can play
    int column = round(e->x() * NBR_HOLE_HOR/DISPLAY_WIDTH);
    this->field->action(column);
}
