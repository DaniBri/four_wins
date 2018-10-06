#include "widget.h"
#include "ui_widget.h"
#include "QPainter"

#define NBR_HOLE_HOR 7
#define NBR_HOLE_VER 6
#define HOLE_SIZE 32
#define DISPLAY_WIDTH 320
#define DISPLAY_HEIGHT 240
Widget::Widget(QWidget *parent) :
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


}

Widget::~Widget()
{
}

void Widget::drawBoard()
{
    // drawing basic lines of board
    QPainter* painter = new QPainter(this);

    // how to get filled ellipse
    painter->setBrush(Qt::white);

    // create offset to border
    int offset_hor = 0.5*(DISPLAY_WIDTH/NBR_HOLE_HOR-HOLE_SIZE);
    int offset_ver = 0.5*(DISPLAY_HEIGHT/NBR_HOLE_VER-HOLE_SIZE);

    // draw holes 7x6
    for (int i = 0; i < NBR_HOLE_VER; ++i) {
        for (int j = 0; j < NBR_HOLE_HOR; ++j) {
            painter->drawEllipse(j*DISPLAY_WIDTH/NBR_HOLE_HOR+offset_hor,i*DISPLAY_HEIGHT/NBR_HOLE_VER+offset_ver,HOLE_SIZE,HOLE_SIZE);
        }
    }

    delete painter;
}

void Widget::paintEvent(QPaintEvent*)
{
    drawBoard();
}
