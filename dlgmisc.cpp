#include "dlgmisc.h"

#include "mainwindow.h"
#include "ui_dlgmisc.h"
#include "ui_mainwindow.h"

extern MainWindow* mw_one;

dlgMisc::dlgMisc(QWidget* parent) : QDialog(parent), ui(new Ui::dlgMisc) {
  ui->setupUi(this);
  setWindowTitle("boot-args");
  QString strArgs1 =
      "-v -s -x -xcpm -gux_defer_usb2 -gux_no_idle -gux_nosleep "
      "-gux_nomsi -disablegfxfirmware -nehalem_error_disable ";
  QString strArgs2 =
      " npci=0x2000 npci=0x3000 rootless=0 slide=0 dart=0 cpus=1  nvda_drv=1 "
      "nv_disable=1 nv_spanmodepolicy=1 keepsyms=1 kext-dev-mode=1 debug=0x100 "
      "kextlog=0xffff ";
  QString strArgs3 =
      " darkwake=no darkwake=0 darkwake=1 darkwake=2 darkwake=3 darkwake=4 "
      "darkwake=8 darkwake=10 ";
  QString strArgsLilu =
      " -liludbg -liludbgall -liluoff -liluuseroff -liluslow -lilulowmem "
      "-lilubeta -lilubetaall -liluforce liludelay=1000 lilucpu=8 liludump=1 ";
  QString strArgsAlc = " alcid=69 -alcoff -alcdbg -alcbeta ";
  QString strArgsWG =
      " -wegdbg -wegoff -wegbeta -wegnoegpu -wegnoigpu -radvesa -igfxvesa "
      "-rad24 -raddvi radpg=15 agdpmod=vit9696 agdpmod=pikera agdpmod=ignore "
      "ngfxgl=1 ngfxcompat=1 ngfxsubmit=0 gfxrst=1 gfxrst=4 igfxframe=frame "
      "igfxsnb=0 igfxgl=0 -igfxnohdmi -cdfon -igfxdump -igfxfbdump "
      "igfxcflbklt=1 applbkl=0 -igfxmlr -igfxhdmidivs -igfxlspcon -igfxi2cdbg "
      "igfxagdc=0 igfxfcms=1 igfxfcmsfbs=0x010203 igfxonln=1 igfxonlnfbs=MASK "
      "wegtree=1 igfxmetal=1 -igfxmpc -igfxblr -igfxdvmt -igfxcdc -igfxrpsc=1 ";
  QString strArgsVsmc =
      " -vsmcdbg -vsmcoff -vsmcbeta -vsmcrpt -vsmccomp vsmcgen=X vsmchbkp=X "
      "vsmcslvl=X smcdebug=0xff watchdog=0 ";
  addBootArgs(strArgs1 + strArgs2 + strArgs3 + strArgsLilu + strArgsAlc +
              strArgsWG + strArgsVsmc);

  ui->listWidget->setCurrentRow(0);
}

dlgMisc::~dlgMisc() { delete ui; }

void dlgMisc::on_btnAdd_clicked() {
  QString str =
      ui->listWidget->item(ui->listWidget->currentRow())->text().trimmed();
  if (str.trimmed() == "") return;
  int row = mw_one->ui->table_nv_add->currentRow();
  if (mw_one->ui->table_nv_add->item(row, 0)->text().trimmed() == "boot-args") {
    mw_one->ui->table_nv_add->cellDoubleClicked(row, 2);
    QString strOrg = mw_one->lineEdit->text().trimmed();

    QStringList strList = strOrg.split(" ");
    bool re = false;
    for (int i = 0; i < strList.count(); i++) {
      QString str1 = strList.at(i);
      qDebug() << str1 << strOrg;
      if (str == str1.trimmed()) re = true;
    }

    if (!re) {
      strOrg = strOrg + " " + str;
      mw_one->lineEdit->setText(strOrg);
      mw_one->lineEditSetText();
    }
  }
}

void dlgMisc::on_listWidget_itemDoubleClicked(QListWidgetItem* item) {
  Q_UNUSED(item);
  on_btnAdd_clicked();
}

void dlgMisc::addBootArgs(QString str) {
  QStringList itemsList = str.split(" ");
  for (int i = 0; i < itemsList.count(); i++) {
    QString strItem = itemsList.at(i);
    ui->listWidget->addItem(strItem.trimmed());
  }
}
