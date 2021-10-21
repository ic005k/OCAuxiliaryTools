#include "dlgmisc.h"

#include "mainwindow.h"
#include "ui_dlgmisc.h"
#include "ui_mainwindow.h"

extern MainWindow* mw_one;

dlgMisc::dlgMisc(QWidget* parent) : QDialog(parent), ui(new Ui::dlgMisc) {
  ui->setupUi(this);
  setWindowTitle("boot-args");
  QStringList itemsList;
  itemsList << "-v"
            << "keepsyms=1"
            << "-s"
            << "-x"
            << "npci=0x2000"
            << "slide=0"
            << "cpus=1"
            << "dart=0"
            << "-xcpm"
            << "darkwake=no"
            << "nvda_drv=1"
            << "nv_disable=1"
            << "kext-dev-mode=1"
            << "rootless=0"
            << "-disablegfxfirmware"
            << "-liludbg"
            << "-liludbgall"
            << "-liluoff"
            << "-liluuseroff"
            << "-liluslow"
            << "-lilulowmem"
            << "-lilubeta"
            << "-lilubetaall"
            << "-liluforce"
            << "liludelay=1000"
            << "lilucpu=8"
            << "liludump=N";
  ui->listWidget->addItems(itemsList);
  ui->listWidget->setCurrentRow(0);
}

dlgMisc::~dlgMisc() { delete ui; }

void dlgMisc::on_btnAdd_clicked() {
  int row = mw_one->ui->table_nv_add->currentRow();
  if (mw_one->ui->table_nv_add->item(row, 0)->text().trimmed() == "boot-args") {
    QString str =
        ui->listWidget->item(ui->listWidget->currentRow())->text().trimmed();
    QString strOrg = mw_one->ui->table_nv_add->item(row, 2)->text().trimmed();
    QStringList strList = strOrg.split(" ");
    bool re = false;
    for (int i = 0; i < strList.count(); i++) {
      QString str1 = strList.at(i);
      if (str == str1.trimmed()) re = true;
    }

    if (!re) {
      strOrg = strOrg + " " + str;
      mw_one->ui->table_nv_add->cellDoubleClicked(row, 2);
      mw_one->lineEdit->setText(strOrg);
    }
  }
}

void dlgMisc::on_listWidget_itemDoubleClicked(QListWidgetItem* item) {
  Q_UNUSED(item);
  on_btnAdd_clicked();
}
