#ifndef CHANGEINFORMATION_H
#define CHANGEINFORMATION_H

#include <QMainWindow>

namespace Ui {
class changeInformation;
}

class changeInformation : public QMainWindow
{
    Q_OBJECT

public:
    explicit changeInformation(QWidget *parent = nullptr);
    ~changeInformation();
private slots:
    void on_change_clicked();

    void on_backBotton_clicked();


private:
    Ui::changeInformation *ui;
};

#endif // CHANGEINFORMATION_H
