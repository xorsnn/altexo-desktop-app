#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    QSettings settings;
    int minDepth = settings.value("altexo/minDepth", 555).toInt();
    int maxDepth = settings.value("altexo/maxDepth", 1005).toInt();

    ui->minDepthSpinBox->setValue(minDepth);
    ui->maxDepthSpinBox->setValue(maxDepth);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::on_minDepthSpinBox_valueChanged(int arg1)
{
    QSettings settings;
    settings.setValue("altexo/minDepth", arg1);
    emit this->settingsChangedSignal();
}

void SettingsDialog::on_maxDepthSpinBox_valueChanged(int arg1)
{
    QSettings settings;
    settings.setValue("altexo/maxDepth", arg1);
    emit this->settingsChangedSignal();
}
