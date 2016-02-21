#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    qDebug() << "set 1";
    ui->setupUi(this);
    qDebug() << "set 2";
    QSettings settings;
    int minDepth = settings.value("altexo/minDepth", 555).toInt();
    int maxDepth = settings.value("altexo/maxDepth", 1005).toInt();
    QString wsLink = settings.value("altexo/wsLink", "ws://altexo.com:8889/ws").toString();

    ui->minDepthSpinBox->setValue(minDepth);
    ui->maxDepthSpinBox->setValue(maxDepth);
    ui->wsEdit->setText(wsLink);
}

SettingsDialog::~SettingsDialog()
{
    qDebug() << "set 3";
    delete ui;
    qDebug() << "set 4";
}

void SettingsDialog::on_minDepthSpinBox_valueChanged(int arg1)
{
    QSettings settings;
    settings.setValue("altexo/minDepth", arg1);
    Q_EMIT this->settingsChangedSignal();
}

void SettingsDialog::on_maxDepthSpinBox_valueChanged(int arg1)
{
    QSettings settings;
    settings.setValue("altexo/maxDepth", arg1);
    Q_EMIT this->settingsChangedSignal();
}


void SettingsDialog::on_wsEdit_textChanged(const QString &arg1)
{
    QSettings settings;
    settings.setValue("altexo/wsLink", arg1);
    Q_EMIT this->settingsChangedSignal();
}
