#ifndef MENU_H
#define MENU_H

#include <QDialog>

namespace Ui {
class menu;
}

class menu : public QDialog
{
    Q_OBJECT

public:
    explicit menu(QWidget *parent = nullptr);
    ~menu();
    void showText();
private slots:
    void on_start_clicked();

    void on_histoty_clicked();

    void on_exit_clicked();

    void on_change_info_clicked();

private:
    Ui::menu *ui;
};

#endif // MENU_H
