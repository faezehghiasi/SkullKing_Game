#ifndef SIGNUP_H
#define SIGNUP_H
#include"player.h"

#include <QMainWindow>
#include<QVector>
#include<QDebug>
#include<QFile>
#include<QString>
#include<QTextStream>
#include<QMessageBox>
extern QVector<Player> listOfPlayer;
//overload operator for QTextStream
QTextStream &operator<<(QTextStream& out ,Player p);
QTextStream& operator>>(QTextStream& in , Player& p);

//read from file function
void readFromFile(QString filename);
//write to file
void writeToFile(QString filename);


namespace Ui {
class SignUp;
}

class SignUp : public QMainWindow
{
 Q_OBJECT

public:
    explicit SignUp(QWidget *parent = nullptr);
    ~SignUp();

private slots:
    void on_singup_push_clicked();

    void on_backButton_clicked();

private:
    Ui::SignUp *ui;

};

#endif // SIGNUP_H
