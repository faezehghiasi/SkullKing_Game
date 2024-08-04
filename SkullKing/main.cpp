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


    QApplication a(argc, argv);
   a.setStyleSheet(
       "QMessageBox {"
       "   background-color: #f4b26d;"
       "   color: #4a270e;" // Dark text color
       "   font-family: 'Comic Sans MS';"
       "   font-size: 14px;"
       "}"
       "QMessageBox QPushButton {"
       "   background-color: #ebbc7c;"
       "   color: #2c3e50;" // Dark text color on buttons
       "   border: 2px solid #2c3e50;" // Dark border
       "   padding: 5px 10px;"
       "   font-size: 14px;"
       "}"
       "QMessageBox QPushButton:hover {"
       "   background-color: #ed9c1a;" // Slightly darker green on hover
       "}"
       "QMessageBox QPushButton:pressed {"
       "   background-color: #f6b66b;" // Even darker green when pressed
       "}"
       );
    QString filename= "myfile.bin";
    readFromFile(filename);
    Skullking w;
    w.show();
    Skullking::delay();
    w.Show_TextBrows();
    return a.exec();
}
