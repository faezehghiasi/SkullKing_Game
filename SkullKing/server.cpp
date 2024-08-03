#include"menu.h"
#include "server.h"
#include "ui_server.h"
#include<QDebug>
#include"clientorserver.h"
#include<QMessageBox>
#include<QDataStream>
#include<QFile>
#include<QRandomGenerator>
#include<QTime>
#include"signup.h"
#include"player.h"
#include<math.h>
#include<QMutex>
#include<QThread>
#include<QString>
#include <QMovie>
#include<QVBoxLayout>
#include<QTimer>
QMutex mx;
QMutex mxForRead;
Server * srv;

Server::Server(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Server)
{
    ui->setupUi(this);
    ui->stopGost->hide();
    ui->gameStoped->hide();

    server = new QTcpServer;
    connect(this,&Server::finishCard,this,&Server::finishCards);

    server_card.cards_button = new QPushButton(this);
    server_card.cards_button->setGeometry(140,230,181,251);
    client_card.cards_button = new QPushButton(this);
    client_card.cards_button->setGeometry(660,230,181,251);
    server_card.cards_button->hide();
    client_card.cards_button->hide();


    scoreNumber= new QLabel(this);
    scoreNumber->setStyleSheet("font: 20pt Broadway;  color: rgb(13, 13, 13);");
    scoreNumber->setGeometry(180,85,141,41);
    scoreNumber->setText(QString::number(currentPlayer.get_score()));
    scoreNumber->show();


    serverName= new QLabel(this);
    serverName->setStyleSheet("font: 18pt Broadway;  color: rgb(13, 13, 13);");
    serverName->setGeometry(120,30,141,41);
    serverName->setText(currentPlayer.get_name());
    serverName->show();

    clientScore = new QLabel(this);
    clientScore->setStyleSheet("font: 20pt Broadway;  color: rgb(13, 13, 13);");
    clientScore->setGeometry(710,110,121,41);
    clientScore->setAlignment(Qt::AlignRight);
    clientScore->setText("0");
    clientScore->show();



    clientName= new QLabel(this);
    clientName->setStyleSheet("font: 18pt Broadway;  color: rgb(13, 13, 13);");
    clientName->setGeometry(650,40,252,51);
    clientName->setAlignment(Qt::AlignRight);
    clientName->show();

    roundNumber = new QLabel(this);
    roundNumber->setAlignment(Qt::AlignCenter);
    roundNumber ->setStyleSheet(
        "QLabel {"
        "   background: transparent;" // Transparent background
        "   color: #bb5a3f;" // Vibrant pink color for the text
        "   font-family: 'Comic Sans MS', cursive, sans-serif;"
        "   font-size: 30px;"
        "   font-weight: bold;"
        "   border: none;" // No border
        "   border-radius: 0px;" // No border radius
        "   padding: 20px;"
        "   text-align: center;"
        "   box-shadow: 0 8px 16px rgba(0, 0, 0, 0.5), 0 16px 32px rgba(0, 0, 0, 0.3);" // Enhanced 3D shadow effect
        "   text-shadow: 1px 1px 4px rgba(0, 0, 0, 0.6), 0 0 25px rgba(255, 105, 180, 0.7);" // Multi-colored 3D text shadow
        "}"
     );
    roundNumber->setGeometry(330,15,351,91);
    roundNumber->hide();

    timerLabel = new QLabel(this);
    timerLabel->setAlignment(Qt::AlignCenter);
    timerLabel->setStyleSheet(
        "QLabel {"
        "   color: #ffffff;"
        "   font-family: 'Comic Sans MS';"
        "   font-size: 75px;"
        "   padding: 30px;"
        "   background-color: rgba(0, 0, 0, 0);"
        "}"
        );
    timerLabel->setGeometry(400, 65, 171, 181);
    timerLabel->hide();
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Server::updateTimer);



    resume = new QPushButton(this);
    resume->setStyleSheet("border-image: url(:/Prefix/resource/Firefly_play_icon_is_circular_in_green_color_in_3D_35008-removebg-preview.png);background-color: rgba(0,0,0,0);border-radius:25px;");
    resume->setGeometry(920,270,71,71);
    resume->setEnabled(false);
    resume->hide();
    connect(resume,SIGNAL(clicked()),this,SLOT(on_resumeButton_clicked()));


    clientPic = new QLabel(this);
    clientPic->setStyleSheet("border-image: url(:/Prefix/resource/Firefly_cute_skeleton_icon_for_game_in_3D_56539-removebg-preview.png);");
    clientPic->setGeometry(740,300,211,301);
    clientPic->hide();

    serverPic = new QLabel(this);
    serverPic->setStyleSheet("border-image: url(:/Prefix/resource/Firefly_cute_skeleton_icon_for_game_in_3D_78259-removebg-preview.png);");
    serverPic->setGeometry(80,320,211,261);
    serverPic->hide();


    crown = new QLabel(this);
    crown->setStyleSheet("border-image: url(:/Prefix/resource/crown.png);");
    crown->hide();

    result = new QLabel(this);
    result->setStyleSheet("font:700 90pt Curlz MT;color: rgb(111, 56, 0);");
    result->setGeometry(270,340,551,171);
    result->setAlignment(Qt::AlignCenter);
    result->hide();

    returnButton=new QPushButton("Back to menu",this);
    returnButton->setGeometry(0,660,231,41);
    returnButton->setStyleSheet(
        "QPushButton {"
        "font: 15pt Stencil;"
        "background-color: rgb(255, 255, 255);"
        "color: rgb(111, 56, 0);"
        "border-radius: 10px;"
        "padding: 10px;"
        "}"
        "QPushButton:pressed {"
        "color: rgb(0, 0, 0);"
        "background-color: rgb(200, 146, 85);"
        "}"
        );
    returnButton->hide();
    returnButton->setEnabled(false);
    connect(returnButton,&QPushButton::clicked,this,&Server::on_returnButton);

}
//************************************************************
Server::~Server()
{
    delete ui;
}
//**************************************************************
void Server::newConnection() {
    socket = server->nextPendingConnection();

    ClientOrServer::server_label->setGeometry(50, 350, 561, 71);
    ClientOrServer::server_label->setStyleSheet("color: rgb(2, 2, 2); font: 15pt Snap ITC;");
    ClientOrServer::server_label->setText("A new player has been added to the game...");

    emit changePage();

    connect(socket, &QTcpSocket::readyRead, this, &Server::readyRead);
    connect(socket, &QTcpSocket::disconnected, this, &Server::disconnected);
}

//*****************************************************************
void Server :: readyRead(){

    QThread::msleep(100);

    QByteArray file_content = socket->readAll();
    QFile file("recivedCard.bin");
    if (file_content.isEmpty()) {
        QMessageBox Mb;
        Mb.critical(0,"ERROR","Received empty data");
    }
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox Mb;
        Mb.critical(0,"ERROR","File can not open");
        return;
    }
    file.write(file_content);
    file .close();

    // استفاده از mutex برای مدیریت همزمانی
    QMutexLocker locker(&mxForRead);
    readFromFileCards("recivedCard.bin",recivedCard);

    if(recivedCard.size()==1){

        if(recivedCard[0].getOrder()=="EXIT"){

            currentPlayer.set_win(currentPlayer.get_win()+1);
            currentPlayer.set_coin(currentPlayer.get_coin()+100);
            auto it = find_if(listOfPlayer.begin(),listOfPlayer.end(),[&](auto p){
                return(currentPlayer.get_username()==p.get_username());
            });
            it->set_win(currentPlayer.get_win());
            it->set_coin(currentPlayer.get_coin());
            writeToFile("myfile.bin");
            endTheGame("you win");
            return;
        }
        //**********************************************
        else if(recivedCard[0].getOrder().endsWith('@')){  // This means that the client has sent basic information such as the name
            srv->show();
            QString name = recivedCard[0].getOrder();
            name.chop(1);
            clientName->setText(name);
            this->play();
        }
        //**********************************************
        else if(recivedCard[0].getOrder()=="SCORE"){       // This means that the end of the game has come and the server must announce the result of the game
            clientScore->setText( QString::number(recivedCard[0].getNumber()));
            int rivalScore=recivedCard[0].getNumber();
            for(auto& x:pushButtons) delete x.cards_button;
            pushButtons.clear();

            caculateScore(rivalScore);
            return;
        }
        //**********************************************
        else if (recivedCard[0].getOrder().endsWith('#')) {  // This means that the client has sent its score
            QString name = recivedCard[0].getOrder();
            name.chop(1);
            clientName->setText(name);
            clientScore->setText(QString::number(recivedCard[0].getNumber()));
        }
        //**********************************************
        else if(recivedCard[0].getOrder()=="STOP"){
            ui->stop->setEnabled(false);
            for(auto& x:pushButtons) x.cards_button->setEnabled(false);
            ui->stopGost->show();
            ui->gameStoped->show();
            remainingTime = 60;
            timerLabel->setText(QString::number(remainingTime));
            timerLabel->show();
            timer->start(1000);

        }
        //***********************************************
        else if(recivedCard[0].getOrder()=="RESUME"){
            ui->stop->setEnabled(true);
            timer->stop();
            timerLabel->hide();
            ui->stopGost->hide();
            ui->gameStoped->hide();
            for(auto& x:pushButtons) x.cards_button->setEnabled(true);
        }
        //**********************************************
        else{
            //client send card
            client_card = recivedCard[0];
            if(!currentPlayer.get_starterOfEachRound()){
                currentPlayer.set_turn(true);
                availbleCards(client_card);
            }
            else {
                buttons temp;
                availbleCards(temp);
            }

            server_card.cards_button->setGeometry(140,230,181,251);
            client_card.cards_button->setGeometry(660,230,181,251);
            set_picture(client_card);
            QTimer::singleShot(800, this, [&]() {
                 worksForCalculateScore();
            });

        }
    }
    else{

        currentPlayer.set_randomCards(recivedCard,currentPlayer.get_countOfTurn());
        showCards(currentPlayer.playeCard);
        for(int i =0 ; i<pushButtons.size();i++){
            set_picture( pushButtons[i]);
        }
    }

}


//*****************************************************************
void Server::disconnected(){

    socket->close();
    socket->deleteLater();
}
//*******************************************************************
bool Server::creation(){
    // استفاده از اتصال جدید
    connect(server, &QTcpServer::newConnection, this, &Server::newConnection);

    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    bool ipFound = false;

    for(int nIter = 0; nIter < list.count(); nIter++) {
        if(!list[nIter].isLoopback()) {
            if (list[nIter].protocol() == QAbstractSocket::IPv4Protocol) {
                serverIp = list[nIter].toString();
                ipFound = true;
                break;
            }
        }
    }

    if (ipFound) {
        QHostAddress myIP;
        myIP.setAddress(serverIp);
        serverIp = "IP: " + serverIp;

        if (ClientOrServer::ip_label) {
            ClientOrServer::ip_label->setText(serverIp);
        }

        if (!server->listen(myIP, 8089)) {
            QMessageBox::critical(this, "Error",QString("Unable to start the server: %1.").arg(server->errorString()));
            ipFound = false;
        }
    } else {
        QMessageBox::critical(this, "Error", "No valid IPv4 address found");
    }
    return ipFound;
}

//***************************************************************************
QTcpSocket* Server::get_socket()
{
    return socket;

}
//*************************************************************************
QVector<buttons>& Server::get_buttons(){return pushButtons;}
//***************************************************************************
void Server::showCards(QList<cards> cCards){
    switch(cCards.size()){
    case 2:{
        for(int i = 0;i<2;i++){
            buttons QP;
            QP.cards_button = new QPushButton(this);
            QP.thisCard= cCards[i];
             pushButtons.push_back(QP);
        }
        int X =270;
        int Y = 490;
        int height = 205;
        int width =135;
        for(int i=0;i<2;i++,X+=150){
            pushButtons[i].cards_button->setGeometry(X,Y,width,height);
           pushButtons[i]. cards_button->show();
        }
        connect(pushButtons[0].cards_button,SIGNAL(clicked()),this,SLOT(on_Buttons0_clicked()));
        connect(pushButtons[1].cards_button,SIGNAL(clicked()),this,SLOT(on_Buttons1_clicked()));
    }
              break;
    case 4:{
        for(int i = 0;i<4;i++){
            buttons QP;
            QP.cards_button = new QPushButton(this);
            QP.thisCard= cCards[i];
             pushButtons.push_back(QP);
        }
        int X =210;
        int Y = 510;
        int height = 191;
        int width =121;
        for(int i=0;i<2;i++,X+=130,Y-=10){
            pushButtons[i].cards_button->setGeometry(X,Y,width,height);
            pushButtons[i].cards_button->show();
        }
        Y+=10;
        for(int i=2;i<4;i++,X+=130,Y+=10){
            pushButtons[i].cards_button->setGeometry(X,Y,width,height);
            pushButtons[i].cards_button->show();
        }
        connect(pushButtons[0].cards_button,SIGNAL(clicked()),this,SLOT(  on_Buttons0_clicked()));
        connect(pushButtons[1].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons1_clicked()));
        connect(pushButtons[2].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons2_clicked()));
        connect(pushButtons[3].cards_button,SIGNAL(clicked()),this,SLOT(  on_Buttons3_clicked()));
    }
               break;
    case 6:{
        for(int i = 0;i<6;i++){
            buttons QP;
            QP.cards_button = new QPushButton(this);
            QP.thisCard= cCards[i];
             pushButtons.push_back(QP);
        }
        int X = 160;
        int Y = 530;
        int height = 171;
        int width =111;
        for(int i=0;i<3;i++,X+=110,Y-=10){
            pushButtons[i].cards_button->setGeometry(X,Y,width,height);
            pushButtons[i].cards_button->show();
        }
        Y+=10;
        for(int i=3;i<6;i++,X+=110,Y+=10){
            pushButtons[i].cards_button->setGeometry(X,Y,width,height);
            pushButtons[i].cards_button->show();
        }
        connect(pushButtons[0].cards_button,SIGNAL(clicked()),this,SLOT(  on_Buttons0_clicked()));
        connect(pushButtons[1].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons1_clicked()));
        connect(pushButtons[2].cards_button,SIGNAL(clicked()),this,SLOT(  on_Buttons2_clicked()));
        connect(pushButtons[3].cards_button,SIGNAL(clicked()),this,SLOT(  on_Buttons3_clicked()));
        connect(pushButtons[4].cards_button,SIGNAL(clicked()),this,SLOT(  on_Buttons4_clicked()));
        connect(pushButtons[5].cards_button,SIGNAL(clicked()),this,SLOT(  on_Buttons5_clicked()));
    }
               break;
    case 8:{
        for(int i = 0;i<8;i++){
            buttons QP;
            QP.cards_button = new QPushButton(this);
            QP.thisCard= cCards[i];
             pushButtons.push_back(QP);
        }
        int X = 60;
        int Y = 530;
        int height = 171;
        int width =111;
        for(int i=0;i<4;i++,X+=110,Y-=10){
            pushButtons[i].cards_button->setGeometry(X,Y,width,height);
            pushButtons[i].cards_button->show();
        }
        Y+=10;
        for(int i=4;i<8;i++,X+=110,Y+=10){
            pushButtons[i].cards_button->setGeometry(X,Y,width,height);
            pushButtons[i].cards_button->show();
        }
        connect(pushButtons[0].cards_button,SIGNAL(clicked()),this,SLOT(  on_Buttons0_clicked()));
        connect(pushButtons[1].cards_button,SIGNAL(clicked()),this,SLOT(  on_Buttons1_clicked()));
        connect(pushButtons[2].cards_button,SIGNAL(clicked()),this,SLOT(  on_Buttons2_clicked()));
        connect(pushButtons[3].cards_button,SIGNAL(clicked()),this,SLOT(  on_Buttons3_clicked()));
        connect(pushButtons[4].cards_button,SIGNAL(clicked()),this,SLOT(  on_Buttons4_clicked()));
        connect(pushButtons[5].cards_button,SIGNAL(clicked()),this,SLOT(  on_Buttons5_clicked()));
        connect(pushButtons[6].cards_button,SIGNAL(clicked()),this,SLOT(  on_Buttons6_clicked()));
        connect(pushButtons[7].cards_button,SIGNAL(clicked()),this,SLOT(  on_Buttons7_clicked()));
    }
               break;
    case 10:{
        for(int i = 0;i<10;i++){
            buttons QP;
            QP.cards_button = new QPushButton(this);
            QP.thisCard= cCards[i];
             pushButtons.push_back(QP);
        }
        int X = 15;
        int Y = 540;
        int height = 161;
        int width =98;
        for(int i=0;i<5;i++,X+=95,Y-=10){
            pushButtons[i].cards_button->setGeometry(X,Y,width,height);
            pushButtons[i].cards_button->show();
        }
          Y+=10;
        for(int i=5;i<10;i++,X+=95,Y+=10){
            pushButtons[i].cards_button->setGeometry(X,Y,width,height);
            pushButtons[i].cards_button->show();
        }

        connect(pushButtons[0].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons0_clicked()));
        connect(pushButtons[1].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons1_clicked()));
        connect(pushButtons[2].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons2_clicked()));
        connect(pushButtons[3].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons3_clicked()));
        connect(pushButtons[4].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons4_clicked()));
        connect(pushButtons[5].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons5_clicked()));
        connect(pushButtons[6].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons6_clicked()));
        connect(pushButtons[7].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons7_clicked()));
        connect(pushButtons[8].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons8_clicked()));
        connect(pushButtons[9].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons9_clicked()));
    }
               break;
    case 12:{
        for(int i = 0;i<12;i++){
            buttons QP;
            QP.cards_button = new QPushButton(this);
            QP.thisCard= cCards[i];
             pushButtons.push_back(QP);
        }
        int X = 5;
        int Y = 560;
        int height = 138;
        int width =83;
        for(int i=0;i<6;i++,X+=81,Y-=10){
            pushButtons[i].cards_button->setGeometry(X,Y,width,height);
            pushButtons[i].cards_button->show();
        }
          Y+=10;
        for(int i=6;i<12;i++,X+=81,Y+=10){
            pushButtons[i].cards_button->setGeometry(X,Y,width,height);
            pushButtons[i].cards_button->show();
        }

        connect(pushButtons[0].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons0_clicked()));
        connect(pushButtons[1].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons1_clicked()));
        connect(pushButtons[2].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons2_clicked()));
        connect(pushButtons[3].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons3_clicked()));
        connect(pushButtons[4].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons4_clicked()));
        connect(pushButtons[5].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons5_clicked()));
        connect(pushButtons[6].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons6_clicked()));
        connect(pushButtons[7].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons7_clicked()));
        connect(pushButtons[8].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons8_clicked()));
        connect(pushButtons[9].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons9_clicked()));
        connect(pushButtons[10].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons10_clicked()));
        connect(pushButtons[11].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons11_clicked()));
    }
               break;
    case 14: {
        for(int i = 0;i<14;i++){
            buttons QP;
            QP.cards_button = new QPushButton(this);
            QP.thisCard= cCards[i];
             pushButtons.push_back(QP);
        }
        int X =5;
        int Y = 540;
        int height = 131;
        int width =71;
        for(int i=0;i<7;i++,X+=71,Y-=10){
            pushButtons[i].cards_button->setGeometry(X,Y,width,height);
            pushButtons[i].cards_button->show();
        }
          Y+=10;
        for(int i=7;i<14;i++,X+=71,Y+=10){
            pushButtons[i].cards_button->setGeometry(X,Y,width,height);
            pushButtons[i].cards_button->show();
        }
        connect(pushButtons[0].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons0_clicked()));
        connect(pushButtons[1].cards_button,SIGNAL(clicked()),this,SLOT(  on_Buttons1_clicked()));
        connect(pushButtons[2].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons2_clicked()));
        connect(pushButtons[3].cards_button,SIGNAL(clicked()),this,SLOT(  on_Buttons3_clicked()));
        connect(pushButtons[4].cards_button,SIGNAL(clicked()),this,SLOT(  on_Buttons4_clicked()));
        connect(pushButtons[5].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons5_clicked()));
        connect(pushButtons[6].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons6_clicked()));
        connect(pushButtons[7].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons7_clicked()));
        connect(pushButtons[8].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons8_clicked()));
        connect(pushButtons[9].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons9_clicked()));
        connect(pushButtons[10].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons10_clicked()));
        connect(pushButtons[11].cards_button,SIGNAL(clicked()),this,SLOT(  on_Buttons11_clicked()));
        connect(pushButtons[12].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons12_clicked()));
        connect(pushButtons[13].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons13_clicked()));

     }
               break;

   }


}
//***************************************************************************
void Server::whoShouldStartTheGameFirst(){

    QTime dietime=QTime::currentTime().addMSecs(2000);
    while(QTime::currentTime()<dietime)
        QCoreApplication::processEvents(QEventLoop::AllEvents,100);


    int server_number =( QRandomGenerator::global()->generate()%8)+1;
    int client_number =( QRandomGenerator::global()->generate()%8)+1;
    tab:if(server_number>client_number){
        currentPlayer.set_turn(true);
        currentPlayer.set_starterOfEachRound(true);
    }
    else if(client_number>server_number){
       currentPlayer.set_turn(false);
       currentPlayer.set_starterOfEachRound(false);
    }
    else{
        server_number =( QRandomGenerator::global()->generate()%8)+1;
        client_number =( QRandomGenerator::global()->generate()%8)+1;
        goto tab;
    }
    server_card.thisCard.setId(3);
    if(!sendName){
        server_card.thisCard.setOrder(currentPlayer.get_name());
        sendName = true;
    }

    server_card.thisCard.setNumber(server_number);
    server_card.thisCard.setValue(server_number);
    client_card.thisCard.setId(3);
    client_card.thisCard.setNumber(client_number);
    client_card.thisCard.setValue(client_number);
    sendCard.push_back(server_card.thisCard);
    sendCard.push_back(client_card.thisCard);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();
    file.close();

    QMutexLocker locker(&mx);
    if (socket->state() == QAbstractSocket::ConnectedState) {

        qint64 bytesWritten = socket->write(file_content);
        if (bytesWritten == -1) {
            QMessageBox mx;
            mx.critical(0, "Error", "Failed to send card");
        }
        socket->flush();
    } else {
        QMessageBox mx;
        mx.critical(0, "Error", "client is offline");
        return;
    }
    locker.unlock();

}
//*************************************************************************************
void Server::set_picture(struct buttons crd){

    switch(crd.thisCard.getId()){
    case 1:{
        switch(crd.thisCard.getNumber()){
        case 1:
            crd.cards_button->setStyleSheet("border-image: url(:/Prefix/resource/treasure1.png);");
            crd.cards_button->setToolTip("<html><head/><body><p><img src=:/Prefix/resource/treasure1.png width=150 height=200/></p></body></html>");
            crd.cards_button->show();
            break;
        case 2:
            crd.cards_button->setStyleSheet("border-image: url(:/Prefix/resource/treasure2.png);");
             crd.cards_button->setToolTip("<html><head/><body><p><img src=:/Prefix/resource/treasure2.png width=150 height=200/></p></body></html>");
            crd.cards_button->show();
            break;
        case 3:
            crd.cards_button->setStyleSheet("border-image: url(:/Prefix/resource/treasure3.png);");
             crd.cards_button->setToolTip("<html><head/><body><p><img src=:/Prefix/resource/treasure3.png width=150 height=200/></p></body></html>");
             crd.cards_button->show();
            break;
        case 4:
            crd.cards_button->setStyleSheet("border-image: url(:/Prefix/resource/treasure4.png);");
             crd.cards_button->setToolTip("<html><head/><body><p><img src=:/Prefix/resource/treasure4.png width=150 height=200/></p></body></html>");
             crd.cards_button->show();
            break;
        case 5:
            crd.cards_button->setStyleSheet("border-image: url(:/Prefix/resource/treasure5.png);");
             crd.cards_button->setToolTip("<html><head/><body><p><img src=:/Prefix/resource/treasure5.png width=150 height=200/></p></body></html>");
                crd.cards_button->show();
            break;
        case 6:
            crd.cards_button->setStyleSheet("border-image: url(:/Prefix/resource/treasure6.png);");
             crd.cards_button->setToolTip("<html><head/><body><p><img src=:/Prefix/resource/treasure6.png width=150 height=200/></p></body></html>");
            crd.cards_button->show();
            break;
        case 7:
            crd.cards_button->setStyleSheet("border-image: url(:/Prefix/resource/treasure7.png);");
             crd.cards_button->setToolTip("<html><head/><body><p><img src=:/Prefix/resource/treasure7.png width=150 height=200/></p></body></html>");
           crd.cards_button->show();
            break;
        case 8:
            crd.cards_button->setStyleSheet("border-image: url(:/Prefix/resource/treasure8.png);");
             crd.cards_button->setToolTip("<html><head/><body><p><img src=:/Prefix/resource/treasure8.png width=150 height=200/></p></body></html>");
            crd.cards_button->show();
            break;
        }

    }
        break;
    case 2:
        switch(crd.thisCard.getNumber()){
        case 1:
            crd.cards_button->setStyleSheet("border-image: url(:/Prefix/resource/mapcard1.png);");
            crd.cards_button->setToolTip("<html><head/><body><p><img src=:/Prefix/resource/mapcard1.png width=150 height=200/></p></body></html>");
           crd.cards_button->show();
            break;
        case 2:
            crd.cards_button->setStyleSheet("border-image: url(:/Prefix/resource/mapcard2.png);");
             crd.cards_button->setToolTip("<html><head/><body><p><img src=:/Prefix/resource/mapcard2.png width=150 height=200/></p></body></html>");
          crd.cards_button->show();
            break;
        case 3:
           crd.cards_button->setStyleSheet("border-image: url(:/Prefix/resource/mapcard3.png);");
            crd.cards_button->setToolTip("<html><head/><body><p><img src=:/Prefix/resource/mapcard3.png width=150 height=200/></p></body></html>");
          crd.cards_button->show();
            break;
        case 4:
            crd.cards_button->setStyleSheet("border-image: url(:/Prefix/resource/mapcard4.png);");
             crd.cards_button->setToolTip("<html><head/><body><p><img src=:/Prefix/resource/mapcard4.png width=150 height=200/></p></body></html>");
           crd.cards_button->show();
            break;
        case 5:
            crd.cards_button->setStyleSheet("border-image: url(:/Prefix/resource/mapcard5.png);");
             crd.cards_button->setToolTip("<html><head/><body><p><img src=:/Prefix/resource/mapcard5.png width=150 height=200/></p></body></html>");
           crd.cards_button->show();
            break;
        case 6:
            crd.cards_button->setStyleSheet("border-image: url(:/Prefix/resource/mapcard6.png);");
             crd.cards_button->setToolTip("<html><head/><body><p><img src=:/Prefix/resource/mapcard6.png width=150 height=200/></p></body></html>");
            crd.cards_button->show();
            break;
        case 7:
           crd.cards_button->setStyleSheet("border-image: url(:/Prefix/resource/mapcard7.png);");
            crd.cards_button->setToolTip("<html><head/><body><p><img src=:/Prefix/resource/mapcard7.png width=150 height=200/></p></body></html>");
           crd.cards_button->show();
            break;
        case 8:
            crd.cards_button->setStyleSheet("border-image: url(:/Prefix/resource/mapcard8.png);");
             crd.cards_button->setToolTip("<html><head/><body><p><img src=:/Prefix/resource/mapcard8.png width=150 height=200/></p></body></html>");
           crd.cards_button->show();
            break;
        }
        break;
    case 3:
        switch(crd.thisCard.getNumber()){
        case 1:
            crd.cards_button->setStyleSheet("border-image: url(:/Prefix/resource/parrot1.png);");
            crd.cards_button->setToolTip("<html><head/><body><p><img src=:/Prefix/resource/parrot1.png  width=150 height=200 /></p></body></html>");
           crd.cards_button->show();
            break;
        case 2:
           crd.cards_button->setStyleSheet("border-image: url(:/Prefix/resource/parrot2.png);");
           crd.cards_button->setToolTip("<html><head/><body><p><img src=:/Prefix/resource/parrot2.png  width=150 height=200 /></p></body></html>");
           crd.cards_button->show();
            break;
        case 3:
            crd.cards_button->setStyleSheet("border-image: url(:/Prefix/resource/parrot3.png);");
            crd.cards_button->setToolTip("<html><head/><body><p><img src=:/Prefix/resource/parrot3.png  width=150 height=200 /></p></body></html>");
           crd.cards_button->show();
            break;
        case 4:
            crd.cards_button->setStyleSheet("border-image: url(:/Prefix/resource/parrot4.png);");
            crd.cards_button->setToolTip("<html><head/><body><p><img src=:/Prefix/resource/parrot4.png  width=150 height=200 /></p></body></html>");
           crd.cards_button->show();
            break;
        case 5:
              crd.cards_button->setStyleSheet("border-image: url(:/Prefix/resource/parrot5.png);");
              crd.cards_button->setToolTip("<html><head/><body><p><img src=:/Prefix/resource/parrot5.png  width=150 height=200 /></p></body></html>");
           crd.cards_button->show();
            break;
        case 6:
               crd.cards_button->setStyleSheet("border-image: url(:/Prefix/resource/parrot6.png);");
               crd.cards_button->setToolTip("<html><head/><body><p><img src=:/Prefix/resource/parrot6.png  width=150 height=200 /></p></body></html>");
           crd.cards_button->show();
            break;
        case 7:
             crd.cards_button->setStyleSheet("border-image: url(:/Prefix/resource/parrot7.png);");
             crd.cards_button->setToolTip("<html><head/><body><p><img src=:/Prefix/resource/parrot7.png  width=150 height=200 /></p></body></html>");
           crd.cards_button->show();
            break;
        case 8:
            crd.cards_button->setStyleSheet("border-image: url(:/Prefix/resource/parrot8.png);");
            crd.cards_button->setToolTip("<html><head/><body><p><img src=:/Prefix/resource/parrot8.png  width=150 height=200 /></p></body></html>");
            crd.cards_button->show();
            break;
        }
        break;
    case 4:
        switch(crd.thisCard.getNumber())
        {
        case 1:
            crd.cards_button->setStyleSheet("border-image: url(:/Prefix/resource/flagcard1.png);");
            crd.cards_button->setToolTip("<html><head/><body><p><img src=:/Prefix/resource/flagcard1.png width=150 height=200/></p></body></html>");
           crd.cards_button->show();
            break;
        case 2:
            crd.cards_button->setStyleSheet("border-image: url(:/Prefix/resource/flagcard2.png);");
            crd.cards_button->setToolTip("<html><head/><body><p><img src=:/Prefix/resource/flagcard2.png width=150 height=200/></p></body></html>");
          crd.cards_button->show();
            break;
        case 3:
             crd.cards_button->setStyleSheet("border-image: url(:/Prefix/resource/flagcard3.png);");
             crd.cards_button->setToolTip("<html><head/><body><p><img src=:/Prefix/resource/flagcard3.png width=150 height=200/></p></body></html>");
           crd.cards_button->show();
            break;
        case 4:
             crd.cards_button->setStyleSheet("border-image: url(:/Prefix/resource/flagcard4.png);");
             crd.cards_button->setToolTip("<html><head/><body><p><img src=:/Prefix/resource/flagcard4.png width=150 height=200/></p></body></html>");
           crd.cards_button->show();
            break;
        case 5:
              crd.cards_button->setStyleSheet("border-image: url(:/Prefix/resource/flagcard5.png);");
              crd.cards_button->setToolTip("<html><head/><body><p><img src=:/Prefix/resource/flagcard5.png width=150 height=200/></p></body></html>");
           crd.cards_button->show();
            break;
        case 6:
               crd.cards_button->setStyleSheet("border-image: url(:/Prefix/resource/flagcard6.png);");
               crd.cards_button->setToolTip("<html><head/><body><p><img src=:/Prefix/resource/flagcard6.png width=150 height=200/></p></body></html>");
           crd.cards_button->show();
            break;
        case 7:
             crd.cards_button->setStyleSheet("border-image: url(:/Prefix/resource/flagcard7.png);");
             crd.cards_button->setToolTip("<html><head/><body><p><img src=:/Prefix/resource/flagcard7.png width=150 height=200/></p></body></html>");
            crd.cards_button->show();
            break;
        case 8:
             crd.cards_button->setStyleSheet("border-image: url(:/Prefix/resource/flagcard8.png);");
             crd.cards_button->setToolTip("<html><head/><body><p><img src=:/Prefix/resource/flagcard8.png width=150 height=200/></p></body></html>");
             crd.cards_button->show();
            break;
        }
        break;
    case 5:
         crd.cards_button->setStyleSheet("border-image: url(:/Prefix/resource/king.png);");
         crd.cards_button->setToolTip("<html><head/><body><p><img src=:/Prefix/resource/king.png width=150 height=200/></p></body></html>");
         crd.cards_button->show();
        break;
    case 6:
        crd.cards_button->setStyleSheet("border-image: url(:/Prefix/resource/queen.png);");
        crd.cards_button->setToolTip("<html><head/><body><p><img src=:/Prefix/resource/queen.png width=150 height=200/></p></body></html>");
        crd.cards_button->show();
        break;
    case 7:
        crd.cards_button->setStyleSheet("border-image: url(:/Prefix/resource/dozd.png);");
        crd.cards_button->setToolTip("<html><head/><body><p><img src=:/Prefix/resource/dozd.png width=150 height=200/></p></body></html>");
        crd.cards_button->show();
        break;
    }
}
//*************************************************************************************
void Server::availbleCards(buttons rivalCard){
    //mitooni bhine tar koni
    int rivalCardId=rivalCard.thisCard.getId();
   if(rivalCardId==1||rivalCardId==2||rivalCardId==3||rivalCardId==4){
       auto findIt=find_if(pushButtons.begin(),pushButtons.end(),[rivalCardId](buttons b){return (b.thisCard.getId()==rivalCardId);});
       //age az ina dashte bashe...
       if(findIt!=pushButtons.end()){
          for(auto& x:pushButtons){
              if(x.thisCard.getId()!=rivalCardId && x.thisCard.getId()!=5 &&x.thisCard.getId()!=6 &&x.thisCard.getId()!=7){
                  x.cards_button->setEnabled(false);
              }
              else x.cards_button->setEnabled(true);
          }
       }
       //age nadashte bashe....
       else for(auto& x:pushButtons)x.cards_button->setEnabled(true);
   }
   else for(auto& x:pushButtons)x.cards_button->setEnabled(true);

}
//********************************************************************************
void Server :: set_client_card(struct buttons cCard){
    client_card =cCard;
}
//********************************************************************************************
void Server :: set_server_card(struct buttons sCard){
    server_card =sCard;
}
//********************************************************************************************
buttons& Server::get_client_card(){
    return client_card;
}
//**********************************************************************************************
buttons& Server::get_server_card(){
    return server_card;
}
//*************************************************************************************************
void Server::change_geometry(){

    server_card.cards_button->setGeometry(140,230,181,251);
    client_card.cards_button->setGeometry(660,230,181,251);
    for(auto& x:pushButtons)x.cards_button->setEnabled(true);

}
//************************************************************************************************
void Server::on_Buttons0_clicked(){

    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
    for(auto& x:pushButtons)x.cards_button->setEnabled(false);
    auto it = find_if(currentPlayer.playeCard.begin(),currentPlayer.playeCard.end(),[&](auto p){
        return (p==pushButtons[0].thisCard);
    });
    if(it!=currentPlayer.playeCard.end()){
        currentPlayer.playeCard.erase(it);
    }
    server_card = pushButtons[0];


    currentPlayer.set_selectedCard(server_card.thisCard);
    sendCard.push_back(pushButtons[0].thisCard);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");

    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::critical(0, "Error", "Failed to open file for reading.");
        return;
    }

    QByteArray file_content = file.readAll();
    file.close();

    QMutexLocker locker(&mx);
    if (socket->state() == QAbstractSocket::ConnectedState) {

        qint64 bytesWritten = socket->write(file_content);
        if (bytesWritten == -1) {
            QMessageBox mx;
            mx.critical(0, "Error", "Failed to send card");
        }
        socket->flush();
    } else {
        QMessageBox mx;
        mx.critical(0, "Error", "client is offline");
        return;
    }
    locker.unlock();

    currentPlayer.set_turn(false);
    move_oneCards(pushButtons[0]);
    pushButtons[0].clear();
    QTimer::singleShot(800, this, [&]() {

        worksForCalculateScore();
    });
}

//**************************************************************************************************
void Server::on_Buttons1_clicked(){


    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
    for(auto& x:pushButtons)x.cards_button->setEnabled(false);

    auto it = find_if(currentPlayer.playeCard.begin(),currentPlayer.playeCard.end(),[&](auto p){
        return (p==pushButtons[1].thisCard);
    });
    if(it!=currentPlayer.playeCard.end()){
        currentPlayer.playeCard.erase(it);
    }
    server_card  = pushButtons[1];

    currentPlayer.set_selectedCard(server_card.thisCard);
    sendCard.push_back(pushButtons[1].thisCard);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);

    QByteArray file_content = file.readAll();
    file.close();

    QMutexLocker locker(&mx);
    if (socket->state() == QAbstractSocket::ConnectedState) {

        qint64 bytesWritten = socket->write(file_content);
        if (bytesWritten == -1) {
            QMessageBox mx;
            mx.critical(0, "Error", "Failed to send card");
        }
        socket->flush();
    } else {
        QMessageBox mx;
        mx.critical(0, "Error", "client is offline");
        return;
    }
    locker.unlock();
    currentPlayer.set_turn(false);
    move_oneCards(pushButtons[1]);
    pushButtons[1].clear();
    QTimer::singleShot(800, this, [&]() {

        worksForCalculateScore();
    });
}
//**************************************************************************************************
void Server::on_Buttons2_clicked(){


    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
    for(auto& x:pushButtons)x.cards_button->setEnabled(false);
    auto it = find_if(currentPlayer.playeCard.begin(),currentPlayer.playeCard.end(),[&](auto p){
        return (p==pushButtons[2].thisCard);
    });
    if(it!=currentPlayer.playeCard.end()){
        currentPlayer.playeCard.erase(it);
    }

    server_card  = pushButtons[2];
    currentPlayer.set_selectedCard(server_card.thisCard);
    sendCard.push_back(pushButtons[2].thisCard);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();
    file.close();

    QMutexLocker locker(&mx);
    if (socket->state() == QAbstractSocket::ConnectedState) {

        qint64 bytesWritten = socket->write(file_content);
        if (bytesWritten == -1) {
            QMessageBox mx;
            mx.critical(0, "Error", "Failed to send card");
        }
        socket->flush();
    } else {
        QMessageBox mx;
        mx.critical(0, "Error", "client is offline");
        return;
    }
    locker.unlock();
    currentPlayer.set_turn(false);
    move_oneCards(pushButtons[2]);
    pushButtons[2].clear();
    QTimer::singleShot(800, this, [&]() {

        worksForCalculateScore();
    });
}
//**************************************************************************************************
void Server::on_Buttons3_clicked(){

    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
    for(auto& x:pushButtons)x.cards_button->setEnabled(false);
    auto it = find_if(currentPlayer.playeCard.begin(),currentPlayer.playeCard.end(),[&](auto p){
        return (p==pushButtons[3].thisCard);
    });
    if(it!=currentPlayer.playeCard.end()){
        currentPlayer.playeCard.erase(it);
    }

    server_card  = pushButtons[3];
    currentPlayer.set_selectedCard(server_card.thisCard);
    sendCard.push_back(pushButtons[3].thisCard);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();
    file.close();
    QMutexLocker locker(&mx);
    if (socket->state() == QAbstractSocket::ConnectedState) {

        qint64 bytesWritten = socket->write(file_content);
        if (bytesWritten == -1) {
            QMessageBox mx;
            mx.critical(0, "Error", "Failed to send card");
        }
        socket->flush();
    } else {
        QMessageBox mx;
        mx.critical(0, "Error", "client is offline");
        return;
    }
    locker.unlock();
    currentPlayer.set_turn(false);
    move_oneCards(pushButtons[3]);
    pushButtons[3].clear();
    QTimer::singleShot(800, this, [&]() {

        worksForCalculateScore();
    });
}
//**************************************************************************************************
void Server::on_Buttons4_clicked(){


    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
    for(auto& x:pushButtons)x.cards_button->setEnabled(false);
    auto it = find_if(currentPlayer.playeCard.begin(),currentPlayer.playeCard.end(),[&](auto p){
        return (p==pushButtons[4].thisCard);
    });
    if(it!=currentPlayer.playeCard.end()){
        currentPlayer.playeCard.erase(it);
    }

    server_card  = pushButtons[4];
    currentPlayer.set_selectedCard(server_card.thisCard);
    sendCard.push_back(pushButtons[4].thisCard);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();
    QMutexLocker locker(&mx);
    if (socket->state() == QAbstractSocket::ConnectedState) {

        qint64 bytesWritten = socket->write(file_content);
        if (bytesWritten == -1) {
            QMessageBox mx;
            mx.critical(0, "Error", "Failed to send card");
        }
        socket->flush();
    } else {
        QMessageBox mx;
        mx.critical(0, "Error", "client is offline");
        return;
    }
    locker.unlock();

    currentPlayer.set_turn(false);
    move_oneCards(pushButtons[4]);
    pushButtons[4].clear();
    QTimer::singleShot(800, this, [&]() {

        worksForCalculateScore();
    });
}
//**************************************************************************************************
void Server::on_Buttons5_clicked(){


    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
    for(auto& x:pushButtons)x.cards_button->setEnabled(false);
    auto it = find_if(currentPlayer.playeCard.begin(),currentPlayer.playeCard.end(),[&](auto p){
        return (p==pushButtons[5].thisCard);
    });
    if(it!=currentPlayer.playeCard.end()){
        currentPlayer.playeCard.erase(it);
    }

    server_card  = pushButtons[5];
    currentPlayer.set_selectedCard(server_card.thisCard);
    sendCard.push_back(pushButtons[5].thisCard);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();
    file.close();

    QMutexLocker locker(&mx);
    if (socket->state() == QAbstractSocket::ConnectedState) {

        qint64 bytesWritten = socket->write(file_content);
        if (bytesWritten == -1) {
            QMessageBox mx;
            mx.critical(0, "Error", "Failed to send card");
        }
        socket->flush();
    } else {
        QMessageBox mx;
        mx.critical(0, "Error", "client is offline");
        return;
    }
    locker.unlock();
    currentPlayer.set_turn(false);
    move_oneCards(pushButtons[5]);
    pushButtons[5].clear();
    QTimer::singleShot(800, this, [&]() {

        worksForCalculateScore();
    });

}
//**************************************************************************************************
void Server::on_Buttons6_clicked(){


    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
    for(auto& x:pushButtons)x.cards_button->setEnabled(false);
    auto it = find_if(currentPlayer.playeCard.begin(),currentPlayer.playeCard.end(),[&](auto p){
        return (p==pushButtons[6].thisCard);
    });
    if(it!=currentPlayer.playeCard.end()){
        currentPlayer.playeCard.erase(it);
    }

    server_card  = pushButtons[6];
    currentPlayer.set_selectedCard(server_card.thisCard);
    sendCard.push_back(pushButtons[6].thisCard);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();
    file.close();

    QMutexLocker locker(&mx);
    if (socket->state() == QAbstractSocket::ConnectedState) {

        qint64 bytesWritten = socket->write(file_content);
        if (bytesWritten == -1) {
            QMessageBox mx;
            mx.critical(0, "Error", "Failed to send card");
        }
        socket->flush();
    } else {
        QMessageBox mx;
        mx.critical(0, "Error", "client is offline");
        return;
    }
    locker.unlock();
    currentPlayer.set_turn(false);
    move_oneCards(pushButtons[6]);
    pushButtons[6].clear();
    QTimer::singleShot(800, this, [&]() {

        worksForCalculateScore();
    });
}
//**************************************************************************************************
void Server::on_Buttons7_clicked(){


    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
    for(auto& x:pushButtons)x.cards_button->setEnabled(false);
    auto it = find_if(currentPlayer.playeCard.begin(),currentPlayer.playeCard.end(),[&](auto p){
        return (p==pushButtons[7].thisCard);
    });
    if(it!=currentPlayer.playeCard.end()){
        currentPlayer.playeCard.erase(it);
    }

    server_card = pushButtons[7];
    currentPlayer.set_selectedCard(server_card.thisCard);
    sendCard.push_back(pushButtons[7].thisCard);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();
    file.close();

    QMutexLocker locker(&mx);
    if (socket->state() == QAbstractSocket::ConnectedState) {

        qint64 bytesWritten = socket->write(file_content);
        if (bytesWritten == -1) {
            QMessageBox mx;
            mx.critical(0, "Error", "Failed to send card");
        }
        socket->flush();
    } else {
        QMessageBox mx;
        mx.critical(0, "Error", "client is offline");
        return;
    }
    locker.unlock();
    currentPlayer.set_turn(false);
    move_oneCards(pushButtons[7]);
    pushButtons[7].clear();
    QTimer::singleShot(800, this, [&]() {

        worksForCalculateScore();
    });
}
//**************************************************************************************************
void Server::on_Buttons8_clicked(){


    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
    for(auto& x:pushButtons)x.cards_button->setEnabled(false);
    auto it = find_if(currentPlayer.playeCard.begin(),currentPlayer.playeCard.end(),[&](auto p){
        return (p==pushButtons[8].thisCard);
    });
    if(it!=currentPlayer.playeCard.end()){
        currentPlayer.playeCard.erase(it);
    }

    server_card  = pushButtons[8];
    currentPlayer.set_selectedCard(server_card.thisCard);
    sendCard.push_back(pushButtons[8].thisCard);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();
    file.close();

    QMutexLocker locker(&mx);
    if (socket->state() == QAbstractSocket::ConnectedState) {

        qint64 bytesWritten = socket->write(file_content);
        if (bytesWritten == -1) {
            QMessageBox mx;
            mx.critical(0, "Error", "Failed to send card");
        }
        socket->flush();
    } else {
        QMessageBox mx;
        mx.critical(0, "Error", "client is offline");
        return;
    }
    locker.unlock();
    currentPlayer.set_turn(false);
    move_oneCards(pushButtons[8]);
    pushButtons[8].clear();
    QTimer::singleShot(800, this, [&]() {

        worksForCalculateScore();
    });

}
//**************************************************************************************************
void Server::on_Buttons9_clicked(){


    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
    for(auto& x:pushButtons)x.cards_button->setEnabled(false);
    auto it = find_if(currentPlayer.playeCard.begin(),currentPlayer.playeCard.end(),[&](auto p){
        return (p==pushButtons[9].thisCard);
    });
    if(it!=currentPlayer.playeCard.end()){
        currentPlayer.playeCard.erase(it);
    }

    server_card  = pushButtons[9];
    currentPlayer.set_selectedCard(server_card.thisCard);
    sendCard.push_back(pushButtons[9].thisCard);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();
    file.close();

    QMutexLocker locker(&mx);
    if (socket->state() == QAbstractSocket::ConnectedState) {

        qint64 bytesWritten = socket->write(file_content);
        if (bytesWritten == -1) {
            QMessageBox mx;
            mx.critical(0, "Error", "Failed to send card");
        }
        socket->flush();
    } else {
        QMessageBox mx;
        mx.critical(0, "Error", "client is offline");
        return;
    }
    locker.unlock();
    currentPlayer.set_turn(false);
    move_oneCards(pushButtons[9]);
    pushButtons[9].clear();
    QTimer::singleShot(800, this, [&]() {

        worksForCalculateScore();
    });
}
//**************************************************************************************************
void Server::on_Buttons10_clicked(){


    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
    for(auto& x:pushButtons)x.cards_button->setEnabled(false);
    auto it = find_if(currentPlayer.playeCard.begin(),currentPlayer.playeCard.end(),[&](auto p){
        return (p==pushButtons[10].thisCard);
    });
    if(it!=currentPlayer.playeCard.end()){
        currentPlayer.playeCard.erase(it);
    }

    server_card  = pushButtons[10];
    currentPlayer.set_selectedCard(server_card.thisCard);
    sendCard.push_back(pushButtons[10].thisCard);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();
    file.close();
    QMutexLocker locker(&mx);
    if (socket->state() == QAbstractSocket::ConnectedState) {

        qint64 bytesWritten = socket->write(file_content);
        if (bytesWritten == -1) {
            QMessageBox mx;
            mx.critical(0, "Error", "Failed to send card");
        }
        socket->flush();
    } else {
        QMessageBox mx;
        mx.critical(0, "Error", "client is offline");
        return;
    }
    locker.unlock();
    currentPlayer.set_turn(false);
    move_oneCards(pushButtons[10]);
    pushButtons[10].clear();
    QTimer::singleShot(800, this, [&]() {

        worksForCalculateScore();
    });
}
//**************************************************************************************************
void Server::on_Buttons11_clicked(){

    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
    for(auto& x:pushButtons)x.cards_button->setEnabled(false);
    auto it = find_if(currentPlayer.playeCard.begin(),currentPlayer.playeCard.end(),[&](auto p){
      return (p==pushButtons[11].thisCard);
    });
    if(it!=currentPlayer.playeCard.end()){
    currentPlayer.playeCard.erase(it);
    }

    server_card  = pushButtons[11];
    currentPlayer.set_selectedCard(server_card.thisCard);
    sendCard.push_back(pushButtons[11].thisCard);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();
    file.close();

    QMutexLocker locker(&mx);
    if (socket->state() == QAbstractSocket::ConnectedState) {

        qint64 bytesWritten = socket->write(file_content);
        if (bytesWritten == -1) {
            QMessageBox mx;
            mx.critical(0, "Error", "Failed to send card");
        }
        socket->flush();
    } else {
        QMessageBox mx;
        mx.critical(0, "Error", "client is offline");
        return;
    }
    locker.unlock();
    currentPlayer.set_turn(false);
    move_oneCards(pushButtons[11]);
    pushButtons[11].clear();
    QTimer::singleShot(800, this, [&]() {

        worksForCalculateScore();
    });
}
//**************************************************************************************************
void Server::on_Buttons12_clicked(){

    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
    for(auto& x:pushButtons)x.cards_button->setEnabled(false);
    auto it = find_if(currentPlayer.playeCard.begin(),currentPlayer.playeCard.end(),[&](auto p){
        return (p==pushButtons[12].thisCard);
    });
    if(it!=currentPlayer.playeCard.end()){
        currentPlayer.playeCard.erase(it);
    }
    server_card  = pushButtons[12];
    currentPlayer.set_selectedCard(server_card.thisCard);
    sendCard.push_back(pushButtons[12].thisCard);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();
    file.close();
    QMutexLocker locker(&mx);
    if (socket->state() == QAbstractSocket::ConnectedState) {

        qint64 bytesWritten = socket->write(file_content);
        if (bytesWritten == -1) {
            QMessageBox mx;
            mx.critical(0, "Error", "Failed to send card");
        }
        socket->flush();
    } else {
        QMessageBox mx;
        mx.critical(0, "Error", "client is offline");
        return;
    }
    locker.unlock();
    currentPlayer.set_turn(false);
    move_oneCards(pushButtons[12]);
    pushButtons[12].clear();
    QTimer::singleShot(800, this, [&]() {

        worksForCalculateScore();
    });
}
//**************************************************************************************************
void Server::on_Buttons13_clicked(){


    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
    for(auto& x:pushButtons)x.cards_button->setEnabled(false);
    auto it = find_if(currentPlayer.playeCard.begin(),currentPlayer.playeCard.end(),[&](auto p){
        return (p==pushButtons[13].thisCard);
    });
    if(it!=currentPlayer.playeCard.end()){
        currentPlayer.playeCard.erase(it);
    }
    server_card  = pushButtons[13];
    currentPlayer.set_selectedCard(server_card.thisCard);
    sendCard.push_back(pushButtons[13].thisCard);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();
    file.close();

    QMutexLocker locker(&mx);
    if (socket->state() == QAbstractSocket::ConnectedState) {

        qint64 bytesWritten = socket->write(file_content);
        if (bytesWritten == -1) {
            QMessageBox mx;
            mx.critical(0, "Error", "Failed to send card");
        }
        socket->flush();
    } else {
        QMessageBox mx;
        mx.critical(0, "Error", "client is offline");
        return;
    }
    locker.unlock();
    currentPlayer.set_turn(false);
    move_oneCards(pushButtons[13]);
    pushButtons[13].clear();
    QTimer::singleShot(800, this, [&]() {

        worksForCalculateScore();
    });

}
//**************************************************************************************************
void Server::move_oneCards(buttons& cCards){

    cCards.cards_button->disconnect();
    cCards.cards_button->hide();
    set_picture(server_card);
    server_card.cards_button->setGeometry(140,230,181,251);
    client_card.cards_button->setGeometry(660,230,181,251);

}
//**************************************************************************************************
void Server::move_twoCards(){

    QMutexLocker locker(&mx);

    QPropertyAnimation *animServer = new QPropertyAnimation(server_card.cards_button, "pos", this);
    QPropertyAnimation *animClient = new QPropertyAnimation(client_card.cards_button, "pos", this);

    animServer->setDuration(410);
    animServer->setEndValue(QPoint(-10, 260));
    animClient->setDuration(410);
    animClient->setEndValue(QPoint(-10, 260));

    connect(animServer, &QAbstractAnimation::finished, this, [&]() {
        server_card.cards_button->hide();
        server_card.clear();
        client_card.clear();
        for (auto& x : pushButtons) {
            x.cards_button->setEnabled(true);
        }
    });

    connect(animClient, &QAbstractAnimation::finished, this, [&]() {
        client_card.cards_button->hide();
        server_card.clear();
        client_card.clear();
        for (auto& x : pushButtons) {
            x.cards_button->setEnabled(true);
        }

        if (currentPlayer.playeCard.size() == 0) {
            if (currentPlayer.get_countOfTurn() < 7) {
                emit finishCard();
            }
        }
    });

    connect(animServer, &QAbstractAnimation::finished, animServer, &QObject::deleteLater);
    connect(animClient, &QAbstractAnimation::finished, animClient, &QObject::deleteLater);
    connect(animClient, &QAbstractAnimation::finished, this, &Server::change_geometry);

    animServer->start();
    animClient->start();

}
//*************************************************************************************************
void Server ::play(){

    QString round = "Round "+ QString::number(currentPlayer.get_countOfTurn())+" 👻🎃";
    roundNumber->setText(round);
    roundNumber->show();
    if(currentPlayer.get_countOfTurn()>1 && currentPlayer.get_countOfTurn()<7)caculateScore(0);


    // server pick cards first
    int turncount = currentPlayer.get_countOfTurn();
    sendCard = currentPlayer.creat_cards();
    currentPlayer.set_randomCards(sendCard,turncount);
    showCards(currentPlayer.playeCard);
    for(int i=0 ; i<pushButtons.size();i++){
        set_picture(pushButtons[i]);

    }
    for(auto& x:pushButtons)x.cards_button->setEnabled(false);
    ui->stop->setEnabled(false);
    ui->exit->setEnabled(false);
    writeToFileCards("sendCard.bin",sendCard);

    QFile file2("sendCard.bin");
    file2.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content2 = file2.readAll();
    file2.close();
    //send cards to client
    QMutexLocker locker(&mx);
    if (socket->state() == QAbstractSocket::ConnectedState) {

        qint64 bytesWritten = socket->write(file_content2);
        if (bytesWritten == -1) {
            QMessageBox mx;
            mx.critical(0, "Error", "Failed to send card");
        }
        socket->flush();
    } else {
        QMessageBox mx;
        mx.critical(0, "Error", "client is offline");
        return;
    }
    locker.unlock();
    //end

    /// making parrote cards
    whoShouldStartTheGameFirst();
    server_card.cards_button->setGeometry(140,230,181,251);
    client_card.cards_button->setGeometry(660,230,181,251);
    set_picture(server_card);
    set_picture(client_card);

    QTimer::singleShot(2000, this, [&]() {
        move_twoCards();
        QTimer::singleShot(1000, this, [&]() {

            //guess label....

            roundNumber->hide();
            guessLabel=new QLineEdit(this);
            guessLabel->setGeometry(300,310,500,51);
            QFont font_line("Algerian");
            guessLabel->setStyleSheet("font : 14pt;color: rgb(255,255,255);background:rgba(0,0,0,0);border:2px solid;border-color: rgb(255,255,255);");
            guessLabel->setFont(font_line);
            guessLabel->setPlaceholderText("Enter your guess");
            for(auto& x:pushButtons)x.cards_button->setEnabled(false);
            guessLabel->show();
            connect(guessLabel, &QLineEdit::editingFinished, this, [&]() {
                bool ok;
                int guess = guessLabel->text().toInt(&ok);

                if (!ok || guess < 0 || guess > pushButtons.size()) {
                    QMessageBox::warning(this, "Invalid Input", "Please enter a valid number.");
                    guessLabel->clear();
                    guessLabel->setFocus();
                } else {
                    currentPlayer.set_guess(guess);
                    guessLabel->hide();
                    for (auto& x : pushButtons) x.cards_button->setEnabled(true);
                    ui->stop->setEnabled(true);
                    ui->exit->setEnabled(true);
                }
            });

        });
    });





}
//***********************************************************************************************
void Server::caculateScore(int rivalScore){
    QString res ;
    //age dorost gofte bashe....
    if(currentPlayer.get_guess()==currentPlayer.get_setWin()){
        //age gofte bashe 0 dast
        if(currentPlayer.get_guess()==0)currentPlayer.set_score(currentPlayer.get_score()+((currentPlayer.get_countOfTurn()-1)*10));
        else currentPlayer.set_score(currentPlayer.get_score()+(currentPlayer.get_guess()*10));
    }
    //age ghalat gofte bashe.....
    else{
        //age gofte bashe 0 dast
        if(currentPlayer.get_guess()==0)currentPlayer.set_score(currentPlayer.get_score()-((currentPlayer.get_countOfTurn()-1)*10));
        else currentPlayer.set_score(currentPlayer.get_score()-(abs(currentPlayer.get_guess()-currentPlayer.get_setWin())*10));
    }
    if(currentPlayer.get_countOfTurn() == 7){
        if(rivalScore>currentPlayer.get_score()){
            cards server_order;
            server_order.setOrder("You Win");
            sendCard.push_back(server_order);
            writeToFileCards("sendCard.bin",sendCard);
            QFile file("sendCard.bin");
            file.open(QFile::ReadOnly | QFile::Text);
            QByteArray file_content = file.readAll();
            file.close();

            QMutexLocker locker(&mx);
            if (socket->state() == QAbstractSocket::ConnectedState) {

                qint64 bytesWritten = socket->write(file_content);
                if (bytesWritten == -1) {
                    QMessageBox mx;
                    mx.critical(0, "Error", "Failed to send card");
                }
                socket->flush();
            } else {
                QMessageBox mx;
                mx.critical(0, "Error", "client is offline");
                return;
            }
            locker.unlock();
            currentPlayer.set_lose(currentPlayer.get_lose()+1);
            res = "you lose";

        }

        else if(rivalScore<currentPlayer.get_score()){
            cards server_order;
            server_order.setOrder("You Lose");
            sendCard.push_back(server_order);
            writeToFileCards("sendCard.bin",sendCard);
            QFile file("sendCard.bin");
            file.open(QFile::ReadOnly | QFile::Text);
            QByteArray file_content = file.readAll();
            file.close();
            QMutexLocker locker(&mx);
            if (socket->state() == QAbstractSocket::ConnectedState) {

                qint64 bytesWritten = socket->write(file_content);
                if (bytesWritten == -1) {
                    QMessageBox mx;
                    mx.critical(0, "Error", "Failed to send card");
                }
                socket->flush();
            } else {
                QMessageBox mx;
                mx.critical(0, "Error", "client is offline");
                return;
            }
            locker.unlock();
            currentPlayer.set_win(currentPlayer.get_win()+1);
            currentPlayer.set_coin(currentPlayer.get_coin()+100);
            res = "you win";

        }
        else{

            cards server_order;
            server_order.setOrder("Equal");
            sendCard.push_back(server_order);
            writeToFileCards("sendCard.bin",sendCard);
            QFile file("sendCard.bin");
            file.open(QFile::ReadOnly | QFile::Text);
            QByteArray file_content = file.readAll();
            file.close();
            QMutexLocker locker(&mx);
            if (socket->state() == QAbstractSocket::ConnectedState) {

                qint64 bytesWritten = socket->write(file_content);
                if (bytesWritten == -1) {
                    QMessageBox mx;
                    mx.critical(0, "Error", "Failed to send card");
                }
                socket->flush();
            } else {
                QMessageBox mx;
                mx.critical(0, "Error", "client is offline");
                return;
            }
            locker.unlock();
            currentPlayer.set_coin(currentPlayer.get_coin()+50);
            res = "Equal";
        }

        auto foundPlayer=find_if(listOfPlayer.begin(),listOfPlayer.end(),[](auto x){return(x.get_username() ==currentPlayer.get_username());});
        foundPlayer->set_lose(currentPlayer.get_lose());
        foundPlayer->set_win(currentPlayer.get_win());
        foundPlayer->set_coin(currentPlayer.get_coin());
        writeToFile("myfile.bin");
    }
    scoreNumber->setText(QString::number(currentPlayer.get_score()));
    sendScore();
    currentPlayer.set_setWin(0);

    if(currentPlayer.get_countOfTurn() == 7){
         endTheGame(res);
    }

}
//****************************************************************************************************
void Server::on_resumeButton_clicked(){


    resume->setEnabled(false);
    resume->hide();
    ui->stop->setEnabled(true);
    ui->stop->show();
    timer->stop();
    timerLabel->hide();
    ui->stopGost->hide();
    ui->gameStoped->hide();

    for(auto& x:pushButtons) x.cards_button->setEnabled(true);
    cards server_order;
    server_order.setOrder("RESUME");
    sendCard.push_back(server_order);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();
    file.close();

    QMutexLocker locker(&mx);
    if (socket->state() == QAbstractSocket::ConnectedState) {

        qint64 bytesWritten = socket->write(file_content);
        if (bytesWritten == -1) {
            QMessageBox mx;
            mx.critical(0, "Error", "Failed to send card");
        }
        socket->flush();
    } else {
        QMessageBox mx;
        mx.critical(0, "Error", "client is offline");

    }
    locker.unlock();

}
//*******************************************************************************************************
void Server::on_stop_clicked()
{

    ui->stop->setEnabled(false);
    ui->stop->hide();
    resume->setEnabled(true);
    resume->show();
    for(auto& x:pushButtons) x.cards_button->setEnabled(false);
    ui->stopGost->show();
    ui->gameStoped->show();
    cards server_order;
    server_order.setOrder("STOP");
    sendCard.push_back(server_order);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();
    file.close();
    QMutexLocker locker(&mx);
    if (socket->state() == QAbstractSocket::ConnectedState) {

        qint64 bytesWritten = socket->write(file_content);
        if (bytesWritten == -1) {
            QMessageBox::critical(0, "Error", "Failed to write data to socket.");
        }
        socket->flush();
    } else {
        QMessageBox::critical(0, "Error", "Socket is not connected.");
        return;
    }
    locker.unlock();

    remainingTime = 60;
    timerLabel->setText(QString::number(remainingTime));
    timerLabel->show();
    timer->start(1000);
}
//*******************************************************************************************************
void Server::on_exit_clicked()
{

    currentPlayer.set_lose(currentPlayer.get_lose()+1);
    auto it = find_if(listOfPlayer.begin(),listOfPlayer.end(),[&](auto p){
        return(currentPlayer.get_username()==p.get_username());
    });
    it->set_lose(currentPlayer.get_lose());

    writeToFile("myfile.bin");

    cards server_order;
    server_order.setOrder("EXIT");
    sendCard.push_back(server_order);

    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();
    file.close();

    QMutexLocker locker(&mx);
    if (socket->state() == QAbstractSocket::ConnectedState) {

        qint64 bytesWritten = socket->write(file_content);
        if (bytesWritten == -1) {
            QMessageBox mx;
            mx.critical(0, "Error", "Failed to send card");
        }
        socket->flush();
    } else {
        QMessageBox mx;
        mx.critical(0, "Error", "client is offline");
    }
    locker.unlock();
    endTheGame("you lose");
}
//*******************************************************************************************************
void Server::sendScore(){

    cards server_order;
    QString order = QString::number(currentPlayer.get_score()) +"#";
    server_order.setOrder(order);
    sendCard.push_back(server_order);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();
    file.close();

    QMutexLocker locker(&mx);
    if (socket->state() == QAbstractSocket::ConnectedState) {

        qint64 bytesWritten = socket->write(file_content);
        if (bytesWritten == -1) {
            QMessageBox mx;
            mx.critical(0, "Error", "Failed to send card");
        }
        socket->flush();
    } else {
        QMessageBox mx;
        mx.critical(0, "Error", "client is offline");
    }
    locker.unlock();
}
//*******************************************************************************************************************
void Server::worksForCalculateScore(){
    if(!client_card.empty()&& !server_card.empty()){
        currentPlayer.calculate(client_card.thisCard);
        QTimer::singleShot(800, this, [&]() {
            move_twoCards();
            if(currentPlayer.playeCard.size()==0){

                if(currentPlayer.get_countOfTurn()==7){
                    for (auto& x : pushButtons) {
                        delete x.cards_button;
                    }
                    pushButtons.clear();
                    ui->stop->setEnabled(false);
                    ui->exit->setEnabled(false);
                    ui->stop->hide();
                    ui->exit->hide();
                }

            }
            sendScore();
        });

    }

}
//******************************************************************************************
void Server::finishCards(){
    currentPlayer.set_countOfTurn(currentPlayer.get_countOfTurn()+1);
    QTimer::singleShot(1300, this, [&]() {
        for (auto& x : pushButtons) {
            delete x.cards_button;
        }
        pushButtons.clear();
        play();
    });
}
//******************************************************************************************
void Server::updateTimer() {

    remainingTime--;
    timerLabel->setText(QString::number(remainingTime));
    if (remainingTime <= 0) {
        timer->stop();
        timerLabel->hide();
        on_resumeButton_clicked();
    }

}
//******************************************************************************************
void Server::endTheGame(QString res){

    clearDataOfGame();
    ui->stop->setEnabled(false);
    ui->exit->setEnabled(false);
    resume->setEnabled(false);
    resume->hide();
    ui->stop->hide();
    ui->exit->hide();
    ui->gost->hide();
    timer->stop();
    timerLabel->hide();
    ui->stopGost->hide();
    ui->gameStoped->hide();
    guessLabel->hide();

    ui->background->setGeometry(0,0,1000,700);
    ui->background->setStyleSheet("border-image: url(:/Prefix/resource/981.jpg);");
    clientPic->show();
    serverPic->show();
    returnButton->show();
    returnButton->setEnabled(true);
    result->show();
    if(res == "you win"){
        result->setText("you win");
        crown->setGeometry(97,223,181,211);
        crown->show();
    }
    else if(res == "you lose"){
        result->setText("you lose");
        crown->setGeometry(761,215,181,211);
        crown->show();
    }
    else{

        result->setText("Equal");
    }
}
//******************************************************************************************
void Server::stopServer() {
    if (server->isListening()) {
        server->close();  // Stop listening
    }

    // Close all pending connections
    foreach(QTcpSocket *socket, server->findChildren<QTcpSocket*>()) {
        socket->close();
        socket->deleteLater();
    }
    delete server;
}
//******************************************************************************************
void Server::clearDataOfGame(){

    writeToFile("myfile.bin");
    currentPlayer.clearDataOfGame();
    sendCard.clear();
    recivedCard.clear();
    for (auto& x : pushButtons) {
        x.cards_button->setEnabled(false);
    }
    for (auto& x : pushButtons) {
        x.cards_button->hide();
    }
    for (auto& x : pushButtons) {
        delete x.cards_button;
    }
    pushButtons.clear();
    server_card.cards_button->hide();
    client_card.cards_button->hide();
    server_card.clear();
    client_card.clear();

}
//******************************************************************************************
void Server::on_returnButton(){


    menu* newPage = new menu;
    this->close();
    newPage->show();
    stopServer();
    delete srv;
    newPage->showText();
}
