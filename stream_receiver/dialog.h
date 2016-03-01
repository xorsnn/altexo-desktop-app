#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QtWidgets>

class QWebView;

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private:
    Ui::Dialog *ui;
    QWebView *view;
};

#endif // DIALOG_H
