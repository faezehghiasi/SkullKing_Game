#ifndef SIGNIN_H
#define SIGNIN_H
#include "forgetpassword.h"
#include"player.h"
#include <QMainWindow>
extern Player currentPlayer;
namespace Ui {
class SignIn;
}

class SignIn : public QMainWindow
{
    Q_OBJECT

public:
    explicit SignIn(QWidget *parent = nullptr);
    ~SignIn();

private slots:
    void on_ButtonLogIn_clicked();

    void on_ButtonForgetPassword_clicked();

private:
    Ui::SignIn *ui;
    forgetPassword * forgetP;
};

#endif // SIGNIN_H
