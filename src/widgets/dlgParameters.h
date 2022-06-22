#ifndef DLGPARAMETERS_H
#define DLGPARAMETERS_H

#include <QDialog>

namespace Ui {
class dlgParameters;
}

class dlgParameters : public QDialog {
    Q_OBJECT

public:
    explicit dlgParameters(QWidget* parent = nullptr);
    ~dlgParameters();
    Ui::dlgParameters* ui;

private slots:
    void slotDisplayLevel();
    void slotScanPolicy();

    void slotPickerAttributes();

    void slotExposeSensitiveData();

private:
};

#endif // DLGPARAMETERS_H
