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
QProgressBar* progBar;

SyncOCDialog::SyncOCDialog(QWidget* parent)
    : QDialog(parent), ui(new Ui::SyncOCDialog) {
  ui->setupUi(this);

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
      "blue;margin:1px,1px,1px,1px;border-radius:6;"
      "color:white}";

  ui->listKexts->setStyleSheet(listStyleMain);
  ui->listOpenCore->setStyleSheet(listStyleMain);
  int size = 25;
  ui->listKexts->setIconSize(QSize(15, 15));
  ui->listOpenCore->setIconSize(QSize(15, 15));
  ui->listKexts->setGridSize(QSize(size, size));
  ui->listOpenCore->setGridSize(QSize(size, size));

  ui->btnStartSync->setDefault(true);
  ui->labelShowDLInfo->setVisible(false);
  ui->labelShowDLInfo->setText("");
  ui->btnUpdate->setEnabled(false);

  ui->listKexts->setViewMode(QListView::ListMode);
  ui->listKexts->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

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

  ui->listKexts->setHidden(true);

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
  strIniFile =
      QDir::homePath() + "/.config/" + strAppName + "/" + strAppName + ".ini";
  QSettings Reg(strIniFile, QSettings::IniFormat);
  ui->editOCDevSource->lineEdit()->setText(
      Reg.value("DevSource", "https://github.com/dortania/build-repo")
          .toString());
}

SyncOCDialog::~SyncOCDialog() { delete ui; }

void SyncOCDialog::on_btnStartSync_clicked() {
  if (!ui->btnCheckUpdate->isEnabled() || !ui->btnUpdateOC->isEnabled()) {
    QMessageBox box;
    box.setText(
        tr("Kexts update check or OpenCore database upgrade is in progress, "
           "please wait for it to finish."));
    box.exec();
    return;
  }

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

void SyncOCDialog::addVerWidget(int currentRow, QString strTV, QString strSV,
                                QString strShowFileName) {
  if (mw_one->mac || mw_one->osx1012)
    verList.at(currentRow)->setFont(QFont("Menlo", 12));
  if (mw_one->win) verList.at(currentRow)->setFont(QFont("consolas"));

  ui->listKexts->item(currentRow)->setText("        " + strShowFileName);
  verList.at(currentRow)->setText(strTV + "    " + strSV + " ");

  QString strStyleSel = "QLabel {color: #e6e6e6;background-color: none;}";
  QString strStyle = "QLabel {color: #2c2c2c;background-color: none;}";
  for (int i = 0; i < ui->listKexts->count(); i++) {
    if (i == currentRow)
      verList.at(currentRow)->setStyleSheet(strStyleSel);
    else
      verList.at(i)->setStyleSheet(strStyle);
  }
}

void SyncOCDialog::on_listKexts_currentRowChanged(int currentRow) {
  if (currentRow < 0) return;

  ui->listKexts->item(ui->listKexts->currentRow())
      ->setForeground(QBrush(Qt::black));
  ui->listKexts->item(ui->listKexts->currentRow())
      ->setBackground(QBrush(QColor("#FFD39B")));

  QString sourceModi, targetModi, sourceFile, targetFile, sourceHash,
      targetHash;

  sourceFile = sourceKexts.at(currentRow);
  targetFile = targetKexts.at(currentRow);

  sourceHash = mw_one->getMD5(mymethod->getKextBin(sourceFile));
  targetHash = mw_one->getMD5(mymethod->getKextBin(targetFile));

  QFileInfo fiSource(sourceFile);
  QFileInfo fiTarget(targetFile);
  sourceModi = fiSource.lastModified().toString();
  targetModi = fiTarget.lastModified().toString();

  QString strShowFileName, strSV, strTV;
  strShowFileName = fiSource.fileName();
  strSV = mymethod->getKextVersion(sourceFile);
  strTV = mymethod->getKextVersion(targetFile);

  bool defUS = false;
  mw_one->myDlgPreference->refreshKextUrl();
  for (int i = 0; i < mw_one->myDlgPreference->ui->tableKextUrl->rowCount();
       i++) {
    QString str =
        mw_one->myDlgPreference->ui->tableKextUrl->item(i, 0)->text().trimmed();
    QString str1 = QFileInfo(targetFile).fileName();
    if (str1 == str || str1.mid(0, 3) == "SMC" || str1.contains("AppleALC") ||
        str1.contains("BlueTool") || str1.contains("VoodooPS2Controller") ||
        str1.contains("VoodooI2C") || str1.contains("Brcm") ||
        str1.contains("IntelSnowMausi"))
      defUS = true;
  }

  if (!defUS) {
    ui->listKexts->item(currentRow)->setIcon(QIcon(":/icon/nous.png"));

  } else {
    if (!QFile(sourceFile).exists()) {
      ui->listKexts->item(currentRow)->setIcon(QIcon(":/icon/ok.png"));
    }
    if (!QFile(targetFile).exists()) {
      ui->listKexts->item(currentRow)->setIcon(QIcon(":/icon/no.png"));
    }
    if (QFile(sourceFile).exists() && QFile(targetFile).exists()) {
      if (strSV > strTV) {
        ui->listKexts->item(currentRow)->setIcon(QIcon(":/icon/no.png"));

      } else {
        ui->listKexts->item(currentRow)->setIcon(QIcon(":/icon/ok.png"));
      }
    }
  }

  addVerWidget(currentRow, strTV, strSV, strShowFileName);

  if (sourceHash != targetHash) {
  } else {
  }
}

void SyncOCDialog::on_listKexts_itemClicked(QListWidgetItem* item) {
  Q_UNUSED(item);
}

void SyncOCDialog::on_listOpenCore_itemClicked(QListWidgetItem* item) {
  Q_UNUSED(item);
}

void SyncOCDialog::setListWidgetStyle() {
  QString fileName = sourceOpenCore.at(ui->listOpenCore->currentRow());
  if (mymethod->isWhatFile(fileName, "efi")) {
    setListWidgetColor("#FFF8DC");
  }
  if (mymethod->isWhatFile(fileName, "efi") && fileName.contains("/Tools/")) {
    setListWidgetColor("#FFEFDB");
  }
  if (mymethod->isWhatFile(fileName, "efi") && fileName.contains("/Drivers/")) {
    setListWidgetColor("#E6E6FA");
  }
}

void SyncOCDialog::setListWidgetColor(QString color) {
  ui->listOpenCore->item(ui->listOpenCore->currentRow())
      ->setForeground(QBrush(Qt::black));
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
                           "md5    " + targetHash + "  " +
                           tr("Available File: ") + "md5    " + sourceHash);

  if (sourceHash != targetHash) {
    ui->listOpenCore->item(currentRow)->setIcon(QIcon(":/icon/no.png"));

  } else {
    ui->listOpenCore->item(currentRow)->setIcon(QIcon(":/icon/ok.png"));
  }
}

void SyncOCDialog::closeEvent(QCloseEvent* event) {
  if (!ui->btnCheckUpdate->isEnabled()) event->ignore();
  writeCheckStateINI();

  QString txt = ui->editOCDevSource->lineEdit()->text().trimmed();
  QSettings Reg(strIniFile, QSettings::IniFormat);
  Reg.setValue("DevSource", txt);
  if (txt != "")
    ocFromDev = "<a href=\"" + txt + "\"" + "> " + tr(" Source ");
  else
    ocFromDev = "";
  if (blDEV) mw_one->dlgSyncOC->ui->lblOCFrom->setText(ocFromDev);
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
  if (!ui->btnUpdateOC->isEnabled()) {
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

  ui->labelShowDLInfo->setVisible(true);

  mymethod->kextUpdate();

  QString Current, Available;

  for (int i = 0; i < sourceKexts.count(); i++) {
    QString sourceFile = sourceKexts.at(i);
    QString targetFile = targetKexts.at(i);

    if (chkList.at(i)->isChecked()) {
      Available = mymethod->getKextVersion(sourceFile);
      Current = mymethod->getKextVersion(targetFile);
      if ((Available > Current || Current == "None") &&
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

void SyncOCDialog::on_btnStop_clicked() {
  mymethod->cancelKextUpdate();

  if (isCheckOC) {
    isCheckOC = false;
    ui->btnUpdateOC->setEnabled(true);
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

void SyncOCDialog::initKextList() {
  chkList.clear();
  textList.clear();
  verList.clear();

  QString strStyle = "QLabel {color: #2c2c2c;background-color: none;}";

  for (int i = 0; i < ui->listKexts->count(); i++) {
    QWidget* w = new QWidget(this);
    QHBoxLayout* layout = new QHBoxLayout(w);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setDirection(QBoxLayout::LeftToRight);

    checkBox = new QCheckBox(w);
    chkList.append(checkBox);

    lblTxt = new QLabel(w);
    lblTxt->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    textList.append(lblTxt);

    lblVer = new QLabel(w);
    lblVer->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    lblVer->setStyleSheet(strStyle);
    verList.append(lblVer);

    layout->addSpacing(4);
    layout->addWidget(checkBox, 0, Qt::AlignLeft | Qt::AlignAbsolute);
    layout->addSpacing(4);
    layout->addWidget(lblTxt, 0, Qt::AlignLeft | Qt::AlignAbsolute);
    layout->addWidget(lblVer, 0, Qt::AlignRight | Qt::AlignAbsolute);

    w->setLayout(layout);
    ui->listKexts->setItemWidget(ui->listKexts->item(i), w);
  }
}

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

void SyncOCDialog::on_btnSettings_clicked() {
  mw_one->myDatabase->close();
  mw_one->on_actionPreferences_triggered();
  mw_one->myDlgPreference->ui->tabWidget->setCurrentIndex(0);
}

void SyncOCDialog::init_Sync_OC_Table() {
  if (blDEV) {
    ui->lblOCVersions->setHidden(true);
    ui->comboOCVersions->setHidden(true);
    ui->comboOCVersions->setCurrentIndex(0);

    ui->lblDevSource->setHidden(false);
    ui->editOCDevSource->setHidden(false);
  } else {
    ui->lblOCVersions->setHidden(false);
    ui->comboOCVersions->setHidden(false);
    ui->comboOCVersions->setCurrentText("");

    ui->lblDevSource->setHidden(true);
    ui->editOCDevSource->setHidden(true);

    QString strDev = ocVerDev;
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
        int end = str1.toInt();
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
  ui->listKexts->clear();
  ui->listOpenCore->clear();
  chkList.clear();

  QString DirName;
  int pathCol = 0;
  QIcon icon;

  QFileInfo fi(SaveFileName);
  DirName = fi.path().mid(0, fi.path().count() - 3);

  if (DirName.isEmpty()) return;

  QString pathOldSource;
  pathOldSource = QDir::homePath() + "/Database/";

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

    item = new QTableWidgetItem(Current);
    ui->tableKexts->setItem(i, 1, item);

    item = new QTableWidgetItem(Available);
    ui->tableKexts->setItem(i, 2, item);

    if (Available != "None") {
      if (Available > Current) {
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

  mw_one->myDlgPreference->refreshKextUrl();
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
  ui->listKexts->setFocus();

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
                           strTV + "  md5    " + targetHash + "  " +
                           tr("Available File: ") + strSV + "  md5    " +
                           sourceHash);
}

void SyncOCDialog::on_btnUpdateOC_clicked() {
  if (!ui->btnCheckUpdate->isEnabled()) {
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
  ui->btnUpdateOC->setEnabled(false);
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
  progBar->setGeometry(ui->btnUpdateOC->x(), ui->btnUpdateOC->y(),
                       ui->btnUpdateOC->width(), ui->btnUpdateOC->height());
  progBar->show();

  if (blDEV) {
    if (mw_one->myDlgPreference->ui->rbtnAPI->isChecked())
      mymethod->getLastReleaseFromUrl(DevSource);
    if (mw_one->myDlgPreference->ui->rbtnWeb->isChecked())
      mymethod->getLastReleaseFromHtml(DevSource + "/releases/latest");
  } else {
    if (ui->comboOCVersions->currentText() == tr("Latest Version")) {
      QString ocUrl = "https://github.com/acidanthera/OpenCorePkg";
      if (mw_one->myDlgPreference->ui->rbtnAPI->isChecked())
        mymethod->getLastReleaseFromUrl(ocUrl);
      if (mw_one->myDlgPreference->ui->rbtnWeb->isChecked())
        mymethod->getLastReleaseFromHtml(ocUrl + "/releases/latest");
    } else {
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
  }
  ocFrom = "<a href=\"" + strOCFrom + "\"" + "> " + tr(" Source ");
  ui->lblOCFrom->setText(ocFrom);
}
