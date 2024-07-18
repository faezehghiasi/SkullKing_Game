#ifndef CLIENTORSERVER_H
#define CLIENTORSERVER_H
#include<QLabel>
#include <QMainWindow>
#include<QPushButton>
#include<QLineEdit>
#include"server.h"
#include"client.h"
#include"signin.h"
#include"menu.h"
namespace Ui {
class ClientOrServer;
}

class ClientOrServer : public QMainWindow
{
    Q_OBJECT

public:
    static QLabel* ip_label;
    static QLabel* server_label;
    friend class Server;
    friend class Client;
    explicit ClientOrServer(QWidget *parent = nullptr);
    static void delay(int);
    ~ClientOrServer();
public slots:
    void connect_button_clicked();
    void chanePage();
    void closePage();
private:
    Ui::ClientOrServer *ui;
    QPushButton* connect_button;
    QLineEdit* Ip_line;
    menu * menu;
    QLabel* client_label;
private slots:
     void on_serverButton_clicked();
     void on_clientButton_clicked();
};

#endif // CLIENTORSERVER_H
