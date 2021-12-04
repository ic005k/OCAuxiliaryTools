#include "dlgdatabase.h"

#include "Method.h"
#include "mainwindow.h"
#include "ui_dlgdatabase.h"

extern QTableWidget *tableDatabase;
extern MainWindow *mw_one;
extern Method *mymethod;
extern QString SaveFileName;

dlgDatabase::dlgDatabase(QWidget *parent)
    : QDialog(parent), ui(new Ui::dlgDatabase) {
  ui->setupUi(this);

  ui->editPing->setHidden(true);
  ui->btnRefreshAll->setHidden(true);

  processPing = new QProcess;
  connect(processPing, SIGNAL(readyReadStandardOutput()), this,
          SLOT(on_readoutput()));
  connect(processPing, SIGNAL(readyReadStandardError()), this,
          SLOT(on_readerror()));
  QPalette pl = ui->editPing->palette();
  pl.setColor(QPalette::Base, Qt::black);
  pl.setColor(QPalette::Text, Qt::green);
  // ui->editPing->setPalette(pl);
  ui->editPing->setReadOnly(true);

  ui->editFind->setClearButtonEnabled(true);

  tableDatabase = ui->tableDatabase;
  tableDatabase->setEditTriggers(QAbstractItemView::NoEditTriggers);
  ui->tableDatabaseFind->setEditTriggers(QAbstractItemView::NoEditTriggers);

  QTableWidgetItem *id0;

  ui->tableDatabase->setColumnWidth(0, 520);
  id0 = new QTableWidgetItem(tr("Config Database"));
  ui->tableDatabase->setHorizontalHeaderItem(0, id0);

  ui->tableDatabase->setAlternatingRowColors(true);
  tableDatabase->horizontalHeader()->setStretchLastSection(
      true);  //设置充满表宽度
  ui->tableDatabaseFind->horizontalHeader()->setStretchLastSection(true);
  tableDatabase->horizontalHeader()->setHidden(true);
  ui->tableDatabaseFind->horizontalHeader()->setHidden(true);
  ui->tableDatabaseFind->setHidden(true);

  tableDatabase->setSelectionBehavior(
      QAbstractItemView::SelectRows);  //设置选择行为时每次选择一行
  ui->tabWidget->setCurrentIndex(0);

  ui->tableKextUrl->setColumnWidth(0, 200);
  ui->tableKextUrl->setColumnWidth(1, 400);
  ui->textEdit->setHidden(true);

  QString qfile = QDir::homePath() + "/.config/QtOCC/QtOCC.ini";
  QSettings Reg(qfile, QSettings::IniFormat);
  QFileInfo fi(qfile);
  QString strDef = "https://ghproxy.com/https://github.com/";
  QLocale locale;
  if (fi.exists()) {
    if (locale.language() == QLocale::Chinese) {
      ui->comboBoxNet->setCurrentText(Reg.value("Net", strDef).toString());
    } else {
      ui->comboBoxNet->setCurrentText(
          Reg.value("Net", "https://github.com/").toString());
    }

    ui->comboBoxWeb->setCurrentText(
        Reg.value("Web", "https://github.com/").toString());
    ui->rbtnAPI->setChecked(Reg.value("rbtnAPI").toBool());
    ui->rbtnWeb->setChecked(Reg.value("rbtnWeb").toBool());
    ui->chkBoxLastFile->setChecked(Reg.value("LastFile").toBool());

  } else {
    if (locale.language() == QLocale::Chinese) {
      ui->comboBoxNet->setCurrentText(strDef);

    } else {
      ui->comboBoxNet->setCurrentText("https://github.com/");
    }
  }

  ui->chkRecentOpen->setChecked(Reg.value("chkRecentOpen", 0).toBool());
  ui->chkOpenDir->setChecked(Reg.value("chkOpenDir", 0).toBool());
  ui->chkMountESP->setChecked(Reg.value("chkMountESP", 1).toBool());
  ui->chkBackupEFI->setChecked(Reg.value("chkBackupEFI", 1).toBool());
  ui->chkDatabase->setChecked(Reg.value("chkDatabase", 1).toBool());
}

dlgDatabase::~dlgDatabase() { delete ui; }
void dlgDatabase::closeEvent(QCloseEvent *event) {
  Q_UNUSED(event);
  saveKextUrl();
  processPing->kill();
  ui->btnPing->setText(tr("Testing"));
}

void dlgDatabase::keyPressEvent(QKeyEvent *event) {
  switch (event->key()) {
    case Qt::Key_Escape:
      close();
      break;

    case Qt::Key_Return:

      break;

    case Qt::Key_Backspace:

      break;

    case Qt::Key_Space:

      break;

    case Qt::Key_F1:

      break;
  }

  if (event->modifiers() == Qt::ControlModifier) {
    if (event->key() == Qt::Key_M) {
      this->setWindowState(Qt::WindowMaximized);
    }
  }
}

void dlgDatabase::on_tableDatabase_cellDoubleClicked(int row, int column) {
  Q_UNUSED(row);
  Q_UNUSED(column);
  mw_one->RefreshAllDatabase = true;

  QFileInfo appInfo(qApp->applicationDirPath());
  QString dirpath = appInfo.filePath() + "/Database/BaseConfigs/";
  QString file = tableDatabase->currentItem()->text();
  mw_one->openFile(dirpath + file);
  close();

  mw_one->RefreshAllDatabase = false;

  mymethod->generateEFI();
}

void dlgDatabase::on_btnFind_clicked() {
  QString text = ui->editFind->text().trimmed();
  if (text == "") return;

  tableDatabase->setFocus();
  tableDatabase->setSelectionMode(QAbstractItemView::MultiSelection);
  tableDatabase->clearSelection();
  ui->tableDatabaseFind->setRowCount(0);
  int count = 0;
  for (int i = 0; i < tableDatabase->rowCount(); i++) {
    tableDatabase->setCurrentCell(i, 0);
    QString str = tableDatabase->currentItem()->text();
    QFileInfo fi(str);
    if (fi.baseName().toLower().contains(text.toLower())) {
      ui->tableDatabaseFind->setRowCount(count + 1);
      QTableWidgetItem *newItem1;
      newItem1 = new QTableWidgetItem(str);
      ui->tableDatabaseFind->setItem(count, 0, newItem1);

      count++;

    } else {
      tableDatabase->selectRow(i);
    }
  }

  if (count > 0)
    ui->tableDatabaseFind->setHidden(false);
  else
    ui->tableDatabaseFind->setHidden(true);

  ui->editFind->setFocus();

  ui->lblCount->setText(QString::number(count));

  tableDatabase->setSelectionMode(QAbstractItemView::ExtendedSelection);
}

void dlgDatabase::on_editFind_textChanged(const QString &arg1) {
  if (arg1 == "") {
    ui->lblCount->setText("0");
    ui->tableDatabaseFind->setHidden(true);
    return;
  }

  on_btnFind_clicked();
}

void dlgDatabase::on_editFind_returnPressed() { on_btnFind_clicked(); }

void dlgDatabase::on_tableDatabaseFind_cellDoubleClicked(int row, int column) {
  Q_UNUSED(row);
  Q_UNUSED(column);

  mw_one->RefreshAllDatabase = true;

  QFileInfo appInfo(qApp->applicationDirPath());

  QString dirpath = appInfo.filePath() + "/Database/BaseConfigs/";
  QString file = ui->tableDatabaseFind->currentItem()->text();
  mw_one->openFile(dirpath + file);
  close();

  mw_one->RefreshAllDatabase = false;

  mymethod->generateEFI();
}

void dlgDatabase::on_btnRefreshAll_clicked() {
  ui->btnRefreshAll->setEnabled(false);
  this->repaint();

  mw_one->RefreshAllDatabase = true;

  QString bakFile;
  if (!SaveFileName.isEmpty()) {
    bakFile = SaveFileName;
  }

  ui->tableDatabase->setFocus();
  ui->tableDatabase->setCurrentCell(0, 0);

  QFileInfo appInfo(qApp->applicationDirPath());
  QString dirpath = appInfo.filePath() + "/Database/BaseConfigs/";

  for (int i = 0; i < ui->tableDatabase->rowCount(); i++) {
    ui->tableDatabase->setCurrentCell(i, 0);
    QString file = ui->tableDatabase->currentItem()->text();
    mw_one->openFile(dirpath + file);
    mw_one->SavePlist(dirpath + file);
  }

  if (!bakFile.isEmpty()) mw_one->openFile(bakFile);

  mw_one->RefreshAllDatabase = false;

  ui->btnRefreshAll->setEnabled(true);
  this->repaint();
}

void dlgDatabase::refreshKextUrl() {
  ui->textEdit->clear();
  ui->textEdit->append("Lilu.kext | https://github.com/acidanthera/Lilu");
  ui->textEdit->append(
      "AppleALC.kext | https://github.com/acidanthera/AppleALC");
  ui->textEdit->append(
      "VoodooPS2Controller.kext | https://github.com/acidanthera/VoodooPS2");
  ui->textEdit->append(
      "WhateverGreen.kext | https://github.com/acidanthera/WhateverGreen");
  ui->textEdit->append(
      "VirtualSMC.kext | https://github.com/acidanthera/VirtualSMC");
  ui->textEdit->append(
      "VoodooI2C.kext | https://github.com/VoodooI2C/VoodooI2C");
  ui->textEdit->append(
      "RestrictEvents.kext | https://github.com/acidanthera/RestrictEvents");
  ui->textEdit->append(
      "HibernationFixup.kext | "
      "https://github.com/acidanthera/HibernationFixup");
  ui->textEdit->append(
      "BrcmPatchRAM.kext | https://github.com/acidanthera/BrcmPatchRAM");
  ui->textEdit->append(
      "CpuTscSync.kext | https://github.com/acidanthera/CpuTscSync");
  ui->textEdit->append(
      "BrightnessKeys.kext | https://github.com/acidanthera/BrightnessKeys");
  ui->textEdit->append(
      "AirportBrcmFixup.kext | "
      "https://github.com/acidanthera/AirportBrcmFixup");
  ui->textEdit->append(
      "RTCMemoryFixup.kext | https://github.com/acidanthera/RTCMemoryFixup");
  ui->textEdit->append(
      "MacHyperVSupport.kext | "
      "https://github.com/acidanthera/MacHyperVSupport");
  ui->textEdit->append("NVMeFix.kext | https://github.com/acidanthera/NVMeFix");
  ui->textEdit->append(
      "RealtekCardReader.kext | "
      "https://github.com/0xFireWolf/RealtekCardReader");
  ui->textEdit->append(
      "RealtekCardReaderFriend.kext | "
      "https://github.com/0xFireWolf/RealtekCardReaderFriend");
  ui->textEdit->append(
      "ECEnabler.kext | https://github.com/1Revenger1/ECEnabler");
  ui->textEdit->append(
      "RealtekRTL8111.kext | https://github.com/Mieze/RTL8111_driver_for_OS_X");
  ui->textEdit->append(
      "CPUFriend.kext | https://github.com/acidanthera/CPUFriend");
  ui->textEdit->append(
      "VoodooInput.kext | https://github.com/acidanthera/VoodooInput");
  ui->textEdit->append(
      "LucyRTL8125Ethernet.kext | "
      "https://github.com/Mieze/LucyRTL8125Ethernet");
  ui->textEdit->append("Innie.kext | https://github.com/cdf/Innie");
  ui->textEdit->append(
      "AtherosE2200Ethernet.kext | "
      "https://github.com/Mieze/AtherosE2200Ethernet");
  ui->textEdit->append(
      "DebugEnhancer.kext | https://github.com/acidanthera/DebugEnhancer");
  ui->textEdit->append(
      "IntelMausi.kext | https://github.com/acidanthera/IntelMausi");
  ui->textEdit->append(
      "ALC256.kext | "
      "https://github.com/ic005k/ALC256");
  ui->textEdit->append(
      "NightShiftEnabler.kext | https://github.com/cdf/NightShiftEnabler");
  ui->textEdit->append(
      "NoTouchID.kext | https://github.com/al3xtjames/NoTouchID");
  ui->textEdit->append(
      "USBInjectAll.kext | https://github.com/johnlimabravo/USBInjectAll");
  ui->textEdit->append(
      "RadeonSensor.kext | https://github.com/aluveitie/RadeonSensor");
  ui->textEdit->append(
      "FakeSMC.kext | "
      "https://github.com/CloverHackyColor/FakeSMC3_with_plugins");
  // ui->textEdit->append(" | ");
  // ui->textEdit->append(" | ");

  QTextEdit *txtEdit = new QTextEdit;
  QString txt = mymethod->loadText(mw_one->strConfigPath + "KextUrl.txt");
  txtEdit->setPlainText(txt);
  for (int i = 0; i < txtEdit->document()->lineCount(); i++) {
    QString line = mymethod->getTextEditLineText(txtEdit, i).trimmed();
    bool re = false;
    for (int j = 0; j < ui->textEdit->document()->lineCount(); j++) {
      QString line2 = mymethod->getTextEditLineText(ui->textEdit, j).trimmed();
      if (line == line2) {
        re = true;
      }
    }
    if (!re) ui->textEdit->append(line);
  }

  ui->tableKextUrl->setRowCount(0);
  for (int i = 0; i < ui->textEdit->document()->lineCount(); i++) {
    QStringList list =
        mymethod->getTextEditLineText(ui->textEdit, i).split("|");
    QString str0, str1;
    if (list.count() == 2) {
      str0 = list.at(0);
      str1 = list.at(1);
      int n = ui->tableKextUrl->rowCount();
      ui->tableKextUrl->setRowCount(n + 1);
      ui->tableKextUrl->setCurrentCell(i, 0);
      ui->tableKextUrl->setItem(i, 0, new QTableWidgetItem(str0.trimmed()));
      ui->tableKextUrl->setItem(i, 1, new QTableWidgetItem(str1.trimmed()));
    }
  }

  ui->tableKextUrl->setFocus();
}

void dlgDatabase::on_btnAdd_clicked() {
  int n = ui->tableKextUrl->rowCount();
  ui->tableKextUrl->setRowCount(n + 1);
  ui->tableKextUrl->setCurrentCell(n, 0);
  ui->tableKextUrl->setItem(n, 0, new QTableWidgetItem(""));
  ui->tableKextUrl->setItem(n, 1, new QTableWidgetItem(""));
}

void dlgDatabase::on_btnDel_clicked() {
  if (ui->tableKextUrl->rowCount() == 0) return;
  int n = ui->tableKextUrl->currentRow();
  ui->tableKextUrl->removeRow(n);
  if (n - 1 == -1) n = 1;
  ui->tableKextUrl->setCurrentCell(n - 1, 0);
  ui->tableKextUrl->setFocus();
}

void dlgDatabase::saveKextUrl() {
  ui->textEdit->clear();
  QString str0, str1;
  for (int i = 0; i < ui->tableKextUrl->rowCount(); i++) {
    str0 = ui->tableKextUrl->item(i, 0)->text().trimmed();
    str1 = ui->tableKextUrl->item(i, 1)->text().trimmed();
    if (str0 != "" || str1 != "") ui->textEdit->append(str0 + " | " + str1);
  }
  mymethod->TextEditToFile(ui->textEdit, mw_one->strConfigPath + "KextUrl.txt");
}

void dlgDatabase::on_btnTest_clicked() {
  mw_one->on_actionOnline_Download_Updates_triggered();
  mw_one->dlgAutoUpdate->ui->btnStartUpdate->setHidden(true);
}

void dlgDatabase::on_comboBoxNet_currentTextChanged(const QString &arg1) {
  QString qfile = QDir::homePath() + "/.config/QtOCC/QtOCC.ini";
  QSettings Reg(qfile, QSettings::IniFormat);
  Reg.setValue("Net", arg1);
}

void dlgDatabase::on_btnOpenUrl_clicked() {
  if (!ui->tableKextUrl->currentIndex().isValid()) return;
  int n = ui->tableKextUrl->currentRow();
  QString strurl = ui->tableKextUrl->item(n, 1)->text().trimmed();
  QUrl url(strurl);
  QDesktopServices::openUrl(url);
}

void dlgDatabase::on_btnPing_clicked() {
  QString url = ui->comboBoxWeb->currentText().trimmed();
  QUrl urlTest(url);
  QDesktopServices::openUrl(urlTest);
  return;

  if (ui->btnPing->text() == tr("Testing"))
    ui->btnPing->setText(tr("Stop"));
  else if (ui->btnPing->text() == tr("Stop"))
    ui->btnPing->setText(tr("Testing"));

  QStringList list = url.split("/");
  QString s1;
  if (list.count() == 4) s1 = list.at(2);
  processPing->kill();
  if (mw_one->win) {
    processPing->start("ping", QStringList() << "-t" << s1);
  } else
    processPing->start("ping", QStringList() << s1);
}

void dlgDatabase::on_readoutput() {
  QString str = processPing->readAllStandardOutput();
  ui->editPing->append(str);
}

void dlgDatabase::on_readerror() {
  QMessageBox::information(0, "Error", processPing->readAllStandardError());
}

void dlgDatabase::on_comboBoxWeb_currentTextChanged(const QString &arg1) {
  QString qfile = QDir::homePath() + "/.config/QtOCC/QtOCC.ini";
  QSettings Reg(qfile, QSettings::IniFormat);
  Reg.setValue("Web", arg1);
}

void dlgDatabase::on_rbtnAPI_clicked() {
  QString qfile = QDir::homePath() + "/.config/QtOCC/QtOCC.ini";
  QSettings Reg(qfile, QSettings::IniFormat);
  Reg.setValue("rbtnAPI", ui->rbtnAPI->isChecked());
  Reg.setValue("rbtnWeb", ui->rbtnWeb->isChecked());
}

void dlgDatabase::on_rbtnWeb_clicked() { on_rbtnAPI_clicked(); }

void dlgDatabase::on_chkBoxLastFile_clicked(bool checked) {
  QString qfile = QDir::homePath() + "/.config/QtOCC/QtOCC.ini";
  QSettings Reg(qfile, QSettings::IniFormat);
  Reg.setValue("LastFile", checked);
}

void dlgDatabase::writeIni(QString key, int arg1) {
  QString qfile = QDir::homePath() + "/.config/QtOCC/QtOCC.ini";
  QSettings Reg(qfile, QSettings::IniFormat);
  Reg.setValue(key, arg1);
}

void dlgDatabase::on_chkOpenDir_stateChanged(int arg1) {
  writeIni("chkOpenDir", arg1);
}

void dlgDatabase::on_chkRecentOpen_stateChanged(int arg1) {
  writeIni("chkRecentOpen", arg1);
}

void dlgDatabase::on_chkMountESP_stateChanged(int arg1) {
  writeIni("chkMountESP", arg1);
}

void dlgDatabase::on_chkBackupEFI_stateChanged(int arg1) {
  writeIni("chkBackupEFI", arg1);
}

void dlgDatabase::on_chkDatabase_stateChanged(int arg1) {
  writeIni("chkDatabase", arg1);
}
