#include "dlgMountESP.h"

#include "Method.h"
#include "mainwindow.h"
#include "ui_dlgMountESP.h"

extern MainWindow* mw_one;
extern Method* mymethod;
extern QString strIniFile;

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

  QSettings Reg(strIniFile, QSettings::IniFormat);
  Reg.setValue("mesp", ui->listWidget->currentRow());

  QString str = ui->listWidget->currentItem()->text().trimmed();
  QStringList strList = str.simplified().split("|");
  QString strDisk;
  if (strList.count() > 0) strDisk = strList.at(0);
  strDisk = strDisk.trimmed();
  mymethod->mount_esp_mac(strDisk);

  QString str0 = strList.at(1);
  QString str1 = str0.toLatin1();
  QString str2 = str1.replace("?", "");
  QString dirpath = "/Volumes/" + str2.trimmed() + "/EFI/";
  qDebug() << dirpath;
  QString dir = "file:" + dirpath;

  if (openConfig) {
    QString strConfig = dirpath + "OC/config.plist";
    QFileInfo fi(strConfig.toLower());
    if (fi.exists()) mw_one->openFile(strConfig);
  } else if (QDesktopServices::openUrl(QUrl(dir, QUrl::TolerantMode))) {
  }

  close();
  delete this;
}
