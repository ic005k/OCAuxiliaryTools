#include "dlgMountESP.h"
#include "mainwindow.h"
#include "ui_dlgMountESP.h"

extern MainWindow* mw_one;

dlgMountESP::dlgMountESP(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::dlgMountESP)
{
    ui->setupUi(this);
}

dlgMountESP::~dlgMountESP()
{
    delete ui;
}

void dlgMountESP::on_btnMount_clicked()
{
    if (ui->listWidget->count() == 0)
        return;

    QString str = ui->listWidget->currentItem()->text().trimmed();
    QStringList strList = str.simplified().split(" ");
    mw_one->mount_esp_mac(strList.at(5));
}
