#include "dlgdatabase.h"

#include "Method.h"
#include "mainwindow.h"
#include "ui_dlgdatabase.h"

extern MainWindow *mw_one;
extern Method *mymethod;
extern QString SaveFileName;
extern bool blDEV;

dlgDatabase::dlgDatabase(QWidget *parent)
    : QDialog(parent), ui(new Ui::dlgDatabase) {
  ui->setupUi(this);

  processPing = new QProcess;
  connect(processPing, SIGNAL(readyReadStandardOutput()), this,
          SLOT(on_readoutput()));
  connect(processPing, SIGNAL(readyReadStandardError()), this,
          SLOT(on_readerror()));

  ui->editFind->setClearButtonEnabled(true);

  // tableDatabase->setEditTriggers(QAbstractItemView::NoEditTriggers);

  QTableWidgetItem *id0;
  ui->tableDatabase->setColumnCount(2);
  ui->tableDatabase->setColumnWidth(0, 400);
  id0 = new QTableWidgetItem(tr("Configs"));
  ui->tableDatabase->setHorizontalHeaderItem(0, id0);
  ui->tableDatabase->setColumnWidth(1, 220);
  id0 = new QTableWidgetItem(tr("Comment"));
  ui->tableDatabase->setHorizontalHeaderItem(1, id0);

  ui->tableDatabase->setAlternatingRowColors(true);
  ui->tableDatabase->horizontalHeader()->setStretchLastSection(
      false);  //设置充满表宽度
  ui->tableDatabase->horizontalHeader()->setHidden(false);
  ui->tableDatabase->setSelectionBehavior(
      QAbstractItemView::SelectItems);  //设置选择行为时每次选择一行或单个条目

  for (int i = 0; i < ui->tableDatabase->columnCount(); i++) {
    ui->tableDatabase->horizontalHeader()->setSectionResizeMode(
        i, QHeaderView::ResizeToContents);
  }

  for (int i = 0; i < ui->tableKextUrl->columnCount(); i++) {
    ui->tableKextUrl->horizontalHeader()->setSectionResizeMode(
        i, QHeaderView::ResizeToContents);
  }

  // Find Table
  ui->tableDatabaseFind->horizontalHeader()->setHidden(true);
  ui->tableDatabaseFind->setHidden(true);
  ui->tableDatabaseFind->horizontalHeader()->setStretchLastSection(false);
  ui->tableDatabaseFind->setEditTriggers(QAbstractItemView::NoEditTriggers);
  ui->tableDatabaseFind->horizontalHeader()->setSectionResizeMode(
      0, QHeaderView::ResizeToContents);

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
  ui->chkShowVolName->setChecked(Reg.value("ShowVolName", 0).toBool());

  ui->chkRecentOpen->setChecked(Reg.value("chkRecentOpen", 0).toBool());
  ui->chkOpenDir->setChecked(Reg.value("chkOpenDir", 0).toBool());
  ui->chkMountESP->setChecked(Reg.value("chkMountESP", 1).toBool());
  ui->chkBackupEFI->setChecked(Reg.value("chkBackupEFI", 1).toBool());
  ui->chkDatabase->setChecked(Reg.value("chkDatabase", 1).toBool());
  ui->chkHideToolbar->setChecked(Reg.value("chkHideToolbar", 0).toBool());
}

dlgDatabase::~dlgDatabase() { delete ui; }
void dlgDatabase::closeEvent(QCloseEvent *event) {
  Q_UNUSED(event);
  saveKextUrl();
  processPing->kill();
  ui->btnPing->setText(tr("Testing"));

  QFileInfo appInfo(qApp->applicationDirPath());
  QString dirpath = appInfo.filePath() + "/Database/BaseConfigs/";
  for (int i = 0; i < ui->tableDatabase->rowCount(); i++) {
    QString plistFile =
        dirpath + ui->tableDatabase->item(i, 0)->text().trimmed();
    if (listItemModi.at(i))
      mymethod->writePlistComment(
          plistFile, ui->tableDatabase->item(i, 1)->text().trimmed());
  }
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
  if (column != 0) return;
  mw_one->RefreshAllDatabase = true;

  QFileInfo appInfo(qApp->applicationDirPath());
  QString dirpath = appInfo.filePath() + "/Database/BaseConfigs/";
  QString file = ui->tableDatabase->item(row, 0)->text();

  if (blDEV) {
    if (file == "SampleCustom.plist" || file == "Sample.plist") {
      dirpath = appInfo.filePath() + "/devDatabase/BaseConfigs/";
    }
  }
  mw_one->openFile(dirpath + file);
  close();

  mw_one->RefreshAllDatabase = false;

  mw_one->on_actionGenerateEFI_triggered();
}

void dlgDatabase::on_btnFind_clicked() {
  QString text = ui->editFind->text().trimmed();
  if (text == "") return;

  ui->tableDatabase->setFocus();
  // tableDatabase->setSelectionMode(QAbstractItemView::MultiSelection);
  // tableDatabase->clearSelection();
  ui->tableDatabaseFind->setRowCount(0);
  int count = 0;
  for (int i = 0; i < ui->tableDatabase->rowCount(); i++) {
    QString str = ui->tableDatabase->item(i, 0)->text();
    QString str1 = str;
    str1.replace(".plist", "");

    if (str1.toLower().contains(text.toLower())) {
      ui->tableDatabaseFind->setRowCount(count + 1);
      QTableWidgetItem *newItem1;
      newItem1 = new QTableWidgetItem(str);
      ui->tableDatabaseFind->setItem(count, 0, newItem1);

      count++;

    } else {
      // tableDatabase->selectRow(i);
    }
  }

  if (count > 0)
    ui->tableDatabaseFind->setHidden(false);
  else
    ui->tableDatabaseFind->setHidden(true);

  ui->editFind->setFocus();

  ui->lblCount->setText(QString::number(count));

  // tableDatabase->setSelectionMode(QAbstractItemView::ExtendedSelection);
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

  if (blDEV) {
    if (file == "SampleCustom.plist" || file == "Sample.plist") {
      dirpath = appInfo.filePath() + "/devDatabase/BaseConfigs/";
    }
  }

  mw_one->openFile(dirpath + file);
  close();

  mw_one->RefreshAllDatabase = false;

  mymethod->generateEFI();
}

void dlgDatabase::refreshKextUrl() {
  QString file = mw_one->strAppExePath + "/Database/preset/KextUrl.txt";
  ui->textEdit->clear();
  ui->textEdit->setPlainText(mymethod->loadText(file));

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
  // QString str = processPing->readAllStandardOutput();
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

void dlgDatabase::on_tableDatabase_itemChanged(QTableWidgetItem *item) {
  listItemModi.removeAt(item->row());
  listItemModi.insert(item->row(), true);
}

void dlgDatabase::on_chkHideToolbar_stateChanged(int arg1) {
  writeIni("chkHideToolbar", arg1);
}

void dlgDatabase::on_chkShowVolName_clicked(bool checked) {
  QString qfile = QDir::homePath() + "/.config/QtOCC/QtOCC.ini";
  QSettings Reg(qfile, QSettings::IniFormat);
  Reg.setValue("ShowVolName", checked);
}

void dlgDatabase::on_tableDatabase_itemDoubleClicked(QTableWidgetItem *item) {
  Q_UNUSED(item);
}

void dlgDatabase::on_btnIntel_clicked() {
  QString qtManulFile =
      mw_one->strAppExePath + "/Database/BaseConfigs/Instructions_Intel.md";
  QDesktopServices::openUrl(QUrl::fromLocalFile(qtManulFile));
}

void dlgDatabase::on_btnAMD_clicked() {
  QString qtManulFile =
      mw_one->strAppExePath + "/Database/BaseConfigs/Instructions_AMD_TRX40.md";
  QDesktopServices::openUrl(QUrl::fromLocalFile(qtManulFile));
}

void dlgDatabase::on_btnOpenDir_clicked() {
  QString dirpath = mw_one->strAppExePath + "/Database/BaseConfigs/";
  QString dir = "file:" + dirpath;
  QDesktopServices::openUrl(QUrl(dir, QUrl::TolerantMode));
}

void dlgDatabase::on_btnIntelOnline_clicked() {
  QUrl url(
      QString("https://github.com/ic005k/QtOpenCoreConfig/blob/master/Database/"
              "BaseConfigs/Instructions_Intel.md"));
  QDesktopServices::openUrl(url);
}

void dlgDatabase::on_btnAMDOnline_clicked() {
  QUrl url(
      QString("https://github.com/ic005k/QtOpenCoreConfig/blob/master/Database/"
              "BaseConfigs/Instructions_AMD_TRX40.md"));
  QDesktopServices::openUrl(url);
}
