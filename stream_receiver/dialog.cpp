#include <QtWebKitWidgets>
#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    this->view = new QWebView(this);
    this->view->resize(640, 480);
    this->view->load(QUrl("http://127.0.0.1/streams/live-stream/altexo/"));
//    TODO make window resizable
//    this->resize(640, 480);
    this->setFixedSize(640, 480);
}

Dialog::~Dialog()
{
    delete ui;
    delete this->view;
}
