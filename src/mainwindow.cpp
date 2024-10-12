#include "mainwindow.h"

#include <fstream>
#include <iostream>
#include <iterator>

#include "BalloonTip.h"
#include "Method.h"
#include "Plist.hpp"
#include "commands.h"
#include "ui_mainwindow.h"

using namespace std;

#include <QBuffer>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>

QString strAppName = "OCAuxiliaryTools";
QString strIniFile =
    QDir::homePath() + "/.config/" + strAppName + "/" + strAppName + ".ini";
QSettings Reg(strIniFile, QSettings::IniFormat);
QString PlistFileName, SaveFileName, strAppExePath;
QVector<QString> openFileLists;
QRegularExpression regxData("[A-Fa-f0-9 ]{0,1024}");
QRegularExpression regxNumber("^-?\[0-9]*$");
Method* mymethod;
QVector<QCheckBox*> chkDisplayLevel, chk_ScanPolicy, chk_PickerAttributes,
    chk_ExposeSensitiveData, chk_Target;
QVariantMap mapTatol;
bool Initialization = false;
bool zh_cn = false;
int red = 0;

extern QString CurVersion, ocVer, ocVerDev, ocFrom, ocFromDev, strOCFrom,
    strACPI, strKexts, strDrivers, strTools, strOCFromDev;
extern bool blDEV;
extern QWidgetList listOCATWidgetHideList, listOCATWidgetDelList;
extern QStringList boolTypeList, intTypeList, dataTypeList, listKey, listType,
    listValue;
;

void MainWindow::changeOpenCore(bool blDEV) {
  init_FindResults();
  ui->mycboxFind->lineEdit()->clear();
  if (!blDEV) {
    dataBaseDir = strAppExePath + "/Database/";
    userDataBaseDir = QDir::homePath() + "/.ocat/Database/";
    if (!ui->actionDEBUG->isChecked()) {
      pathSource = userDataBaseDir;
      ocVer = ocVer.replace(" " + tr("DEBUG"), "");
      lblVer->setText("  OpenCore " + ocVer);
      aboutDlg->ui->lblVersion->setText(tr("Version") + "  " + CurVersion +
                                        " for OpenCore " + ocVer);

    } else {
      pathSource = userDataBaseDir + "DEBUG/";
      if (!QFile(pathSource + "EFI/OC/OpenCore.efi").exists()) {
        QMessageBox::critical(
            this, "",
            tr("The debug database does not exist, please "
               "update it in the UI of  Upgrade OC and Kexts."));
      }
      ocVer = ocVer.replace(" " + tr("DEBUG"), "");
      ocVer = ocVer + " " + tr("DEBUG");
      lblVer->setText("  OpenCore " + ocVer);
      aboutDlg->ui->lblVersion->setText(tr("Version") + "  " + CurVersion +
                                        " for OpenCore " + ocVer);
    }

    QString str = "https://github.com/acidanthera/OpenCorePkg/releases/tag/";
    QString str1 = ocVer;
    strOCFrom = str + str1.replace(" " + tr("DEBUG"), "");

  } else {  // blDEV
    dataBaseDir = strAppExePath + "/Database/";
    userDataBaseDir = QDir::homePath() + "/.ocat/devDatabase/";
    if (!QFile(userDataBaseDir + "EFI/OC/OpenCore.efi").exists()) {
      QMessageBox::critical(
          this, "",
          tr("The development version database does not exist, please "
             "update it online in the  Upgrade OpenCore and Kexts UI."));
    }

    ocVerDev = ocVerDev.replace(" [" + tr("DEV") + "]", "");
    ocVerDev = ocVerDev + " [" + tr("DEV") + "]";

    if (!ui->actionDEBUG->isChecked()) {
      pathSource = userDataBaseDir;
      ocVerDev = ocVerDev.replace(" " + tr("DEBUG"), "");
      lblVer->setText("  OpenCore " + ocVerDev);
      aboutDlg->ui->lblVersion->setText(tr("Version") + "  " + CurVersion +
                                        " for OpenCore " + ocVerDev);

    } else {
      pathSource = userDataBaseDir + "DEBUG/";
      if (!QFile(pathSource + "EFI/OC/OpenCore.efi").exists()) {
        QMessageBox::critical(
            this, "",
            tr("The debug database does not exist, please "
               "update it in the UI of  Upgrade OC and Kexts."));
      }
      ocVerDev = ocVerDev.replace(" " + tr("DEBUG"), "");
      ocVerDev = ocVerDev + " " + tr("DEBUG");
      lblVer->setText("  OpenCore " + ocVerDev);
      aboutDlg->ui->lblVersion->setText(tr("Version") + "  " + CurVersion +
                                        " for OpenCore " + ocVerDev);
    }
  }

  for (int i = 0; i < listOCATWidgetHideList.count(); i++) {
    listOCATWidgetHideList.at(i)->setHidden(false);
  }

  for (int i = 0; i < listOCATWidgetDelList.count(); i++) {
    QWidget* frame = listOCATWidgetDelList.at(i);
    frame->parentWidget()->layout()->removeWidget(frame);
    delete (frame);
  }
  listOCATWidgetDelList.clear();

  smart_UpdateKeyField();
  init_LineEditDataCheck();
  for (int i = 0; i < listOCATWidgetDelList.count(); i++) {
    QWidget* w = listOCATWidgetDelList.at(i);
    if (w->objectName().mid(0, 5) == "frame") {
      if (w->children().at(1)->objectName().mid(0, 3) == "lbl") {
        QLabel* lbl = (QLabel*)w->children().at(1);
        init_Label(lbl);
      }
    }
    if (w->objectName().mid(0, 3) == "chk") {
      QCheckBox* chk = (QCheckBox*)w;
      init_CheckBox(chk);
    }
  }

  if (myDlgPreference->ui->chkHideToolbar->isChecked()) {
    title = lblVer->text() + "      ";
    setWindowTitle(title + "[*]" + SaveFileName);
  } else
    title = "[*]";

  if (QFile(SaveFileName).exists()) {
    oc_Validate(false);
  }
}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  Initialization = true;
  loading = true;
  init_Widgets();
  setUIMargin();
  init_MainUI();
  init_setWindowModified();
  init_hardware_info();
  initui_Misc();
  initui_PlatformInfo();
  initui_UEFI();
  init_CopyPasteLine();
  setTableEditTriggers();
  initRecentFilesForToolBar();
  load_LastFile();
  loading = false;
  Initialization = false;
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::load_LastFile() {
  if (myDlgPreference->ui->chkBoxLastFile->isChecked()) {
    QString file = Reg.value("LastFileName").toString();
    if (QFile(file).exists()) {
      openFile(file);
    }
  }
  this->setWindowModified(false);
  updateIconStatus();
}

QWidget* MainWindow::getSubTabWidget(int m, int s) {
  for (int j = 0; j < mainTabList.count(); j++) {
    if (j == m) {
      for (int i = 0; i < mainTabList.at(j)->tabBar()->count(); i++) {
        if (i == s) return mainTabList.at(j)->widget(i);
      }
    }
  }

  return NULL;
}

void MainWindow::setUIMargin() {
  int m = 1;
  QObjectList list = getAllGridLayout(getAllUIControls(this));
  for (int i = 0; i < list.count(); i++) {
    QGridLayout* w = (QGridLayout*)list.at(i);
    w->setContentsMargins(m, m, m, m);
  }

  ui->centralwidget->layout()->setSpacing(m);
}

void MainWindow::recentOpen(QString filename) { openFile(filename); }

void MainWindow::initRecentFilesForToolBar() {
  QStringList rfList = m_recentFiles->getRecentFiles();
  reFileMenu->clear();
  for (int i = 0; i < rfList.count(); i++) {
    QFileInfo fi(rfList.at(i));
    QAction* act = new QAction(QString::number(i + 1) + " . " + fi.filePath());
    reFileMenu->addAction(act);
    connect(act, &QAction::triggered,
            [=]() { openFile(m_recentFiles->getRecentFiles().at(i)); });
  }
  if (rfList.count() > 0) {
    reFileMenu->addSeparator();
    QAction* actClearHistory = new QAction(tr("Clear History"));
    connect(actClearHistory, &QAction::triggered, [=]() {
      reFileMenu->clear();
      m_recentFiles->getRecentFiles().clear();
      m_recentFiles->clearHistory();
    });
    reFileMenu->addAction(actClearHistory);
  }
}

void MainWindow::openFile(QString PlistFileName) {
  if (!PlistFileName.isEmpty()) {
    if (!PListSerializer::fileValidation(PlistFileName)) {
      QMessageBox::warning(this, "", tr("Invalid plist file."), tr("OK"));
      return;
    }

    mymethod->removeFileSystemWatch(SaveFileName);
    SaveFileName = PlistFileName;
    mymethod->addFileSystemWatch(SaveFileName);
    FileSystemWatcher::addWatchPath(SaveFileName);

  } else
    return;

  if (myDlgPreference->ui->chkBoxLastFile->isChecked()) {
    Reg.setValue("LastFileName", SaveFileName);
  }

  loading = true;

  // 初始化
  init_Table(-1);
  listDPAdd.clear();
  listDPDel.clear();
  listNVRAMAdd.clear();
  listNVRAMDel.clear();
  listNVRAMLs.clear();
  ui->editSystemUUID->setText("");
  ui->editMLB->setText("");
  ui->editSystemSerialNumber->setText("");
  ui->mycboxEmulate->setCurrentIndex(0);

  QFile file(PlistFileName);
  QVariantMap map = PListParser::parsePList(&file).toMap();

  NoteValues.clear();
  NoteKeys.clear();
  for (int i = 0; i < map.count(); i++) {
    QString key = map.keys().at(i);
    if (key.trimmed().mid(0, 1) == "#") {
      NoteKeys.append(key);
      NoteValues.append(map.values().at(i));
    }
  }

  ParserACPI(map);
  ParserBooter(map);
  ParserDP(map);

  ParserKernel(map, "Add");
  ParserKernel(map, "Block");
  ParserKernel(map, "Emulate");
  ParserKernel(map, "Force");
  ParserKernel(map, "Patch");
  ParserKernel(map, "Quirks");
  ParserKernel(map, "Scheme");

  ParserMisc(map);
  ParserNvram(map);
  ParserPlatformInfo(map);
  ParserUEFI(map);

  file.close();
  loading = false;

  ui->actionSave->setEnabled(true);
  undoStack->clear();
  oc_Validate(false);
  if (!isGetEFI) {
    QSettings settings;
    QFileInfo fi(PlistFileName);
    settings.setValue("currentDirectory", fi.absolutePath());
    m_recentFiles->setMostRecentFile(PlistFileName);
    initRecentFilesForToolBar();

    QString strEFI = fi.path().mid(0, fi.path().count() - 3);
    QFileInfo f1(strEFI + "/OC");
    QFileInfo f3(strEFI + "/OC/Drivers");
    if (f1.isDir() && f3.isDir()) {
      ui->actionUpgrade_OC->setEnabled(true);
      ui->actionUpgrade_OC->setToolTip(tr("Upgrade OpenCore and Kexts"));
    } else {
      ui->actionUpgrade_OC->setToolTip(
          tr("The Upgrade OpenCore and Kexts is not available, please check "
             "the integrity of the EFI directory structure, mainly the "
             "existence of the EFI/OC and EFI/OC/Drivers directories."));
    }
  }

  openFileAfter();

  checkFiles(ui->table_acpi_add);
  checkFiles(ui->table_kernel_add);
  checkFiles(ui->tableTools);
  checkFiles(ui->table_uefi_drivers);

  FindTextChange = true;
  strOrgMd5 = getMD5(SaveFileName);

  setWindowTitle(title + PlistFileName);
  checkSystemAudioVolume();

  this->setWindowModified(false);
  updateIconStatus();
  ui->lblStatusShow->setText("V" + CurVersion);
}

bool MainWindow::IsProcessExist(QString processName) {
  QProcess process;
  process.start("tasklist", QStringList() << "");
  process.waitForFinished();

  QByteArray result = process.readAllStandardOutput();
  QString str = result;
  if (str.contains(processName))
    return true;
  else
    return false;
}

QString MainWindow::getMD5(QString targetFile) {
  QCryptographicHash hashTest(QCryptographicHash::Md5);
  QFile f2(targetFile);
  f2.open(QFile::ReadOnly);
  hashTest.reset();  // 重置（很重要）
  hashTest.addData(&f2);
  QString targetHash = hashTest.result().toHex();
  f2.close();
  return targetHash;
}

void MainWindow::init_Table(int index) {
  if (index == -1) {
    listOfTableWidget.clear();
    listOfTableWidget = getAllTableWidget(getAllUIControls(ui->tabTotal));
    for (int i = 0; i < listOfTableWidget.count(); i++) {
      QTableWidget* w = (QTableWidget*)listOfTableWidget.at(i);
      if (w->objectName().mid(0, 5) == "table") w->setRowCount(0);
    }
  } else {
    for (int i = 0; i < ui->tabTotal->tabBar()->count(); i++) {
      if (index == i) {
        listOfTableWidget.clear();
        listOfTableWidget =
            getAllTableWidget(getAllUIControls(ui->tabTotal->widget(i)));
        for (int j = 0; j < listOfTableWidget.count(); j++) {
          QTableWidget* w = (QTableWidget*)listOfTableWidget.at(j);
          if (w->objectName().mid(0, 5) == "table") w->setRowCount(0);
        }
      }
    }
  }
}

void MainWindow::openFileAfter() {
  listOfTableWidget.clear();
  listOfTableWidget = getAllTableWidget(getAllUIControls(ui->tabTotal));
  for (int i = 0; i < listOfTableWidget.count(); i++) {
    QTableWidget* w = (QTableWidget*)listOfTableWidget.at(i);

    if (w->rowCount() > 0) {
      w->setCurrentCell(0, 0);
      w->setFocus();
    }
  }
}

void MainWindow::checkFiles(QTableWidget* table) {
  int fileCol = 0;
  for (int n = 0; n < table->columnCount(); n++) {
    QString txt = table->horizontalHeaderItem(n)->text();
    if (txt == "Path" || txt == "BundlePath") {
      fileCol = n;
      break;
    }
  }
  if (table == ui->table_acpi_add) markColor(table, strACPI, fileCol);
  if (table == ui->table_kernel_add) markColor(table, strKexts, fileCol);
  if (table == ui->table_uefi_drivers) markColor(table, strDrivers, fileCol);
  if (table == ui->tableTools) markColor(table, strTools, fileCol);
}

void MainWindow::markColor(QTableWidget* table, QString path, int col) {
  QIcon icon;
  QTableWidgetItem* id1;
  for (int i = 0; i < table->rowCount(); i++) {
    QString strFile = path + table->item(i, col)->text().trimmed();
    QFileInfo fi(strFile);
    if (fi.exists()) {
      icon.addFile(":/icon/green.svg", QSize(10, 10));
      id1 = new QTableWidgetItem(icon, QString::number(i + 1));
      table->setVerticalHeaderItem(i, id1);

      if (table == ui->table_kernel_add) {
        QString strVer = getKextVersion(strKexts + table->item(i, 0)->text());
        QString text = table->item(i, 1)->text();
        if (text.trimmed().length() > 0) {
          if (text.mid(0, 1) == "V") {
            QStringList strList = text.split("|");
            if (strList.count() >= 2) {
              text.replace(strList.at(0), "");
              text = "V" + strVer + " " + text;
            } else
              text = "V" + strVer;

          } else
            text = "V" + strVer + " | " + text;

        } else
          text = "V" + strVer;
        table->setItem(i, 1, new QTableWidgetItem(text));
      }

    } else {
      icon.addFile(":/icon/red.svg", QSize(10, 10));
      id1 = new QTableWidgetItem(icon, QString::number(i + 1));
      table->setVerticalHeaderItem(i, id1);
    }
  }
}

QString MainWindow::getKextVersion(QString kextFile) {
  QString strInfo = kextFile + "/Contents/Info.plist";
  QTextEdit* txtEdit = new QTextEdit;
  txtEdit->setPlainText(Method::loadText(strInfo));
  for (int i = 0; i < txtEdit->document()->lineCount(); i++) {
    QString str0 = Method::getTextEditLineText(txtEdit, i).trimmed();
    str0.replace("</key>", "");
    str0.replace("<key>", "");
    if (str0 == "CFBundleVersion") {
      QString str1 = Method::getTextEditLineText(txtEdit, i + 1).trimmed();
      str1.replace("<string>", "");
      str1.replace("</string>", "");
      return str1;
    }
  }

  return "";
}

void MainWindow::on_btnOpen() {
  QFileDialog fd;

  PlistFileName = fd.getOpenFileName(this, "Config file", "",
                                     "Config file(*.plist);;All files(*.*)");

  openFile(PlistFileName);
}

void MainWindow::ParserACPI(QVariantMap map) {
  map = map["ACPI"].toMap();
  if (map.isEmpty()) return;

  // 分析"Add"
  QVariantList map_add = map["Add"].toList();
  Method::set_TableData(ui->table_acpi_add, map_add);

  // 分析Delete
  QVariantList map_del = map["Delete"].toList();
  Method::set_TableData(ui->table_acpi_del, map_del);

  // 分析Patch
  QVariantList map_patch = map["Patch"].toList();
  Method::set_TableData(ui->table_acpi_patch, map_patch);

  // 分析Quirks
  QVariantMap map_quirks = map["Quirks"].toMap();
  getValue(map_quirks, ui->tabACPI4);
}

void MainWindow::initui_ACPI() {
  ui->btnExportMaster->setText(tr("Export") + "  ACPI");
  ui->btnImportMaster->setText(tr("Import") + "  ACPI");

  mymethod->init_PresetQuirks(ui->comboBoxACPI, "ACPI-Quirks.txt");

  // ACPI-Add
  Method::init_Table(ui->table_acpi_add,
                     Method::get_HorizontalHeaderList("ACPI", "Add"));

  // ACPI-Delete
  Method::init_Table(ui->table_acpi_del,
                     Method::get_HorizontalHeaderList("ACPI", "Delete"));

  // ACPI-Patch
  Method::init_Table(ui->table_acpi_patch,
                     Method::get_HorizontalHeaderList("ACPI", "Patch"));
}

void MainWindow::initui_Booter() {
  mymethod->init_PresetQuirks(ui->comboBoxBooter, "Booter-Quirks.txt");

  // MmioWhitelist
  Method::init_Table(ui->table_booter, Method::get_HorizontalHeaderList(
                                           "Booter", "MmioWhitelist"));

  // Patch
  Method::init_Table(ui->table_Booter_patch,
                     Method::get_HorizontalHeaderList("Booter", "Patch"));
}

void MainWindow::ParserBooter(QVariantMap map) {
  map = map["Booter"].toMap();
  if (map.isEmpty()) return;

  // Patch
  QVariantList map_patch = map["Patch"].toList();
  Method::set_TableData(ui->table_Booter_patch, map_patch);

  // MmioWhitelist
  QVariantList map_mwl = map["MmioWhitelist"].toList();
  Method::set_TableData(ui->table_booter, map_mwl);

  // Quirks
  QVariantMap map_quirks = map["Quirks"].toMap();
  getValue(map_quirks, ui->tabBooter3);
  if (map_quirks["ResizeAppleGpuBars"].toString() == "")
    ui->editIntResizeAppleGpuBars->setText("-1");
}

void MainWindow::initui_DP() {
  // Add
  ui->table_dp_add0->setMinimumWidth(200);
  ui->table_dp_add0->setMaximumWidth(550);
  Method::init_Table(ui->table_dp_add0, QStringList() << "PCILists");
  ui->table_dp_add0->horizontalHeader()->setStretchLastSection(true);

  Method::init_Table(ui->table_dp_add, QStringList() << "Key"
                                                     << "Data Type"
                                                     << "Value");
  // QSplitter* splitterMain = new QSplitter(Qt::Horizontal, this);
  // splitterMain->addWidget(ui->table_dp_add0);
  // splitterMain->addWidget(ui->table_dp_add);
  // ui->gridLayout_dp_add->addWidget(splitterMain);

  // Delete
  Method::init_Table(ui->table_dp_del0, QStringList() << "PCILists");
  ui->table_dp_del0->horizontalHeader()->setStretchLastSection(true);

  Method::init_Table(ui->table_dp_del, QStringList() << "Value");
  ui->table_dp_del->horizontalHeader()->setStretchLastSection(true);
}

void MainWindow::ParserDP(QVariantMap map) {
  map = map["DeviceProperties"].toMap();
  if (map.isEmpty()) return;

  // Add
  QVariantMap map_add = map["Add"].toMap();
  QVariantMap map_sub;
  ui->table_dp_add0->setRowCount(map_add.count());

  for (int i = 0; i < map_add.count(); i++) {
    QString strAdd0 = map_add.keys().at(i).trimmed();
    ui->table_dp_add0->setItem(i, 0, new QTableWidgetItem(strAdd0));

    map_sub = map_add[map_add.keys().at(i)].toMap();
    ui->table_dp_add->setRowCount(map_sub.keys().count());

    // Add Sub items
    for (int j = 0; j < map_sub.keys().count(); j++) {
      // Key
      QTableWidgetItem* newItem1 = new QTableWidgetItem(map_sub.keys().at(j));
      ui->table_dp_add->setItem(j, 0, newItem1);

      // Data type
      QString dtype = map_sub[map_sub.keys().at(j)].typeName();
      QString ztype;
      if (dtype == "QByteArray") ztype = "Data";
      if (dtype == "QString") ztype = "String";
      if (dtype == "qlonglong") ztype = "Number";
      QTableWidgetItem* newItem2 = new QTableWidgetItem(ztype);
      newItem2->setTextAlignment(Qt::AlignCenter);
      ui->table_dp_add->setItem(j, 1, newItem2);

      // Value
      QTableWidgetItem* newItem3;
      QString type_name = map_sub[map_sub.keys().at(j)].typeName();
      if (type_name == "QByteArray") {
        QByteArray tohex = map_sub[map_sub.keys().at(j)].toByteArray();
        QString va = tohex.toHex().toUpper();
        newItem3 = new QTableWidgetItem(va);
      } else
        newItem3 =
            new QTableWidgetItem(map_sub[map_sub.keys().at(j)].toString());
      ui->table_dp_add->setItem(j, 2, newItem3);
    }

    // 保存子条目里面的数据，以便以后加载
    for (int n = 0; n < ui->table_dp_add->rowCount(); n++) {
      listDPAdd.append(strAdd0 + "*|*" +
                       ui->table_dp_add->item(n, 0)->text().trimmed() + "*|*" +
                       ui->table_dp_add->item(n, 1)->text().trimmed() + "*|*" +
                       ui->table_dp_add->item(n, 2)->text().trimmed());
    }
  }

  int last = ui->table_dp_add0->rowCount();
  ui->table_dp_add0->setCurrentCell(last - 1, 0);

  // Delete
  init_value(map["Delete"].toMap(), ui->table_dp_del0, ui->table_dp_del, -1);
}

void MainWindow::initui_Kernel() {
  mymethod->init_PresetQuirks(ui->comboBoxKernel, "Kernel-Quirks.txt");

  // Add
  Method::init_Table(ui->table_kernel_add,
                     Method::get_HorizontalHeaderList("Kernel", "Add"));

  // Block
  Method::init_Table(ui->table_kernel_block,
                     Method::get_HorizontalHeaderList("Kernel", "Block"));

  // Force
  Method::init_Table(ui->table_kernel_Force,
                     Method::get_HorizontalHeaderList("Kernel", "Force"));

  // Patch
  Method::init_Table(ui->table_kernel_patch,
                     Method::get_HorizontalHeaderList("Kernel", "Patch"));

  // Scheme
  ui->cboxKernelArch->clear();
  ui->cboxKernelArch->addItem("Auto");
  ui->cboxKernelArch->addItem("i386");
  ui->cboxKernelArch->addItem("i386-user32");
  ui->cboxKernelArch->addItem("x86_64");
  ui->cboxKernelCache->clear();
  ui->cboxKernelCache->addItem("Auto");
  ui->cboxKernelCache->addItem("Cacheless");
  ui->cboxKernelCache->addItem("Mkext");
  ui->cboxKernelCache->addItem("Prelinked");
}

void MainWindow::ParserKernel(QVariantMap map, QString subitem) {
  map = map["Kernel"].toMap();
  if (map.isEmpty()) return;

  // Add
  if (subitem == "Add") {
    QVariantList map_add = map["Add"].toList();
    Method::set_TableData(ui->table_kernel_add, map_add);
  }

  // Block
  if (subitem == "Block") {
    QVariantList map_block = map["Block"].toList();
    Method::set_TableData(ui->table_kernel_block, map_block);
  }

  // Force
  if (subitem == "Force") {
    QVariantList map_Force = map["Force"].toList();
    Method::set_TableData(ui->table_kernel_Force, map_Force);
  }

  // Patch
  if (subitem == "Patch") {
    QVariantList map_patch = map["Patch"].toList();
    Method::set_TableData(ui->table_kernel_patch, map_patch);
  }

  // Emulate
  if (subitem == "Emulate") {
    QVariantMap map_Emulate = map["Emulate"].toMap();
    getValue(map_Emulate, ui->tabKernel5);
  }

  // Quirks
  if (subitem == "Quirks") {
    QVariantMap map_quirks = map["Quirks"].toMap();
    getValue(map_quirks, ui->tabKernel6);
  }

  // Scheme
  if (subitem == "Scheme") {
    QVariantMap map_Scheme = map["Scheme"].toMap();
    getValue(map_Scheme, ui->tabKernel7);
  }
}

void MainWindow::initui_Misc() {
  // Boot
  ui->cboxHibernateMode->addItem("None");
  ui->cboxHibernateMode->addItem("Auto");
  ui->cboxHibernateMode->addItem("RTC");
  ui->cboxHibernateMode->addItem("NVRAM");

  ui->cboxLauncherOption->addItem("Disabled");
  ui->cboxLauncherOption->addItem("Full");
  ui->cboxLauncherOption->addItem("Short");
  ui->cboxLauncherOption->addItem("System");

  ui->cboxLauncherPath->addItem("Default");

  ui->cboxPickerMode->addItem("Builtin");
  ui->cboxPickerMode->addItem("External");
  ui->cboxPickerMode->addItem("Apple");

  QPalette pe;
  pe = ui->lblColorEffect->palette();
  pe.setColor(QPalette::Window, Qt::black);
  ui->lblColorEffect->setAutoFillBackground(true);
  pe.setColor(QPalette::WindowText, Qt::white);
  ui->lblColorEffect->setPalette(pe);

  textColor.append("#000000");
  textColor.append("#000098");
  textColor.append("#009800");
  textColor.append("#009898");
  textColor.append("#980000");
  textColor.append("#980098");
  textColor.append("#989800");
  textColor.append("#bfbfbf");
  textColor.append("#303030");
  textColor.append("#0000ff");
  textColor.append("#00ff00");
  textColor.append("#00ffff");
  textColor.append("#ff0000");
  textColor.append("#ff00ff");
  textColor.append("#ffff00");
  textColor.append("#ffffff");

  backColor.append("#000000");
  backColor.append("#000098");
  backColor.append("#009800");
  backColor.append("#009898");
  backColor.append("#980000");
  backColor.append("#980098");
  backColor.append("#989800");
  backColor.append("#bfbfbf");

  // 添加颜色下拉框,字色
  QStringList itemList;
  for (int i = 0; i < ui->mycboxTextColor->count(); i++) {
    itemList.append(ui->mycboxTextColor->itemText(i));
  }
  ui->mycboxTextColor->clear();

  for (int i = 0; i < textColor.count(); i++) {
    QPixmap pix(QSize(100, 20));
    pix.fill(QColor(textColor.at(i)));
    ui->mycboxTextColor->addItem(QIcon(pix), itemList.at(i));
    ui->mycboxTextColor->setIconSize(QSize(70, 20));
    ui->mycboxTextColor->setSizeAdjustPolicy(QComboBox::AdjustToContents);
  }

  // 背景色
  itemList.clear();
  for (int i = 0; i < ui->mycboxBackColor->count(); i++) {
    itemList.append(ui->mycboxBackColor->itemText(i));
  }
  ui->mycboxBackColor->clear();

  for (int i = 0; i < backColor.count(); i++) {
    QPixmap pix(QSize(100, 20));
    pix.fill(QColor(backColor.at(i)));
    ui->mycboxBackColor->addItem(QIcon(pix), itemList.at(i));
    ui->mycboxBackColor->setIconSize(QSize(70, 20));
    ui->mycboxBackColor->setSizeAdjustPolicy(QComboBox::AdjustToContents);
  }

  // Debug

  // Security

  ui->btnGetPassHash->setEnabled(false);

  ui->cboxDmgLoading->addItem("Disabled");
  ui->cboxDmgLoading->addItem("Signed");
  ui->cboxDmgLoading->addItem("Any");

  ui->cboxVault->addItem("Optional");
  ui->cboxVault->addItem("Basic");
  ui->cboxVault->addItem("Secure");

  ui->cboxSecureBootModel->setEditable(true);
}

void MainWindow::ParserMisc(QVariantMap map) {
  map = map["Misc"].toMap();
  if (map.isEmpty()) return;

  // Boot
  QVariantMap map_boot = map["Boot"].toMap();
  getValue(map_boot, ui->tabMisc1);

  QObjectList listOfLineEdit = getAllLineEdit(getAllUIControls(ui->tabMisc1));
  for (int i = 0; i < listOfLineEdit.count(); i++) {
    QLineEdit* w = (QLineEdit*)listOfLineEdit.at(i);
    if (w->objectName() == "editShowPicker") {
      if (w->text() == "true" || w->text() == "false") w->setText("Always");
    }
  }

  ui->editIntConsoleAttributes->setText(
      map_boot["ConsoleAttributes"].toString());

  QString hm = map_boot["HibernateMode"].toString();

  // Debug
  QVariantMap map_debug = map["Debug"].toMap();
  getValue(map_debug, ui->tabMisc2);

  // Security
  QVariantMap map_security = map["Security"].toMap();
  getValue(map_security, ui->tabMisc3);

  hm = map_security["SecureBootModel"].toString().trimmed();
  if (hm == "") hm = "Disabled";

  for (int i = 0; i < ui->cboxSecureBootModel->count(); i++) {
    QString str = ui->cboxSecureBootModel->itemText(i);
    if (str == hm) ui->cboxSecureBootModel->setCurrentIndex(i);
  }

  // BlessOverride
  QVariantList map_bo = map["BlessOverride"].toList();
  Method::set_TableData(ui->tableBlessOverride, map_bo);

  // Entries
  QVariantList map_Entries = map["Entries"].toList();
  Method::set_TableData(ui->tableEntries, map_Entries);

  // Tools
  QVariantList map_Tools = map["Tools"].toList();
  Method::set_TableData(ui->tableTools, map_Tools);

  // Serial
  QVariantMap map_Serial = map["Serial"].toMap();
  getValue(map_Serial, ui->frame_chk_29);

  // Serial-Custom
  QVariantMap mapSerialCustom = map_Serial["Custom"].toMap();
  getValue(mapSerialCustom, ui->gboxCustom);
  if (ui->editDatPciDeviceInfo->text().trimmed() == "")
    ui->editDatPciDeviceInfo->setText("FF");
}

void MainWindow::initui_NVRAM() {
  // Add
  ui->table_nv_add0->setMinimumWidth(300);
  ui->table_nv_add0->setMaximumWidth(400);
  Method::init_Table(ui->table_nv_add0, QStringList() << "UUID");
  ui->table_nv_add0->horizontalHeader()->setStretchLastSection(true);

  Method::init_Table(ui->table_nv_add, QStringList() << "Key"
                                                     << "Data Type"
                                                     << "Value");

  // 分割窗口
  // QSplitter* splitterMain = new QSplitter(Qt::Horizontal, this);
  // splitterMain->addWidget(ui->table_nv_add0);
  // splitterMain->addWidget(ui->table_nv_add);
  // ui->gridLayout_nv_add->addWidget(splitterMain);

  // Delete
  Method::init_Table(ui->table_nv_del0, QStringList() << "UUID");
  ui->table_nv_del0->horizontalHeader()->setStretchLastSection(true);

  Method::init_Table(ui->table_nv_del, QStringList() << "Value");
  ui->table_nv_del->horizontalHeader()->setStretchLastSection(true);

  // LegacySchema
  Method::init_Table(ui->table_nv_ls0, QStringList() << "UUID");
  ui->table_nv_ls0->horizontalHeader()->setStretchLastSection(true);

  Method::init_Table(ui->table_nv_ls, QStringList() << "Value");
  ui->table_nv_ls->horizontalHeader()->setStretchLastSection(true);
}

void MainWindow::AddNvramAdd(QVariantMap map_add, int currentRow,
                             bool blPreset) {
  int i = 0;
  if (blPreset)
    i = ui->table_nv_add0->rowCount() - 1;
  else
    i = currentRow;

  QString strAdd0 = map_add.keys().at(currentRow);
  ui->table_nv_add0->setItem(i, 0, new QTableWidgetItem(strAdd0));

  // Add Sub
  QVariantMap map_sub;
  map_sub = map_add[map_add.keys().at(currentRow)].toMap();
  ui->table_nv_add->setRowCount(map_sub.keys().count());  // Sub key total

  for (int j = 0; j < map_sub.keys().count(); j++) {
    ui->table_nv_add->setItem(
        j, 0, new QTableWidgetItem(map_sub.keys().at(j)));  // Key

    QString dtype = map_sub[map_sub.keys().at(j)].typeName();
    QString ztype;
    if (dtype == "QByteArray") ztype = "Data";
    if (dtype == "QString") ztype = "String";
    if (dtype == "qlonglong") ztype = "Number";
    if (dtype == "bool") ztype = "Boolean";
    QTableWidgetItem* newItem1 = new QTableWidgetItem(ztype);  // Data type
    newItem1->setTextAlignment(Qt::AlignCenter);
    ui->table_nv_add->setItem(j, 1, newItem1);

    QString type_name = map_sub[map_sub.keys().at(j)].typeName();
    QTableWidgetItem* newItem2;
    if (type_name == "QByteArray") {
      QByteArray tohex = map_sub[map_sub.keys().at(j)].toByteArray();
      QString va = tohex.toHex().toUpper();
      newItem2 = new QTableWidgetItem(va);
    } else
      newItem2 = new QTableWidgetItem(map_sub[map_sub.keys().at(j)].toString());
    ui->table_nv_add->setItem(j, 2, newItem2);
  }

  for (int n = 0; n < ui->table_nv_add->rowCount(); n++) {
    listNVRAMAdd.append(strAdd0 + "*|*" +
                        ui->table_nv_add->item(n, 0)->text().trimmed() + "*|*" +
                        ui->table_nv_add->item(n, 1)->text().trimmed() + "*|*" +
                        ui->table_nv_add->item(n, 2)->text().trimmed());
  }
}

void MainWindow::ParserNvram(QVariantMap map) {
  map = map["NVRAM"].toMap();
  if (map.isEmpty()) return;

  // Root
  getValue(map, ui->gboxNVRAM);

  // Add
  QVariantMap map_add;
  map_add = map["Add"].toMap();
  ui->table_nv_add0->setRowCount(map_add.count());
  for (int i = 0; i < map_add.count(); i++) {
    AddNvramAdd(map_add, i, false);
  }

  int last = ui->table_nv_add0->rowCount();
  ui->table_nv_add0->setCurrentCell(last - 1, 0);

  // Delete
  init_value(map["Delete"].toMap(), ui->table_nv_del0, ui->table_nv_del, -1);

  // LegacySchema
  init_value(map["LegacySchema"].toMap(), ui->table_nv_ls0, ui->table_nv_ls,
             -1);
}

void MainWindow::write_ini(QTableWidget* table, QTableWidget* mytable, int i) {
  QString name = table->item(i, 0)->text().trimmed();
  if (name == "") name = "Item" + QString::number(i + 1);

  name = name.replace("/", "-");

  QString plistPath =
      strConfigPath + CurrentDateTime + table->objectName() + name + ".ini";

  QFile file(plistPath);
  if (file.exists()) file.remove();

  QSettings Reg(plistPath, QSettings::IniFormat);

  for (int k = 0; k < mytable->rowCount(); k++) {
    Reg.setValue(QString::number(k + 1) + "/key", mytable->item(k, 0)->text());
    Reg.setValue(QString::number(k + 1) + "/class",
                 mytable->item(k, 1)->text());
    Reg.setValue(QString::number(k + 1) + "/value",
                 mytable->item(k, 2)->text());
  }

  Reg.setValue("total", mytable->rowCount());

  IniFile.push_back(plistPath);
}

void MainWindow::read_ini(QTableWidget* table, QTableWidget* mytable, int i) {
  QString name = table->item(i, 0)->text().trimmed();
  if (name == "") name = "Item" + QString::number(i + 1);

  name = name.replace("/", "-");

  QString plistPath =
      strConfigPath + CurrentDateTime + table->objectName() + name + ".ini";

  QFile file(plistPath);
  if (file.exists()) {
    QSettings Reg(plistPath, QSettings::IniFormat);
    int count = Reg.value("total").toInt();
    mytable->setRowCount(count);
    QTableWidgetItem* newItem1;
    for (int k = 0; k < count; k++) {
      newItem1 = new QTableWidgetItem(
          Reg.value(QString::number(k + 1) + "/key").toString());
      mytable->setItem(k, 0, newItem1);

      newItem1 = new QTableWidgetItem(
          Reg.value(QString::number(k + 1) + "/class").toString());
      newItem1->setTextAlignment(Qt::AlignCenter);
      mytable->setItem(k, 1, newItem1);

      newItem1 = new QTableWidgetItem(
          Reg.value(QString::number(k + 1) + "/value").toString());
      mytable->setItem(k, 2, newItem1);
    }
  }
}

void MainWindow::write_value_ini(QTableWidget* table, QTableWidget* subtable,
                                 int i) {
  QString name = table->item(i, 0)->text().trimmed();
  if (name == "") name = "Item" + QString::number(i + 1);
  name = name.replace("/", "-");

  QString plistPath =
      strConfigPath + CurrentDateTime + table->objectName() + name + ".ini";

  QFile file(plistPath);
  if (file.exists()) file.remove();

  QSettings Reg(plistPath, QSettings::IniFormat);

  for (int k = 0; k < subtable->rowCount(); k++) {
    Reg.setValue(QString::number(k + 1) + "/key", subtable->item(k, 0)->text());
  }

  Reg.setValue("total", subtable->rowCount());

  IniFile.push_back(plistPath);
}

void MainWindow::read_value_ini(QTableWidget* table, QTableWidget* mytable,
                                int i) {
  QString name = table->item(i, 0)->text().trimmed();
  if (name == "") name = "Item" + QString::number(i + 1);

  name = name.replace("/", "-");

  QString plistPath =
      strConfigPath + CurrentDateTime + table->objectName() + name + ".ini";

  QFile file(plistPath);
  if (file.exists()) {
    QSettings Reg(plistPath, QSettings::IniFormat);
    int count = Reg.value("total").toInt();
    mytable->setRowCount(count);
    QTableWidgetItem* newItem1;
    for (int k = 0; k < count; k++) {
      newItem1 = new QTableWidgetItem(
          Reg.value(QString::number(k + 1) + "/key").toString());
      mytable->setItem(k, 0, newItem1);
    }
  }
}

void MainWindow::on_table_dp_add0_cellClicked(int row, int column) {
  Q_UNUSED(row);
  Q_UNUSED(column);

  readLeftTable(ui->table_dp_add0, ui->table_dp_add);

  setStatusBarText(ui->table_dp_add0);
}

void MainWindow::readLeftTable(QTableWidget* t0, QTableWidget* t) {
  if (!t0->currentIndex().isValid()) return;
  blReadLeftTable = true;
  bool md = isWindowModified();
  QString strLeft = t0->currentItem()->text().trimmed();
  QStringList listAdd;
  if (t0 == ui->table_dp_add0) listAdd = listDPAdd;
  if (t0 == ui->table_nv_add0) listAdd = listNVRAMAdd;
  // for (int i = 0; i < listAdd.count(); i++) qDebug() << listAdd.at(i);
  t->setRowCount(0);
  for (int i = 0; i < listAdd.count(); i++) {
    QString str = listAdd.at(i);
    QStringList list = str.split("*|*");
    if (list.count() == 4) {
      if (strLeft == list.at(0)) {
        int count = t->rowCount();
        t->setRowCount(count + 1);
        t->setItem(count, 0, new QTableWidgetItem(list.at(1)));
        QTableWidgetItem* newItem1 = new QTableWidgetItem(list.at(2));
        newItem1->setTextAlignment(Qt::AlignCenter);
        t->setItem(count, 1, newItem1);
        t->setItem(count, 2, new QTableWidgetItem(list.at(3)));
      }
    }
  }
  blReadLeftTable = false;
  setWindowModified(md);
  updateIconStatus();
}

void MainWindow::on_table_dp_add_itemChanged(QTableWidgetItem* item) {
  Q_UNUSED(item);

  if (writeINI) {
    mymethod->writeLeftTable(ui->table_dp_add0, ui->table_dp_add);

    this->setWindowModified(true);
    updateIconStatus();
  }
}

void MainWindow::on_table_nv_add0_cellClicked(int row, int column) {
  Q_UNUSED(row);
  Q_UNUSED(column);

  readLeftTable(ui->table_nv_add0, ui->table_nv_add);
}

void MainWindow::on_table_nv_add_itemChanged(QTableWidgetItem* item) {
  Q_UNUSED(item);

  if (writeINI) mymethod->writeLeftTable(ui->table_nv_add0, ui->table_nv_add);

  if (!loading) {
    this->setWindowModified(true);
    updateIconStatus();
  }

  if (!Initialization) {
    if (!ui->table_nv_add->currentIndex().isValid()) return;
    if (ui->table_nv_add->rowCount() > 0) {
      int row = ui->table_nv_add->currentRow();
      if (ui->table_nv_add->item(row, 0)->text() == "SystemAudioVolume") {
        if (ui->table_nv_add->currentColumn() == 2) {
          checkSystemAudioVolume();
        }
      }
    }
  }
}

void MainWindow::init_value(QVariantMap map_fun, QTableWidget* table,
                            QTableWidget* subtable, int currentRow) {
  QTableWidgetItem* newItem1;
  QStringList list;
  if (currentRow == -1) {
    table->setRowCount(map_fun.count());
    subtable->setRowCount(0);

    for (int i = 0; i < map_fun.count(); i++) {
      QString str0 = map_fun.keys().at(i);
      newItem1 = new QTableWidgetItem(str0);
      table->setItem(i, 0, newItem1);

      // 加载子条目
      QVariantList map_sub_list =
          map_fun[map_fun.keys().at(i)].toList();   // 是个数组
      subtable->setRowCount(map_sub_list.count());  // 子键的个数
      for (int j = 0; j < map_sub_list.count(); j++) {
        newItem1 = new QTableWidgetItem(map_sub_list.at(j).toString());  // 键
        subtable->setItem(j, 0, newItem1);
        // qDebug() << subtable->item(j, 0)->text();
      }

      // 保存子条目里面的数据，以便以后加载
      for (int n = 0; n < subtable->rowCount(); n++) {
        list.append(table->objectName() + "*|*" + str0 + "*|*" +
                    subtable->item(n, 0)->text().trimmed());
      }
    }

    if (table == ui->table_nv_del0) listNVRAMDel = list;
    if (table == ui->table_nv_ls0) listNVRAMLs = list;
    if (table == ui->table_dp_del0) listDPDel = list;
    // for (int m = 0; m < list.count(); m++) qDebug() << list.at(m);

  } else {
    QString str0 = map_fun.keys().at(currentRow);
    newItem1 = new QTableWidgetItem(str0);
    table->setItem(table->rowCount() - 1, 0, newItem1);

    // 加载子条目
    QVariantList map_sub_list =
        map_fun[map_fun.keys().at(currentRow)].toList();  // 是个数组
    subtable->setRowCount(map_sub_list.count());          // 子键的个数
    for (int j = 0; j < map_sub_list.count(); j++) {
      newItem1 = new QTableWidgetItem(map_sub_list.at(j).toString());  // 键
      subtable->setItem(j, 0, newItem1);
    }

    // 保存子条目里面的数据，以便以后加载
    //  write_value_ini(table, subtable, table->rowCount() - 1);
  }

  int last = table->rowCount();
  table->setCurrentCell(last - 1, 0);
}

void MainWindow::on_table_nv_del0_cellClicked(int row, int column) {
  Q_UNUSED(row);
  Q_UNUSED(column);

  // loadRightTable(ui->table_nv_del0, ui->table_nv_del);
  readLeftTableOnlyValue(ui->table_nv_del0, ui->table_nv_del);
}

void MainWindow::on_table_nv_ls0_cellClicked(int row, int column) {
  Q_UNUSED(row);
  Q_UNUSED(column);

  // loadRightTable(ui->table_nv_ls0, ui->table_nv_ls);
  readLeftTableOnlyValue(ui->table_nv_ls0, ui->table_nv_ls);
}

void MainWindow::on_table_nv_del_itemChanged(QTableWidgetItem* item) {
  if (item->text().isEmpty()) {
  }

  if (writeINI) {
    // write_value_ini(ui->table_nv_del0, ui->table_nv_del,
    //                ui->table_nv_del0->currentRow());
    mymethod->writeLeftTableOnlyValue(ui->table_nv_del0, ui->table_nv_del);

    this->setWindowModified(true);
    updateIconStatus();
  }
}

void MainWindow::on_table_nv_ls_itemChanged(QTableWidgetItem* item) {
  if (item->text().isEmpty()) {
  }

  if (writeINI) {
    mymethod->writeLeftTableOnlyValue(ui->table_nv_ls0, ui->table_nv_ls);

    this->setWindowModified(true);
    updateIconStatus();
  }
}

void MainWindow::on_table_dp_del0_cellClicked(int row, int column) {
  Q_UNUSED(row);
  Q_UNUSED(column);

  readLeftTableOnlyValue(ui->table_dp_del0, ui->table_dp_del);

  setStatusBarText(ui->table_dp_del0);
}

void MainWindow::readLeftTableOnlyValue(QTableWidget* t0, QTableWidget* t) {
  if (!t0->currentIndex().isValid()) return;
  blReadLeftTable = true;
  bool md = isWindowModified();
  QString strLeft = t0->currentItem()->text().trimmed();
  QStringList listAdd;

  if (t0 == ui->table_dp_del0) listAdd = listDPDel;
  if (t0 == ui->table_nv_del0) listAdd = listNVRAMDel;
  if (t0 == ui->table_nv_ls0) listAdd = listNVRAMLs;
  t->setRowCount(0);
  for (int i = 0; i < listAdd.count(); i++) {
    QString str = listAdd.at(i);
    QStringList list = str.split("*|*");
    if (list.count() == 3) {
      if (strLeft == list.at(1) && t0->objectName() == list.at(0)) {
        int count = t->rowCount();
        t->setRowCount(count + 1);
        t->setItem(count, 0, new QTableWidgetItem(list.at(2)));
      }
    }
  }
  blReadLeftTable = false;
  setWindowModified(md);
  updateIconStatus();
}

void MainWindow::on_table_dp_del_itemChanged(QTableWidgetItem* item) {
  Q_UNUSED(item);

  if (writeINI) {
    mymethod->writeLeftTableOnlyValue(ui->table_dp_del0, ui->table_dp_del);

    this->setWindowModified(true);
    updateIconStatus();
  }
}

void MainWindow::initui_PlatformInfo() {
  ui->mychkSaveDataHub->setChecked(Reg.value("SaveDataHub", false).toBool());
  ui->actionAutoChkUpdate->setChecked(
      Reg.value("AutoChkUpdate", true).toBool());

  QFont font;
  font.setBold(true);
  ui->mychkSaveDataHub->setFont(font);

  ui->cboxUpdateSMBIOSMode->addItem("TryOverwrite");
  ui->cboxUpdateSMBIOSMode->addItem("Create");
  ui->cboxUpdateSMBIOSMode->addItem("Overwrite");
  ui->cboxUpdateSMBIOSMode->addItem("Custom");

  ui->cboxSystemMemoryStatus->addItem("Auto");
  ui->cboxSystemMemoryStatus->addItem("Upgradable");
  ui->cboxSystemMemoryStatus->addItem("Soldered");

  // SystemProductName
  QStringList pi;
  pi.push_back("");
  pi.push_back("MacPro1,1    Intel Core Xeon 5130 x2 @ 2.00 GHz");
  pi.push_back("MacPro2,1    Intel Xeon X5365 x2 @ 3.00 GHz");
  pi.push_back("MacPro3,1    Intel Xeon E5462 x2 @ 2.80 GHz");
  pi.push_back("MacPro4,1    Intel Xeon W3520 @ 2.66 GHz");
  pi.push_back("MacPro5,1    Intel Xeon W3530 @ 2.80 GHz");
  pi.push_back("MacPro6,1    Intel Xeon E5-1620 v2 @ 3.70 GHz");
  pi.push_back("MacPro7,1    Intel Xeon W-3245M CPU @ 3.20 GHz");

  pi.push_back("MacBook1,1    Intel Core Duo T2400 @ 1.83 GHz");
  pi.push_back("MacBook2,1    Intel Core 2 Duo T5600 @ 1.83 GHz");
  pi.push_back("MacBook3,1    Intel Core 2 Duo T7500 @ 2.20 GHz");
  pi.push_back("MacBook4,1    Intel Core 2 Duo T8300 @ 2.40 GHz");
  pi.push_back("MacBook5,1    Intel Core 2 Duo P8600 @ 2.40 GHz");
  pi.push_back("MacBook5,2    Intel Core 2 Duo P7450 @ 2.13 GHz");
  pi.push_back("MacBook6,1    Intel Core 2 Duo P7550 @ 2.26 GHz");
  pi.push_back("MacBook7,1    Intel Core 2 Duo P8600 @ 2.40 GHz");
  pi.push_back("MacBook8,1    Intel Core M 5Y51 @ 1.10 GHz");
  pi.push_back("MacBook9,1    Intel Core m3-6Y30 @ 1.10 GHz");
  pi.push_back("MacBook10,1  Intel Core m3-7Y32 @ 1.10 GHz");

  pi.push_back("MacBookAir1,1    Intel Core 2 Duo P7500 @ 1.60 GHz");
  pi.push_back("MacBookAir2,1    Intel Core 2 Duo SL9600 @ 2.13 GHz");
  pi.push_back("MacBookAir3,1    Intel Core 2 Duo SU9400 @ 1.40 GHz");
  pi.push_back("MacBookAir3,2    Intel Core 2 Duo SL9400 @ 1.86 GHz");
  pi.push_back("MacBookAir4,1    Intel Core i5-2467M @ 1.60 GHz");
  pi.push_back("MacBookAir4,2    Intel Core i5-2557M @ 1.70 GHz");
  pi.push_back("MacBookAir5,1    Intel Core i5-3317U @ 1.70 GHz");
  pi.push_back("MacBookAir5,2    Intel Core i5-3317U @ 1.70GHz");
  pi.push_back("MacBookAir6,1    Intel Core i5-4250U @ 1.30 GHz");
  pi.push_back("MacBookAir6,2    Intel Core i5-4250U @ 1.30 GHz");
  pi.push_back("MacBookAir7,1    Intel Core i5-5250U @ 1.60 GHz");
  pi.push_back("MacBookAir7,2    Intel Core i5-5250U @ 1.60 GHz");
  pi.push_back("MacBookAir8,1    Intel Core i5-8210Y @ 1.60 GHz");
  pi.push_back("MacBookAir8,2    Intel Core i5-8210Y @ 1.60 GHz");
  pi.push_back("MacBookAir9,1    Intel Core i3-1000NG4 @ 1.10 GHz");

  pi.push_back("MacBookPro1,1    Intel Core Duo L2400 @ 1.66 GHz");
  pi.push_back("MacBookPro1,2    Intel Core Duo T2600 @ 2.16 GHz");
  pi.push_back("MacBookPro2,1    Intel Core 2 Duo T7600 @ 2.33 GHz");
  pi.push_back("MacBookPro2,2    Intel Core 2 Duo T7400 @ 2.16 GHz");
  pi.push_back("MacBookPro3,1    Intel Core 2 Duo T7700 @ 2.40 GHz");
  pi.push_back("MacBookPro4,1    Intel Core 2 Duo T8300 @ 2.40 GHz");
  pi.push_back("MacBookPro5,1    Intel Core 2 Duo P8600 @ 2.40 GHz");
  pi.push_back("MacBookPro5,2    Intel Core 2 Duo T9600 @ 2.80 GHz");
  pi.push_back("MacBookPro5,3    Intel Core 2 Duo P8800 @ 2.66 GHz");
  pi.push_back("MacBookPro5,4    Intel Core 2 Duo P8700 @ 2.53 GHz");
  pi.push_back("MacBookPro5,5    Intel Core 2 Duo P7550 @ 2.26 GHz");
  pi.push_back("MacBookPro6,1    Intel Core i5-540M @ 2.53 GHz");
  pi.push_back("MacBookPro6,2    Intel Core i5-520M @ 2.40 GHz");
  pi.push_back("MacBookPro7,1    Intel Core 2 Duo P8600 @ 2.40 GHz");
  pi.push_back("MacBookPro8,1    Intel Core i5-2415M @ 2.30 GHz");
  pi.push_back("MacBookPro8,2    Intel Core i7-2675QM @ 2.20 GHz");
  pi.push_back("MacBookPro8,3    Intel Core i7-2820QM @ 2.30 GHz");
  pi.push_back("MacBookPro9,1    Intel Core i7-3615QM @ 2.30 GHz");
  pi.push_back("MacBookPro9,2    Intel Core i5-3210M @ 2.50 GHz");
  pi.push_back("MacBookPro10,1    Intel Core i7-3615QM @ 2.30 GHz");
  pi.push_back("MacBookPro10,2    Intel Core i5-3210M @ 2.50 GHz");
  pi.push_back("MacBookPro11,1    Intel Core i5-4258U @ 2.40 GHz");
  pi.push_back("MacBookPro11,2    Intel Core i7-4770HQ @ 2.20 GHz");
  pi.push_back("MacBookPro11,3    Intel Core i7-4850HQ @ 2.30 GHz");
  pi.push_back("MacBookPro11,4    Intel Core i7-4770HQ @ 2.20 GHz");
  pi.push_back("MacBookPro11,5    Intel Core i7-4870HQ @ 2.50 GHz");
  pi.push_back("MacBookPro12,1    Intel Core i5-5257U @ 2.70 GHz");
  pi.push_back("MacBookPro13,1    Intel Core i5-6360U @ 2.00 GHz");
  pi.push_back("MacBookPro13,2    Intel Core i7-6567U @ 3.30 GHz");
  pi.push_back("MacBookPro13,3    Intel Core i7-6700HQ @ 2.60 GHz");
  pi.push_back("MacBookPro14,1    Intel Core i5-7360U @ 2.30 GHz");
  pi.push_back("MacBookPro14,2    Intel Core i5-7267U @ 3.10 GHz");
  pi.push_back("MacBookPro14,3    Intel Core i7-7700HQ @ 2.80 GHz");
  pi.push_back("MacBookPro15,1    Intel Core i7-8750H @ 2.20 GHz");
  pi.push_back("MacBookPro15,2    Intel Core i7-8559U @ 2.70 GHz");
  pi.push_back("MacBookPro15,3    Intel Core i7-8850H @ 2.60 GHz");
  pi.push_back("MacBookPro15,4    Intel Core i5-8257U @ 1.40 GHz");
  pi.push_back("MacBookPro16,1    Intel Core i7-9750H @ 2.60 GHz");
  pi.push_back("MacBookPro16,2    Intel Core i5-1038NG7 @ 2.00 GHz");
  pi.push_back("MacBookPro16,3    Intel Core i5-8257U @ 1.40 GHz");
  pi.push_back("MacBookPro16,4    Intel Core i7-9750H @ 2.60 GHz");

  pi.push_back("Macmini1,1    Intel Core Solo T1200 @ 1.50 GHz");
  pi.push_back("Macmini2,1    Intel Core 2 Duo T5600 @ 1.83 GHz");
  pi.push_back("Macmini3,1    Intel Core 2 Duo P7350 @ 2.00 GHz");
  pi.push_back("Macmini4,1    Intel Core 2 Duo P8600 @ 2.40 GHz");
  pi.push_back("Macmini5,1    Intel Core i5-2415M @ 2.30 GHz");
  pi.push_back("Macmini5,2    Intel Core i5-2520M @ 2.50 GHz");
  pi.push_back("Macmini5,3    Intel Core i7-2635QM @ 2.00 GHz");
  pi.push_back("Macmini6,1    Intel Core i5-3210M @ 2.50 GHz");
  pi.push_back("Macmini6,2    Intel Core i7-3615QM @ 2.30 GHz");
  pi.push_back("Macmini7,1    Intel Core i5-4260U @ 1.40 GHz");
  pi.push_back("Macmini8,1    Intel Core i7-8700B @ 3.20 GHz");

  pi.push_back("Xserve1,1    Intel Xeon 5130 x2 @ 2.00 GHz");
  pi.push_back("Xserve2,1    Intel Xeon E5462 x2 @ 2.80 GHz");
  pi.push_back("Xserve3,1    Intel Xeon E5520 x2 @ 2.26 GHz");

  pi.push_back("iMacPro1,1    Intel Xeon W-2140B CPU @ 3.20 GHz");

  pi.push_back("iMac4,1    Intel Core Duo T2400 @ 1.83 GHz");
  pi.push_back("iMac4,2    Intel Core Duo T2400 @ 1.83 GHz");
  pi.push_back("iMac5,1    Intel Core 2 Duo T7200 @ 2.00 GHz");
  pi.push_back("iMac5,2    Intel Core 2 Duo T5600 @ 1.83 GHz");
  pi.push_back("iMac6,1    Intel Core 2 Duo T7400 @ 2.16 GHz");
  pi.push_back("iMac7,1    Intel Core 2 Duo T7300 @ 2.00 GHz");
  pi.push_back("iMac8,1    Intel Core 2 Duo E8435 @ 3.06 GHz");
  pi.push_back("iMac9,1    Intel Core 2 Duo E8135 @ 2.66 GHz");
  pi.push_back("iMac10,1    Intel Core 2 Duo E7600 @ 3.06 GHz");
  pi.push_back("iMac11,1    Intel Core i5-750 @ 2.66 GHz");
  pi.push_back("iMac11,2    Intel Core i3-540 @ 3.06 GHz");
  pi.push_back("iMac11,3    Intel Core i5-760 @ 2.80 GHz");
  pi.push_back("iMac12,1    Intel Core i5-2400S @ 2.50 GHz");
  pi.push_back("iMac12,2    Intel Core i7-2600 @ 3.40 GHz");
  pi.push_back("iMac13,1    Intel Core i7-3770S @ 3.10 GHz");
  pi.push_back("iMac13,2    Intel Core i5-3470S @ 2.90 GHz");
  pi.push_back("iMac13,3    Intel Core i5-3470S @ 2.90 GHz");
  pi.push_back("iMac14,1    Intel Core i5-4570R @ 2.70 GHz");
  pi.push_back("iMac14,2    Intel Core i7-4771 @ 3.50 GHz");
  pi.push_back("iMac14,3    Intel Core i5-4570S @ 2.90 GHz");
  pi.push_back("iMac14,4    Intel Core i5-4260U @ 1.40 GHz");
  pi.push_back("iMac15,1    Intel Core i7-4790k @ 4.00 GHz");
  pi.push_back("iMac16,1    Intel Core i5-5250U @ 1.60 GHz");
  pi.push_back("iMac16,2    Intel Core i5-5675R @ 3.10 GHz");
  pi.push_back("iMac17,1    Intel Core i5-6500 @ 3.20 GHz");
  pi.push_back("iMac18,1    Intel Core i5-7360U @ 2.30 GHz");
  pi.push_back("iMac18,2    Intel Core i5-7400 @ 3.00 GHz");
  pi.push_back("iMac18,3    Intel Core i5-7600K @ 3.80 GHz");
  pi.push_back("iMac19,1    Intel Core i9-9900K @ 3.60 GHz");
  pi.push_back("iMac19,2    Intel Core i5-8500 @ 3.00 GHz");
  pi.push_back("iMac20,1    Intel Core i5-10500 @ 3.10 GHz");
  pi.push_back("iMac20,2    Intel Core i9-10910 @ 3.60 GHz");

  ui->cboxSystemProductName->addItems(pi);

#ifdef Q_OS_MAC
  QProcess* si = new QProcess;
  si->start(userDataBaseDir + "mac/macserial", QStringList() << "-s");
  si->waitForFinished();
  ui->textMacInfo->clear();
  QString result = si->readAll();
  ui->textMacInfo->append(result);
#endif
}

void MainWindow::ParserPlatformInfo(QVariantMap map) {
  map = map["PlatformInfo"].toMap();
  if (map.isEmpty()) return;

  getValue(map, ui->gbox01);

  QString usm = map["UpdateSMBIOSMode"].toString();
  ui->cboxUpdateSMBIOSMode->setCurrentText(usm.trimmed());

  if (usm.trimmed() == "Custom") {
    ui->cboxUpdateSMBIOSMode->setCurrentText(usm.trimmed());
    ui->chkCustomSMBIOSGuid->setChecked(true);
  } else
    ui->chkCustomSMBIOSGuid->setChecked(false);

  // Generic
  QVariantMap mapGeneric = map["Generic"].toMap();
  getValue(mapGeneric, ui->tabPlatformInfo1);

  if (ui->cboxSystemMemoryStatus->currentText() == "")
    ui->cboxSystemMemoryStatus->setCurrentIndex(0);

  QString spn = mapGeneric["SystemProductName"].toString();
  ui->cboxSystemProductName->setCurrentText(spn);
  for (int i = 0; i < ui->cboxSystemProductName->count(); i++) {
    if (getSystemProductName(ui->cboxSystemProductName->itemText(i)) == spn) {
      ui->cboxSystemProductName->setCurrentIndex(i);

      break;
    }
  }

  // DataHub
  QVariantMap mapDataHub = map["DataHub"].toMap();
  getValue(mapDataHub, ui->tabPlatformInfo2);

  if (ui->cboxSystemProductName->currentText() == "") {
    spn = mapDataHub["SystemProductName"].toString();
    ui->cboxSystemProductName->setCurrentText(spn);
    for (int i = 0; i < ui->cboxSystemProductName->count(); i++) {
      if (getSystemProductName(ui->cboxSystemProductName->itemText(i)) == spn) {
        ui->cboxSystemProductName->setCurrentIndex(i);

        break;
      }
    }
  }

  if (ui->editSystemSerialNumber->text() == "")
    ui->editSystemSerialNumber->setText(
        ui->editSystemSerialNumber_data->text());
  if (ui->editSystemUUID->text() == "")
    ui->editSystemUUID->setText(ui->editSystemUUID_DataHub->text());

  // Memory
  QVariantMap mapMemory = map["Memory"].toMap();
  getValue(mapMemory, ui->tabPlatformInfo3);

  // Memory-Devices
  QVariantList mapMemoryDevices = mapMemory["Devices"].toList();
  Method::set_TableData(ui->tableDevices, mapMemoryDevices);

  // PlatformNVRAM
  QVariantMap mapPlatformNVRAM = map["PlatformNVRAM"].toMap();
  getValue(mapPlatformNVRAM, ui->tabPlatformInfo4);

  if (ui->editMLB->text() == "")
    ui->editMLB->setText(ui->editMLB_PNVRAM->text());
  if (ui->editDatROM->text() == "")
    ui->editDatROM->setText(ui->editDatROM_PNVRAM->text());
  else
    ui->editDatROM_PNVRAM->setText(ui->editDatROM->text());

  // SMBIOS
  QVariantMap mapSMBIOS = map["SMBIOS"].toMap();
  getValue(mapSMBIOS, ui->tabPlatformInfo5);
}

void MainWindow::initui_UEFI() {
  mymethod->init_PresetQuirks(ui->comboBoxUEFI, "UEFI-Quirks.txt");

  // APFS
  ui->calendarWidget->setVisible(false);

  // Audio
  QStringList list;
  list.append("Auto");
  list.append("Enabled");
  list.append("Disabled");
  ui->cboxPlayChime->addItems(list);
  ui->lblSystemAudioVolume->setText("");

  // Input
  ui->cboxKeySupportMode->clear();
  ui->cboxKeySupportMode->addItem("Auto");
  ui->cboxKeySupportMode->addItem("V1");
  ui->cboxKeySupportMode->addItem("V2");
  ui->cboxKeySupportMode->addItem("AMI");

  // Output
  ui->cboxConsoleMode->setEditable(true);
  ui->cboxConsoleMode->addItem("");
  ui->cboxConsoleMode->addItem("Max");

  ui->cboxResolution->setEditable(true);
  ui->cboxResolution->addItem("Max");

  ui->cboxTextRenderer->addItem("BuiltinGraphics");
  ui->cboxTextRenderer->addItem("SystemGraphics");
  ui->cboxTextRenderer->addItem("SystemText");
  ui->cboxTextRenderer->addItem("SystemGeneric");
}

void MainWindow::ParserUEFI(QVariantMap map) {
  map = map["UEFI"].toMap();
  if (map.isEmpty()) return;

  getValue(map, ui->gboxUEFI);
  // ui->chkConnectDrivers->setChecked(map["ConnectDrivers"].toBool());

  // 1. APFS
  QVariantMap map_apfs = map["APFS"].toMap();
  getValue(map_apfs, ui->tabUEFI1);
  if (ui->editIntMinDate->text() == "") ui->editIntMinDate->setText("-1");
  if (ui->editIntMinVersion->text() == "") ui->editIntMinVersion->setText("-1");

  // 2. AppleInput
  QVariantMap map_AppleInput = map["AppleInput"].toMap();
  getValue(map_AppleInput, ui->tabUEFI2);

  QObjectList listObj = getAllLineEdit(getAllUIControls(ui->tabUEFI2));
  for (int i = 0; i < listObj.count(); i++) {
    QLineEdit* w = (QLineEdit*)listObj.at(i);
    if (w->objectName() == "editCustomDelays") {
      if (w->text() == "true" || w->text() == "false") w->setText("Auto");
      break;
    }
  }

  if (ui->editIntKeyInitialDelay->text() == "")
    ui->editIntKeyInitialDelay->setText("0");

  if (ui->editIntKeySubsequentDelay->text() == "")
    ui->editIntKeySubsequentDelay->setText("5");

  if (ui->editIntPointerSpeedDiv->text() == "")
    ui->editIntPointerSpeedDiv->setText("1");

  if (ui->editIntPointerSpeedMul->text() == "")
    ui->editIntPointerSpeedMul->setText("1");

  // 3. Audio
  QVariantMap map_audio = map["Audio"].toMap();
  getValue(map_audio, ui->tabUEFI3);
  if (ui->cboxPlayChime->currentText() == "true")
    ui->cboxPlayChime->setCurrentText("Auto");

  // 4. Drivers
  QVariantList map_Drivers = map["Drivers"].toList();
  Method::set_TableData(ui->table_uefi_drivers, map_Drivers);

  // 5. Input
  QVariantMap map_input = map["Input"].toMap();
  getValue(map_input, ui->tabUEFI5);

  // 6. Output
  QVariantMap map_output = map["Output"].toMap();
  getValue(map_output, ui->tabUEFI6);
  QString strGop = ui->cboxGopPassThrough->currentText();
  if (strGop == "true" || strGop == "false")
    ui->cboxGopPassThrough->setCurrentIndex(0);
  if (ui->editIntUIScale->text() == "") ui->editIntUIScale->setText("-1");
  // 7. ProtocolOverrides
  QVariantMap map_po = map["ProtocolOverrides"].toMap();
  getValue(map_po, ui->tabUEFI7);

  // 8. Quirks
  QVariantMap map_uefi_Quirks = map["Quirks"].toMap();
  getValue(map_uefi_Quirks, ui->tabUEFI8);
  if (map_uefi_Quirks["ResizeGpuBars"].toString() == "")
    ui->editIntResizeGpuBars->setText("-1");

  // 9. ReservedMemory
  QVariantList mapRM = map["ReservedMemory"].toList();
  Method::set_TableData(ui->table_uefi_ReservedMemory, mapRM);

  // 10. Unload
  QVariantList mapUnload = map["Unload"].toList();
  Method::set_TableData(ui->table_uefi_Unload, mapUnload);
}

void MainWindow::on_btnSave() { SavePlist(SaveFileName); }

bool MainWindow::getBool(QTableWidget* table, int row, int column) {
  QString be = table->item(row, column)->text();
  if (be.trimmed() == "true" || be.trimmed().toLower() == "yes") return true;
  if (be.trimmed() == "false" || be.trimmed().toLower() == "no") return false;

  return false;
}

void MainWindow::SavePlist(QString FileName) {
  if (QFile(SaveFileName).exists()) {
    QString oldfile = QFileInfo(SaveFileName).path() + "/oldConfig.plist";
    QFile tempfile(oldfile);
    tempfile.remove();
    QFile::copy(SaveFileName, oldfile);
    FileSystemWatcher::removeWatchPath(SaveFileName);
  }
  lineEditSetText();
  removeAllLineEdit();
  mymethod->OCValidationProcessing();

  QVariantMap OpenCore;

  for (int i = 0; i < NoteKeys.count(); i++) {
    OpenCore[NoteKeys.at(i)] = NoteValues.at(i);
  }

  OpenCore["ACPI"] = SaveACPI();
  OpenCore["Booter"] = SaveBooter();
  OpenCore["DeviceProperties"] = SaveDeviceProperties();
  OpenCore["Kernel"] = SaveKernel();
  OpenCore["Misc"] = SaveMisc();
  OpenCore["NVRAM"] = SaveNVRAM();
  OpenCore["PlatformInfo"] = SavePlatformInfo();
  OpenCore["UEFI"] = SaveUEFI();

  QFile tempfile(FileName);
  tempfile.remove();

  PListSerializer::toPList(OpenCore, FileName);

  if (closeSave) {
    clear_temp_data();
  }

  if (!isGetEFI) {
    oc_Validate(false);
  }

  checkFiles(ui->table_acpi_add);
  checkFiles(ui->table_kernel_add);
  checkFiles(ui->tableTools);
  checkFiles(ui->table_uefi_drivers);

  FileSystemWatcher::addWatchPath(SaveFileName);
  strOrgMd5 = getMD5(SaveFileName);

  this->setWindowModified(false);
  updateIconStatus();
}

QVariantMap MainWindow::SaveACPI() {
  // Add
  QVariantMap acpiMap;
  QVariantMap mapMain = mapTatol["ACPI"].toMap();
  QStringList list = mapMain.keys();

  for (int i = 0; i < ui->table_acpi_add->rowCount(); i++) {
    QFileInfo fi(SaveFileName);
    QString str0, oldFile, newFile;
    str0 = ui->table_acpi_add->item(i, 0)->text();
    oldFile = fi.path() + "/ACPI/" + str0;
    if (str0.contains(" ")) {
      str0 = str0.replace(" ", "");
      ui->table_acpi_add->setItem(i, 0, new QTableWidgetItem(str0));

      newFile = fi.path() + "/ACPI/" + str0;
      QFileInfo fiReName(oldFile);
      if (fiReName.exists()) QFile::rename(oldFile, newFile);
    }
  }

  if (list.removeOne("Add"))
    acpiMap["Add"] = Method::get_TableData(ui->table_acpi_add);

  // Delete
  if (list.removeOne("Delete"))
    acpiMap["Delete"] = Method::get_TableData(ui->table_acpi_del);

  // Patch
  if (list.removeOne("Patch"))
    acpiMap["Patch"] = Method::get_TableData(ui->table_acpi_patch);

  // Quirks
  if (list.removeOne("Quirks")) {
    QVariantMap acpiQuirks;
    acpiMap["Quirks"] = setValue(acpiQuirks, ui->tabACPI4);
  }

  return acpiMap;
}

QVariantMap MainWindow::SaveBooter() {
  QVariantMap subMap;
  QVariantMap mapMain = mapTatol["Booter"].toMap();
  QStringList list = mapMain.keys();

  // MmioWhitelist
  if (list.removeOne("MmioWhitelist"))
    subMap["MmioWhitelist"] = Method::get_TableData(ui->table_booter);

  // Patch
  if (list.removeOne("Patch")) {
    subMap["Patch"] = Method::get_TableData(ui->table_Booter_patch);
  }

  // Quirks
  if (list.removeOne("Quirks")) {
    QVariantMap mapQuirks;
    subMap["Quirks"] = setValue(mapQuirks, ui->tabBooter3);
  }

  return subMap;
}

QVariantMap MainWindow::SaveDeviceProperties() {
  // Add
  QVariantMap subMap;
  QVariantMap dictList;
  QVariantMap valueList;
  QVariantList arrayList;

  int currentRow = ui->table_dp_add0->currentRow();
  int cuRowAdd = ui->table_dp_add->currentRow();
  int cuColAdd = ui->table_dp_add->currentColumn();
  for (int i = 0; i < ui->table_dp_add0->rowCount(); i++) {
    valueList.clear();  // 先必须清理下列表，很重要
    // 先加载表中的数据
    ui->table_dp_add0->setCurrentCell(i, 0);
    on_table_dp_add0_cellClicked(i, 0);

    for (int k = 0; k < ui->table_dp_add->rowCount(); k++) {
      QString dataType = ui->table_dp_add->item(k, 1)->text();  // 数据类型
      QString value = ui->table_dp_add->item(k, 2)->text();
      if (dataType == "String")
        valueList[ui->table_dp_add->item(k, 0)->text()] = value;
      if (dataType == "Data") {
        // 将以字符串方式显示的16进制原样转换成QByteArray
        valueList[ui->table_dp_add->item(k, 0)->text()] = HexStrToByte(value);
      }
      if (dataType == "Number")
        valueList[ui->table_dp_add->item(k, 0)->text()] = value.toLongLong();
    }
    dictList[ui->table_dp_add0->item(i, 0)->text()] = valueList;
  }
  subMap["Add"] = dictList;

  ui->table_dp_add0->clearSelection();
  ui->table_dp_add0->setCurrentCell(currentRow, 0);
  on_table_dp_add0_cellClicked(currentRow, 0);
  ui->table_dp_add->setCurrentCell(cuRowAdd, cuColAdd);
  ui->table_dp_add->setFocus();

  // Delete
  dictList.clear();
  currentRow = ui->table_dp_del0->currentRow();
  for (int i = 0; i < ui->table_dp_del0->rowCount(); i++) {
    valueList.clear();
    arrayList.clear();

    // 先加载表中的数据
    ui->table_dp_del0->setCurrentCell(i, 0);
    on_table_dp_del0_cellClicked(i, 0);

    for (int k = 0; k < ui->table_dp_del->rowCount(); k++) {
      arrayList.append(ui->table_dp_del->item(k, 0)->text());
    }
    dictList[ui->table_dp_del0->item(i, 0)->text()] = arrayList;
  }
  ui->table_dp_del0->clearSelection();
  ui->table_dp_del0->setCurrentCell(currentRow, 0);
  subMap["Delete"] = dictList;

  return subMap;
}

QVariantMap MainWindow::SaveKernel() {
  // Add
  QVariantMap subMap;
  QVariantMap mapMain = mapTatol["Kernel"].toMap();
  QStringList list = mapMain.keys();

  for (int i = 0; i < ui->table_kernel_add->rowCount(); i++) {
    QFileInfo fi(SaveFileName);
    QString str0, oldFile, newFile;
    str0 = ui->table_kernel_add->item(i, 0)->text();
    oldFile = fi.path() + "/Kexts/" + str0;
    if (str0.contains(" ")) {
      str0 = str0.replace(" ", "");
      ui->table_kernel_add->setItem(i, 0, new QTableWidgetItem(str0));

      newFile = fi.path() + "/Kexts/" + str0;
      QDir fiReName(oldFile);
      if (fiReName.exists()) {
        fiReName.rename(oldFile, newFile);
      }
    }
  }

  if (list.removeOne("Add"))
    subMap["Add"] = Method::get_TableData(ui->table_kernel_add);

  // Block
  if (list.removeOne("Block"))
    subMap["Block"] = Method::get_TableData(ui->table_kernel_block);

  // Force
  if (list.removeOne("Force"))
    subMap["Force"] = Method::get_TableData(ui->table_kernel_Force);

  // Patch
  if (list.removeOne("Patch"))
    subMap["Patch"] = Method::get_TableData(ui->table_kernel_patch);

  // Emulate
  if (list.removeOne("Emulate")) {
    QVariantMap mapValue;
    subMap["Emulate"] = setValue(mapValue, ui->tabKernel5);
  }

  // Quirks
  if (list.removeOne("Quirks")) {
    QVariantMap mapQuirks;
    subMap["Quirks"] = setValue(mapQuirks, ui->tabKernel6);
  }

  // Scheme
  if (list.removeOne("Scheme")) {
    QVariantMap mapScheme;
    subMap["Scheme"] = setValue(mapScheme, ui->tabKernel7);
  }

  return subMap;
}

QVariantMap MainWindow::SaveMisc() {
  QVariantMap subMap;
  QVariantMap valueList;
  QVariantMap mapMain = mapTatol["Misc"].toMap();
  QStringList list = mapMain.keys();

  // Boot
  if (list.removeOne("Boot"))
    subMap["Boot"] = setValue(valueList, ui->tabMisc1);

  // Debug
  if (list.removeOne("Debug")) {
    valueList.clear();
    subMap["Debug"] = setValue(valueList, ui->tabMisc2);
  }

  // Security
  if (list.removeOne("Security")) {
    valueList.clear();
    subMap["Security"] = setValue(valueList, ui->tabMisc3);
  }

  QString hm = ui->cboxSecureBootModel->currentText().trimmed();
  if (hm == "") hm = "Disabled";
  valueList["SecureBootModel"] = hm;

  // BlessOverride
  if (list.removeOne("BlessOverride"))
    subMap["BlessOverride"] = Method::get_TableData(ui->tableBlessOverride);

  // Entries
  if (list.removeOne("Entries"))
    subMap["Entries"] = Method::get_TableData(ui->tableEntries);

  // Tools
  if (list.removeOne("Tools"))
    subMap["Tools"] = Method::get_TableData(ui->tableTools);

  // Serial
  if (list.removeOne("Serial")) {
    valueList.clear();
    // subMap["Serial"] = setValue(valueList, ui->tabMisc7);
    valueList = setValue(valueList, ui->frame_chk_29);

    QVariantMap Map, vlist;
    Map["Custom"] = setValue(vlist, ui->gboxCustom);
    valueList["Custom"] = Map["Custom"];
    subMap["Serial"] = valueList;
  }

  return subMap;
}

QVariantMap MainWindow::SaveNVRAM() {
  // Add
  QVariantMap subMap;
  QVariantMap dictList;
  QVariantList arrayList;
  QVariantMap valueList;

  // Root
  subMap = setValue(subMap, ui->gboxNVRAM);

  int currentRow = ui->table_nv_add0->currentRow();
  int currentRowAdd = ui->table_nv_add->currentRow();
  int currentColAdd = ui->table_nv_add->currentColumn();
  for (int i = 0; i < ui->table_nv_add0->rowCount(); i++) {
    valueList.clear();
    // 先加载表中的数据
    ui->table_nv_add0->setCurrentCell(i, 0);
    on_table_nv_add0_cellClicked(i, 0);

    for (int k = 0; k < ui->table_nv_add->rowCount(); k++) {
      QString dataType = ui->table_nv_add->item(k, 1)->text();  // 数据类型
      QString value = ui->table_nv_add->item(k, 2)->text();
      if (dataType == "String")
        valueList[ui->table_nv_add->item(k, 0)->text()] = value;
      if (dataType == "Data") {
        // 将以字符串方式显示的16进制原样转换成QByteArray
        valueList[ui->table_nv_add->item(k, 0)->text()] = HexStrToByte(value);
      }
      if (dataType == "Number")
        valueList[ui->table_nv_add->item(k, 0)->text()] = value.toLongLong();
      if (dataType == "Boolean") {
        valueList[ui->table_nv_add->item(k, 0)->text()] =
            getBool(ui->table_nv_add, k, 2);
      }
    }
    dictList[ui->table_nv_add0->item(i, 0)->text()] = valueList;
  }
  subMap["Add"] = dictList;

  ui->table_nv_add0->clearSelection();
  ui->table_nv_add0->setCurrentCell(currentRow, 0);
  on_table_nv_add0_cellClicked(currentRow, 0);
  ui->table_nv_add->setCurrentCell(currentRowAdd, currentColAdd);
  ui->table_nv_add->setFocus();

  // Delete
  dictList.clear();  // 先清理之前的数据
  currentRow = ui->table_nv_del0->currentRow();
  for (int i = 0; i < ui->table_nv_del0->rowCount(); i++) {
    valueList.clear();  // 先必须清理下列表，很重要
    arrayList.clear();

    // 先加载表中的数据
    ui->table_nv_del0->setCurrentCell(i, 0);
    on_table_nv_del0_cellClicked(i, 0);

    for (int k = 0; k < ui->table_nv_del->rowCount(); k++) {
      arrayList.append(ui->table_nv_del->item(k, 0)->text());
    }
    dictList[ui->table_nv_del0->item(i, 0)->text()] = arrayList;
  }
  ui->table_nv_del0->clearSelection();
  ui->table_nv_del0->setCurrentCell(currentRow, 0);
  subMap["Delete"] = dictList;

  // LegacySchema
  dictList.clear();  // 先清理之前的数据
  currentRow = ui->table_nv_ls0->currentRow();
  for (int i = 0; i < ui->table_nv_ls0->rowCount(); i++) {
    valueList.clear();  // 先必须清理下列表，很重要
    arrayList.clear();

    // 先加载表中的数据
    ui->table_nv_ls0->setCurrentCell(i, 0);
    on_table_nv_ls0_cellClicked(i, 0);

    for (int k = 0; k < ui->table_nv_ls->rowCount(); k++) {
      arrayList.append(ui->table_nv_ls->item(k, 0)->text());
    }
    dictList[ui->table_nv_ls0->item(i, 0)->text()] = arrayList;
  }
  ui->table_nv_ls0->clearSelection();
  ui->table_nv_ls0->setCurrentCell(currentRow, 0);
  subMap["LegacySchema"] = dictList;

  return subMap;
}

QVariantMap MainWindow::SavePlatformInfo() {
  QVariantMap subMap;
  QVariantMap valueList;
  QVariantMap mapMain = mapTatol["PlatformInfo"].toMap();
  QStringList list = mapMain.keys();

  // Tatol
  subMap = setValue(valueList, ui->gbox01);

  // Generic
  if (list.removeOne("Generic")) {
    valueList.clear();
    if (ui->editDatROM->text().count() > 12) ui->btnROM->clicked();

    subMap["Generic"] = setValue(valueList, ui->tabPlatformInfo1);

    if (getSystemProductName(ui->cboxSystemProductName->currentText()) != "")
      valueList["SystemProductName"] =
          getSystemProductName(ui->cboxSystemProductName->currentText());
    else
      valueList["SystemProductName"] = ui->cboxSystemProductName->currentText();
  }

  // DataHub
  if (list.removeOne("DataHub")) {
    valueList.clear();
    if (ui->mychkSaveDataHub->isChecked() || !ui->chkAutomatic->isChecked())
      subMap["DataHub"] = setValue(valueList, ui->tabPlatformInfo2);
  }

  // Memory
  if (list.removeOne("Memory")) {
    list.append("Memory");
    valueList.clear();
    valueList = setValue(valueList, ui->tabPlatformInfo3);
  }

  // Devices Memory
  if (list.removeOne("Memory")) {
    QVariantMap Map;
    Map["Devices"] = Method::get_TableData(ui->tableDevices);
    valueList["Devices"] = Map["Devices"];
    if (ui->tableDevices->rowCount() > 0) subMap["Memory"] = valueList;
  }

  // PlatformNVRAM
  if (list.removeOne("PlatformNVRAM")) {
    valueList.clear();
    if (ui->mychkSaveDataHub->isChecked() || !ui->chkAutomatic->isChecked())
      subMap["PlatformNVRAM"] = setValue(valueList, ui->tabPlatformInfo4);
  }

  // SMBIOS
  if (list.removeOne("SMBIOS")) {
    valueList.clear();
    if (ui->mychkSaveDataHub->isChecked() || !ui->chkAutomatic->isChecked())
      subMap["SMBIOS"] = setValue(valueList, ui->tabPlatformInfo5);
  }

  return subMap;
}

QVariantMap MainWindow::SaveUEFI() {
  QVariantMap subMap;
  QVariantMap dictList;

  // 0.Tatol
  subMap = setValue(subMap, ui->gboxUEFI);
  // subMap["ConnectDrivers"] = getChkBool(ui->chkConnectDrivers);

  // 1. APFS
  subMap["APFS"] = setValue(dictList, ui->tabUEFI1);

  // 2. AppleInput
  QVariantMap mapAI = setValue(dictList, ui->tabUEFI2);
  if (mapAI.count() > 0) subMap["AppleInput"] = mapAI;

  // 3. Audio
  dictList.clear();
  subMap["Audio"] = setValue(dictList, ui->tabUEFI3);

  // 4. Drivers
  subMap["Drivers"] = Method::get_TableData(ui->table_uefi_drivers);

  // 5. Input
  dictList.clear();
  subMap["Input"] = setValue(dictList, ui->tabUEFI5);

  // 6. Output
  dictList.clear();
  subMap["Output"] = setValue(dictList, ui->tabUEFI6);

  // 7. ProtocolOverrides
  dictList.clear();
  subMap["ProtocolOverrides"] = setValue(dictList, ui->tabUEFI7);

  // 8. Quirks
  dictList.clear();
  subMap["Quirks"] = setValue(dictList, ui->tabUEFI8);

  // 9. ReservedMemory
  subMap["ReservedMemory"] =
      Method::get_TableData(ui->table_uefi_ReservedMemory);

  // 10. Unload
  QString ver = lblVer->text();
  ver = ver.replace("OpenCore", "");
  QStringList list = ver.split("[");
  if (list.count() > 0) {
    ver = list.at(0);
  }
  ver = ver.trimmed();
  qDebug() << "ver=" << ver;
  if (isUnload) {
    subMap["Unload"] = Method::get_TableData(ui->table_uefi_Unload);
  }

  return subMap;
}

bool MainWindow::getChkBool(QCheckBox* chkbox) {
  if (chkbox->isChecked())
    return true;
  else
    return false;

  return false;
}

QString MainWindow::ByteToHexStr(QByteArray ba) {
  QString str = ba.toHex().toUpper();

  return str;
}

QByteArray MainWindow::HexStrToByte(QString value) {
  QByteArray ba;
  QVector<QString> byte;
  int len = value.length();
  int k = 0;
  ba.resize(len / 2);
  for (int i = 0; i < len / 2; i++) {
    byte.push_back(value.mid(k, 2));
    ba[k / 2] = byte[k / 2].toUInt(nullptr, 16);
    k = k + 2;
  }

  return ba;
}

QByteArray MainWindow::HexStringToByteArray(QString HexString) {
  bool ok;
  QByteArray ret;
  HexString = HexString.trimmed();
  HexString = HexString.simplified();
  QStringList sl = HexString.split(" ");

  foreach (QString s, sl) {
    if (!s.isEmpty()) {
      char c = s.toInt(&ok, 16) & 0xFF;
      if (ok) {
        ret.append(c);
      } else {
        // qDebug() << "非法的16进制字符：" << s;
        // QMessageBox::warning(0, tr("错误："),
        //                      QString("非法的16进制字符: \"%1\"").arg(s));
      }
    }
  }
  qDebug() << ret;
  return ret;
}

void MainWindow::on_table_acpi_add_cellClicked(int row, int column) {
  if (!ui->table_acpi_add->currentIndex().isValid()) return;

  set_InitCheckBox(ui->table_acpi_add, row, column);

  setStatusBarText(ui->table_acpi_add);
}

void MainWindow::init_enabled_data(QTableWidget* table, int row, int column,
                                   QString str) {
  QTableWidgetItem* chkbox = new QTableWidgetItem(str);

  table->setItem(row, column, chkbox);
  table->item(row, column)->setTextAlignment(Qt::AlignCenter);
  if (str == "true")

    chkbox->setCheckState(Qt::Checked);
  else

    chkbox->setCheckState(Qt::Unchecked);
}

void MainWindow::enabled_change(QTableWidget* table, int row, int column,
                                int cc) {
  if (table->currentIndex().isValid()) {
    if (column == cc) {
      if (table->item(row, column)->checkState() == Qt::Checked) {
        table->item(row, column)->setTextAlignment(Qt::AlignCenter);
        table->item(row, column)->setText("true");

      } else {
        if (table->item(row, column)->checkState() == Qt::Unchecked) {
          table->item(row, column)->setTextAlignment(Qt::AlignCenter);
          table->item(row, column)->setText("false");
        }
      }
    }
  }
}

void MainWindow::on_table_acpi_del_cellClicked(int row, int column) {
  if (!ui->table_acpi_del->currentIndex().isValid()) return;

  set_InitCheckBox(ui->table_acpi_del, row, column);

  setStatusBarText(ui->table_acpi_del);
}

void MainWindow::setStatusBarText(QTableWidget* table) {
  if (loading) return;
  if (!table->currentIndex().isValid()) return;

  QString text0 = table->item(table->currentRow(), 0)->text();
  QString text = table->currentItem()->text();
  QString str;

  if (getTableFieldDataType(table) == "Data")
    str = QString::number(text.count() / 2) + " Bytes  " + text;
  else
    str = text;

  if (text0.length() > 65) text0 = text0.mid(0, 60) + "...";
  if (str.length() > 80) str = str.mid(0, 65) + "...";
  if (table->currentColumn() != 0)

    ui->lblStatusShow->setText(text0 + " -> " + str);
  else

    ui->lblStatusShow->setText(str);

  if (getTableFieldDataType(table) == "Data") {
    ui->txtEditHex->setText(table->currentItem()->text());

    setConversionWidgetVisible(true);
  } else {
    setConversionWidgetVisible(false);
  }
}

void MainWindow::on_table_acpi_patch_cellClicked(int row, int column) {
  if (!ui->table_acpi_patch->currentIndex().isValid()) return;

  set_InitCheckBox(ui->table_acpi_patch, row, column);

  setStatusBarText(ui->table_acpi_patch);
}

void MainWindow::on_table_booter_cellClicked(int row, int column) {
  if (!ui->table_booter->currentIndex().isValid()) return;

  set_InitCheckBox(ui->table_booter, row, column);

  setStatusBarText(ui->table_booter);
}

void MainWindow::on_table_kernel_add_cellClicked(int row, int column) {
  if (!ui->table_kernel_add->currentIndex().isValid()) return;

  set_InitCheckBox(ui->table_kernel_add, row, column);

  if (ui->table_kernel_add->horizontalHeaderItem(column)->text() == "Arch") {
    cboxArch = new QComboBox;
    cboxArch->setEditable(true);
    cboxArch->addItem("Any");
    cboxArch->addItem("i386");
    cboxArch->addItem("x86_64");
    cboxArch->addItem("");

    connect(cboxArch, SIGNAL(currentTextChanged(QString)), this,
            SLOT(arch_addChange()));
    c_row = row;

    ui->table_kernel_add->setCellWidget(row, column, cboxArch);
    cboxArch->setCurrentText(ui->table_kernel_add->item(row, column)->text());
  }

  setStatusBarText(ui->table_kernel_add);
}

void MainWindow::on_table_kernel_block_cellClicked(int row, int column) {
  QTableWidget* t = ui->table_kernel_block;
  if (!t->currentIndex().isValid()) return;

  set_InitCheckBox(t, row, column);

  QString txt = t->horizontalHeaderItem(column)->text();
  if (txt == "Arch") {
    cboxArch = new QComboBox;
    cboxArch->setEditable(true);
    cboxArch->addItem("Any");
    cboxArch->addItem("i386");
    cboxArch->addItem("x86_64");
    cboxArch->addItem("");
    connect(cboxArch, SIGNAL(currentTextChanged(QString)), this,
            SLOT(arch_blockChange()));
    c_row = row;
    t->setCellWidget(row, column, cboxArch);
    cboxArch->setCurrentText(t->item(row, column)->text());
  }
  if (txt == "Strategy") {
    cboxArch = new QComboBox;
    cboxArch->setEditable(true);
    cboxArch->addItem("Disable");
    cboxArch->addItem("Exclude");
    cboxArch->addItem("");
    connect(cboxArch, SIGNAL(currentTextChanged(QString)), this,
            SLOT(arch_blockChange()));
    c_row = row;
    t->setCellWidget(row, column, cboxArch);
    cboxArch->setCurrentText(t->item(row, column)->text());
  }

  setStatusBarText(t);
}

void MainWindow::on_table_kernel_patch_cellClicked(int row, int column) {
  if (!ui->table_kernel_patch->currentIndex().isValid()) return;

  set_InitCheckBox(ui->table_kernel_patch, row, column);

  if (ui->table_kernel_patch->horizontalHeaderItem(column)->text() == "Arch") {
    cboxArch = new QComboBox;
    cboxArch->setEditable(true);
    cboxArch->addItem("Any");
    cboxArch->addItem("i386");
    cboxArch->addItem("x86_64");
    cboxArch->addItem("");

    connect(cboxArch, SIGNAL(currentTextChanged(QString)), this,
            SLOT(arch_patchChange()));
    c_row = row;

    ui->table_kernel_patch->setCellWidget(row, column, cboxArch);
    cboxArch->setCurrentText(ui->table_kernel_patch->item(row, column)->text());
  }

  setStatusBarText(ui->table_kernel_patch);
}

void MainWindow::on_tableEntries_cellClicked(int row, int column) {
  if (!ui->tableEntries->currentIndex().isValid()) return;

  set_InitCheckBox(ui->tableEntries, row, column);

  setStatusBarText(ui->tableEntries);
}

void MainWindow::on_tableTools_cellClicked(int row, int column) {
  if (!ui->tableTools->currentIndex().isValid()) return;

  set_InitCheckBox(ui->tableTools, row, column);

  setStatusBarText(ui->tableTools);
}

void MainWindow::on_table_uefi_ReservedMemory_cellClicked(int row, int column) {
  if (!ui->table_uefi_ReservedMemory->currentIndex().isValid()) return;

  set_InitCheckBox(ui->table_uefi_ReservedMemory, row, column);

  QString txt =
      ui->table_uefi_ReservedMemory->horizontalHeaderItem(column)->text();
  if (txt == "Type") {
    cboxReservedMemoryType = new QComboBox(this);
    cboxReservedMemoryType->setEditable(true);
    QStringList sl_type;

    sl_type.append("Reserved");
    sl_type.append("LoaderCode");
    sl_type.append("LoaderData");
    sl_type.append("BootServiceCode");
    sl_type.append("BootServiceData");
    sl_type.append("RuntimeCode");
    sl_type.append("RuntimeData");
    sl_type.append("Available");
    sl_type.append("Persistent");
    sl_type.append("UnusableMemory");
    sl_type.append("ACPIReclaimMemory");
    sl_type.append("ACPIMemoryNVS");
    sl_type.append("MemoryMappedIO");
    sl_type.append("MemoryMappedIOPortSpace");
    sl_type.append("PalCode");
    cboxReservedMemoryType->addItems(sl_type);

    connect(cboxReservedMemoryType, SIGNAL(currentTextChanged(QString)), this,
            SLOT(ReservedMemoryTypeChange()));
    c_row = row;

    ui->table_uefi_ReservedMemory->setCellWidget(row, column,
                                                 cboxReservedMemoryType);
    cboxReservedMemoryType->setCurrentText(
        ui->table_uefi_ReservedMemory->item(row, column)->text());
  }

  setStatusBarText(ui->table_uefi_ReservedMemory);
}

void MainWindow::on_btnKernelPatchAdd_clicked() {
  Method::add_OneLine(ui->table_kernel_patch);
}

void MainWindow::on_btnKernelPatchDel_clicked() {
  del_item(ui->table_kernel_patch);
}

void MainWindow::add_item(QTableWidget* table, int total_column) {
  table->setSelectionMode(QAbstractItemView::SingleSelection);
  int t = table->rowCount();
  table->setRowCount(t + 1);

  // 用""初始化各项值
  for (int i = 0; i < total_column; i++) {
    table->setItem(t, i, new QTableWidgetItem(""));
  }
  table->setFocus();
  table->setCurrentCell(t, 0);

  table->setSelectionMode(QAbstractItemView::ExtendedSelection);
}

QString MainWindow::getSubTabStr(int tabIndex) {
  int subtabIndex;
  QString subtabStr;

  for (int i = 0; i < mainTabList.count(); i++) {
    if (i == tabIndex) {
      subtabIndex = mainTabList.at(i)->currentIndex();
      subtabStr = mainTabList.at(i)->tabText(subtabIndex);
    }
  }

  return subtabStr;
}

void MainWindow::del_item(QTableWidget* table) {
  if (!table->currentIndex().isValid()) return;

  int row = table->currentRow();

  // std::vector<int> vecItemIndex; //保存选中行的索引
  QItemSelectionModel* selections =
      table->selectionModel();  // 返回当前的选择模式
  QModelIndexList selectedsList =
      selections->selectedIndexes();  // 返回所有选定的模型项目索引列表

  for (int i = 0; i < selectedsList.count(); i++) {
    // vecItemIndex.push_back(selectedsList.at(i).row());
    int t = selectedsList.at(i).row();

    // 删除部分的Redo/Undo
    QStringList fieldList;
    for (int j = 0; j < table->columnCount(); j++) {
      fieldList.append(table->item(t, j)->text());
    }

    int tabIndex = ui->tabTotal->currentIndex();
    QString subtabStr;
    subtabStr = getSubTabStr(tabIndex);

    QString text = ui->tabTotal->tabText(tabIndex) + " -> " + subtabStr +
                   " -> " + fieldList.at(0);

    QTableWidget* table0 = NULL;
    int table0CurrentRow = -1;
    bool loadINI = false;
    bool writeINI = false;

    if (table == ui->table_dp_add0) {
      table0 = ui->table_dp_add;
      table0CurrentRow = ui->table_dp_add0->currentRow();
      loadINI = true;
    }

    if (table == ui->table_dp_add) {
      table0 = ui->table_dp_add0;
      table0CurrentRow = ui->table_dp_add0->currentRow();
      writeINI = true;
    }

    if (table == ui->table_dp_del0) {
      table0 = ui->table_dp_del;
      table0CurrentRow = ui->table_dp_del0->currentRow();
      loadINI = true;
    }

    if (table == ui->table_dp_del) {
      table0 = ui->table_dp_del0;
      table0CurrentRow = ui->table_dp_del0->currentRow();
      writeINI = false;
    }

    if (table == ui->table_nv_add0) {
      table0 = ui->table_nv_add;
      table0CurrentRow = ui->table_nv_add0->currentRow();
      loadINI = true;
    }

    if (table == ui->table_nv_add) {
      table0 = ui->table_nv_add0;
      table0CurrentRow = ui->table_nv_add0->currentRow();
      writeINI = true;
    }

    if (table == ui->table_nv_del0) {
      table0 = ui->table_nv_del;
      table0CurrentRow = ui->table_nv_del0->currentRow();
      loadINI = true;
    }

    if (table == ui->table_nv_del) {
      table0 = ui->table_nv_del0;
      table0CurrentRow = ui->table_nv_del0->currentRow();
      writeINI = false;
    }

    if (table == ui->table_nv_ls0) {
      table0 = ui->table_nv_ls;
      table0CurrentRow = ui->table_nv_ls0->currentRow();
      loadINI = true;
    }

    if (table == ui->table_nv_ls) {
      table0 = ui->table_nv_ls0;
      table0CurrentRow = ui->table_nv_ls0->currentRow();
      writeINI = false;
    }

    QUndoCommand* deleteCommand = new DeleteCommand(
        writeINI, loadINI, table0, table0CurrentRow, table, t, text, fieldList);
    undoStack->push(deleteCommand);

    selections = table->selectionModel();
    selectedsList = selections->selectedIndexes();

    i = -1;
  }

  if (row > table->rowCount()) {
    row = table->rowCount();
  }

  table->setFocus();
  table->setCurrentCell(row - 1, 0);
  if (row == 0) table->setCurrentCell(0, 0);

  this->setWindowModified(true);
  updateIconStatus();
}

void MainWindow::on_btnACPIAdd_Del_clicked() { del_item(ui->table_acpi_add); }

void MainWindow::on_btnACPIDel_Add_clicked() {
  Method::add_OneLine(ui->table_acpi_del);
}

void MainWindow::on_btnACPIDel_Del_clicked() { del_item(ui->table_acpi_del); }

void MainWindow::on_btnACPIPatch_Add_clicked() {
  Method::add_OneLine(ui->table_acpi_patch);
}

void MainWindow::on_btnACPIPatch_Del_clicked() {
  del_item(ui->table_acpi_patch);
}

void MainWindow::on_btnBooter_Add_clicked() {
  Method::add_OneLine(ui->table_booter);
}

void MainWindow::on_btnBooter_Del_clicked() { del_item(ui->table_booter); }

void MainWindow::on_btnDPDel_Add0_clicked() {
  add_item(ui->table_dp_del0, 1);
  ui->table_dp_del->setRowCount(0);  // 先清除右边表中的所有条目
  on_btnDPDel_Add_clicked();         // 同时右边增加一个新条目
  ui->table_dp_del0->setFocus();

  write_value_ini(ui->table_dp_del0, ui->table_dp_del,
                  ui->table_dp_del0->rowCount() - 1);

  this->setWindowModified(true);
  updateIconStatus();
}

void MainWindow::on_btnDPDel_Del0_clicked() {
  mymethod->delRightTableItem(ui->table_dp_del0, ui->table_dp_del);
}

void MainWindow::on_btnDPDel_Add_clicked() {
  if (ui->table_dp_del0->rowCount() == 0) return;

  loading = true;
  add_item(ui->table_dp_del, 1);
  loading = false;

  // 保存数据
  write_value_ini(ui->table_dp_del0, ui->table_dp_del,
                  ui->table_dp_del0->currentRow());

  this->setWindowModified(true);
  updateIconStatus();
}

void MainWindow::on_btnDPDel_Del_clicked() {
  if (ui->table_dp_del->rowCount() <= 0) return;
  del_item(ui->table_dp_del);
  write_value_ini(ui->table_dp_del0, ui->table_dp_del,
                  ui->table_dp_del0->currentRow());
}

void MainWindow::on_btnACPIAdd_Add_clicked() {
  QFileDialog fd;
  QStringList FileName =
      fd.getOpenFileNames(this, "file", "", "acpi file(*.aml);;all(*.*)");

  addACPIItem(FileName);
}

void MainWindow::addACPIItem(QStringList FileName) {
  if (FileName.isEmpty()) return;
  int pathCol = 0;
  for (int n = 0; n < ui->table_acpi_add->columnCount(); n++) {
    QString txt = ui->table_acpi_add->horizontalHeaderItem(n)->text();
    if (txt == "Path") {
      pathCol = n;
      break;
    }
  }
  QStringList tempList =
      mymethod->delDuplication(FileName, ui->table_acpi_add, pathCol);
  FileName.clear();
  FileName = tempList;

  for (int i = 0; i < FileName.count(); i++) {
    QString strBaseName = QFileInfo(FileName.at(i)).fileName();
    Method::add_OneLine(ui->table_acpi_add);
    for (int n = 0; n < ui->table_acpi_add->columnCount(); n++) {
      QString txt = ui->table_acpi_add->horizontalHeaderItem(n)->text();
      if (txt == "Path") {
        ui->table_acpi_add->setItem(ui->table_acpi_add->rowCount() - 1, n,
                                    new QTableWidgetItem(strBaseName));
      }
    }

    QDir dir(strACPI);
    if (dir.exists()) {
      QFile::copy(FileName.at(i), strACPI + strBaseName);
    }
  }

  checkFiles(ui->table_acpi_add);
}

void MainWindow::on_btnDPAdd_Add0_clicked() {
  loading = true;

  add_item(ui->table_dp_add0, 1);
  ui->table_dp_add->setRowCount(0);  // 先清除右边表中的所有条目
  on_btnDPAdd_Add_clicked();         // 同时右边增加一个新条目
  ui->table_dp_add0->setFocus();
  mymethod->writeLeftTable(ui->table_dp_add0, ui->table_dp_add);

  this->setWindowModified(true);
  updateIconStatus();

  loading = false;
}

void MainWindow::on_btnDPAdd_Del0_clicked() {
  mymethod->delRightTableItem(ui->table_dp_add0, ui->table_dp_add);
}

void MainWindow::on_btnDPAdd_Add_clicked() {
  if (ui->table_dp_add0->rowCount() == 0) return;

  loading = true;
  add_item(ui->table_dp_add, 3);
  loading = false;

  // 保存数据
  //  write_ini(ui->table_dp_add0, ui->table_dp_add,
  //           ui->table_dp_add0->currentRow());
  mymethod->writeLeftTable(ui->table_dp_add0, ui->table_dp_add);

  this->setWindowModified(true);
  updateIconStatus();
}

void MainWindow::on_btnDPAdd_Del_clicked() {
  if (ui->table_dp_add->rowCount() <= 0) return;
  del_item(ui->table_dp_add);
  write_ini(ui->table_dp_add0, ui->table_dp_add,
            ui->table_dp_add0->currentRow());
}

void MainWindow::on_btnKernelAdd_Add_clicked() {
  QFileDialog fd;
  QStringList FileName;

#ifdef Q_OS_WIN32
  FileName.append(fd.getExistingDirectory());
#endif

#ifdef Q_OS_LINUX
  FileName.append(fd.getExistingDirectory());
#endif

#ifdef Q_OS_MAC
  FileName = fd.getOpenFileNames(this, "kext", "", "kext(*.kext);;all(*.*)");
#endif

  for (int i = 0; i < FileName.count(); i++) {
    QString str = FileName.at(i);
    if (!str.toLower().contains(".kext")) {
      FileName.removeAt(i);
      i--;
    }
  }

  if (FileName.count() == 0) return;

  addKexts(FileName);
}

void MainWindow::addKexts(QStringList FileName) {
  QTableWidget* t = ui->table_kernel_add;

  int pathCol = 0;
  for (int n = 0; n < t->columnCount(); n++) {
    QString txt = t->horizontalHeaderItem(n)->text();
    if (txt == "Path") {
      pathCol = n;
      break;
    }
  }
  QStringList tempList = mymethod->delDuplication(FileName, t, pathCol);
  FileName.clear();
  FileName = tempList;
  int file_count = FileName.count();
  if (file_count == 0 || FileName[0] == "") return;
  for (int j = 0; j < file_count; j++) {
    QFileInfo fileInfo(FileName[j]);

    QFileInfo fileInfoList;
    QString filePath = fileInfo.absolutePath();

    QDir fileDir(filePath + "/" + fileInfo.fileName() + "/Contents/MacOS/");

    if (fileDir.exists())  // 如果目录存在，则遍历里面的文件
    {
      fileDir.setFilter(QDir::Files);  // 只遍历本目录
      QFileInfoList fileList = fileDir.entryInfoList();
      int fileCount = fileList.count();
      for (int i = 0; i < fileCount; i++)  // 一般只有一个二进制文件
      {
        fileInfoList = fileList[i];
      }
    }

    QString strBaseName = QFileInfo(FileName[j]).fileName();
    Method::add_OneLine(t);
    for (int n = 0; n < t->columnCount(); n++) {
      QString txt = t->horizontalHeaderItem(n)->text();
      if (txt == "BundlePath") {
        t->setItem(t->rowCount() - 1, n, new QTableWidgetItem(strBaseName));
      }
      if (txt == "ExecutablePath") {
        if (fileInfoList.fileName() != "")
          t->setItem(t->rowCount() - 1, n,
                     new QTableWidgetItem("Contents/MacOS/" +
                                          fileInfoList.fileName()));
        else
          t->setItem(t->rowCount() - 1, n, new QTableWidgetItem(""));
      }
    }

    QDir dir(strKexts);
    if (dir.exists()) {
      copyDirectoryFiles(FileName.at(j), strKexts + strBaseName, false);
    }

    // 如果里面还有PlugIns目录，则需要继续遍历插件目录
    QDir piDir(filePath + "/" + fileInfo.fileName() + "/Contents/PlugIns/");

    if (piDir.exists()) {
      piDir.setFilter(QDir::Dirs);  // 过滤器：只遍历里面的目录

      QFileInfoList fileList;
      QFileInfoList List = piDir.entryInfoList();
      for (int z = 0; z < List.count(); z++) {
        QString str = List.at(z).fileName();

        if (str.mid(str.count() - 4, 4) == "kext") {
          fileList.push_back(List.at(z));
        }
      }

      int fileCount = fileList.count();
      QVector<QString> kext_file;

      for (int i = 0; i < fileCount; i++)  // 找出里面的kext文件(目录）
      {
        QString strKext = fileList[i].fileName();
        kext_file.push_back(strKext);
      }

      if (fileCount > 0)  // 里面有目录
      {
        for (int i = 0; i < fileCount; i++) {
          QDir fileDir(filePath + "/" + fileInfo.fileName() +
                       "/Contents/PlugIns/" + kext_file[i] +
                       "/Contents/MacOS/");
          if (fileDir.exists()) {
            fileDir.setFilter(QDir::Files);  // 只遍历本目录里面的文件
            QFileInfoList fileList = fileDir.entryInfoList();
            int fileCount = fileList.count();
            for (int i = 0; i < fileCount; i++)  // 一般只有一个二进制文件
            {
              fileInfoList = fileList[i];
            }

            // 写入到表里
            Method::add_OneLine(t);
            for (int n = 0; n < t->columnCount(); n++) {
              QString txt = t->horizontalHeaderItem(n)->text();
              if (txt == "BundlePath") {
                t->setItem(
                    t->rowCount() - 1, n,
                    new QTableWidgetItem(QFileInfo(FileName[j]).fileName() +
                                         "/Contents/PlugIns/" + kext_file[i]));
              }
              if (txt == "ExecutablePath") {
                t->setItem(t->rowCount() - 1, n,
                           new QTableWidgetItem("Contents/MacOS/" +
                                                fileInfoList.fileName()));
              }
            }

          } else {  // 不存在二进制文件，只存在一个Info.plist文件的情况

            QDir fileDir(filePath + "/" + fileInfo.fileName() +
                         "/Contents/PlugIns/" + kext_file[i] + "/Contents/");
            if (fileDir.exists()) {
              // 写入到表里
              Method::add_OneLine(t);
              for (int n = 0; n < t->columnCount(); n++) {
                QString txt = t->horizontalHeaderItem(n)->text();
                if (txt == "BundlePath") {
                  t->setItem(t->rowCount() - 1, n,
                             new QTableWidgetItem(
                                 QFileInfo(FileName[j]).fileName() +
                                 "/Contents/PlugIns/" + kext_file[i]));
                }
                if (txt == "ExecutablePath") {
                  t->setItem(t->rowCount() - 1, n, new QTableWidgetItem(""));
                }
              }
            }
          }
        }
      }
    }

  }  // for (int j = 0; j < file_count; j++)

  // Sort
  sortForKexts();

  checkFiles(t);
}

void MainWindow::sortForKexts() {
  for (int i = 0; i < ui->table_kernel_add->rowCount(); i++) {
    QString str0 = ui->table_kernel_add->item(i, 0)->text();

    if (str0.toLower().trimmed() == "lilu.kext") {
      if (i != 0) {
        ui->table_kernel_add->setCurrentCell(i, 0);
        for (int j = 0; j < i; j++) MoveItem(ui->table_kernel_add, true);
        ui->chkDisableLinkeditJettison->setChecked(true);
        break;
      }
    }
  }

  for (int i = 0; i < ui->table_kernel_add->rowCount(); i++) {
    QString str0 = ui->table_kernel_add->item(i, 0)->text();

    if (str0.toLower().trimmed() == "virtualsmc.kext") {
      if (i != 1) {
        ui->table_kernel_add->setCurrentCell(i, 0);
        for (int j = 0; j < i - 1; j++) MoveItem(ui->table_kernel_add, true);
        break;
      }
    }
  }
}

void MainWindow::on_btnKernelBlock_Add_clicked() {
  Method::add_OneLine(ui->table_kernel_block);
}

void MainWindow::on_btnKernelBlock_Del_clicked() {
  del_item(ui->table_kernel_block);
}

void MainWindow::on_btnMiscBO_Add_clicked() {
  Method::add_OneLine(ui->tableBlessOverride);
}

void MainWindow::on_btnMiscBO_Del_clicked() {
  del_item(ui->tableBlessOverride);
}

void MainWindow::on_btnMiscEntries_Add_clicked() {
  Method::add_OneLine(ui->tableEntries);
}

void MainWindow::on_btnMiscTools_Add_clicked() {
  QFileDialog fd;

  QStringList FileName = fd.getOpenFileNames(this, "tools efi file", "",
                                             "efi file(*.efi);;all files(*.*)");

  addEFITools(FileName);
}

void MainWindow::addEFITools(QStringList FileName) {
  if (FileName.isEmpty()) return;

  int pathCol = 0;
  for (int n = 0; n < ui->tableTools->columnCount(); n++) {
    QString txt = ui->tableTools->horizontalHeaderItem(n)->text();
    if (txt == "Path") {
      pathCol = n;
      break;
    }
  }
  QStringList tempList =
      mymethod->delDuplication(FileName, ui->tableTools, pathCol);
  FileName.clear();
  FileName = tempList;

  for (int i = 0; i < FileName.count(); i++) {
    QString strBaseName = QFileInfo(FileName.at(i)).fileName();
    Method::add_OneLine(ui->tableTools);
    for (int n = 0; n < ui->tableTools->columnCount(); n++) {
      QString txt = ui->tableTools->horizontalHeaderItem(n)->text();
      if (txt == "Path") {
        ui->tableTools->setItem(ui->tableTools->rowCount() - 1, n,
                                new QTableWidgetItem(strBaseName));
      }
      if (txt == "Name") {
        QString str = strBaseName;
        ui->tableTools->setItem(ui->tableTools->rowCount() - 1, n,
                                new QTableWidgetItem(str.replace(".efi", "")));
      }
    }

    QDir dir(strTools);
    if (dir.exists()) {
      QFile::copy(FileName.at(i), strTools + strBaseName);
    }
  }

  checkFiles(ui->tableTools);
}

void MainWindow::on_btnMiscEntries_Del_clicked() { del_item(ui->tableEntries); }

void MainWindow::on_btnMiscTools_Del_clicked() { del_item(ui->tableTools); }

void MainWindow::on_btnNVRAMAdd_Add0_clicked() {
  add_item(ui->table_nv_add0, 1);
  ui->table_nv_add->setRowCount(0);  // 先清除右边表中的所有条目
  on_btnNVRAMAdd_Add_clicked();      // 同时右边增加一个新条目
  ui->table_nv_add0->setFocus();

  mymethod->writeLeftTable(ui->table_nv_add0, ui->table_nv_add);

  this->setWindowModified(true);
  updateIconStatus();
}

void MainWindow::on_btnNVRAMAdd_Add_clicked() {
  if (ui->table_nv_add0->rowCount() == 0) return;

  loading = true;
  add_item(ui->table_nv_add, 3);
  loading = false;

  // 保存数据
  write_ini(ui->table_nv_add0, ui->table_nv_add,
            ui->table_nv_add0->currentRow());

  this->setWindowModified(true);
  updateIconStatus();
}

void MainWindow::on_btnNVRAMAdd_Del0_clicked() {
  mymethod->delRightTableItem(ui->table_nv_add0, ui->table_nv_add);
}

void MainWindow::on_btnNVRAMAdd_Del_clicked() {
  if (ui->table_nv_add->rowCount() <= 0) return;
  del_item(ui->table_nv_add);
}

void MainWindow::on_btnNVRAMDel_Add0_clicked() {
  add_item(ui->table_nv_del0, 1);
  ui->table_nv_del->setRowCount(0);  // 先清除右边表中的所有条目
  on_btnNVRAMDel_Add_clicked();      // 同时右边增加一个新条目
  ui->table_nv_del0->setFocus();

  write_value_ini(ui->table_nv_del0, ui->table_nv_del,
                  ui->table_nv_del0->rowCount() - 1);

  this->setWindowModified(true);
  updateIconStatus();
}

void MainWindow::on_btnNVRAMDel_Add_clicked() {
  if (ui->table_nv_del0->rowCount() == 0) return;

  loading = true;
  add_item(ui->table_nv_del, 1);
  loading = false;

  // 保存数据
  write_value_ini(ui->table_nv_del0, ui->table_nv_del,
                  ui->table_nv_del0->currentRow());

  this->setWindowModified(true);
  updateIconStatus();
}

void MainWindow::on_btnNVRAMLS_Add0_clicked() {
  add_item(ui->table_nv_ls0, 1);
  ui->table_nv_ls->setRowCount(0);  // 先清除右边表中的所有条目
  on_btnNVRAMLS_Add_clicked();      // 同时右边增加一个新条目
  ui->table_nv_ls0->setFocus();

  write_value_ini(ui->table_nv_ls0, ui->table_nv_ls,
                  ui->table_nv_ls0->rowCount() - 1);

  this->setWindowModified(true);
  updateIconStatus();
}

void MainWindow::on_btnNVRAMLS_Add_clicked() {
  if (ui->table_nv_ls0->rowCount() == 0) return;

  loading = true;
  add_item(ui->table_nv_ls, 1);
  loading = false;

  // 保存数据
  write_value_ini(ui->table_nv_ls0, ui->table_nv_ls,
                  ui->table_nv_ls0->currentRow());

  this->setWindowModified(true);
  updateIconStatus();
}

void MainWindow::on_btnNVRAMDel_Del0_clicked() {
  mymethod->delRightTableItem(ui->table_nv_del0, ui->table_nv_del);
}

void MainWindow::on_btnNVRAMLS_Del0_clicked() {
  mymethod->delRightTableItem(ui->table_nv_ls0, ui->table_nv_ls);
}

void MainWindow::on_btnNVRAMDel_Del_clicked() {
  if (ui->table_nv_del->rowCount() <= 0) return;
  del_item(ui->table_nv_del);
  write_value_ini(ui->table_nv_del0, ui->table_nv_del,
                  ui->table_nv_del0->currentRow());
}

void MainWindow::on_btnNVRAMLS_Del_clicked() {
  if (ui->table_nv_ls->rowCount() <= 0) return;
  del_item(ui->table_nv_ls);
  write_value_ini(ui->table_nv_ls0, ui->table_nv_ls,
                  ui->table_nv_ls0->currentRow());
}

void MainWindow::on_btnUEFIRM_Add_clicked() {
  Method::add_OneLine(ui->table_uefi_ReservedMemory);
}

void MainWindow::on_btnUEFIRM_Del_clicked() {
  del_item(ui->table_uefi_ReservedMemory);
}

void MainWindow::on_btnUEFIDrivers_Add_clicked() {
  QFileDialog fd;
  QStringList FileName =
      fd.getOpenFileNames(this, "file", "", "efi file(*.efi);;all(*.*)");
  addEFIDrivers(FileName);
}

void MainWindow::addEFIDrivers(QStringList FileName) {
  if (FileName.isEmpty()) return;
  QTableWidget* t = ui->table_uefi_drivers;
  int pathCol = 0;
  for (int n = 0; n < t->columnCount(); n++) {
    QString txt = t->horizontalHeaderItem(n)->text();
    if (txt == "Path") {
      pathCol = n;
      break;
    }
  }
  QStringList tempList = mymethod->delDuplication(FileName, t, pathCol);
  FileName.clear();
  FileName = tempList;
  int colCount = t->columnCount();
  for (int i = 0; i < FileName.count(); i++) {
    int row = t->rowCount() + 1;
    QString strBaseName = QFileInfo(FileName.at(i)).fileName();
    if (colCount == 1) {
      t->setRowCount(row);
      t->setItem(row - 1, 0, new QTableWidgetItem(strBaseName));
    }
    if (colCount > 1) {
      t->setRowCount(row);
      for (int n = 0; n < colCount; n++) {
        QString txt = t->horizontalHeaderItem(n)->text();
        if (txt == "Path") {
          t->setItem(row - 1, n, new QTableWidgetItem(strBaseName));
        } else if (Method::isBool(txt)) {
          init_enabled_data(t, row - 1, n, "true");
        } else if (txt == "Load") {
          QTableWidgetItem* item = new QTableWidgetItem("Enabled");
          item->setTextAlignment(Qt::AlignCenter);
          t->setItem(row - 1, n, item);
        } else {
          t->setItem(row - 1, n, new QTableWidgetItem(""));
        }
      }
    }

    ui->table_uefi_drivers->setFocus();
    ui->table_uefi_drivers->setCurrentCell(row - 1, 0);

    QDir dir(strDrivers);
    if (dir.exists()) {
      QFile::copy(FileName.at(i), strDrivers + strBaseName);
    }
  }

  this->setWindowModified(true);
  updateIconStatus();
  checkFiles(ui->table_uefi_drivers);
}

void MainWindow::on_btnUEFIDrivers_Del_clicked() {
  del_item(ui->table_uefi_drivers);
}

void MainWindow::MoveItem(QTableWidget* t, bool up) {
  t->setFocus();
  t->setSelectionMode(QAbstractItemView::SingleSelection);
  t->setSelectionBehavior(QAbstractItemView::SelectRows);

  if (up) {
    if (t->rowCount() == 0 || t->currentRow() == 0 || t->currentRow() < 0) {
      t->setSelectionMode(QAbstractItemView::ExtendedSelection);
      t->setSelectionBehavior(QAbstractItemView::SelectItems);
      return;
    }
  } else {
    if (t->currentRow() == t->rowCount() - 1 || t->currentRow() < 0) {
      t->setSelectionMode(QAbstractItemView::ExtendedSelection);
      t->setSelectionBehavior(QAbstractItemView::SelectItems);
      return;
    }
  }

  QStringList items;
  int cr = t->currentRow();
  int cols = t->columnCount();
  QList<int> enabledList;
  for (int i = 0; i < cols; i++) {
    if (Method::isBool(t->horizontalHeaderItem(i)->text()))
      enabledList.append(i);
  }

  if (up) {
    // 先将上面的内容进行备份
    for (int i = 0; i < cols; i++) {
      items.append(t->item(cr - 1, i)->text());
    }

    // 将下面的内容移到上面
    for (int i = 0; i < cols; i++) {
      t->item(cr - 1, i)->setText(t->item(cr, i)->text());
    }

    for (int i = 0; i < enabledList.count(); i++) {
      if (t->item(cr, enabledList.at(i))->text() == "true")
        t->item(cr - 1, enabledList.at(i))->setCheckState(Qt::Checked);
      else
        t->item(cr - 1, enabledList.at(i))->setCheckState(Qt::Unchecked);
    }

    // 最后将之前的备份恢复到下面
    for (int i = 0; i < cols; i++) {
      t->item(cr, i)->setText(items.at(i));
    }

    for (int i = 0; i < enabledList.count(); i++) {
      if (items.at(enabledList.at(i)) == "true")
        t->item(cr, enabledList.at(i))->setCheckState(Qt::Checked);
      else
        t->item(cr, enabledList.at(i))->setCheckState(Qt::Unchecked);
    }

    t->setCurrentCell(cr - 1, t->currentColumn());

  } else {
    // 先将下面的内容进行备份
    for (int i = 0; i < cols; i++) {
      items.append(t->item(cr + 1, i)->text());
    }

    // 将上面的内容移到下面
    for (int i = 0; i < cols; i++) {
      t->item(cr + 1, i)->setText(t->item(cr, i)->text());
    }

    for (int i = 0; i < enabledList.count(); i++) {
      if (t->item(cr, enabledList.at(i))->text() == "true")
        t->item(cr + 1, enabledList.at(i))->setCheckState(Qt::Checked);
      else
        t->item(cr + 1, enabledList.at(i))->setCheckState(Qt::Unchecked);
    }

    // 最后将之前的备份恢复到上面
    for (int i = 0; i < cols; i++) {
      t->item(cr, i)->setText(items.at(i));
    }

    for (int i = 0; i < enabledList.count(); i++) {
      if (items.at(enabledList.at(i)) == "true")
        t->item(cr, enabledList.at(i))->setCheckState(Qt::Checked);
      else
        t->item(cr, enabledList.at(i))->setCheckState(Qt::Unchecked);
    }

    t->setCurrentCell(cr + 1, t->currentColumn());
  }

  t->setSelectionMode(QAbstractItemView::ExtendedSelection);
  t->setSelectionBehavior(QAbstractItemView::SelectItems);

  this->setWindowModified(true);
  updateIconStatus();
  checkFiles(t);
}

void MainWindow::on_btnKernelAdd_Up_clicked() {
  MoveItem(ui->table_kernel_add, true);
}

void MainWindow::on_btnKernelAdd_Down_clicked() {
  MoveItem(ui->table_kernel_add, false);
}

void MainWindow::on_btnSaveAs() {
  QFileDialog fd;

  PlistFileName =
      fd.getSaveFileName(this, "plist", QDir::homePath() + "/Desktop/",
                         "plist(*.plist);;all(*.*)");
  if (!PlistFileName.isEmpty()) {
    setWindowTitle(title + "      [*]" + PlistFileName);

    mymethod->removeFileSystemWatch(SaveFileName);
    SaveFileName = PlistFileName;
    mymethod->addFileSystemWatch(SaveFileName);

  } else {
    if (closeSave) clear_temp_data();

    return;
  }

  SavePlist(PlistFileName);

  ui->actionSave->setEnabled(true);

  QSettings settings;
  QFileInfo fInfo(PlistFileName);
  settings.setValue("currentDirectory", fInfo.absolutePath());
  // qDebug() << settings.fileName(); //最近打开的文件所保存的位置
  m_recentFiles->setMostRecentFile(PlistFileName);
  initRecentFilesForToolBar();
}

void MainWindow::about() {
  aboutDlg->setModal(true);
  aboutDlg->show();
}

void MainWindow::on_btnKernelAdd_Del_clicked() {
  del_item(ui->table_kernel_add);
}

void MainWindow::on_table_dp_add_cellClicked(int row, int column) {
  if (column == 1) {
    cboxDataClass = new QComboBox;
    cboxDataClass->setEditable(true);
    cboxDataClass->addItem("Data");
    cboxDataClass->addItem("String");
    cboxDataClass->addItem("Number");
    cboxDataClass->addItem("");
    connect(cboxDataClass, SIGNAL(currentTextChanged(QString)), this,
            SLOT(dataClassChange_dp()));
    c_row = row;

    ui->table_dp_add->setCellWidget(row, column, cboxDataClass);
    cboxDataClass->setCurrentText(ui->table_dp_add->item(row, 1)->text());
  }

  setStatusBarText(ui->table_dp_add);
}

void MainWindow::on_table_dp_add_currentCellChanged(int currentRow,
                                                    int currentColumn,
                                                    int previousRow,
                                                    int previousColumn) {
  ui->table_dp_add->removeCellWidget(previousRow, 1);

  currentCellChanged(ui->table_dp_add, previousRow, previousColumn, currentRow,
                     currentColumn);
}

void MainWindow::arch_addChange() {
  int cc = ui->table_kernel_add->currentColumn();
  if (ui->table_kernel_add->horizontalHeaderItem(cc)->text() == "Arch")
    ui->table_kernel_add->item(c_row, cc)->setText(cboxArch->currentText());
}

void MainWindow::arch_ForceChange() {
  int cc = ui->table_kernel_Force->currentColumn();
  if (ui->table_kernel_Force->horizontalHeaderItem(cc)->text() == "Arch")
    ui->table_kernel_Force->item(c_row, cc)->setText(cboxArch->currentText());
}

void MainWindow::arch_blockChange() {
  QTableWidget* t = ui->table_kernel_block;
  int ArchCol = 0;
  int StrategyCol = 0;
  QString txt = t->horizontalHeaderItem(t->currentColumn())->text();
  for (int i = 0; i < t->columnCount(); i++) {
    if (t->horizontalHeaderItem(i)->text() == "Arch") ArchCol = i;
    if (t->horizontalHeaderItem(i)->text() == "Strategy") StrategyCol = i;
  }
  if (txt == "Arch") t->item(c_row, ArchCol)->setText(cboxArch->currentText());
  if (txt == "Strategy")
    t->item(c_row, StrategyCol)->setText(cboxArch->currentText());
}

void MainWindow::arch_patchChange() {
  int cc = ui->table_kernel_patch->currentColumn();
  if (ui->table_kernel_patch->horizontalHeaderItem(cc)->text() == "Arch")
    ui->table_kernel_patch->item(c_row, cc)->setText(cboxArch->currentText());
}

void MainWindow::arch_Booter_patchChange() {
  int cc = ui->table_Booter_patch->currentColumn();
  if (ui->table_Booter_patch->horizontalHeaderItem(cc)->text() == "Arch")
    ui->table_Booter_patch->item(c_row, cc)->setText(cboxArch->currentText());
}

void MainWindow::ReservedMemoryTypeChange() {
  int cc = ui->table_uefi_ReservedMemory->currentColumn();
  if (ui->table_uefi_ReservedMemory->horizontalHeaderItem(cc)->text() == "Type")
    ui->table_uefi_ReservedMemory->item(c_row, cc)->setText(
        cboxReservedMemoryType->currentText());
}

void MainWindow::dataClassChange_dp() {
  ui->table_dp_add->item(c_row, 1)->setTextAlignment(Qt::AlignCenter);
  ui->table_dp_add->item(c_row, 1)->setText(cboxDataClass->currentText());

  if (!loading)
    write_ini(ui->table_dp_add0, ui->table_dp_add,
              ui->table_dp_add0->currentRow());
}

void MainWindow::dataClassChange_nv() {
  ui->table_nv_add->item(c_row, 1)->setTextAlignment(Qt::AlignCenter);
  ui->table_nv_add->item(c_row, 1)->setText(cboxDataClass->currentText());

  if (!loading)
    write_ini(ui->table_nv_add0, ui->table_nv_add,
              ui->table_nv_add0->currentRow());
}

void MainWindow::on_table_nv_add_cellClicked(int row, int column) {
  if (column == 1) {
    cboxDataClass = new QComboBox;
    cboxDataClass->setEditable(true);
    cboxDataClass->addItem("Data");
    cboxDataClass->addItem("String");
    cboxDataClass->addItem("Number");
    cboxDataClass->addItem("Boolean");
    cboxDataClass->addItem("");
    connect(cboxDataClass, SIGNAL(currentTextChanged(QString)), this,
            SLOT(dataClassChange_nv()));
    c_row = row;

    ui->table_nv_add->setCellWidget(row, column, cboxDataClass);
    cboxDataClass->setCurrentText(ui->table_nv_add->item(row, 1)->text());
  }

  if (ui->table_nv_add0->currentItem()->text().trimmed() ==
      "7C436110-AB2A-4BBB-A880-FE41995C9F82") {
    if (ui->table_nv_add->item(row, 0)->text().trimmed() ==
        "SystemAudioVolume") {
      if (column == 0 || column == 2) {
        // QToolTip::showText(
        //    QCursor::pos(),
        //"MinimumVolume: " + ui->editIntMinimumVolume->text().trimmed());
      }
    }
  }

  setStatusBarText(ui->table_nv_add);
}

void MainWindow::on_table_nv_add_currentCellChanged(int currentRow,
                                                    int currentColumn,
                                                    int previousRow,
                                                    int previousColumn) {
  ui->table_nv_add->removeCellWidget(previousRow, 1);

  currentCellChanged(ui->table_nv_add, previousRow, previousColumn, currentRow,
                     currentColumn);
}

void MainWindow::initLineEdit(QTableWidget* Table, int previousRow,
                              int previousColumn, int currentRow,
                              int currentColumn) {
  if (!Initialization) {
    if (Table->rowCount() == 0) return;

    Table->removeCellWidget(previousRow, previousColumn);
    removeAllLineEdit();

    lineEdit = new QLineEdit(this);
    lineEditModifyed = false;

    lineEdit->setContextMenuPolicy(Qt::CustomContextMenu);

    QAction* undoAction = new QAction(tr("Undo"));
    QAction* redoAction = new QAction(tr("Redo"));
    QAction* copyAction = new QAction(tr("Copy"));
    // copyAction->setShortcuts(QKeySequence::Copy);
    QAction* cutAction = new QAction(tr("Cut"));
    // cutAction->setShortcuts(QKeySequence::Cut);
    QAction* pasteAction = new QAction(tr("Paste"));
    // pasteAction->setShortcuts(QKeySequence::Paste);
    QAction* setallAction = new QAction(tr("Select All"));

    QMenu* popMenu = new QMenu(this);
    popMenu->addAction(undoAction);
    popMenu->addAction(redoAction);
    popMenu->addSeparator();
    popMenu->addAction(copyAction);
    popMenu->addAction(cutAction);
    popMenu->addAction(pasteAction);
    popMenu->addSeparator();
    popMenu->addAction(setallAction);

    connect(undoAction, &QAction::triggered, [=]() { lineEdit->undo(); });
    connect(redoAction, &QAction::triggered, [=]() { lineEdit->redo(); });
    connect(copyAction, &QAction::triggered, [=]() { lineEdit->copy(); });
    connect(cutAction, &QAction::triggered, [=]() { lineEdit->cut(); });
    connect(pasteAction, &QAction::triggered, [=]() {
      lineEdit->paste();
      lineEditSetText();
    });
    connect(setallAction, &QAction::triggered,
            [=]() { lineEdit->selectAll(); });

    Table->setCurrentCell(currentRow, currentColumn);
    Table->setCellWidget(currentRow, currentColumn, lineEdit);

    lineEdit->setToolTip("");
    if (getTableFieldDataType(myTable) == "Int") {
      QValidator* validator =
          new QRegularExpressionValidator(regxNumber, lineEdit);
      lineEdit->setValidator(validator);
      lineEdit->setPlaceholderText(tr("Integer"));
      lineEdit->setToolTip(tr("Integer"));
    }

    if (getTableFieldDataType(myTable) == "Data") {
      QValidator* validator =
          new QRegularExpressionValidator(regxData, lineEdit);
      lineEdit->setValidator(validator);
      lineEdit->setPlaceholderText(tr("Hexadecimal"));
      lineEdit->setToolTip(tr("Hexadecimal"));
    }

    if (Table == ui->table_nv_add) {
      if (currentColumn == 2) {
        if (Table->item(currentRow, 1)->text() == "Boolean") {
          lineEdit->setPlaceholderText(tr("true or false"));
          lineEdit->setToolTip(tr("true or false"));
        }
      }
    }

    loading = true;
    if (Table->currentIndex().isValid()) {
      lineEdit->setText(Table->item(currentRow, currentColumn)->text());
      lineEdit->selectAll();
    }

    loading = false;

    lineEdit->setFocus();
    lineEdit->setClearButtonEnabled(false);

    connect(lineEdit, &QLineEdit::returnPressed, this,
            &MainWindow::lineEditSetText);
    connect(lineEdit, &QLineEdit::textChanged, this,
            &MainWindow::lineEdit_textChanged);
    connect(lineEdit, &QLineEdit::customContextMenuRequested,
            [=](const QPoint& pos) {
              Q_UNUSED(pos);

              QString str = lineEdit->selectedText();

              if (str.length() == 0) {
                copyAction->setEnabled(false);
                cutAction->setEnabled(false);
              } else {
                copyAction->setEnabled(true);
                cutAction->setEnabled(true);
              }

              QClipboard* clipboard = QApplication::clipboard();
              QString str1 = clipboard->text();
              if (str1.length() > 0)
                pasteAction->setEnabled(true);
              else
                pasteAction->setEnabled(false);

              if (lineEditModifyed) {
                undoAction->setEnabled(true);
                redoAction->setEnabled(true);
              } else {
                undoAction->setEnabled(false);
                redoAction->setEnabled(false);
              }

              if (lineEdit->text().length() > 0)
                setallAction->setEnabled(true);
              else
                setallAction->setEnabled(false);

              popMenu->exec(QCursor::pos());
            });

    InitEdit = true;
  }
}

QString MainWindow::getTableFieldDataType(QTableWidget* table) {
  int col = table->currentColumn();
  QString strHeader = table->horizontalHeaderItem(col)->text();

  if (Method::isInt(strHeader)) return "Int";
  if (Method::isData(strHeader)) return "Data";

  if (table == ui->table_dp_add || table == ui->table_nv_add) {
    int row, col;
    row = table->currentRow();
    col = table->currentColumn();

    if (table->item(row, 1)->text() == "Number" && col == 2) return "Int";
    if (table->item(row, 1)->text() == "Data" && col == 2) return "Data";
  }

  return "";
}

void MainWindow::reg_win() {
  QString appPath = qApp->applicationFilePath();

  QString dir = qApp->applicationDirPath();
  // 注意路径的替换
  appPath.replace("/", "\\");
  QString type = "QtiASL";
  QSettings* regType =
      new QSettings("HKEY_CLASSES_ROOT\\.plist", QSettings::NativeFormat);
  QSettings* regIcon = new QSettings("HKEY_CLASSES_ROOT\\.plist\\DefaultIcon",
                                     QSettings::NativeFormat);
  QSettings* regShell = new QSettings(
      "HKEY_CLASSES_ROOT\\" + strAppName + "\\shell\\open\\command",
      QSettings::NativeFormat);

  regType->remove("Default");
  regType->setValue("Default", type);

  regIcon->remove("Default");
  // 0 使用当前程序内置图标
  regIcon->setValue("Default", appPath + ",1");

  // 百分号问题
  QString shell = "\"" + appPath + "\" ";
  shell = shell + "\"%1\"";

  regShell->remove("Default");
  regShell->setValue("Default", shell);

  delete regIcon;
  delete regShell;
  delete regType;

  // 通知系统刷新
#ifdef Q_OS_WIN32
  //::SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST|SHCNF_FLUSH, 0, 0);
#endif
}

void MainWindow::on_table_kernel_add_currentCellChanged(int currentRow,
                                                        int currentColumn,
                                                        int previousRow,
                                                        int previousColumn) {
  ui->table_kernel_add->removeCellWidget(previousRow, 7);

  currentCellChanged(ui->table_kernel_add, previousRow, previousColumn,
                     currentRow, currentColumn);
}

void MainWindow::on_table_kernel_block_currentCellChanged(int currentRow,
                                                          int currentColumn,
                                                          int previousRow,
                                                          int previousColumn) {
  ui->table_kernel_block->removeCellWidget(previousRow, 5);

  currentCellChanged(ui->table_kernel_block, previousRow, previousColumn,
                     currentRow, currentColumn);
}

void MainWindow::on_table_kernel_patch_currentCellChanged(int currentRow,
                                                          int currentColumn,
                                                          int previousRow,
                                                          int previousColumn) {
  ui->table_kernel_patch->removeCellWidget(previousRow, 13);

  currentCellChanged(ui->table_kernel_patch, previousRow, previousColumn,
                     currentRow, currentColumn);
}

QString MainWindow::getSystemProductName(QString arg1) {
  QString str;
  for (int i = 0; i < arg1.count(); i++) {
    if (arg1.mid(i, 1) == " ") {
      str = arg1.mid(0, i).trimmed();
      return str;
      break;
    }
  }
  return arg1;
}

void MainWindow::on_cboxSystemProductName_currentIndexChanged(
    const QString& arg1) {
  QString str = getSystemProductName(arg1);
  ui->editSystemProductName->setText(str);
  ui->editSystemProductName_2->setText(str);
  this->setWindowModified(true);
  updateIconStatus();
}

void MainWindow::readResult() {
  QTextEdit* textMacInfo = new QTextEdit;
  textMacInfo->clear();
  QString result = gs->readAll();
  textMacInfo->append(result);
  // 取第三行的数据，第一行留给提示用
  QString str = textMacInfo->document()->findBlockByNumber(2).text().trimmed();

  QString str1, str2;
  for (int i = 0; i < str.count(); i++) {
    if (str.mid(i, 1) == "|") {
      str1 = str.mid(0, i).trimmed();
      str2 = str.mid(i + 1, str.count() - i + 1).trimmed();
    }
  }

  ui->editSystemSerialNumber->setText(str1);
  ui->editSystemSerialNumber_data->setText(str1);
  ui->editSystemSerialNumber_2->setText(str1);
  ui->editSystemSerialNumber_PNVRAM->setText(str1);

  ui->editMLB->setText(str2);
  ui->editMLB_PNVRAM->setText(str2);

  on_btnSystemUUID_clicked();
}

void MainWindow::on_btnSystemProductName_clicked() {
  QString arg1 = ui->cboxSystemProductName->currentText();
  if (!loading && arg1 != "") {
    gs = new QProcess;

    QString str = getSystemProductName(arg1);
    ui->editSystemProductName->setText(str);
    ui->editSystemProductName_2->setText(str);

#ifdef Q_OS_WIN32

    gs->start(userDataBaseDir + "win/macserial.exe",
              QStringList() << "-m" << str);  // 阻塞为execute

#endif

#ifdef Q_OS_LINUX

    gs->start(userDataBaseDir + "linux/macserial", QStringList()
                                                       << "-m" << str);

#endif

#ifdef Q_OS_MAC

    gs->start(userDataBaseDir + "mac/macserial", QStringList() << "-m" << str);

#endif

    connect(gs, SIGNAL(finished(int)), this, SLOT(readResult()));
  }
}

void MainWindow::on_btnSystemUUID_clicked() {
  QUuid id = QUuid::createUuid();
  QString strTemp = id.toString();
  QString strId = strTemp.mid(1, strTemp.count() - 2).toUpper();
  ui->editSystemUUID->setText(strId);
}

void MainWindow::on_table_kernel_Force_cellClicked(int row, int column) {
  if (!ui->table_kernel_Force->currentIndex().isValid()) return;

  set_InitCheckBox(ui->table_kernel_Force, row, column);

  QString txt = ui->table_kernel_Force->horizontalHeaderItem(column)->text();
  if (txt == "Arch") {
    cboxArch = new QComboBox;
    cboxArch->setEditable(true);
    cboxArch->addItem("Any");
    cboxArch->addItem("i386");
    cboxArch->addItem("x86_64");
    cboxArch->addItem("");

    connect(cboxArch, SIGNAL(currentTextChanged(QString)), this,
            SLOT(arch_ForceChange()));
    c_row = row;

    ui->table_kernel_Force->setCellWidget(row, column, cboxArch);
    cboxArch->setCurrentText(ui->table_kernel_Force->item(row, column)->text());
  }

  setStatusBarText(ui->table_kernel_Force);
}

void MainWindow::on_table_kernel_Force_currentCellChanged(int currentRow,
                                                          int currentColumn,
                                                          int previousRow,
                                                          int previousColumn) {
  ui->table_kernel_Force->removeCellWidget(previousRow, 8);

  currentCellChanged(ui->table_kernel_Force, previousRow, previousColumn,
                     currentRow, currentColumn);
}

void MainWindow::on_btnKernelForce_Add_clicked() {
  Method::add_OneLine(ui->table_kernel_Force);
}

void MainWindow::on_btnKernelForce_Del_clicked() {
  del_item(ui->table_kernel_Force);
}

void MainWindow::dragEnterEvent(QDragEnterEvent* e) {
  if (e->mimeData()->hasFormat("text/uri-list")) {
    e->acceptProposedAction();
  }
}

void MainWindow::dropEvent(QDropEvent* e) {
  QList<QUrl> urls = e->mimeData()->urls();
  if (urls.isEmpty()) {
    return;
  }

  QStringList fileList;
  for (int i = 0; i < urls.count(); i++) {
    fileList.append(urls.at(i).toLocalFile());
  }

  // QString fileName = urls.first().toLocalFile();
  if (fileList.isEmpty()) {
    return;
  }

  // plist
  QFileInfo fi(fileList.at(0));
  if (fi.suffix().toLower() == "plist") {
    PlistFileName = fileList.at(0);
    openFile(PlistFileName);
  }

  // aml
  if (fi.suffix().toLower() == "aml") {
    if (ui->tabTotal->currentIndex() == 0 && ui->tabACPI->currentIndex() == 0) {
      addACPIItem(fileList);
    }
  }

  // kext
  QString fileSuffix;
#ifdef Q_OS_WIN32
  fileSuffix = fi.suffix().toLower();
#endif

#ifdef Q_OS_LINUX
  fileSuffix = fi.suffix().toLower();
#endif

#ifdef Q_OS_MAC
  QString str1 = fileList.at(0);
  QString str2 = str1.mid(str1.length() - 5, 4);
  fileSuffix = str2.toLower();
#endif

  if (fileSuffix == "kext") {
    if (ui->tabTotal->currentIndex() == 3 &&
        ui->tabKernel->currentIndex() == 0) {
      QStringList kextList;
      for (int i = 0; i < fileList.count(); i++) {
        QString str3 = fileList.at(i);
        QString str4;
#ifdef Q_OS_WIN32
        str4 = str3.mid(0, str3.length());
#endif

#ifdef Q_OS_LINUX
        str4 = str3.mid(0, str3.length());
#endif

#ifdef Q_OS_MAC
        str4 = str3.mid(0, str3.length() - 1);
#endif

        kextList.append(str4);
      }

      addKexts(kextList);
    }
  }

  // efi tools
  if (fi.suffix().toLower() == "efi") {
    if (ui->tabTotal->currentIndex() == 4 && ui->tabMisc->currentIndex() == 5) {
      addEFITools(fileList);
    }
  }

  // efi drivers
  if (fi.suffix().toLower() == "efi") {
    if (ui->tabTotal->currentIndex() == 7 && ui->tabUEFI->currentIndex() == 3) {
      addEFIDrivers(fileList);
    }
  }
}

#ifdef Q_OS_WIN32
void MainWindow::runAdmin(QString file, QString arg) {
  QString exePath = file;
  WCHAR exePathArray[1024] = {0};
  exePath.toWCharArray(exePathArray);

  QString command = arg;  //"-mount:*";//带参数运行
  WCHAR commandArr[1024] = {0};
  command.toWCharArray(commandArr);
  HINSTANCE hNewExe =
      ShellExecute(NULL, L"runas", exePathArray, commandArr, NULL,
                   SW_SHOWMAXIMIZED);  // SW_NORMAL SW_SHOWMAXIMIZED
  if (hNewExe) {
  };
}
#endif

void MainWindow::mount_esp() {
#ifdef Q_OS_WIN32

  QString exec =
      QCoreApplication::applicationDirPath() + "/Database/win/FindESP.exe";

  // runAdmin(exec, "-unmount:*");
  runAdmin(exec, "-mount:*");  // 可选参数-Updater

  QString exec2 =
      QCoreApplication::applicationDirPath() + "/Database/win/winfile.exe";

  runAdmin(exec2, NULL);  // 此时参数为空

#endif

#ifdef Q_OS_LINUX

#endif

#ifdef Q_OS_MAC

  di = new QProcess;
  di->start("diskutil", QStringList() << "list");
  connect(di, SIGNAL(finished(int)), this, SLOT(readResultDiskInfo()));

#endif
}

void MainWindow::readResultDiskInfo() {
  dlgMESP = new dlgMountESP(this);
  dlgMESP->setModal(true);

  int row = Reg.value("mesp", 0).toInt();

  dlgMESP->ui->listWidget->clear();

  QTextEdit* textDiskInfo = new QTextEdit;
  textDiskInfo->clear();
  QString result = di->readAll();
  textDiskInfo->append(result);

  QString str0, str1;
  int count = textDiskInfo->document()->lineCount();
  for (int i = 0; i < count; i++) {
    str0 = textDiskInfo->document()->findBlockByNumber(i).text().trimmed();
    QStringList strList = str0.simplified().split(" ");
    if (strList.count() >= 5) {
      if (strList.at(1).toUpper() == "EFI") {
        QString strDisk = strList.at(strList.count() - 1).mid(0, 5);
        QString strIDENTIFIER = strList.at(strList.count() - 1);
        str1 = strList.at(strList.count() - 1);
        str1 = str1.trimmed();
        if (myDlgPreference->ui->chkShowVolName->isChecked()) {
          str1 = str1 + " | " + mymethod->getVolForPartition(strIDENTIFIER) +
                 " | " + mymethod->getDriverName(strDisk) + " | " +
                 mymethod->getDriverVolInfo(strDisk);
        } else {
          str1 = str1 + " | " + mymethod->getVolForPartition(strIDENTIFIER) +
                 " | " + mymethod->getDriverName(strDisk);
        }
        dlgMESP->ui->listWidget->addItem(new QListWidgetItem(str1));
        // dlgMESP->ui->listWidget->setIconSize(QSize(30, 30));
        // dlgMESP->ui->listWidget->addItem(
        //    new QListWidgetItem(QIcon(":/icon/espicon.png"), str1));
      }
    }
  }

  if (dlgMESP->ui->listWidget->count() > 0) {
    if (row < dlgMESP->ui->listWidget->count())
      dlgMESP->ui->listWidget->setCurrentRow(row);
    else
      dlgMESP->ui->listWidget->setCurrentRow(0);
  }

  if (!Initialization) dlgMESP->show();
}

void MainWindow::closeEvent(QCloseEvent* event) {
  Reg.setValue("SaveDataHub", ui->mychkSaveDataHub->isChecked());
  Reg.setValue("AutoChkUpdate", ui->actionAutoChkUpdate->isChecked());
  Reg.setValue("Net", myDlgPreference->ui->comboBoxNet->currentText());
  Reg.setValue("LastFileName", SaveFileName);

  int textTotal = ui->mycboxFind->count();
  Reg.setValue("textTotal", textTotal);
  for (int i = 0; i < textTotal; i++) {
    Reg.setValue(QString::number(i), ui->mycboxFind->itemText(i));
  }

  Reg.setValue("x", this->x());
  Reg.setValue("y", this->y());
  Reg.setValue("width", this->width());
  Reg.setValue("height", this->height());

  if (this->isWindowModified()) {
    this->setFocus();

    int choice;

    this->setFocus();

    QMessageBox message(
        QMessageBox::Warning, tr("Application"),
        tr("The document has been modified.") + "\n" +
            tr("Do you want to save your changes?") + "\n\n" + SaveFileName,
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

    message.setButtonText(QMessageBox::Save, QString(tr("Save")));
    message.setButtonText(QMessageBox::Cancel, QString(tr("Cancel")));
    message.setButtonText(QMessageBox::Discard, QString(tr("Discard")));
    message.setDefaultButton(QMessageBox::Save);
    choice = message.exec();

    switch (choice) {
      case QMessageBox::Save:

        closeSave = true;
        if (SaveFileName == "")
          on_btnSaveAs();
        else
          on_btnSave();

        event->accept();
        break;
      case QMessageBox::Discard:
        clear_temp_data();
        event->accept();
        break;
      case QMessageBox::Cancel:
        ui->listMain->setFocus();
        event->ignore();

        ui->mycboxFind->setFocus();
        break;
    }
  } else {
    clear_temp_data();
    event->accept();
  }

  mymethod->cancelKextUpdate();
  QString tempDir = QDir::homePath() + "/tempocat/";
  deleteDirfile(tempDir);
}

void MainWindow::on_table_uefi_ReservedMemory_currentCellChanged(
    int currentRow, int currentColumn, int previousRow, int previousColumn) {
  currentCellChanged(ui->table_uefi_ReservedMemory, previousRow, previousColumn,
                     currentRow, currentColumn);
}

void MainWindow::on_table_uefi_Unload_currentCellChanged(int currentRow,
                                                         int currentColumn,
                                                         int previousRow,
                                                         int previousColumn) {
  currentCellChanged(ui->table_uefi_Unload, previousRow, previousColumn,
                     currentRow, currentColumn);
}

void MainWindow::on_btnHelp() {
  QString qtManulFile = userDataBaseDir + "doc/Configuration.pdf";
  QDesktopServices::openUrl(QUrl::fromLocalFile(qtManulFile));
}

void MainWindow::on_tabTotal_tabBarClicked(int index) {
  switch (index) {
    case 0:
      ui->btnExportMaster->setText(tr("Export") + "  ACPI");
      ui->btnImportMaster->setText(tr("Import") + "  ACPI");

      break;

    case 1:
      ui->btnExportMaster->setText(tr("Export") + "  Booter");
      ui->btnImportMaster->setText(tr("Import") + "  Booter");
      break;

    case 2:
      ui->btnExportMaster->setText(tr("Export") + "  DeviceProperties");
      ui->btnImportMaster->setText(tr("Import") + "  DeviceProperties");
      break;

    case 3:
      ui->btnExportMaster->setText(tr("Export") + "  Kernel");
      ui->btnImportMaster->setText(tr("Import") + "  Kernel");
      break;

    case 4:
      ui->btnExportMaster->setText(tr("Export") + "  Misc");
      ui->btnImportMaster->setText(tr("Import") + "  Misc");
      break;

    case 5:
      ui->btnExportMaster->setText(tr("Export") + "  NVRAM");
      ui->btnImportMaster->setText(tr("Import") + "  NVRAM");
      break;

    case 6:
      ui->btnExportMaster->setText(tr("Export") + "  PlatformInfo");
      ui->btnImportMaster->setText(tr("Import") + "  PlatformInfo");
      break;

    case 7:
      ui->btnExportMaster->setText(tr("Export") + "  UEFI");
      ui->btnImportMaster->setText(tr("Import") + "  UEFI");
      break;
  }
}

void MainWindow::on_tabTotal_currentChanged(int index) {
  on_tabTotal_tabBarClicked(index);

  currentMainTabWidget = ui->tabTotal->widget(index);
}

void MainWindow::on_btnDevices_add_clicked() {
  Method::add_OneLine(ui->tableDevices);
}

void MainWindow::on_btnDevices_del_clicked() { del_item(ui->tableDevices); }

void MainWindow::on_cboxUpdateSMBIOSMode_currentIndexChanged(
    const QString& arg1) {
  if (arg1 == "Custom")
    ui->chkCustomSMBIOSGuid->setChecked(true);  // 联动
  else
    ui->chkCustomSMBIOSGuid->setChecked(false);
}

void MainWindow::ExposeSensitiveData() {
  initExposeSensitiveDataValue();

  int total = 0;
  for (int i = 0; i < chk.count(); i++) {
    if (chk.at(i)->isChecked()) total = total + v.at(i);
  }

  ui->editIntExposeSensitiveData->setText(QString::number(total));
}

void MainWindow::initExposeSensitiveDataValue() {
  chk.clear();
  for (int i = 0; i < chk_ExposeSensitiveData.count(); i++) {
    chk.append(chk_ExposeSensitiveData.at(i));
  }

  v.clear();
  v.append(1);
  v.append(2);
  v.append(4);
  v.append(8);
}

void MainWindow::on_editIntExposeSensitiveData_textChanged(
    const QString& arg1) {
  int total = arg1.toInt();

  initExposeSensitiveDataValue();

  for (int i = 0; i < chk.count(); i++) chk.at(i)->setChecked(false);

  method(v, total);

  // 10 to 16
  QString hex =
      QString("%1").arg(total, 2, 16, QLatin1Char('0'));  // 保留2位，不足补零
  ui->lblExposeSensitiveData->setText("0x" + hex.toUpper());
}

void MainWindow::ScanPolicy() {
  int total = 0;

  initScanPolicyValue();

  for (int i = 0; i < chk.count(); i++) {
    if (chk.at(i)->isChecked()) total = total + v.at(i);
  }

  ui->editIntScanPolicy->setText(QString::number(total));
}

void MainWindow::initScanPolicyValue() {
  chk.clear();
  for (int i = 0; i < chk_ScanPolicy.count(); i++) {
    chk.append(chk_ScanPolicy.at(i));
  }

  v.clear();
  v.append(1);
  v.append(2);
  v.append(256);
  v.append(512);
  v.append(1024);
  v.append(2048);
  v.append(4096);

  v.append(8192);
  v.append(16384);

  v.append(65536);
  v.append(131072);
  v.append(262144);
  v.append(524288);
  v.append(1048576);
  v.append(2097152);
  v.append(4194304);
  v.append(8388608);
  v.append(16777216);
}
void MainWindow::on_editIntScanPolicy_textChanged(const QString& arg1) {
  int total = arg1.toInt();

  initScanPolicyValue();

  for (int i = 0; i < chk.count(); i++) chk.at(i)->setChecked(false);

  method(v, total);

  // 10 to 16
  QString hex =
      QString("%1").arg(total, 8, 16, QLatin1Char('0'));  // 保留8位，不足补零
  ui->lblScanPolicy->setText("0x" + hex.toUpper());
}

void MainWindow::method(QVector<unsigned int> nums, unsigned int sum) {
  QVector<unsigned int> list;

  method(nums, sum, list, -1);
}

void MainWindow::method(QVector<unsigned int> nums, unsigned int sum,
                        QVector<unsigned int> list, int index) {
  if (sum == 0) {
    for (unsigned val : list) {
      for (int i = 0; i < nums.count(); i++) {
        if (val == v.at(i)) {
          chk.at(i)->setChecked(true);
        }
      }
    }
  } else if (sum > 0) {
    for (int i = index + 1; i < nums.count(); i++) {
      list.append(nums.at(i));
      method(nums, sum - nums.at(i), list, i);
      list.remove(list.size() - 1);
    }
  }
}

void MainWindow::initDisplayLevelValue() {
  v.clear();
  v.append(1);
  v.append(2);
  v.append(4);
  v.append(8);
  v.append(16);
  v.append(32);
  v.append(64);
  v.append(128);
  v.append(256);
  v.append(1024);
  v.append(4096);
  v.append(16384);
  v.append(65536);
  v.append(131072);
  v.append(524288);
  v.append(1048576);
  v.append(2097152);
  v.append(4194304);
  v.append(2147483648);

  chk.clear();
  for (int i = 0; i < chkDisplayLevel.count(); i++)
    chk.append(chkDisplayLevel.at(i));
}

void MainWindow::DisplayLevel() {
  initDisplayLevelValue();

  unsigned int total = 0;

  for (int i = 0; i < chk.count(); i++) {
    if (chk.at(i)->isChecked()) total = total + v.at(i);
  }

  ui->editIntDisplayLevel->setText(QString::number(total));
}

void MainWindow::on_editIntDisplayLevel_textChanged(const QString& arg1) {
  // 10 to 16
  unsigned int total = arg1.toULongLong();
  QString hex =
      QString("%1").arg(total, 8, 16, QLatin1Char('0'));  // 保留8位，不足补零
  ui->lblDisplayLevel->setText("0x" + hex.toUpper());

  initDisplayLevelValue();

  for (int i = 0; i < chk.count(); i++) {
    chk.at(i)->setChecked(false);
  }

  method(v, total);
}

void MainWindow::initPickerAttributesValue() {
  chk.clear();
  for (int i = 0; i < chk_PickerAttributes.count(); i++) {
    chk.append(chk_PickerAttributes.at(i));
  }

  v.clear();
  v.append(1);
  v.append(2);
  v.append(4);
  v.append(8);
  v.append(16);
  v.append(32);
  v.append(64);
  v.append(128);
}

void MainWindow::PickerAttributes() {
  initPickerAttributesValue();

  int total = 0;
  for (int i = 0; i < v.count(); i++) {
    if (chk.at(i)->isChecked()) total = total + v.at(i);
  }

  ui->editIntPickerAttributes->setText(QString::number(total));
}

void MainWindow::on_editIntPickerAttributes_textChanged(const QString& arg1) {
  int total = arg1.toInt();

  initPickerAttributesValue();

  for (int i = 0; i < v.count(); i++) chk.at(i)->setChecked(false);

  method(v, total);

  // 10 to 16
  QString hex =
      QString("%1").arg(total, 2, 16, QLatin1Char('0'));  // 保留2位，不足补零
  ui->lblPickerAttributes->setText("0x" + hex.toUpper());
}

void MainWindow::contextMenuEvent(QContextMenuEvent* event) {
  Q_UNUSED(event);

  QMenu menu(ui->table_nv_add);

  QAction* actionbootargs = new QAction("boot-args");

  menu.addAction(actionbootargs);

  // menu.exec();
}

void MainWindow::show_menu(const QPoint pos) {
  if (ui->table_nv_add0->currentIndex().data().toString() ==
      "7C436110-AB2A-4BBB-A880-FE41995C9F82") {
    // 设置菜单选项
    QMenu* menu = new QMenu(ui->table_nv_add);

    QAction* act1 = new QAction("+  boot-args", ui->table_nv_add);
    connect(act1, SIGNAL(triggered()), this, SLOT(on_nv1()));

    QAction* act2 = new QAction("+  bootercfg", ui->table_nv_add);
    connect(act2, SIGNAL(triggered()), this, SLOT(on_nv2()));

    QAction* act3 = new QAction("+  bootercfg-once", ui->table_nv_add);
    connect(act3, SIGNAL(triggered()), this, SLOT(on_nv3()));

    QAction* act4 = new QAction("+  efiboot-perf-record", ui->table_nv_add);
    connect(act4, SIGNAL(triggered()), this, SLOT(on_nv4()));

    QAction* act5 = new QAction("+  fmm-computer-name", ui->table_nv_add);
    connect(act5, SIGNAL(triggered()), this, SLOT(on_nv5()));

    QAction* act6 = new QAction("+  nvda_drv", ui->table_nv_add);
    connect(act6, SIGNAL(triggered()), this, SLOT(on_nv6()));

    QAction* act7 = new QAction("+  run-efi-updater", ui->table_nv_add);
    connect(act7, SIGNAL(triggered()), this, SLOT(on_nv7()));

    QAction* act8 = new QAction("+  StartupMute", ui->table_nv_add);
    connect(act8, SIGNAL(triggered()), this, SLOT(on_nv8()));

    QAction* act9 = new QAction("+  SystemAudioVolume", ui->table_nv_add);
    connect(act9, SIGNAL(triggered()), this, SLOT(on_nv9()));

    QAction* act10 = new QAction("+  csr-active-config", ui->table_nv_add);
    connect(act10, SIGNAL(triggered()), this, SLOT(on_nv10()));

    QAction* act11 = new QAction("+  prev-lang:kbd", ui->table_nv_add);
    connect(act11, SIGNAL(triggered()), this, SLOT(on_nv11()));

    QAction* act12 = new QAction("+  security-mode", ui->table_nv_add);
    connect(act12, SIGNAL(triggered()), this, SLOT(on_nv12()));

    menu->addAction(act1);
    menu->addAction(act2);
    menu->addAction(act3);
    menu->addAction(act4);
    menu->addAction(act5);
    menu->addAction(act6);
    menu->addAction(act7);
    menu->addAction(act8);
    menu->addAction(act9);
    menu->addAction(act10);
    menu->addAction(act11);
    menu->addAction(act12);

    int x0, y0;
    x0 = QCursor::pos().x();
    y0 = QCursor::pos().y();

    menu->move(x0 - 200, y0 - 200);

    menu->show();

    int x = pos.x();
    int y = pos.y();
    QModelIndex index = ui->table_nv_add->indexAt(QPoint(x, y));
    int row = index.row();  // 获得QTableWidget列表点击的行数
    QMessageBox box;
    box.setText(QString::number(row));
    // box.exec();
  }
}

void MainWindow::on_nv1() { mymethod->set_nv_key("boot-args", "String"); }

void MainWindow::on_nv2() { mymethod->set_nv_key("bootercfg", "Data"); }

void MainWindow::on_nv3() { mymethod->set_nv_key("bootercfg-once", "Data"); }

void MainWindow::on_nv4() {
  mymethod->set_nv_key("efiboot-perf-record", "Data");
}

void MainWindow::on_nv5() {
  mymethod->set_nv_key("fmm-computer-name", "String");
}

void MainWindow::on_nv6() { mymethod->set_nv_key("nvda_drv", "String"); }

void MainWindow::on_nv7() { mymethod->set_nv_key("run-efi-updater", "String"); }

void MainWindow::on_nv8() { mymethod->set_nv_key("StartupMute", "Data"); }

void MainWindow::on_nv9() { mymethod->set_nv_key("SystemAudioVolume", "Data"); }

void MainWindow::on_nv10() {
  mymethod->set_nv_key("csr-active-config", "Data");
}

void MainWindow::on_nv11() { mymethod->set_nv_key("prev-lang:kbd", "Data"); }

void MainWindow::on_nv12() { mymethod->set_nv_key("security-mode", "Data"); }

void MainWindow::show_menu0(const QPoint pos) {
  // 设置菜单选项
  QMenu* menu = new QMenu(ui->table_nv_add0);

  QAction* act1 =
      new QAction("+  4D1EDE05-38C7-4A6A-9CC6-4BCCA8B38C14", ui->table_nv_add0);
  connect(act1, SIGNAL(triggered()), this, SLOT(on_nv01()));

  QAction* act2 =
      new QAction("+  7C436110-AB2A-4BBB-A880-FE41995C9F82", ui->table_nv_add0);
  connect(act2, SIGNAL(triggered()), this, SLOT(on_nv02()));

  QAction* act3 =
      new QAction("+  8BE4DF61-93CA-11D2-AA0D-00E098032B8C", ui->table_nv_add0);
  connect(act3, SIGNAL(triggered()), this, SLOT(on_nv03()));

  QAction* act4 =
      new QAction("+  4D1FDA02-38C7-4A6A-9CC6-4BCCA8B30102", ui->table_nv_add0);
  connect(act4, SIGNAL(triggered()), this, SLOT(on_nv04()));

  menu->addAction(act1);
  menu->addAction(act2);
  menu->addAction(act3);
  menu->addAction(act4);

  menu->move(cursor().pos());
  menu->show();
  // 获得鼠标点击的x，y坐标点
  int x = pos.x();
  int y = pos.y();
  QModelIndex index = ui->table_nv_add0->indexAt(QPoint(x, y));
  int row = index.row();  // 获得QTableWidget列表点击的行数
  QMessageBox box;
  box.setText(QString::number(row));
  // box.exec();
}

void MainWindow::on_nv01() {
  bool re = false;

  for (int i = 0; i < ui->table_nv_add0->rowCount(); i++) {
    QString str;
    str = ui->table_nv_add0->item(i, 0)->text();
    if (str == "4D1EDE05-38C7-4A6A-9CC6-4BCCA8B38C14") {
      ui->table_nv_add0->setCurrentCell(i, 0);
      on_table_nv_add0_cellClicked(i, 0);
      re = true;
    }
  }

  if (!re) {
    on_btnNVRAMAdd_Add0_clicked();

    ui->table_nv_add0->setItem(
        ui->table_nv_add0->rowCount() - 1, 0,
        new QTableWidgetItem("4D1EDE05-38C7-4A6A-9CC6-4BCCA8B38C14"));
  }
}

void MainWindow::on_nv02() {
  bool re = false;

  for (int i = 0; i < ui->table_nv_add0->rowCount(); i++) {
    QString str;
    str = ui->table_nv_add0->item(i, 0)->text();
    if (str == "7C436110-AB2A-4BBB-A880-FE41995C9F82") {
      ui->table_nv_add0->setCurrentCell(i, 0);
      on_table_nv_add0_cellClicked(i, 0);
      re = true;
    }
  }

  if (!re) {
    on_btnNVRAMAdd_Add0_clicked();

    ui->table_nv_add0->setItem(
        ui->table_nv_add0->rowCount() - 1, 0,
        new QTableWidgetItem("7C436110-AB2A-4BBB-A880-FE41995C9F82"));
  }
}

void MainWindow::on_nv03() {
  bool re = false;

  for (int i = 0; i < ui->table_nv_add0->rowCount(); i++) {
    QString str;
    str = ui->table_nv_add0->item(i, 0)->text();
    if (str == "8BE4DF61-93CA-11D2-AA0D-00E098032B8C") {
      ui->table_nv_add0->setCurrentCell(i, 0);
      on_table_nv_add0_cellClicked(i, 0);
      re = true;
    }
  }

  if (!re) {
    on_btnNVRAMAdd_Add0_clicked();

    ui->table_nv_add0->setItem(
        ui->table_nv_add0->rowCount() - 1, 0,
        new QTableWidgetItem("8BE4DF61-93CA-11D2-AA0D-00E098032B8C"));
  }
}

void MainWindow::on_nv04() {
  bool re = false;

  for (int i = 0; i < ui->table_nv_add0->rowCount(); i++) {
    QString str;
    str = ui->table_nv_add0->item(i, 0)->text();
    if (str == "4D1FDA02-38C7-4A6A-9CC6-4BCCA8B30102") {
      ui->table_nv_add0->setCurrentCell(i, 0);
      on_table_nv_add0_cellClicked(i, 0);
      re = true;
    }
  }

  if (!re) {
    on_btnNVRAMAdd_Add0_clicked();

    ui->table_nv_add0->setItem(
        ui->table_nv_add0->rowCount() - 1, 0,
        new QTableWidgetItem("4D1FDA02-38C7-4A6A-9CC6-4BCCA8B30102"));
  }
}

void MainWindow::init_hardware_info() {
  ui->btnGenerateFromHardware->setEnabled(false);
  ui->tabTotal->removeTab(9);

  if (win) {
    ui->listHardwareInfo->addItem(tr("CpuName") + "  :  " + getCpuName());
    ui->listHardwareInfo->addItem(tr("CpuId") + "  :  " + getCpuId());
    ui->listHardwareInfo->addItem(tr("CpuCoresNum") + "  :  " +
                                  getCpuCoresNum());
    ui->listHardwareInfo->addItem(tr("CpuCpuLogicalProcessorsNum") + "  :  " +
                                  getCpuLogicalProcessorsNum());

    ui->listHardwareInfo->addItem("");

    ui->listHardwareInfo->addItem(tr("MainboardName") + "  :  " +
                                  getMainboardName());
    ui->listHardwareInfo->addItem(tr("BaseBordNum") + "  :  " +
                                  getBaseBordNum());
    ui->listHardwareInfo->addItem(tr("MainboardUUID") + "  :  " +
                                  getMainboardUUID());
    ui->listHardwareInfo->addItem(tr("BiosNum") + "  :  " + getBiosNum());
    ui->listHardwareInfo->addItem(tr("MainboardVendor") + "  :  " +
                                  getMainboardVendor());

    ui->listHardwareInfo->addItem("");

    ui->listHardwareInfo->addItem(tr("DiskNum") + "  :  " + getDiskNum());

    ui->listHardwareInfo->addItem("");

    ui->listHardwareInfo->addItem(tr("Physical Memory") + "  :  " +
                                  getWMIC("wmic memorychip get Capacity"));
  }

  if (mac) {
    ui->tabTotal->removeTab(8);
    ui->listHardwareInfo->addItem(tr("CPU") + "  :  \n" +
                                  getMacInfo("sysctl machdep.cpu"));
  }

  if (linuxOS) {
    ui->tabTotal->removeTab(8);
  }
}

void MainWindow::setListMainIcon() {
  ui->listMain->setViewMode(QListWidget::IconMode);

  ui->listMain->clear();
  QStringList strItemList, strItemList1;

  strItemList.clear();
  strIconList << ":/icon/m1.png"
              << ":/icon/m2.png"
              << ":/icon/m3.png"
              << ":/icon/m4.png"
              << ":/icon/m5.png"
              << ":/icon/m6.png"
              << ":/icon/m7.png"
              << ":/icon/m8.png";

  strIconListSel.clear();
  strIconListSel << ":/icon/m1sel.png"
                 << ":/icon/m2sel.png"
                 << ":/icon/m3sel.png"
                 << ":/icon/m4sel.png"
                 << ":/icon/m5sel.png"
                 << ":/icon/m6sel.png"
                 << ":/icon/m7sel.png"
                 << ":/icon/m8sel.png";

  strItemList << tr("ACPI") << tr("Booter") << tr("DeviceProperties")
              << tr("Kernel") << tr("Misc") << tr("NVRAM") << tr("PlatformInfo")
              << tr("UEFI");

  strItemList1 << "ACPI"
               << "Booter"
               << "DP"
               << "Kernel"
               << "Misc"
               << "NVRAM"
               << "PI"
               << "UEFI";

  int iSize = 25;
  ui->listMain->setIconSize(QSize(iSize, iSize));
  ui->listMain->setFixedWidth(74);
  ui->listMain->setSpacing(4);
  QSize size(64, 45);
  for (int i = 0; i < strItemList.count(); i++) {
    ui->listMain->addItem(
        new QListWidgetItem(QIcon(strIconList.at(i)), strItemList1.at(i)));

    ui->listMain->item(i)->setSizeHint(size);

    ui->listMain->item(i)->setToolTip(strItemList.at(i));
  }

  /*if (win) {
      strIconList.append(":/icon/m9.png");
      strItemList.append(tr("Hardware Information"));
      ui->listMain->addItem(new QListWidgetItem(QIcon(":/icon/m9.png"),
  "Info")); ui->listMain->item(8)->setSizeHint(size);
      ui->listMain->item(8)->setToolTip(tr("Hardware Information"));
  }*/
}

void MainWindow::on_listMain_itemSelectionChanged() {
  int cu = ui->listMain->currentRow();
  ui->tabTotal->setCurrentIndex(cu);
  int index = 0;
  if (ui->listMain->currentRow() != 8)
    index = mainTabList.at(cu)->currentIndex();

  ui->listSub->clear();
  ui->listSub->setViewMode(QListWidget::IconMode);
  QStringList strList;

  if (ui->listMain->currentRow() == 0) {
    strList << tr("Add") << tr("Delete") << tr("Patch") << tr("Quirks");
  }

  if (ui->listMain->currentRow() == 1) {
    strList << tr("MmioWhitelist") << tr("Patch") << tr("Quirks");
  }

  if (ui->listMain->currentRow() == 2) {
    strList << tr("Add") << tr("Delete");
  }

  if (ui->listMain->currentRow() == 3) {
    strList << tr("Add") << tr("Block") << tr("Force") << tr("Patch")
            << tr("Emulate") << tr("Quirks") << tr("Scheme");
  }

  if (ui->listMain->currentRow() == 4) {
    strList << tr("Boot") << tr("Debug") << tr("Security")
            << tr("BlessOverride") << tr("Entries") << tr("Tools")
            << tr("Serial");
  }

  if (ui->listMain->currentRow() == 5) {
    strList << tr("Add") << tr("Delete") << tr("LegacySchema");
  }

  if (ui->listMain->currentRow() == 6) {
    strList << tr("Generic") << tr("DataHub") << tr("Memory")
            << tr("PlatformNVRAM") << tr("SMBIOS");

    if (mac || osx1012) strList.append(tr("SerialInfo"));
  }

  if (ui->listMain->currentRow() == 7) {
    strList.append(tr("APFS"));
    strList.append(tr("AppleInput"));
    strList.append(tr("Audio"));
    strList.append(tr("Drivers"));
    strList.append(tr("Input"));
    strList.append(tr("Output"));
    strList.append(tr("ProtocolOverrides"));
    strList.append(tr("Quirks"));
    strList.append(tr("ReservedMemory"));

    if (isUnload) {
      strList.append(tr("Unload"));
    } else {
      strList.removeOne("Unload");
    }
  }

  if (ui->listMain->currentRow() == 8) {
    index = 0;
    strList << tr("Hardware Information");
  }

  QStringList str;
  int w1 = 0;
  int w2 = 0;
  for (int i = 0; i < strList.count(); i++) {
    ui->listSub->addItem(strList.at(i));
    str = strList.at(i).split("\n");
    if (str.count() == 1)
      w1 = getTextWidth(ui->listSub->item(i)->text(), ui->listSub);
    if (str.count() == 2) {
      w1 = getTextWidth(str.at(0), ui->listSub);
      w2 = getTextWidth(str.at(1), ui->listSub);
      if (w1 < w2) w1 = w2;
    }
    ui->listSub->item(i)->setSizeHint(
        QSize(w1, ui->listSub->maximumHeight() - 12));
  }

  ui->listSub->setCurrentRow(index);
}

void MainWindow::init_listMainSub() {
  QFont myFont(this->font().family(), this->font().pixelSize());
  QFontMetrics fm(myFont);
  int fontHeight = fm.height() + 4;

  ui->listSub->setSpacing(4);
  ui->listSub->setFixedHeight(fontHeight * 2 + 8);

  ui->listMain->setViewMode(QListView::ListMode);
  ui->listSub->setViewMode(QListView::ListMode);
  ui->listMain->setMovement(QListView::Static);  // 禁止拖动
  ui->listSub->setMovement(QListView::Static);
  ui->listMain->setFocusPolicy(Qt::NoFocus);  // 去掉选中时的虚线
  ui->listSub->setFocusPolicy(Qt::NoFocus);

  setListMainIcon();

  ui->tabTotal->tabBar()->setHidden(true);

  ui->tabACPI->tabBar()->setVisible(false);
  ui->tabBooter->tabBar()->setHidden(true);
  ui->tabDP->tabBar()->setHidden(true);
  ui->tabKernel->tabBar()->setHidden(true);
  ui->tabMisc->tabBar()->setHidden(true);
  ui->tabPlatformInfo->tabBar()->setHidden(true);
  ui->tabNVRAM->tabBar()->setHidden(true);
  ui->tabUEFI->tabBar()->setHidden(true);

  ui->tabTotal->setCurrentIndex(0);
  ui->tabACPI->setCurrentIndex(0);
  ui->tabBooter->setCurrentIndex(0);
  ui->tabDP->setCurrentIndex(0);
  ui->tabKernel->setCurrentIndex(0);
  ui->tabMisc->setCurrentIndex(0);
  ui->tabNVRAM->setCurrentIndex(0);
  ui->tabPlatformInfo->setCurrentIndex(0);
  ui->tabUEFI->setCurrentIndex(0);
}

void MainWindow::init_ToolBarIcon() {
  int iSize;
  if (win || linuxOS)
    iSize = 20;
  else
    iSize = 23;
  ui->toolBar->setIconSize(QSize(iSize, iSize));

  if (red < 55) {
    btn0->setIcon(QIcon(":/icon/rp0.png"));
    ui->actionOpen_Directory->setIcon(QIcon(":/icon/opendir0.png"));
    if (!isWindowModified()) ui->actionSave->setIcon(QIcon(":/icon/save0.png"));
    if (!blOCValidateError)
      ui->actionOcvalidate->setIcon(QIcon(":/icon/ov0.png"));
    ui->actionMountEsp->setIcon(QIcon(":/icon/esp0.png"));
    ui->actionUpgrade_OC->setIcon(QIcon(":/icon/um0.png"));
    ui->actionDatabase->setIcon(QIcon(":/icon/db0.png"));
    undoAction->setIcon(QIcon(":/icon/undo0.png"));
    redoAction->setIcon(QIcon(":/icon/redo0.png"));
    ui->actionFind->setIcon(QIcon(":/icon/find0.png"));
    ui->actionBackup_EFI->setIcon(QIcon(":/icon/be0.png"));
    btnBak->setIcon(QIcon(":/icon/be0.png"));
    btnClear->setIcon(QIcon(":/icon/c0.png"));
  } else {
    btn0->setIcon(QIcon(":/icon/rp.png"));
    ui->actionOpen_Directory->setIcon(QIcon(":/icon/opendir.png"));
    if (!isWindowModified()) ui->actionSave->setIcon(QIcon(":/icon/save.png"));
    if (!blOCValidateError)
      ui->actionOcvalidate->setIcon(QIcon(":/icon/ov.png"));

    ui->actionMountEsp->setIcon(QIcon(":/icon/esp.png"));
    ui->actionUpgrade_OC->setIcon(QIcon(":/icon/um.png"));
    ui->actionDatabase->setIcon(QIcon(":/icon/db.png"));
    undoAction->setIcon(QIcon(":/icon/undo.png"));
    redoAction->setIcon(QIcon(":/icon/redo.png"));
    ui->actionFind->setIcon(QIcon(":/icon/find.png"));
    ui->actionBackup_EFI->setIcon(QIcon(":/icon/be.png"));
    btnBak->setIcon(QIcon(":/icon/be.png"));
    btnClear->setIcon(QIcon(":/icon/c.png"));
  }

  QString listStyleMain, listStyle;
  if (red < 55)
    listStyleMain =

        "QListWidget::item:hover{background-color:rgba(190,190,190,50);"
        "margin:"
        "1px,1px,1px,"
        "1px;border-radius:6;"
        "color:#e6e6e6}"

        "QListWidget::item:selected{background:rgba(190,190,190,50); "
        "border:0px "
        "blue;margin:1px,1px,1px,1px;border-radius:6;"
        "color:#07a4f6}"

        "QListWidget::item{background-color:none;margin:1px,1px,1px,"
        "1px;"
        "color:#bfbfbf}";
  else
    listStyleMain =
        "QListWidget::item:hover{background-color:#e6e6e6;margin:1px,1px,1px,"
        "1px;border-radius:6;"
        "color:black}"
        "QListWidget::item:selected{background:#e6e6e6; border:0px "
        "blue;margin:1px,1px,1px,1px;border-radius:6;"
        "color:blue}";

  listStyle =
      "QListWidget::item:selected{background:lightblue; border:0px blue; "
      "color:black}";
  ui->listMain->setStyleSheet(listStyleMain);
  ui->listSub->setStyleSheet(listStyleMain);

  updateIconStatus();

  ui->toolBar->setFixedHeight(42);
}

void MainWindow::init_FileMenu() {
  ui->actionNew_Key_Field->setVisible(false);
  // New
  if (mac || osx1012) ui->actionNewWindow->setIconVisibleInMenu(false);

  // Open
  if (mac || osx1012) ui->actionOpen->setIconVisibleInMenu(false);
  connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::on_btnOpen);
  ui->actionOpen->setShortcut(tr("ctrl+o"));

  lblVer = new QLabel(this);
  lblVer->installEventFilter(this);
  QFont font;
  font.setBold(true);
  lblVer->setFont(font);
  ui->toolBar->addWidget(lblVer);

  QWidget* spacer = new QWidget(this);
  spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  ui->toolBar->addWidget(spacer);

  // Recent Open
  QCoreApplication::setOrganizationName("ic005k");
  QCoreApplication::setOrganizationDomain("github.com/ic005k");
  QCoreApplication::setApplicationName("OC Auxiliary Tools");
  m_recentFiles = new RecentFiles(this);
  // m_recentFiles->attachToMenuAfterItem(ui->menuFile, tr("Save As..."),
  //                                     SLOT(recentOpen(QString)));
  // m_recentFiles->setNumOfRecentFiles(10);
  reFileMenu = new QMenu(this);
  reFileMenu->setTitle(tr("Recently Open"));
  ui->menuFile->insertMenu(ui->actionOpen_Directory, reFileMenu);

  btn0 = new QToolButton(this);
  btn0->setToolTip(tr("Open Recent..."));
  btn0->setIcon(QIcon(":/icon/rp.png"));
  btn0->setPopupMode(QToolButton::InstantPopup);
  if (Reg.value("chkRecentOpen", 1).toBool() == true) {
    ui->toolBar->addWidget(btn0);
    btn0->setMenu(reFileMenu);
  } else {
    btn0->setVisible(false);
  }

#ifdef Q_OS_MAC
  reFileMenu->setAsDockMenu();
#endif

  // Open Dir
  if (mac || osx1012) ui->actionOpen_Directory->setIconVisibleInMenu(false);
  ui->actionOpen_Directory->setIcon(QIcon(":/icon/opendir.png"));
  if (Reg.value("chkOpenDir", 1).toBool() == true) {
    ui->toolBar->addAction(ui->actionOpen_Directory);
  }

  // Save
  if (mac || osx1012) ui->actionSave->setIconVisibleInMenu(false);
  connect(ui->actionSave, &QAction::triggered, this, &MainWindow::on_btnSave);
  ui->actionSave->setShortcut(tr("ctrl+s"));
  ui->actionSave->setIcon(QIcon(":/icon/save.png"));
  ui->toolBar->addAction(ui->actionSave);
  ui->actionSave->setEnabled(false);

  // SaveAs
  if (mac || osx1012) ui->actionSave_As->setIconVisibleInMenu(false);
  connect(ui->actionSave_As, &QAction::triggered, this,
          &MainWindow::on_btnSaveAs);
  ui->actionSave_As->setShortcut(tr("ctrl+shift+s"));

  // Preferences
  // ui->actionPreferences->setMenuRole(QAction::PreferencesRole);

  // Quit
  ui->actionQuit->setMenuRole(QAction::QuitRole);
}

void MainWindow::init_EditMenu() {
  ui->actionPlist_editor->setVisible(false);
  ui->actionDSDT_SSDT_editor->setVisible(false);

  // Edit
  ui->actionInitDatabaseLinux->setVisible(false);

  // Find
  if (mac || osx1012) ui->actionFind->setIconVisibleInMenu(false);
  ui->actionFind->setShortcut(tr("ctrl+f"));
  ui->actionFind->setIcon(QIcon(":/icon/find.png"));

  if (mac || osx1012) ui->actionGo_to_the_previous->setIconVisibleInMenu(false);
  ui->actionGo_to_the_previous->setShortcut(tr("ctrl+3"));

  if (mac || osx1012) ui->actionGo_to_the_next->setIconVisibleInMenu(false);
  ui->actionGo_to_the_next->setShortcut(tr("ctrl+4"));

  // OC Validate
  if (mac || osx1012) ui->actionOcvalidate->setIconVisibleInMenu(false);

  ui->actionOcvalidate->setShortcut(tr("ctrl+l"));
  ui->actionOcvalidate->setIcon(QIcon(":/icon/ov.png"));
  ui->toolBar->addAction(ui->actionOcvalidate);
  ui->toolBar->addAction(ui->actionUpgrade_OC);

  // MountESP
  if (mac || osx1012) ui->actionMountEsp->setIconVisibleInMenu(false);

  ui->actionMountEsp->setShortcut(tr("ctrl+m"));
  ui->actionMountEsp->setIcon(QIcon(":/icon/esp.png"));
  if (Reg.value("chkMountESP", 1).toBool() == true) {
    ui->toolBar->addAction(ui->actionMountEsp);
  }

  // GenerateEFI
  if (mac || osx1012) ui->actionGenerateEFI->setIconVisibleInMenu(false);

  // Update OC Main Program
  if (mac || osx1012) ui->actionUpgrade_OC->setIconVisibleInMenu(false);

  ui->actionUpgrade_OC->setIcon(QIcon(":/icon/um.png"));
  // ui->actionUpgrade_OC->setEnabled(false);

  // Open DataBase
  if (mac || osx1012) ui->actionDatabase->setIconVisibleInMenu(false);
  ui->actionDatabase->setShortcut(tr("ctrl+d"));
  ui->actionDatabase->setIcon(QIcon(":/icon/db.png"));

  // Open DataBase Dir
  if (mac || osx1012)
    ui->actionOpen_database_directory->setIconVisibleInMenu(false);

  // Move Up(Down)
  ui->actionMove_Up->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Up));
  ui->actionMove_Down->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Down));

  // Backup EFI
  if (mac || osx1012) ui->actionBackup_EFI->setIconVisibleInMenu(false);
  ui->actionBackup_EFI->setIcon(QIcon(":/icon/be.png"));

  btnBak = new QToolButton(this);
  btnBak->setIcon(QIcon(":/icon/be.png"));
  btnBak->setToolTip(ui->actionBackup_EFI->text());

  QMenu* bakMenu = new QMenu(this);
  QAction* actOpenBakDir = new QAction(tr("Open backup directory"), this);
  bakMenu->addAction(actOpenBakDir);
  btnBak->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(btnBak, &QPushButton::customContextMenuRequested,
          [=](const QPoint& pos) {
            Q_UNUSED(pos);
            bakMenu->exec(QCursor::pos());
          });

  connect(actOpenBakDir, &QAction::triggered, [=]() {
    QString dirpath = QDir::homePath() + "/Desktop/Backup EFI/";
    QString dir = "file:" + dirpath;
    QDesktopServices::openUrl(QUrl(dir, QUrl::TolerantMode));
  });
  connect(btnBak, &QToolButton::clicked,
          [=]() { on_actionBackup_EFI_triggered(); });

  if (Reg.value("chkBackupEFI", 1).toBool() == true) {
    ui->toolBar->addWidget(btnBak);
  } else {
    btnBak->setVisible(false);
  }

  if (Reg.value("chkDatabase", 1).toBool() == true) {
    ui->toolBar->addAction(ui->actionDatabase);
  }
}

void MainWindow::init_HelpMenu() {
  // Help
  if (zh_cn)
    ui->actionOpenCoreChineseDoc->setVisible(true);
  else
    ui->actionOpenCoreChineseDoc->setVisible(false);

  connect(ui->btnHelp, &QAction::triggered, this, &MainWindow::on_btnHelp);
  ui->btnHelp->setShortcut(tr("ctrl+p"));

  connect(ui->btnCheckUpdate, &QAction::triggered, this,
          &MainWindow::on_btnCheckUpdate);
  ui->btnCheckUpdate->setShortcut(tr("ctrl+u"));

  connect(ui->actionAbout_2, &QAction::triggered, this, &MainWindow::about);
  ui->actionAbout_2->setMenuRole(QAction::AboutRole);

  connect(ui->actionOpenCore, &QAction::triggered, this, &MainWindow::on_line1);
  connect(ui->actionOpenCore_Factory, &QAction::triggered, this,
          &MainWindow::on_line2);

  if (mac || osx1012) ui->actionOpenCore_Factory->setIconVisibleInMenu(false);

  if (mac || osx1012) ui->btnCheckUpdate->setIconVisibleInMenu(false);

  if (mac || osx1012) ui->btnHelp->setIconVisibleInMenu(false);

  // Bug Report
  if (mac || osx1012) ui->actionBug_Report->setIconVisibleInMenu(false);
  ui->actionBug_Report->setIcon(QIcon(":/icon/about.png"));

  // Auto check update
  manager = new QNetworkAccessManager(this);
  connect(manager, SIGNAL(finished(QNetworkReply*)), this,
          SLOT(replyFinished(QNetworkReply*)));
  autoCheckUpdate = true;
  on_btnCheckUpdate();
}

void MainWindow::init_UndoRedo() {
  // Undo/Redo
  undoStack = new QUndoStack(this);

  undoView = new QUndoView(undoStack);
  undoView->setWindowTitle(tr("Command List"));
  // undoView->show();
  undoView->setAttribute(Qt::WA_QuitOnClose, false);

  undoAction = undoStack->createUndoAction(this, tr("Undo"));
  undoAction->setShortcuts(QKeySequence::Undo);
  if (mac || osx1012) undoAction->setIconVisibleInMenu(false);

  redoAction = undoStack->createRedoAction(this, tr("Redo"));
  redoAction->setShortcuts(QKeySequence::Redo);
  if (mac || osx1012) redoAction->setIconVisibleInMenu(false);

  ui->menuTools->addSeparator();
  ui->menuTools->addAction(undoAction);
  ui->menuTools->addAction(redoAction);

  // Undo
  // undoAction->setShortcut(tr("ctrl+1"));
  undoAction->setIcon(QIcon(":/icon/undo.png"));
  ui->toolBar->addAction(undoAction);

  // Redo
  // redoAction->setShortcut(tr("ctrl+2"));
  redoAction->setIcon(QIcon(":/icon/redo.png"));
  ui->toolBar->addAction(redoAction);
}

void MainWindow::init_ToolButtonStyle() {
  QObjectList list, list1, list2;
  list = mymethod->getAllToolButton(getAllUIControls(ui->tabTotal));
  list1 = mymethod->getAllToolButton(
      getAllUIControls(myDlgPreference->ui->tabWidget));
  list2 = mymethod->getAllToolButton(getAllUIControls(dlgSyncOC));
  QString strStyle;
  strStyle =
      "QToolButton:hover{ "
      "color:rgb(0, 0, 255); "
      "border-style:solid; "
      "border-top-left-radius:2px;  "
      "border-top-right-radius:2px; "
      "background:#bfbfbf; "
      "border:1px;"
      "border-radius:5px;padding:2px 4px; }"

      "QToolButton:pressed{ "
      "color:rgb(255, 255, 255); "
      "border-style:solid; "
      "border-top-left-radius:2px;  "
      "border-top-right-radius:2px; "
      "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop:0 "
      "rgb(226,236,241),"
      "stop: 0.3 rgb(190,190,190),"
      "stop: 1 rgb(160,160,160));"
      "border:1px;"
      "border-radius:5px;padding:2px 4px; }";

  for (int i = 0; i < list.count(); i++) {
    QToolButton* w = (QToolButton*)list.at(i);
    w->setStyleSheet(strStyle);
  }
  for (int i = 0; i < list1.count(); i++) {
    QToolButton* w = (QToolButton*)list1.at(i);
    w->setStyleSheet(strStyle);
  }
  for (int i = 0; i < list2.count(); i++) {
    QToolButton* w = (QToolButton*)list2.at(i);
    w->setStyleSheet(strStyle);
  }
}

void MainWindow::init_ToolBar() {
  // Hide ToolBar

  ui->frameToolBar->setHidden(true);
  ui->statusbar->setHidden(true);
  myDlgPreference->ui->chkHideToolbar->setChecked(
      Reg.value("chkHideToolbar", 0).toBool());
  if (myDlgPreference->ui->chkHideToolbar->isChecked()) {
    ui->toolBar->setHidden(true);
    ui->frameToolBar->setFixedHeight(ui->mycboxFind->height() + 4);

  } else {
    ui->toolBar->addWidget(ui->mycboxFind);
    ui->toolBar->addAction(ui->actionFind);
    ui->mycboxFind->setMaximumWidth(320);
  }
}

void MainWindow::init_SearchUI() {
  // Search
  ui->mycboxFind->setAttribute(Qt::WA_MacShowFocusRect, false);
  Method::setComboBoxQss(ui->mycboxFind, 6, 1, "#C0C0C0", "#4169E1");

  ui->mycboxFind->lineEdit()->setClearButtonEnabled(false);
  ui->mycboxFind->lineEdit()->setPlaceholderText(tr("Search"));
  connect(ui->mycboxFind->lineEdit(), &QLineEdit::returnPressed, this,
          &MainWindow::on_actionFind_triggered);

  ui->listFind->setAlternatingRowColors(true);

  clearTextsAction = new QAction(this);
  clearTextsAction->setToolTip(tr("Clear search history"));
  clearTextsAction->setIcon(QIcon(":/icon/clear.png"));
  ui->mycboxFind->lineEdit()->addAction(clearTextsAction,
                                        QLineEdit::LeadingPosition);
  connect(clearTextsAction, SIGNAL(triggered()), this, SLOT(clearFindTexts()));

  QSize size = QSize(85, ui->mycboxFind->lineEdit()->sizeHint().height());
  ui->lblCount->setMinimumSize(size);
  ui->lblCount->setMaximumSize(size);
  QHBoxLayout* buttonLayout = new QHBoxLayout();
  QMargins mar(0, 1, 1, 1);
  buttonLayout->setContentsMargins(mar);
  buttonLayout->addStretch();
  buttonLayout->addWidget(ui->lblCount);

  btnClear = new QToolButton(this);
  int b_w = ui->mycboxFind->lineEdit()->height() - 2;
  btnClear->setMaximumWidth(b_w);
  btnClear->setMaximumHeight(b_w);
  btnClear->setStyleSheet("QToolButton{border:none;}");
  btnClear->setIcon(QIcon(":/icon/c.png"));
  btnClear->setCursor(QCursor(Qt::ArrowCursor));
  buttonLayout->addWidget(btnClear);

  ui->lblCount->setAlignment(Qt::AlignRight);
  ui->mycboxFind->lineEdit()->setLayout(buttonLayout);
  // 设置输入区，不让输入的文字被隐藏在lblCount下
  ui->mycboxFind->lineEdit()->setTextMargins(0, 1, size.width(), 1);
  connect(btnClear, &QToolButton::clicked, [=]() {
    ui->mycboxFind->lineEdit()->clear();
    init_ToolBarIcon();
  });

  int textTotal = Reg.value("textTotal").toInt();
  for (int i = 0; i < textTotal; i++) {
    ui->mycboxFind->addItem(Reg.value(QString::number(i)).toString());
  }

  ui->mycboxFind->setCurrentText("");
  if (textTotal > 0)
    clearTextsAction->setEnabled(true);
  else
    clearTextsAction->setEnabled(false);

  ui->listFind->setHidden(true);
}

void MainWindow::init_MainUI() {
  init_FileMenu();
  init_EditMenu();
  init_UndoRedo();
  init_ToolBar();
  init_HelpMenu();
  init_SearchUI();
  init_LineEditDataCheck();
  init_CopyLabel();
  init_CopyCheckbox();
  copyText(ui->listFind);
  copyText(ui->listMain);
  copyText(ui->listSub);
  init_InitialValue();
  init_TableStyle();
  init_ToolBarIcon();
  Method::init_UIWidget(this, red);

  orgComboBoxStyle = ui->cboxKernelArch->styleSheet();
  orgLineEditStyle = ui->editBID->styleSheet();
  orgLabelStyle = ui->label->styleSheet();
  orgCheckBoxStyle = ui->chkFadtEnableReset->styleSheet();

  ui->frameTip->setAutoFillBackground(true);
  ui->frameTip->setPalette(QPalette(QColor(255, 204, 204)));
  ui->btnNo->setDefault(true);
  ui->frameTip->setHidden(true);

  ui->actionOpenCore_DEV->setChecked(blDEV);
  ui->actionDEBUG->setChecked(Reg.value("DEBUG", 0).toBool());
  if (ui->actionDEBUG->isChecked()) {
    if (!blDEV) {
      if (!QFile(QDir::homePath() + "/.ocat/Database/DEBUG/EFI/OC/OpenCore.efi")
               .exists())
        ui->actionDEBUG->setChecked(false);
    } else if (!QFile(QDir::homePath() +
                      "/.ocat/devDatabase/DEBUG/EFI/OC/OpenCore.efi")
                    .exists())
      ui->actionDEBUG->setChecked(false);
  }
  on_actionOpenCore_DEV_triggered();

  // Resize Windows
  int x, y, w, h;
  x = Reg.value("x", "0").toInt();
  y = Reg.value("y", "0").toInt();
  w = Reg.value("width", "900").toInt();
  h = Reg.value("height", "500").toInt();
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

  ui->txtEditHex->setPlaceholderText(tr("Hexadecimal"));
  ui->txtEditASCII->setPlaceholderText(tr("ASCII"));
  ui->btnUpdateHex->setFixedHeight(ui->txtEditHex->height() - 12);
  ui->btnUpdateHex->setFixedWidth(ui->btnUpdateHex->height());
  setConversionWidgetVisible(false);

  ui->btnNVRAMAdd_Add0->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(ui->btnNVRAMAdd_Add0, SIGNAL(customContextMenuRequested(QPoint)),
          this, SLOT(show_menu0(QPoint)));
  ui->btnNVRAMAdd_Add->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(ui->btnNVRAMAdd_Add, SIGNAL(customContextMenuRequested(QPoint)), this,
          SLOT(show_menu(QPoint)));

  // Read and add new key fields
  QStringList list = dlgNewKeyField::getAllNewKey();
  for (int i = 0; i < list.count(); i++) {
    int m, s;
    QString Key = list.at(i);
    QString KeyBak = Key;
    QString ObjectName = KeyBak.replace("key", "");
    m = dlgNewKeyField::getKeyMainSub(Key).at(0);
    s = dlgNewKeyField::getKeyMainSub(Key).at(1);

    QWidget* tab = getSubTabWidget(m, s);
    bool re = false;
    if (Key.mid(0, 6) == "keychk") {
      QObjectList listOfCheckBox = getAllCheckBox(getAllUIControls(tab));
      for (int i = 0; i < listOfCheckBox.count(); i++) {
        QCheckBox* w = (QCheckBox*)listOfCheckBox.at(i);
        if (w->objectName() == ObjectName) re = true;
      }
    }
    if (Key.mid(0, 7) == "keyedit") {
      QObjectList listOfEdit = getAllLineEdit(getAllUIControls(tab));
      for (int i = 0; i < listOfEdit.count(); i++) {
        QLineEdit* w = (QLineEdit*)listOfEdit.at(i);
        if (w->objectName() == ObjectName) re = true;
      }
    }
    if (!re) dlgNewKeyField::readNewKey(tab, Key);
  }
}

void MainWindow::init_Widgets() {
  QPalette pal = this->palette();
  QBrush brush = pal.window();
  red = brush.color().red();
  this->setAcceptDrops(true);
#ifdef Q_OS_MAC
#if (QT_VERSION < QT_VERSION_CHECK(5, 15, 0))
  osx1012 = true;
#endif
  if (osx1012)
    mac = false;
  else
    mac = true;
#endif

#ifdef Q_OS_WIN32
  reg_win();
  win = true;
#endif

#ifdef Q_OS_LINUX
  ui->actionMountEsp->setEnabled(false);
  linuxOS = true;
#endif
  strAppExePath = qApp->applicationDirPath();
  strConfigPath = QDir::homePath() + "/.config/" + strAppName + "/";

  QString y = QString::number(QDate::currentDate().year());
  QString m = QString::number(QDate::currentDate().month());
  QString d = QString::number(QDate::currentDate().day());
  QString h = QString::number(QTime::currentTime().hour());
  QString mm = QString::number(QTime::currentTime().minute());
  QString s = QString::number(QTime::currentTime().second());
  CurrentDateTime = y + m + d + h + mm + s;

  blDEV = Reg.value("OpenCoreDEV", false).toBool();
  if (!QFile(QDir::homePath() + "/.ocat/devDatabase/EFI/OC/OpenCore.efi")
           .exists()) {
    blDEV = false;
  }

  QString fileSample, fileSampleDev;
  fileSample =
      QDir::homePath() + "/.ocat/Database/BaseConfigs/SampleCustom.plist";
  fileSampleDev =
      QDir::homePath() + "/.ocat/devDatabase/BaseConfigs/SampleCustom.plist";
  QFile file(fileSample);
  QFile fileDev(fileSampleDev);
  if (blDEV) {
    if (fileDev.exists())
      mapTatol = PListParser::parsePList(&fileDev).toMap();
    else
      mapTatol = PListParser::parsePList(&file).toMap();
  } else {
    mapTatol = PListParser::parsePList(&file).toMap();
  }

  mymethod = new Method(this);
  aboutDlg = new aboutDialog(this);
  myDatabase = new dlgDatabase(this);
  myToolTip = new Tooltip(this);
  dlgOCV = new dlgOCValidate(this);
  dlgPar = new dlgParameters(this);
  dlgAutoUpdate = new AutoUpdateDialog(this);
  dlgSyncOC = new SyncOCDialog(this);
  dlgPresetValues = new dlgPreset(this);
  dlgMiscBootArgs = new dlgMisc(this);
  myDlgKernelPatch = new dlgKernelPatch(this);
  myDlgPreference = new dlgPreference(this);
  myDlgNewKeyField = new dlgNewKeyField(this);

  timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(updateStatus()));
}

void MainWindow::init_TableStyle() {
  listOfTableWidget.clear();
  listOfTableWidget = getAllTableWidget(getAllUIControls(ui->tabTotal));
  for (int i = 0; i < listOfTableWidget.count(); i++) {
    QTableWidget* t;
    t = (QTableWidget*)listOfTableWidget.at(i);
    QString strStyle =
        "QTableWidget{outline:0px;}"
        "QTableWidget::item::selected{color : white;background : rgb(0, 124, "
        "221);}";

    t->setStyleSheet(strStyle);
    t->setCornerButtonEnabled(true);
    t->setFrameShape(QFrame::NoFrame);
  }
}

void MainWindow::init_InitialValue() {
  tableList0.append(ui->table_dp_add0);
  tableList0.append(ui->table_dp_del0);
  tableList0.append(ui->table_nv_add0);
  tableList0.append(ui->table_nv_del0);
  tableList0.append(ui->table_nv_ls0);

  tableList.append(ui->table_dp_add);
  tableList.append(ui->table_dp_del);
  tableList.append(ui->table_nv_add);
  tableList.append(ui->table_nv_del);
  tableList.append(ui->table_nv_ls);

  mainTabList.clear();
  mainTabList.append(ui->tabACPI);
  mainTabList.append(ui->tabBooter);
  mainTabList.append(ui->tabDP);
  mainTabList.append(ui->tabKernel);
  mainTabList.append(ui->tabMisc);
  mainTabList.append(ui->tabNVRAM);
  mainTabList.append(ui->tabPlatformInfo);
  mainTabList.append(ui->tabUEFI);

  chk_Target.clear();
  chk_Target.append(ui->mychkT1);
  chk_Target.append(ui->mychkT2);
  chk_Target.append(ui->mychkT3);
  chk_Target.append(ui->mychkT4);
  chk_Target.append(ui->mychkT5);
  chk_Target.append(ui->mychkT6);
  chk_Target.append(ui->mychkT7);
  for (int i = 0; i < chk_Target.count(); i++) {
    connect(chk_Target.at(i), &QCheckBox::clicked, this, &MainWindow::Target);
  }
}

void MainWindow::init_LineEditDataCheck() {
  // LineEdit data check
  listOfLineEdit.clear();
  listOfLineEdit = getAllLineEdit(getAllUIControls(ui->tabTotal));
  for (int i = 0; i < listOfLineEdit.count(); i++) {
    QLineEdit* w = (QLineEdit*)listOfLineEdit.at(i);

    if (w->objectName().mid(0, 7) == "editInt") {
      QValidator* validator = new QRegularExpressionValidator(regxNumber, w);
      w->setValidator(validator);
      w->setPlaceholderText(tr("Integer"));
    }

    if (w->objectName().mid(0, 7) == "editDat") {
      QValidator* validator = new QRegularExpressionValidator(regxData, w);
      w->setValidator(validator);
      w->setPlaceholderText(tr("Hexadecimal"));

      connect(w, &QLineEdit::textChanged, [=]() {
        w->setToolTip(QString::number(w->text().replace(" ", "").length() / 2) +
                      " Bytes");
      });
    }
  }
}

void MainWindow::init_CopyCheckbox() {
  // Copy CheckBox
  listOfCheckBox.clear();
  listOfCheckBox = getAllCheckBox(getAllUIControls(ui->tabTotal));
  for (int i = 0; i < listOfCheckBox.count(); i++) {
    QCheckBox* w = (QCheckBox*)listOfCheckBox.at(i);
    listUICheckBox.append(w);
    init_CheckBox(w);
  }
}

void MainWindow::init_CheckBox(QCheckBox* w) {
  setCheckBoxWidth(w);
  w->setContextMenuPolicy(Qt::CustomContextMenu);
  w->installEventFilter(this);
  mymethod->setToolTip(w, w->text());
  QAction* copyAction = new QAction(tr("CopyText") + "  " + w->text());
  QAction* showTipsAction = new QAction(tr("Show Tips"));
  QMenu* copyMenu = new QMenu(this);
  copyMenu->addAction(copyAction);
  copyMenu->addAction(showTipsAction);
  connect(copyAction, &QAction::triggered, [=]() {
    QString str = copyAction->text().trimmed();
    QString str1 = str.replace(tr("CopyText"), "");
    QClipboard* clipboard = QApplication::clipboard();
    clipboard->setText(str1.trimmed());
  });
  connect(showTipsAction, &QAction::triggered, [=]() {
    QString str = copyAction->text().trimmed();
    QString str1 = str.replace(tr("CopyText"), "").trimmed();
    myToolTip->popup(QCursor::pos(), str1 + "\n\n", w->toolTip());
  });
  mymethod->setStatusBarTip(w);
  connect(w, &QCheckBox::customContextMenuRequested, [=](const QPoint& pos) {
    Q_UNUSED(pos);
    if (w->toolTip().trimmed() == "")
      showTipsAction->setVisible(true);
    else
      showTipsAction->setVisible(true);
    copyMenu->exec(QCursor::pos());
  });
}

void MainWindow::init_CopyLabel() {
  // Copy Label
  listOfLabel.clear();
  listOfLabel = getAllLabel(getAllUIControls(ui->tabTotal));
  for (int i = 0; i < listOfLabel.count(); i++) {
    QLabel* w = (QLabel*)listOfLabel.at(i);
    listUILabel.append(w);
    w->setFixedWidth(getTextWidth(w->text(), w));

    if (w == ui->lblPickerAttributes || w == ui->lblDisplayLevel ||
        w == ui->lblExposeSensitiveData || w == ui->lblHaltLevel ||
        w == ui->lblScanPolicy || w == ui->lblTargetHex) {
      w->setTextInteractionFlags(Qt::TextSelectableByMouse);
    } else {
      init_Label(w);
    }
  }
}

void MainWindow::init_Label(QLabel* w) {
  w->setContextMenuPolicy(Qt::CustomContextMenu);
  w->installEventFilter(this);
  mymethod->setToolTip(w, w->text());
  QAction* copyAction = new QAction(tr("CopyText") + "  " + w->text());
  QAction* showTipsAction = new QAction(tr("Show Tips"));
  QMenu* copyMenu = new QMenu(this);
  copyMenu->addAction(copyAction);
  copyMenu->addAction(showTipsAction);
  connect(copyAction, &QAction::triggered, [=]() {
    QString str = copyAction->text().trimmed();
    QString str1 = str.replace(tr("CopyText"), "");
    QClipboard* clipboard = QApplication::clipboard();
    clipboard->setText(str1.trimmed());
  });
  connect(showTipsAction, &QAction::triggered, [=]() {
    QString str = copyAction->text().trimmed();
    QString str1 = str.replace(tr("CopyText"), "").trimmed();
    myToolTip->popup(QCursor::pos(), str1 + "\n\n", w->toolTip());
  });
  mymethod->setStatusBarTip(w);
  connect(w, &QLabel::customContextMenuRequested, [=](const QPoint& pos) {
    Q_UNUSED(pos);
    if (w->toolTip().trimmed() == "")
      showTipsAction->setVisible(false);
    else
      showTipsAction->setVisible(true);
    copyMenu->exec(QCursor::pos());
  });
}

void MainWindow::setCheckBoxWidth(QCheckBox* cbox) {
  QFont myFont(cbox->font().family(), cbox->font().pixelSize());
  QString str;
  str = cbox->text() + "              ";
  QFontMetrics fm(myFont);
  int w;

#if (QT_VERSION <= QT_VERSION_CHECK(5, 9, 9))
  w = fm.width(str);
#endif

#if (QT_VERSION >= QT_VERSION_CHECK(5, 15, 0))
  w = fm.horizontalAdvance(str);
#endif

  cbox->setMaximumWidth(w);
}

int MainWindow::getMainHeight() { return this->height(); }

int MainWindow::getMainWidth() { return this->width(); }

void MainWindow::copyText(QListWidget* listW) {
  listW->setContextMenuPolicy(Qt::CustomContextMenu);
  QAction* copyAction3 = new QAction(tr("CopyText"));
  QMenu* copyMenu3 = new QMenu(this);
  copyMenu3->addAction(copyAction3);
  connect(copyAction3, &QAction::triggered, [=]() {
    QString str = listW->item(listW->currentRow())->text().trimmed();

    QClipboard* clipboard = QApplication::clipboard();
    clipboard->setText(str);
  });

  connect(listW, &QListWidget::customContextMenuRequested,
          [=](const QPoint& pos) {
            Q_UNUSED(pos);
            if (listW->count() > 0) copyMenu3->exec(QCursor::pos());
          });
}

void MainWindow::clearFindTexts() {
  for (int i = 0; i < ui->mycboxFind->count(); i++) {
    Reg.remove(QString::number(i));
  }
  ui->mycboxFind->clear();
  clearTextsAction->setEnabled(false);
}

QString MainWindow::getDatabaseVer() {
  // Read database version information
  QFileInfo appInfo(qApp->applicationDirPath());
  QString strLastModify =
      QFileInfo(appInfo.filePath() + "/Database/EFI/OC/OpenCore.efi")
          .lastModified()
          .toString();

  QString DatabaseVer;
  if (!blDEV)
    DatabaseVer = ocVer + "    " + strLastModify;
  else
    DatabaseVer = ocVerDev + "    " + strLastModify;

  return DatabaseVer;
}

void MainWindow::on_line1() {
  QUrl url(QString("https://github.com/acidanthera/OpenCorePkg/releases"));
  QDesktopServices::openUrl(url);
}

void MainWindow::on_line2() {
  QUrl url(QString("https://github.com/acidanthera/OpenCorePkg/actions"));
  QDesktopServices::openUrl(url);
}

void MainWindow::on_table_dp_add0_itemChanged(QTableWidgetItem* item) {
  Q_UNUSED(item);

  if (writeINI) {
    mymethod->writeLeftTable(ui->table_dp_add0, ui->table_dp_add);
    this->setWindowModified(true);
    updateIconStatus();
  }
}

void MainWindow::on_table_dp_del0_itemChanged(QTableWidgetItem* item) {
  Q_UNUSED(item);

  if (writeINI) {
    write_value_ini(ui->table_dp_del0, ui->table_dp_del,
                    ui->table_dp_del0->currentRow());
    this->setWindowModified(true);
    updateIconStatus();
  }
}

void MainWindow::on_table_nv_add0_itemChanged(QTableWidgetItem* item) {
  Q_UNUSED(item);

  if (writeINI) {
    mymethod->writeLeftTable(ui->table_nv_add0, ui->table_nv_add);
    this->setWindowModified(true);
    updateIconStatus();
  }
}

void MainWindow::on_table_nv_del0_itemChanged(QTableWidgetItem* item) {
  Q_UNUSED(item);

  if (writeINI) {
    write_value_ini(ui->table_nv_del0, ui->table_nv_del,
                    ui->table_nv_del0->currentRow());
    this->setWindowModified(true);
    updateIconStatus();
  }
}

void MainWindow::on_table_nv_ls0_itemChanged(QTableWidgetItem* item) {
  Q_UNUSED(item);

  if (writeINI) {
    write_value_ini(ui->table_nv_ls0, ui->table_nv_ls,
                    ui->table_nv_ls0->currentRow());
    this->setWindowModified(true);
    updateIconStatus();
  }
}

void MainWindow::on_editIntTarget_textChanged(const QString& arg1) {
  int total = arg1.toInt();

  initTargetValue();

  for (int i = 0; i < chk.count(); i++) chk.at(i)->setChecked(false);

  method(v, total);

  // 10转16
  QString hex = QString("%1").arg(total, 2, 16, QLatin1Char('0'));
  ui->lblTargetHex->setText("0x" + hex.toUpper());
}

void MainWindow::on_editIntHaltLevel_textChanged(const QString& arg1) {
  // 10 to 16
  unsigned int dec = arg1.toULongLong();
  QString hex =
      QString("%1").arg(dec, 8, 16, QLatin1Char('0'));  // 保留8位，不足补零
  ui->lblHaltLevel->setText("0x" + hex.toUpper());
}

void MainWindow::clear_temp_data() {
  for (int i = 0; i < IniFile.count(); i++) {
    QFile file(IniFile.at(i));
    // qDebug() << IniFile.at(i);
    if (file.exists()) {
      file.remove();
      i = -1;
    }
  }
}

void MainWindow::on_table_dp_del_cellClicked(int row, int column) {
  Q_UNUSED(row);
  Q_UNUSED(column);

  setStatusBarText(ui->table_dp_del);
}

void MainWindow::on_tableBlessOverride_cellClicked(int row, int column) {
  Q_UNUSED(row);
  Q_UNUSED(column);

  setStatusBarText(ui->tableBlessOverride);
}

void MainWindow::on_table_nv_del_cellClicked(int row, int column) {
  Q_UNUSED(row);
  Q_UNUSED(column);

  setStatusBarText(ui->table_nv_del);
}

void MainWindow::on_table_nv_ls_cellClicked(int row, int column) {
  Q_UNUSED(row);
  Q_UNUSED(column);

  setStatusBarText(ui->table_nv_ls);
}

void MainWindow::on_tableDevices_cellClicked(int row, int column) {
  Q_UNUSED(row);
  Q_UNUSED(column);

  setStatusBarText(ui->tableDevices);
}

void MainWindow::on_table_uefi_drivers_cellClicked(int row, int column) {
  if (!ui->table_uefi_drivers->currentIndex().isValid()) return;

  set_InitCheckBox(ui->table_uefi_drivers, row, column);

  if (column == 3 &&
      ui->table_uefi_drivers->horizontalHeaderItem(3)->text() == tr("Load")) {
    cboxDataClass = new QComboBox;
    cboxDataClass->setEditable(true);
    cboxDataClass->addItem("Enabled");
    cboxDataClass->addItem("Disabled");
    cboxDataClass->addItem("Early");
    cboxDataClass->addItem("");
    connect(cboxDataClass, SIGNAL(currentTextChanged(QString)), this,
            SLOT(dataClassChange_uefi_drivers()));
    c_row = row;

    ui->table_uefi_drivers->setCellWidget(row, column, cboxDataClass);
    cboxDataClass->setCurrentText(ui->table_uefi_drivers->item(row, 3)->text());
  }

  setStatusBarText(ui->table_uefi_drivers);
}

void MainWindow::on_btnBooterPatchAdd_clicked() {
  Method::add_OneLine(ui->table_Booter_patch);
}

void MainWindow::on_btnBooterPatchDel_clicked() {
  del_item(ui->table_Booter_patch);
}

void MainWindow::on_table_Booter_patch_cellClicked(int row, int column) {
  if (!ui->table_Booter_patch->currentIndex().isValid()) return;

  set_InitCheckBox(ui->table_Booter_patch, row, column);

  int cc = ui->table_Booter_patch->currentColumn();
  if (ui->table_Booter_patch->horizontalHeaderItem(cc)->text() == "Arch") {
    cboxArch = new QComboBox;
    cboxArch->setEditable(true);
    cboxArch->addItem("Any");
    cboxArch->addItem("i386");
    cboxArch->addItem("x86_64");
    cboxArch->addItem("");

    connect(cboxArch, SIGNAL(currentTextChanged(QString)), this,
            SLOT(arch_Booter_patchChange()));
    c_row = row;

    ui->table_Booter_patch->setCellWidget(row, cc, cboxArch);
    cboxArch->setCurrentText(ui->table_Booter_patch->item(row, cc)->text());
  }

  setStatusBarText(ui->table_Booter_patch);
}

void MainWindow::on_table_Booter_patch_currentCellChanged(int currentRow,
                                                          int currentColumn,
                                                          int previousRow,
                                                          int previousColumn) {
  int colCount = ui->table_Booter_patch->columnCount();
  for (int i = 0; i < colCount; i++) {
    QString txt = ui->table_Booter_patch->horizontalHeaderItem(i)->text();
    if (txt == "Arch") {
      ui->table_Booter_patch->removeCellWidget(previousRow, i);
      break;
    }
  }

  currentCellChanged(ui->table_Booter_patch, previousRow, previousColumn,
                     currentRow, currentColumn);
}

void MainWindow::on_btnCheckUpdate() {
  QNetworkRequest quest;
  quest.setUrl(QUrl("https://api.github.com/repos/ic005k/" + strAppName +
                    "/releases/"
                    "latest"));
  quest.setHeader(QNetworkRequest::UserAgentHeader, "RT-Thread ART");
  manager->get(quest);
}

void MainWindow::replyFinished(QNetworkReply* reply) {
  QString str = reply->readAll();
  parse_UpdateJSON(str);
  reply->deleteLater();
}

QString MainWindow::getUrl(QVariantList list) {
  aboutDlg->listDownCount.clear();
  QString macUrl, winUrl, linuxUrl, osx1012Url;
  for (int i = 0; i < list.count(); i++) {
    QVariantMap map = list[i].toMap();
    QString fName = map["name"].toString();

    if (fName.contains("OCAT_Mac.dmg"))
      macUrl = map["browser_download_url"].toString();

    if (fName.contains("Win")) winUrl = map["browser_download_url"].toString();

    if (fName.contains("Linux"))
      linuxUrl = map["browser_download_url"].toString();

    if (fName.contains("below"))
      osx1012Url = map["browser_download_url"].toString();

    aboutDlg->listDownCount.append(map["download_count"].toString());
  }

  QString Url;
  if (mac) Url = macUrl;
  if (win) Url = winUrl;
  if (linuxOS) Url = linuxUrl;
  if (osx1012) Url = osx1012Url;

  return Url;
}

int MainWindow::parse_UpdateJSON(QString str) {
  QJsonParseError err_rpt;
  QJsonDocument root_Doc = QJsonDocument::fromJson(str.toUtf8(), &err_rpt);

  if (err_rpt.error != QJsonParseError::NoError) {
    if (!autoCheckUpdate) {
      QMessageBox::critical(this, "", tr("Network error!"));
    }
    autoCheckUpdate = false;
    return -1;
  }

  if (root_Doc.isObject()) {
    QJsonObject root_Obj = root_Doc.object();

    QVariantList list = root_Obj.value("assets").toArray().toVariantList();
    QString Url = getUrl(list);
    dlgAutoUpdate->strUrlOrg = Url;

    QString UpdateTime = root_Obj.value("published_at").toString();
    QString ReleaseNote = root_Obj.value("body").toString();
    // QJsonObject PulseValue = root_Obj.value("assets").toObject();
    QString Verison = root_Obj.value("tag_name").toString();

    this->setFocus();
    QStringList noteList = ReleaseNote.split("---");
    if (noteList.count() == 3) {
      if (zh_cn)
        ReleaseNote = noteList.at(0) + noteList.at(1);
      else
        ReleaseNote = noteList.at(0) + noteList.at(2);
    }

    if (Verison > CurVersion && Url != "") {
      QString warningStr = tr("New version detected!") + "\n" +
                           tr("Version: ") + "V" + Verison + "\n" +
                           tr("Published at: ") + UpdateTime + "\n" +
                           tr("Release Notes: ") + "\n" + ReleaseNote;

      if (ui->actionAutoChkUpdate->isChecked()) {
        ui->btnCheckUpdate->setIcon(QIcon(":/icon/newver.png"));
        ui->btnCheckUpdate->setToolTip(tr("There is a new version"));

        int ret = QMessageBox::warning(this, "", warningStr, tr("Download"),
                                       tr("Cancel"));
        if (ret == 0) {
          on_actionOnline_Download_Updates_triggered();
        }
      } else {
        if (!autoCheckUpdate) {
          int ret = QMessageBox::warning(this, "", warningStr, tr("Download"),
                                         tr("Cancel"));
          if (ret == 0) {
            on_actionOnline_Download_Updates_triggered();
          }
        }
      }

    } else {
      if (!autoCheckUpdate) {
        QMessageBox::information(
            this, "", tr("You are currently using the latest version!"));
      }
    }

    ui->mycboxFind->setFocus();
  }
  autoCheckUpdate = false;
  return 0;
}

int MainWindow::deleteDirfile(QString dirName) {
  QDir directory(dirName);
  if (!directory.exists()) {
    return true;
  }

  QString srcPath = QDir::toNativeSeparators(dirName);
  if (!srcPath.endsWith(QDir::separator())) srcPath += QDir::separator();

  QStringList fileNames = directory.entryList(
      QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);
  bool error = false;
  for (QStringList::size_type i = 0; i != fileNames.size(); ++i) {
    QString filePath = srcPath + fileNames.at(i);
    QFileInfo fileInfo(filePath);
    if (fileInfo.isFile() || fileInfo.isSymLink()) {
      QFile::setPermissions(filePath, QFile::WriteOwner);
      if (!QFile::remove(filePath)) {
        error = true;
      }
    } else if (fileInfo.isDir()) {
      if (!deleteDirfile(filePath)) {
        error = true;
      }
    }
  }

  if (!directory.rmdir(QDir::toNativeSeparators(directory.path()))) {
    error = true;
  }
  return !error;
}

bool MainWindow::DeleteDirectory(const QString& path) {
  if (path.isEmpty()) {
    return false;
  }

  QDir dir(path);
  if (!dir.exists()) {
    return true;
  }

  dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
  QFileInfoList fileList = dir.entryInfoList();
  foreach (QFileInfo fi, fileList) {
    if (fi.isFile()) {
      fi.dir().remove(fi.fileName());
    } else {
      DeleteDirectory(fi.absoluteFilePath());
    }
  }
  return dir.rmpath(dir.absolutePath());
}

// 拷贝文件：
bool MainWindow::copyFileToPath(QString sourceDir, QString toDir,
                                bool coverFileIfExist) {
  toDir.replace("\\", "/");
  if (sourceDir == toDir) {
    return true;
  }
  if (!QFile::exists(sourceDir)) {
    return false;
  }
  QDir* createfile = new QDir;
  bool exist = createfile->exists(toDir);
  if (exist) {
    if (coverFileIfExist) {
      createfile->remove(toDir);
    }
  }  // end if

  if (!QFile::copy(sourceDir, toDir)) {
    return false;
  }
  return true;
}

// 拷贝文件夹：
bool MainWindow::copyDirectoryFiles(const QString& fromDir,
                                    const QString& toDir,
                                    bool coverFileIfExist) {
  QDir sourceDir(fromDir);
  QDir targetDir(toDir);
  if (!targetDir.exists()) { /**< 如果目标目录不存在，则进行创建 */
    if (!targetDir.mkdir(targetDir.absolutePath())) return false;
  }

  QFileInfoList fileInfoList = sourceDir.entryInfoList();
  foreach (QFileInfo fileInfo, fileInfoList) {
    if (fileInfo.fileName() == "." || fileInfo.fileName() == "..") continue;

    if (fileInfo.isDir()) { /**< 当为目录时，递归的进行copy */
      if (!copyDirectoryFiles(fileInfo.filePath(),
                              targetDir.filePath(fileInfo.fileName()),
                              coverFileIfExist))
        return false;
    } else { /**< 当允许覆盖操作时，将旧文件进行删除操作 */
      if (coverFileIfExist && targetDir.exists(fileInfo.fileName())) {
        targetDir.remove(fileInfo.fileName());
      }

      /// 进行文件copy
      if (!QFile::copy(fileInfo.filePath(),
                       targetDir.filePath(fileInfo.fileName()))) {
        return false;
      }
    }
  }
  return true;
}

int MainWindow::getTextWidth(QString str, QWidget* w) {
  str = str.trimmed();
  str = str + "    ";

  QFont myFont(w->font().family(), w->font().pointSize());

  QFontMetrics fm(myFont);
  int mw;

#if (QT_VERSION <= QT_VERSION_CHECK(5, 9, 9))
  mw = fm.width(str);

#endif

#if (QT_VERSION >= QT_VERSION_CHECK(5, 15, 0))
  mw = fm.horizontalAdvance(str);

#endif

  return mw;
}

void MainWindow::on_listSub_itemSelectionChanged() {
  if (ui->listMain->currentRow() == 0) {
    ui->tabACPI->setCurrentIndex(ui->listSub->currentRow());
  }

  if (ui->listMain->currentRow() == 1) {
    ui->tabBooter->setCurrentIndex(ui->listSub->currentRow());
  }

  if (ui->listMain->currentRow() == 2) {
    ui->tabDP->setCurrentIndex(ui->listSub->currentRow());
  }

  if (ui->listMain->currentRow() == 3) {
    ui->tabKernel->setCurrentIndex(ui->listSub->currentRow());
  }

  if (ui->listMain->currentRow() == 4) {
    ui->tabMisc->setCurrentIndex(ui->listSub->currentRow());
  }

  if (ui->listMain->currentRow() == 5) {
    ui->tabNVRAM->setCurrentIndex(ui->listSub->currentRow());
  }

  if (ui->listMain->currentRow() == 6) {
    ui->tabPlatformInfo->setCurrentIndex(ui->listSub->currentRow());
  }

  if (ui->listMain->currentRow() == 7) {
    ui->tabUEFI->setCurrentIndex(ui->listSub->currentRow());
  }
}

void MainWindow::resizeEvent(QResizeEvent* event) {
  Q_UNUSED(event);
  int index0 = ui->listMain->currentRow();
  int index1 = ui->listSub->currentRow();
  init_listMainSub();
  ui->listMain->setCurrentRow(index0);
  ui->listSub->setCurrentRow(index1);
}

QString MainWindow::getWMIC(const QString& cmd) {
  QProcess* p = new QProcess;
  p->start(cmd, QStringList() << "");
  p->waitForFinished();
  QString result = QString::fromLocal8Bit(p->readAllStandardOutput());
  QStringList list = cmd.split(" ");
  result = result.remove(list.last(), Qt::CaseInsensitive);
  result = result.replace("\r", "");
  result = result.replace("\n", "");
  result = result.simplified();
  return result;
}

QString MainWindow::getCpuName()  // 获取cpu名称：wmic cpu get Name
{
  return getWMIC("wmic cpu get name");
}

QString MainWindow::getCpuId()  // 查询cpu序列号：wmic cpu get processorid
{
  return getWMIC("wmic cpu get processorid");
}

QString
MainWindow::getCpuCoresNum()  // 获取cpu核心数：wmic cpu get NumberOfCores
{
  return getWMIC("wmic cpu get NumberOfCores");
}

QString MainWindow::getCpuLogicalProcessorsNum()  // 获取cpu线程数：wmic cpu get
                                                  //  NumberOfLogicalProcessors
{
  return getWMIC("wmic cpu get NumberOfLogicalProcessors");
}

QString MainWindow::getDiskNum()  // 查看硬盘：wmic diskdrive get serialnumber
{
  return getWMIC("wmic diskdrive where index=0 get serialnumber");
}

QString MainWindow::getBaseBordNum()  // 查询主板序列号：wmic baseboard get
                                      //  serialnumber
{
  return getWMIC("wmic baseboard get serialnumber");
}

QString MainWindow::getBiosNum()  // 查询BIOS序列号：wmic bios get serialnumber
{
  return getWMIC("wmic bios get serialnumber");
}

QString MainWindow::getMainboardName() {
  return getWMIC("wmic csproduct get Name");
}

QString MainWindow::getMainboardUUID() {
  return getWMIC("wmic csproduct get uuid");
}

QString MainWindow::getMainboardVendor() {
  return getWMIC("wmic csproduct get Vendor");
}

QString MainWindow::getMacInfo(const QString& cmd) {
  QProcess p;
  QStringList sl;
  sl << "";
  p.start(cmd, sl);
  p.waitForFinished();
  QString result = QString::fromLocal8Bit(p.readAllStandardOutput());

  result = result.replace("machdep.cpu.", "");
  return result;
}

void MainWindow::on_table_dp_add0_itemSelectionChanged() {
  readLeftTable(ui->table_dp_add0, ui->table_dp_add);
}

void MainWindow::on_table_dp_del0_itemSelectionChanged() {
  readLeftTableOnlyValue(ui->table_dp_del0, ui->table_dp_del);
}

void MainWindow::on_table_nv_add0_itemSelectionChanged() {
  readLeftTable(ui->table_nv_add0, ui->table_nv_add);
}

void MainWindow::on_table_nv_del0_itemSelectionChanged() {
  readLeftTableOnlyValue(ui->table_nv_del0, ui->table_nv_del);
}

void MainWindow::on_table_nv_ls0_itemSelectionChanged() {
  readLeftTableOnlyValue(ui->table_nv_ls0, ui->table_nv_ls);
}

void MainWindow::on_table_acpi_add_itemEntered(QTableWidgetItem* item) {
  Q_UNUSED(item);
}

void MainWindow::on_table_acpi_add_cellEntered(int row, int column) {
  Q_UNUSED(row);
  Q_UNUSED(column);
}

void MainWindow::lineEdit_textChanged(const QString& arg1) {
  Q_UNUSED(arg1);
  lineEditModifyed = true;
}

void MainWindow::lineEditSetText() {
  if (!Initialization) {
    lineEditEnter = true;
    InitEdit = false;

    int row;
    int col;
    QString oldText;
    row = myTable->currentRow();
    col = myTable->currentColumn();

    if (!lineEditModifyed) {
      myTable->removeCellWidget(row, col);
      myTable->setCurrentCell(row, col);
      myTable->setFocus();

      return;
    }

    if (getTableFieldDataType(myTable) == "Data") {
      int count = lineEdit->text().trimmed().count();

      if (count % 2 != 0) return;

      QString text = lineEdit->text().trimmed();
      lineEdit->setText(text.toUpper());
    }

    oldText = myTable->item(row, col)->text();

    bool textAlignCenter = false;
    QString colTextList =
        "CountLimitSkipBaseSkipMaxKernelMinKernelAssetTagBankLocatorDeviceLoc"
        "at"
        "orManufacturerPartNumberSerialNumberSizeSpeed";
    QString strItem = myTable->horizontalHeaderItem(col)->text();
    QStringList strEn = strItem.split("\n");
    if (strEn.count() == 2) {
      if (colTextList.contains(strEn.at(1))) {
        textAlignCenter = true;
      }
    } else {
      if (colTextList.contains(strItem)) {
        textAlignCenter = true;
      }
    }

    QString cuText = lineEdit->text();
    int reCount = 0;
    if (myTable == ui->table_dp_add0 || myTable == ui->table_dp_del0 ||
        myTable == ui->table_nv_add0 || myTable == ui->table_nv_del0 ||
        myTable == ui->table_nv_ls0) {
      if (myTable->rowCount() > 1) {
        for (int i = 0; i < myTable->rowCount(); i++) {
          QString str = myTable->item(i, 0)->text();
          if (str == cuText) reCount++;
        }

        if (reCount > 0)
          lineEdit->setText(cuText + "-" + QString::number(reCount));
      }
    }

    QUndoCommand* editCommand = new EditCommand(
        textAlignCenter, oldText, myTable, myTable->currentRow(),
        myTable->currentColumn(), lineEdit->text());
    undoStack->push(editCommand);

    this->setWindowModified(true);
    updateIconStatus();

    myTable->removeCellWidget(row, col);
    myTable->setCurrentCell(row, col);
    myTable->setFocus();
  }
}

void MainWindow::lineEdit_textEdited(const QString& arg1) { Q_UNUSED(arg1); }

void MainWindow::currentCellChanged(QTableWidget* t, int previousRow,
                                    int previousColumn, int currentRow,
                                    int currentColumn) {
  if (InitEdit) {
    InitEdit = false;
    t->setCurrentCell(previousRow, previousColumn);
    lineEditSetText();
    t->setCurrentCell(currentRow, currentColumn);
  }

  t->removeCellWidget(previousRow, previousColumn);
  lineEditEnter = false;

  setStatusBarText(t);
}

void MainWindow::on_table_nv_ls_currentCellChanged(int currentRow,
                                                   int currentColumn,
                                                   int previousRow,
                                                   int previousColumn) {
  currentCellChanged(ui->table_nv_ls, previousRow, previousColumn, currentRow,
                     currentColumn);
}

void MainWindow::on_table_acpi_add_currentCellChanged(int currentRow,
                                                      int currentColumn,
                                                      int previousRow,
                                                      int previousColumn) {
  currentCellChanged(ui->table_acpi_add, previousRow, previousColumn,
                     currentRow, currentColumn);
}

void MainWindow::on_table_nv_add0_currentCellChanged(int currentRow,
                                                     int currentColumn,
                                                     int previousRow,
                                                     int previousColumn) {
  if (!loading) {
    currentCellChanged(ui->table_nv_add0, previousRow, previousColumn,
                       currentRow, currentColumn);
  }
}

void MainWindow::on_table_acpi_del_currentCellChanged(int currentRow,
                                                      int currentColumn,
                                                      int previousRow,
                                                      int previousColumn) {
  currentCellChanged(ui->table_acpi_del, previousRow, previousColumn,
                     currentRow, currentColumn);
}

void MainWindow::on_table_acpi_patch_currentCellChanged(int currentRow,
                                                        int currentColumn,
                                                        int previousRow,
                                                        int previousColumn) {
  currentCellChanged(ui->table_acpi_patch, previousRow, previousColumn,
                     currentRow, currentColumn);
}

void MainWindow::on_table_booter_currentCellChanged(int currentRow,
                                                    int currentColumn,
                                                    int previousRow,
                                                    int previousColumn) {
  currentCellChanged(ui->table_booter, previousRow, previousColumn, currentRow,
                     currentColumn);
}

void MainWindow::on_tableBlessOverride_currentCellChanged(int currentRow,
                                                          int currentColumn,
                                                          int previousRow,
                                                          int previousColumn) {
  currentCellChanged(ui->tableBlessOverride, previousRow, previousColumn,
                     currentRow, currentColumn);
}

void MainWindow::on_tableEntries_currentCellChanged(int currentRow,
                                                    int currentColumn,
                                                    int previousRow,
                                                    int previousColumn) {
  currentCellChanged(ui->tableEntries, previousRow, previousColumn, currentRow,
                     currentColumn);
}

void MainWindow::on_tableTools_currentCellChanged(int currentRow,
                                                  int currentColumn,
                                                  int previousRow,
                                                  int previousColumn) {
  currentCellChanged(ui->tableTools, previousRow, previousColumn, currentRow,
                     currentColumn);
}

void MainWindow::on_tableDevices_currentCellChanged(int currentRow,
                                                    int currentColumn,
                                                    int previousRow,
                                                    int previousColumn) {
  currentCellChanged(ui->tableDevices, previousRow, previousColumn, currentRow,
                     currentColumn);
}

void MainWindow::on_table_uefi_drivers_currentCellChanged(int currentRow,
                                                          int currentColumn,
                                                          int previousRow,
                                                          int previousColumn) {
  currentCellChanged(ui->table_uefi_drivers, previousRow, previousColumn,
                     currentRow, currentColumn);
}

void MainWindow::on_table_dp_add0_currentCellChanged(int currentRow,
                                                     int currentColumn,
                                                     int previousRow,
                                                     int previousColumn) {
  if (!loading) {
    currentCellChanged(ui->table_dp_add0, previousRow, previousColumn,
                       currentRow, currentColumn);
  }
}

void MainWindow::on_table_dp_del0_currentCellChanged(int currentRow,
                                                     int currentColumn,
                                                     int previousRow,
                                                     int previousColumn) {
  if (!loading) {
    currentCellChanged(ui->table_dp_del0, previousRow, previousColumn,
                       currentRow, currentColumn);
  }
}

void MainWindow::on_table_dp_del_currentCellChanged(int currentRow,
                                                    int currentColumn,
                                                    int previousRow,
                                                    int previousColumn) {
  currentCellChanged(ui->table_dp_del, previousRow, previousColumn, currentRow,
                     currentColumn);
}

void MainWindow::removeWidget(QTableWidget* table) {
  int row = table->currentRow();
  int col = table->currentColumn();

  table->removeCellWidget(row, col);

  lineEdit = NULL;
}

void MainWindow::removeAllLineEdit() {
  listOfTableWidget.clear();
  listOfTableWidget = getAllTableWidget(getAllUIControls(ui->tabTotal));
  for (int i = 0; i < listOfTableWidget.count(); i++) {
    QTableWidget* w = (QTableWidget*)listOfTableWidget.at(i);

    removeWidget(w);
  }
}

void MainWindow::on_table_nv_del0_currentCellChanged(int currentRow,
                                                     int currentColumn,
                                                     int previousRow,
                                                     int previousColumn) {
  if (!loading) {
    currentCellChanged(ui->table_nv_del0, previousRow, previousColumn,
                       currentRow, currentColumn);
  }
}

void MainWindow::on_table_nv_ls0_currentCellChanged(int currentRow,
                                                    int currentColumn,
                                                    int previousRow,
                                                    int previousColumn) {
  if (!loading) {
    currentCellChanged(ui->table_nv_ls0, previousRow, previousColumn,
                       currentRow, currentColumn);
  }
}

void MainWindow::on_table_nv_del_currentCellChanged(int currentRow,
                                                    int currentColumn,
                                                    int previousRow,
                                                    int previousColumn) {
  currentCellChanged(ui->table_nv_del, previousRow, previousColumn, currentRow,
                     currentColumn);
}

void MainWindow::on_table_dp_add0_cellDoubleClicked(int row, int column) {
  myTable = new QTableWidget;
  myTable = ui->table_dp_add0;

  initLineEdit(myTable, row, column, row, column);
}

void MainWindow::on_table_dp_add_cellDoubleClicked(int row, int column) {
  myTable = new QTableWidget;
  myTable = ui->table_dp_add;

  initLineEdit(myTable, row, column, row, column);
}

void MainWindow::on_table_dp_del0_cellDoubleClicked(int row, int column) {
  myTable = new QTableWidget;
  myTable = ui->table_dp_del0;

  initLineEdit(myTable, row, column, row, column);
}

void MainWindow::on_table_acpi_add_cellDoubleClicked(int row, int column) {
  set_InitLineEdit(ui->table_acpi_add, row, column);
}

void MainWindow::setTableEditTriggers() {
  listOfTableWidget.clear();
  listOfTableWidget = getAllTableWidget(getAllUIControls(ui->tabTotal));
  for (int i = 0; i < listOfTableWidget.count(); i++) {
    QTableWidget* w = (QTableWidget*)listOfTableWidget.at(i);
    w->setEditTriggers(QAbstractItemView::NoEditTriggers);
  }
}

void MainWindow::on_table_acpi_del_cellDoubleClicked(int row, int column) {
  set_InitLineEdit(ui->table_acpi_del, row, column);
}

void MainWindow::on_table_acpi_patch_cellDoubleClicked(int row, int column) {
  set_InitLineEdit(ui->table_acpi_patch, row, column);
}

void MainWindow::on_table_booter_cellDoubleClicked(int row, int column) {
  set_InitLineEdit(ui->table_booter, row, column);
}

void MainWindow::on_table_Booter_patch_cellDoubleClicked(int row, int column) {
  set_InitLineEdit(ui->table_Booter_patch, row, column);
}

void MainWindow::set_InitLineEdit(QTableWidget* t, int row, int column) {
  QString txt = t->horizontalHeaderItem(column)->text();
  if (txt != "Enabled" && txt != "Arch" && txt != "All" && txt != "Auxiliary" &&
      txt != "TextMode" && txt != "RealPath") {
    myTable = new QTableWidget;
    myTable = t;
    initLineEdit(myTable, row, column, row, column);
  }
}

void MainWindow::set_InitCheckBox(QTableWidget* t, int row, int column) {
  QString txt = t->horizontalHeaderItem(column)->text();
  if (Method::isBool(txt)) {
    enabled_change(t, row, column, column);
  }
}

void MainWindow::on_table_kernel_add_cellDoubleClicked(int row, int column) {
  set_InitLineEdit(ui->table_kernel_add, row, column);
}

void MainWindow::on_table_kernel_block_cellDoubleClicked(int row, int column) {
  set_InitLineEdit(ui->table_kernel_block, row, column);
}

void MainWindow::on_table_kernel_Force_cellDoubleClicked(int row, int column) {
  set_InitLineEdit(ui->table_kernel_Force, row, column);
}

void MainWindow::on_table_kernel_patch_cellDoubleClicked(int row, int column) {
  set_InitLineEdit(ui->table_kernel_patch, row, column);
}

void MainWindow::on_tableBlessOverride_cellDoubleClicked(int row, int column) {
  set_InitLineEdit(ui->tableBlessOverride, row, column);
}

void MainWindow::on_tableEntries_cellDoubleClicked(int row, int column) {
  set_InitLineEdit(ui->tableEntries, row, column);
}

void MainWindow::on_tableTools_cellDoubleClicked(int row, int column) {
  set_InitLineEdit(ui->tableTools, row, column);
}

void MainWindow::on_table_nv_add0_cellDoubleClicked(int row, int column) {
  set_InitLineEdit(ui->table_nv_add0, row, column);
}

void MainWindow::on_table_nv_add_cellDoubleClicked(int row, int column) {
  set_InitLineEdit(ui->table_nv_add, row, column);
}

void MainWindow::on_table_nv_del0_cellDoubleClicked(int row, int column) {
  set_InitLineEdit(ui->table_nv_del0, row, column);
}

void MainWindow::on_table_nv_del_cellDoubleClicked(int row, int column) {
  set_InitLineEdit(ui->table_nv_del, row, column);
}

void MainWindow::on_table_nv_ls0_cellDoubleClicked(int row, int column) {
  set_InitLineEdit(ui->table_nv_ls0, row, column);
}

void MainWindow::on_table_nv_ls_cellDoubleClicked(int row, int column) {
  set_InitLineEdit(ui->table_nv_ls, row, column);
}

void MainWindow::on_tableDevices_cellDoubleClicked(int row, int column) {
  set_InitLineEdit(ui->tableDevices, row, column);
}

void MainWindow::on_table_uefi_drivers_cellDoubleClicked(int row, int column) {
  set_InitLineEdit(ui->table_uefi_drivers, row, column);
}

void MainWindow::on_table_uefi_ReservedMemory_cellDoubleClicked(int row,
                                                                int column) {
  set_InitLineEdit(ui->table_uefi_ReservedMemory, row, column);
}

void MainWindow::on_table_uefi_Unload_cellDoubleClicked(int row, int column) {
  set_InitLineEdit(ui->table_uefi_Unload, row, column);
}

void MainWindow::on_table_dp_del_cellDoubleClicked(int row, int column) {
  set_InitLineEdit(ui->table_dp_del, row, column);
}

void MainWindow::on_actionNewWindow_triggered() {
  QString path = strAppExePath + "/OCAuxiliaryTools";
  QStringList arguments;
  QString fn = "";
  arguments << fn;
  QProcess* process = new QProcess;
  process->setEnvironment(process->environment());
  process->start(path, arguments);
}

QObjectList MainWindow::getAllUIControls(QObject* parent) {
  QObjectList lstOfChildren, lstTemp;
  if (parent) {
    lstOfChildren = parent->children();
  }
  if (lstOfChildren.isEmpty()) {
    return lstOfChildren;
  }

  lstTemp =
      lstOfChildren; /*  这里要注意，如果不拷贝原先的list，直接使用，会有问题；
                      */

  foreach (QObject* obj, lstTemp) {
    QObjectList lst = getAllUIControls(obj);
    if (!lst.isEmpty()) {
      lstOfChildren.append(lst);
    }
  }
  return lstOfChildren;
}

QObjectList MainWindow::getAllCheckBox(QObjectList lstUIControls) {
  QObjectList lstOfCheckBox;
  foreach (QObject* obj, lstUIControls) {
    if (obj->metaObject()->className() == QStringLiteral("QCheckBox")) {
      lstOfCheckBox.append(obj);
    }
  }
  return lstOfCheckBox;
}

QObjectList MainWindow::getAllTableWidget(QObjectList lstUIControls) {
  QObjectList lstOfTableWidget;
  foreach (QObject* obj, lstUIControls) {
    if (obj->metaObject()->className() == QStringLiteral("QTableWidget")) {
      lstOfTableWidget.append(obj);
    }
  }
  return lstOfTableWidget;
}

QObjectList MainWindow::getAllLabel(QObjectList lstUIControls) {
  QObjectList lstOfLabel;
  foreach (QObject* obj, lstUIControls) {
    if (obj->metaObject()->className() == QStringLiteral("QLabel")) {
      lstOfLabel.append(obj);
    }
  }
  return lstOfLabel;
}

QObjectList MainWindow::getAllLineEdit(QObjectList lstUIControls) {
  QObjectList lstOfLineEdit;
  foreach (QObject* obj, lstUIControls) {
    if (obj->metaObject()->className() == QStringLiteral("QLineEdit")) {
      lstOfLineEdit.append(obj);
    }
  }
  return lstOfLineEdit;
}

QObjectList MainWindow::getAllFrame(QObjectList lstUIControls) {
  QObjectList lstOfWidget;
  foreach (QObject* obj, lstUIControls) {
    if (obj->metaObject()->className() == QStringLiteral("QFrame")) {
      lstOfWidget.append(obj);
    }
  }
  return lstOfWidget;
}

QObjectList MainWindow::getAllGridLayout(QObjectList lstUIControls) {
  QObjectList lstOfGridLayout;
  foreach (QObject* obj, lstUIControls) {
    if (obj->metaObject()->className() == QStringLiteral("QGridLayout")) {
      lstOfGridLayout.append(obj);
    }
  }
  return lstOfGridLayout;
}

QObjectList MainWindow::getAllComboBox(QObjectList lstUIControls) {
  QObjectList lstOfComboBox;
  foreach (QObject* obj, lstUIControls) {
    if (obj->metaObject()->className() == QStringLiteral("QComboBox")) {
      lstOfComboBox.append(obj);
    }
  }
  return lstOfComboBox;
}

QObjectList MainWindow::getAllToolButton(QObjectList lstUIControls) {
  QObjectList lstOfToolbutton;
  foreach (QObject* obj, lstUIControls) {
    if (obj->metaObject()->className() == QStringLiteral("QToolButton")) {
      lstOfToolbutton.append(obj);
    }
  }
  return lstOfToolbutton;
}

void MainWindow::findCheckBox(QString findText) {
  // CheckBox  1
  listOfCheckBox.clear();
  listOfCheckBox = getAllCheckBox(getAllUIControls(ui->tabTotal));

  listOfCheckBoxResults.clear();

  for (int i = 0; i < listOfCheckBox.count(); i++) {
    QCheckBox* chkbox = (QCheckBox*)listOfCheckBox.at(i);
    if (chkbox->text().toLower().contains(findText.trimmed().toLower()) &&
        !chkbox->isHidden()) {
      findCount++;
      listOfCheckBoxResults.append(chkbox);
      listNameResults.append("1" + chkbox->objectName());
      ui->listFind->addItem(chkbox->text());
    }
  }
}

void MainWindow::findLabel(QString findText) {
  // Label  3
  listOfLabel.clear();
  listOfLabel = getAllLabel(getAllUIControls(ui->tabTotal));
  listOfLabelResults.clear();
  for (int i = 0; i < listOfLabel.count(); i++) {
    QLabel* lbl = (QLabel*)listOfLabel.at(i);
    if (lbl->text().toLower().contains(findText.trimmed().toLower())) {
      if (!lbl->isHidden()) {
        findCount++;
        listOfLabelResults.append(lbl);
        listNameResults.append("3" + lbl->objectName());

        ui->listFind->addItem(lbl->text());
      }
    }
  }
}

void MainWindow::findLineEdit(QString findText) {
  // LineEdit  4
  listOfLineEdit.clear();
  listOfLineEdit = getAllLineEdit(getAllUIControls(ui->tabTotal));
  listOfComboBox.clear();
  listOfComboBox = getAllComboBox(getAllUIControls(ui->tabTotal));
  listOfLineEditResults.clear();
  for (int i = 0; i < listOfLineEdit.count(); i++) {
    QLineEdit* edit = (QLineEdit*)listOfLineEdit.at(i);
    bool add = true;

    for (int j = 0; j < listOfComboBox.count(); j++) {
      QComboBox* cbox = (QComboBox*)listOfComboBox.at(j);
      if (edit == cbox->lineEdit()) {
        add = false;
      }
    }

    if (edit == lineEdit) add = false;

    if (add) {
      if (edit->text().toLower().contains(findText.trimmed().toLower()) &&
          !edit->isHidden()) {
        findCount++;
        listOfLineEditResults.append(edit);
        listNameResults.append("4" + edit->objectName());

        ui->listFind->addItem(edit->text());
      }
    }
  }
}

void MainWindow::findComboBox(QString findText) {
  // ComboBox  5
  listOfComboBox.clear();
  listOfComboBox = getAllComboBox(getAllUIControls(ui->tabTotal));
  listOfComboBoxResults.clear();
  for (int i = 0; i < listOfComboBox.count(); i++) {
    QComboBox* cbox = (QComboBox*)listOfComboBox.at(i);
    QString objname = cbox->objectName();
    if (objname.mid(0, 4) == "cbox") {
      if (cbox->currentText().toLower().contains(
              findText.trimmed().toLower()) &&
          !cbox->isHidden()) {
        findCount++;
        listOfComboBoxResults.append(cbox);
        listNameResults.append("5" + cbox->objectName());

        ui->listFind->addItem(cbox->currentText());
      }
    }
  }
}

void MainWindow::findTableHeader(QString findText) {
  // TableHeader 6

  listOfTableWidget.clear();
  listOfTableWidget = getAllTableWidget(getAllUIControls(ui->tabTotal));
  listOfTableWidgetHeaderResults.clear();
  for (int i = 0; i < listOfTableWidget.count(); i++) {
    QTableWidget* t;
    t = (QTableWidget*)listOfTableWidget.at(i);

    for (int j = 0; j < t->columnCount(); j++) {
      QString strColumn = t->horizontalHeaderItem(j)->text();
      if (strColumn.toLower().contains(findText)) {
        findCount++;
        listOfTableWidgetHeaderResults.append(t);
        listNameResults.append("6" + t->objectName());
        ui->listFind->addItem(strColumn);
      }
    }
  }
}

void MainWindow::findTabText(QString findText) {
  // TabText 7
  for (int i = 0; i < ui->listMain->count(); i++) {
    QString strMain = ui->listMain->item(i)->text();
    if (strMain.toLower().contains(findText)) {
      findCount++;
      listNameResults.append("7-" + QString::number(i) + "-0");
      ui->listFind->addItem(strMain);
    }

    ui->listMain->setCurrentRow(i);
    for (int j = 0; j < ui->listSub->count(); j++) {
      QString strSub = ui->listSub->item(j)->text();
      if (strSub.toLower().contains(findText)) {
        findCount++;
        listNameResults.append("7-" + QString::number(i) + "-" +
                               QString::number(j));
        ui->listFind->addItem(strSub);
      }
    }
  }
}

void MainWindow::on_actionFind_triggered() {
  ui->mycboxFind->lineEdit()->selectAll();
  ui->mycboxFind->setFocus();
  if (myDlgPreference->ui->chkHideToolbar->isChecked() &&
      ui->toolBar->isHidden()) {
    ui->frameToolBar->setHidden(false);
    ui->mycboxFind->setFocus();
  }

  find = true;

  if (!FindTextChange) {
    on_actionGo_to_the_next_triggered();
    return;
  }

  bool curWinModi = this->isWindowModified();

  QString findText = ui->mycboxFind->currentText().trimmed().toLower();

  if (findText == "") return;

  if (ui->mycboxFind->count() > 0) clearTextsAction->setEnabled(true);

  findCount = 0;
  listNameResults.clear();
  loading = true;
  ui->listFind->clear();
  loading = false;
  indexOfResults = -1;

  init_FindResults();

  findCheckBox(findText);

  mymethod->findTable(findText);

  findLabel(findText);

  findLineEdit(findText);

  findComboBox(findText);

  findTableHeader(findText);

  findTabText(findText);

  ui->lblCount->setText(QString::number(findCount));

  if (listNameResults.count() > 0) {
    ui->listFind->setHidden(false);

    ui->listFind->setCurrentRow(0);

    int index = ui->listMain->currentRow();
    setListMainIcon();
    ui->listMain->setCurrentRow(index);

    QStringList strList;
    for (int i = 0; i < ui->mycboxFind->count(); i++) {
      strList.append(ui->mycboxFind->itemText(i));
    }

    for (int i = 0; i < strList.count(); i++) {
      if (findText == strList.at(i)) {
        strList.removeAt(i);
      }
    }

    strList.insert(0, findText);
    AddCboxFindItem = true;
    ui->mycboxFind->clear();
    ui->mycboxFind->addItems(strList);
    AddCboxFindItem = false;
    FindTextChange = false;
    Method::setComboBoxQss(ui->mycboxFind, 6, 1, "#C0C0C0", "#4169E1");
    init_ToolBarIcon();

  } else {
    Method::setComboBoxQss(ui->mycboxFind, 6, 2, "#FF0000", "#FF0000");
  }

  this->setWindowModified(curWinModi);
  updateIconStatus();

  find = false;

  ui->mycboxFind->lineEdit()->selectAll();
}

void MainWindow::init_FindResults() {
  clearCheckBoxMarker();
  clearComboBoxMarker();
  clearLabelMarker();
  clearLineEditMarker();
  clearTableHeaderMarker();

  listOfCheckBoxResults.clear();
  listOfLabelResults.clear();
  listOfLineEditResults.clear();
  listOfComboBoxResults.clear();
  listOfTableWidgetResults.clear();
  listOfTableWidgetHeaderResults.clear();
}

void MainWindow::setPalette(QWidget* w, QColor backColor, QColor textColor) {
  QPalette palette;
  palette = w->palette();
  palette.setColor(QPalette::Base, backColor);
  palette.setColor(QPalette::Text, textColor);
  w->setPalette(palette);
}

void MainWindow::findTable(QTableWidget* t, QString text) {
  for (int i = 0; i < t->rowCount(); i++) {
    for (int j = 0; j < t->columnCount(); ++j) {
      if (t->item(i, j)->text().trimmed().toLower().contains(
              text.trimmed().toLower())) {
        findCount++;
        listOfTableWidgetResults.append(t);
        listNameResults.append("2" + t->objectName());

        ui->listFind->addItem(t->item(i, j)->text());

        // 命名规则：当前位置+对象名称
        QString name =
            QString::number(listNameResults.count() - 1) + t->objectName();

        QString plistPath = strConfigPath + CurrentDateTime + name + ".ini";
        // qDebug() << plistPath;

        QFile file(plistPath);
        if (file.exists())  // 如果文件存在，则先删除它
          file.remove();

        QSettings Reg(plistPath, QSettings::IniFormat);

        Reg.setValue("row", i);
        Reg.setValue("col", j);

        if (t == ui->table_dp_add) {
          Reg.setValue("leftTable", ui->table_dp_add0->objectName());
          Reg.setValue("leftTableRow", ui->table_dp_add0->currentRow());
        }

        if (t == ui->table_dp_del) {
          Reg.setValue("leftTable", ui->table_dp_del0->objectName());
          Reg.setValue("leftTableRow", ui->table_dp_del0->currentRow());
        }

        if (t == ui->table_nv_add) {
          Reg.setValue("leftTable", ui->table_nv_add0->objectName());
          Reg.setValue("leftTableRow", ui->table_nv_add0->currentRow());
        }

        if (t == ui->table_nv_del) {
          Reg.setValue("leftTable", ui->table_nv_del0->objectName());
          Reg.setValue("leftTableRow", ui->table_nv_del0->currentRow());
        }

        if (t == ui->table_nv_ls) {
          Reg.setValue("leftTable", ui->table_nv_ls0->objectName());
          Reg.setValue("leftTableRow", ui->table_nv_ls0->currentRow());
        }

        IniFile.push_back(plistPath);
      }
    }
  }
}

void MainWindow::on_actionGo_to_the_previous_triggered() {
  if (listNameResults.count() == 0) return;

  int row = ui->listFind->currentRow();
  row = row - 1;
  if (row == -1) row = 0;

  ui->listFind->setCurrentRow(row);
  goResults(row);
}

void MainWindow::on_actionGo_to_the_next_triggered() {
  if (listNameResults.count() == 0) return;
  int row = ui->listFind->currentRow();
  if (row < 0) return;
  row = row + 1;
  if (row == ui->listFind->count()) row = 0;

  ui->listFind->setCurrentRow(row);
  goResults(row);
}

void MainWindow::goResultsCheckbox(QString objName) {
  // chkbox 1
  bool end = false;
  QString name = objName.mid(1, objName.length() - 1);

  if (objName.mid(0, 1) == "1") {
    for (int i = 0; i < ui->listMain->count(); i++) {
      if (end) break;

      ui->listMain->setCurrentRow(i);
      for (int j = 0; j < ui->listSub->count(); j++) {
        if (end) break;

        ui->listSub->setCurrentRow(j);
        currentTabWidget = getSubTabWidget(i, j);
        listOfCheckBox.clear();
        listOfCheckBox = getAllCheckBox(getAllUIControls(currentTabWidget));
        for (int k = 0; k < listOfCheckBox.count(); k++) {
          if (listOfCheckBox.at(k)->objectName() == name) {
            orgCheckBoxStyle = ui->chkFadtEnableReset->styleSheet();
            QString style =
                "QCheckBox{background-color:rgb(255,0,0);color:rgb(255,255,"
                "255)"
                ";}";
            QCheckBox* w = (QCheckBox*)listOfCheckBox.at(k);
            w->setStyleSheet(style);
            end = true;

            break;
          }
        }
      }
    }

    if (!end) {
      for (int i = 0; i < ui->listMain->count(); i++) {
        if (end) break;
        ui->listMain->setCurrentRow(i);
        listOfCheckBox.clear();
        listOfCheckBox = getAllCheckBox(getAllUIControls(currentMainTabWidget));
        for (int k = 0; k < listOfCheckBox.count(); k++) {
          QCheckBox* w = (QCheckBox*)listOfCheckBox.at(k);
          if (w->objectName() == name) {
            QString style =
                "QCheckBox{background-color:rgb(255,0,0);color:rgb(255,255,"
                "255)"
                ";}";
            w->setStyleSheet(style);
            end = true;
            break;
          }
        }
      }
    }
  }
}

void MainWindow::goResultsTable(QString objName, int index) {
  // table 2
  bool end = false;
  QString name = objName.mid(1, objName.length() - 1);

  if (objName.mid(0, 1) == "2") {
    for (int i = 0; i < ui->listMain->count(); i++) {
      if (end) break;

      ui->listMain->setCurrentRow(i);
      for (int j = 0; j < ui->listSub->count(); j++) {
        if (end) break;

        ui->listSub->setCurrentRow(j);
        currentTabWidget = getSubTabWidget(i, j);
        listOfTableWidget.clear();
        listOfTableWidget =
            getAllTableWidget(getAllUIControls(currentTabWidget));
        for (int k = 0; k < listOfTableWidget.count(); k++) {
          if (listOfTableWidget.at(k)->objectName() == name) {
            QString nameINI = QString::number(index) + name;

            QString plistPath =
                strConfigPath + CurrentDateTime + nameINI + ".ini";

            QFile file(plistPath);
            if (file.exists()) {
              QSettings Reg(plistPath, QSettings::IniFormat);
              int row = Reg.value("row").toInt();
              int col = Reg.value("col").toInt();

              if (Reg.value("leftTable").toString() ==
                  ui->table_dp_add0->objectName()) {
                ui->table_dp_add0->setCurrentCell(
                    Reg.value("leftTableRow").toInt(), 0);
              }

              if (Reg.value("leftTable").toString() ==
                  ui->table_dp_del0->objectName()) {
                ui->table_dp_del0->setCurrentCell(
                    Reg.value("leftTableRow").toInt(), 0);
              }

              if (Reg.value("leftTable").toString() ==
                  ui->table_nv_add0->objectName()) {
                ui->table_nv_add0->setCurrentCell(
                    Reg.value("leftTableRow").toInt(), 0);
              }

              if (Reg.value("leftTable").toString() ==
                  ui->table_nv_del0->objectName()) {
                ui->table_nv_del0->setCurrentCell(
                    Reg.value("leftTableRow").toInt(), 0);
              }

              if (Reg.value("leftTable").toString() ==
                  ui->table_nv_ls0->objectName()) {
                ui->table_nv_ls0->setCurrentCell(
                    Reg.value("leftTableRow").toInt(), 0);
              }

              QTableWidget* w = (QTableWidget*)listOfTableWidget.at(k);

              w->clearSelection();
              w->setCurrentCell(row, col);
            }

            end = true;

            break;
          }
        }
      }
    }
  }
}

void MainWindow::goResultsLabel(QString objName) {
  // label  3
  bool end = false;
  QString name = objName.mid(1, objName.length() - 1);

  if (objName.mid(0, 1) == "3") {
    for (int i = 0; i < ui->listMain->count(); i++) {
      if (end) break;

      ui->listMain->setCurrentRow(i);
      for (int j = 0; j < ui->listSub->count(); j++) {
        if (end) break;

        ui->listSub->setCurrentRow(j);
        currentTabWidget = getSubTabWidget(i, j);
        listOfLabel.clear();
        listOfLabel = getAllLabel(getAllUIControls(currentTabWidget));
        for (int k = 0; k < listOfLabel.count(); k++) {
          if (listOfLabel.at(k)->objectName() == name) {
            orgLabelStyle = ui->label->styleSheet();
            QString style =
                "QLabel{background-color:rgb(255,0,0);color:rgb(255,255,255);"
                "}";
            QLabel* w = (QLabel*)listOfLabel.at(k);
            w->setStyleSheet(style);
            end = true;
            break;
          }
        }
      }
    }

    if (!end) {
      for (int i = 0; i < ui->listMain->count(); i++) {
        if (end) break;

        ui->listMain->setCurrentRow(i);
        listOfLabel.clear();
        listOfLabel = getAllLabel(getAllUIControls(currentMainTabWidget));
        for (int k = 0; k < listOfLabel.count(); k++) {
          if (listOfLabel.at(k)->objectName() == name) {
            QString style =
                "QLabel{background-color:rgba(255,0,0,255);color:rgb(255,255,"
                "255);}";
            QLabel* w = (QLabel*)listOfLabel.at(k);
            w->setStyleSheet(style);
            end = true;
            break;
          }
        }
      }
    }
  }
}

void MainWindow::goResultsLineEdit(QString objName) {
  // lineedit  4
  bool end = false;
  QString name = objName.mid(1, objName.length() - 1);

  if (objName.mid(0, 1) == "4") {
    for (int i = 0; i < ui->listMain->count(); i++) {
      if (end) break;

      ui->listMain->setCurrentRow(i);
      for (int j = 0; j < ui->listSub->count(); j++) {
        if (end) break;

        ui->listSub->setCurrentRow(j);
        currentTabWidget = getSubTabWidget(i, j);
        listOfLineEdit.clear();
        listOfLineEdit = getAllLineEdit(getAllUIControls(currentTabWidget));
        for (int k = 0; k < listOfLineEdit.count(); k++) {
          if (listOfLineEdit.at(k)->objectName() == name) {
            orgLineEditStyle = ui->editBID->styleSheet();

            QString style =
                "QLineEdit{background-color:rgba(255,0,0,255);color:rgb(255,"
                "255,255);}";
            QLineEdit* w = (QLineEdit*)listOfLineEdit.at(k);
            w->setStyleSheet(style);
            end = true;
            break;
          }
        }
      }
    }

    if (!end) {
      for (int i = 0; i < ui->listMain->count(); i++) {
        if (end) break;

        ui->listMain->setCurrentRow(i);
        listOfLineEdit.clear();
        listOfLineEdit = getAllLineEdit(getAllUIControls(currentMainTabWidget));
        for (int k = 0; k < listOfLineEdit.count(); k++) {
          if (listOfLineEdit.at(k)->objectName() == name) {
            QString style =
                "QLineEdit{background-color:rgba(255,0,0,255);color:rgb(255,"
                "255,255);}";
            QLineEdit* w = (QLineEdit*)listOfLineEdit.at(k);
            w->setStyleSheet(style);
            end = true;
            break;
          }
        }
      }
    }
  }
}

void MainWindow::goResultsComboBox(QString objName) {
  // combobox  5
  bool end = false;
  QString name = objName.mid(1, objName.length() - 1);

  if (objName.mid(0, 1) == "5") {
    for (int i = 0; i < ui->listMain->count(); i++) {
      if (end) break;

      ui->listMain->setCurrentRow(i);
      for (int j = 0; j < ui->listSub->count(); j++) {
        if (end) break;

        ui->listSub->setCurrentRow(j);
        currentTabWidget = getSubTabWidget(i, j);
        listOfComboBox.clear();
        listOfComboBox = getAllComboBox(getAllUIControls(currentTabWidget));
        for (int k = 0; k < listOfComboBox.count(); k++) {
          if (listOfComboBox.at(k)->objectName() == name) {
            orgComboBoxStyle = ui->cboxKernelArch->styleSheet();

            QComboBox* w = (QComboBox*)listOfComboBox.at(k);

            // QString style =
            // "QComboBox{border:none;background:rgb(255,0,0);color:rgb(255,255,255);}";
            // w->setStyleSheet(style);

            setPalette(w, QColor(255, 0, 0), Qt::white);
            end = true;
            break;
          }
        }
      }
    }

    if (!end) {
      for (int i = 0; i < ui->listMain->count(); i++) {
        if (end) break;

        ui->listMain->setCurrentRow(i);
        listOfComboBox.clear();
        listOfComboBox = getAllComboBox(getAllUIControls(currentMainTabWidget));
        for (int k = 0; k < listOfComboBox.count(); k++) {
          if (listOfComboBox.at(k)->objectName() == name) {
            orgComboBoxStyle = ui->cboxKernelArch->styleSheet();
            QComboBox* w = (QComboBox*)listOfComboBox.at(k);

            // w->setStyleSheet(style);
            // QString style =
            // "QComboBox{border:none;background-color:rgba(255,0,0,255);color:rgb(255,255,255);}";

            setPalette(w, QColor(255, 0, 0), Qt::white);

            end = true;
            break;
          }
        }
      }
    }
  }
}

void MainWindow::goResultsTableHeader(QString objName) {
  // table header 6
  bool end = false;
  QString name = objName.mid(1, objName.length() - 1);

  if (objName.mid(0, 1) == "6") {
    for (int i = 0; i < ui->listMain->count(); i++) {
      if (end) break;

      ui->listMain->setCurrentRow(i);
      for (int j = 0; j < ui->listSub->count(); j++) {
        if (end) break;

        ui->listSub->setCurrentRow(j);
        currentTabWidget = getSubTabWidget(i, j);
        listOfTableWidget.clear();
        listOfTableWidget =
            getAllTableWidget(getAllUIControls(currentTabWidget));
        for (int k = 0; k < listOfTableWidget.count(); k++) {
          if (listOfTableWidget.at(k)->objectName() == name) {
            QTableWidget* w = (QTableWidget*)listOfTableWidget.at(k);

            for (int x = 0; x < w->columnCount(); x++) {
              QString strColumn = w->horizontalHeaderItem(x)->text();
              if (strColumn == ui->listFind->currentItem()->text()) {
                w->clearSelection();

                brushTableHeaderBackground =
                    w->horizontalHeaderItem(x)->background();
                brushTableHeaderForeground =
                    w->horizontalHeaderItem(x)->foreground();

                if (!win && !osx1012) {
                  w->horizontalHeaderItem(x)->setBackground(QColor(255, 0, 0));
                  w->horizontalHeaderItem(x)->setForeground(
                      QColor(255, 255, 255));

                } else {
                  w->horizontalHeaderItem(x)->setForeground(QColor(255, 0, 0));
                }

                if (w->rowCount() == 0) {
                } else
                  w->setCurrentCell(0, x);

                if (!win) w->clearSelection();

                end = true;
              }
            }

            break;
          }
        }
      }
    }
  }
}

void MainWindow::goResults(int index) {
  find = true;

  QString objName = listNameResults.at(index);

  QPalette pal = this->palette();
  QBrush brush = pal.window();
  red = brush.color().red();

  clearCheckBoxMarker();
  clearLabelMarker();
  clearLineEditMarker();
  clearComboBoxMarker();
  clearTableHeaderMarker();

  removeAllLineEdit();

  goResultsCheckbox(objName);

  goResultsTable(objName, index);

  goResultsLabel(objName);

  goResultsLineEdit(objName);

  goResultsComboBox(objName);

  goResultsTableHeader(objName);

  // listMain and listSub 7
  if (objName.mid(0, 1) == "7") {
    QStringList strList = objName.split("-");
    if (strList.count() == 3) {
      int m = strList.at(1).toInt();
      int s = strList.at(2).toInt();
      ui->listMain->setCurrentRow(m);
      ui->listSub->setCurrentRow(s);
    }
  }

  ui->lblCount->setText(QString::number(findCount) + " ( " +
                        QString::number(ui->listFind->currentRow() + 1) +
                        " ) ");

  find = false;
}

void MainWindow::on_mycboxFind_currentTextChanged(const QString& arg1) {
  if (AddCboxFindItem) {
    return;
  }

  FindTextChange = true;

  if (arg1.trimmed() == "") {
    ui->lblCount->setText("0");
    listNameResults.clear();
    ui->listFind->setHidden(true);

    loading = true;
    ui->listFind->clear();
    loading = false;

    clearCheckBoxMarker();
    clearComboBoxMarker();
    clearLabelMarker();
    clearLineEditMarker();
    clearTableHeaderMarker();

    QPalette pal = this->palette();
    QBrush brush = pal.window();
    red = brush.color().red();

    Method::setComboBoxQss(ui->mycboxFind, 6, 1, "#C0C0C0", "#4169E1");
  }
}

void MainWindow::clearCheckBoxMarker() {
  for (int i = 0; i < listOfCheckBoxResults.count(); i++) {
    QCheckBox* w = (QCheckBox*)listOfCheckBoxResults.at(i);
    w->setStyleSheet(orgCheckBoxStyle);
  }
}

void MainWindow::clearLabelMarker() {
  for (int i = 0; i < listOfLabelResults.count(); i++) {
    QLabel* w = (QLabel*)listOfLabelResults.at(i);
    w->setStyleSheet(orgLabelStyle);
  }
}

void MainWindow::clearComboBoxMarker() {
  for (int i = 0; i < listOfComboBoxResults.count(); i++) {
    QComboBox* w = (QComboBox*)listOfComboBoxResults.at(i);

    if (red < 55) {
      setPalette(w, QColor(50, 50, 50), Qt::white);

    } else {
      setPalette(w, Qt::white, Qt::black);
    }
  }
}

void MainWindow::clearLineEditMarker() {
  for (int i = 0; i < listOfLineEditResults.count(); i++) {
    QLineEdit* w = (QLineEdit*)listOfLineEditResults.at(i);
    w->setStyleSheet(orgLineEditStyle);
  }
}

void MainWindow::clearTableHeaderMarker() {
  for (int i = 0; i < listOfTableWidgetHeaderResults.count(); i++) {
    QTableWidget* w = (QTableWidget*)listOfTableWidgetHeaderResults.at(i);

    for (int j = 0; j < w->columnCount(); j++) {
      if (!osx1012) {
        w->horizontalHeaderItem(j)->setBackground(brushTableHeaderBackground);
        w->horizontalHeaderItem(j)->setForeground(brushTableHeaderForeground);
      } else {
        w->horizontalHeaderItem(j)->setForeground(QColor(0, 0, 0));
      }
    }
  }
}

void MainWindow::on_listFind_currentRowChanged(int currentRow) {
  Q_UNUSED(currentRow);

  if (!loading) {
    goResults(currentRow);
  }
}

void MainWindow::on_listFind_itemClicked(QListWidgetItem* item) {
  Q_UNUSED(item);
  if (ui->listFind->currentRow() >= 0) goResults(ui->listFind->currentRow());
}

void MainWindow::acpi_cellDoubleClicked() {
  QTableWidget* t;
  int row, col;

  // ACPI
  t = ui->table_acpi_add;
  if (t->hasFocus()) {
    row = t->currentRow();
    col = t->currentColumn();
    on_table_acpi_add_cellDoubleClicked(row, col);
  }

  t = ui->table_acpi_del;
  if (t->hasFocus()) {
    row = t->currentRow();
    col = t->currentColumn();
    on_table_acpi_del_cellDoubleClicked(row, col);
  }

  t = ui->table_acpi_patch;
  if (t->hasFocus()) {
    row = t->currentRow();
    col = t->currentColumn();
    on_table_acpi_patch_cellDoubleClicked(row, col);
  }
}

void MainWindow::booter_cellDoubleClicked() {
  QTableWidget* t;
  int row, col;

  // Booter
  t = ui->table_booter;
  if (t->hasFocus()) {
    row = t->currentRow();
    col = t->currentColumn();
    on_table_booter_cellDoubleClicked(row, col);
  }

  t = ui->table_Booter_patch;
  if (t->hasFocus()) {
    row = t->currentRow();
    col = t->currentColumn();
    on_table_Booter_patch_cellDoubleClicked(row, col);
  }
}

void MainWindow::dp_cellDoubleClicked() {
  QTableWidget* t;
  int row, col;

  // DP
  t = ui->table_dp_add0;
  if (t->hasFocus() && t->currentIndex().isValid()) {
    row = t->currentRow();
    col = t->currentColumn();
    on_table_dp_add0_cellDoubleClicked(row, col);
  }

  t = ui->table_dp_add;
  if (t->hasFocus() && t->currentIndex().isValid()) {
    row = t->currentRow();
    col = t->currentColumn();
    on_table_dp_add_cellDoubleClicked(row, col);
  }

  t = ui->table_dp_del0;
  if (t->hasFocus() && t->currentIndex().isValid()) {
    row = t->currentRow();
    col = t->currentColumn();
    on_table_dp_del0_cellDoubleClicked(row, col);
  }

  t = ui->table_dp_del;
  if (t->hasFocus() && t->currentIndex().isValid()) {
    row = t->currentRow();
    col = t->currentColumn();
    on_table_dp_del_cellDoubleClicked(row, col);
  }
}

void MainWindow::kernel_cellDoubleClicked() {
  QTableWidget* t;
  int row, col;

  // Kernel
  t = ui->table_kernel_add;
  if (t->hasFocus()) {
    row = t->currentRow();
    col = t->currentColumn();
    on_table_kernel_add_cellDoubleClicked(row, col);
  }

  t = ui->table_kernel_block;
  if (t->hasFocus()) {
    row = t->currentRow();
    col = t->currentColumn();
    on_table_kernel_block_cellDoubleClicked(row, col);
  }

  t = ui->table_kernel_Force;
  if (t->hasFocus()) {
    row = t->currentRow();
    col = t->currentColumn();
    on_table_kernel_Force_cellDoubleClicked(row, col);
  }

  t = ui->table_kernel_patch;
  if (t->hasFocus()) {
    row = t->currentRow();
    col = t->currentColumn();
    on_table_kernel_patch_cellDoubleClicked(row, col);
  }
}

void MainWindow::misc_cellDoubleClicked() {
  QTableWidget* t;
  int row, col;

  // Misc
  t = ui->tableBlessOverride;
  if (t->hasFocus()) {
    row = t->currentRow();
    col = t->currentColumn();
    on_tableBlessOverride_cellDoubleClicked(row, col);
  }

  t = ui->tableEntries;
  if (t->hasFocus()) {
    row = t->currentRow();
    col = t->currentColumn();
    on_tableEntries_cellDoubleClicked(row, col);
  }

  t = ui->tableTools;
  if (t->hasFocus()) {
    row = t->currentRow();
    col = t->currentColumn();
    on_tableTools_cellDoubleClicked(row, col);
  }
}

void MainWindow::nvram_cellDoubleClicked() {
  QTableWidget* t;
  int row, col;

  // NVRAM
  t = ui->table_nv_add0;
  if (t->hasFocus() && t->currentIndex().isValid()) {
    row = t->currentRow();
    col = t->currentColumn();
    on_table_nv_add0_cellDoubleClicked(row, col);
  }

  t = ui->table_nv_add;
  if (t->hasFocus() && t->currentIndex().isValid()) {
    row = t->currentRow();
    col = t->currentColumn();
    on_table_nv_add_cellDoubleClicked(row, col);
  }

  t = ui->table_nv_del0;
  if (t->hasFocus() && t->currentIndex().isValid()) {
    row = t->currentRow();
    col = t->currentColumn();
    on_table_nv_del0_cellDoubleClicked(row, col);
  }

  t = ui->table_nv_del;
  if (t->hasFocus() && t->currentIndex().isValid()) {
    row = t->currentRow();
    col = t->currentColumn();
    on_table_nv_del_cellDoubleClicked(row, col);
  }

  t = ui->table_nv_ls0;
  if (t->hasFocus() && t->currentIndex().isValid()) {
    row = t->currentRow();
    col = t->currentColumn();
    on_table_nv_ls0_cellDoubleClicked(row, col);
  }

  t = ui->table_nv_ls;
  if (t->hasFocus() && t->currentIndex().isValid()) {
    row = t->currentRow();
    col = t->currentColumn();
    on_table_nv_ls_cellDoubleClicked(row, col);
  }
}

void MainWindow::pi_cellDoubleClicked() {
  QTableWidget* t;
  int row, col;

  // PI
  t = ui->tableDevices;
  if (t->hasFocus()) {
    row = t->currentRow();
    col = t->currentColumn();
    on_tableDevices_cellDoubleClicked(row, col);
  }
}

void MainWindow::uefi_cellDoubleClicked() {
  QTableWidget* t;
  int row, col;

  // UEFI
  t = ui->table_uefi_drivers;
  if (t->hasFocus()) {
    row = t->currentRow();
    col = t->currentColumn();
    on_table_uefi_drivers_cellDoubleClicked(row, col);
  }

  t = ui->table_uefi_ReservedMemory;
  if (t->hasFocus()) {
    row = t->currentRow();
    col = t->currentColumn();
    on_table_uefi_ReservedMemory_cellDoubleClicked(row, col);
  }

  t = ui->table_uefi_Unload;
  if (t->hasFocus()) {
    row = t->currentRow();
    col = t->currentColumn();
    on_table_uefi_Unload_cellDoubleClicked(row, col);
  }
}

void MainWindow::EnterPress() {
  if (!lineEditEnter) {
    acpi_cellDoubleClicked();

    booter_cellDoubleClicked();

    dp_cellDoubleClicked();

    kernel_cellDoubleClicked();

    misc_cellDoubleClicked();

    nvram_cellDoubleClicked();

    pi_cellDoubleClicked();

    uefi_cellDoubleClicked();
  }

  lineEditEnter = false;
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
  switch (event->key()) {
    case Qt::Key_Escape:

      myTable->removeCellWidget(myTable->currentRow(),
                                myTable->currentColumn());
      myTable->setFocus();
      InitEdit = false;

      break;

    case Qt::Key_Return:
      EnterPress();
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

void MainWindow::keyReleaseEvent(QKeyEvent* event) {
  if (event->key() == Qt::Key_Up) {
  }
}

void MainWindow::init_setWindowModified() {
  // CheckBox
  listOfCheckBox.clear();
  listOfCheckBox = getAllCheckBox(getAllUIControls(ui->tabTotal));
  for (int i = 0; i < listOfCheckBox.count(); i++) {
    QCheckBox* w = (QCheckBox*)listOfCheckBox.at(i);

    connect(w, &QCheckBox::stateChanged, this, &MainWindow::setWM);
  }

  // LineEdit
  listOfLineEdit.clear();
  listOfLineEdit = getAllLineEdit(getAllUIControls(ui->tabTotal));
  for (int i = 0; i < listOfLineEdit.count(); i++) {
    QLineEdit* w = (QLineEdit*)listOfLineEdit.at(i);
    if (w != lineEdit)
      connect(w, &QLineEdit::textChanged, this, &MainWindow::setWM);
  }

  // ComboBox
  listOfComboBox.clear();
  listOfComboBox = getAllComboBox(getAllUIControls(ui->tabTotal));
  for (int i = 0; i < listOfComboBox.count(); i++) {
    QComboBox* w = (QComboBox*)listOfComboBox.at(i);
    if (w != ui->comboBoxACPI && w != ui->comboBoxBooter &&
        w != ui->comboBoxKernel && w != ui->comboBoxUEFI)
      connect(w, &QComboBox::currentTextChanged, this, &MainWindow::setWM);
  }

  // Table
  listOfTableWidget.clear();
  listOfTableWidget = getAllTableWidget(getAllUIControls(ui->tabTotal));
  for (int i = 0; i < listOfTableWidget.count(); i++) {
    QTableWidget* w = (QTableWidget*)listOfTableWidget.at(i);

    connect(w, &QTableWidget::itemChanged, this, &MainWindow::setWM_RightTable);
  }
}

void MainWindow::setWM_RightTable() {
  if (!LoadRightTable) {
    this->setWindowModified(true);
    updateIconStatus();
  }
}

void MainWindow::setWM() {
  this->setWindowModified(true);
  updateIconStatus();
}

QString MainWindow::getReReCount(QTableWidget* w, QString text) {
  bool re = false;
  int reCount = 0;

  if (w->rowCount() > 0) {
    for (int m = 0; m < tableList0.count(); m++) {
      if (w == tableList0.at(m)) {
        for (int k = 0; k < w->rowCount(); k++) {
          if (w->item(k, 0)->text().trimmed().contains(text)) {
            re = true;
            reCount++;
          }
        }

        break;
      }
    }
  }

  return QString::number(re) + "-" + QString::number(reCount);
}

void MainWindow::pasteLine(QTableWidget* w, QAction* pasteAction) {
  connect(pasteAction, &QAction::triggered, [=]() {
    QString name = w->objectName();
    QString qfile = strConfigPath + name + ".ini";
    QSettings Reg(qfile, QSettings::IniFormat);

    QFile file(qfile);

    if (file.exists()) {
      int rowCount = Reg.value("rowCount").toInt();
      for (int i = rowCount - 1; i > -1; i--) {
        loading = true;

        QString text =
            Reg.value(QString::number(i) + "/col" + QString::number(0))
                .toString()
                .trimmed();
        QString oldColText0 = text;
        int row = 0;
        if (w->rowCount() > 0) row = w->currentRow();

        QStringList strList = getReReCount(w, text).split("-");
        bool re = strList.at(0).toInt();
        int reCount = strList.at(1).toInt();

        if (w->rowCount() > 0) w->setCurrentCell(row, 0);

        QStringList colTextList;
        for (int j = 0; j < w->columnCount(); j++) {
          text = Reg.value(QString::number(i) + "/col" + QString::number(j))
                     .toString()
                     .trimmed();

          if (re) {
            text = text + "-" + QString::number(reCount);
          }

          colTextList.append(text);
        }

        bool writeini = false;
        bool writevalueini = false;
        int leftTableCurrentRow = 0;
        for (int m = 0; m < tableList.count(); m++) {
          if (w == tableList.at(m)) {
            if (m == 0 || m == 2)
              writeini = true;
            else
              writevalueini = true;
            leftTableCurrentRow = getLetfTableCurrentRow(w);
            break;
          }
        }

        // Undo / Redo
        QString infoStr;
        for (int x = 0; x < colTextList.count(); x++) {
          infoStr = infoStr + "  [" + colTextList.at(x) + "]";
        }

        QString infoText = QString::number(row + 1) + "  " + infoStr;
        QUndoCommand* pastelineCommand = new CopyPasteLineCommand(
            w, row, 0, infoText, colTextList, oldColText0, writeini,
            writevalueini, leftTableCurrentRow);
        undoStack->push(pastelineCommand);

        loading = false;
      }
    }
    file.close();
  });
}

void MainWindow::copyLine(QTableWidget* w, QAction* copyAction) {
  connect(copyAction, &QAction::triggered, [=]() {
    if (w->rowCount() == 0) return;

    QString name = w->objectName();
    QString qfile = strConfigPath + name + ".ini";

    QItemSelectionModel* selections =
        w->selectionModel();  // 返回当前的选择模式
    QModelIndexList selectedsList =
        selections->selectedIndexes();  // 返回所有选定的模型项目索引列表

    QSettings Reg(qfile, QSettings::IniFormat);
    Reg.setValue("rowCount", selectedsList.count());
    Reg.setValue("CurrentDateTime", CurrentDateTime);

    for (int i = 0; i < selectedsList.count(); i++) {
      int curRow = selectedsList.at(i).row();
      w->setCurrentCell(curRow, 0);

      for (int j = 0; j < w->columnCount(); j++) {
        Reg.setValue(QString::number(i) + "/col" + QString::number(j),
                     w->item(w->currentRow(), j)->text());
      }

      if (w == ui->table_dp_add0)
        write_ini(ui->table_dp_add0, ui->table_dp_add, curRow);
      if (w == ui->table_nv_add0)
        write_ini(ui->table_nv_add0, ui->table_nv_add, curRow);
      if (w == ui->table_nv_del0)
        write_value_ini(ui->table_nv_del0, ui->table_nv_del, curRow);
      if (w == ui->table_nv_ls0)
        write_value_ini(ui->table_nv_ls0, ui->table_nv_ls, curRow);
      if (w == ui->table_dp_del0)
        write_value_ini(ui->table_dp_del0, ui->table_dp_del, curRow);
    }
  });
}

void MainWindow::cutLine(QTableWidget* w, QAction* cutAction,
                         QAction* copyAction) {
  connect(cutAction, &QAction::triggered,
          [=]() {
            QItemSelectionModel* selections = w->selectionModel();
            QModelIndexList selectedsList = selections->selectedIndexes();

            copyAction->triggered(true);

            w->clearSelection();
            w->setSelectionMode(QAbstractItemView::MultiSelection);
            for (int z = 0; z < selectedsList.count(); z++) {
              w->selectRow(selectedsList.at(z).row());
            }

            if (w == ui->table_dp_add)
              on_btnDPAdd_Del_clicked();

            else if (w == ui->table_dp_del)
              on_btnDPDel_Del_clicked();

            else if (w == ui->table_nv_add)
              on_btnNVRAMAdd_Del_clicked();

            else if (w == ui->table_nv_del)
              on_btnNVRAMDel_Del_clicked();

            else if (w == ui->table_nv_ls)
              on_btnNVRAMLS_Del_clicked();

            else
              del_item(w);

            w->setSelectionMode(QAbstractItemView::ExtendedSelection);
          }

  );
}

void MainWindow::setPopMenuEnabled(QString qfile, QTableWidget* w,
                                   QAction* cutAction, QAction* pasteAction,
                                   QAction* copyAction) {
  if (w == ui->table_dp_add0 || w == ui->table_dp_del0 ||
      w == ui->table_nv_add0 || w == ui->table_nv_del0 ||
      w == ui->table_nv_ls0) {
    QSettings Reg(qfile, QSettings::IniFormat);
    QString text = Reg.value("0/col" + QString::number(0)).toString().trimmed();

    text = text.replace("/", "-");
    QString oldRightTable = Reg.value("CurrentDateTime").toString() +
                            w->objectName() + text + ".ini";
    QFileInfo fi(strConfigPath + oldRightTable);

    if (!fi.exists())
      pasteAction->setEnabled(false);
    else {
      copyAction->setEnabled(true);
      cutAction->setEnabled(true);
    }
  } else {
    if (w->rowCount() > 0) {
      copyAction->setEnabled(true);
      cutAction->setEnabled(true);
    }
  }
}

void MainWindow::setPopMenuEnabledForDP_NVRAM(QTableWidget* w,
                                              QAction* pasteAction) {
  for (int i = 0; i < tableList.count(); i++) {
    if (w == tableList.at(i)) {
      if (tableList0.at(i)->rowCount() > 0)
        pasteAction->setEnabled(true);
      else
        pasteAction->setEnabled(false);
    }
  }
}

void MainWindow::tablePopMenu(QTableWidget* w, QAction* cutAction,
                              QAction* copyAction, QAction* pasteAction,
                              QAction* showtipAction, QMenu* popMenu) {
  connect(w, &QTableWidget::customContextMenuRequested, [=](const QPoint& pos) {
    Q_UNUSED(pos);

    QString name = w->objectName();

    QString qfile = strConfigPath + name + ".ini";
    QFile file(qfile);
    if (file.exists()) {
      pasteAction->setEnabled(true);

      setPopMenuEnabled(qfile, w, cutAction, pasteAction, copyAction);

      setPopMenuEnabledForDP_NVRAM(w, pasteAction);

    } else
      pasteAction->setEnabled(false);

    if (w->rowCount() == 0) {
      copyAction->setEnabled(false);
      cutAction->setEnabled(false);
    } else {
      copyAction->setEnabled(true);
      cutAction->setEnabled(true);
    }

    if (w->toolTip().trimmed() == "")
      showtipAction->setVisible(false);
    else
      showtipAction->setVisible(true);

    popMenu->exec(QCursor::pos());
  });
}

void MainWindow::set_AutoColWidth(QTableWidget* w, bool autoColWidth) {
  QStringList keyList;
  keyList = QStringList() << "Enabled"
                          << "Load"
                          << "Arch"
                          << "All"
                          << "Type"
                          << "Data Type"
                          << "TextMode"
                          << "Auxiliary"
                          << "RealPath"
                          << "Strategy";

  if (autoColWidth) {
    for (int y = 0; y < w->columnCount(); y++) {
      QString item = w->horizontalHeaderItem(y)->text();
      if (!keyList.removeOne(item)) {
        w->horizontalHeader()->setSectionResizeMode(
            y, QHeaderView::ResizeToContents);
      }
    }
  } else {
    w->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
  }
}

void MainWindow::init_CopyPasteLine() {
  listOfTableWidget.clear();
  listOfTableWidget = getAllTableWidget(getAllUIControls(ui->tabTotal));
  for (int i = 0; i < listOfTableWidget.count(); i++) {
    QTableWidget* w = (QTableWidget*)listOfTableWidget.at(i);
    if (w->objectName().mid(0, 5) != "table") {
      listOfTableWidget.removeOne(w);
      i--;
    }
  }
  for (int i = 0; i < listOfTableWidget.count(); i++) {
    QTableWidget* w = (QTableWidget*)listOfTableWidget.at(i);

    // Auto Col Width

    bool isAutoColWidth =
        Reg.value(w->objectName() + "AutoColWidth", true).toBool();
    set_AutoColWidth(w, isAutoColWidth);

    w->setContextMenuPolicy(Qt::CustomContextMenu);
    w->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
    w->installEventFilter(this);

    // 拷贝当前列表头文本
    QAction* copyTableHeaderTextAction = new QAction(tr("CopyText"));
    QMenu* popTableHeaderMenu = new QMenu(this);
    popTableHeaderMenu->addAction(copyTableHeaderTextAction);
    connect(copyTableHeaderTextAction, &QAction::triggered, [=]() {
      if (w->rowCount() > 0) {
        QString str1 = w->horizontalHeaderItem(w->currentColumn())->text();

        QClipboard* clipboard = QApplication::clipboard();
        clipboard->setText(str1.trimmed());
      }
    });
    connect(w->horizontalHeader(), &QTableWidget::customContextMenuRequested,
            [=](const QPoint& pos) {
              Q_UNUSED(pos);
              if (w->rowCount() > 0) popTableHeaderMenu->exec(QCursor::pos());
            });

    cutAction = new QAction(tr("Cut Line"));
    copyAction = new QAction(tr("Copy Line"));
    pasteAction = new QAction(tr("Paste Line"));
    QAction* showtipAction = new QAction(tr("Show Tips"));
    QAction* add = new QAction(tr("Add Item"));
    QAction* del = new QAction(tr("Delete Item"));
    QAction* up = new QAction(tr("Move Up"));
    QAction* down = new QAction(tr("Move Down"));
    QAction* preset = new QAction(tr("Preset"));
    QAction* browdatabase = new QAction(tr("Browse Database"));
    QAction* bootargs = new QAction(tr("Add boot-args"));
    QAction* autoColWidth = new QAction(tr("Auto Column Width"));
    QAction* actCopyLineToText = new QAction(tr("Copy Line to Text"));
    autoColWidth->setCheckable(true);
    autoColWidth->setChecked(isAutoColWidth);
    QMenu* popMenu = new QMenu(this);

    popMenu->addAction(up);
    popMenu->addAction(down);
    popMenu->addAction(add);
    popMenu->addAction(del);
    popMenu->addAction(preset);
    popMenu->addAction(browdatabase);
    popMenu->addAction(bootargs);
    popMenu->addSeparator();
    popMenu->addAction(cutAction);
    popMenu->addAction(copyAction);
    popMenu->addAction(pasteAction);
    popMenu->addSeparator();
    popMenu->addAction(autoColWidth);
    popMenu->addSeparator();
    popMenu->addAction(actCopyLineToText);
    popMenu->addSeparator();
    popMenu->addAction(showtipAction);

    if (w == ui->table_nv_add0 || w == ui->table_nv_add ||
        w == ui->table_nv_del0 || w == ui->table_nv_del ||
        w == ui->table_nv_ls0 || w == ui->table_nv_ls ||
        w == ui->table_dp_add0 || w == ui->table_dp_add ||
        w == ui->table_dp_del0 || w == ui->table_dp_del) {
      up->setVisible(false);
      down->setVisible(false);
    }

    if (w != ui->table_acpi_patch && w != ui->table_kernel_patch &&
        w != ui->table_nv_add0 && w != ui->table_nv_del0 &&
        w != ui->table_nv_ls0 && w != ui->table_kernel_add) {
      preset->setVisible(false);
    }

    if (w != ui->table_nv_add) bootargs->setVisible(false);

    if (w != ui->table_acpi_add && w != ui->table_kernel_add &&
        w != ui->tableTools && w != ui->table_uefi_drivers) {
      browdatabase->setVisible(false);
    }

    // Browse Database
    connect(browdatabase, &QAction::triggered, [=]() {
      if (w == ui->table_acpi_add) ui->btnOpenACPIDir->click();
      if (w == ui->table_kernel_add) ui->btnOpenKextDir->click();
      if (w == ui->tableTools) ui->btnOpenToolsDir->click();
      if (w == ui->table_uefi_drivers) ui->btnOpenDriversDir->click();
    });

    // boot-args
    connect(bootargs, &QAction::triggered, [=]() {
      if (w == ui->table_nv_add) ui->btnAddbootArgs->click();
    });

    // Up
    connect(up, &QAction::triggered, [=]() { on_actionMove_Up_triggered(); });

    // Down
    connect(down, &QAction::triggered,
            [=]() { on_actionMove_Down_triggered(); });

    // Add
    connect(add, &QAction::triggered, [=]() { on_actionAdd_triggered(); });

    // Del
    connect(del, &QAction::triggered, [=]() { on_actionDelete_triggered(); });

    // Preset
    connect(preset, &QAction::triggered, [=]() {
      if (w == ui->table_acpi_patch) ui->btnACPIPatch->click();

      if (w == ui->table_kernel_add) ui->btnKextPreset->click();

      if (w == ui->table_kernel_patch) ui->btnPresetKernelPatch->click();

      if (w == ui->table_nv_add0) ui->btnPresetNVAdd->click();

      if (w == ui->table_nv_del0) ui->btnPresetNVDelete->click();

      if (w == ui->table_nv_ls0) ui->btnPresetNVLegacy->click();
    });

    // Auto Col Width
    connect(autoColWidth, &QAction::triggered, [=]() {
      bool isAutoColWidth = autoColWidth->isChecked();
      Reg.setValue(w->objectName() + "AutoColWidth", isAutoColWidth);
      set_AutoColWidth(w, isAutoColWidth);
    });

    // Copy Lint to Text
    connect(actCopyLineToText, &QAction::triggered, [=]() {
      if (!w->currentIndex().isValid()) return;
      int row = w->currentRow();
      if (row < 0) return;
      QClipboard* clipboard = QApplication::clipboard();
      QString newText;

      for (int x = 0; x < w->columnCount(); x++) {
        newText = newText + " | " + w->item(row, x)->text().trimmed();
      }
      newText = newText + " | ";
      clipboard->setText(newText);
    });

    // Show Tip
    connect(showtipAction, &QAction::triggered,
            [=]() { myToolTip->popup(QCursor::pos(), "", w->toolTip()); });

    // 剪切行
    cutLine(w, cutAction, copyAction);

    // 复制行
    copyLine(w, copyAction);

    // 粘贴行
    pasteLine(w, pasteAction);

    tablePopMenu(w, cutAction, copyAction, pasteAction, showtipAction, popMenu);
  }
}

void MainWindow::loadRightTable(QTableWidget* t0, QTableWidget* t) {
  if (!t0->currentIndex().isValid()) return;
  bool md = this->isWindowModified();
  if (!loading) {
    loading = true;
    LoadRightTable = true;
    read_ini(t0, t, t0->currentRow());
    LoadRightTable = false;
    loading = false;
    setStatusBarText(t0);
  }
  this->setWindowModified(md);
  updateIconStatus();
}

void MainWindow::endPasteLine(QTableWidget* w, int row, QString colText0) {
  w->setCurrentCell(row, 0);

  if (w == ui->table_dp_add0 || w == ui->table_dp_del0 ||
      w == ui->table_nv_add0 || w == ui->table_nv_del0 ||
      w == ui->table_nv_ls0) {
    QString name = w->objectName();
    QString qfile = strConfigPath + name + ".ini";
    QSettings Reg(qfile, QSettings::IniFormat);

    colText0 = colText0.replace("/", "-");
    QString oldRightTable = Reg.value("CurrentDateTime").toString() +
                            w->objectName() + colText0 + ".ini";

    QString newText = w->item(row, 0)->text().trimmed();
    newText = newText.replace("/", "-");
    QString newReghtTable =
        CurrentDateTime + w->objectName() + newText + ".ini";

    QFileInfo fi(strConfigPath + oldRightTable);
    if (fi.exists()) {
      QFile::copy(strConfigPath + oldRightTable, strConfigPath + newReghtTable);
      IniFile.push_back(strConfigPath + newReghtTable);
    }

    if (w == ui->table_dp_add0) {
      loading = false;
      loadRightTable(w, ui->table_dp_add);
    }

    if (w == ui->table_dp_del0) {
      loading = false;
      loadRightTable(w, ui->table_dp_del);
    }

    if (w == ui->table_nv_add0) {
      loading = false;
      loadRightTable(w, ui->table_nv_add);
    }

    if (w == ui->table_nv_del0) {
      loading = false;
      loadRightTable(w, ui->table_nv_del);
    }

    if (w == ui->table_nv_ls0) {
      loading = false;
      loadRightTable(w, ui->table_nv_ls);
    }

    if (w == ui->table_dp_add0)
      mymethod->writeLeftTable(ui->table_dp_add0, ui->table_dp_add);

    if (w == ui->table_nv_add0)
      mymethod->writeLeftTable(ui->table_nv_add0, ui->table_nv_add);

    if (w == ui->table_nv_del0)
      mymethod->writeLeftTableOnlyValue(ui->table_nv_del0, ui->table_nv_del);

    if (w == ui->table_nv_ls0)
      mymethod->writeLeftTableOnlyValue(ui->table_nv_ls0, ui->table_nv_ls);

    if (w == ui->table_dp_del0)
      mymethod->writeLeftTableOnlyValue(ui->table_dp_del0, ui->table_dp_del);
  }
}

void MainWindow::endDelLeftTable(QTableWidget* t0) {
  QTableWidget* t = NULL;

  if (t0 == ui->table_dp_add0 || t0 == ui->table_dp_del0 ||
      t0 == ui->table_nv_add0 || t0 == ui->table_nv_del0 ||
      t0 == ui->table_nv_ls0) {
    if (t0 == ui->table_dp_add0) t = ui->table_dp_add;

    if (t0 == ui->table_dp_del0) t = ui->table_dp_del;

    if (t0 == ui->table_nv_add0) t = ui->table_nv_add;

    if (t0 == ui->table_nv_del0) t = ui->table_nv_del;

    if (t0 == ui->table_nv_ls0) t = ui->table_nv_ls;

    if (t0->rowCount() == 0) {
      t->setRowCount(0);
    }

    if (t0->rowCount() > 0) {
      loadRightTable(t0, t);
    }
  }
}

QTableWidget* MainWindow::getLeftTable(QTableWidget* table) {
  for (int i = 0; i < tableList.count(); i++) {
    if (table == tableList.at(i)) {
      return tableList0.at(i);
    }
  }

  return NULL;
}

int MainWindow::getLetfTableCurrentRow(QTableWidget* table) {
  for (int i = 0; i < tableList.count(); i++) {
    if (table == tableList.at(i)) {
      return tableList0.at(i)->currentRow();
    }
  }

  return 0;
}

void MainWindow::clearAllTableSelection() {
  // Table
  listOfTableWidget.clear();
  listOfTableWidget = getAllTableWidget(getAllUIControls(ui->tabTotal));
  for (int i = 0; i < listOfTableWidget.count(); i++) {
    QTableWidget* w = (QTableWidget*)listOfTableWidget.at(i);

    w->clearSelection();
  }
}

void MainWindow::paintEvent(QPaintEvent* event) {
  Q_UNUSED(event);

  // 获取背景色
  QPalette pal = this->palette();
  QBrush brush = pal.window();
  int c_red = brush.color().red();

  if (c_red != red) {
    red = c_red;
    Method::setComboBoxQss(ui->mycboxFind, 6, 1, "#C0C0C0", "#4169E1");
    init_ToolBarIcon();
    Method::init_UIWidget(this, red);
    Method::init_UIWidget(myDlgPreference, red);
    Method::init_UIWidget(dlgSyncOC, red);
    Method::init_UIWidget(myDatabase, red);
    dlgSyncOC->init_ItemColor();
  }

  if (!One) {
    One = true;
    ui->listMain->setCurrentRow(0);
    on_listMain_itemSelectionChanged();
  }
}

bool MainWindow::eventFilter(QObject* obj, QEvent* event) {
  if (obj->metaObject()->className() == QStringLiteral("QLabel") ||
      obj->metaObject()->className() == QStringLiteral("QCheckBox") ||
      obj->metaObject()->className() == QStringLiteral("QTableWidget")) {
    if (event->type() == QEvent::ToolTip) {
      QToolTip::hideText();
      event->ignore();

      if (myDlgPreference->ui->chkHoverTips->isChecked()) {
        if (obj->metaObject()->className() == QStringLiteral("QLabel")) {
          QLabel* w = (QLabel*)obj;

          // myToolTip->popup(QCursor::pos(), w->text(), w->toolTip());
          mymethod->show_Tip(w->text(), w->toolTip());
        }

        if (obj->metaObject()->className() == QStringLiteral("QCheckBox")) {
          QCheckBox* w = (QCheckBox*)obj;
          // myToolTip->popup(QCursor::pos(), w->text(), w->toolTip());
          mymethod->show_Tip(w->text(), w->toolTip());
        }
      }

      return true;  // 不让事件继续传播
    } else if (event->type() == QEvent::MouseMove) {
    }
  }

  if (obj == lblVer) {
    if (event->type() == QEvent::MouseButtonPress)  // mouse button pressed
    {
      QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
      if (mouseEvent->button() == Qt::LeftButton) {
        if (blDEV) {
          QUrl url(
              dlgSyncOC->ui->editOCDevSource->lineEdit()->text().trimmed());
          QDesktopServices::openUrl(url);

        } else {
          QUrl url(strOCFrom);
          QDesktopServices::openUrl(url);
        }

        return true;
      } else {
        return false;
      }
    } else {
      return false;
    }
  } else {
    // pass the event on to the parent class
    return QMainWindow::eventFilter(obj, event);
  }

  return MainWindow::eventFilter(obj, event);
}

void MainWindow::on_actionBug_Report_triggered() {
  QUrl url(QString("https://github.com/ic005k/" + strAppName + "/issues"));
  QDesktopServices::openUrl(url);
}

void MainWindow::on_actionDiscussion_Forum_triggered() {
  QUrl url(
      QString("https://www.insanelymac.com/forum/topic/"
              "344752-open-source-cross-platform-opencore-auxiliary-tools/"));
  QDesktopServices::openUrl(url);
}

void MainWindow::getCheckBoxValue(QVariantMap map, QWidget* tab) {
  QObjectList listCheckBox;
  listCheckBox = getAllCheckBox(getAllUIControls(tab));
  for (int i = 0; i < listCheckBox.count(); i++) {
    QCheckBox* chkbox = (QCheckBox*)listCheckBox.at(i);
    QString strObjName = chkbox->objectName();
    QString name = strObjName.mid(3, strObjName.count() - 2);
    if (strObjName.mid(0, 3) == "chk") chkbox->setChecked(map[name].toBool());
  }
}

void MainWindow::getComboBoxValue(QVariantMap map, QWidget* tab) {
  QObjectList listComboBox;
  listComboBox = getAllComboBox(getAllUIControls(tab));
  for (int i = 0; i < listComboBox.count(); i++) {
    QComboBox* w = (QComboBox*)listComboBox.at(i);
    QString name = w->objectName().mid(4, w->objectName().count() - 3);

    QString cu_text = map[name].toString().trimmed();
    if (w != ui->mycboxFind) {
      if (cu_text != "")
        w->setCurrentText(cu_text);
      else
        w->setCurrentIndex(0);
    }
  }
}

void MainWindow::getEditValue(QVariantMap map, QWidget* tab) {
  QObjectList listLineEdit;
  listLineEdit = getAllLineEdit(getAllUIControls(tab));
  for (int i = 0; i < listLineEdit.count(); i++) {
    QLineEdit* w = (QLineEdit*)listLineEdit.at(i);

    QString str0, name;
    str0 = w->objectName().mid(4, w->objectName().count() - 3);  // 去edit

    if (str0.mid(0, 3) == "Dat" || str0.mid(0, 3) == "Int")
      name = str0.mid(3, str0.count() - 2);
    else
      name = str0;

    if (name != "") {
      QStringList strList = name.split("_");

      if (str0.mid(0, 3) == "Dat") {
        if (strList.count() > 0)
          w->setText(ByteToHexStr(map[strList.at(0)].toByteArray()));
        else
          w->setText(ByteToHexStr(map[name].toByteArray()));
      } else {
        if (strList.count() > 0) {
          QString t0 = map[strList.at(0)].toString();
          if (t0.length() > 0) w->setText(t0);
        } else {
          QString t1 = map[name].toString();
          if (t1.length() > 0) w->setText(t1);
        }
      }
    }
  }
}

void MainWindow::getValue(QVariantMap map, QWidget* tab) {
  getCheckBoxValue(map, tab);

  getEditValue(map, tab);

  getComboBoxValue(map, tab);
}

bool MainWindow::editExclusion(QLineEdit* w, QString name) {
  // 用name ！= “”过滤掉获取的ComBox里面的edit
  if (name != "" && w != ui->myeditPassInput && name != "pinbox_lineedit")
    return true;

  return false;
}

QVariantMap MainWindow::setEditValue(QVariantMap map, QWidget* tab) {
  // edit
  QObjectList listLineEdit;
  listLineEdit = getAllLineEdit(getAllUIControls(tab));
  for (int i = 0; i < listLineEdit.count(); i++) {
    QLineEdit* w = (QLineEdit*)listLineEdit.at(i);
    if (!w->isHidden() && w->objectName().mid(0, 4) == "edit") {
      QString str0, name;
      str0 = w->objectName().mid(4, w->objectName().count() - 3);  // 去edit

      if (str0.mid(0, 3) == "Dat" || str0.mid(0, 3) == "Int")
        name = str0.mid(3, str0.count() - 2);
      else
        name = str0;

      if (editExclusion(w, name)) {
        QStringList strList = name.split("_");

        if (str0.mid(0, 3) == "Dat") {
          if (strList.count() > 0)
            map.insert(strList.at(0), HexStrToByte(w->text().replace(" ", "")));
          else
            map.insert(name, HexStrToByte(w->text().replace(" ", "")));
        }

        else if (str0.mid(0, 3) == "Int") {
          if (strList.count() > 0)
            map.insert(strList.at(0), w->text().trimmed().toLongLong());
          else
            map.insert(name, w->text().trimmed().toLongLong());
        }

        else {
          if (strList.count() > 0)
            map.insert(strList.at(0), w->text().trimmed());
          else
            map.insert(name, w->text().trimmed());
        }
      }
    }
  }

  return map;
}

QVariantMap MainWindow::setCheckBoxValue(QVariantMap map, QWidget* tab) {
  // chk
  QObjectList listCheckBox;
  listCheckBox = getAllCheckBox(getAllUIControls(tab));
  for (int i = 0; i < listCheckBox.count(); i++) {
    QCheckBox* chkbox = (QCheckBox*)listCheckBox.at(i);
    QString strObjName = chkbox->objectName();
    if (!chkbox->isHidden() && strObjName.mid(0, 3) == "chk") {
      QString name = strObjName.mid(3, strObjName.count() - 2);
      map.insert(name, getChkBool(chkbox));
    }
  }

  return map;
}

QVariantMap MainWindow::setComboBoxValue(QVariantMap map, QWidget* tab) {
  // combobox
  QObjectList listComboBox;
  listComboBox = getAllComboBox(getAllUIControls(tab));
  for (int i = 0; i < listComboBox.count(); i++) {
    QComboBox* w = (QComboBox*)listComboBox.at(i);
    if (!w->isHidden() && w->objectName().mid(0, 4) == "cbox") {
      QString name = w->objectName().mid(4, w->objectName().count() - 3);

      if (w->objectName().mid(0, 4) == "cbox") {
        if (name != "SystemProductName") {
          map.insert(name, w->currentText().trimmed());

        } else
          map.insert(name, getSystemProductName(w->currentText().trimmed()));

        if (name == "SecureBootModel") {
          QString cStr = w->currentText().trimmed();

          if (cStr.contains("-")) {
            QStringList cList = cStr.split("-");
            cStr = cList.at(0);
          }

          map.insert(name, cStr);
        }
      }
    }
  }

  return map;
}

QVariantMap MainWindow::setValue(QVariantMap map, QWidget* tab) {
  map = setCheckBoxValue(map, tab);

  map = setEditValue(map, tab);

  map = setComboBoxValue(map, tab);

  return map;
}

void MainWindow::on_actionQuit_triggered() { this->close(); }

void MainWindow::on_actionUpgrade_OC_triggered() {
  dlgSyncOC->init_Sync_OC_Table();
  dlgSyncOC->resizeWindowsPos();
}

void MainWindow::initColorValue() {
  textColorInt.clear();
  textColorInt.append(0);
  textColorInt.append(1);
  textColorInt.append(2);
  textColorInt.append(3);
  textColorInt.append(4);
  textColorInt.append(5);
  textColorInt.append(6);
  textColorInt.append(7);
  textColorInt.append(8);
  textColorInt.append(9);
  textColorInt.append(10);
  textColorInt.append(11);
  textColorInt.append(12);
  textColorInt.append(13);
  textColorInt.append(14);
  textColorInt.append(15);

  backColorInt.clear();
  backColorInt.append(0);
  backColorInt.append(16);
  backColorInt.append(32);
  backColorInt.append(48);
  backColorInt.append(64);
  backColorInt.append(80);
  backColorInt.append(96);
  backColorInt.append(112);
}

void MainWindow::on_mycboxTextColor_currentIndexChanged(int index) {
  Q_UNUSED(index);

  initColorValue();

  int bcIndex = ui->mycboxBackColor->currentIndex();
  int tcIndex = ui->mycboxTextColor->currentIndex();

  int total = 0;
  if (bcIndex >= 0 && tcIndex >= 0)
    total = backColorInt.at(bcIndex) + textColorInt.at(tcIndex);

  ui->editIntConsoleAttributes->setText(QString::number(total));

  if (bcIndex >= 0 && tcIndex >= 0 && !Initialization) {
    QPalette pe;
    pe = ui->lblColorEffect->palette();
    pe.setColor(QPalette::Window, QColor(backColor.at(bcIndex)));
    ui->lblColorEffect->setAutoFillBackground(true);
    if (total != 0)
      pe.setColor(QPalette::WindowText, QColor(textColor.at(tcIndex)));
    else
      pe.setColor(QPalette::WindowText, Qt::white);
    ui->lblColorEffect->setPalette(pe);
  }
}

void MainWindow::on_mycboxBackColor_currentIndexChanged(int index) {
  on_mycboxTextColor_currentIndexChanged(index);
}

void MainWindow::on_editIntConsoleAttributes_textChanged(const QString& arg1) {
  int total = arg1.toInt();

  initColorValue();

  for (int i = 0; i < textColorInt.count(); i++) {
    for (int j = 0; j < backColorInt.count(); j++) {
      if (total == textColorInt.at(i) + backColorInt.at(j)) {
        ui->mycboxTextColor->setCurrentIndex(i);
        ui->mycboxBackColor->setCurrentIndex(j);
        break;
      }
    }
  }
}

void MainWindow::initTargetValue() {
  chk.clear();
  for (int i = 0; i < chk_Target.count(); i++) {
    chk.append(chk_Target.at(i));
  }

  v.clear();
  v.append(1);
  v.append(2);
  v.append(4);
  v.append(8);
  v.append(16);
  v.append(32);
  v.append(64);
}

void MainWindow::Target() {
  initTargetValue();
  int total = 0;

  for (int i = 0; i < chk.count(); i++) {
    if (chk.at(i)->isChecked()) {
      total = total + v.at(i);
    }
  }

  ui->editIntTarget->setText(QString::number(total));
}

void MainWindow::on_btnGetPassHash_clicked() {
  QString strPass = "";
  chkdataPassHash = new QProcess;

#ifdef Q_OS_WIN32
  if (!QFile(userDataBaseDir + "win/ocpasswordgen.exe").exists()) return;
  chkdataPassHash->start(userDataBaseDir + "win/ocpasswordgen.exe",
                         QStringList() << strPass);
#endif

#ifdef Q_OS_LINUX
  if (!QFile(userDataBaseDir + "linux/ocpasswordgen").exists()) return;
  chkdataPassHash->start(userDataBaseDir + "linux/ocpasswordgen",
                         QStringList() << strPass);
#endif

#ifdef Q_OS_MAC
  if (!QFile(userDataBaseDir + "mac/ocpasswordgen").exists()) return;
  chkdataPassHash->start(userDataBaseDir + "mac/ocpasswordgen", QStringList()
                                                                    << strPass);
#endif

  ui->btnGetPassHash->setEnabled(false);
  this->repaint();
  ui->progressBar->setMaximum(0);
  chkdataPassHash->waitForStarted();
  QString strData = ui->myeditPassInput->text().trimmed() + "\n";
  const char* cstr;  // = strData.toLocal8Bit().constData();
  strData = strData.toLocal8Bit();
  string strStd = strData.toStdString();
  cstr = strStd.c_str();

  chkdataPassHash->write(cstr);

  connect(chkdataPassHash, SIGNAL(finished(int)), this,
          SLOT(readResultPassHash()));
}

void MainWindow::readResultPassHash() {
  QString result = chkdataPassHash->readAll();

  QStringList strList = result.split("\n");

  QStringList strHashList, strSaltList;
  strHashList = strList.at(1).split(":");
  strSaltList = strList.at(2).split(":");

  QString strHash, strSalt;
  strHash = strHashList.at(1);
  strHash = strHash.replace("<", "");
  strHash = strHash.replace(">", "").trimmed();

  strSalt = strSaltList.at(1);
  strSalt = strSalt.replace("<", "");
  strSalt = strSalt.replace(">", "").trimmed();

  ui->editDatPasswordHash->setText(strHash);
  ui->editDatPasswordSalt->setText(strSalt);

  ui->progressBar->setMaximum(100);
  ui->btnGetPassHash->setEnabled(true);
  this->repaint();
}

void MainWindow::on_toolButton_clicked() {
  if (ui->toolButton->text() == tr("Select date")) {
    ui->calendarWidget->setVisible(true);
    ui->toolButton->setText(tr("Hide"));
  } else if (ui->toolButton->text() == tr("Hide")) {
    ui->calendarWidget->setVisible(false);
    ui->toolButton->setText(tr("Select date"));
  }
}

void MainWindow::on_calendarWidget_selectionChanged() {
  QString y, m, d;
  y = QString::number(ui->calendarWidget->selectedDate().year());
  m = QString::number(ui->calendarWidget->selectedDate().month());
  d = QString::number(ui->calendarWidget->selectedDate().day());

  if (m.count() == 1) m = "0" + m;

  if (d.count() == 1) d = "0" + d;

  QString str = y + m + d;
  ui->editIntMinDate->setText(str);
}

void MainWindow::on_btnROM_clicked() {
  QUuid id = QUuid::createUuid();
  QString strTemp = id.toString();
  QString strId = strTemp.mid(1, strTemp.count() - 2).toUpper();
  QStringList strList = strId.split("-");
  ui->editDatROM->setText(strList.at(4));

  ui->editDatROM_PNVRAM->setText(ui->editDatROM->text());
}

void MainWindow::on_myeditPassInput_textChanged(const QString& arg1) {
  if (ui->progressBar->maximum() == 0) return;

  if (arg1.trimmed().count() > 0) {
    ui->btnGetPassHash->setEnabled(true);
    this->repaint();
  } else {
    ui->btnGetPassHash->setEnabled(false);
    this->repaint();
  }
}

void MainWindow::on_myeditPassInput_returnPressed() {
  if (ui->btnGetPassHash->isEnabled()) on_btnGetPassHash_clicked();
}

void MainWindow::on_actionDatabase_triggered() {
  QString url =
      "https://github.com/5T33Z0/OC-Little-Translated/tree/main/F_Desktop_EFIs/"
      "Config_Templates";
  QString txt = "<a href=\"" + url + "\"" + "> " +
                tr(" Intel CPU configuration template ");
  QMessageBox box;
  box.setText(txt);
  box.exec();

  return;

  myDatabase->setModal(true);
  myDatabase->show();

  QFileInfo appInfo(qApp->applicationDirPath());

  QString dirpath = appInfo.filePath() + "/Database/BaseConfigs/";
  QDir dir(dirpath);
  QStringList nameFilters;
  nameFilters << "*.plist";
  QStringList filesTemp =
      dir.entryList(nameFilters, QDir::Files | QDir::Readable, QDir::Name);
  QStringList files;
  for (int j = 0; j < filesTemp.count(); j++) {
    if (filesTemp.at(j).mid(0, 1) != ".") files.append(filesTemp.at(j));
  }

  myDatabase->init_Database(files);
}

void MainWindow::oc_Validate(bool show) {
  chkdata = new QProcess;

#ifdef Q_OS_WIN32
  chkdata->start(userDataBaseDir + "win/ocvalidate.exe", QStringList()
                                                             << SaveFileName);
#endif

#ifdef Q_OS_LINUX
  chkdata->start(userDataBaseDir + "linux/ocvalidate", QStringList()
                                                           << SaveFileName);
#endif

#ifdef Q_OS_MAC
  chkdata->start(userDataBaseDir + "mac/ocvalidate", QStringList()
                                                         << SaveFileName);
#endif

  chkdata->waitForFinished();
  QString result =
      QString::fromLocal8Bit(chkdata->readAll());  // 与保存文件的格式一致
  QString str;
  QString strMsg;
  ui->lblOCVTip->setFixedHeight(16);
  ui->lblOCVTip->setFixedWidth(16);
  ui->lblOCVTip->setText("");

  if (result.trimmed() == "Failed to read") return;

  dlgOCV->setTextOCV(result.trimmed());
  if (dlgOCV->ui->textEdit->document()->lineCount() <= 4) {
    blOCValidateError = false;
    str = tr("OK !");
    strMsg = result + "\n\n" + str;

    if (red < 55)
      ui->actionOcvalidate->setIcon(QIcon(":/icon/ov0.png"));
    else
      ui->actionOcvalidate->setIcon(QIcon(":/icon/ov.png"));
    ui->actionOcvalidate->setToolTip(ui->actionOcvalidate->text());

    dlgOCV->setGoEnabled(false);

    if (myDlgPreference->ui->chkHideToolbar->isChecked()) {
      ui->lblOCVTip->setStyleSheet(
          "QLabel{"
          "border-image:url(:/icon/temp.png) 4 4 4 4 stretch stretch;"
          "}");
      ui->lblOCVTip->setToolTip("");
    }

  } else {
    blOCValidateError = true;
    strMsg = result;
    dlgOCV->setGoEnabled(true);

    ui->actionOcvalidate->setIcon(QIcon(":/icon/overror.png"));
    ui->actionOcvalidate->setToolTip(
        tr("There is a issue with the configuration file."));

    if (myDlgPreference->ui->chkHideToolbar->isChecked()) {
      ui->lblOCVTip->setStyleSheet(
          "QLabel{"
          "border-image:url(:/icon/overror.png) 4 4 4 4 stretch stretch;"
          "}");
      ui->lblOCVTip->setToolTip(ui->actionOcvalidate->toolTip());
    }
  }

  dlgOCV->setTextOCV(strMsg);
  dlgOCV->setWindowFlags(dlgOCV->windowFlags() | Qt::WindowStaysOnTopHint);
  if (mac || osx1012) dlgOCV->ui->textEdit->setFont(QFont("Menlo"));
  if (win) dlgOCV->ui->textEdit->setFont(QFont("consolas"));
  if (win || linuxOS) {
    dlgOCV->ui->btnCreateVault->setVisible(false);
    dlgOCV->ui->chkSignature->setVisible(false);
  }
  if (ui->cboxVault->currentText().trimmed() == "Optional") {
    dlgOCV->ui->btnCreateVault->setEnabled(false);
    dlgOCV->ui->chkSignature->setEnabled(false);
  } else {
    dlgOCV->ui->btnCreateVault->setEnabled(true);
    dlgOCV->ui->chkSignature->setEnabled(true);
  }
  if (ui->cboxVault->currentText().trimmed() == "Secure")
    dlgOCV->ui->chkSignature->setChecked(true);
  if (ui->cboxVault->currentText().trimmed() == "Basic")
    dlgOCV->ui->chkSignature->setChecked(false);

  if (show) dlgOCV->show();
}

void MainWindow::on_actionOcvalidate_triggered() { oc_Validate(true); }

void MainWindow::on_actionMountEsp_triggered() { mount_esp(); }

void MainWindow::on_actionGenerateEFI_triggered() {
  mymethod->generateEFI(SaveFileName);
}

void MainWindow::on_btnExportMaster_triggered() {
  mymethod->on_btnExportMaster();
}

void MainWindow::on_btnImportMaster_triggered() {
  mymethod->on_btnImportMaster();
}

void MainWindow::on_editDatPasswordHash_textChanged(const QString& arg1) {
  ui->editDatPasswordHash->setToolTip(QString::number(arg1.count() / 2) +
                                      " Bytes");
}

void MainWindow::on_editDatPasswordSalt_textChanged(const QString& arg1) {
  ui->editDatPasswordSalt->setToolTip(QString::number(arg1.count() / 2) +
                                      " Bytes");
}

void MainWindow::openDir(QString strDir) {
  QString dirpath = QDir::homePath() + "/.ocat" + strDir;
  QString dir = "file:" + dirpath;
  QDesktopServices::openUrl(QUrl(dir, QUrl::TolerantMode));
}
void MainWindow::on_btnOpenACPIDir_clicked() {
  openDir("/Database/EFI/OC/ACPI");
}

void MainWindow::on_btnOpenDriversDir_clicked() {
  openDir("/Database/EFI/OC/Drivers");
}

void MainWindow::on_btnOpenToolsDir_clicked() {
  openDir("/Database/EFI/OC/Tools");
}

void MainWindow::on_btnOpenKextDir_clicked() {
  openDir("/Database/EFI/OC/Kexts");
}

void MainWindow::showDlgPar(bool b1, bool b2, bool b3, bool b4) {
  dlgPar->ui->gScanPolicy->setVisible(b1);
  dlgPar->ui->gPickerAttributes->setVisible(b2);
  dlgPar->ui->gExposeSensitiveData->setVisible(b3);
  dlgPar->ui->gDisplayLevel->setVisible(b4);
  dlgPar->resize(QSize(1, 1));
  dlgPar->setModal(true);
  dlgPar->show();
}

void MainWindow::on_btnDisplayLevel_clicked() {
  showDlgPar(false, false, false, true);
}

void MainWindow::on_btnScanPolicy_clicked() {
  showDlgPar(true, false, false, false);
}

void MainWindow::on_btnPickerAttributes_clicked() {
  showDlgPar(false, true, false, false);
}

void MainWindow::on_btnExposeSensitiveData_clicked() {
  showDlgPar(false, false, true, false);
}

void MainWindow::on_actionPlist_editor_triggered() {
  QUrl url(QString("https://github.com/ic005k/Xplist/blob/main/README.md"));
  QDesktopServices::openUrl(url);
}

void MainWindow::on_actionDSDT_SSDT_editor_triggered() {
  QUrl url(QString("https://github.com/ic005k/Xiasl/blob/master/README.md"));
  QDesktopServices::openUrl(url);
}

void MainWindow::on_actionDifferences_triggered() {
  QString qtManulFile = userDataBaseDir + "doc/Differences.pdf";
  QDesktopServices::openUrl(QUrl::fromLocalFile(qtManulFile));
}

void MainWindow::on_btnUp_clicked() { MoveItem(ui->table_acpi_add, true); }

void MainWindow::on_btnDown_clicked() { MoveItem(ui->table_acpi_add, false); }

void MainWindow::CellEnter(int row, QTableWidget* tw) {
  tw->clearSelection();

  int colCount = tw->columnCount();

  int rowsel;

  if (tw->currentIndex().row() < row)
    rowsel = row - 1;  // down

  else if (tw->currentIndex().row() > row)
    rowsel = row + 1;  // up

  else
    return;

  QList<QTableWidgetItem*> rowItems, rowItems1;
  for (int col = 0; col < colCount; ++col) {
    rowItems << tw->takeItem(row, col);
    rowItems1 << tw->takeItem(rowsel, col);
  }

  for (int cola = 0; cola < colCount; ++cola) {
    tw->setItem(rowsel, cola, rowItems.at(cola));
    tw->setItem(row, cola, rowItems1.at(cola));
  }
}

void MainWindow::cellEnteredSlot(int row, int column) {
  Q_UNUSED(column);

  if (ui->listMain->currentRow() == 0 && ui->listSub->currentRow() == 0)
    CellEnter(row, ui->table_acpi_add);
  if (ui->listMain->currentRow() == 3 && ui->listSub->currentRow() == 0)
    CellEnter(row, ui->table_kernel_add);
}

void MainWindow::CheckChange(QTableWidget* tw, int arg1, QToolButton* btnDel) {
  if (arg1 == 0) {
    btnDel->setEnabled(true);
    tw->setSelectionMode(QAbstractItemView::ExtendedSelection);
    tw->setSelectionBehavior(QAbstractItemView::SelectItems);
    disconnect(tw, SIGNAL(cellEntered(int, int)), this,
               SLOT(cellEnteredSlot(int, int)));
    tw->setCurrentCell(tw->currentRow(), tw->currentColumn());
  } else if (arg1 == 2) {
    btnDel->setEnabled(false);
    tw->setSelectionMode(QAbstractItemView::SingleSelection);
    tw->setSelectionBehavior(QAbstractItemView::SelectRows);
    connect(tw, SIGNAL(cellEntered(int, int)), this,
            SLOT(cellEnteredSlot(int, int)));
    tw->setCurrentCell(tw->currentRow(), tw->currentColumn());
  }

  this->repaint();
}

void MainWindow::on_btnUp_UEFI_Drivers_clicked() {
  MoveItem(ui->table_uefi_drivers, true);
}

void MainWindow::on_btnDown_UEFI_Drivers_clicked() {
  MoveItem(ui->table_uefi_drivers, false);
}

void MainWindow::on_actionLatest_Release_triggered() {
  QUrl url(
      QString("https://github.com/ic005k/" + strAppName + "/releases/latest"));
  QDesktopServices::openUrl(url);
}

void MainWindow::on_actionOnline_Download_Updates_triggered() {
  ShowAutoUpdateDlg(false);
}

void MainWindow::ShowAutoUpdateDlg(bool Database) {
  Q_UNUSED(Database);
  if (dlgAutoUpdate->strUrlOrg == "") return;
  if (dlgAutoUpdate->isVisible()) return;

  // dlgAutoUpdate->setWindowFlags(dlgAutoUpdate->windowFlags() |
  //                              Qt::WindowStaysOnTopHint);
  dlgAutoUpdate->setModal(true);
  dlgAutoUpdate->show();
  dlgAutoUpdate->startWgetDownload();
  if (linuxOS) {
    dlgAutoUpdate->ui->progressBar->setVisible(false);
    dlgAutoUpdate->ui->btnShowLog->setVisible(false);
    dlgAutoUpdate->ui->textEdit->setVisible(true);
  } else {
    dlgAutoUpdate->ui->textEdit->setVisible(false);
    dlgAutoUpdate->setMaximumHeight(100);
  }
}

void MainWindow::on_txtEditHex_textChanged(const QString& arg1) {
  if (txtEditASCIITextChanged) {
    txtEditASCIITextChanged = false;
    return;
  }

  QValidator* validator =
      new QRegularExpressionValidator(regxData, ui->txtEditHex);
  ui->txtEditHex->setValidator(validator);

  txtEditHexTextChanged = true;
  QString str0, str;
  str0 = arg1;
  str = str0.replace(" ", "");

  if (str.length() % 2 == 0)
    ui->txtEditASCII->setText(HexStrToByte(str));
  else
    ui->txtEditASCII->setText("");
}

void MainWindow::on_txtEditASCII_textChanged(const QString& arg1) {
  if (txtEditHexTextChanged) {
    txtEditHexTextChanged = false;
    return;
  }

  txtEditASCIITextChanged = true;
  QString str = arg1;
  ui->txtEditHex->setText(str.toUtf8().toHex().toUpper());
}

void MainWindow::on_listSub_currentRowChanged(int currentRow) {
  ui->lblStatusShow->setText("V" + CurVersion);

  if (find) {
    return;
  }
  Q_UNUSED(currentRow);
  setConversionWidgetVisible(false);
  mymethod->UpdateStatusBarInfo();

  if (!Initialization) {
    QObjectList list = getAllTableWidget(getAllUIControls(
        getSubTabWidget(ui->listMain->currentRow(), currentRow)));
    if (list.count() >= 1) {
      // QTableWidget* w = (QTableWidget*)list.at(0);
      // w->setFocus();
    }
  }
}

void MainWindow::on_listMain_currentRowChanged(int currentRow) {
  for (int i = 0; i < 8; i++) {
    if (ui->listMain->currentRow() == i)
      ui->listMain->item(i)->setIcon(QIcon(strIconListSel.at(i)));
    else
      ui->listMain->item(i)->setIcon(QIcon(strIconList.at(i)));
  }

  // Misc
  if (currentRow == 4) {
    bool mod = this->isWindowModified();
    QString str = ui->editIntConsoleAttributes->text().trimmed();
    ui->editIntConsoleAttributes->setText("");
    ui->editIntConsoleAttributes->setText(str);
    this->setWindowModified(mod);
    updateIconStatus();
  }

  setConversionWidgetVisible(false);
  mymethod->UpdateStatusBarInfo();
}

void MainWindow::setConversionWidgetVisible(bool v) {
  if (!v) {
    ui->txtEditHex->setVisible(false);
    ui->lblHexASCII->setVisible(false);
    ui->txtEditASCII->setVisible(false);
    ui->btnUpdateHex->setVisible(false);
  } else {
    ui->txtEditHex->setVisible(true);
    ui->lblHexASCII->setVisible(true);
    ui->txtEditASCII->setVisible(true);
    ui->btnUpdateHex->setVisible(true);
  }
}

void MainWindow::on_btnUpdateHex_clicked() {
  QString str = ui->txtEditHex->text().trimmed();
  str = str.replace(" ", "");
  if (str.length() % 2 != 0) return;

  QObjectList listTable;
  QTableWidget* t;
  bool txtHexFocus = ui->txtEditHex->hasFocus();
  bool txtASCIIFocus = ui->txtEditASCII->hasFocus();

  QWidget* w =
      getSubTabWidget(ui->listMain->currentRow(), ui->listSub->currentRow());
  listTable = getAllTableWidget(getAllUIControls(w));

  if (listTable.count() > 1) {
    for (int i = 0; i < listTable.count(); i++) {
      t = (QTableWidget*)listTable.at(i);
      if (t == ui->table_dp_add || t == ui->table_nv_add) t->setFocus();
    }
  } else if (listTable.count() == 1) {
    t = (QTableWidget*)listTable.at(0);
    t->setFocus();
  }

  QKeyEvent* tabKey =
      new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
  QCoreApplication::sendEvent(this, tabKey);

  lineEdit->setText(str);

  QCoreApplication::sendEvent(lineEdit, tabKey);

  if (txtHexFocus) ui->txtEditHex->setFocus();
  if (txtASCIIFocus) ui->txtEditASCII->setFocus();
}

void MainWindow::on_actionOpen_Directory_triggered() {
  QFileInfo fi(SaveFileName);
  QString str = fi.filePath().replace(fi.fileName(), "");
  QString dir;
  if (ui->listMain->currentRow() == 0 && ui->listSub->currentRow() == 0) {
    dir = strACPI;
  } else if (ui->listMain->currentRow() == 3 &&
             ui->listSub->currentRow() == 0) {
    dir = strKexts;
  } else if (ui->listMain->currentRow() == 4 &&
             ui->listSub->currentRow() == 5) {
    dir = strTools;
  } else if (ui->listMain->currentRow() == 7 &&
             ui->listSub->currentRow() == 3) {
    dir = strDrivers;
  } else
    dir = str;

  if (QDir(dir).exists())
    QDesktopServices::openUrl(QUrl("file:" + dir, QUrl::TolerantMode));
  else
    QDesktopServices::openUrl(QUrl("file:" + str, QUrl::TolerantMode));
}

void MainWindow::on_actionOpen_database_directory_triggered() {
  QString dir;
  if (blDEV)
    dir = "file:" + userDataBaseDir;
  else
    dir = "file:" + userDataBaseDir;
  QDesktopServices::openUrl(QUrl(dir, QUrl::TolerantMode));
}

void MainWindow::on_btnDPAddPreset_clicked() {
  dlgPresetValues->blACPIPatch = false;
  dlgPresetValues->blDPAdd = true;
  dlgPresetValues->blKernelPatch = false;
  dlgPresetValues->blNVDelete = false;
  dlgPresetValues->blNVLegacy = false;
  dlgPresetValues->blNVAdd = false;
  dlgPresetValues->blKext = false;

  dlgPresetValues->setModal(true);
  dlgPresetValues->loadPreset("DeviceProperties", "Add", "",
                              dlgPresetValues->ui->listPreset);
  dlgPresetValues->show();
}

void MainWindow::on_btnACPIPatch_clicked() {
  dlgPresetValues->blACPIPatch = true;
  dlgPresetValues->blDPAdd = false;
  dlgPresetValues->blKernelPatch = false;
  dlgPresetValues->blNVDelete = false;
  dlgPresetValues->blNVLegacy = false;
  dlgPresetValues->blNVAdd = false;
  dlgPresetValues->blKext = false;

  dlgPresetValues->setModal(true);
  dlgPresetValues->loadPreset("ACPI", "Patch", "Comment",
                              dlgPresetValues->ui->listPreset);
  dlgPresetValues->show();
}

void MainWindow::on_btnPresetKernelPatch_clicked() {
  dlgPresetValues->blACPIPatch = false;
  dlgPresetValues->blDPAdd = false;
  dlgPresetValues->blKernelPatch = true;
  dlgPresetValues->blNVDelete = false;
  dlgPresetValues->blNVLegacy = false;
  dlgPresetValues->blNVAdd = false;
  dlgPresetValues->blKext = false;

  myDlgKernelPatch->setModal(true);
  myDlgKernelPatch->loadFiles();
  myDlgKernelPatch->show();
}

void MainWindow::on_btnPresetNVAdd_clicked() {
  dlgPresetValues->blACPIPatch = false;
  dlgPresetValues->blDPAdd = false;
  dlgPresetValues->blKernelPatch = false;
  dlgPresetValues->blNVDelete = false;
  dlgPresetValues->blNVLegacy = false;
  dlgPresetValues->blNVAdd = true;
  dlgPresetValues->blKext = false;

  dlgPresetValues->setModal(true);
  dlgPresetValues->loadPreset("NVRAM", "Add", "",
                              dlgPresetValues->ui->listPreset);
  dlgPresetValues->show();
}

void MainWindow::on_btnPresetNVDelete_clicked() {
  dlgPresetValues->blACPIPatch = false;
  dlgPresetValues->blDPAdd = false;
  dlgPresetValues->blKernelPatch = false;
  dlgPresetValues->blNVDelete = true;
  dlgPresetValues->blNVLegacy = false;
  dlgPresetValues->blNVAdd = false;
  dlgPresetValues->blKext = false;

  dlgPresetValues->setModal(true);
  dlgPresetValues->loadPreset("NVRAM", "Delete", "",
                              dlgPresetValues->ui->listPreset);
  dlgPresetValues->show();
}

void MainWindow::on_btnPresetNVLegacy_clicked() {
  dlgPresetValues->blACPIPatch = false;
  dlgPresetValues->blDPAdd = false;
  dlgPresetValues->blKernelPatch = false;
  dlgPresetValues->blNVDelete = false;
  dlgPresetValues->blNVLegacy = true;
  dlgPresetValues->blNVAdd = false;
  dlgPresetValues->blKext = false;

  dlgPresetValues->setModal(true);
  dlgPresetValues->loadPreset("NVRAM", "LegacySchema", "",
                              dlgPresetValues->ui->listPreset);
  dlgPresetValues->show();
}

void MainWindow::on_actionAutoChkUpdate_triggered() {}

void MainWindow::on_actionEdit_Presets_triggered() {
  QString dirpath = strAppExePath + "/Database/preset/PreSet.plist";
  QString dir = "file:" + dirpath;
  QDesktopServices::openUrl(QUrl(dir, QUrl::TolerantMode));
}

void MainWindow::on_btnNo_clicked() { ui->frameTip->setHidden(true); }

void MainWindow::on_btnYes_clicked() {
  ui->frameTip->setHidden(true);
  openFile(SaveFileName);
}

void MainWindow::on_btnAddbootArgs_clicked() {
  int row = ui->table_nv_add->currentRow();
  if (row < 0) return;
  if (ui->table_nv_add->item(row, 0)->text().trimmed() == "boot-args") {
    dlgMiscBootArgs->setModal(true);
    dlgMiscBootArgs->show();
  }
}

void MainWindow::updateStatus() { mymethod->autoTip(); }

void MainWindow::updateIconStatus() {
  if (isWindowModified()) {
    if (red < 55)
      ui->actionSave->setIcon(QIcon(":/icon/savetip0.png"));
    else
      ui->actionSave->setIcon(QIcon(":/icon/savetip.png"));
  } else {
    if (red < 55)
      ui->actionSave->setIcon(QIcon(":/icon/save0.png"));
    else
      ui->actionSave->setIcon(QIcon(":/icon/save.png"));
  }
}

void MainWindow::on_table_nv_add0_itemClicked(QTableWidgetItem* item) {
  Q_UNUSED(item);
}

void MainWindow::on_btnKextPreset_clicked() { mymethod->kextPreset(); }

void MainWindow::on_actionOCAuxiliaryToolsDoc_triggered() {
  QUrl url(
      QString("https://github.com/5T33Z0/OC-Little-Translated/blob/main/"
              "D_Updating_OpenCore/README.md"));
  QDesktopServices::openUrl(url);
}

void MainWindow::on_actionBackup_EFI_triggered() { mymethod->backupEFI(); }

void MainWindow::on_comboBoxACPI_currentTextChanged(const QString& arg1) {
  if (Initialization) return;
  mymethod->getMarkerQuirks(arg1, "ACPI", ui->tabACPI4, "ACPI-Quirks.txt");
}

void MainWindow::on_comboBoxBooter_currentIndexChanged(const QString& arg1) {
  if (Initialization) return;
  mymethod->getMarkerQuirks(arg1, "Booter", ui->tabBooter3,
                            "Booter-Quirks.txt");
}

void MainWindow::on_comboBoxKernel_currentIndexChanged(const QString& arg1) {
  if (Initialization) return;
  mymethod->getMarkerQuirks(arg1, "Kernel", ui->tabKernel6,
                            "Kernel-Quirks.txt");
}

void MainWindow::on_comboBoxUEFI_currentIndexChanged(const QString& arg1) {
  if (Initialization) return;
  mymethod->getMarkerQuirks(arg1, "UEFI", ui->tabUEFI8, "UEFI-Quirks.txt");
}

void MainWindow::on_actionOpenCore_DEV_triggered() {
  blDEV = ui->actionOpenCore_DEV->isChecked();
  changeOpenCore(blDEV);

  Reg.setValue("OpenCoreDEV", blDEV);
  Reg.setValue("DEBUG", ui->actionDEBUG->isChecked());
}

void MainWindow::mousePressEvent(QMouseEvent* e) {
  if (e->button() == Qt::LeftButton) {
    isDrag = true;
    m_position = e->globalPos() - this->pos();
    e->accept();
  }
}

void MainWindow::mouseMoveEvent(QMouseEvent* e) {
  if (isDrag & (e->buttons() & Qt::LeftButton)) {
    move(e->globalPos() - m_position);
    e->accept();
  }
}

void MainWindow::mouseReleaseEvent(QMouseEvent*) { isDrag = false; }

void MainWindow::on_actionPreferences_triggered() {
  if (osx1012)
    myDlgPreference->ui->tabWidget->setDocumentMode(true);
  else
    myDlgPreference->ui->tabWidget->setDocumentMode(false);

  myDlgPreference->setWindowTitle(tr("Preferences"));
  myDlgPreference->refreshKextUrl(true);
  myDlgPreference->setModal(true);
  myDlgPreference->show();
  myDlgPreference->ui->myeditFind->setFocus();
  if (myDlgPreference->ui->tableKextUrl->rowCount() > 0) {
    myDlgPreference->ui->tableKextUrl->clearSelection();
  }
}

void MainWindow::on_actionDocumentation_triggered() {
  if (!zh_cn) {
    QUrl url_en(QString("https://github.com/ic005k/" + strAppName +
                        "/blob/master/READMe.md"));
    QDesktopServices::openUrl(url_en);
  } else {
    QUrl url_cn(QString("https://github.com/ic005k/" + strAppName +
                        "/blob/master/READMe.md"));
    QDesktopServices::openUrl(url_cn);
  }
}

void MainWindow::on_btnHide_clicked() {
  ui->frameToolBar->setHidden(true);
  ui->mycboxFind->lineEdit()->clear();
}

void MainWindow::on_actionMove_Up_triggered() {
  if (!Initialization) {
    QObjectList list = getAllTableWidget(getAllUIControls(getSubTabWidget(
        ui->listMain->currentRow(), ui->listSub->currentRow())));
    if (list.count() >= 1) {
      QTableWidget* w = (QTableWidget*)list.at(0);

      if (w == ui->table_nv_add0 || w == ui->table_nv_add ||
          w == ui->table_nv_del0 || w == ui->table_nv_del ||
          w == ui->table_nv_ls0 || w == ui->table_nv_ls ||
          w == ui->table_dp_add0 || w == ui->table_dp_add ||
          w == ui->table_dp_del0 || w == ui->table_dp_del) {
        return;
      }

      if (w->hasFocus()) MoveItem(w, true);
    }
  }
}

void MainWindow::on_actionMove_Down_triggered() {
  if (!Initialization) {
    QObjectList list = getAllTableWidget(getAllUIControls(getSubTabWidget(
        ui->listMain->currentRow(), ui->listSub->currentRow())));
    if (list.count() >= 1) {
      QTableWidget* w = (QTableWidget*)list.at(0);

      if (w == ui->table_nv_add0 || w == ui->table_nv_add ||
          w == ui->table_nv_del0 || w == ui->table_nv_del ||
          w == ui->table_nv_ls0 || w == ui->table_nv_ls ||
          w == ui->table_dp_add0 || w == ui->table_dp_add ||
          w == ui->table_dp_del0 || w == ui->table_dp_del) {
        return;
      }

      if (w->hasFocus()) MoveItem(w, false);
    }
  }
}

void MainWindow::on_actionAdd_triggered() {
  if (ui->listMain->currentRow() == 0 && ui->listSub->currentRow() == 0) {
    ui->btnACPIAdd_Add->click();
  }

  if (ui->table_acpi_del->hasFocus()) ui->btnACPIDel_Add->click();
  if (ui->table_acpi_patch->hasFocus()) ui->btnACPIPatch_Add->click();

  if (ui->table_booter->hasFocus()) ui->btnBooter_Add->click();
  if (ui->table_Booter_patch->hasFocus()) ui->btnBooterPatchAdd->click();

  if (ui->table_dp_add0->hasFocus()) ui->btnDPAdd_Add0->click();
  if (ui->table_dp_add->hasFocus()) ui->btnDPAdd_Add->click();

  if (ui->table_dp_del0->hasFocus()) ui->btnDPDel_Add0->click();
  if (ui->table_dp_del->hasFocus()) ui->btnDPDel_Add->click();

  if (ui->listMain->currentRow() == 3 && ui->listSub->currentRow() == 0)
    ui->btnKernelAdd_Add->click();
  if (ui->table_kernel_block->hasFocus()) ui->btnKernelBlock_Add->click();
  if (ui->table_kernel_Force->hasFocus()) ui->btnKernelForce_Add->click();
  if (ui->table_kernel_patch->hasFocus()) ui->btnKernelPatchAdd->click();

  if (ui->tableBlessOverride->hasFocus()) ui->btnMiscBO_Add->click();
  if (ui->tableEntries->hasFocus()) ui->btnMiscEntries_Add->click();
  if (ui->listMain->currentRow() == 4 && ui->listSub->currentRow() == 5)
    ui->btnMiscTools_Add->click();

  if (ui->table_nv_add0->hasFocus()) ui->btnNVRAMAdd_Add0->click();
  if (ui->table_nv_add->hasFocus()) ui->btnNVRAMAdd_Add->click();

  if (ui->table_nv_del0->hasFocus()) ui->btnNVRAMDel_Add0->click();
  if (ui->table_nv_del->hasFocus()) ui->btnNVRAMDel_Add->click();

  if (ui->table_nv_ls0->hasFocus()) ui->btnNVRAMLS_Add0->click();
  if (ui->table_nv_ls->hasFocus()) ui->btnNVRAMLS_Add->click();

  if (ui->tableDevices->hasFocus()) ui->btnDevices_add->click();

  if (ui->listMain->currentRow() == 7 && ui->listSub->currentRow() == 3)
    ui->btnUEFIDrivers_Add->click();

  if (ui->table_uefi_ReservedMemory->hasFocus()) ui->btnUEFIRM_Add->click();

  if (ui->table_uefi_Unload->hasFocus()) ui->btnUEFIUnload_Add->click();
}

void MainWindow::on_actionDelete_triggered() {
  if (ui->table_acpi_add->hasFocus()) ui->btnACPIAdd_Del->click();
  if (ui->table_acpi_del->hasFocus()) ui->btnACPIDel_Del->click();
  if (ui->table_acpi_patch->hasFocus()) ui->btnACPIPatch_Del->click();

  if (ui->table_booter->hasFocus()) ui->btnBooter_Del->click();
  if (ui->table_Booter_patch->hasFocus()) ui->btnBooterPatchDel->click();

  if (ui->table_dp_add0->hasFocus()) ui->btnDPAdd_Del0->click();
  if (ui->table_dp_add->hasFocus()) ui->btnDPAdd_Del->click();

  if (ui->table_dp_del0->hasFocus()) ui->btnDPDel_Del0->click();
  if (ui->table_dp_del->hasFocus()) ui->btnDPDel_Del->click();

  if (ui->table_kernel_add->hasFocus()) ui->btnKernelAdd_Del->click();
  if (ui->table_kernel_block->hasFocus()) ui->btnKernelBlock_Del->click();
  if (ui->table_kernel_Force->hasFocus()) ui->btnKernelForce_Del->click();
  if (ui->table_kernel_patch->hasFocus()) ui->btnKernelPatchDel->click();

  if (ui->tableBlessOverride->hasFocus()) ui->btnMiscBO_Del->click();
  if (ui->tableEntries->hasFocus()) ui->btnMiscEntries_Del->click();
  if (ui->tableTools->hasFocus()) ui->btnMiscTools_Del->click();

  if (ui->table_nv_add0->hasFocus()) ui->btnNVRAMAdd_Del0->click();
  if (ui->table_nv_add->hasFocus()) ui->btnNVRAMAdd_Del->click();

  if (ui->table_nv_del0->hasFocus()) ui->btnNVRAMDel_Del0->click();
  if (ui->table_nv_del->hasFocus()) ui->btnNVRAMDel_Del->click();

  if (ui->table_nv_ls0->hasFocus()) ui->btnNVRAMLS_Del0->click();
  if (ui->table_nv_ls->hasFocus()) ui->btnNVRAMLS_Del->click();

  if (ui->tableDevices->hasFocus()) ui->btnDevices_del->click();

  if (ui->table_uefi_drivers->hasFocus()) ui->btnUEFIDrivers_Del->click();

  if (ui->table_uefi_ReservedMemory->hasFocus()) ui->btnUEFIRM_Del->click();

  if (ui->table_uefi_Unload->hasFocus()) ui->btnUEFIUnload_Del->click();
}

void MainWindow::checkSystemAudioVolume() {
  ui->lblSystemAudioVolume->setText("");
  if (ui->table_nv_add0->rowCount() > 0) {
    int old = ui->table_nv_add0->currentRow();
    for (int i = 0; i < ui->table_nv_add0->rowCount(); i++) {
      QString item = ui->table_nv_add0->item(i, 0)->text().trimmed();
      if (item == "7C436110-AB2A-4BBB-A880-FE41995C9F82") {
        ui->table_nv_add0->setCurrentCell(i, 0);
        if (ui->table_nv_add->rowCount() > 0) {
          for (int j = 0; j < ui->table_nv_add->rowCount(); j++) {
            QString str = ui->table_nv_add->item(j, 0)->text();
            if (str == "SystemAudioVolume") {
              QString value = ui->table_nv_add->item(j, 2)->text();
              bool ok = false;
              ulong dec = value.toLongLong(&ok, 16);
              ui->lblSystemAudioVolume->setText("SystemAudioVolume: 0x" +
                                                value + " (" +
                                                QString::number(dec) + ")");
              break;
            }
          }
        }
      }
    }
    ui->table_nv_add0->setCurrentCell(old, 0);
  }
}

void MainWindow::on_mycboxEmulate_currentTextChanged(const QString& arg1) {
  if (arg1 == "None") {
    ui->editDatCpuid1Data->setText("");
    ui->editDatCpuid1Mask->setText("");
  }

  if (arg1 == "Emulate Haswell (0x0306C3)") {
    ui->editDatCpuid1Data->setText(
        "C3 06 03 00 00 00 00 00 00 00 00 00 00 00 00 00");
    ui->editDatCpuid1Mask->setText(
        "FF FF FF FF 00 00 00 00 00 00 00 00 00 00 00 00");
  }

  if (arg1 == "Emulate Broadwell (0x0306D4)") {
    ui->editDatCpuid1Data->setText(
        "D4 06 03 00 00 00 00 00 00 00 00 00 00 00 00 00");
    ui->editDatCpuid1Mask->setText(
        "FF FF FF FF 00 00 00 00 00 00 00 00 00 00 00 00");
  }

  if (arg1 == "Emulate Coffee Lake") {
    ui->editDatCpuid1Data->setText(
        "EA 06 09 00 00 00 00 00 00 00 00 00 00 00 00 00");
    ui->editDatCpuid1Mask->setText(
        "FF FF FF FF 00 00 00 00 00 00 00 00 00 00 00 00");
  }

  if (arg1 == "Emulate Comet Lake U42 (0x0806EC)") {
    ui->editDatCpuid1Data->setText(
        "EC 06 08 00 00 00 00 00 00 00 00 00 00 00 00 00");
    ui->editDatCpuid1Mask->setText(
        "FF FF FF FF 00 00 00 00 00 00 00 00 00 00 00 00");
  }

  if (arg1 == "Emulate Comet Lake (0x0A0655)") {
    ui->editDatCpuid1Data->setText(
        "55 06 0A 00 00 00 00 00 00 00 00 00 00 00 00 00");
    ui->editDatCpuid1Mask->setText(
        "FF FF FF FF 00 00 00 00 00 00 00 00 00 00 00 00");
  }

  if (arg1 == "Emulate Cascade Lake") {
    ui->editDatCpuid1Data->setText(
        "57 06 05 00 00 00 00 00 00 00 00 00 00 00 00 00");
    ui->editDatCpuid1Mask->setText(
        "FF FF FF FF 00 00 00 00 00 00 00 00 00 00 00 00");
  }
}

void MainWindow::on_btnImport_clicked() {
  QFileDialog fd;
  QString plistFile =
      fd.getOpenFileName(this, "Clover Config file", "",
                         "Clover Config file(*.plist);;All files(*.*)");

  if (!QFile(plistFile).exists()) return;

  if (!mymethod->isKeyExists(plistFile, "SmUUID")) {
    QMessageBox box;
    box.setText(tr("This is not a valid Clover configuration file."));
    box.exec();
    return;
  }

  ui->editSystemUUID->setText(mymethod->readPlist(plistFile, "SmUUID"));
  ui->editSystemSerialNumber->setText(
      mymethod->readPlist(plistFile, "SerialNumber"));
  ui->editMLB->setText(mymethod->readPlist(plistFile, "MLB"));
  ui->editDatROM->setText(mymethod->readPlist(plistFile, "ROM"));

  QString pname = mymethod->readPlist(plistFile, "ProductName");
  for (int i = 0; i < ui->cboxSystemProductName->count(); i++) {
    if (ui->cboxSystemProductName->itemText(i).contains(pname)) {
      ui->cboxSystemProductName->setCurrentIndex(i);
      break;
    }
  }
}

void MainWindow::on_btnExport_clicked() {
  QFileDialog fd;
  QString plistFile =
      fd.getOpenFileName(this, "Clover Config file", "",
                         "Clover Config file(*.plist);;All files(*.*)");

  if (!QFile(plistFile).exists()) return;

  if (!mymethod->isKeyExists(plistFile, "SmUUID")) {
    QMessageBox box;
    box.setText(tr("This is not a valid Clover configuration file."));
    box.exec();
    return;
  }

  QString uuid = ui->editSystemUUID->text().trimmed();
  mymethod->writePlist(plistFile, "SmUUID", uuid);

  QString serial = ui->editSystemSerialNumber->text().trimmed();
  mymethod->writePlist(plistFile, "SerialNumber", serial);

  QString mlb = ui->editMLB->text().trimmed();
  mymethod->writePlist(plistFile, "MLB", mlb);

  QString rom = ui->editDatROM->text().trimmed();
  mymethod->writePlist(plistFile, "ROM", rom);

  QString pname = ui->cboxSystemProductName->currentText();
  pname = pname.split(" ").at(0);
  mymethod->writePlist(plistFile, "ProductName", pname);
}

void MainWindow::on_actionDEBUG_triggered() {
  on_actionOpenCore_DEV_triggered();
}

void MainWindow::on_actionInitDatabaseLinux_triggered() {
  // Init Linux Database

  if (linuxOS) {
    copyDirectoryFiles(strAppExePath + "/Database/",
                       QDir::homePath() + "/.ocat/Database/", true);
  }
}

void MainWindow::on_actionNew_Key_Field_triggered() {
  QWidget* tab =
      getSubTabWidget(ui->listMain->currentRow(), ui->listSub->currentRow());
  QObjectList list = getAllTableWidget(getAllUIControls(tab));
  if (list.count() > 0) {
    QMessageBox::critical(this, "",
                          tr("You cannot add a new key field to a page where a "
                             "data table exists."));
    return;
  }
  myDlgNewKeyField->setModal(true);
  myDlgNewKeyField->show();
}

void MainWindow::on_btnSystemSerialNumber_clicked() {
  QString str = "https://checkcoverage.apple.com/?sn=" +
                ui->editSystemSerialNumber->text().trimmed();
  QUrl url(str);
  QDesktopServices::openUrl(url);
}

void MainWindow::smart_UpdateKeyField() {
  if (!Reg.value("SmartKey").toBool()) {
  };

  QString fileSample;
  fileSample = userDataBaseDir + "BaseConfigs/SampleCustom.plist";
  QFile file(fileSample);
  if (file.exists()) {
    mapTatol.clear();
    mapTatol = PListParser::parsePList(&file).toMap();
  }

  dlgNewKeyField::check_SampleFile(mapTatol, ui->tabACPI4, "ACPI", "Quirks");

  dlgNewKeyField::check_SampleFile(mapTatol, ui->tabBooter3, "Booter",
                                   "Quirks");

  dlgNewKeyField::check_SampleFile(mapTatol, ui->tabKernel5, "Kernel",
                                   "Emulate");
  dlgNewKeyField::check_SampleFile(mapTatol, ui->tabKernel6, "Kernel",
                                   "Quirks");
  dlgNewKeyField::check_SampleFile(mapTatol, ui->tabKernel7, "Kernel",
                                   "Scheme");

  dlgNewKeyField::check_SampleFile(mapTatol, ui->tabMisc1, "Misc", "Boot");
  dlgNewKeyField::check_SampleFile(mapTatol, ui->tabMisc2, "Misc", "Debug");
  dlgNewKeyField::check_SampleFile(mapTatol, ui->tabMisc3, "Misc", "Security");
  dlgNewKeyField::check_SampleFile(mapTatol, ui->tabMisc7, "Misc", "Serial");

  dlgNewKeyField::check_SampleFile(mapTatol, ui->gboxNVRAM, "NVRAM", "");

  dlgNewKeyField::check_SampleFile(mapTatol, ui->tabPlatformInfo1,
                                   "PlatformInfo", "Generic");
  dlgNewKeyField::check_SampleFile(mapTatol, ui->tabPlatformInfo2,
                                   "PlatformInfo", "DataHub");
  dlgNewKeyField::check_SampleFile(mapTatol, ui->tabPlatformInfo3,
                                   "PlatformInfo", "Memory");
  dlgNewKeyField::check_SampleFile(mapTatol, ui->tabPlatformInfo4,
                                   "PlatformInfo", "PlatformNVRAM");
  dlgNewKeyField::check_SampleFile(mapTatol, ui->tabPlatformInfo5,
                                   "PlatformInfo", "SMBIOS");
  dlgNewKeyField::check_SampleFile(mapTatol, ui->gbox01, "PlatformInfo", "");

  dlgNewKeyField::check_SampleFile(mapTatol, ui->gboxUEFI, "UEFI", "");
  dlgNewKeyField::check_SampleFile(mapTatol, ui->tabUEFI1, "UEFI", "APFS");
  dlgNewKeyField::check_SampleFile(mapTatol, ui->tabUEFI2, "UEFI",
                                   "AppleInput");
  dlgNewKeyField::check_SampleFile(mapTatol, ui->tabUEFI3, "UEFI", "Audio");
  dlgNewKeyField::check_SampleFile(mapTatol, ui->tabUEFI5, "UEFI", "Input");
  dlgNewKeyField::check_SampleFile(mapTatol, ui->tabUEFI6, "UEFI", "Output");
  dlgNewKeyField::check_SampleFile(mapTatol, ui->tabUEFI7, "UEFI",
                                   "ProtocolOverrides");

  dlgNewKeyField::check_SampleFile(mapTatol, ui->tabUEFI8, "UEFI", "Quirks");

  QVariantMap mapMain;
  QVariantMap mapSub;
  // ACPI
  initui_ACPI();

  // Booter
  initui_Booter();

  // DeviceProperties
  initui_DP();

  // Kernel
  initui_Kernel();

  // Misc
  // BlessOverride
  QStringList list1 = QStringList() << "BlessOverride";
  Method::init_Table(ui->tableBlessOverride, list1);
  ui->tableBlessOverride->horizontalHeader()->setStretchLastSection(true);

  // Entries
  Method::init_Table(ui->tableEntries,
                     Method::get_HorizontalHeaderList("Misc", "Entries"));

  // Tools
  Method::init_Table(ui->tableTools,
                     Method::get_HorizontalHeaderList("Misc", "Tools"));

  // Misc-Serial-Custom
  ui->gboxCustom->hide();
  QVariantMap mapMisc = mapTatol["Misc"].toMap();
  QVariantMap mapSerial = mapMisc["Serial"].toMap();
  QVariantMap mapCustom = mapSerial["Custom"].toMap();
  qDebug() << "Serial-Custom" << mapCustom.count();
  if (mapCustom.count() > 0) {
    ui->gboxCustom->show();
  }

  // NVRAM
  initui_NVRAM();

  // PlatformInfo
  // Memory-Devices
  mapMain.clear();
  mapSub.clear();
  mapMain = mapTatol["PlatformInfo"].toMap();
  mapSub = mapMain["Memory"].toMap();
  QVariantList maplist = mapSub["Devices"].toList();
  QVariantMap map;
  QStringList list;
  if (maplist.count() > 0) {
    map = maplist.at(0).toMap();
    list = map.keys();

    for (int i = 0; i < list.count(); i++) {
      QString name = list.at(i);
      QString type = map[name].typeName();
      if (type == "bool") {
        boolTypeList.removeOne(name);
        boolTypeList.append(name);
      }
      if (type == "qlonglong") {
        intTypeList.removeOne(name);
        intTypeList.append(name);
      }
      if (type == "QByteArray") {
        dataTypeList.removeOne(name);
        dataTypeList.append(name);
      }
    }
  }
  Method::init_Table(ui->tableDevices, list);

  // UEFI
  // Drivers
  QStringList listDrivers = Method::get_HorizontalHeaderList("UEFI", "Drivers");
  Method::init_Table(ui->table_uefi_drivers, listDrivers);

  // ReservedMemory
  Method::init_Table(
      ui->table_uefi_ReservedMemory,
      Method::get_HorizontalHeaderList("UEFI", "ReservedMemory"));

  // Unload
  mapMain.clear();
  mapSub.clear();
  mapMain = mapTatol["UEFI"].toMap();
  if (mapMain.keys().contains("Unload")) {
    isUnload = true;
  } else {
    isUnload = false;
  }
  if (ui->listMain->currentRow() == 7) {
    on_listMain_itemSelectionChanged();
  }
  Method::init_Table(ui->table_uefi_Unload,
                     Method::get_HorizontalHeaderList("UEFI", "Unload"));

  if (!loading) {
    for (int i = 0; i < listOCATWidgetDelList.count(); i++) {
      QWidget* w0 = listOCATWidgetDelList.at(i);
      if (w0->objectName().mid(0, 3) == "chk") {
        QCheckBox* w = (QCheckBox*)listOCATWidgetDelList.at(i);
        connect(w, &QCheckBox::stateChanged, this, &MainWindow::setWM);
      } else {
        QLineEdit* w = (QLineEdit*)w0->children().at(2);
        connect(w, &QLineEdit::textChanged, this, &MainWindow::setWM);
      }
    }

    if (QFile(SaveFileName).exists()) {
      openFile(SaveFileName);
    }
    init_AutoColumnWidth();
  }
}

void MainWindow::init_AutoColumnWidth() {
  QObjectList listOfTableWidget =
      getAllTableWidget(getAllUIControls(ui->tabTotal));
  for (int i = 0; i < listOfTableWidget.count(); i++) {
    QTableWidget* w = (QTableWidget*)listOfTableWidget.at(i);
    // Auto Col Width

    bool isAutoColWidth =
        Reg.value(w->objectName() + "AutoColWidth", true).toBool();
    set_AutoColWidth(w, isAutoColWidth);
  }
}

void MainWindow::on_editSystemSerialNumber_textChanged(const QString& arg1) {
#ifdef Q_OS_MAC
  QProcess* p = new QProcess;
  p->start(userDataBaseDir + "mac/macserial", QStringList() << "-i" << arg1);
  p->waitForFinished();
  ui->textMacInfo->clear();
  QString result = p->readAll();
  ui->textMacInfo->append(arg1 + " :");
  ui->textMacInfo->append("");
  ui->textMacInfo->append(result);
#endif

  ui->editSystemSerialNumber_2->setText(arg1);
  ui->editSystemSerialNumber_data->setText(arg1);
  ui->editSystemSerialNumber_PNVRAM->setText(arg1);
}

void MainWindow::on_editSystemUUID_textChanged(const QString& arg1) {
  ui->editSystemUUID_DataHub->setText(arg1);
  ui->editSystemUUID_PlatformNVRAM->setText(arg1);
  ui->editSystemUUID_SMBIOS->setText(arg1);
}

void MainWindow::on_editMLB_textChanged(const QString& arg1) {
  ui->editMLB_PNVRAM->setText(arg1);
}

void MainWindow::on_editDatROM_textChanged(const QString& arg1) {
  ui->editDatROM_PNVRAM->setText(arg1);
}

void MainWindow::on_actionOpenCoreChineseDoc_triggered() {
  QUrl url(QString("https://oc.skk.moe/"));
  QDesktopServices::openUrl(url);
}

void MainWindow::dataClassChange_uefi_drivers() {
  ui->table_uefi_drivers->item(c_row, 3)->setTextAlignment(Qt::AlignCenter);
  ui->table_uefi_drivers->item(c_row, 3)->setText(cboxDataClass->currentText());
}

void MainWindow::on_btnUEFIUnload_Add_clicked() {
  Method::add_OneLine(ui->table_uefi_Unload);
}

void MainWindow::on_btnUEFIUnload_Del_clicked() {
  del_item(ui->table_uefi_Unload);
}
