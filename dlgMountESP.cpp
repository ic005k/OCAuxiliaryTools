#include "dlgMountESP.h"

#include "mainwindow.h"
#include "ui_dlgMountESP.h"

extern MainWindow* mw_one;

dlgMountESP::dlgMountESP(QWidget* parent)
    : QDialog(parent), ui(new Ui::dlgMountESP) {
  ui->setupUi(this);
  ui->btnMountOpenConfig->setDefault(true);
}

dlgMountESP::~dlgMountESP() { delete ui; }

void dlgMountESP::on_btnMount_clicked() { mountESP(false); }

void dlgMountESP::on_btnMountOpenConfig_clicked() { mountESP(true); }

void dlgMountESP::mountESP(bool openConfig) {
  if (ui->listWidget->count() == 0) return;

  QString str = ui->listWidget->currentItem()->text().trimmed();
  QStringList strList = str.simplified().split(" ");
  mw_one->mount_esp_mac(strList.at(5));

  QString str0 = strList.at(2);
  QString str1 = str0.toLatin1();
  QString str2 = str1.replace("?", "");
  QString dirpath = "/Volumes/" + str2 + "/EFI/";
  QString dir = "file:" + dirpath;

  if (openConfig) {
    QString strConfig = dirpath + "OC/Config.plist";
    QFileInfo fi(strConfig.toLower());
    if (fi.exists()) mw_one->openFile(strConfig);
  } else if (QDesktopServices::openUrl(QUrl(dir, QUrl::TolerantMode))) {
  }

  close();
}
