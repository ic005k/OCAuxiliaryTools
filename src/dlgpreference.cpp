#include "dlgpreference.h"

#include <QDir>
#include <QKeyEvent>
#include <QSettings>

#include "Method.h"
#include "mainwindow.h"
#include "ui_dlgpreference.h"
#include "ui_mainwindow.h"

extern MainWindow *mw_one;
extern Method *mymethod;
extern QString SaveFileName, strIniFile, strAppName, ocFromDev, strOCFromDev,
    strAppExePath;
extern bool blDEV;
extern QProgressBar *progBar;
extern int red;
extern QSettings Reg;

dlgPreference::dlgPreference(QWidget *parent)
    : QDialog(parent), ui(new Ui::dlgPreference) {
  ui->setupUi(this);

  Method::init_UIWidget(this, red);
  for (int i = 0; i < ui->tableKextUrl->columnCount(); i++) {
    ui->tableKextUrl->horizontalHeader()->setSectionResizeMode(
        i, QHeaderView::ResizeToContents);
  }

  // Kexts Urls
  ui->tableKextUrl->setAlternatingRowColors(true);
  ui->tableKextUrl->horizontalHeader()->setStretchLastSection(true);
  ui->tableKextUrl->setColumnWidth(0, 200);
  ui->tableKextUrl->setColumnWidth(1, 400);
  ui->textEdit->setHidden(true);

  QString strDef = "https://gh.flyinbug.top/gh/https://github.com/";
  QLocale locale;
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
  ui->rbtnToken->setChecked(Reg.value("rbtnToken").toBool());
  ui->editToken->setText(Reg.value("editToken").toString());

  ui->chkBoxLastFile->setChecked(Reg.value("LastFile").toBool());

  ui->chkShowVolName->setChecked(Reg.value("ShowVolName", 0).toBool());

  ui->chkRecentOpen->setChecked(Reg.value("chkRecentOpen", 1).toBool());
  ui->chkOpenDir->setChecked(Reg.value("chkOpenDir", 1).toBool());
  ui->chkMountESP->setChecked(Reg.value("chkMountESP", 1).toBool());
  ui->chkBackupEFI->setChecked(Reg.value("chkBackupEFI", 1).toBool());
  ui->chkDatabase->setChecked(Reg.value("chkDatabase", 1).toBool());
  ui->chkHideToolbar->setChecked(Reg.value("chkHideToolbar", 0).toBool());
  ui->chkHoverTips->setChecked(Reg.value("HoverTips", 0).toBool());
  ui->chkTabIndent->setChecked(Reg.value("TabIndent", 0).toBool());
  ui->chkProxy->setChecked(Reg.value("Proxy", 0).toBool());
  ui->txtHostName->setText(Reg.value("HostName", "127.0.0.1").toString());
  ui->txtPort->setText(Reg.value("Port", "38457").toString());

  ui->rbtnWeb->hide();
  ui->chkProxy->setChecked(false);
  ui->tabWidget->setCurrentIndex(0);
}

dlgPreference::~dlgPreference() { delete ui; }

void dlgPreference::closeEvent(QCloseEvent *event) {
  if (!ui->btnDownloadKexts->isEnabled()) event->ignore();
  ui->myeditFind->clear();
  saveKextUrl();
  Reg.setValue("Proxy", ui->chkProxy->isChecked());
  Reg.setValue("HostName", ui->txtHostName->text().trimmed());
  Reg.setValue("Port", ui->txtPort->text().trimmed());
  Reg.setValue("HoverTips", ui->chkHoverTips->isChecked());
  Reg.setValue("TabIndent", ui->chkTabIndent->isChecked());

  Reg.setValue("rbtnAPI", ui->rbtnAPI->isChecked());
  Reg.setValue("rbtnWeb", ui->rbtnWeb->isChecked());
  Reg.setValue("rbtnToken", ui->rbtnToken->isChecked());
  Reg.setValue("editToken", ui->editToken->text());
}

void dlgPreference::keyPressEvent(QKeyEvent *event) {
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

void dlgPreference::saveKextUrl() {
  ui->textEdit->clear();
  QString str0, str1;
  for (int i = 0; i < ui->tableKextUrl->rowCount(); i++) {
    str0 = ui->tableKextUrl->item(i, 0)->text().trimmed();
    str1 = ui->tableKextUrl->item(i, 1)->text().trimmed();
    if (str0 != "" || str1 != "") ui->textEdit->append(str0 + " | " + str1);
  }
  mymethod->TextEditToFile(ui->textEdit, mw_one->strConfigPath + "KextUrl.txt");
}

void dlgPreference::refreshKextUrl(bool writeTable) {
  isRefresh = true;
  // strAppExePath app exe path
  // mw_one->strConfigPath .config path
  QString file = strAppExePath + "/Database/preset/KextUrl.txt";
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

  listKexts.clear();
  for (int i = 0; i < ui->textEdit->document()->lineCount(); i++) {
    QString line = mymethod->getTextEditLineText(ui->textEdit, i).trimmed();
    if (line != "") listKexts.append(line);
  }

  std::sort(listKexts.begin(), listKexts.end(),
            [](const QString &s1, const QString &s2) { return s1 < s2; });

  if (writeTable) {
    ui->tableKextUrl->setRowCount(0);
    ui->tableKextUrl->setRowCount(listKexts.count());
  }
  listFind.clear();
  for (int i = 0; i < listKexts.count(); i++) {
    QString line = listKexts.at(i);
    QStringList list = line.split("|");
    QString str0, str1;
    if (list.count() == 2) {
      str0 = list.at(0);
      str1 = list.at(1);
      if (writeTable) {
        ui->tableKextUrl->setCurrentCell(i, 0);
        ui->tableKextUrl->setItem(i, 0, new QTableWidgetItem(str0.trimmed()));
        ui->tableKextUrl->setItem(i, 1, new QTableWidgetItem(str1.trimmed()));
      }
      listFind.append(str0.trimmed() + "|" + str1.trimmed());
    }
  }
  ui->tableKextUrl->setFocus();
  isRefresh = false;
}

void dlgPreference::on_btnAdd_clicked() {
  isRefresh = true;
  int n = ui->tableKextUrl->rowCount();
  ui->tableKextUrl->setRowCount(n + 1);
  ui->tableKextUrl->setCurrentCell(n, 0);
  ui->tableKextUrl->setItem(n, 0, new QTableWidgetItem(""));
  ui->tableKextUrl->setItem(n, 1, new QTableWidgetItem(""));
  ui->tableKextUrl->setFocus();
  isRefresh = false;
}

void dlgPreference::on_btnDel_clicked() {
  if (ui->tableKextUrl->rowCount() == 0) return;
  int n = ui->tableKextUrl->currentRow();
  QString str = ui->tableKextUrl->item(n, 0)->text().trimmed() + "|" +
                ui->tableKextUrl->item(n, 1)->text().trimmed();
  listFind.removeOne(str);
  ui->tableKextUrl->removeRow(n);
  if (n - 1 == -1) n = 1;
  ui->tableKextUrl->setCurrentCell(n - 1, 0);
  ui->tableKextUrl->setFocus();
}

void dlgPreference::on_btnOpenUrl_clicked() {
  if (!ui->tableKextUrl->currentIndex().isValid()) return;
  int n = ui->tableKextUrl->currentRow();
  QString strurl = ui->tableKextUrl->item(n, 1)->text().trimmed();
  QUrl url(strurl);
  QDesktopServices::openUrl(url);
}

void dlgPreference::on_rbtnAPI_clicked() {
  QSettings Reg(strIniFile, QSettings::IniFormat);
  Reg.setValue("rbtnAPI", ui->rbtnAPI->isChecked());
  Reg.setValue("rbtnWeb", ui->rbtnWeb->isChecked());
  Reg.setValue("rbtnToken", ui->rbtnToken->isChecked());
  Reg.setValue("editToken", ui->editToken->text());
}

void dlgPreference::on_rbtnWeb_clicked() { on_rbtnAPI_clicked(); }

void dlgPreference::on_rbtnToken_clicked() { on_rbtnAPI_clicked(); }

void dlgPreference::on_btnTest_clicked() {
  mw_one->on_actionOnline_Download_Updates_triggered();
  mw_one->dlgAutoUpdate->ui->btnStartUpdate->setHidden(true);
}

void dlgPreference::on_comboBoxNet_currentTextChanged(const QString &arg1) {
  QSettings Reg(strIniFile, QSettings::IniFormat);
  Reg.setValue("Net", arg1);
}

void dlgPreference::on_comboBoxWeb_currentTextChanged(const QString &arg1) {
  QSettings Reg(strIniFile, QSettings::IniFormat);
  Reg.setValue("Web", arg1);
}

void dlgPreference::writeIni(QString key, int arg1) {
  QSettings Reg(strIniFile, QSettings::IniFormat);
  Reg.setValue(key, arg1);
}

void dlgPreference::on_chkBoxLastFile_clicked(bool checked) {
  QSettings Reg(strIniFile, QSettings::IniFormat);
  Reg.setValue("LastFile", checked);
}

void dlgPreference::on_chkOpenDir_stateChanged(int arg1) {
  writeIni("chkOpenDir", arg1);
}

void dlgPreference::on_chkRecentOpen_stateChanged(int arg1) {
  writeIni("chkRecentOpen", arg1);
}

void dlgPreference::on_chkMountESP_stateChanged(int arg1) {
  writeIni("chkMountESP", arg1);
}

void dlgPreference::on_chkBackupEFI_stateChanged(int arg1) {
  writeIni("chkBackupEFI", arg1);
}

void dlgPreference::on_chkDatabase_stateChanged(int arg1) {
  writeIni("chkDatabase", arg1);
}

void dlgPreference::on_chkHideToolbar_stateChanged(int arg1) {
  writeIni("chkHideToolbar", arg1);
}

void dlgPreference::on_chkShowVolName_clicked(bool checked) {
  QSettings Reg(strIniFile, QSettings::IniFormat);
  Reg.setValue("ShowVolName", checked);
}

void dlgPreference::on_btnPing_clicked() {
  QString strurl = ui->comboBoxWeb->currentText();
  QUrl url(strurl);
  QDesktopServices::openUrl(url);
}

void dlgPreference::on_chkSmartKey_clicked(bool checked) {
  QSettings Reg(strIniFile, QSettings::IniFormat);
  Reg.setValue("SmartKey", checked);
}

void dlgPreference::on_btnDownloadKexts_clicked() {
  if (!mw_one->dlgSyncOC->ui->btnCheckUpdate->isEnabled()) return;
  if (!mw_one->dlgSyncOC->ui->btnGetOC->isEnabled()) return;
  ui->btnDownloadKexts->setEnabled(false);
  ui->myeditFind->setEnabled(false);
  repaint();

  progBar = new QProgressBar(this);
  progBar->setTextVisible(false);
  progBar->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  progBar->setStyleSheet(
      "QProgressBar{border:0px solid #FFFFFF;"
      "height:30;"
      "background:rgba(25,255,25,0);"
      "text-align:right;"
      "color:rgb(255,255,255);"
      "border-radius:0px;}"

      "QProgressBar:chunk{"
      "border-radius:0px;"
      "background-color:rgba(25,255,0,100);"
      "}");

  mymethod->downloadAllKexts();

  for (int i = 0; i < ui->tableKextUrl->rowCount(); i++)
    ui->tableKextUrl->removeCellWidget(i, 1);
  ui->btnDownloadKexts->setEnabled(true);
  ui->myeditFind->setEnabled(true);
  repaint();
}

void dlgPreference::on_btnStop_clicked() {
  if (ui->btnDownloadKexts->isEnabled()) return;

  mymethod->cancelKextUpdate();
}

void dlgPreference::find(QString arg1) {
  isRefresh = true;
  QStringList list;
  for (int i = 0; i < listFind.count(); i++) {
    QString str = listFind.at(i);
    if (str.toLower().contains(arg1.toLower())) {
      list.append(str.trimmed());
    }
  }

  ui->tableKextUrl->setRowCount(list.count());
  for (int i = 0; i < list.count(); i++) {
    QString str = list.at(i);
    str = str.trimmed();
    QStringList list0 = str.split("|");
    QString str1, str2;
    str1 = list0.at(0);
    str2 = list0.at(1);
    ui->tableKextUrl->setItem(i, 0, new QTableWidgetItem(str1.trimmed()));
    ui->tableKextUrl->setItem(i, 1, new QTableWidgetItem(str2.trimmed()));
  }

  if (arg1.trimmed() == "") {
    writeTable(listFind);
  }
  isRefresh = false;
}

void dlgPreference::writeTable(QStringList listFind) {
  ui->tableKextUrl->setRowCount(listFind.count());
  for (int i = 0; i < listFind.count(); i++) {
    QString str = listFind.at(i);
    str = str.trimmed();
    QStringList list0 = str.split("|");
    QString str1, str2;
    str1 = list0.at(0);
    str2 = list0.at(1);
    ui->tableKextUrl->setItem(i, 0, new QTableWidgetItem(str1.trimmed()));
    ui->tableKextUrl->setItem(i, 1, new QTableWidgetItem(str2.trimmed()));
  }
}

void dlgPreference::on_myeditFind_textChanged(const QString &arg1) {
  find(arg1);
}

void dlgPreference::on_tableKextUrl_itemChanged(QTableWidgetItem *item) {
  Q_UNUSED(item);
  if (isRefresh) return;

  saveKextUrl();
  refreshKextUrl(false);
}

void dlgPreference::on_myeditFind_returnPressed() {
  find(ui->myeditFind->text().trimmed());
  ui->myeditFind->selectAll();
}

void dlgPreference::on_btnTokenHelp_clicked() {
  QUrl url_en(QString(
      "https://www.howtogeek.com/devops/"
      "how-to-set-up-https-personal-access-tokens-for-github-authentication/"));
  QDesktopServices::openUrl(url_en);
}
