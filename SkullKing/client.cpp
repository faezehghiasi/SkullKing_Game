#include"signup.h"
#include "client.h"
#include"signin.h"
#include "ui_client.h"
#include"clientorserver.h"
#include <QDataStream>
#include<QFile>
#include<QTimer>
#include"menu.h"
#include<QMutex>
#include<QThread>
#include<QMovie>
QMutex mx2;
QMutex mxForRead2;
QString Ip;
Client* cln;
Client::Client(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Client)
{
    ui->setupUi(this);
    ui->gameStoped->hide();
    ui->stopGost->hide();
    connect(this,&Client::sendEndGame,this,&Client::sendServerendOfGame);

    server_card.cards_button = new QPushButton(this);
    server_card.cards_button->setGeometry(660,230,181,251);
    client_card.cards_button = new QPushButton(this);
    client_card.cards_button->setGeometry(140,230,181,251);
    server_card.cards_button->hide();
    client_card.cards_button->hide();


    scoreNumber= new QLabel(this);
    scoreNumber->setStyleSheet("font: 20pt Broadway;  color: rgb(13, 13, 13);");
    scoreNumber->setGeometry(180,85,141,41);
    scoreNumber->setText(QString::number(currentPlayer.get_score()));
    scoreNumber->show();


    serverName= new QLabel(this);
    serverName->setStyleSheet("font: 18pt Broadway;  color: rgb(13, 13, 13);");
    serverName->setGeometry(650,40,252,51);
    serverName->setAlignment(Qt::AlignRight);
    serverName->show();

    serverScore = new QLabel(this);
    serverScore->setStyleSheet("font: 20pt Broadway;  color: rgb(13, 13, 13);");
    serverScore->setGeometry(710,110,121,41);
    serverScore->setAlignment(Qt::AlignRight);
    serverScore->setText("0");
    serverScore->show();


    clientName= new QLabel(this);
    clientName->setStyleSheet("font: 18pt Broadway;  color: rgb(13, 13, 13);");
    clientName->setText(currentPlayer.get_name());
    clientName->setGeometry(120,30,141,41);
    clientName->show();

    roundNumber = new QLabel(this);
    roundNumber->setAlignment(Qt::AlignCenter);
    roundNumber ->setStyleSheet(
        "QLabel {"
        "   background: transparent;" // Transparent background
        "   color: #bb5a3f;" // Vibrant pink color for the text
        "   font-family: 'Comic Sans MS', cursive, sans-serif;"
        "   font-size: 30px;" // Larger font size
        "   font-weight: bold;"
        "   border: none;" // No border
        "   border-radius: 0px;" // No border radius
        "   padding: 20px;" // Increased padding for better spacing
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
    timerLabel->setGeometry(400, 70, 171, 181);
    timerLabel->hide();
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Client::updateTimer);


    resume = new QPushButton(this);
    resume->setStyleSheet("border-image: url(:/Prefix/resource/Firefly_play_icon_is_circular_in_green_color_in_3D_35008-removebg-preview.png);background-color: rgba(0,0,0,0);border-radius:25px;");
    resume->setGeometry(920,270,71,71);
    resume->setEnabled(false);
    resume->hide();
    connect(resume,SIGNAL(clicked()),this,SLOT(on_resumeButton_clicked()));


    serverPic = new QLabel(this);
    serverPic->setStyleSheet("border-image: url(:/Prefix/resource/Firefly_cute_skeleton_icon_for_game_in_3D_56539-removebg-preview.png);");
    serverPic->setGeometry(760,310,211,301);
    serverPic->hide();

    clientPic = new QLabel(this);
    clientPic->setStyleSheet("border-image: url(:/Prefix/resource/Firefly_cute_skeleton_icon_for_game_in_3D_78259-removebg-preview.png);");
    clientPic->setGeometry(30,330,211,261);
    clientPic->hide();


    crown = new QLabel(this);
    crown->setStyleSheet("border-image: url(:/Prefix/resource/crown.png);");
    crown->hide();

    result = new QLabel(this);
    result->setStyleSheet("font:700 90pt Curlz MT;color: rgb(111, 56, 0);");
    result->setGeometry(240,350,541,181);
    result->setAlignment(Qt::AlignCenter);
    result->hide();

    returnButton=new QPushButton("Back to menu",this);
    returnButton->setGeometry(10,630,211,71);
    returnButton->setStyleSheet(
        "QPushButton {"
        "font: 15pt Stencil;"
        "color: rgb(111, 56, 0);"
        "border-radius: 10px;"
        "padding: 10px;"
        "}"
        "QPushButton:pressed {"
        "color: rgb(200, 146, 85);"
        "}"
        );
    returnButton->hide();
    returnButton->setEnabled(false);
    connect(returnButton,&QPushButton::clicked,this,&Client::on_returnButton);

}
//*****************************************************************************
bool Client::creation() {
    socket = new QTcpSocket;

    connect(socket, &QTcpSocket::connected, this, &Client::connected);
    connect(socket, &QTcpSocket::disconnected, this, &Client::disconnected);
    connect(socket, &QTcpSocket::readyRead, this, &Client::readyRead);

    QHostAddress add;
    if (!add.setAddress(Ip)) { // بررسی اینکه آیا آدرس معتبر است یا نه
        QMessageBox::critical(this, "Error", "Invalid IP address");
        return false;
    }

    socket->connectToHost(add, 8089);
    if (!socket->waitForConnected(2000)) {
        QMessageBox::critical(this, "Error", "Server not found");
        return false;
    }
    return true;
}

//******************************************************************************
Client::~Client()
{
    delete ui;
}
//*****************************************************************************
void Client::connected() {

}
//*****************************************************************************
void Client::disconnected() {

    socket->close();
}
//****************************************************************************
void Client::readyRead() {

    QThread::msleep(100);

    QByteArray file_content = socket->readAll();
    QFile file("recivedCard.bin");
    if(!file.open(QFile::WriteOnly | QFile::Text)){
        return;
    }
    file.write(file_content);
    file .close();

    // استفاده از mutex برای مدیریت همزمانی
    QMutexLocker locker(&mxForRead2);
    readFromFileCards("recivedCard.bin",recivedCard);

    if(recivedCard.size()==1){

        if(recivedCard[0].getOrder().endsWith('$')){ // This means that the server has sent basic information such as the name
            emit closePage();
            cln->show();
            QString name = recivedCard[0].getOrder();
            name.chop(1);
            serverName->setText(name);
            sendName();
        }

        //**********************************************
        else if (recivedCard[0].getOrder().endsWith('#')) { // This means that the server has sent its score
            QString scoreNumber = recivedCard[0].getOrder();
            scoreNumber.chop(1);
            serverScore->setText(scoreNumber);

        }
        //****************************************
        else if(recivedCard[0].getOrder()=="EXIT"){

            currentPlayer.set_win(currentPlayer.get_win()+1);
            currentPlayer.set_coin(currentPlayer.get_coin()+100);
            auto it = find_if(listOfPlayer.begin(),listOfPlayer.end(),[&](auto p){
                return(currentPlayer.get_username()==p.get_username());
            });
            it->set_win(currentPlayer.get_win());
            it->set_coin(currentPlayer.get_coin());
            writeToFile("myfile.bin");
            endTheGame("you win");
        }
        //*******************************************
        else if(recivedCard[0].getOrder()=="You Win"){

            currentPlayer.set_win(currentPlayer.get_win()+1);
            currentPlayer.set_coin(currentPlayer.get_coin()+100);
            auto foundPlayer=find_if(listOfPlayer.begin(),listOfPlayer.end(),[](auto x){return(x.get_username()==currentPlayer.get_username());});
            foundPlayer->set_win(currentPlayer.get_win());
            foundPlayer->set_coin(currentPlayer.get_coin());
            writeToFile("myfile.bin");
            endTheGame("you win");
        }
        //*********************************************
        else if(recivedCard[0].getOrder()=="You Lose"){
            currentPlayer.set_lose(currentPlayer.get_lose()+1);
            auto foundPlayer=find_if(listOfPlayer.begin(),listOfPlayer.end(),[](auto x){return(x.get_username()==currentPlayer.get_username());});
            foundPlayer->set_lose(currentPlayer.get_lose());


            writeToFile("myfile.bin");
            endTheGame("you lose");
        }
        //*********************************************
        else if(recivedCard[0].getOrder()=="Equal"){
            currentPlayer.set_coin(currentPlayer.get_coin()+50);
            auto foundPlayer=find_if(listOfPlayer.begin(),listOfPlayer.end(),[](auto x){return(x.get_username()==currentPlayer.get_username());});
            foundPlayer->set_coin(currentPlayer.get_coin());
            writeToFile("myfile.bin");
            endTheGame("Equal");

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
            //server send card
            server_card = recivedCard[0];
            if(!currentPlayer.get_starterOfEachRound()){
                currentPlayer.set_turn(true);
                availbleCards(server_card);
            }
            else {
                buttons temp;
                availbleCards(temp);
            }

            server_card.cards_button->setGeometry(660,230,181,251);
            client_card.cards_button->setGeometry(140,230,181,251);
            set_picture(server_card);

            ///calculateing the score
            QTimer::singleShot(800, this, [&]() {
                worksForCalculateScore();
            });
            /// end
        }

    }
    //************************************************************************************
    else if(recivedCard.size()==2){  // server send parrot cards

        if(recivedCard[0].getNumber()<recivedCard[1].getNumber()) {
            currentPlayer.set_turn(true);
            currentPlayer.set_starterOfEachRound(true);
        }
        else{
            currentPlayer.set_turn(false);
            currentPlayer.set_starterOfEachRound(false);
        }
        server_card = recivedCard[0];
        client_card = recivedCard[1];

        server_card.cards_button->setGeometry(660,230,181,251);
        client_card.cards_button->setGeometry(140,230,181,251);
        set_picture(client_card);
        set_picture(server_card);
        ClientOrServer::delay(2000);
        move_twoCards();
        ClientOrServer::delay(1000);


        guessLabel=new QLineEdit(this);
        guessLabel->setGeometry(300,310,500,51);
        QFont font_line("Algerian");
        guessLabel->setStyleSheet("font : 14pt;color: rgb(255,255,255);background:rgba(0,0,0,0);border:2px solid;border-color: rgb(255,255,255);");
        guessLabel->setFont(font_line);
        guessLabel->setPlaceholderText("Enter your guess");
        for(auto& x:pushButtons)x.cards_button->setEnabled(false);
        roundNumber->hide();
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



    }
    //*************************************************************************************************
    else{
             // server has sent the playing cards to the client
        QString round = "Round "+ QString::number(currentPlayer.get_countOfTurn())+" 👻🎃";
        roundNumber->setText(round);
        roundNumber->show();
        if (currentPlayer.get_countOfTurn()>1)calculateScore();
        for(auto& x:pushButtons) delete x.cards_button;
        pushButtons.clear();
        currentPlayer.set_randomCards(recivedCard,currentPlayer.get_countOfTurn());
        showCards(currentPlayer.playeCard);
        for(int i =0 ; i<pushButtons.size();i++){
            set_picture( pushButtons[i]);
        }
        for(auto& x:pushButtons)x.cards_button->setEnabled(false);
        ui->stop->setEnabled(false);
        ui->exit->setEnabled(false);
        currentPlayer.set_countOfTurn(currentPlayer.get_countOfTurn()+1);

    }

}
//***************************************************************************
void Client::showCards(QList<cards> cCards){

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
void Client::availbleCards(buttons rivalCard){

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
//***********************************************************************************
void Client::on_Buttons0_clicked(){

    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
    for(auto& x:pushButtons)x.cards_button->setEnabled(false);
    auto it = find_if(currentPlayer.playeCard.begin(),currentPlayer.playeCard.end(),[&](auto p){
        return (p==pushButtons[0].thisCard);
    });
    if(it!=currentPlayer.playeCard.end()) currentPlayer.playeCard.erase(it);
    client_card = pushButtons[0];
    currentPlayer.set_selectedCard(client_card.thisCard);


    sendCard.push_back(pushButtons[0].thisCard);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();
    file.close();
    QMutexLocker locker(&mx2);
    if (socket->state() == QAbstractSocket::ConnectedState) {

        qint64 bytesWritten = socket->write(file_content);
        if (bytesWritten == -1) {
            QMessageBox mx;
            mx.critical(0, "Error", "Failed to send card");
        }
        socket->flush();
    } else {
        QMessageBox mx;
        mx.critical(0, "Error", "server is offline");
        return;
    }
    locker.unlock();

    currentPlayer.set_turn(false);
    move_oneCards(pushButtons[0]);
    ClientOrServer::delay(800);
    pushButtons[0].clear();

    worksForCalculateScore();


}
//**************************************************************************************************
void Client::on_Buttons1_clicked(){

    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
    for(auto& x:pushButtons)x.cards_button->setEnabled(false);
    auto it = find_if(currentPlayer.playeCard.begin(),currentPlayer.playeCard.end(),[&](auto p){
        return (p==pushButtons[1].thisCard);
    });
    if(it!=currentPlayer.playeCard.end()) currentPlayer.playeCard.erase(it);
    client_card = pushButtons[1];
    currentPlayer.set_selectedCard(client_card.thisCard);
    sendCard.push_back(pushButtons[1].thisCard);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();
    file.close();

    QMutexLocker locker(&mx2);
    if (socket->state() == QAbstractSocket::ConnectedState) {

        qint64 bytesWritten = socket->write(file_content);
        if (bytesWritten == -1) {
            QMessageBox mx;
            mx.critical(0, "Error", "Failed to send card");
        }
        socket->flush();
    } else {
        QMessageBox mx;
        mx.critical(0, "Error", "server is offline");
        return;
    }
    locker.unlock();

    currentPlayer.set_turn(false);
    move_oneCards(pushButtons[1]);
    ClientOrServer::delay(800);
    pushButtons[1].clear();
    worksForCalculateScore();


}
//**************************************************************************************************
void Client::on_Buttons2_clicked(){

    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
    for(auto& x:pushButtons)x.cards_button->setEnabled(false);
    auto it = find_if(currentPlayer.playeCard.begin(),currentPlayer.playeCard.end(),[&](auto p){
        return (p==pushButtons[2].thisCard);
    });
    if(it!=currentPlayer.playeCard.end())currentPlayer.playeCard.erase(it);
    client_card = pushButtons[2];
    currentPlayer.set_selectedCard(client_card.thisCard);


    sendCard.push_back(pushButtons[2].thisCard);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();
    file.close();

    QMutexLocker locker(&mx2);
    if (socket->state() == QAbstractSocket::ConnectedState) {

        qint64 bytesWritten = socket->write(file_content);
        if (bytesWritten == -1) {
            QMessageBox mx;
            mx.critical(0, "Error", "Failed to send card");
        }
        socket->flush();
    } else {
        QMessageBox mx;
        mx.critical(0, "Error", "server is offline");
        return;
    }
    locker.unlock();

    currentPlayer.set_turn(false);
    move_oneCards(pushButtons[2]);
    ClientOrServer::delay(800);
    pushButtons[2].clear();
    worksForCalculateScore();



}
//**************************************************************************************************
void Client::on_Buttons3_clicked(){

    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
    for(auto& x:pushButtons)x.cards_button->setEnabled(false);
    auto it = find_if(currentPlayer.playeCard.begin(),currentPlayer.playeCard.end(),[&](auto p){
      return (p==pushButtons[3].thisCard);
    });
    if(it!=currentPlayer.playeCard.end())currentPlayer.playeCard.erase(it);
    client_card = pushButtons[3];
    currentPlayer.set_selectedCard(client_card.thisCard);
    sendCard.push_back(pushButtons[3].thisCard);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();
    file.close();

    QMutexLocker locker(&mx2);
    if (socket->state() == QAbstractSocket::ConnectedState) {

        qint64 bytesWritten = socket->write(file_content);
        if (bytesWritten == -1) {
            QMessageBox mx;
            mx.critical(0, "Error", "Failed to send card");
        }
        socket->flush();
    } else {
        QMessageBox mx;
        mx.critical(0, "Error", "server is offline");
        return;
    }
    locker.unlock();

    currentPlayer.set_turn(false);
    move_oneCards(pushButtons[3]);
    ClientOrServer::delay(800);
    pushButtons[3].clear();
    worksForCalculateScore();



}
//**************************************************************************************************
void Client::on_Buttons4_clicked(){

    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
    for(auto& x:pushButtons)x.cards_button->setEnabled(false);
    auto it = find_if(currentPlayer.playeCard.begin(),currentPlayer.playeCard.end(),[&](auto p){
        return (p==pushButtons[4].thisCard);
    });
    if(it!=currentPlayer.playeCard.end())currentPlayer.playeCard.erase(it);
    client_card = pushButtons[4];
    currentPlayer.set_selectedCard(client_card.thisCard);
    sendCard.push_back(pushButtons[4].thisCard);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();
    file.close();

    QMutexLocker locker(&mx2);
    if (socket->state() == QAbstractSocket::ConnectedState) {

        qint64 bytesWritten = socket->write(file_content);
        if (bytesWritten == -1) {
            QMessageBox mx;
            mx.critical(0, "Error", "Failed to send card");
        }
        socket->flush();
    } else {
        QMessageBox mx;
        mx.critical(0, "Error", "server is offline");
        return;
    }
    locker.unlock();

    currentPlayer.set_turn(false);
    move_oneCards(pushButtons[4]);
    ClientOrServer::delay(800);
    pushButtons[4].clear();
    worksForCalculateScore();


}
//**************************************************************************************************
void Client::on_Buttons5_clicked(){

    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
    for(auto& x:pushButtons)x.cards_button->setEnabled(false);
    auto it = find_if(currentPlayer.playeCard.begin(),currentPlayer.playeCard.end(),[&](auto p){
        return (p==pushButtons[5].thisCard);
    });
    if(it!=currentPlayer.playeCard.end())currentPlayer.playeCard.erase(it);
    client_card = pushButtons[5];
    currentPlayer.set_selectedCard(client_card.thisCard);
    sendCard.push_back(pushButtons[5].thisCard);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();
    file.close();
    QMutexLocker locker(&mx2);
    if (socket->state() == QAbstractSocket::ConnectedState) {

        qint64 bytesWritten = socket->write(file_content);
        if (bytesWritten == -1) {
            QMessageBox mx;
            mx.critical(0, "Error", "Failed to send card");
        }
        socket->flush();
    } else {
        QMessageBox mx;
        mx.critical(0, "Error", "server is offline");
        return;
    }
    locker.unlock();

    currentPlayer.set_turn(false);
    move_oneCards(pushButtons[5]);
    ClientOrServer::delay(800);
    pushButtons[5].clear();
    worksForCalculateScore();



}
//**************************************************************************************************
void Client::on_Buttons6_clicked(){

    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
    for(auto& x:pushButtons)x.cards_button->setEnabled(false);
    auto it = find_if(currentPlayer.playeCard.begin(),currentPlayer.playeCard.end(),[&](auto p){
        return (p==pushButtons[6].thisCard);
    });
    if(it!=currentPlayer.playeCard.end())currentPlayer.playeCard.erase(it);
    client_card = pushButtons[6];
    currentPlayer.set_selectedCard(client_card.thisCard);
    sendCard.push_back(pushButtons[6].thisCard);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();
    file.close();

    QMutexLocker locker(&mx2);
    if (socket->state() == QAbstractSocket::ConnectedState) {

        qint64 bytesWritten = socket->write(file_content);
        if (bytesWritten == -1) {
            QMessageBox mx;
            mx.critical(0, "Error", "Failed to send card");
        }
        socket->flush();
    } else {
        QMessageBox mx;
        mx.critical(0, "Error", "server is offline");
        return;
    }
    locker.unlock();

    currentPlayer.set_turn(false);
    move_oneCards(pushButtons[6]);
    ClientOrServer::delay(800);
    pushButtons[6].clear();
    worksForCalculateScore();



}
//**************************************************************************************************
void Client::on_Buttons7_clicked(){

    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
    for(auto& x:pushButtons)x.cards_button->setEnabled(false);
    auto it = find_if(currentPlayer.playeCard.begin(),currentPlayer.playeCard.end(),[&](auto p){
        return (p==pushButtons[7].thisCard);
    });
    if(it!=currentPlayer.playeCard.end())currentPlayer.playeCard.erase(it);
    client_card = pushButtons[7];
    currentPlayer.set_selectedCard(client_card.thisCard);
    sendCard.push_back(pushButtons[7].thisCard);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();
    file.close();

    QMutexLocker locker(&mx2);
    if (socket->state() == QAbstractSocket::ConnectedState) {

        qint64 bytesWritten = socket->write(file_content);
        if (bytesWritten == -1) {
            QMessageBox mx;
            mx.critical(0, "Error", "Failed to send card");
        }
        socket->flush();
    } else {
        QMessageBox mx;
        mx.critical(0, "Error", "server is offline");
        return;
    }
    locker.unlock();

    currentPlayer.set_turn(false);
    move_oneCards(pushButtons[7]);
    ClientOrServer::delay(800);
    pushButtons[7].clear();
    worksForCalculateScore();


}
//**************************************************************************************************
void Client::on_Buttons8_clicked(){

    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
    for(auto& x:pushButtons)x.cards_button->setEnabled(false);
    auto it = find_if(currentPlayer.playeCard.begin(),currentPlayer.playeCard.end(),[&](auto p){
        return (p==pushButtons[8].thisCard);
    });
    if(it!=currentPlayer.playeCard.end())currentPlayer.playeCard.erase(it);
    client_card = pushButtons[8];
    currentPlayer.set_selectedCard(client_card.thisCard);
    sendCard.push_back(pushButtons[8].thisCard);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();
    file.close();

    QMutexLocker locker(&mx2);
    if (socket->state() == QAbstractSocket::ConnectedState) {

        qint64 bytesWritten = socket->write(file_content);
        if (bytesWritten == -1) {
            QMessageBox mx;
            mx.critical(0, "Error", "Failed to send card");
        }
        socket->flush();
    } else {
        QMessageBox mx;
        mx.critical(0, "Error", "server is offline");
        return;
    }
    locker.unlock();

    currentPlayer.set_turn(false);
    move_oneCards(pushButtons[8]);
    ClientOrServer::delay(800);
    pushButtons[8].clear();
    worksForCalculateScore();



}
//**************************************************************************************************
void Client::on_Buttons9_clicked(){

    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
    for(auto& x:pushButtons)x.cards_button->setEnabled(false);
    auto it = find_if(currentPlayer.playeCard.begin(),currentPlayer.playeCard.end(),[&](auto p){
        return (p==pushButtons[9].thisCard);
    });
    if(it!=currentPlayer.playeCard.end())currentPlayer.playeCard.erase(it);
    client_card = pushButtons[9];
    currentPlayer.set_selectedCard(client_card.thisCard);
    sendCard.push_back(pushButtons[9].thisCard);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();
    file.close();

    QMutexLocker locker(&mx2);
    if (socket->state() == QAbstractSocket::ConnectedState) {

        qint64 bytesWritten = socket->write(file_content);
        if (bytesWritten == -1) {
            QMessageBox mx;
            mx.critical(0, "Error", "Failed to send card");
        }
        socket->flush();
    } else {
        QMessageBox mx;
        mx.critical(0, "Error", "server is offline");
        return;
    }
    locker.unlock();

    currentPlayer.set_turn(false);
    move_oneCards(pushButtons[9]);
    ClientOrServer::delay(800);
    pushButtons[9].clear();
    worksForCalculateScore();


}
//**************************************************************************************************
void Client::on_Buttons10_clicked(){

    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
    for(auto& x:pushButtons)x.cards_button->setEnabled(false);
    auto it = find_if(currentPlayer.playeCard.begin(),currentPlayer.playeCard.end(),[&](auto p){
        return (p==pushButtons[10].thisCard);
    });
    if(it!=currentPlayer.playeCard.end())currentPlayer.playeCard.erase(it);
    client_card = pushButtons[10];
    currentPlayer.set_selectedCard(client_card.thisCard);
    sendCard.push_back(pushButtons[10].thisCard);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();
    file.close();

    QMutexLocker locker(&mx2);
    if (socket->state() == QAbstractSocket::ConnectedState) {

        qint64 bytesWritten = socket->write(file_content);
        if (bytesWritten == -1) {
            QMessageBox mx;
            mx.critical(0, "Error", "Failed to send card");
        }
        socket->flush();
    } else {
        QMessageBox mx;
        mx.critical(0, "Error", "server is offline");
        return;
    }
    locker.unlock();

    currentPlayer.set_turn(false);
    move_oneCards(pushButtons[10]);
    ClientOrServer::delay(800);
    pushButtons[10].clear();
    worksForCalculateScore();



}
//**************************************************************************************************
void Client::on_Buttons11_clicked(){

    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
    for(auto& x:pushButtons)x.cards_button->setEnabled(false);
    auto it = find_if(currentPlayer.playeCard.begin(),currentPlayer.playeCard.end(),[&](auto p){
        return (p==pushButtons[11].thisCard);
    });
    if(it!=currentPlayer.playeCard.end())currentPlayer.playeCard.erase(it);
    client_card = pushButtons[11];
    currentPlayer.set_selectedCard(client_card.thisCard);
    sendCard.push_back(pushButtons[11].thisCard);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();
    file.close();

    QMutexLocker locker(&mx2);
    if (socket->state() == QAbstractSocket::ConnectedState) {

        qint64 bytesWritten = socket->write(file_content);
        if (bytesWritten == -1) {
            QMessageBox mx;
            mx.critical(0, "Error", "Failed to send card");
        }
        socket->flush();
    } else {
        QMessageBox mx;
        mx.critical(0, "Error", "server is offline");
        return;
    }
    locker.unlock();

    currentPlayer.set_turn(false);
    move_oneCards(pushButtons[11]);
    ClientOrServer::delay(800);
    pushButtons[11].clear();
    worksForCalculateScore();


}
//**************************************************************************************************
void Client::on_Buttons12_clicked(){

    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
    for(auto& x:pushButtons)x.cards_button->setEnabled(false);
    auto it = find_if(currentPlayer.playeCard.begin(),currentPlayer.playeCard.end(),[&](auto p){
        return (p==pushButtons[12].thisCard);
    });
    if(it!=currentPlayer.playeCard.end())currentPlayer.playeCard.erase(it);
    client_card = pushButtons[12];
    currentPlayer.set_selectedCard(client_card.thisCard);
    sendCard.push_back(pushButtons[12].thisCard);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();
    file.close();

    QMutexLocker locker(&mx2);
    if (socket->state() == QAbstractSocket::ConnectedState) {

        qint64 bytesWritten = socket->write(file_content);
        if (bytesWritten == -1) {
            QMessageBox mx;
            mx.critical(0, "Error", "Failed to send card");
        }
        socket->flush();
    } else {
        QMessageBox mx;
        mx.critical(0, "Error", "server is offline");
        return;
    }
    locker.unlock();

    currentPlayer.set_turn(false);
    move_oneCards(pushButtons[12]);
    ClientOrServer::delay(800);
    pushButtons[12].clear();
    worksForCalculateScore();



}
//**************************************************************************************************
void Client::on_Buttons13_clicked(){

    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
    for(auto& x:pushButtons)x.cards_button->setEnabled(false);
    auto it = find_if(currentPlayer.playeCard.begin(),currentPlayer.playeCard.end(),[&](auto p){
        return (p==pushButtons[13].thisCard);
    });
    if(it!=currentPlayer.playeCard.end())currentPlayer.playeCard.erase(it);
    client_card = pushButtons[13];
    currentPlayer.set_selectedCard(client_card.thisCard);
    sendCard.push_back(pushButtons[13].thisCard);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();
    file.close();

    QMutexLocker locker(&mx2);
    if (socket->state() == QAbstractSocket::ConnectedState) {

        qint64 bytesWritten = socket->write(file_content);
        if (bytesWritten == -1) {
            QMessageBox mx;
            mx.critical(0, "Error", "Failed to send card");
        }
        socket->flush();
    } else {
        QMessageBox mx;
        mx.critical(0, "Error", "server is offline");
        return;
    }
    locker.unlock();

    currentPlayer.set_turn(false);
    move_oneCards(pushButtons[13]);
    ClientOrServer::delay(800);
    pushButtons[13].clear();

    worksForCalculateScore();

}
//**************************************************************************************************
void Client :: set_client_card(struct buttons cCard){
    client_card =cCard;
}
//********************************************************************************************
void Client :: set_server_card(struct buttons sCard){
    server_card =sCard;
}
//********************************************************************************************
struct buttons Client::get_client_card(){
    return client_card;
}
//**********************************************************************************************
struct buttons Client::get_server_card(){
    return server_card;}
//**********************************************************************************************
void Client::set_picture(struct buttons crd){

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
//**********************************************************************************************************************
QVector<buttons> Client:: get_PushPuttons(){
    return pushButtons;
}
//**********************************************************************************************************************
void Client::move_oneCards(buttons& cCards){

    cCards.cards_button->disconnect();
    cCards.cards_button->hide();
    set_picture(client_card);
    server_card.cards_button->setGeometry(660,230,181,251);
    client_card.cards_button->setGeometry(140,230,181,251);

}
//**************************************************************************************************************************
void Client::change_geometry(){

    server_card.cards_button->setGeometry(660,230,181,251);
    client_card.cards_button->setGeometry(140,230,181,251);
    for(auto& x:pushButtons)x.cards_button->setEnabled(true);

}
//**************************************************************************************************************************
void Client::move_twoCards(){

    QPropertyAnimation *animServer = new QPropertyAnimation(server_card.cards_button, "pos", this);
    QPropertyAnimation *animClient = new QPropertyAnimation(client_card.cards_button, "pos", this);
    animServer->setDuration(410);
    animServer->setEndValue(QPoint(-10, 260));
    animServer->start();
    animClient->setDuration(410);
    animClient->setEndValue(QPoint(-10, 260));
    animClient->start();

    connect(animServer,&QAbstractAnimation::finished,this,[&](){
        server_card.cards_button->hide();
        server_card.clear();
        client_card.clear();
        for(auto& x:pushButtons)x.cards_button->setEnabled(true);

        if (currentPlayer.playeCard.size() == 0) {
            if (currentPlayer.get_countOfTurn() > 7) {
                emit sendEndGame();
            }
        }
    });

    connect(animClient,&QAbstractAnimation::finished,this,[&](){
        client_card.cards_button->hide();
        server_card.clear();
        client_card.clear();
        for(auto& x:pushButtons)x.cards_button->setEnabled(true);
    });

    connect(animServer,SIGNAL(finished()),animServer,SLOT(deleteLater()));
    connect(animClient,SIGNAL(finished()),animClient,SLOT(deleteLater()));
    connect(animServer,SIGNAL(finished()),this,SLOT(change_geometry()));
}
//*******************************************************************************************************************************************
void Client::calculateScore(){
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
    //auto foundPlayer=find_if(listOfPlayer.begin(),listOfPlayer.end(),[](auto x){return(x.get_username()==currentPlayer.get_username());});
   // foundPlayer->set_score(currentPlayer.get_score());
    //writeToFile("myfile.bin");
    scoreNumber->setText(QString::number(currentPlayer.get_score()));
    sendScore();
    currentPlayer.set_setWin(0);
}
//********************************************************************************************************************
void Client::sendServerendOfGame(){

    calculateScore();
    cards client_order;
    client_order.setNumber(currentPlayer.get_score());
    client_order.setOrder("SCORE");
    sendCard.push_back(client_order);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();
    file.close();

    QMutexLocker locker(&mx2);
    if (socket->state() == QAbstractSocket::ConnectedState) {

        qint64 bytesWritten = socket->write(file_content);
        if (bytesWritten == -1) {
            QMessageBox mx;
            mx.critical(0, "Error", "Failed to send card");
        }
        socket->flush();
    } else {
        QMessageBox mx;
        mx.critical(0, "Error", "server is offline");
    }
    locker.unlock();
}
//*********************************************************************************************************************
void Client::on_resumeButton_clicked(){


    resume->setEnabled(false);
    resume->hide();
    ui->stop->setEnabled(true);
    ui->stop->show();
    timer->stop();
    timerLabel->hide();
    ui->stopGost->hide();
    ui->gameStoped->hide();

    for(auto& x:pushButtons) x.cards_button->setEnabled(true);
    cards client_order;
    client_order.setOrder("RESUME");
    sendCard.push_back(client_order);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();
    file.close();
    QMutexLocker locker(&mx2);
    if (socket->state() == QAbstractSocket::ConnectedState) {

        qint64 bytesWritten = socket->write(file_content);
        if (bytesWritten == -1) {
            QMessageBox mx;
            mx.critical(0, "Error", "Failed to send card");
        }
        socket->flush();
    }
    else {
        QMessageBox mx;
        mx.critical(0, "Error", "server is offline");
    }
    locker.unlock();
}
//******************************************************************************************************************
void Client::on_stop_clicked()
{

    ui->stop->setEnabled(false);
    ui->stop->hide();
    resume->setEnabled(true);
    resume->show();
    for(auto& x:pushButtons) x.cards_button->setEnabled(false);
    ui->stopGost->show();
    ui->gameStoped->show();

    cards client_order;
    client_order.setOrder("STOP");
    sendCard.push_back(client_order);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();
    file.close();
    QMutexLocker locker(&mx2);
    if (socket->state() == QAbstractSocket::ConnectedState) {

        qint64 bytesWritten = socket->write(file_content);
        if (bytesWritten == -1) {
            QMessageBox mx;
            mx.critical(0, "Error", "Failed to send card");
        }
        socket->flush();
    } else {
        QMessageBox mx;
        mx.critical(0, "Error", "server is offline");

    }
    locker.unlock();

    remainingTime = 60;
    timerLabel->setText(QString::number(remainingTime));
    timerLabel->show();
    timer->start(1000);
}

//*******************************************************************************************************************
void Client::on_exit_clicked()
{
    currentPlayer.set_lose(currentPlayer.get_lose()+1);
    auto it = find_if(listOfPlayer.begin(),listOfPlayer.end(),[&](auto p){
        return(currentPlayer.get_username()==p.get_username());
    });
    it->set_lose(currentPlayer.get_lose());

    writeToFile("myfile.bin");
    cards client_order;
    client_order.setOrder("EXIT");
    sendCard.push_back(client_order);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();
    file.close();

    QMutexLocker locker(&mx2);
    if (socket->state() == QAbstractSocket::ConnectedState) {

        qint64 bytesWritten = socket->write(file_content);
        if (bytesWritten == -1) {
            QMessageBox mx;
            mx.critical(0, "Error", "Failed to send card");
        }
        socket->flush();
    } else {
        QMessageBox mx;
        mx.critical(0, "Error", "server is offline");
    }
    locker.unlock();
    endTheGame("you lose");
}

// //*******************************************************************************************************************
void Client::sendName(){

    cards client_order;
    QString order=currentPlayer.get_name()+"@";
    client_order.setOrder(order);
    sendCard.push_back(client_order);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();
    file.close();

    QMutexLocker locker(&mx2);
    if (socket->state() == QAbstractSocket::ConnectedState) {

        qint64 bytesWritten = socket->write(file_content);
        if (bytesWritten == -1) {
            QMessageBox mx;
            mx.critical(0, "Error", "Failed to send card");
        }
        socket->flush();
    } else {
        QMessageBox mx;
        mx.critical(0, "Error", "server is offline");
    }
    locker.unlock();
}
//*******************************************************************************************************************
void Client::worksForCalculateScore(){

    if(!client_card.empty()&& !server_card.empty()){
        currentPlayer.calculate(server_card.thisCard);
        ClientOrServer::delay(900);
        move_twoCards();
        if(currentPlayer.playeCard.size()==0){
            ///////////////////////////////////////
            if(currentPlayer.get_countOfTurn() == 7){
                for(auto& x:pushButtons) delete x.cards_button;
                pushButtons.clear();
                ui->stop->setEnabled(false);
                ui->exit->setEnabled(false);
                ui->stop->hide();
                ui->exit->hide();
            }
            ////////////////////////////////////////////////////
            else{
                for(auto& x:pushButtons)x.cards_button->setEnabled(false);
                for(auto& x:pushButtons) delete x.cards_button;
                pushButtons.clear();
            }

        }
        sendScore();
    }
}
//*******************************************************************************************************************
void Client::sendScore(){

    cards client_order;
    QString order= currentPlayer.get_name() +"#";
    client_order.setOrder(order);
    client_order.setNumber(currentPlayer.get_score());
    sendCard.push_back(client_order);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();
    file.close();

    QMutexLocker locker(&mx2);
    if (socket->state() == QAbstractSocket::ConnectedState) {

        qint64 bytesWritten = socket->write(file_content);
        if (bytesWritten == -1) {
            QMessageBox mx;
            mx.critical(0, "Error", "Failed to send card");
        }
        socket->flush();
    } else {
        QMessageBox mx;
        mx.critical(0, "Error", "server is offline");

    }
    locker.unlock();


}
//******************************************************************************************
void Client::updateTimer() {

    remainingTime--;
    timerLabel->setText(QString::number(remainingTime));
    if (remainingTime <= 0) {
        timer->stop();
        timerLabel->hide();
        on_resumeButton_clicked();
    }

}
//******************************************************************************************
void Client::endTheGame(QString res){

    clearDataOfGame();
    ui->stop->setEnabled(false);
    ui->exit->setEnabled(false);
    ui->stop->hide();
    ui->exit->hide();
    ui->gost->hide();
    ui->background->setGeometry(0,0,1000,700);
    ui->background->setStyleSheet("border:4px solid;background-color:rgb(241,224,152);border-color :#000;");
    clientPic->show();
    serverPic->show();
    returnButton->show();
    returnButton->setEnabled(true);
    result->show();
    if(res == "you win"){
        result->setText("you win");
        crown->setGeometry(45,230,181,211);
        crown->show();
    }
    else if(res == "you lose"){
        result->setText("you lose");
        crown->setGeometry(777,223,181,211);
        crown->show();
    }
    else{

        result->setText("Equal");
    }
}
//******************************************************************************************
void Client::disconnectFromServer() {
    if (socket->state() == QAbstractSocket::ConnectedState) {
        socket->disconnectFromHost();
        if (socket->state() != QAbstractSocket::UnconnectedState) {
            socket->waitForDisconnected(2000);  // Wait for up to 3 seconds to disconnect
        }
    }
    socket->close();
    delete socket;  // If socket was dynamically allocated

}
//******************************************************************************************
void Client::clearDataOfGame(){

    writeToFile("myfile.bin");
    currentPlayer.clearDataOfGame();
    sendCard.clear();
    recivedCard.clear();
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
void Client::on_returnButton(){

    menu* newPage;
    newPage=new menu;
    this->close();
    newPage->show();
    disconnectFromServer();
    delete cln;
    newPage->showText();
}
