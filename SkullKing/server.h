#ifndef SERVER_H
#define SERVER_H
#include<QTcpSocket>
#include<QTcpServer>
#include <QMainWindow>
#include<QPushButton>
#include<QNetworkInterface>
#include"cards.h"
#include<QPushButton>
#include"player.h"
#include <QPropertyAnimation>
namespace Ui {
class Server;
}

class Server : public QMainWindow
{
    Q_OBJECT

public:
     QString serverIp;
     friend class ClientOrServer;
    explicit Server(QWidget *parent = nullptr);
     void creation();
     void play(int cturn);
     QTcpSocket* get_socket();
     void showCards(QList<cards> cCards);
     void whoShouldStartTheGameFirst();
     void set_picture(struct buttons crd);
     void availbleCards(buttons rivalCard);
     void set_client_card(struct buttons cCard);
     void set_server_card(struct buttons sCard);
     struct buttons& get_client_card();
     struct buttons& get_server_card();
     QVector<buttons>& get_buttons();
     void move_oneCards(buttons& c);
     void move_twoCards();
    ~Server();
signals:
    void sendIp(QString ip);
    void changePage();
public slots:
    void readyRead();
    void newConnection();
    void disconnected();
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




private:
    Ui::Server *ui;
    QTcpServer* server;
    QTcpSocket* socket;
    QVector<struct buttons> pushButtons;
    struct buttons server_card;
    struct buttons client_card;

};
extern Server* srv;
#endif // SERVER_H
