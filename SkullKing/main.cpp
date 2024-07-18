#include "skullking.h"
//#include"signin.h"
//#include"signup.h"
#include"player.h"
#include <QApplication>
#include<QTime>
#include"clientorserver.h"
#include"server.h"
QLabel* ClientOrServer::ip_label;
QLabel* ClientOrServer::server_label;
int main(int argc, char *argv[])
{

   QString filename= "myfile.bin";
   readFromFile(filename);
    QApplication a(argc, argv);
    Skullking w;
    w.show();
    Skullking::delay();
    w.Show_TextBrows();
    return a.exec();
}
