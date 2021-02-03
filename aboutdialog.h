#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDateTime>
#include <QDialog>
#include <QFileInfo>

namespace Ui {
class aboutDialog;
}

class aboutDialog : public QDialog {
    Q_OBJECT

public:
    explicit aboutDialog(QWidget* parent = nullptr);
    ~aboutDialog();

private slots:

private:
    Ui::aboutDialog* ui;
};

#endif // ABOUTDIALOG_H
