#ifndef SEARCHFAILEDDIALOG_H
#define SEARCHFAILEDDIALOG_H

#include <QDialog>

namespace Ui {
class SearchFailedDialog;
}

class SearchFailedDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SearchFailedDialog(QWidget *parent = 0);
    ~SearchFailedDialog();

private slots:
    void on_confirmButton_clicked();

private:
    Ui::SearchFailedDialog *ui;
};

#endif // SEARCHFAILEDDIALOG_H
