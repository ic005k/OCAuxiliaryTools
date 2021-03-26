#ifndef DLGOCVALIDATE_H
#define DLGOCVALIDATE_H

#include <QAction>
#include <QClipboard>
#include <QDialog>
#include <QMenu>

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

private:
    Ui::dlgOCValidate* ui;
};

#endif // DLGOCVALIDATE_H
