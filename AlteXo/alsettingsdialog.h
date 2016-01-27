#ifndef ALSETTINGSDIALOG_H
#define ALSETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class AlSettingsDialog;
}

class AlSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AlSettingsDialog(QWidget *parent = 0);
    ~AlSettingsDialog();

private:
    Ui::AlSettingsDialog *ui;
};

#endif // ALSETTINGSDIALOG_H
