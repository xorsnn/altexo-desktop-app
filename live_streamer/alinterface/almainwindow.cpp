#include "almainwindow.h"
#include "ui_almainwindow.h"

#include <QDebug>
AlMainWindow::AlMainWindow(QWidget *parent) :
    QMainWindow(parent), conductor_(NULL),
    ui(new Ui::AlMainWindow)
{
    ui->setupUi(this);
}

AlMainWindow::~AlMainWindow()
{
    delete ui;
}

void AlMainWindow::on_StartButton_clicked()
{
    qDebug() << "start";
//    this->conductor_->StartAll();
    Q_EMIT this->signalStartButton_clicked();
}

void AlMainWindow::setConductor(rtc::scoped_refptr<AlConductor> conductor) {
    this->conductor_ = conductor;
}

void AlMainWindow::slotSDPText(const QString &sdp) {
    qDebug() << "AlMainWindow::slotSDPText";
    this->ui->pOfferText->setPlainText(sdp);
}

void AlMainWindow::slotOnLocalIceCandidate(const QString &iceCandidate)
{
    QString str = this->ui->pOwnICEText->toPlainText();
    str += iceCandidate + "\n";
    this->ui->pOwnICEText->setPlainText(str);
}

void AlMainWindow::on_pProcessAnswerButton_clicked()
{
    qDebug() << "onAnswerClicked";
    Q_EMIT this->signalProcessAnswerButton_clicked(this->ui->pAnswerText->toPlainText());
}

void AlMainWindow::on_pProcessRemoteICEButton_clicked()
{
    qDebug() << "onRemoteICEClicked";
    Q_EMIT signalProcessRemoteICEButton_clicked(this->ui->pRemoteICEText->toPlainText());

}
