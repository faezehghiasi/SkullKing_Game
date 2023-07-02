#include "changeinformation.h"
#include "ui_changeinformation.h"
#include"player.h"
#include"signup.h"
#include"signin.h"
#include<QMessageBox>
#include"menuselection.h"

changeInformation::changeInformation(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::changeInformation)
{
    ui->setupUi(this);
}

changeInformation::~changeInformation()
{
    delete ui;
}

void changeInformation::on_change_clicked()
{
    auto it=find_if(listOfPlayer.begin(),listOfPlayer.end(),[=](Player x){return x.get_username()==currentPlayer.get_username();});
    it->set_name(ui->nameLine->text());
    it->set_username(ui->usernamLine->text());
    it->set_password(ui->passLine->text());
    it->set_address(ui->addressLine->text());
    it->set_phoneNumber(ui->phoneLine->text());
    writeToFile("myfile.bin");
    QMessageBox Mb;
    Mb.information(0,"Change Information","Change information was successful");
    MenuSelection* w;
    w=new MenuSelection;
    this->hide();
    w->show();


}

