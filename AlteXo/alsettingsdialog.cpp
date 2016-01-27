#include "alsettingsdialog.h"
#include "ui_alsettingsdialog.h"

AlSettingsDialog::AlSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AlSettingsDialog)
{
    ui->setupUi(this);
}

AlSettingsDialog::~AlSettingsDialog()
{
    delete ui;
}
