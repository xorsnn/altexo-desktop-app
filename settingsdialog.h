#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QSettings>
#include <QDebug>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();

private:
    Ui::SettingsDialog *ui;

signals:
    void settingsChangedSignal();

private slots:
    void on_minDepthSpinBox_valueChanged(int arg1);
    void on_maxDepthSpinBox_valueChanged(int arg1);
};

#endif // SETTINGSDIALOG_H
