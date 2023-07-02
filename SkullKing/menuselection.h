#ifndef MENUSELECTION_H
#define MENUSELECTION_H
#include <QMainWindow>
namespace Ui {
class MenuSelection;
}

class MenuSelection : public QMainWindow
{
    Q_OBJECT

public:
    explicit MenuSelection(QWidget *parent = nullptr);
    ~MenuSelection();

public slots:
    void startTheGame();
private slots:
    void on_start_clicked();

    void on_change_info_clicked();

private:
    Ui::MenuSelection *ui;
};
#endif // MENUSELECTION_H
