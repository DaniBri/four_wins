#include "widget.h"
#include "ui_widget.h"
#include "QPainter"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    //Background color palett
    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::green);

    //Window setup
    ui->setupUi(this);
    this->setFixedHeight(240);
    this->setFixedWidth(320);
    this->setWindowTitle("Four Wins");
    this->setAutoFillBackground(true);
    this->setPalette(pal);


}

Widget::~Widget()
{
    delete ui;
}

void paintEvent(QPaintEvent*)
{
}

void Widget::drawBoard()
{

}
