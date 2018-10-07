#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "QColor"

class Gamefield;
class Display : public QWidget
{
    Q_OBJECT

public:
    explicit Display(QWidget *parent = 0);
    ~Display();

    void initRelations(Gamefield* field);
private:
    void drawBoard();
    void paintToken(int column, int row, QColor color);
    Gamefield* field;
    int player;
protected:
    void paintEvent(QPaintEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
};

#endif // WIDGET_H
