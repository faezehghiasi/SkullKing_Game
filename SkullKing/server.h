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
#include<QLineEdit>
#include<QMutex>
#include<QLabel>
extern QMutex mx;
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
     bool creation();
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
     void play();
     void caculateScore(int rivalScore);
     void worksForCalculateScore();
     void stopServer();
     void sendScore();
     QLabel*scoreNumber;
     QLabel*clientScore;
     QLabel*serverName;
     QLabel*clientName;
     bool sendName = false;
     void endTheGame(QString res);

    ~Server();
signals:
    void sendIp(QString ip);
    void changePage();
    void finishCard();
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
    void change_geometry();
    void on_resumeButton_clicked();
    void finishCards();

private slots:


    void updateTimer();
    void on_stop_clicked();
    void on_exit_clicked();

private:

    Ui::Server *ui;
    QTcpServer* server;
    QTcpSocket* socket;
    QVector<struct buttons> pushButtons;
    struct buttons server_card;
    struct buttons client_card;
    QLineEdit* guessLabel;
    QPushButton* resume;
    QLabel* roundNumber;
    QLabel *timerLabel;
    QTimer *timer;
    int remainingTime;
    QLabel* clientPic;
    QLabel *serverPic;
    QLabel *crown;
    QLabel *result;
};
extern Server* srv;
#endif // SERVER_H
