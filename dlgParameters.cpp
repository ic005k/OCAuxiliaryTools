#include "dlgParameters.h"
#include "mainwindow.h"
#include "ui_dlgParameters.h"

extern MainWindow* mw_one;
extern QVector<QCheckBox*> chkDisplayLevel;
extern QVector<QCheckBox*> chk_ScanPolicy;

dlgParameters::dlgParameters(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::dlgParameters)
{
    ui->setupUi(this);

    chkDisplayLevel.clear();
    chkDisplayLevel.append(ui->chkD1);
    chkDisplayLevel.append(ui->chkD2);
    chkDisplayLevel.append(ui->chkD3);
    chkDisplayLevel.append(ui->chkD4);
    chkDisplayLevel.append(ui->chkD5);
    chkDisplayLevel.append(ui->chkD6);
    chkDisplayLevel.append(ui->chkD7);
    chkDisplayLevel.append(ui->chkD8);
    chkDisplayLevel.append(ui->chkD9);
    chkDisplayLevel.append(ui->chkD10);
    chkDisplayLevel.append(ui->chkD11);
    chkDisplayLevel.append(ui->chkD12);
    chkDisplayLevel.append(ui->chkD13);
    chkDisplayLevel.append(ui->chkD14);
    chkDisplayLevel.append(ui->chkD15);
    chkDisplayLevel.append(ui->chkD16);
    chkDisplayLevel.append(ui->chkD17);
    chkDisplayLevel.append(ui->chkD18);
    chkDisplayLevel.append(ui->chkD19);

    for (int i = 0; i < chkDisplayLevel.count(); i++) {
        connect(chkDisplayLevel.at(i), &QCheckBox::clicked, this, &dlgParameters::slotDisplayLevel);
    }

    chk_ScanPolicy.clear();
    chk_ScanPolicy.append(ui->chk1);
    chk_ScanPolicy.append(ui->chk2);
    chk_ScanPolicy.append(ui->chk3);
    chk_ScanPolicy.append(ui->chk4);
    chk_ScanPolicy.append(ui->chk5);
    chk_ScanPolicy.append(ui->chk6);
    chk_ScanPolicy.append(ui->chk7);
    chk_ScanPolicy.append(ui->chk8);
    chk_ScanPolicy.append(ui->chk9);
    chk_ScanPolicy.append(ui->chk10);
    chk_ScanPolicy.append(ui->chk11);
    chk_ScanPolicy.append(ui->chk12);
    chk_ScanPolicy.append(ui->chk13);
    chk_ScanPolicy.append(ui->chk14);
    chk_ScanPolicy.append(ui->chk15);
    chk_ScanPolicy.append(ui->chk16);

    for (int i = 0; i < chk_ScanPolicy.count(); i++) {
        connect(chk_ScanPolicy.at(i), &QCheckBox::clicked, this, &dlgParameters::slotScanPolicy);
    }
}

dlgParameters::~dlgParameters()
{
    delete ui;
}

void dlgParameters::slotDisplayLevel()
{
    mw_one->DisplayLevel();
}

void dlgParameters::slotScanPolicy()
{
    mw_one->ScanPolicy();
}
