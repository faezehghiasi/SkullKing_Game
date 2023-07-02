#ifndef CLIENT_H
#define CLIENT_H
#include"cards.h"
#include <QMainWindow>
#include <QObject>
#include<QTcpSocket>
#include <QAbstractSocket>
#include<QLineEdit>
#include<QMessageBox>
#include"player.h"
#include<QPushButton>
extern QString Ip;
namespace Ui {
class Client;
}

class Client : public QMainWindow
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = nullptr);
    void creation();
    ~Client();
    void showCards(QList<cards>Ccards);
     void availbleCards(buttons rivalCard);
    void calculate(cards server_card,cards client_card);
    void set_picture(struct buttons crd);
    void set_client_card(struct buttons cCard);
    void set_server_card(struct buttons sCard);
    QVector<buttons> get_PushPuttons();
    struct buttons get_client_card();
    struct buttons get_server_card();
    void move_oneCards(buttons& c);
    void move_twoCards();
    void calculateScore();
    QLabel *scoreLabel;
    QLabel*scoreNumber;
signals:
    void existClient();
    void startTheGame();
public slots:
    void connected();
    void disconnected();
    void readyRead();
    void on_Buttons0_clicked();
    void on_Buttons1_clicked();
    void on_Buttons2_clicked();
    void on_Buttons3_clicked();
    void on_Buttons4_clicked();
    void on_Buttons5_clicked();
    void on_Buttons6_clicked();
    void on_Buttons7_clicked();
    void on_Buttons8_clicked();
    void on_Buttons9_clicked();
    void on_Buttons10_clicked();
    void on_Buttons11_clicked();
    void on_Buttons12_clicked();
    void on_Buttons13_clicked();
private slots:
    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

private:
    Ui::Client *ui;
    QTcpSocket* socket;
    QVector<struct buttons> pushButtons;
    QString message;
    struct buttons server_card;
    struct buttons client_card;
    QLineEdit* guessLabel;
    QPushButton * continueTheGameButton;
};
extern Client* cln;

#endif // CLIENT_H
