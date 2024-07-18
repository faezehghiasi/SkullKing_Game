#ifndef SLOWTYPER_H
#define SLOWTYPER_H
#include <QApplication>
#include <QTextBrowser>
#include <QThread>

class SlowTyper : public QThread {
    Q_OBJECT

public:
    SlowTyper(QTextBrowser *browser, const QString &text, int interval = 100, QObject *parent = nullptr)
        : QThread(parent), browser(browser), text(text), interval(interval) {}

protected:
    void run() override {
        for (int i = 0; i < text.size(); ++i) {
            QMetaObject::invokeMethod(browser, "insertPlainText", Q_ARG(QString, QString(text[i])));
            QThread::msleep(interval);
        }
    }

private:
    QTextBrowser *browser;
    QString text;
    int interval;
};

#endif // SLOWTYPER_H
