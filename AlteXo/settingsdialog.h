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

Q_SIGNALS:
    void settingsChangedSignal();

private Q_SLOTS:
    void on_minDepthSpinBox_valueChanged(int arg1);
    void on_maxDepthSpinBox_valueChanged(int arg1);
    void on_wsEdit_textChanged(const QString &arg1);
};

#endif // SETTINGSDIALOG_H
