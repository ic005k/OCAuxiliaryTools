#include "syncocdialog.h"

#include "Method.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_syncocdialog.h"

extern MainWindow* mw_one;
extern QString ocVer, ocVerDev, ocFrom, ocFromDev, strIniFile, strAppName,
    SaveFileName, strAppExePath, strOCFrom;
extern bool blDEV;
extern bool Initialization;
extern Method* mymethod;
extern int red;
QProgressBar* progBar;
extern QSettings Reg;

SyncOCDialog::SyncOCDialog(QWidget* parent)
    : QDialog(parent), ui(new Ui::SyncOCDialog) {
  ui->setupUi(this);

  ui->lblInfo->hide();
  ui->lblInfo->setWordWrap(true);
  QString str =
      "QLabel{background-color:rgb(255,25,25); color:rgb(255,255,255)}";
  ui->lblInfo->setStyleSheet(str);
  progInfo = new QProgressBar(this);
  progInfo->hide();
  QString txt = "https://dortania.github.io/builds/";
  QString kextsDevFrom = "<a href=\"" + txt + "\"" + "> " + tr(" Source ");
  ui->lblKextDevFrom->setText(kextsDevFrom);

  Method::init_UIWidget(this, red);
  QFont font;
  font.setBold(true);
  ui->lblKexts->setFont(font);
  ui->lblOpenCore->setFont(font);

  ui->lblResourcesFrom->setText(
      "<a href=\"https://github.com/acidanthera/OcBinaryData/\">" +
      tr("Source"));

  QString listStyleMain =
      "QListWidget{outline:0px;}"
      "QListWidget::item:selected{background:rgb(0,124,221); border:0px "
      "blue;margin:0px,0px,0px,0px;border-radius:5;"
      "color:white}";
  ui->listOpenCore->setStyleSheet(listStyleMain);
  ui->listOpenCore->setAlternatingRowColors(true);
  ui->listOpenCore->setIconSize(QSize(16, 16));
  // ui->listOpenCore->setGridSize(QSize(24, 24));

  ui->btnStartSync->setDefault(true);
  ui->labelShowDLInfo->setVisible(false);
  ui->labelShowDLInfo->setText("");
  ui->btnUpdate->setEnabled(false);

  ui->listOpenCore->setViewMode(QListView::ListMode);
  ui->listOpenCore->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

  ui->tableKexts->setEditTriggers(QAbstractItemView::NoEditTriggers);
  ui->tableKexts->horizontalHeader()->setStretchLastSection(true);
  ui->tableKexts->setAlternatingRowColors(true);
  ui->tableKexts->setSelectionMode(QAbstractItemView::SingleSelection);
  ui->tableKexts->setSelectionBehavior(QAbstractItemView::SelectRows);
  for (int i = 0; i < ui->tableKexts->columnCount(); i++) {
    ui->tableKexts->horizontalHeader()->setSectionResizeMode(
        i, QHeaderView::ResizeToContents);
  }

  ui->comboOCVersions->addItems(QStringList() << tr("Latest Version") << "0.7.8"
                                              << "0.7.7"
                                              << "0.7.6"
                                              << "0.7.5"
                                              << "0.7.4"
                                              << "0.7.3"
                                              << "0.7.2"
                                              << "0.7.1"
                                              << "0.7.0"
                                              << "0.6.9"
                                              << "0.6.8"
                                              << "0.6.7"
                                              << "0.6.6"
                                              << "0.6.5"
                                              << "0.6.4"
                                              << "0.6.3"
                                              << "0.6.2"

  );
  ui->comboOCVersions->clear();
  ui->editOCDevSource->lineEdit()->setText(
      Reg.value("DevSource", "https://github.com/dortania/build-repo")
          .toString());
  ui->chkIncludeResource->setChecked(Reg.value("IncludeResource", 1).toBool());
  ui->chkKextsDev->setChecked(Reg.value("KextsDev", 0).toBool());

  mgr = new QNetworkAccessManager(this);
  connect(mgr, SIGNAL(finished(QNetworkReply*)), this,
          SLOT(query(QNetworkReply*)));
}

SyncOCDialog::~SyncOCDialog() { delete ui; }

void SyncOCDialog::on_btnStartSync_clicked() {
  if (!ui->btnCheckUpdate->isEnabled() || !ui->btnGetOC->isEnabled() ||
      !dlEnd) {
    QMessageBox box;
    box.setText(
        tr("Kexts update check or OpenCore database upgrade is in progress, "
           "please wait for it to finish."));
    box.exec();
    return;
  }

  int chkCount = 0;
  for (int i = 0; i < ui->listOpenCore->count(); i++) {
    if (ui->listOpenCore->item(i)->checkState() == Qt::Checked) {
      chkCount++;
    }
  }
  if (chkCount == 0) return;

  bool ok = true;
  // Kexts
  for (int i = 0; i < sourceKexts.count(); i++) {
    QString strSou = sourceKexts.at(i);
    QString strTar = targetKexts.at(i);
    QString strSV, strTV;
    strSV = mymethod->getKextVersion(strSou);
    strTV = mymethod->getKextVersion(strTar);
    if (QDir(strSou).exists()) {
      if (chkList.at(i)->isChecked()) {
        if (strSV >= strTV || strTV == "None") {
          mw_one->copyDirectoryFiles(strSou, strTar, true);
        }
      }
    }
  }

  // OpenCore
  for (int i = 0; i < sourceOpenCore.count(); i++) {
    QString strSou = sourceOpenCore.at(i);
    QString strTar = targetOpenCore.at(i);
    if (QFile(strSou).exists()) {
      if (ui->listOpenCore->item(i)->checkState() == Qt::Checked) {
        QFile::remove(strTar);
        ok = QFile::copy(strSou, strTar);
      }
    }
  }

  if (ui->chkIncludeResource->isChecked())
    mw_one->copyDirectoryFiles(sourceResourcesDir, targetResourcesDir, true);

  QMessageBox box;
  if (ok) {
    close();
    if (!blDEV)
      box.setText(tr("Successfully synced to OpenCore: ") + ocVer + "        " +
                  ocFrom);
    else
      box.setText(tr("Successfully synced to OpenCore: ") + ocVerDev +
                  "        " + ocFromDev);
    box.exec();

    mw_one->checkFiles(mw_one->ui->table_kernel_add);
    mw_one->checkFiles(mw_one->ui->table_uefi_drivers);
    mw_one->checkFiles(mw_one->ui->tableTools);
  }
}

bool SyncOCDialog::eventFilter(QObject* o, QEvent* e) {
  if (o == lblVer && (e->type() == QEvent::MouseButtonPress ||
                      e->type() == QEvent::MouseButtonRelease)) {
    e->accept();
    return true;
  }

  return QWidget::eventFilter(o, e);
}

void SyncOCDialog::on_listOpenCore_itemClicked(QListWidgetItem* item) {
  Q_UNUSED(item);
}

void SyncOCDialog::init_ItemColor() {
  int row = ui->listOpenCore->currentRow();
  if (row < 0) return;
  for (int i = 0; i < ui->listOpenCore->count(); i++) {
    ui->listOpenCore->setCurrentRow(i);
  }
  ui->listOpenCore->setCurrentRow(row);
}

void SyncOCDialog::setListWidgetStyle() {
  QString fileName = sourceOpenCore.at(ui->listOpenCore->currentRow());
  if (mymethod->isWhatFile(fileName, "efi")) {
    if (red > 55)
      setListWidgetColor("#FFF8DC");
    else
      setListWidgetColor("#3d243a");
  }
  if (mymethod->isWhatFile(fileName, "efi") && fileName.contains("/Tools/")) {
    if (red > 55)
      setListWidgetColor("#FFEFDB");
    else
      setListWidgetColor("#12270c");
  }
  if (mymethod->isWhatFile(fileName, "efi") && fileName.contains("/Drivers/")) {
    if (red > 55)
      setListWidgetColor("#E6E6FA");
    else
      setListWidgetColor("#24293d");
  }
}

void SyncOCDialog::setListWidgetColor(QString color) {
  if (red > 55)
    ui->listOpenCore->item(ui->listOpenCore->currentRow())
        ->setForeground(QBrush(Qt::black));
  else
    ui->listOpenCore->item(ui->listOpenCore->currentRow())
        ->setForeground(QBrush(Qt::white));
  ui->listOpenCore->item(ui->listOpenCore->currentRow())
      ->setBackground(QBrush(QColor(color)));
}

void SyncOCDialog::on_listOpenCore_currentRowChanged(int currentRow) {
  if (currentRow < 0) return;

  setListWidgetStyle();

  QString sourceModi, targetModi, sourceFile, targetFile, sourceHash,
      targetHash;

  sourceFile = sourceOpenCore.at(currentRow);
  targetFile = targetOpenCore.at(currentRow);

  if (mymethod->isKext(sourceFile))
    sourceHash = mw_one->getMD5(mymethod->getKextBin(sourceFile));
  else
    sourceHash = mw_one->getMD5(sourceFile);
  if (mymethod->isKext(targetFile))
    targetHash = mw_one->getMD5(mymethod->getKextBin(targetFile));
  else
    targetHash = mw_one->getMD5(targetFile);

  QFileInfo fiSource(sourceFile);
  QFileInfo fiTarget(targetFile);
  sourceModi = fiSource.lastModified().toString();
  targetModi = fiTarget.lastModified().toString();

  QString strShowFileName;
  strShowFileName = fiSource.fileName();
  ui->lblShowInfo->setText(strShowFileName + "\n" + tr("Current File: ") +
                           "md5    " + targetHash + "    " +
                           tr("Available File: ") + "md5    " + sourceHash);

  if (sourceHash != targetHash) {
    ui->listOpenCore->item(currentRow)->setIcon(QIcon(":/icon/no.png"));

  } else {
    ui->listOpenCore->item(currentRow)->setIcon(QIcon(":/icon/ok.png"));
  }
}

void SyncOCDialog::closeEvent(QCloseEvent* event) {
  if (!ui->btnCheckUpdate->isEnabled()) event->ignore();
  if (!dlEnd) event->ignore();

  writeCheckStateINI();
  Reg.setValue("IncludeResource", ui->chkIncludeResource->isChecked());
  Reg.setValue("KextsDev", ui->chkKextsDev->isChecked());
  QString txt = ui->editOCDevSource->lineEdit()->text().trimmed();
  Reg.setValue("DevSource", txt);
  if (txt != "")
    ocFromDev = "<a href=\"" + txt + "\"" + "> " + tr(" Source ");
  else
    ocFromDev = "";
  if (blDEV) mw_one->dlgSyncOC->ui->lblOCFrom->setText(ocFromDev);

  saveWindowsPos();
}

void SyncOCDialog::saveWindowsPos() {
  Reg.setValue("sync-x", this->x());
  Reg.setValue("sync-y", this->y());
  Reg.setValue("sync-width", this->width());
  Reg.setValue("sync-height", this->height());
}

void SyncOCDialog::resizeWindowsPos() {
  // Resize Sync Windows
  int x, y, w, h;
  x = Reg.value("sync-x", "0").toInt();
  y = Reg.value("sync-y", "0").toInt();
  w = Reg.value("sync-width", "900").toInt();
  h = Reg.value("sync-height", "500").toInt();
  if (x < 0) {
    w = w + x;
    x = 0;
  }
  if (y < 0) {
    h = h + y;
    y = 0;
  }
  QRect rect(x, y, w, h);
  move(rect.topLeft());
  resize(rect.size());
}

void SyncOCDialog::writeCheckStateINI() {
  QString qfile = mw_one->strConfigPath + "chk.ini";
  // QString strTag = QDir::fromNativeSeparators(SaveFileName);
  QString strTag = SaveFileName;
  strTag.replace("/", "-");
  QString str_0;
  QSettings Reg(qfile, QSettings::IniFormat);
  for (int i = 0; i < sourceKexts.count(); i++) {
    str_0 = ui->tableKexts->item(i, 3)->text().trimmed();
    Reg.setValue(strTag + str_0, chkList.at(i)->isChecked());
  }

  // OpenCore
  for (int i = 0; i < ui->listOpenCore->count(); i++) {
    Reg.setValue(strTag + ui->listOpenCore->item(i)->text().trimmed(),
                 ui->listOpenCore->item(i)->checkState());
  }
}

void SyncOCDialog::keyPressEvent(QKeyEvent* event) {
  switch (event->key()) {
    case Qt::Key_Escape:
      if (!ui->btnCheckUpdate->isEnabled())
        return;
      else
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
}

void SyncOCDialog::on_btnCheckUpdate_clicked() {
  if (!ui->btnGetOC->isEnabled()) {
    QMessageBox box;
    box.setText(
        tr("The OpenCore database upgrade is in progress, please wait for it "
           "to finish."));
    box.exec();
    return;
  }

  if (sourceKexts.count() == 0) return;

  ui->btnUpdate->setEnabled(false);
  repaint();

  mymethod->blBreak = false;
  if (ui->chkKextsDev->isChecked() ||
      mw_one->myDlgPreference->ui->rbtnWeb->isChecked())
    progInfo->setGeometry(ui->btnCheckUpdate->x(), ui->btnCheckUpdate->y(),
                          ui->btnCheckUpdate->width(),
                          ui->btnCheckUpdate->height());

  if (ui->chkKextsDev->isChecked()) {
    getKextHtmlInfo(getJsonUrl(), false);
  }

  if (mymethod->blBreak) return;

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

  ui->labelShowDLInfo->setVisible(false);

  mymethod->kextUpdate();

  QString Current, Available;
  QString curMd5, avaMd5;

  for (int i = 0; i < sourceKexts.count(); i++) {
    QString sourceFile = sourceKexts.at(i);
    QString targetFile = targetKexts.at(i);

    if (chkList.at(i)->isChecked()) {
      Available = mymethod->getKextVersion(sourceFile);
      Current = mymethod->getKextVersion(targetFile);
      avaMd5 = mw_one->getMD5(mymethod->getKextBin(sourceFile));
      curMd5 = mw_one->getMD5(mymethod->getKextBin(targetFile));
      if ((curMd5 != avaMd5 || Current == "None") &&
          QDir(sourceFile).exists()) {
        ui->btnUpdate->setEnabled(true);
        repaint();
      }
    }
  }

  writeCheckStateINI();
  init_Sync_OC_Table();

  for (int i = 0; i < ui->tableKexts->rowCount(); i++)
    ui->tableKexts->removeCellWidget(i, 3);

  ui->tableKexts->setFocus();

  ui->btnCheckUpdate->setEnabled(true);
}

QString SyncOCDialog::getJsonUrl() {
  QString strMirror =
      mw_one->myDlgPreference->ui->comboBoxNet->currentText().trimmed();
  if (strMirror == "https://download.fastgit.org/" ||
      strMirror == "https://archive.fastgit.org/")
    strMirror = "https://gh.flyinbug.top/gh/https://github.com/";
  strMirror.replace("https://github.com/", "");
  QString url = strMirror +
                "https://raw.githubusercontent.com/dortania/build-repo/builds/"
                "config.json";
  qDebug() << "json url=" << url;
  return url;
}

void SyncOCDialog::on_btnStop_clicked() {
  if (ui->chkKextsDev->isChecked()) {
    if (!dlEnd) {
      dlEnd = true;
      progInfo->close();
      ui->lblInfo->hide();
    }
  }

  mymethod->cancelKextUpdate();

  if (isCheckOC) {
    isCheckOC = false;
    ui->btnGetOC->setEnabled(true);
    ui->btnGetLastOC->setEnabled(true);
    delete progBar;
  }
}

void SyncOCDialog::on_btnUpdate_clicked() {
  writeCheckStateINI();
  mymethod->finishKextUpdate(false);
  ui->btnUpdate->setEnabled(false);
  repaint();

  init_Sync_OC_Table();
}

void SyncOCDialog::on_btnSelectAll_clicked() {
  for (int i = 0; i < sourceKexts.count(); i++) chkList.at(i)->setChecked(true);
}

void SyncOCDialog::on_btnClearAll_clicked() {
  for (int i = 0; i < sourceKexts.count(); i++)
    chkList.at(i)->setChecked(false);
}

void SyncOCDialog::resizeEvent(QResizeEvent* event) { Q_UNUSED(event); }

void SyncOCDialog::readCheckStateINI() {
  QString strTag = SaveFileName;
  strTag.replace("/", "-");
  QString qfile = mw_one->strConfigPath + "chk.ini";
  QSettings Reg(qfile, QSettings::IniFormat);
  for (int i = 0; i < sourceKexts.count(); i++) {
    QString str_0 = ui->tableKexts->item(i, 3)->text();
    QString strValue = strTag + str_0;

    bool strCheck = Reg.value(strValue).toBool();
    chkList.at(i)->setChecked(strCheck);
  }

  // OpenCore
  for (int i = 0; i < ui->listOpenCore->count(); i++) {
    QString strValue = strTag + ui->listOpenCore->item(i)->text().trimmed();
    bool yes = false;
    for (int m = 0; m < Reg.allKeys().count(); m++) {
      if (Reg.allKeys().at(m).contains(strValue)) {
        yes = true;
      }
    }
    if (yes) {
      int strCheck = Reg.value(strValue).toInt();
      if (strCheck == 2) ui->listOpenCore->item(i)->setCheckState(Qt::Checked);
      if (strCheck == 0)
        ui->listOpenCore->item(i)->setCheckState(Qt::Unchecked);
    }
  }
}

void SyncOCDialog::init_Sync_OC_Table() {
  if (blDEV) {
    ui->btnGetLastOC->hide();
    ui->btnGetOC->setText(tr("Get OpenCore"));
    ui->lblOCVersions->setHidden(true);
    ui->comboOCVersions->setHidden(true);
    ui->comboOCVersions->setCurrentIndex(0);

    ui->lblDevSource->setHidden(false);
    ui->editOCDevSource->setHidden(false);
    ui->btnImport->setHidden(false);
  } else {
    ui->btnGetLastOC->show();
    ui->lblOCVersions->setHidden(false);
    ui->comboOCVersions->setHidden(false);
    ui->comboOCVersions->setCurrentText("");

    ui->lblDevSource->setHidden(true);
    ui->editOCDevSource->setHidden(true);
    ui->btnImport->setHidden(true);

    QString strDev = Reg.value("maxVer", "0.7.8").toString();
    if (strDev.contains(" ")) strDev = strDev.split(" ").at(0);
    if (strDev > ui->comboOCVersions->itemText(1)) {
      ui->comboOCVersions->clear();
      QString a0, b0, c0;
      QStringList list = strDev.split(".");
      if (list.count() == 3) {
        a0 = list.at(0);
        b0 = list.at(1);
        c0 = list.at(2);
        QStringList lver;

        QString str0 = "061";
        QString str1 = a0 + b0 + c0;
        int start = str0.toInt();
        int end = str1.toInt() + 1;
        QString a, b, c;

        for (int i = 0; i < 200; i++) {
          start++;
          QString str00 = QString::number(start);
          if (str00.length() == 2) {
            a = "0";
            b = str00.mid(0, 1);
            c = str00.mid(1, 1);
          }
          if (str00.length() == 3) {
            a = str00.mid(0, 1);
            b = str00.mid(1, 1);
            c = str00.mid(2, 1);
          }
          lver.insert(0, a + "." + b + "." + c);
          if (start == end) break;
        }
        lver.removeAt(0);
        lver.insert(0, tr("Latest Version"));
        ui->comboOCVersions->addItems(lver);
      }
    }
    QString str = ocVer;
    if (str.contains(" ")) str = str.split(" ").at(0);

    ui->comboOCVersions->setCurrentText(str);
  }

  sourceKexts.clear();
  targetKexts.clear();
  sourceOpenCore.clear();
  targetOpenCore.clear();
  ui->listOpenCore->clear();
  chkList.clear();

  QString DirName;
  int pathCol = 0;
  QIcon icon;

  QFileInfo fi(SaveFileName);
  DirName = fi.path().mid(0, fi.path().count() - 3);

  // if (DirName.isEmpty()) return;

  QString pathOldSource;
  pathOldSource = QDir::homePath() + "/.ocat/Database/";

  QString file1, file2, file3, file4;
  QString targetFile1, targetFile2, targetFile3, targetFile4;

  file1 = mw_one->pathSource + "EFI/OC/OpenCore.efi";
  file2 = mw_one->pathSource + "EFI/BOOT/BOOTx64.efi";
  file3 = mw_one->pathSource + "EFI/OC/Drivers/OpenRuntime.efi";
  file4 = mw_one->pathSource + "EFI/OC/Drivers/OpenCanopy.efi";

  sourceOpenCore.append(file1);
  sourceOpenCore.append(file2);
  sourceOpenCore.append(file3);
  sourceOpenCore.append(file4);

  targetFile1 = DirName + "/OC/OpenCore.efi";
  targetFile2 = DirName + "/BOOT/BOOTx64.efi";
  targetFile3 = DirName + "/OC/Drivers/OpenRuntime.efi";
  targetFile4 = DirName + "/OC/Drivers/OpenCanopy.efi";
  targetOpenCore.append(targetFile1);
  targetOpenCore.append(targetFile2);
  targetOpenCore.append(targetFile3);
  targetOpenCore.append(targetFile4);

  // Drivers
  QString str1, str2;
  pathCol = get_PathCol(mw_one->ui->table_uefi_drivers, "Path");
  for (int i = 0; i < mw_one->ui->table_uefi_drivers->rowCount(); i++) {
    str1 = mw_one->ui->table_uefi_drivers->item(i, pathCol)->text();
    str2 = mw_one->pathSource + "EFI/OC/Drivers/" + str1;

    bool re = false;
    for (int j = 0; j < sourceOpenCore.count(); j++) {
      if (sourceOpenCore.at(j) == str2) re = true;
    }
    if (!re) {
      sourceOpenCore.append(str2);
      targetOpenCore.append(DirName + "/OC/Drivers/" + str1);
    }
  }

  // Kexts
  pathCol = get_PathCol(mw_one->ui->table_kernel_add, "BundlePath");
  for (int i = 0; i < mw_one->ui->table_kernel_add->rowCount(); i++) {
    QString strKextName =
        mw_one->ui->table_kernel_add->item(i, pathCol)->text().trimmed();
    if (!strKextName.contains("/Contents/PlugIns/")) {
      sourceKexts.append(pathOldSource + "EFI/OC/Kexts/" + strKextName);
      targetKexts.append(DirName + "/OC/Kexts/" + strKextName);
    }
  }

  // Tools
  QStringList dbToolsFileList;
  pathCol = get_PathCol(mw_one->ui->tableTools, "Path");
  dbToolsFileList =
      mymethod->DirToFileList(mw_one->pathSource + "EFI/OC/Tools/", "*.efi");
  for (int i = 0; i < mw_one->ui->tableTools->rowCount(); i++) {
    QString strName =
        mw_one->ui->tableTools->item(i, pathCol)->text().trimmed();
    if (mymethod->isEqualInList(strName, dbToolsFileList)) {
      sourceOpenCore.append(mw_one->pathSource + "EFI/OC/Tools/" + strName);
      targetOpenCore.append(DirName + "/OC/Tools/" + strName);
    }
  }

  // Init Kexts
  ui->tableKexts->setRowCount(0);
  for (int i = 0; i < sourceKexts.count(); i++) {
    QString f = sourceKexts.at(i);
    QString str_name = mymethod->getFileName(f);
    ui->tableKexts->setRowCount(ui->tableKexts->rowCount() + 1);
    QTableWidgetItem* item = new QTableWidgetItem(str_name);
    ui->tableKexts->setItem(i, 3, item);

    QCheckBox* chk = new QCheckBox(this);
    chkList.append(chk);
    ui->tableKexts->setCellWidget(i, 0, chk);
    QString strF1 = sourceKexts.at(i);
    QString strF2 = targetKexts.at(i);
    QString Current = mymethod->getKextVersion(strF2);
    QString Available = mymethod->getKextVersion(strF1);

    QString curMd5 = mw_one->getMD5(mymethod->getKextBin(strF2));
    QString avaMd5 = mw_one->getMD5(mymethod->getKextBin(strF1));

    item = new QTableWidgetItem(Current);
    ui->tableKexts->setItem(i, 1, item);

    item = new QTableWidgetItem(Available);
    ui->tableKexts->setItem(i, 2, item);

    if (Available != "None") {
      // if (Available > Current) {
      if (curMd5 != avaMd5) {
        chk->setChecked(true);

        icon.addFile(":/icon/no.png", QSize(10, 10));
        QTableWidgetItem* id1 = new QTableWidgetItem(icon, "");
        ui->tableKexts->setVerticalHeaderItem(i, id1);
      } else {
        chk->setChecked(false);

        icon.addFile(":/icon/ok.png", QSize(10, 10));
        QTableWidgetItem* id1 = new QTableWidgetItem(icon, "");
        ui->tableKexts->setVerticalHeaderItem(i, id1);
      }
    } else {
      chk->setChecked(false);

      icon.addFile(":/icon/ok.png", QSize(10, 10));
      QTableWidgetItem* id1 = new QTableWidgetItem(icon, "");
      ui->tableKexts->setVerticalHeaderItem(i, id1);
    }
  }

  mw_one->myDlgPreference->refreshKextUrl(true);
  for (int j = 0; j < sourceKexts.count(); j++) {
    QString str1 = QFileInfo(sourceKexts.at(j)).fileName();
    bool defUS = false;
    for (int i = 0; i < mw_one->myDlgPreference->ui->tableKextUrl->rowCount();
         i++) {
      QString str = mw_one->myDlgPreference->ui->tableKextUrl->item(i, 0)
                        ->text()
                        .trimmed();

      if (str1 == str || str1.mid(0, 3) == "SMC" || str1.contains("AppleALC") ||
          str1.contains("BlueTool") || str1.contains("VoodooPS2Controller") ||
          str1.contains("VoodooI2C") || str1.contains("Brcm") ||
          str1.contains("IntelSnowMausi"))
        defUS = true;
    }

    if (!defUS) {
      icon.addFile(":/icon/nous.png", QSize(10, 10));
      QTableWidgetItem* id1 = new QTableWidgetItem(icon, "");
      ui->tableKexts->setVerticalHeaderItem(j, id1);
    }
  }

  // OpenCore
  for (int i = 0; i < sourceOpenCore.count(); i++) {
    QString f = sourceOpenCore.at(i);
    QString str_name = mymethod->getFileName(f);
    ui->listOpenCore->addItem(str_name);
  }

  for (int i = 0; i < ui->listOpenCore->count(); i++) {
    ui->listOpenCore->item(i)->setCheckState(Qt::Checked);
  }

  if (!blDEV) {
    ui->lblOCFrom->setText(ocFrom);
    setWindowTitle(tr("Sync OC") + " -> " + ocVer);
  } else {
    ui->lblOCFrom->setText(ocFromDev);
    setWindowTitle(tr("Sync OC") + " -> " + ocVerDev);
  }

  setModal(true);
  show();
  ui->tableKexts->setFocus();

  for (int i = 0; i < ui->listOpenCore->count(); i++) {
    ui->listOpenCore->setCurrentRow(i);
  }
  repaint();

  // Resources
  sourceResourcesDir = pathOldSource + "EFI/OC/Resources/";
  targetResourcesDir = DirName + "/OC/Resources/";

  // Read check status
  readCheckStateINI();
}

int SyncOCDialog::get_PathCol(QTableWidget* t, QString pathText) {
  for (int i = 0; i < t->horizontalHeader()->count(); i++) {
    QString strCol = t->horizontalHeaderItem(i)->text();
    if (strCol == pathText) {
      return i;
    }
  }
  if (t->horizontalHeader()->count() == 1) return 0;
  return 0;
}

void SyncOCDialog::on_tableKexts_itemSelectionChanged() {
  int row = ui->tableKexts->currentRow();

  if (row < 0) return;

  QString strShowFileName, strSV, strTV;
  strShowFileName = ui->tableKexts->item(row, 3)->text();
  strSV = "";  // mymethod->getKextVersion(sourceKexts.at(row));
  strTV = "";  // mymethod->getKextVersion(targetKexts.at(row));

  QString sourceHash =
      mw_one->getMD5(mymethod->getKextBin(sourceKexts.at(row)));
  QString targetHash =
      mw_one->getMD5(mymethod->getKextBin(targetKexts.at(row)));

  ui->lblShowInfo->setText(strShowFileName + "\n" + tr("Current File: ") +
                           strTV + "  md5    " + targetHash + "    " +
                           tr("Available File: ") + strSV + "  md5    " +
                           sourceHash);
}

void SyncOCDialog::on_btnGetOC_clicked() {
  if (!ui->btnCheckUpdate->isEnabled() || !dlEnd) {
    QMessageBox box;
    box.setText(tr(
        "Kexts update check is in progress, please wait for it to complete."));
    box.exec();
    return;
  }

  QString tempDir = QDir::homePath() + "/tempocat/";
  mw_one->deleteDirfile(tempDir);

  QString DevSource;
  if (blDEV) {
    DevSource = ui->editOCDevSource->lineEdit()->text().trimmed();
    if (DevSource == "") {
      QMessageBox box;
      box.setText(
          tr("Please enter an available OpenCore development version update "
             "source."));
      box.exec();
      return;
    }
  }

  isCheckOC = true;
  mymethod->blBreak = false;
  mymethod->isReply = false;
  ui->btnGetOC->setEnabled(false);
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

  progBar->setGeometry(ui->frame->x(), ui->frame->y() - 8, ui->frame->width(),
                       ui->frame->height());
  progBar->show();

  progInfo->setGeometry(ui->btnGetOC->x(), ui->btnGetOC->y(),
                        ui->btnGetOC->width(), ui->btnGetOC->height());

  if (blDEV) {
    if (DevSource == "https://github.com/dortania/build-repo") {
      getKextHtmlInfo(getJsonUrl(), false);
      QString url = getKextDevDL(bufferJson, "OpenCorePkg");
      mymethod->startDownload(url);
    } else {
      if (mw_one->myDlgPreference->ui->rbtnAPI->isChecked() ||
          mw_one->myDlgPreference->ui->rbtnToken->isChecked())
        mymethod->getLastReleaseFromUrl(DevSource);
      if (mw_one->myDlgPreference->ui->rbtnWeb->isChecked())
        mymethod->getLastReleaseFromHtml(DevSource + "/releases");
    }

  } else {
    if (ui->comboOCVersions->currentText() == tr("Latest Version")) {
      QString ocUrl = "https://github.com/acidanthera/OpenCorePkg";
      if (mw_one->myDlgPreference->ui->rbtnAPI->isChecked() ||
          mw_one->myDlgPreference->ui->rbtnToken->isChecked())
        mymethod->getLastReleaseFromUrl(ocUrl);
      if (mw_one->myDlgPreference->ui->rbtnWeb->isChecked())
        mymethod->getLastReleaseFromHtml(ocUrl + "/releases");
    } else {
      progInfo->hide();
      mymethod->startDownload(downLink);
    }
  }
}

void SyncOCDialog::on_comboOCVersions_currentTextChanged(const QString& arg1) {
  if (Initialization) return;

  QString str = "https://github.com/acidanthera/OpenCorePkg/releases/tag/";
  QString url = str + arg1;
  if (arg1 == tr("Latest Version")) {
    url = "https://github.com/acidanthera/OpenCorePkg/releases/latest";
    strOCFrom = url;
  } else {
    strOCFrom = str + arg1;
    if (mw_one->ui->actionDEBUG->isChecked())
      downLink =
          "https://github.com/acidanthera/OpenCorePkg/releases/download/" +
          arg1 + "/OpenCore-" + arg1 + "-DEBUG.zip";
    else
      downLink =
          "https://github.com/acidanthera/OpenCorePkg/releases/download/" +
          arg1 + "/OpenCore-" + arg1 + "-RELEASE.zip";

    ui->btnGetOC->setText(tr("Get OpenCore") + "  " + arg1);
  }

  ocFrom = "<a href=\"" + strOCFrom + "\"" + "> " + tr(" Source ");
  ui->lblOCFrom->setText(ocFrom);
}

void SyncOCDialog::on_btnImport_clicked() {
  if (!ui->btnCheckUpdate->isEnabled() || !ui->btnGetOC->isEnabled() ||
      !dlEnd) {
    QMessageBox box;
    box.setText(
        tr("Kexts update check or OpenCore database upgrade is in progress, "
           "please wait for it to finish."));
    box.exec();
    return;
  }

  QFileDialog fd;
  QString FileName =
      fd.getOpenFileName(this, "file", "", "zip file(*.zip);;all(*.*)");
  if (QFile(FileName).exists()) {
    QString path = QDir::homePath() + "/tempocat/";
    mw_one->deleteDirfile(path);
    QDir dir;
    dir.mkpath(path);
    QFileInfo fi(FileName);
    QString tar = path + fi.fileName();
    mw_one->copyFileToPath(FileName, tar, true);
    isCheckOC = true;
    mymethod->unZip(fi.fileName());
    isCheckOC = false;
  }
}

void SyncOCDialog::on_editOCDevSource_currentTextChanged(const QString& arg1) {
  strOCFrom = arg1;
  ocFrom = "<a href=\"" + strOCFrom + "\"" + "> " + tr(" Source ");
  ui->lblOCFrom->setText(ocFrom);
}

void SyncOCDialog::on_ProgBarvalueChanged(QProgressBar* m_bar) {
  int value = m_bar->value();
  m_bar->setFixedHeight(5);
  QImage m_image(":/icon/prog.png");
  QString qss =
      "QProgressBar{"
      "border: 0px solid rgb(16, 135, 209);"
      "background: rgba(24,24,255,255);"
      "border-radius: 0px; }"
      "QProgressBar::chunk:enabled {"
      "border-radius: 0px; "
      "background: qlineargradient(x1:0, y1:0, x2:1, y2:0";
  QString qss0 =
      "QProgressBar{border:0px solid #FFFFFF;"
      "height:30;"
      "background:rgba(24,24,255,255);"
      "text-align:right;"
      "color:rgb(255,255,255);"
      "border-radius:0px;}"

      "QProgressBar:chunk{"
      "border-radius:0px;"
      "background-color:rgba(255,127,36,255);"
      "}";

  double v = m_bar->maximum();
  double EndColor = static_cast<double>(value) / v;

  for (int i = 0; i < 100; i++) {
    double Current = EndColor * i / 100;
    QRgb rgb =
        m_image.pixel((m_image.width() - 1) * Current, m_image.height() / 2);
    QColor c(rgb);
    qss.append(QString(",stop:%1  rgb(%2,%3,%4)")
                   .arg(i / 100.0)
                   .arg(c.red())
                   .arg(c.green())
                   .arg(c.blue()));
  }

  qss.append(");}");
  if (m_bar->maximum() == 0)
    m_bar->setStyleSheet(qss0);
  else
    m_bar->setStyleSheet(qss);
}

void SyncOCDialog::on_btnSet_clicked() {
  mw_one->myDatabase->close();
  mw_one->on_actionPreferences_triggered();
  mw_one->myDlgPreference->ui->tabWidget->setCurrentIndex(0);
}

void SyncOCDialog::query(QNetworkReply* reply) {
  bufferJson = reply->readAll();
  dlEnd = true;
  // qDebug() << bufferJson;
  if (writefile) {
    QString FILE_NAME = mw_one->strConfigPath + "code.txt";
    QFile file(FILE_NAME);
    if (file.exists()) file.remove();
    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);
    out << bufferJson;
    file.close();
  }
  progInfo->close();
  ui->lblInfo->hide();
}

QString SyncOCDialog::getKextDevDL(QString bufferJson, QString kextName) {
  QString dl;
  QJsonDocument jsonDoc;
  QJsonParseError parseJsonErr;
  jsonDoc = QJsonDocument::fromJson(bufferJson.toUtf8(), &parseJsonErr);
  if (parseJsonErr.error != QJsonParseError::NoError) {
    qDebug() << "json error:" << parseJsonErr.errorString();

  } else {
    QJsonObject res = jsonDoc.object();
    qDebug() << res.count() << jsonDoc.isArray() << jsonDoc.isObject();
    if (jsonDoc.isObject()) {
      QStringList Keys = res.keys();
      qDebug() << Keys;
      int index = 0;
      for (int i = 0; i < Keys.count(); i++) {
        // if (Keys.at(i) == kextName)
        if (kextName.contains(Keys.at(i))) {
          index = i;
        }
      }
      QVariantList list, list1;

      list = res.value(Keys.at(index))
                 .toObject()
                 .value("versions")
                 .toArray()
                 .toVariantList();

      if (list.count() > 0) {
        list1 = list.at(0)
                    .toJsonDocument()
                    .object()
                    .value("links")
                    .toArray()
                    .toVariantList();

        QVariantMap map = list[0].toMap();
        QVariantMap map1 = map["links"].toMap();

        if (!mw_one->ui->actionDEBUG->isChecked())
          dl = map1["release"].toString();
        else
          dl = map1["debug"].toString();
        qDebug() << dl;
        return dl;
      }
    }
  }

  return dl;
}

QString SyncOCDialog::getKextHtmlInfo(QString url, bool writeFile) {
  init_InfoShow();
  writefile = writeFile;
  mgr->get(QNetworkRequest(QUrl(url)));

  dlEnd = false;
  while (!dlEnd && !mymethod->blBreak) {
    QCoreApplication::processEvents();
  }

  return bufferJson;
}

void SyncOCDialog::init_InfoShow() {
  if (ui->chkKextsDev->isChecked() && ui->btnGetOC->isEnabled() &&
      mw_one->myDlgPreference->ui->btnDownloadKexts->isEnabled())
    ui->lblInfo->show();
  ui->lblInfo->setText(
      tr("Please wait while we get the download information of Kexts "
         "development version..."));

  progInfo->setTextVisible(false);
  progInfo->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  progInfo->setStyleSheet(
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

  progInfo->setMaximum(0);
  progInfo->setMinimum(0);
  if (mw_one->myDlgPreference->ui->btnDownloadKexts->isEnabled())
    progInfo->show();
}

void SyncOCDialog::on_btnGetLastOC_clicked() {
  ui->btnGetLastOC->setEnabled(false);
  ui->comboOCVersions->setCurrentIndex(0);
  ui->btnGetOC->click();
}
