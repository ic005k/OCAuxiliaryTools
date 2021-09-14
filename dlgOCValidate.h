#ifndef DLGOCVALIDATE_H
#define DLGOCVALIDATE_H

#include <QAction>
#include <QClipboard>
#include <QDebug>
#include <QDialog>
#include <QMenu>
#include <QMessageBox>
#include <QTimer>

namespace Ui {
class dlgOCValidate;
}

class dlgOCValidate : public QDialog {
    Q_OBJECT

public:
    explicit dlgOCValidate(QWidget* parent = nullptr);
    ~dlgOCValidate();
    void setTextOCV(QString str);
    void setGoEnabled(bool enabled);

private slots:
    void on_btnClose_clicked();

    void on_btnGo_clicked();

private:
    Ui::dlgOCValidate* ui;
};

#endif // DLGOCVALIDATE_H
