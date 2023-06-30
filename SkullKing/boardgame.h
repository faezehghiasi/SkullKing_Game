#ifndef BOARDGAME_H
#define BOARDGAME_H

#include <QMainWindow>

namespace Ui {
class boardGame;
}

class boardGame : public QMainWindow
{
    Q_OBJECT

public:
    explicit boardGame(QWidget *parent = nullptr);
    ~boardGame();


private:
    Ui::boardGame *ui;
    int numberOfTurn;
};

#endif // BOARDGAME_H
