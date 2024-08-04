#ifndef SKULLKING_H
#define SKULLKING_H

#include <QMainWindow>
#include"signin.h"
#include"signup.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Skullking; }
QT_END_NAMESPACE

class Skullking : public QMainWindow
{
    Q_OBJECT

public:
    Skullking(QWidget *parent = nullptr);
    void Show_TextBrows();
   static void delay();
    ~Skullking();

private slots:
   void on_signIn_clicked();

   void on_SignUp_clicked();

   private:
    Ui::Skullking *ui;
    SignIn* signIn;
    SignUp* signUp;
};
#endif // SKULLKING_H
