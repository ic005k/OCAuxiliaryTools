#include "mainwindow.h"

#include <fstream>
#include <iostream>
#include <iterator>

#include "Method.h"
#include "Plist.hpp"
#include "commands.h"
#include "plistparser.h"
#include "plistserializer.h"
#include "ui_aboutdialog.h"
#include "ui_mainwindow.h"

using namespace std;

#include <QBuffer>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>

QString PlistFileName;
QString SaveFileName;
QVector<QString> filelist;
QWidgetList wdlist;
QTableWidget* tableDatabase;
QRegExp regx("[A-Fa-f0-9]{0,1024}");
QRegExp regxNumber("^-?\[0-9]*$");
extern Method* mymethod;
QVector<QCheckBox*> chkDisplayLevel;
QVector<QCheckBox*> chk_ScanPolicy;
QVector<QCheckBox*> chk_PickerAttributes;
QVector<QCheckBox*> chk_ExposeSensitiveData;
QVector<QCheckBox*> chk_Target;

QString CurVerison = "20211001";
QString ocVer = "0.7.4";

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  Initialization = true;
  loading = true;

  loadLocal();

  title = "OC Auxiliary Tools   V" + CurVerison + " for OpenCore " + ocVer +
          " [*]  ";
  setWindowTitle(title);

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
  ui->actionUpgrade_Database->setEnabled(false);
  linuxOS = true;
#endif

  init_MainUI();

  init_setWindowModified();

  init_hardware_info();

  aboutDlg = new aboutDialog(this);
  myDatabase = new dlgDatabase(this);
  dlgOCV = new dlgOCValidate(this);
  dlgPar = new dlgParameters(this);
  dlgAutoUpdate = new AutoUpdateDialog(this);
  dlgSyncOC = new SyncOCDialog(this);

  QDir dir;
  if (dir.mkpath(QDir::homePath() + "/.config/QtOCC/")) {
  }

  setUIMargin();

  initui_booter();
  initui_dp();
  initui_kernel();
  initui_misc();
  initui_nvram();
  initui_PlatformInfo();
  initui_UEFI();
  initui_acpi();

  init_CopyPasteLine();

  setTableEditTriggers();

  //接受文件拖放打开
  this->setAcceptDrops(true);

  //最近打开的文件
  QCoreApplication::setOrganizationName("ic005k");
  QCoreApplication::setOrganizationDomain("github.com/ic005k");
  QCoreApplication::setApplicationName("OC Auxiliary Tools");

  m_recentFiles = new RecentFiles(this);
  // m_recentFiles->attachToMenuAfterItem(ui->menuFile, tr("Save As..."),
  // SLOT(recentOpen(QString)));
  m_recentFiles->setNumOfRecentFiles(10);

  // 最近打开的文件快捷通道
  initRecentFilesForToolBar();

  // 检查更新
  manager = new QNetworkAccessManager(this);
  connect(manager, SIGNAL(finished(QNetworkReply*)), this,
          SLOT(replyFinished(QNetworkReply*)));

  autoCheckUpdate = true;
  on_btnCheckUpdate();

  this->setWindowModified(false);

  loading = false;
  Initialization = false;
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::setUIMargin() {
  int m = 5;

  ui->gridLayout->setMargin(m);
  ui->centralwidget->layout()->setMargin(m);
  ui->centralwidget->layout()->setSpacing(m);

  ui->gridLayout_43->setMargin(m);
  ui->gridLayout_3->setMargin(m);
  ui->gridLayout_4->setMargin(m);
  ui->gridLayout_5->setMargin(m);
  ui->gridLayout_28->setMargin(m);

  ui->gridLayout_53->setMargin(m);
  ui->gridLayout_7->setMargin(m);
  ui->gridLayout_38->setMargin(m);

  ui->gridLayout_8->setMargin(m);
  ui->gridLayout_9->setMargin(m);
  ui->gridLayout_10->setMargin(m);

  ui->gridLayout_11->setMargin(m);
  ui->gridLayout_12->setMargin(m);
  ui->gridLayout_13->setMargin(m);
  ui->gridLayout_42->setMargin(m);
  ui->gridLayout_14->setMargin(m);

  ui->gridLayout_16->setMargin(m);

  ui->gridLayout_20->setMargin(m);
  ui->gridLayout_21->setMargin(m);
  ui->gridLayout_22->setMargin(m);

  ui->gridLayout_23->setMargin(m);
  ui->gridLayout_17->setMargin(m);
  ui->gridLayout_18->setMargin(m);
  ui->gridLayout_19->setMargin(m);

  ui->gridLayout_32->setMargin(m);

  ui->gridLayout_37->setMargin(m);

  ui->gridLayout_29->setMargin(m);
  ui->gridLayout_30->setMargin(m);
  ui->gridLayout_59->setMargin(m);

  ui->gridLayout_69->setMargin(m);
}

void MainWindow::recentOpen(QString filename) { openFile(filename); }

void MainWindow::initRecentFilesForToolBar() {
  QStringList rfList = m_recentFiles->getRecentFiles();
  reFileMenu->clear();
  for (int i = 0; i < rfList.count(); i++) {
    QFileInfo fi(rfList.at(i));
    QAction* act = new QAction(QString::number(i + 1) + " . " + fi.baseName());
    reFileMenu->addAction(act);
    connect(act, &QAction::triggered,
            [=]() { openFile(m_recentFiles->getRecentFiles().at(i)); });
  }
}

void MainWindow::openFile(QString PlistFileName) {
  if (!PlistFileName.isEmpty()) {
    setWindowTitle(title + PlistFileName);
    SaveFileName = PlistFileName;
  } else
    return;

  loading = true;

  //初始化
  mymethod->init_Table(-1);

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
  ParserKernel(map);
  ParserMisc(map);
  ParserNvram(map);
  ParserPlatformInfo(map);
  ParserUEFI(map);

  file.close();
  loading = false;

  ui->actionSave->setEnabled(true);

  undoStack->clear();

  if (!RefreshAllDatabase) {
    OpenFileValidate = true;
    on_actionOcvalidate_triggered();

    QSettings settings;
    QFileInfo fi(PlistFileName);
    settings.setValue("currentDirectory", fi.absolutePath());
    m_recentFiles->setMostRecentFile(PlistFileName);
    initRecentFilesForToolBar();

    QString strEFI = fi.path().mid(0, fi.path().count() - 3);
    QFileInfo f1(strEFI + "/OC");
    QFileInfo f2(strEFI + "/BOOT");
    QFileInfo f3(strEFI + "/OC/Drivers");
    if (f1.isDir() && f2.isDir() && f3.isDir())
      ui->actionUpgrade_OC->setEnabled(true);
    else
      ui->actionUpgrade_OC->setEnabled(false);
  }

  openFileAfter();

  this->setWindowModified(false);
  FindTextChange = true;
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

void MainWindow::on_btnOpen() {
  QFileDialog fd;

  PlistFileName = fd.getOpenFileName(this, "Config file", "",
                                     "Config file(*.plist);;All files(*.*)");

  openFile(PlistFileName);
}

void MainWindow::ParserACPI(QVariantMap map) {
  map = map["ACPI"].toMap();
  if (map.isEmpty()) return;

  //分析"Add"
  QVariantList map_add = map["Add"].toList();
  // qDebug() << map_add;
  ui->table_acpi_add->setRowCount(map_add.count());
  for (int i = 0; i < map_add.count(); i++) {
    QVariantMap map3 = map_add.at(i).toMap();

    QTableWidgetItem* newItem1;
    newItem1 = new QTableWidgetItem(map3["Path"].toString());
    ui->table_acpi_add->setItem(i, 0, newItem1);

    init_enabled_data(ui->table_acpi_add, i, 1, map3["Enabled"].toString());

    newItem1 = new QTableWidgetItem(map3["Comment"].toString());
    ui->table_acpi_add->setItem(i, 2, newItem1);
  }

  //分析Delete
  QVariantList map_del = map["Delete"].toList();

  ui->table_acpi_del->setRowCount(map_del.count());
  for (int i = 0; i < map_del.count(); i++) {
    QVariantMap map3 = map_del.at(i).toMap();

    QTableWidgetItem* newItem1;
    newItem1 = new QTableWidgetItem(
        ByteToHexStr(map3["TableSignature"].toByteArray()));
    ui->table_acpi_del->setItem(i, 0, newItem1);

    newItem1 =
        new QTableWidgetItem(ByteToHexStr(map3["OemTableId"].toByteArray()));
    ui->table_acpi_del->setItem(i, 1, newItem1);

    newItem1 = new QTableWidgetItem(map3["TableLength"].toString());
    ui->table_acpi_del->setItem(i, 2, newItem1);

    init_enabled_data(ui->table_acpi_del, i, 3, map3["All"].toString());

    init_enabled_data(ui->table_acpi_del, i, 4, map3["Enabled"].toString());

    newItem1 = new QTableWidgetItem(map3["Comment"].toString());
    ui->table_acpi_del->setItem(i, 5, newItem1);
  }

  //分析Patch
  QVariantList map_patch = map["Patch"].toList();
  ui->table_acpi_patch->setRowCount(map_patch.count());
  for (int i = 0; i < map_patch.count(); i++) {
    QVariantMap map3 = map_patch.at(i).toMap();

    QTableWidgetItem* newItem1;
    newItem1 = new QTableWidgetItem(
        ByteToHexStr(map3["TableSignature"].toByteArray()));
    ui->table_acpi_patch->setItem(i, 0, newItem1);

    newItem1 =
        new QTableWidgetItem(ByteToHexStr(map3["OemTableId"].toByteArray()));
    ui->table_acpi_patch->setItem(i, 1, newItem1);

    newItem1 = new QTableWidgetItem(map3["TableLength"].toString());
    ui->table_acpi_patch->setItem(i, 2, newItem1);

    ui->table_acpi_patch->setItem(
        i, 3, new QTableWidgetItem(ByteToHexStr(map3["Find"].toByteArray())));

    newItem1 =
        new QTableWidgetItem(ByteToHexStr(map3["Replace"].toByteArray()));
    ui->table_acpi_patch->setItem(i, 4, newItem1);

    newItem1 = new QTableWidgetItem(map3["Comment"].toString());
    ui->table_acpi_patch->setItem(i, 5, newItem1);

    newItem1 = new QTableWidgetItem(map3["Mask"].toString());
    ui->table_acpi_patch->setItem(i, 6, newItem1);

    newItem1 = new QTableWidgetItem(map3["ReplaceMask"].toString());
    ui->table_acpi_patch->setItem(i, 7, newItem1);

    newItem1 = new QTableWidgetItem(map3["Count"].toString());
    newItem1->setTextAlignment(Qt::AlignCenter);
    ui->table_acpi_patch->setItem(i, 8, newItem1);

    newItem1 = new QTableWidgetItem(map3["Limit"].toString());
    newItem1->setTextAlignment(Qt::AlignCenter);
    ui->table_acpi_patch->setItem(i, 9, newItem1);

    newItem1 = new QTableWidgetItem(map3["Skip"].toString());
    newItem1->setTextAlignment(Qt::AlignCenter);
    ui->table_acpi_patch->setItem(i, 10, newItem1);

    init_enabled_data(ui->table_acpi_patch, i, 11, map3["Enabled"].toString());

    ui->table_acpi_patch->setItem(
        i, 12, new QTableWidgetItem(map3["Base"].toString()));

    newItem1 = new QTableWidgetItem(map3["BaseSkip"].toString());
    newItem1->setTextAlignment(Qt::AlignCenter);
    ui->table_acpi_patch->setItem(i, 13, newItem1);
  }

  //分析Quirks
  QVariantMap map_quirks = map["Quirks"].toMap();
  getValue(map_quirks, ui->tabACPI4);
}

void MainWindow::initui_acpi() {
  ui->btnExportMaster->setText(tr("Export") + "  ACPI");
  ui->btnImportMaster->setText(tr("Import") + "  ACPI");

  QTableWidgetItem* id0;
  QStringList fieldList;

  // ACPI-Add
  ui->table_acpi_add->setColumnCount(3);

  fieldList.clear();
  fieldList.append(tr("Path"));
  fieldList.append(tr("Enabled"));
  fieldList.append(tr("Comment"));
  ui->table_acpi_add->setHorizontalHeaderLabels(fieldList);
  ui->table_acpi_add->setAlternatingRowColors(true);  //底色交替显示
  ui->btnUp->setVisible(true);
  ui->btnDown->setVisible(true);
  ui->checkACPIAdd->setVisible(false);

  // ACPI-Delete
  id0 = new QTableWidgetItem(tr("TableSignature"));
  ui->table_acpi_del->setHorizontalHeaderItem(0, id0);

  // ui->table_acpi_del->setColumnWidth(1, 350);
  id0 = new QTableWidgetItem(tr("OemTableId"));
  ui->table_acpi_del->setHorizontalHeaderItem(1, id0);

  id0 = new QTableWidgetItem(tr("TableLength"));
  ui->table_acpi_del->setHorizontalHeaderItem(2, id0);

  id0 = new QTableWidgetItem(tr("All"));
  ui->table_acpi_del->setHorizontalHeaderItem(3, id0);

  id0 = new QTableWidgetItem(tr("Enabled"));
  ui->table_acpi_del->setHorizontalHeaderItem(4, id0);

  id0 = new QTableWidgetItem(tr("Comment"));
  ui->table_acpi_del->setHorizontalHeaderItem(5, id0);

  ui->table_acpi_del->setAlternatingRowColors(true);

  // ACPI-Patch
  ui->table_acpi_patch->setColumnCount(14);

  id0 = new QTableWidgetItem(tr("TableSignature"));
  ui->table_acpi_patch->setHorizontalHeaderItem(0, id0);

  id0 = new QTableWidgetItem(tr("OemTableId"));
  ui->table_acpi_patch->setHorizontalHeaderItem(1, id0);

  id0 = new QTableWidgetItem(tr("TableLength"));
  ui->table_acpi_patch->setHorizontalHeaderItem(2, id0);

  id0 = new QTableWidgetItem(tr("Find"));
  ui->table_acpi_patch->setHorizontalHeaderItem(3, id0);

  id0 = new QTableWidgetItem(tr("Replace"));
  ui->table_acpi_patch->setHorizontalHeaderItem(4, id0);

  id0 = new QTableWidgetItem(tr("Comment"));
  ui->table_acpi_patch->setHorizontalHeaderItem(5, id0);

  id0 = new QTableWidgetItem(tr("Mask"));
  ui->table_acpi_patch->setHorizontalHeaderItem(6, id0);

  id0 = new QTableWidgetItem(tr("ReplaceMask"));
  ui->table_acpi_patch->setHorizontalHeaderItem(7, id0);

  id0 = new QTableWidgetItem(tr("Count"));
  ui->table_acpi_patch->setHorizontalHeaderItem(8, id0);

  id0 = new QTableWidgetItem(tr("Limit"));
  ui->table_acpi_patch->setHorizontalHeaderItem(9, id0);

  id0 = new QTableWidgetItem(tr("Skip"));
  ui->table_acpi_patch->setHorizontalHeaderItem(10, id0);

  id0 = new QTableWidgetItem(tr("Enabled"));
  ui->table_acpi_patch->setHorizontalHeaderItem(11, id0);

  id0 = new QTableWidgetItem(tr("Base"));
  ui->table_acpi_patch->setHorizontalHeaderItem(12, id0);

  id0 = new QTableWidgetItem(tr("BaseSkip"));
  ui->table_acpi_patch->setHorizontalHeaderItem(13, id0);

  ui->table_acpi_patch->setAlternatingRowColors(true);
}

void MainWindow::initui_booter() {
  QTableWidgetItem* id0;

  // Patch

  ui->table_Booter_patch->setColumnCount(11);

  ui->table_Booter_patch->setColumnWidth(0, 300);
  id0 = new QTableWidgetItem(tr("Identifier"));
  ui->table_Booter_patch->setHorizontalHeaderItem(0, id0);

  ui->table_Booter_patch->setColumnWidth(1, 350);
  id0 = new QTableWidgetItem(tr("Comment"));
  ui->table_Booter_patch->setHorizontalHeaderItem(1, id0);

  ui->table_Booter_patch->setColumnWidth(2, 300);
  id0 = new QTableWidgetItem(tr("Find"));
  ui->table_Booter_patch->setHorizontalHeaderItem(2, id0);

  ui->table_Booter_patch->setColumnWidth(3, 300);
  id0 = new QTableWidgetItem(tr("Replace"));
  ui->table_Booter_patch->setHorizontalHeaderItem(3, id0);

  ui->table_Booter_patch->setColumnWidth(4, 350);
  id0 = new QTableWidgetItem(tr("Mask"));
  ui->table_Booter_patch->setHorizontalHeaderItem(4, id0);

  ui->table_Booter_patch->setColumnWidth(5, 350);
  id0 = new QTableWidgetItem(tr("ReplaceMask"));
  ui->table_Booter_patch->setHorizontalHeaderItem(5, id0);

  ui->table_Booter_patch->setColumnWidth(0, 350);
  id0 = new QTableWidgetItem(tr("Count"));
  ui->table_Booter_patch->setHorizontalHeaderItem(6, id0);

  ui->table_Booter_patch->setColumnWidth(0, 350);
  id0 = new QTableWidgetItem(tr("Limit"));
  ui->table_Booter_patch->setHorizontalHeaderItem(7, id0);

  ui->table_Booter_patch->setColumnWidth(0, 350);
  id0 = new QTableWidgetItem(tr("Skip"));
  ui->table_Booter_patch->setHorizontalHeaderItem(8, id0);

  ui->table_Booter_patch->setColumnWidth(0, 350);
  id0 = new QTableWidgetItem(tr("Enabled"));
  ui->table_Booter_patch->setHorizontalHeaderItem(9, id0);

  id0 = new QTableWidgetItem(tr("Arch"));
  ui->table_Booter_patch->setHorizontalHeaderItem(10, id0);

  ui->table_Booter_patch->setAlternatingRowColors(true);

  // MmioWhitelist

  ui->table_booter->setColumnWidth(0, 450);
  id0 = new QTableWidgetItem(tr("Address"));
  ui->table_booter->setHorizontalHeaderItem(0, id0);

  id0 = new QTableWidgetItem(tr("Enabled"));
  ui->table_booter->setHorizontalHeaderItem(1, id0);

  id0 = new QTableWidgetItem(tr("Comment"));
  ui->table_booter->setHorizontalHeaderItem(2, id0);

  ui->table_booter->setAlternatingRowColors(true);
}

void MainWindow::ParserBooter(QVariantMap map) {
  map = map["Booter"].toMap();
  if (map.isEmpty()) return;

  // Patch
  QVariantList map_patch = map["Patch"].toList();

  ui->table_Booter_patch->setRowCount(map_patch.count());
  for (int i = 0; i < map_patch.count(); i++) {
    QVariantMap map3 = map_patch.at(i).toMap();

    QByteArray ba = map3["Find"].toByteArray();

    QTableWidgetItem* newItem1;

    newItem1 = new QTableWidgetItem(map3["Identifier"].toString());
    ui->table_Booter_patch->setItem(i, 0, newItem1);

    newItem1 = new QTableWidgetItem(map3["Comment"].toString());
    ui->table_Booter_patch->setItem(i, 1, newItem1);

    //此时需要将ASCII转换成HEX
    QByteArray tohex = map3["Find"].toByteArray();
    QString va = tohex.toHex().toUpper();
    newItem1 = new QTableWidgetItem(va);
    ui->table_Booter_patch->setItem(i, 2, newItem1);

    tohex = map3["Replace"].toByteArray();
    va = tohex.toHex().toUpper();
    newItem1 = new QTableWidgetItem(va);
    ui->table_Booter_patch->setItem(i, 3, newItem1);

    tohex = map3["Mask"].toByteArray();
    va = tohex.toHex().toUpper();
    newItem1 = new QTableWidgetItem(va);
    ui->table_Booter_patch->setItem(i, 4, newItem1);

    tohex = map3["ReplaceMask"].toByteArray();
    va = tohex.toHex().toUpper();
    newItem1 = new QTableWidgetItem(va);
    ui->table_Booter_patch->setItem(i, 5, newItem1);

    newItem1 = new QTableWidgetItem(map3["Count"].toString());
    newItem1->setTextAlignment(Qt::AlignCenter);
    ui->table_Booter_patch->setItem(i, 6, newItem1);

    newItem1 = new QTableWidgetItem(map3["Limit"].toString());
    newItem1->setTextAlignment(Qt::AlignCenter);
    ui->table_Booter_patch->setItem(i, 7, newItem1);

    newItem1 = new QTableWidgetItem(map3["Skip"].toString());
    newItem1->setTextAlignment(Qt::AlignCenter);
    ui->table_Booter_patch->setItem(i, 8, newItem1);

    init_enabled_data(ui->table_Booter_patch, i, 9, map3["Enabled"].toString());

    newItem1 = new QTableWidgetItem(map3["Arch"].toString());
    newItem1->setTextAlignment(Qt::AlignCenter);
    ui->table_Booter_patch->setItem(i, 10, newItem1);
  }

  // MmioWhitelist

  QVariantList map_add = map["MmioWhitelist"].toList();

  ui->table_booter->setRowCount(map_add.count());
  for (int i = 0; i < map_add.count(); i++) {
    QVariantMap map3 = map_add.at(i).toMap();

    QTableWidgetItem* newItem1;
    newItem1 = new QTableWidgetItem(map3["Address"].toString());
    ui->table_booter->setItem(i, 0, newItem1);

    init_enabled_data(ui->table_booter, i, 1, map3["Enabled"].toString());

    newItem1 = new QTableWidgetItem(map3["Comment"].toString());
    ui->table_booter->setItem(i, 2, newItem1);
  }

  // Quirks
  QVariantMap map_quirks = map["Quirks"].toMap();
  getValue(map_quirks, ui->tabBooter3);
}

void MainWindow::initui_dp() {
  QTableWidgetItem* id0;

  // Add
  ui->table_dp_add0->setMinimumWidth(200);
  ui->table_dp_add0->setMaximumWidth(350);

  id0 = new QTableWidgetItem(tr("PCILists"));
  ui->table_dp_add0->setHorizontalHeaderItem(0, id0);
  ui->table_dp_add0->setAlternatingRowColors(true);
  ui->table_dp_add0->horizontalHeader()->setStretchLastSection(true);

  ui->table_dp_add->setColumnWidth(0, 300);

  id0 = new QTableWidgetItem(tr("Key"));
  ui->table_dp_add->setHorizontalHeaderItem(0, id0);

  id0 = new QTableWidgetItem(tr("Class"));
  ui->table_dp_add->setHorizontalHeaderItem(1, id0);

  ui->table_dp_add->setColumnWidth(2, 260);
  id0 = new QTableWidgetItem(tr("Value"));
  ui->table_dp_add->setHorizontalHeaderItem(2, id0);

  ui->table_dp_add->setAlternatingRowColors(true);

  // 分割窗口
  QSplitter* splitterMain = new QSplitter(Qt::Horizontal, this);
  splitterMain->addWidget(ui->table_dp_add0);
  splitterMain->addWidget(ui->table_dp_add);
  ui->gridLayout_dp_add->addWidget(splitterMain);

  // Delete

  id0 = new QTableWidgetItem(tr("PCILists"));
  ui->table_dp_del0->setHorizontalHeaderItem(0, id0);

  ui->table_dp_del0->setAlternatingRowColors(true);
  ui->table_dp_del0->horizontalHeader()->setStretchLastSection(true);

  id0 = new QTableWidgetItem(tr("Value"));
  ui->table_dp_del->setHorizontalHeaderItem(0, id0);

  ui->table_dp_del->setAlternatingRowColors(true);
  ui->table_dp_del->horizontalHeader()->setStretchLastSection(true);
}

void MainWindow::ParserDP(QVariantMap map) {
  map = map["DeviceProperties"].toMap();
  if (map.isEmpty()) return;

  // Add
  QVariantMap map_add = map["Add"].toMap();

  QVariantMap map_sub;

  ui->table_dp_add0->setRowCount(map_add.count());
  QTableWidgetItem* newItem1;
  for (int i = 0; i < map_add.count(); i++) {
    newItem1 = new QTableWidgetItem(map_add.keys().at(i));
    ui->table_dp_add0->setItem(i, 0, newItem1);

    //加载子条目
    map_sub = map_add[map_add.keys().at(i)].toMap();
    ui->table_dp_add->setRowCount(map_sub.keys().count());  //子键的个数

    for (int j = 0; j < map_sub.keys().count(); j++) {
      // QTableWidgetItem *newItem1;
      newItem1 = new QTableWidgetItem(map_sub.keys().at(j));  //键
      ui->table_dp_add->setItem(j, 0, newItem1);

      QString dtype = map_sub[map_sub.keys().at(j)].typeName();
      QString ztype;
      if (dtype == "QByteArray") ztype = "Data";
      if (dtype == "QString") ztype = "String";
      if (dtype == "qlonglong") ztype = "Number";

      newItem1 = new QTableWidgetItem(ztype);  //数据类型
      newItem1->setTextAlignment(Qt::AlignCenter);
      ui->table_dp_add->setItem(j, 1, newItem1);

      QString type_name = map_sub[map_sub.keys().at(j)].typeName();
      if (type_name == "QByteArray") {
        QByteArray tohex = map_sub[map_sub.keys().at(j)].toByteArray();
        QString va = tohex.toHex().toUpper();
        newItem1 = new QTableWidgetItem(va);

      } else
        newItem1 =
            new QTableWidgetItem(map_sub[map_sub.keys().at(j)].toString());
      ui->table_dp_add->setItem(j, 2, newItem1);
    }

    //保存子条目里面的数据，以便以后加载
    write_ini(ui->table_dp_add0, ui->table_dp_add, i);
  }

  int last = ui->table_dp_add0->rowCount();
  ui->table_dp_add0->setCurrentCell(last - 1, 0);

  // Delete
  init_value(map["Delete"].toMap(), ui->table_dp_del0, ui->table_dp_del);
}

void MainWindow::initui_kernel() {
  QTableWidgetItem* id0;
  // Add
  ui->table_kernel_add->setColumnCount(8);

  ui->table_kernel_add->setColumnWidth(0, 250);
  id0 = new QTableWidgetItem(tr("BundlePath"));
  ui->table_kernel_add->setHorizontalHeaderItem(0, id0);

  ui->table_kernel_add->setColumnWidth(1, 250);
  id0 = new QTableWidgetItem(tr("Comment"));
  ui->table_kernel_add->setHorizontalHeaderItem(1, id0);

  ui->table_kernel_add->setColumnWidth(2, 250);
  id0 = new QTableWidgetItem(tr("ExecutablePath"));
  ui->table_kernel_add->setHorizontalHeaderItem(2, id0);

  ui->table_kernel_add->setColumnWidth(0, 250);
  id0 = new QTableWidgetItem(tr("PlistPath"));
  ui->table_kernel_add->setHorizontalHeaderItem(3, id0);

  ui->table_kernel_add->setColumnWidth(0, 250);
  id0 = new QTableWidgetItem(tr("MinKernel"));
  ui->table_kernel_add->setHorizontalHeaderItem(4, id0);

  ui->table_kernel_add->setColumnWidth(0, 250);
  id0 = new QTableWidgetItem(tr("MaxKernel"));
  ui->table_kernel_add->setHorizontalHeaderItem(5, id0);

  ui->table_kernel_add->setColumnWidth(0, 250);
  id0 = new QTableWidgetItem(tr("Enabled"));
  ui->table_kernel_add->setHorizontalHeaderItem(6, id0);

  ui->table_kernel_add->setColumnWidth(0, 250);
  id0 = new QTableWidgetItem(tr("Arch"));
  ui->table_kernel_add->setHorizontalHeaderItem(7, id0);

  ui->table_kernel_add->setAlternatingRowColors(true);
  // ui->table_kernel_add->horizontalHeader()->setStretchLastSection(true);
  ui->checkKernelAdd->setVisible(false);

  // Block
  ui->table_kernel_block->setColumnCount(6);
  ui->table_kernel_block->setColumnWidth(0, 350);
  id0 = new QTableWidgetItem(tr("Identifier"));
  ui->table_kernel_block->setHorizontalHeaderItem(0, id0);

  ui->table_kernel_block->setColumnWidth(1, 350);
  id0 = new QTableWidgetItem(tr("Comment"));
  ui->table_kernel_block->setHorizontalHeaderItem(1, id0);

  ui->table_kernel_block->setColumnWidth(2, 250);
  id0 = new QTableWidgetItem(tr("MinKernel"));
  ui->table_kernel_block->setHorizontalHeaderItem(2, id0);

  ui->table_kernel_block->setColumnWidth(3, 250);
  id0 = new QTableWidgetItem(tr("MaxKernel"));
  ui->table_kernel_block->setHorizontalHeaderItem(3, id0);

  ui->table_kernel_block->setColumnWidth(0, 250);
  id0 = new QTableWidgetItem(tr("Enabled"));
  ui->table_kernel_block->setHorizontalHeaderItem(4, id0);

  id0 = new QTableWidgetItem(tr("Arch"));
  ui->table_kernel_block->setHorizontalHeaderItem(5, id0);

  ui->table_kernel_block->setAlternatingRowColors(true);

  // Force
  ui->table_kernel_Force->setColumnCount(9);

  ui->table_kernel_Force->setColumnWidth(0, 250);
  id0 = new QTableWidgetItem(tr("BundlePath"));
  ui->table_kernel_Force->setHorizontalHeaderItem(0, id0);

  ui->table_kernel_Force->setColumnWidth(1, 250);
  id0 = new QTableWidgetItem(tr("Comment"));
  ui->table_kernel_Force->setHorizontalHeaderItem(1, id0);

  ui->table_kernel_Force->setColumnWidth(2, 250);
  id0 = new QTableWidgetItem(tr("ExecutablePath"));
  ui->table_kernel_Force->setHorizontalHeaderItem(2, id0);

  ui->table_kernel_Force->setColumnWidth(3, 250);
  id0 = new QTableWidgetItem(tr("Identifier"));
  ui->table_kernel_Force->setHorizontalHeaderItem(3, id0);

  ui->table_kernel_Force->setColumnWidth(0, 250);
  id0 = new QTableWidgetItem(tr("PlistPath"));
  ui->table_kernel_Force->setHorizontalHeaderItem(4, id0);

  ui->table_kernel_Force->setColumnWidth(0, 250);
  id0 = new QTableWidgetItem(tr("MinKernel"));
  ui->table_kernel_Force->setHorizontalHeaderItem(5, id0);

  ui->table_kernel_Force->setColumnWidth(0, 250);
  id0 = new QTableWidgetItem(tr("MaxKernel"));
  ui->table_kernel_Force->setHorizontalHeaderItem(6, id0);

  ui->table_kernel_Force->setColumnWidth(0, 250);
  id0 = new QTableWidgetItem(tr("Enabled"));
  ui->table_kernel_Force->setHorizontalHeaderItem(7, id0);

  ui->table_kernel_Force->setColumnWidth(0, 250);
  id0 = new QTableWidgetItem(tr("Arch"));
  ui->table_kernel_Force->setHorizontalHeaderItem(8, id0);

  ui->table_kernel_Force->setAlternatingRowColors(true);

  // Patch
  ui->table_kernel_patch->setColumnCount(14);
  ui->table_kernel_patch->setColumnWidth(0, 300);
  id0 = new QTableWidgetItem(tr("Identifier"));
  ui->table_kernel_patch->setHorizontalHeaderItem(0, id0);

  ui->table_kernel_patch->setColumnWidth(0, 350);
  id0 = new QTableWidgetItem(tr("Base"));
  ui->table_kernel_patch->setHorizontalHeaderItem(1, id0);

  ui->table_kernel_patch->setColumnWidth(0, 350);
  id0 = new QTableWidgetItem(tr("Comment"));
  ui->table_kernel_patch->setHorizontalHeaderItem(2, id0);

  ui->table_kernel_patch->setColumnWidth(3, 300);
  id0 = new QTableWidgetItem(tr("Find"));
  ui->table_kernel_patch->setHorizontalHeaderItem(3, id0);

  ui->table_kernel_patch->setColumnWidth(4, 300);
  id0 = new QTableWidgetItem(tr("Replace"));
  ui->table_kernel_patch->setHorizontalHeaderItem(4, id0);

  ui->table_kernel_patch->setColumnWidth(0, 350);
  id0 = new QTableWidgetItem(tr("Mask"));
  ui->table_kernel_patch->setHorizontalHeaderItem(5, id0);

  ui->table_kernel_patch->setColumnWidth(0, 350);
  id0 = new QTableWidgetItem(tr("ReplaceMask"));
  ui->table_kernel_patch->setHorizontalHeaderItem(6, id0);

  ui->table_kernel_patch->setColumnWidth(0, 350);
  id0 = new QTableWidgetItem(tr("MinKernel"));
  ui->table_kernel_patch->setHorizontalHeaderItem(7, id0);

  ui->table_kernel_patch->setColumnWidth(0, 350);
  id0 = new QTableWidgetItem(tr("MaxKernel"));
  ui->table_kernel_patch->setHorizontalHeaderItem(8, id0);

  ui->table_kernel_patch->setColumnWidth(0, 350);
  id0 = new QTableWidgetItem(tr("Count"));
  ui->table_kernel_patch->setHorizontalHeaderItem(9, id0);

  ui->table_kernel_patch->setColumnWidth(0, 350);
  id0 = new QTableWidgetItem(tr("Limit"));
  ui->table_kernel_patch->setHorizontalHeaderItem(10, id0);

  ui->table_kernel_patch->setColumnWidth(0, 350);
  id0 = new QTableWidgetItem(tr("Skip"));
  ui->table_kernel_patch->setHorizontalHeaderItem(11, id0);

  ui->table_kernel_patch->setColumnWidth(0, 350);
  id0 = new QTableWidgetItem(tr("Enabled"));
  ui->table_kernel_patch->setHorizontalHeaderItem(12, id0);

  id0 = new QTableWidgetItem(tr("Arch"));
  ui->table_kernel_patch->setHorizontalHeaderItem(13, id0);

  ui->table_kernel_patch->setAlternatingRowColors(true);

  // Scheme
  ui->cboxKernelArch->addItem("Auto");
  ui->cboxKernelArch->addItem("i386");
  ui->cboxKernelArch->addItem("i386-user32");
  ui->cboxKernelArch->addItem("x86_64");

  ui->cboxKernelCache->addItem("Auto");
  ui->cboxKernelCache->addItem("Cacheless");
  ui->cboxKernelCache->addItem("Mkext");
  ui->cboxKernelCache->addItem("Prelinked");
}

void MainWindow::ParserKernel(QVariantMap map) {
  map = map["Kernel"].toMap();
  if (map.isEmpty()) return;

  //分析"Add"
  QVariantList map_add = map["Add"].toList();

  ui->table_kernel_add->setRowCount(map_add.count());
  for (int i = 0; i < map_add.count(); i++) {
    QVariantMap map3 = map_add.at(i).toMap();

    QTableWidgetItem* newItem1;

    newItem1 = new QTableWidgetItem(map3["BundlePath"].toString());
    ui->table_kernel_add->setItem(i, 0, newItem1);

    newItem1 = new QTableWidgetItem(map3["Comment"].toString());
    ui->table_kernel_add->setItem(i, 1, newItem1);

    newItem1 = new QTableWidgetItem(map3["ExecutablePath"].toString());
    ui->table_kernel_add->setItem(i, 2, newItem1);

    newItem1 = new QTableWidgetItem(map3["PlistPath"].toString());
    ui->table_kernel_add->setItem(i, 3, newItem1);

    newItem1 = new QTableWidgetItem(map3["MinKernel"].toString());
    newItem1->setTextAlignment(Qt::AlignCenter);
    ui->table_kernel_add->setItem(i, 4, newItem1);

    newItem1 = new QTableWidgetItem(map3["MaxKernel"].toString());
    newItem1->setTextAlignment(Qt::AlignCenter);
    ui->table_kernel_add->setItem(i, 5, newItem1);

    init_enabled_data(ui->table_kernel_add, i, 6, map3["Enabled"].toString());

    newItem1 = new QTableWidgetItem(map3["Arch"].toString());
    if (map3["Arch"].toString().trimmed() == "")
      newItem1 = new QTableWidgetItem("Any");
    newItem1->setTextAlignment(Qt::AlignCenter);
    ui->table_kernel_add->setItem(i, 7, newItem1);
  }

  // Block
  QVariantList map_block = map["Block"].toList();

  ui->table_kernel_block->setRowCount(map_block.count());
  for (int i = 0; i < map_block.count(); i++) {
    QVariantMap map3 = map_block.at(i).toMap();

    QTableWidgetItem* newItem1;

    newItem1 = new QTableWidgetItem(map3["Identifier"].toString());
    ui->table_kernel_block->setItem(i, 0, newItem1);

    newItem1 = new QTableWidgetItem(map3["Comment"].toString());
    ui->table_kernel_block->setItem(i, 1, newItem1);

    newItem1 = new QTableWidgetItem(map3["MinKernel"].toString());
    newItem1->setTextAlignment(Qt::AlignCenter);
    ui->table_kernel_block->setItem(i, 2, newItem1);

    newItem1 = new QTableWidgetItem(map3["MaxKernel"].toString());
    newItem1->setTextAlignment(Qt::AlignCenter);
    ui->table_kernel_block->setItem(i, 3, newItem1);

    init_enabled_data(ui->table_kernel_block, i, 4, map3["Enabled"].toString());

    newItem1 = new QTableWidgetItem(map3["Arch"].toString());
    if (map3["Arch"].toString().trimmed() == "")
      newItem1 = new QTableWidgetItem("Any");
    newItem1->setTextAlignment(Qt::AlignCenter);
    ui->table_kernel_block->setItem(i, 5, newItem1);
  }

  //分析"Force"
  QVariantList map_Force = map["Force"].toList();

  ui->table_kernel_Force->setRowCount(map_Force.count());
  for (int i = 0; i < map_Force.count(); i++) {
    QVariantMap map3 = map_Force.at(i).toMap();

    QTableWidgetItem* newItem1;

    newItem1 = new QTableWidgetItem(map3["BundlePath"].toString());
    ui->table_kernel_Force->setItem(i, 0, newItem1);

    newItem1 = new QTableWidgetItem(map3["Comment"].toString());
    ui->table_kernel_Force->setItem(i, 1, newItem1);

    newItem1 = new QTableWidgetItem(map3["ExecutablePath"].toString());
    ui->table_kernel_Force->setItem(i, 2, newItem1);

    newItem1 = new QTableWidgetItem(map3["Identifier"].toString());
    ui->table_kernel_Force->setItem(i, 3, newItem1);

    newItem1 = new QTableWidgetItem(map3["PlistPath"].toString());
    ui->table_kernel_Force->setItem(i, 4, newItem1);

    newItem1 = new QTableWidgetItem(map3["MinKernel"].toString());
    newItem1->setTextAlignment(Qt::AlignCenter);
    ui->table_kernel_Force->setItem(i, 5, newItem1);

    newItem1 = new QTableWidgetItem(map3["MaxKernel"].toString());
    newItem1->setTextAlignment(Qt::AlignCenter);
    ui->table_kernel_Force->setItem(i, 6, newItem1);

    init_enabled_data(ui->table_kernel_Force, i, 7, map3["Enabled"].toString());

    newItem1 = new QTableWidgetItem(map3["Arch"].toString());
    if (map3["Arch"].toString().trimmed() == "")
      newItem1 = new QTableWidgetItem("Any");
    newItem1->setTextAlignment(Qt::AlignCenter);
    ui->table_kernel_Force->setItem(i, 8, newItem1);
  }

  // Patch
  QVariantList map_patch = map["Patch"].toList();

  ui->table_kernel_patch->setRowCount(map_patch.count());
  for (int i = 0; i < map_patch.count(); i++) {
    QVariantMap map3 = map_patch.at(i).toMap();

    QByteArray ba = map3["Find"].toByteArray();

    QTableWidgetItem* newItem1;

    newItem1 = new QTableWidgetItem(map3["Identifier"].toString());
    ui->table_kernel_patch->setItem(i, 0, newItem1);

    newItem1 = new QTableWidgetItem(map3["Base"].toString());
    ui->table_kernel_patch->setItem(i, 1, newItem1);

    newItem1 = new QTableWidgetItem(map3["Comment"].toString());
    ui->table_kernel_patch->setItem(i, 2, newItem1);

    //此时需要将ASCII转换成HEX
    QByteArray tohex = map3["Find"].toByteArray();
    QString va = tohex.toHex().toUpper();
    newItem1 = new QTableWidgetItem(va);
    ui->table_kernel_patch->setItem(i, 3, newItem1);

    tohex = map3["Replace"].toByteArray();
    va = tohex.toHex().toUpper();
    newItem1 = new QTableWidgetItem(va);
    ui->table_kernel_patch->setItem(i, 4, newItem1);

    tohex = map3["Mask"].toByteArray();
    va = tohex.toHex().toUpper();
    newItem1 = new QTableWidgetItem(va);
    ui->table_kernel_patch->setItem(i, 5, newItem1);

    tohex = map3["ReplaceMask"].toByteArray();
    va = tohex.toHex().toUpper();
    newItem1 = new QTableWidgetItem(va);
    ui->table_kernel_patch->setItem(i, 6, newItem1);

    newItem1 = new QTableWidgetItem(map3["MinKernel"].toString());
    newItem1->setTextAlignment(Qt::AlignCenter);
    ui->table_kernel_patch->setItem(i, 7, newItem1);

    newItem1 = new QTableWidgetItem(map3["MaxKernel"].toString());
    newItem1->setTextAlignment(Qt::AlignCenter);
    ui->table_kernel_patch->setItem(i, 8, newItem1);

    newItem1 = new QTableWidgetItem(map3["Count"].toString());
    newItem1->setTextAlignment(Qt::AlignCenter);
    ui->table_kernel_patch->setItem(i, 9, newItem1);

    newItem1 = new QTableWidgetItem(map3["Limit"].toString());
    newItem1->setTextAlignment(Qt::AlignCenter);
    ui->table_kernel_patch->setItem(i, 10, newItem1);

    newItem1 = new QTableWidgetItem(map3["Skip"].toString());
    newItem1->setTextAlignment(Qt::AlignCenter);
    ui->table_kernel_patch->setItem(i, 11, newItem1);

    init_enabled_data(ui->table_kernel_patch, i, 12,
                      map3["Enabled"].toString());

    newItem1 = new QTableWidgetItem(map3["Arch"].toString());
    if (map3["Arch"].toString().trimmed() == "")
      newItem1 = new QTableWidgetItem("Any");
    newItem1->setTextAlignment(Qt::AlignCenter);
    ui->table_kernel_patch->setItem(i, 13, newItem1);
  }

  // Emulate
  QVariantMap map_Emulate = map["Emulate"].toMap();
  getValue(map_Emulate, ui->tabKernel5);

  // Quirks
  QVariantMap map_quirks = map["Quirks"].toMap();
  getValue(map_quirks, ui->tabKernel6);

  // Scheme
  QVariantMap map_Scheme = map["Scheme"].toMap();
  getValue(map_Scheme, ui->tabKernel7);
}

void MainWindow::initui_misc() {
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
  pe.setColor(QPalette::Background, Qt::black);
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

  //添加颜色下拉框,字色
  QStringList itemList;
  for (int i = 0; i < ui->cboxTextColor->count(); i++) {
    itemList.append(ui->cboxTextColor->itemText(i));
  }
  ui->cboxTextColor->clear();

  for (int i = 0; i < textColor.count(); i++) {
    QPixmap pix(QSize(100, 20));
    pix.fill(QColor(textColor.at(i)));
    ui->cboxTextColor->addItem(QIcon(pix), itemList.at(i));
    ui->cboxTextColor->setIconSize(QSize(70, 20));
    ui->cboxTextColor->setSizeAdjustPolicy(QComboBox::AdjustToContents);
  }

  // 背景色
  itemList.clear();
  for (int i = 0; i < ui->cboxBackColor->count(); i++) {
    itemList.append(ui->cboxBackColor->itemText(i));
  }
  ui->cboxBackColor->clear();

  for (int i = 0; i < backColor.count(); i++) {
    QPixmap pix(QSize(100, 20));
    pix.fill(QColor(backColor.at(i)));
    ui->cboxBackColor->addItem(QIcon(pix), itemList.at(i));
    ui->cboxBackColor->setIconSize(QSize(70, 20));
    ui->cboxBackColor->setSizeAdjustPolicy(QComboBox::AdjustToContents);
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

  // BlessOverride
  QTableWidgetItem* id0;
  id0 = new QTableWidgetItem(tr("BlessOverride"));
  ui->tableBlessOverride->setHorizontalHeaderItem(0, id0);

  ui->tableBlessOverride->setAlternatingRowColors(true);
  ui->tableBlessOverride->horizontalHeader()->setStretchLastSection(true);

  // Entries
  ui->tableEntries->setColumnCount(8);

  ui->tableEntries->setColumnWidth(0, 550);
  id0 = new QTableWidgetItem(tr("Path"));
  ui->tableEntries->setHorizontalHeaderItem(0, id0);

  id0 = new QTableWidgetItem(tr("Arguments"));
  ui->tableEntries->setHorizontalHeaderItem(1, id0);

  id0 = new QTableWidgetItem(tr("Name"));
  ui->tableEntries->setHorizontalHeaderItem(2, id0);

  ui->tableEntries->setColumnWidth(3, 250);
  id0 = new QTableWidgetItem(tr("Comment"));
  ui->tableEntries->setHorizontalHeaderItem(3, id0);

  id0 = new QTableWidgetItem(tr("Auxiliary"));
  ui->tableEntries->setHorizontalHeaderItem(4, id0);

  id0 = new QTableWidgetItem(tr("Enabled"));
  ui->tableEntries->setHorizontalHeaderItem(5, id0);

  id0 = new QTableWidgetItem(tr("TextMode"));
  ui->tableEntries->setHorizontalHeaderItem(6, id0);

  id0 = new QTableWidgetItem(tr("Flavour"));
  ui->tableEntries->setHorizontalHeaderItem(7, id0);

  ui->tableEntries->setAlternatingRowColors(true);

  // Tools
  ui->tableTools->setColumnCount(9);

  ui->tableTools->setColumnWidth(0, 450);
  id0 = new QTableWidgetItem(tr("Path"));
  ui->tableTools->setHorizontalHeaderItem(0, id0);

  id0 = new QTableWidgetItem(tr("Arguments"));
  ui->tableTools->setHorizontalHeaderItem(1, id0);

  id0 = new QTableWidgetItem(tr("Name"));
  ui->tableTools->setHorizontalHeaderItem(2, id0);

  ui->tableTools->setColumnWidth(3, 250);
  id0 = new QTableWidgetItem(tr("Comment"));
  ui->tableTools->setHorizontalHeaderItem(3, id0);

  id0 = new QTableWidgetItem(tr("Auxiliary"));
  ui->tableTools->setHorizontalHeaderItem(4, id0);

  id0 = new QTableWidgetItem(tr("Enabled"));
  ui->tableTools->setHorizontalHeaderItem(5, id0);

  id0 = new QTableWidgetItem(tr("RealPath"));
  ui->tableTools->setHorizontalHeaderItem(6, id0);

  id0 = new QTableWidgetItem(tr("TextMode"));
  ui->tableTools->setHorizontalHeaderItem(7, id0);

  id0 = new QTableWidgetItem(tr("Flavour"));
  ui->tableTools->setHorizontalHeaderItem(8, id0);

  ui->tableTools->setAlternatingRowColors(true);
}

void MainWindow::ParserMisc(QVariantMap map) {
  map = map["Misc"].toMap();
  if (map.isEmpty()) return;

  //分析"Boot"
  QVariantMap map_boot = map["Boot"].toMap();
  getValue(map_boot, ui->tabMisc1);

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
    if (str.contains(hm)) ui->cboxSecureBootModel->setCurrentIndex(i);
  }

  // BlessOverride(数组)
  QVariantList map_BlessOverride = map["BlessOverride"].toList();
  ui->tableBlessOverride->setRowCount(map_BlessOverride.count());
  for (int i = 0; i < map_BlessOverride.count(); i++) {
    QTableWidgetItem* newItem1;
    newItem1 = new QTableWidgetItem(map_BlessOverride.at(i).toString());
    ui->tableBlessOverride->setItem(i, 0, newItem1);
  }

  // Entries
  QVariantList map_Entries = map["Entries"].toList();
  ui->tableEntries->setRowCount(map_Entries.count());
  for (int i = 0; i < map_Entries.count(); i++) {
    QVariantMap map3 = map_Entries.at(i).toMap();

    QTableWidgetItem* newItem1;

    newItem1 = new QTableWidgetItem(map3["Path"].toString());
    ui->tableEntries->setItem(i, 0, newItem1);

    newItem1 = new QTableWidgetItem(map3["Arguments"].toString());
    ui->tableEntries->setItem(i, 1, newItem1);

    newItem1 = new QTableWidgetItem(map3["Name"].toString());
    newItem1->setTextAlignment(Qt::AlignCenter);
    ui->tableEntries->setItem(i, 2, newItem1);

    newItem1 = new QTableWidgetItem(map3["Comment"].toString());
    ui->tableEntries->setItem(i, 3, newItem1);

    init_enabled_data(ui->tableEntries, i, 4, map3["Auxiliary"].toString());
    init_enabled_data(ui->tableEntries, i, 5, map3["Enabled"].toString());
    init_enabled_data(ui->tableEntries, i, 6, map3["TextMode"].toString());

    newItem1 = new QTableWidgetItem(map3["Flavour"].toString());
    if (map3["Flavour"].toString().trimmed() == "")
      newItem1 = new QTableWidgetItem("Auto");
    ui->tableEntries->setItem(i, 7, newItem1);
  }

  // Tools
  QVariantList map_Tools = map["Tools"].toList();
  ui->tableTools->setRowCount(map_Tools.count());
  for (int i = 0; i < map_Tools.count(); i++) {
    QVariantMap map3 = map_Tools.at(i).toMap();

    QTableWidgetItem* newItem1;

    newItem1 = new QTableWidgetItem(map3["Path"].toString());
    ui->tableTools->setItem(i, 0, newItem1);

    newItem1 = new QTableWidgetItem(map3["Arguments"].toString());
    ui->tableTools->setItem(i, 1, newItem1);

    newItem1 = new QTableWidgetItem(map3["Name"].toString());
    newItem1->setTextAlignment(Qt::AlignCenter);
    ui->tableTools->setItem(i, 2, newItem1);

    newItem1 = new QTableWidgetItem(map3["Comment"].toString());
    ui->tableTools->setItem(i, 3, newItem1);

    init_enabled_data(ui->tableTools, i, 4, map3["Auxiliary"].toString());

    init_enabled_data(ui->tableTools, i, 5, map3["Enabled"].toString());

    init_enabled_data(ui->tableTools, i, 6, map3["RealPath"].toString());

    init_enabled_data(ui->tableTools, i, 7, map3["TextMode"].toString());

    newItem1 = new QTableWidgetItem(map3["Flavour"].toString());
    if (map3["Flavour"].toString().trimmed() == "")
      newItem1 = new QTableWidgetItem("Auto");
    ui->tableTools->setItem(i, 8, newItem1);
  }
}

void MainWindow::initui_nvram() {
  QString y = QString::number(QDate::currentDate().year());
  QString m = QString::number(QDate::currentDate().month());
  QString d = QString::number(QDate::currentDate().day());

  QString h = QString::number(QTime::currentTime().hour());
  QString mm = QString::number(QTime::currentTime().minute());
  QString s = QString::number(QTime::currentTime().second());

  CurrentDateTime = y + m + d + h + mm + s;

  QTableWidgetItem* id0;

  // Add
  ui->table_nv_add0->setMinimumWidth(300);
  ui->table_nv_add0->setMaximumWidth(525);

  id0 = new QTableWidgetItem(tr("UUID"));
  ui->table_nv_add0->setHorizontalHeaderItem(0, id0);
  ui->table_nv_add0->setAlternatingRowColors(true);
  ui->table_nv_add0->horizontalHeader()->setStretchLastSection(true);

  ui->btnNVRAMAdd_Add0->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(ui->btnNVRAMAdd_Add0, SIGNAL(customContextMenuRequested(QPoint)),
          this, SLOT(show_menu0(QPoint)));

  ui->table_nv_add->setColumnWidth(0, 200);
  id0 = new QTableWidgetItem(tr("Key"));
  ui->table_nv_add->setHorizontalHeaderItem(0, id0);

  ui->table_nv_add->setColumnWidth(2, 350);
  id0 = new QTableWidgetItem(tr("Value"));
  ui->table_nv_add->setHorizontalHeaderItem(2, id0);

  id0 = new QTableWidgetItem(tr("Class"));
  ui->table_nv_add->setHorizontalHeaderItem(1, id0);

  ui->table_nv_add->setAlternatingRowColors(true);

  ui->btnNVRAMAdd_Add->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(ui->btnNVRAMAdd_Add, SIGNAL(customContextMenuRequested(QPoint)), this,
          SLOT(show_menu(QPoint)));

  // 分割窗口
  QSplitter* splitterMain = new QSplitter(Qt::Horizontal, this);
  splitterMain->addWidget(ui->table_nv_add0);
  splitterMain->addWidget(ui->table_nv_add);
  ui->gridLayout_nv_add->addWidget(splitterMain);

  // Delete
  id0 = new QTableWidgetItem(tr("UUID"));
  ui->table_nv_del0->setHorizontalHeaderItem(0, id0);

  ui->table_nv_del0->setAlternatingRowColors(true);
  ui->table_nv_del0->horizontalHeader()->setStretchLastSection(true);

  id0 = new QTableWidgetItem(tr("Value"));
  ui->table_nv_del->setHorizontalHeaderItem(0, id0);

  ui->table_nv_del->setAlternatingRowColors(true);
  ui->table_nv_del->horizontalHeader()->setStretchLastSection(true);

  // LegacySchema
  id0 = new QTableWidgetItem(tr("UUID"));
  ui->table_nv_ls0->setHorizontalHeaderItem(0, id0);
  ui->table_nv_ls0->setAlternatingRowColors(true);
  ui->table_nv_ls0->horizontalHeader()->setStretchLastSection(true);

  id0 = new QTableWidgetItem(tr("Value"));
  ui->table_nv_ls->setHorizontalHeaderItem(0, id0);
  ui->table_nv_ls->setAlternatingRowColors(true);
  ui->table_nv_ls->horizontalHeader()->setStretchLastSection(true);
}

void MainWindow::ParserNvram(QVariantMap map) {
  map = map["NVRAM"].toMap();
  if (map.isEmpty()) return;

  // Add
  QVariantMap map_add = map["Add"].toMap();

  QVariantMap map_sub;

  ui->table_nv_add0->setRowCount(map_add.count());
  QTableWidgetItem* newItem1;
  for (int i = 0; i < map_add.count(); i++) {
    newItem1 = new QTableWidgetItem(map_add.keys().at(i));
    ui->table_nv_add0->setItem(i, 0, newItem1);

    //加载子条目
    map_sub = map_add[map_add.keys().at(i)].toMap();
    ui->table_nv_add->setRowCount(map_sub.keys().count());  //子键的个数

    for (int j = 0; j < map_sub.keys().count(); j++) {
      newItem1 = new QTableWidgetItem(map_sub.keys().at(j));  //键
      ui->table_nv_add->setItem(j, 0, newItem1);

      QString dtype = map_sub[map_sub.keys().at(j)].typeName();
      QString ztype;
      if (dtype == "QByteArray") ztype = "Data";
      if (dtype == "QString") ztype = "String";
      if (dtype == "qlonglong") ztype = "Number";
      newItem1 = new QTableWidgetItem(ztype);  //数据类型
      newItem1->setTextAlignment(Qt::AlignCenter);
      ui->table_nv_add->setItem(j, 1, newItem1);

      QString type_name = map_sub[map_sub.keys().at(j)].typeName();
      if (type_name == "QByteArray") {
        QByteArray tohex = map_sub[map_sub.keys().at(j)].toByteArray();
        QString va = tohex.toHex().toUpper();
        newItem1 = new QTableWidgetItem(va);
      } else
        newItem1 =
            new QTableWidgetItem(map_sub[map_sub.keys().at(j)].toString());
      ui->table_nv_add->setItem(j, 2, newItem1);
    }

    //保存子条目里面的数据，以便以后加载

    write_ini(ui->table_nv_add0, ui->table_nv_add, i);
  }

  int last = ui->table_nv_add0->rowCount();
  ui->table_nv_add0->setCurrentCell(last - 1, 0);

  // Delete
  init_value(map["Delete"].toMap(), ui->table_nv_del0, ui->table_nv_del);

  // LegacySchema
  init_value(map["LegacySchema"].toMap(), ui->table_nv_ls0, ui->table_nv_ls);

  //分析Quirks
  ui->chkLegacyEnable->setChecked(map["LegacyEnable"].toBool());
  ui->chkLegacyOverwrite->setChecked(map["LegacyOverwrite"].toBool());
  ui->chkWriteFlash->setChecked(map["WriteFlash"].toBool());
}

void MainWindow::write_ini(QTableWidget* table, QTableWidget* mytable, int i) {
  QString name = table->item(i, 0)->text().trimmed();
  if (name == "") name = "Item" + QString::number(i + 1);

  name = name.replace("/", "-");

  QString plistPath = QDir::homePath() + "/.config/QtOCC/" + CurrentDateTime +
                      table->objectName() + name + ".ini";

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

  QString plistPath = QDir::homePath() + "/.config/QtOCC/" + CurrentDateTime +
                      table->objectName() + name + ".ini";

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

void MainWindow::read_value_ini(QTableWidget* table, QTableWidget* mytable,
                                int i) {
  QString name = table->item(i, 0)->text().trimmed();
  if (name == "") name = "Item" + QString::number(i + 1);

  name = name.replace("/", "-");

  QString plistPath = QDir::homePath() + "/.config/QtOCC/" + CurrentDateTime +
                      table->objectName() + name + ".ini";

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

  loadRightTable(ui->table_dp_add0, ui->table_dp_add);

  setStatusBarText(ui->table_dp_add0);
}

void MainWindow::on_table_dp_add_itemChanged(QTableWidgetItem* item) {
  Q_UNUSED(item);

  if (writeINI) {
    write_ini(ui->table_dp_add0, ui->table_dp_add,
              ui->table_dp_add0->currentRow());

    this->setWindowModified(true);
  }
}

void MainWindow::on_table_nv_add0_cellClicked(int row, int column) {
  Q_UNUSED(row);
  Q_UNUSED(column);

  loadRightTable(ui->table_nv_add0, ui->table_nv_add);
}

void MainWindow::on_table_nv_add_itemChanged(QTableWidgetItem* item) {
  Q_UNUSED(item);

  if (writeINI)
    write_ini(ui->table_nv_add0, ui->table_nv_add,
              ui->table_nv_add0->currentRow());

  if (!loading) this->setWindowModified(true);
}

void MainWindow::init_value(QVariantMap map_fun, QTableWidget* table,
                            QTableWidget* subtable) {
  table->setRowCount(map_fun.count());
  subtable->setRowCount(0);
  QTableWidgetItem* newItem1;
  for (int i = 0; i < map_fun.count(); i++) {
    newItem1 = new QTableWidgetItem(map_fun.keys().at(i));
    table->setItem(i, 0, newItem1);

    //加载子条目
    QVariantList map_sub_list =
        map_fun[map_fun.keys().at(i)].toList();   //是个数组
    subtable->setRowCount(map_sub_list.count());  //子键的个数

    for (int j = 0; j < map_sub_list.count(); j++) {
      newItem1 = new QTableWidgetItem(map_sub_list.at(j).toString());  //键
      subtable->setItem(j, 0, newItem1);
    }

    //保存子条目里面的数据，以便以后加载
    write_value_ini(table, subtable, i);
  }

  int last = table->rowCount();
  table->setCurrentCell(last - 1, 0);
}

void MainWindow::write_value_ini(QTableWidget* table, QTableWidget* subtable,
                                 int i) {
  QString name = table->item(i, 0)->text().trimmed();
  if (name == "") name = "Item" + QString::number(i + 1);
  name = name.replace("/", "-");

  QString plistPath = QDir::homePath() + "/.config/QtOCC/" + CurrentDateTime +
                      table->objectName() + name + ".ini";

  QFile file(plistPath);
  if (file.exists()) file.remove();

  QSettings Reg(plistPath, QSettings::IniFormat);

  for (int k = 0; k < subtable->rowCount(); k++) {
    Reg.setValue(QString::number(k + 1) + "/key", subtable->item(k, 0)->text());
  }

  Reg.setValue("total", subtable->rowCount());

  IniFile.push_back(plistPath);
}

void MainWindow::on_table_nv_del0_cellClicked(int row, int column) {
  Q_UNUSED(row);
  Q_UNUSED(column);

  loadRightTable(ui->table_nv_del0, ui->table_nv_del);
}

void MainWindow::on_table_nv_ls0_cellClicked(int row, int column) {
  Q_UNUSED(row);
  Q_UNUSED(column);

  loadRightTable(ui->table_nv_ls0, ui->table_nv_ls);
}

void MainWindow::on_table_nv_del_itemChanged(QTableWidgetItem* item) {
  if (item->text().isEmpty()) {
  }

  if (writeINI) {
    write_value_ini(ui->table_nv_del0, ui->table_nv_del,
                    ui->table_nv_del0->currentRow());

    this->setWindowModified(true);
  }
}

void MainWindow::on_table_nv_ls_itemChanged(QTableWidgetItem* item) {
  if (item->text().isEmpty()) {
  }

  if (writeINI) {
    write_value_ini(ui->table_nv_ls0, ui->table_nv_ls,
                    ui->table_nv_ls0->currentRow());

    this->setWindowModified(true);
  }
}

void MainWindow::on_table_dp_del0_cellClicked(int row, int column) {
  Q_UNUSED(row);
  Q_UNUSED(column);

  loadRightTable(ui->table_dp_del0, ui->table_dp_del);

  setStatusBarText(ui->table_dp_del0);
}

void MainWindow::on_table_dp_del_itemChanged(QTableWidgetItem* item) {
  if (item->text().isEmpty()) {
  }

  if (writeINI) {
    write_value_ini(ui->table_dp_del0, ui->table_dp_del,
                    ui->table_dp_del0->currentRow());

    this->setWindowModified(true);
  }
}

void MainWindow::initui_PlatformInfo() {
  QString qfile = QDir::homePath() + "/.config/QtOCC/QtOCC.ini";
  QFileInfo fi(qfile);
  if (fi.exists()) {
    QSettings Reg(qfile, QSettings::IniFormat);

    ui->chkSaveDataHub->setChecked(Reg.value("SaveDataHub").toBool());
  }

  ui->cboxUpdateSMBIOSMode->addItem("TryOverwrite");
  ui->cboxUpdateSMBIOSMode->addItem("Create");
  ui->cboxUpdateSMBIOSMode->addItem("Overwrite");
  ui->cboxUpdateSMBIOSMode->addItem("Custom");

  ui->cboxSystemMemoryStatus->addItem("Auto");
  ui->cboxSystemMemoryStatus->addItem("Upgradable");
  ui->cboxSystemMemoryStatus->addItem("Soldered");

  // Memory-Devices
  QTableWidgetItem* id0;
  ui->tableDevices->setColumnWidth(0, 160);
  id0 = new QTableWidgetItem(tr("AssetTag"));
  ui->tableDevices->setHorizontalHeaderItem(0, id0);

  id0 = new QTableWidgetItem(tr("BankLocator"));
  ui->tableDevices->setHorizontalHeaderItem(1, id0);

  ui->tableDevices->setColumnWidth(2, 220);
  id0 = new QTableWidgetItem(tr("DeviceLocator"));
  ui->tableDevices->setHorizontalHeaderItem(2, id0);

  id0 = new QTableWidgetItem(tr("Manufacturer"));
  ui->tableDevices->setHorizontalHeaderItem(3, id0);
  ui->tableDevices->setColumnWidth(3, 160);

  id0 = new QTableWidgetItem(tr("PartNumber"));
  ui->tableDevices->setHorizontalHeaderItem(4, id0);
  ui->tableDevices->setColumnWidth(4, 160);

  ui->tableDevices->setColumnWidth(5, 220);
  id0 = new QTableWidgetItem(tr("SerialNumber"));
  ui->tableDevices->setHorizontalHeaderItem(5, id0);

  id0 = new QTableWidgetItem(tr("Size"));
  ui->tableDevices->setHorizontalHeaderItem(6, id0);

  id0 = new QTableWidgetItem(tr("Speed"));
  ui->tableDevices->setHorizontalHeaderItem(7, id0);

  ui->tableDevices->setAlternatingRowColors(true);  //底色交替显示

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

  //获取当前Mac信息
  QFileInfo appInfo(qApp->applicationDirPath());
  si = new QProcess;

#ifdef Q_OS_WIN32
  QFile file(appInfo.filePath() + "/Database/win/macserial.exe");
  if (file.exists())
    gs->execute(appInfo.filePath() + "/Database/win/macserial.exe",
                QStringList() << "-s");  //阻塞execute

  ui->tabPlatformInfo->removeTab(5);

#endif

#ifdef Q_OS_LINUX
  gs->execute(appInfo.filePath() + "/Database/linux/macserial", QStringList()
                                                                    << "-s");

  ui->tabPlatformInfo->removeTab(5);

#endif

#ifdef Q_OS_MAC
  si->start(appInfo.filePath() + "/Database/mac/macserial", QStringList()
                                                                << "-s");
#endif

  connect(si, SIGNAL(finished(int)), this, SLOT(readResultSystemInfo()));
}

void MainWindow::ParserPlatformInfo(QVariantMap map) {
  map = map["PlatformInfo"].toMap();
  if (map.isEmpty()) return;

  ui->chkAutomatic->setChecked(map["Automatic"].toBool());
  ui->chkCustomMemory->setChecked(map["CustomMemory"].toBool());
  ui->chkUpdateDataHub->setChecked(map["UpdateDataHub"].toBool());
  ui->chkUpdateNVRAM->setChecked(map["UpdateNVRAM"].toBool());
  ui->chkUpdateSMBIOS->setChecked(map["UpdateSMBIOS"].toBool());
  ui->chkUseRawUuidEncoding->setChecked(map["UseRawUuidEncoding"].toBool());

  QString usm = map["UpdateSMBIOSMode"].toString();
  ui->cboxUpdateSMBIOSMode->setCurrentText(usm.trimmed());

  if (usm.trimmed() == "Custom") {
    ui->cboxUpdateSMBIOSMode->setCurrentText(usm.trimmed());
    ui->chkCustomSMBIOSGuid->setChecked(true);  //联动
  } else
    ui->chkCustomSMBIOSGuid->setChecked(false);

  // Generic
  QVariantMap mapGeneric = map["Generic"].toMap();
  getValue(mapGeneric, ui->tabPlatformInfo1);

  if (ui->cboxSystemMemoryStatus->currentText() == "")
    ui->cboxSystemMemoryStatus->setCurrentIndex(0);

  if (ui->editMLB_PNVRAM->text().trimmed() == "")
    ui->editMLB_PNVRAM->setText(mapGeneric["MLB"].toString());

  QByteArray ba = mapGeneric["ROM"].toByteArray();
  QString va = ba.toHex().toUpper();

  if (ui->editDatROM_PNVRAM->text().trimmed() == "")
    ui->editDatROM_PNVRAM->setText(va);

  //机型
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
    for (int i = 0; i < ui->cboxSystemProductName->count(); i++) {
      if (getSystemProductName(ui->cboxSystemProductName->itemText(i)) ==
          mapDataHub["SystemProductName"].toString()) {
        ui->cboxSystemProductName->setCurrentIndex(i);
        break;
      }
    }
  }

  if (ui->editSystemSerialNumber->text().trimmed() == "")
    ui->editSystemSerialNumber->setText(
        mapDataHub["SystemSerialNumber"].toString());

  if (ui->editSystemUUID->text().trimmed() == "")
    ui->editSystemUUID->setText(mapDataHub["SystemUUID"].toString());

  // Memory
  QVariantMap mapMemory = map["Memory"].toMap();
  getValue(mapMemory, ui->tabPlatformInfo3);

  // Memory-Devices
  QVariantList mapMemoryDevices = mapMemory["Devices"].toList();

  ui->tableDevices->setRowCount(mapMemoryDevices.count());
  for (int i = 0; i < mapMemoryDevices.count(); i++) {
    QVariantMap map3 = mapMemoryDevices.at(i).toMap();

    QTableWidgetItem* newItem1;

    newItem1 = new QTableWidgetItem(map3["AssetTag"].toString());
    newItem1->setTextAlignment(Qt::AlignCenter);
    ui->tableDevices->setItem(i, 0, newItem1);

    newItem1 = new QTableWidgetItem(map3["BankLocator"].toString());
    newItem1->setTextAlignment(Qt::AlignCenter);
    ui->tableDevices->setItem(i, 1, newItem1);

    newItem1 = new QTableWidgetItem(map3["DeviceLocator"].toString());
    newItem1->setTextAlignment(Qt::AlignCenter);
    ui->tableDevices->setItem(i, 2, newItem1);

    newItem1 = new QTableWidgetItem(map3["Manufacturer"].toString());
    newItem1->setTextAlignment(Qt::AlignCenter);
    ui->tableDevices->setItem(i, 3, newItem1);

    newItem1 = new QTableWidgetItem(map3["PartNumber"].toString());
    newItem1->setTextAlignment(Qt::AlignCenter);
    ui->tableDevices->setItem(i, 4, newItem1);

    newItem1 = new QTableWidgetItem(map3["SerialNumber"].toString());
    newItem1->setTextAlignment(Qt::AlignCenter);
    ui->tableDevices->setItem(i, 5, newItem1);

    newItem1 = new QTableWidgetItem(map3["Size"].toString());
    newItem1->setTextAlignment(Qt::AlignCenter);
    ui->tableDevices->setItem(i, 6, newItem1);

    newItem1 = new QTableWidgetItem(map3["Speed"].toString());
    newItem1->setTextAlignment(Qt::AlignCenter);
    ui->tableDevices->setItem(i, 7, newItem1);
  }

  // PlatformNVRAM
  QVariantMap mapPlatformNVRAM = map["PlatformNVRAM"].toMap();
  getValue(mapPlatformNVRAM, ui->tabPlatformInfo4);

  if (ui->editMLB->text().trimmed() == "")
    ui->editMLB->setText(mapPlatformNVRAM["MLB"].toString());

  if (ui->editDatROM->text().trimmed() == "")
    ui->editDatROM->setText(
        ByteToHexStr(mapPlatformNVRAM["ROM"].toByteArray()));

  // SMBIOS
  QVariantMap mapSMBIOS = map["SMBIOS"].toMap();
  getValue(mapSMBIOS, ui->tabPlatformInfo5);
}

void MainWindow::initui_UEFI() {
  // APFS
  ui->calendarWidget->setVisible(false);

  // Audio
  QStringList list;
  list.append("Auto");
  list.append("Enabled");
  list.append("Disabled");
  ui->cboxPlayChime->addItems(list);

  // Drivers
  QTableWidgetItem* id0;

  ui->table_uefi_drivers->setColumnCount(4);

  id0 = new QTableWidgetItem(tr("Path"));
  ui->table_uefi_drivers->setHorizontalHeaderItem(0, id0);

  id0 = new QTableWidgetItem(tr("Enabled"));
  ui->table_uefi_drivers->setHorizontalHeaderItem(1, id0);

  id0 = new QTableWidgetItem(tr("Arguments"));
  ui->table_uefi_drivers->setHorizontalHeaderItem(2, id0);

  id0 = new QTableWidgetItem(tr("Comment"));
  ui->table_uefi_drivers->setHorizontalHeaderItem(3, id0);

  ui->table_uefi_drivers->setAlternatingRowColors(true);
  // ui->table_uefi_drivers->horizontalHeader()->setStretchLastSection(true);

  // Input
  ui->cboxKeySupportMode->addItem("Auto");
  ui->cboxKeySupportMode->addItem("V1");
  ui->cboxKeySupportMode->addItem("V2");
  ui->cboxKeySupportMode->addItem("AMI");

  // Output
  ui->cboxConsoleMode->setEditable(true);
  ui->cboxConsoleMode->addItem("Max");

  ui->cboxResolution->setEditable(true);
  ui->cboxResolution->addItem("Max");

  ui->cboxTextRenderer->addItem("BuiltinGraphics");
  ui->cboxTextRenderer->addItem("SystemGraphics");
  ui->cboxTextRenderer->addItem("SystemText");
  ui->cboxTextRenderer->addItem("SystemGeneric");

  // ReservedMemory

  ui->table_uefi_ReservedMemory->setColumnWidth(0, 300);
  id0 = new QTableWidgetItem(tr("Address"));
  ui->table_uefi_ReservedMemory->setHorizontalHeaderItem(0, id0);

  ui->table_uefi_ReservedMemory->setColumnWidth(1, 320);
  id0 = new QTableWidgetItem(tr("Comment"));
  ui->table_uefi_ReservedMemory->setHorizontalHeaderItem(1, id0);

  ui->table_uefi_ReservedMemory->setColumnWidth(2, 300);
  id0 = new QTableWidgetItem(tr("Size"));
  ui->table_uefi_ReservedMemory->setHorizontalHeaderItem(2, id0);

  ui->table_uefi_ReservedMemory->setColumnWidth(3, 300);
  id0 = new QTableWidgetItem(tr("Type"));
  ui->table_uefi_ReservedMemory->setHorizontalHeaderItem(3, id0);

  id0 = new QTableWidgetItem(tr("Enabled"));
  ui->table_uefi_ReservedMemory->setHorizontalHeaderItem(4, id0);

  ui->table_uefi_ReservedMemory->setAlternatingRowColors(true);
}

void MainWindow::ParserUEFI(QVariantMap map) {
  map = map["UEFI"].toMap();
  if (map.isEmpty()) return;

  // 1. APFS
  QVariantMap map_apfs = map["APFS"].toMap();
  getValue(map_apfs, ui->tabUEFI1);

  // 2. AppleInput
  QVariantMap map_AppleInput = map["AppleInput"].toMap();
  getValue(map_AppleInput, ui->tabUEFI2);

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

  // 4. Drivers
  QVariantList map_Drivers = map["Drivers"].toList();  //数组
  ui->table_uefi_drivers->setRowCount(map_Drivers.count());
  for (int i = 0; i < map_Drivers.count(); i++) {
    // QTableWidgetItem* id0;
    // id0 = new QTableWidgetItem(map_Drivers.at(i).toString());
    // //老版本OC（0.7.3之前）只有一列记录的情况
    // ui->table_uefi_drivers->setItem(i, 0, id0);

    QVariantMap map3 = map_Drivers.at(i).toMap();

    if (map3.count() > 1) {
      QTableWidgetItem* newItem1;
      newItem1 = new QTableWidgetItem(map3["Path"].toString());
      ui->table_uefi_drivers->setItem(i, 0, newItem1);

      init_enabled_data(ui->table_uefi_drivers, i, 1,
                        map3["Enabled"].toString());

      newItem1 = new QTableWidgetItem(map3["Arguments"].toString());
      ui->table_uefi_drivers->setItem(i, 2, newItem1);

      newItem1 = new QTableWidgetItem(map3["Comment"].toString());
      ui->table_uefi_drivers->setItem(i, 3, newItem1);

    } else {
      QString strEnabled = "true";
      QTableWidgetItem* newItem1;
      QString strPath = map_Drivers.at(i).toString();
      if (strPath.mid(0, 1) == "#") {
        strEnabled = "false";
        strPath = strPath.replace("#", "");
      }
      newItem1 = new QTableWidgetItem(strPath);
      ui->table_uefi_drivers->setItem(i, 0, newItem1);

      init_enabled_data(ui->table_uefi_drivers, i, 1, strEnabled);

      newItem1 = new QTableWidgetItem("");
      ui->table_uefi_drivers->setItem(i, 2, newItem1);

      newItem1 = new QTableWidgetItem("");
      ui->table_uefi_drivers->setItem(i, 3, newItem1);
    }
  }

  ui->chkConnectDrivers->setChecked(map["ConnectDrivers"].toBool());

  // 5. Input
  QVariantMap map_input = map["Input"].toMap();
  getValue(map_input, ui->tabUEFI5);

  // 6. Output
  QVariantMap map_output = map["Output"].toMap();
  getValue(map_output, ui->tabUEFI6);
  QString strGop = ui->cboxGopPassThrough->currentText();
  if (strGop == "true" || strGop == "false")
    ui->cboxGopPassThrough->setCurrentIndex(0);

  // 7. ProtocolOverrides
  QVariantMap map_po = map["ProtocolOverrides"].toMap();
  getValue(map_po, ui->tabUEFI7);

  // 8. Quirks
  QVariantMap map_uefi_Quirks = map["Quirks"].toMap();
  getValue(map_uefi_Quirks, ui->tabUEFI8);

  // 9. ReservedMemory
  QTableWidgetItem* newItem1;
  QVariantList map3 = map["ReservedMemory"].toList();
  ui->table_uefi_ReservedMemory->setRowCount(map3.count());
  for (int i = 0; i < map3.count(); i++) {
    QVariantMap map_sub = map3.at(i).toMap();
    newItem1 = new QTableWidgetItem(map_sub["Address"].toString());
    ui->table_uefi_ReservedMemory->setItem(i, 0, newItem1);

    newItem1 = new QTableWidgetItem(map_sub["Comment"].toString());
    ui->table_uefi_ReservedMemory->setItem(i, 1, newItem1);

    newItem1 = new QTableWidgetItem(map_sub["Size"].toString());
    ui->table_uefi_ReservedMemory->setItem(i, 2, newItem1);

    QString strType = map_sub["Type"].toString();
    if (strType != "")
      newItem1 = new QTableWidgetItem(strType);
    else
      newItem1 = new QTableWidgetItem("Reserved");
    ui->table_uefi_ReservedMemory->setItem(i, 3, newItem1);

    init_enabled_data(ui->table_uefi_ReservedMemory, i, 4,
                      map_sub["Enabled"].toString());
  }
}

void MainWindow::on_btnSave() { SavePlist(SaveFileName); }

bool MainWindow::getBool(QTableWidget* table, int row, int column) {
  QString be = table->item(row, column)->text();
  if (be.trimmed() == "true") return true;
  if (be.trimmed() == "false") return false;

  return false;
}

void MainWindow::SavePlist(QString FileName) {
  removeAllLineEdit();

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

  PListSerializer::toPList(OpenCore, FileName);

  this->setWindowModified(false);

  if (closeSave) {
    clear_temp_data();
  }

  if (!RefreshAllDatabase) {
    OpenFileValidate = true;
    on_actionOcvalidate_triggered();
  }
}

QVariantMap MainWindow::SaveACPI() {
  // ACPI

  // Add
  QVariantMap acpiMap;

  QVariantList acpiAdd;
  QVariantMap acpiAddSub;

  for (int i = 0; i < ui->table_acpi_add->rowCount(); i++) {
    acpiAddSub["Path"] = ui->table_acpi_add->item(i, 0)->text();

    acpiAddSub["Enabled"] = getBool(ui->table_acpi_add, i, 1);

    acpiAddSub["Comment"] = ui->table_acpi_add->item(i, 2)->text();

    acpiAdd.append(acpiAddSub);  //最后一层
  }

  acpiMap["Add"] = acpiAdd;  //第二层

  // Delete

  QVariantList acpiDel;
  QVariantMap acpiDelSub;
  QString str;

  for (int i = 0; i < ui->table_acpi_del->rowCount(); i++) {
    str = ui->table_acpi_del->item(i, 0)->text();
    acpiDelSub["TableSignature"] = HexStrToByte(str);

    str = ui->table_acpi_del->item(i, 1)->text();
    acpiDelSub["OemTableId"] = HexStrToByte(str);

    acpiDelSub["TableLength"] =
        ui->table_acpi_del->item(i, 2)->text().toLongLong();

    acpiDelSub["All"] = getBool(ui->table_acpi_del, i, 3);
    acpiDelSub["Enabled"] = getBool(ui->table_acpi_del, i, 4);
    acpiDelSub["Comment"] = ui->table_acpi_del->item(i, 5)->text();

    acpiDel.append(acpiDelSub);  //最后一层
  }

  acpiMap["Delete"] = acpiDel;  //第二层

  // Patch

  QVariantList acpiPatch;
  QVariantMap acpiPatchSub;

  for (int i = 0; i < ui->table_acpi_patch->rowCount(); i++) {
    str = ui->table_acpi_patch->item(i, 0)->text();
    acpiPatchSub["TableSignature"] = HexStrToByte(str);

    str = ui->table_acpi_patch->item(i, 1)->text();
    acpiPatchSub["OemTableId"] = HexStrToByte(str);

    acpiPatchSub["TableLength"] =
        ui->table_acpi_patch->item(i, 2)->text().toLongLong();
    acpiPatchSub["Find"] =
        HexStrToByte(ui->table_acpi_patch->item(i, 3)->text());
    acpiPatchSub["Replace"] =
        HexStrToByte(ui->table_acpi_patch->item(i, 4)->text());
    acpiPatchSub["Comment"] = ui->table_acpi_patch->item(i, 5)->text();
    acpiPatchSub["Mask"] = ui->table_acpi_patch->item(i, 6)->text().toLatin1();
    acpiPatchSub["ReplaceMask"] =
        ui->table_acpi_patch->item(i, 7)->text().toLatin1();
    acpiPatchSub["Count"] =
        ui->table_acpi_patch->item(i, 8)->text().toLongLong();
    acpiPatchSub["Limit"] =
        ui->table_acpi_patch->item(i, 9)->text().toLongLong();
    acpiPatchSub["Skip"] =
        ui->table_acpi_patch->item(i, 10)->text().toLongLong();
    acpiPatchSub["Enabled"] = getBool(ui->table_acpi_patch, i, 11);

    acpiPatchSub["Base"] = ui->table_acpi_patch->item(i, 12)->text();
    acpiPatchSub["BaseSkip"] =
        ui->table_acpi_patch->item(i, 13)->text().toLongLong();

    acpiPatch.append(acpiPatchSub);  //最后一层
  }

  acpiMap["Patch"] = acpiPatch;  //第二层

  // Quirks
  QVariantMap acpiQuirks;
  acpiMap["Quirks"] = setValue(acpiQuirks, ui->tabACPI4);

  return acpiMap;
}

QVariantMap MainWindow::SaveBooter() {
  QVariantMap subMap;
  QVariantList arrayList;
  QVariantMap valueList;

  // Patch
  arrayList.clear();
  valueList.clear();
  for (int i = 0; i < ui->table_Booter_patch->rowCount(); i++) {
    valueList["Identifier"] = ui->table_Booter_patch->item(i, 0)->text();

    valueList["Comment"] = ui->table_Booter_patch->item(i, 1)->text();
    valueList["Find"] =
        HexStrToByte(ui->table_Booter_patch->item(i, 2)->text());
    valueList["Replace"] =
        HexStrToByte(ui->table_Booter_patch->item(i, 3)->text());
    valueList["Mask"] =
        HexStrToByte(ui->table_Booter_patch->item(i, 4)->text());
    valueList["ReplaceMask"] =
        HexStrToByte(ui->table_Booter_patch->item(i, 5)->text());

    valueList["Count"] =
        ui->table_Booter_patch->item(i, 6)->text().toLongLong();
    valueList["Limit"] =
        ui->table_Booter_patch->item(i, 7)->text().toLongLong();
    valueList["Skip"] = ui->table_Booter_patch->item(i, 8)->text().toLongLong();
    valueList["Enabled"] = getBool(ui->table_Booter_patch, i, 9);
    valueList["Arch"] = ui->table_Booter_patch->item(i, 10)->text();

    arrayList.append(valueList);
  }

  subMap["Patch"] = arrayList;

  // MmioWhitelist
  arrayList.clear();
  valueList.clear();

  for (int i = 0; i < ui->table_booter->rowCount(); i++) {
    QString str = ui->table_booter->item(i, 0)->text().trimmed();
    valueList["Address"] = str.toLongLong();

    valueList["Enabled"] = getBool(ui->table_booter, i, 1);

    valueList["Comment"] = ui->table_booter->item(i, 2)->text();

    arrayList.append(valueList);  //最后一层
  }

  subMap["MmioWhitelist"] = arrayList;  //第二层

  // Quirks
  QVariantMap mapQuirks;
  subMap["Quirks"] = setValue(mapQuirks, ui->tabBooter3);

  return subMap;
}

QVariantMap MainWindow::SaveDeviceProperties() {
  // Add
  QVariantMap subMap;
  QVariantMap dictList;
  QVariantMap valueList;
  QVariantList arrayList;

  int currentRow = ui->table_dp_add0->currentRow();
  for (int i = 0; i < ui->table_dp_add0->rowCount(); i++) {
    valueList.clear();  //先必须清理下列表，很重要
    //先加载表中的数据
    ui->table_dp_add0->setCurrentCell(i, 0);
    on_table_dp_add0_cellClicked(i, 0);

    for (int k = 0; k < ui->table_dp_add->rowCount(); k++) {
      QString dataType = ui->table_dp_add->item(k, 1)->text();  //数据类型
      QString value = ui->table_dp_add->item(k, 2)->text();
      if (dataType == "String")
        valueList[ui->table_dp_add->item(k, 0)->text()] = value;
      if (dataType == "Data") {
        //将以字符串方式显示的16进制原样转换成QByteArray
        valueList[ui->table_dp_add->item(k, 0)->text()] = HexStrToByte(value);
      }
      if (dataType == "Number")
        valueList[ui->table_dp_add->item(k, 0)->text()] = value.toLongLong();
    }
    dictList[ui->table_dp_add0->item(i, 0)->text()] = valueList;
  }

  ui->table_dp_add0->clearSelection();
  ui->table_dp_add0->setCurrentCell(currentRow, 0);

  subMap["Add"] = dictList;

  // Delete
  dictList.clear();
  currentRow = ui->table_dp_del0->currentRow();
  for (int i = 0; i < ui->table_dp_del0->rowCount(); i++) {
    valueList.clear();
    arrayList.clear();

    //先加载表中的数据
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
  QVariantList dictList;
  QVariantMap valueList;
  for (int i = 0; i < ui->table_kernel_add->rowCount(); i++) {
    valueList["BundlePath"] = ui->table_kernel_add->item(i, 0)->text();
    valueList["Comment"] = ui->table_kernel_add->item(i, 1)->text();
    valueList["ExecutablePath"] = ui->table_kernel_add->item(i, 2)->text();
    valueList["PlistPath"] = ui->table_kernel_add->item(i, 3)->text();
    valueList["MinKernel"] = ui->table_kernel_add->item(i, 4)->text();
    valueList["MaxKernel"] = ui->table_kernel_add->item(i, 5)->text();
    valueList["Enabled"] = getBool(ui->table_kernel_add, i, 6);
    valueList["Arch"] = ui->table_kernel_add->item(i, 7)->text();

    dictList.append(valueList);
  }

  subMap["Add"] = dictList;

  // Block
  dictList.clear();  //必须先清理之前的数据，否则之前的数据会加到当前数据里面来
  valueList.clear();
  for (int i = 0; i < ui->table_kernel_block->rowCount(); i++) {
    valueList["Identifier"] = ui->table_kernel_block->item(i, 0)->text();
    valueList["Comment"] = ui->table_kernel_block->item(i, 1)->text();
    valueList["MinKernel"] = ui->table_kernel_block->item(i, 2)->text();
    valueList["MaxKernel"] = ui->table_kernel_block->item(i, 3)->text();
    valueList["Enabled"] = getBool(ui->table_kernel_block, i, 4);
    valueList["Arch"] = ui->table_kernel_block->item(i, 5)->text();

    dictList.append(valueList);
  }

  subMap["Block"] = dictList;

  // Force
  dictList.clear();
  valueList.clear();
  for (int i = 0; i < ui->table_kernel_Force->rowCount(); i++) {
    valueList["BundlePath"] = ui->table_kernel_Force->item(i, 0)->text();
    valueList["Comment"] = ui->table_kernel_Force->item(i, 1)->text();
    valueList["ExecutablePath"] = ui->table_kernel_Force->item(i, 2)->text();
    valueList["Identifier"] = ui->table_kernel_Force->item(i, 3)->text();
    valueList["PlistPath"] = ui->table_kernel_Force->item(i, 4)->text();
    valueList["MinKernel"] = ui->table_kernel_Force->item(i, 5)->text();
    valueList["MaxKernel"] = ui->table_kernel_Force->item(i, 6)->text();
    valueList["Enabled"] = getBool(ui->table_kernel_Force, i, 7);
    valueList["Arch"] = ui->table_kernel_Force->item(i, 8)->text();

    dictList.append(valueList);
  }

  subMap["Force"] = dictList;

  // Patch
  dictList.clear();
  valueList.clear();
  for (int i = 0; i < ui->table_kernel_patch->rowCount(); i++) {
    valueList["Identifier"] = ui->table_kernel_patch->item(i, 0)->text();
    valueList["Base"] = ui->table_kernel_patch->item(i, 1)->text();
    valueList["Comment"] = ui->table_kernel_patch->item(i, 2)->text();
    valueList["Find"] =
        HexStrToByte(ui->table_kernel_patch->item(i, 3)->text());
    valueList["Replace"] =
        HexStrToByte(ui->table_kernel_patch->item(i, 4)->text());
    valueList["Mask"] =
        HexStrToByte(ui->table_kernel_patch->item(i, 5)->text());
    valueList["ReplaceMask"] =
        HexStrToByte(ui->table_kernel_patch->item(i, 6)->text());
    valueList["MinKernel"] = ui->table_kernel_patch->item(i, 7)->text();
    valueList["MaxKernel"] = ui->table_kernel_patch->item(i, 8)->text();
    valueList["Count"] =
        ui->table_kernel_patch->item(i, 9)->text().toLongLong();
    valueList["Limit"] =
        ui->table_kernel_patch->item(i, 10)->text().toLongLong();
    valueList["Skip"] =
        ui->table_kernel_patch->item(i, 11)->text().toLongLong();
    valueList["Enabled"] = getBool(ui->table_kernel_patch, i, 12);
    valueList["Arch"] = ui->table_kernel_patch->item(i, 13)->text();

    dictList.append(valueList);
  }

  subMap["Patch"] = dictList;

  // Emulate
  QVariantMap mapValue;
  subMap["Emulate"] = setValue(mapValue, ui->tabKernel5);

  // Quirks
  QVariantMap mapQuirks;
  subMap["Quirks"] = setValue(mapQuirks, ui->tabKernel6);

  // Scheme
  QVariantMap mapScheme;
  subMap["Scheme"] = setValue(mapScheme, ui->tabKernel7);

  return subMap;
}

QVariantMap MainWindow::SaveMisc() {
  QVariantMap subMap;
  QVariantList dictList;
  QVariantMap valueList;

  // Boot
  subMap["Boot"] = setValue(valueList, ui->tabMisc1);

  // Debug
  valueList.clear();
  subMap["Debug"] = setValue(valueList, ui->tabMisc2);

  // Security
  valueList.clear();
  subMap["Security"] = setValue(valueList, ui->tabMisc3);

  QString hm = ui->cboxSecureBootModel->currentText().trimmed();
  if (hm == "") hm = "Disabled";
  valueList["SecureBootModel"] = hm;

  // BlessOverride
  for (int i = 0; i < ui->tableBlessOverride->rowCount(); i++) {
    dictList.append(ui->tableBlessOverride->item(i, 0)->text());
  }
  subMap["BlessOverride"] = dictList;

  // Entries
  valueList.clear();
  dictList.clear();
  for (int i = 0; i < ui->tableEntries->rowCount(); i++) {
    valueList["Path"] = ui->tableEntries->item(i, 0)->text();
    valueList["Arguments"] = ui->tableEntries->item(i, 1)->text();
    valueList["Name"] = ui->tableEntries->item(i, 2)->text();
    valueList["Comment"] = ui->tableEntries->item(i, 3)->text();
    valueList["Auxiliary"] = getBool(ui->tableEntries, i, 4);
    valueList["Enabled"] = getBool(ui->tableEntries, i, 5);
    valueList["TextMode"] = getBool(ui->tableEntries, i, 6);
    valueList["Flavour"] = ui->tableEntries->item(i, 7)->text();

    dictList.append(valueList);
  }
  subMap["Entries"] = dictList;

  // Tools
  valueList.clear();
  dictList.clear();
  for (int i = 0; i < ui->tableTools->rowCount(); i++) {
    valueList["Path"] = ui->tableTools->item(i, 0)->text();
    valueList["Arguments"] = ui->tableTools->item(i, 1)->text();
    valueList["Name"] = ui->tableTools->item(i, 2)->text();
    valueList["Comment"] = ui->tableTools->item(i, 3)->text();
    valueList["Auxiliary"] = getBool(ui->tableTools, i, 4);
    valueList["Enabled"] = getBool(ui->tableTools, i, 5);
    valueList["RealPath"] = getBool(ui->tableTools, i, 6);
    valueList["TextMode"] = getBool(ui->tableTools, i, 7);
    valueList["Flavour"] = ui->tableTools->item(i, 8)->text();

    dictList.append(valueList);
  }
  subMap["Tools"] = dictList;

  return subMap;
}

QVariantMap MainWindow::SaveNVRAM() {
  // Add
  QVariantMap subMap;
  QVariantMap dictList;
  QVariantList arrayList;
  QVariantMap valueList;

  int currentRow = ui->table_nv_add0->currentRow();
  for (int i = 0; i < ui->table_nv_add0->rowCount(); i++) {
    valueList.clear();  //先必须清理下列表，很重要
    //先加载表中的数据
    ui->table_nv_add0->setCurrentCell(i, 0);
    on_table_nv_add0_cellClicked(i, 0);

    for (int k = 0; k < ui->table_nv_add->rowCount(); k++) {
      QString dataType = ui->table_nv_add->item(k, 1)->text();  //数据类型
      QString value = ui->table_nv_add->item(k, 2)->text();
      if (dataType == "String")
        valueList[ui->table_nv_add->item(k, 0)->text()] = value;
      if (dataType == "Data") {
        //将以字符串方式显示的16进制原样转换成QByteArray
        valueList[ui->table_nv_add->item(k, 0)->text()] = HexStrToByte(value);
      }
      if (dataType == "Number")
        valueList[ui->table_nv_add->item(k, 0)->text()] = value.toLongLong();
    }
    dictList[ui->table_nv_add0->item(i, 0)->text()] = valueList;
  }
  ui->table_nv_add0->clearSelection();
  ui->table_nv_add0->setCurrentCell(currentRow, 0);
  subMap["Add"] = dictList;

  // Delete
  dictList.clear();  //先清理之前的数据
  currentRow = ui->table_nv_del0->currentRow();
  for (int i = 0; i < ui->table_nv_del0->rowCount(); i++) {
    valueList.clear();  //先必须清理下列表，很重要
    arrayList.clear();

    //先加载表中的数据
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
  dictList.clear();  //先清理之前的数据
  currentRow = ui->table_nv_ls0->currentRow();
  for (int i = 0; i < ui->table_nv_ls0->rowCount(); i++) {
    valueList.clear();  //先必须清理下列表，很重要
    arrayList.clear();

    //先加载表中的数据
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

  subMap["LegacyEnable"] = getChkBool(ui->chkLegacyEnable);
  subMap["LegacyOverwrite"] = getChkBool(ui->chkLegacyOverwrite);
  subMap["WriteFlash"] = getChkBool(ui->chkWriteFlash);

  return subMap;
}

QVariantMap MainWindow::SavePlatformInfo() {
  QVariantMap subMap;
  QVariantMap valueList;

  // Generic
  valueList.clear();
  subMap["Generic"] = setValue(valueList, ui->tabPlatformInfo1);

  if (getSystemProductName(ui->cboxSystemProductName->currentText()) != "")
    valueList["SystemProductName"] =
        getSystemProductName(ui->cboxSystemProductName->currentText());
  else
    valueList["SystemProductName"] = ui->cboxSystemProductName->currentText();

  // DataHub
  valueList.clear();

  if (ui->chkSaveDataHub->isChecked() || !ui->chkAutomatic->isChecked())
    subMap["DataHub"] = setValue(valueList, ui->tabPlatformInfo2);

  // Memory
  valueList.clear();
  valueList["DataWidth"] = ui->editIntDataWidth->text().toLongLong();
  valueList["ErrorCorrection"] =
      ui->editIntErrorCorrection->text().toLongLong();
  valueList["FormFactor"] = ui->editIntFormFactor->text().toLongLong();
  valueList["MaxCapacity"] = ui->editIntMaxCapacity->text().toLongLong();
  valueList["TotalWidth"] = ui->editIntTotalWidth->text().toLongLong();
  valueList["Type"] = ui->editIntType->text().toLongLong();
  valueList["TypeDetail"] = ui->editIntTypeDetail->text().toLongLong();

  // Memory-Devices
  QVariantMap Map;
  QVariantList Array;
  QVariantMap AddSub;

  for (int i = 0; i < ui->tableDevices->rowCount(); i++) {
    AddSub["AssetTag"] = ui->tableDevices->item(i, 0)->text();
    AddSub["BankLocator"] = ui->tableDevices->item(i, 1)->text();
    AddSub["DeviceLocator"] = ui->tableDevices->item(i, 2)->text();
    AddSub["Manufacturer"] = ui->tableDevices->item(i, 3)->text();
    AddSub["PartNumber"] = ui->tableDevices->item(i, 4)->text();
    AddSub["SerialNumber"] = ui->tableDevices->item(i, 5)->text();
    AddSub["Size"] = ui->tableDevices->item(i, 6)->text().toLongLong();
    AddSub["Speed"] = ui->tableDevices->item(i, 7)->text().toLongLong();

    Array.append(AddSub);  //最后一层
  }

  Map["Devices"] = Array;  //第二层

  valueList["Devices"] = Map["Devices"];

  // if (ui->chkCustomMemory->isChecked()) {
  if (ui->chkSaveDataHub->isChecked() || !ui->chkAutomatic->isChecked())
    subMap["Memory"] = valueList;
  //}

  // PlatformNVRAM
  valueList.clear();

  if (ui->chkSaveDataHub->isChecked() || !ui->chkAutomatic->isChecked())
    subMap["PlatformNVRAM"] = setValue(valueList, ui->tabPlatformInfo4);

  // SMBIOS
  valueList.clear();

  if (ui->chkSaveDataHub->isChecked() || !ui->chkAutomatic->isChecked())
    subMap["SMBIOS"] = setValue(valueList, ui->tabPlatformInfo5);

  subMap["Automatic"] = getChkBool(ui->chkAutomatic);
  subMap["CustomMemory"] = getChkBool(ui->chkCustomMemory);
  subMap["UpdateDataHub"] = getChkBool(ui->chkUpdateDataHub);
  subMap["UpdateNVRAM"] = getChkBool(ui->chkUpdateNVRAM);
  subMap["UpdateSMBIOS"] = getChkBool(ui->chkUpdateSMBIOS);

  subMap["UseRawUuidEncoding"] = getChkBool(ui->chkUseRawUuidEncoding);

  subMap["UpdateSMBIOSMode"] = ui->cboxUpdateSMBIOSMode->currentText();

  return subMap;
}

QVariantMap MainWindow::SaveUEFI() {
  QVariantMap subMap;
  QVariantMap dictList;
  QVariantList arrayList;
  QVariantMap valueList;

  // 1. APFS
  subMap["APFS"] = setValue(dictList, ui->tabUEFI1);

  // 2. AppleInput
  subMap["AppleInput"] = setValue(dictList, ui->tabUEFI2);

  // 3. Audio
  dictList.clear();
  subMap["Audio"] = setValue(dictList, ui->tabUEFI3);

  // 4. Drivers
  arrayList.clear();
  QVariantMap uefiAddSub;
  for (int i = 0; i < ui->table_uefi_drivers->rowCount(); i++) {
    // arrayList.append(ui->table_uefi_drivers->item(i, 0)->text());
    // //之前只有一条记录的情况

    uefiAddSub["Path"] = ui->table_uefi_drivers->item(i, 0)->text();

    uefiAddSub["Enabled"] = getBool(ui->table_uefi_drivers, i, 1);

    uefiAddSub["Arguments"] = ui->table_uefi_drivers->item(i, 2)->text();

    uefiAddSub["Comment"] = ui->table_uefi_drivers->item(i, 3)->text();

    arrayList.append(uefiAddSub);  //最后一层
  }
  subMap["Drivers"] = arrayList;
  subMap["ConnectDrivers"] = getChkBool(ui->chkConnectDrivers);

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
  arrayList.clear();
  valueList.clear();
  for (int i = 0; i < ui->table_uefi_ReservedMemory->rowCount(); i++) {
    valueList["Address"] =
        ui->table_uefi_ReservedMemory->item(i, 0)->text().toLongLong();
    valueList["Comment"] = ui->table_uefi_ReservedMemory->item(i, 1)->text();
    valueList["Size"] =
        ui->table_uefi_ReservedMemory->item(i, 2)->text().toLongLong();
    valueList["Type"] = ui->table_uefi_ReservedMemory->item(i, 3)->text();
    valueList["Enabled"] = getBool(ui->table_uefi_ReservedMemory, i, 4);

    arrayList.append(valueList);
  }
  subMap["ReservedMemory"] = arrayList;

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
        qDebug() << "非法的16进制字符：" << s;
        QMessageBox::warning(0, tr("错误："),
                             QString("非法的16进制字符: \"%1\"").arg(s));
      }
    }
  }
  qDebug() << ret;
  return ret;
}

void MainWindow::on_table_acpi_add_cellClicked(int row, int column) {
  if (!ui->table_acpi_add->currentIndex().isValid()) return;

  enabled_change(ui->table_acpi_add, row, column, 1);

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

  enabled_change(ui->table_acpi_del, row, column, 3);
  enabled_change(ui->table_acpi_del, row, column, 4);

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

  if (table->currentColumn() != 0)
    ui->statusbar->showMessage(text0 + " -> " + str);
  else
    ui->statusbar->showMessage(str);

  if (getTableFieldDataType(table) == "Data") {
    ui->txtEditHex->setText(table->currentItem()->text());

    setConversionWidgetVisible(true);
  } else {
    setConversionWidgetVisible(false);
  }
}

void MainWindow::on_table_acpi_patch_cellClicked(int row, int column) {
  if (!ui->table_acpi_patch->currentIndex().isValid()) return;

  enabled_change(ui->table_acpi_patch, row, column, 11);

  setStatusBarText(ui->table_acpi_patch);
}

void MainWindow::on_table_booter_cellClicked(int row, int column) {
  if (!ui->table_booter->currentIndex().isValid()) return;

  enabled_change(ui->table_booter, row, column, 1);

  setStatusBarText(ui->table_booter);
}

void MainWindow::on_table_kernel_add_cellClicked(int row, int column) {
  if (!ui->table_kernel_add->currentIndex().isValid()) return;

  enabled_change(ui->table_kernel_add, row, column, 6);

  if (column == 7) {
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
    cboxArch->setCurrentText(ui->table_kernel_add->item(row, 7)->text());
  }

  setStatusBarText(ui->table_kernel_add);
}

void MainWindow::on_table_kernel_block_cellClicked(int row, int column) {
  if (!ui->table_kernel_block->currentIndex().isValid()) return;

  enabled_change(ui->table_kernel_block, row, column, 4);

  if (column == 5) {
    cboxArch = new QComboBox;
    cboxArch->setEditable(true);
    cboxArch->addItem("Any");
    cboxArch->addItem("i386");
    cboxArch->addItem("x86_64");
    cboxArch->addItem("");

    connect(cboxArch, SIGNAL(currentTextChanged(QString)), this,
            SLOT(arch_blockChange()));
    c_row = row;

    ui->table_kernel_block->setCellWidget(row, column, cboxArch);
    cboxArch->setCurrentText(ui->table_kernel_block->item(row, 5)->text());
  }

  setStatusBarText(ui->table_kernel_block);
}

void MainWindow::on_table_kernel_patch_cellClicked(int row, int column) {
  if (!ui->table_kernel_patch->currentIndex().isValid()) return;

  enabled_change(ui->table_kernel_patch, row, column, 12);

  if (column == 13) {
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
    cboxArch->setCurrentText(ui->table_kernel_patch->item(row, 13)->text());
  }

  setStatusBarText(ui->table_kernel_patch);
}

void MainWindow::on_tableEntries_cellClicked(int row, int column) {
  if (!ui->tableEntries->currentIndex().isValid()) return;

  enabled_change(ui->tableEntries, row, column, 5);

  enabled_change(ui->tableEntries, row, column, 4);

  enabled_change(ui->tableEntries, row, column, 6);

  setStatusBarText(ui->tableEntries);
}

void MainWindow::on_tableTools_cellClicked(int row, int column) {
  if (!ui->tableTools->currentIndex().isValid()) return;

  enabled_change(ui->tableTools, row, column, 5);

  enabled_change(ui->tableTools, row, column, 4);

  enabled_change(ui->tableTools, row, column, 6);

  enabled_change(ui->tableTools, row, column, 7);

  setStatusBarText(ui->tableTools);
}

void MainWindow::on_table_uefi_ReservedMemory_cellClicked(int row, int column) {
  if (!ui->table_uefi_ReservedMemory->currentIndex().isValid()) return;

  enabled_change(ui->table_uefi_ReservedMemory, row, column, 4);

  if (column == 3) {
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
        ui->table_uefi_ReservedMemory->item(row, 3)->text());
  }

  setStatusBarText(ui->table_uefi_ReservedMemory);
}

void MainWindow::on_btnKernelPatchAdd_clicked() {
  add_item(ui->table_kernel_patch, 12);
  init_enabled_data(ui->table_kernel_patch,
                    ui->table_kernel_patch->rowCount() - 1, 12, "true");

  QTableWidgetItem* newItem1 = new QTableWidgetItem("Any");
  newItem1->setTextAlignment(Qt::AlignCenter);
  ui->table_kernel_patch->setItem(ui->table_kernel_patch->currentRow(), 13,
                                  newItem1);

  this->setWindowModified(true);
}

void MainWindow::on_btnKernelPatchDel_clicked() {
  del_item(ui->table_kernel_patch);
}

void MainWindow::add_item(QTableWidget* table, int total_column) {
  int t = table->rowCount();
  table->setRowCount(t + 1);

  //用""初始化各项值
  for (int i = 0; i < total_column; i++) {
    table->setItem(t, i, new QTableWidgetItem(""));
  }
  table->setFocus();
  table->setCurrentCell(t, 0);
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
  if (table->rowCount() == 0) return;

  int row = table->currentRow();

  // std::vector<int> vecItemIndex; //保存选中行的索引
  QItemSelectionModel* selections =
      table->selectionModel();  //返回当前的选择模式
  QModelIndexList selectedsList =
      selections->selectedIndexes();  //返回所有选定的模型项目索引列表

  for (int i = 0; i < selectedsList.count(); i++) {
    // vecItemIndex.push_back(selectedsList.at(i).row());
    int t = selectedsList.at(i).row();

    //删除部分的Redo/Undo
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
}

void MainWindow::on_btnACPIAdd_Del_clicked() { del_item(ui->table_acpi_add); }

void MainWindow::on_btnACPIDel_Add_clicked() {
  add_item(ui->table_acpi_del, 6);

  init_enabled_data(ui->table_acpi_del, ui->table_acpi_del->rowCount() - 1, 3,
                    "false");
  init_enabled_data(ui->table_acpi_del, ui->table_acpi_del->rowCount() - 1, 4,
                    "true");

  this->setWindowModified(true);
}

void MainWindow::on_btnACPIDel_Del_clicked() { del_item(ui->table_acpi_del); }

void MainWindow::on_btnACPIPatch_Add_clicked() {
  add_item(ui->table_acpi_patch, 14);
  init_enabled_data(ui->table_acpi_patch, ui->table_acpi_patch->rowCount() - 1,
                    11, "true");

  this->setWindowModified(true);
}

void MainWindow::on_btnACPIPatch_Del_clicked() {
  del_item(ui->table_acpi_patch);
}

void MainWindow::on_btnBooter_Add_clicked() {
  add_item(ui->table_booter, 3);
  init_enabled_data(ui->table_booter, ui->table_booter->rowCount() - 1, 1,
                    "true");

  this->setWindowModified(true);
}

void MainWindow::on_btnBooter_Del_clicked() { del_item(ui->table_booter); }

void MainWindow::on_btnDPDel_Add0_clicked() {
  add_item(ui->table_dp_del0, 1);
  ui->table_dp_del->setRowCount(0);  //先清除右边表中的所有条目
  on_btnDPDel_Add_clicked();         //同时右边增加一个新条目

  write_value_ini(ui->table_dp_del0, ui->table_dp_del,
                  ui->table_dp_del0->rowCount() - 1);

  this->setWindowModified(true);
}

void MainWindow::on_btnDPDel_Del0_clicked() {
  if (ui->table_dp_del0->rowCount() == 0) return;

  //先记住被删的条目位置
  int delindex = ui->table_dp_del0->currentRow();
  int count = ui->table_dp_del0->rowCount();

  QString qz = QDir::homePath() + "/.config/QtOCC/" + CurrentDateTime +
               ui->table_dp_del0->objectName();
  QFile file(qz + QString::number(delindex + 1) + ".ini");
  del_item(ui->table_dp_del0);
  if (file.exists()) file.remove();

  //改名，以适应新的索引
  if (delindex < count) {
    for (int i = delindex; i < ui->table_dp_del0->rowCount(); i++) {
      QFile file(qz + QString::number(i + 2) + ".ini");
      file.rename(qz + QString::number(i + 1) + ".ini");
    }
  }

  if (ui->table_dp_del0->rowCount() == 0) {
    ui->table_dp_del->setRowCount(0);
  }

  if (ui->table_dp_del0->rowCount() > 0)
    on_table_dp_del0_cellClicked(ui->table_dp_del0->currentRow(), 0);

  this->setWindowModified(true);
}

void MainWindow::on_btnDPDel_Add_clicked() {
  if (ui->table_dp_del0->rowCount() == 0) return;

  loading = true;
  add_item(ui->table_dp_del, 1);
  loading = false;

  //保存数据
  write_value_ini(ui->table_dp_del0, ui->table_dp_del,
                  ui->table_dp_del0->currentRow());

  this->setWindowModified(true);
}

void MainWindow::on_btnDPDel_Del_clicked() {
  del_item(ui->table_dp_del);

  //保存数据
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

  for (int i = 0; i < FileName.count(); i++) {
    int row = ui->table_acpi_add->rowCount() + 1;

    ui->table_acpi_add->setRowCount(row);

    // QUndoCommand* addCommand = new AddCommand(ui->table_acpi_add, row - 1, 0,
    // QFileInfo(FileName.at(i)).fileName()); undoStack->push(addCommand);
    ui->table_acpi_add->setItem(
        row - 1, 0, new QTableWidgetItem(QFileInfo(FileName.at(i)).fileName()));
    init_enabled_data(ui->table_acpi_add, row - 1, 1, "true");
    ui->table_acpi_add->setItem(row - 1, 2, new QTableWidgetItem(""));

    ui->table_acpi_add->setFocus();
    ui->table_acpi_add->setCurrentCell(row - 1, 0);
  }

  this->setWindowModified(true);
}

void MainWindow::on_btnDPAdd_Add0_clicked() {
  loading = true;

  add_item(ui->table_dp_add0, 1);
  ui->table_dp_add->setRowCount(0);  //先清除右边表中的所有条目
  on_btnDPAdd_Add_clicked();         //同时右边增加一个新条目
  write_ini(ui->table_dp_add0, ui->table_dp_add,
            ui->table_dp_add0->rowCount() - 1);

  this->setWindowModified(true);

  loading = false;
}

void MainWindow::on_btnDPAdd_Del0_clicked() {
  if (ui->table_dp_add0->rowCount() == 0) return;

  del_item(ui->table_dp_add0);

  if (ui->table_dp_add0->rowCount() == 0) {
    ui->table_dp_add->setRowCount(0);
  }

  if (ui->table_dp_add0->rowCount() > 0)
    on_table_dp_add0_cellClicked(ui->table_dp_add0->currentRow(), 0);

  this->setWindowModified(true);
}

void MainWindow::on_btnDPAdd_Add_clicked() {
  if (ui->table_dp_add0->rowCount() == 0) return;

  loading = true;
  add_item(ui->table_dp_add, 3);
  loading = false;

  //保存数据
  write_ini(ui->table_dp_add0, ui->table_dp_add,
            ui->table_dp_add0->currentRow());

  this->setWindowModified(true);
}

void MainWindow::on_btnDPAdd_Del_clicked() {
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

  addKexts(FileName);
}

void MainWindow::addKexts(QStringList FileName) {
  int file_count = FileName.count();

  if (file_count == 0 || FileName[0] == "") return;

  for (int k = 0; k < FileName.count(); k++) {
    QString file = FileName.at(k);
    QFileInfo fi(file);
    if (fi.baseName().toLower() == "lilu") {
      FileName.removeAt(k);
      FileName.insert(0, file);
    }

    if (fi.baseName().toLower() == "virtualsmc") {
      FileName.removeAt(k);
      FileName.insert(1, file);
    }
  }

  for (int j = 0; j < file_count; j++) {
    QFileInfo fileInfo(FileName[j]);

    QFileInfo fileInfoList;
    QString filePath = fileInfo.absolutePath();

    QDir fileDir(filePath + "/" + fileInfo.fileName() + "/Contents/MacOS/");

    if (fileDir.exists())  //如果目录存在，则遍历里面的文件
    {
      fileDir.setFilter(QDir::Files);  //只遍历本目录
      QFileInfoList fileList = fileDir.entryInfoList();
      int fileCount = fileList.count();
      for (int i = 0; i < fileCount; i++)  //一般只有一个二进制文件
      {
        fileInfoList = fileList[i];
      }
    }

    QTableWidget* t = new QTableWidget;
    t = ui->table_kernel_add;
    int row = t->rowCount() + 1;

    t->setRowCount(row);
    t->setItem(row - 1, 0,
               new QTableWidgetItem(QFileInfo(FileName[j]).fileName()));
    t->setItem(row - 1, 1, new QTableWidgetItem(""));

    if (fileInfoList.fileName() != "")
      t->setItem(
          row - 1, 2,
          new QTableWidgetItem("Contents/MacOS/" + fileInfoList.fileName()));
    else
      t->setItem(row - 1, 2, new QTableWidgetItem(""));

    t->setItem(row - 1, 3, new QTableWidgetItem("Contents/Info.plist"));
    t->setItem(row - 1, 4, new QTableWidgetItem(""));
    t->setItem(row - 1, 5, new QTableWidgetItem(""));
    init_enabled_data(t, row - 1, 6, "true");

    QTableWidgetItem* newItem1 = new QTableWidgetItem("x86_64");
    newItem1->setTextAlignment(Qt::AlignCenter);
    t->setItem(row - 1, 7, newItem1);

    //如果里面还有PlugIns目录，则需要继续遍历插件目录
    QDir piDir(filePath + "/" + fileInfo.fileName() + "/Contents/PlugIns/");

    if (piDir.exists()) {
      piDir.setFilter(QDir::Dirs);  //过滤器：只遍历里面的目录
      QFileInfoList fileList = piDir.entryInfoList();
      int fileCount = fileList.count();
      QVector<QString> kext_file;

      for (int i = 0; i < fileCount; i++)  //找出里面的kext文件(目录）
      {
        kext_file.push_back(fileList[i].fileName());
      }

      if (fileCount >= 3)  //里面有目录
      {
        for (int i = 0; i < fileCount - 2; i++) {
          QDir fileDir(filePath + "/" + fileInfo.fileName() +
                       "/Contents/PlugIns/" + kext_file[i + 2] +
                       "/Contents/MacOS/");
          if (fileDir.exists()) {
            fileDir.setFilter(QDir::Files);  //只遍历本目录里面的文件
            QFileInfoList fileList = fileDir.entryInfoList();
            int fileCount = fileList.count();
            for (int i = 0; i < fileCount; i++)  //一般只有一个二进制文件
            {
              fileInfoList = fileList[i];
            }

            //写入到表里
            int row = t->rowCount() + 1;

            t->setRowCount(row);
            t->setItem(
                row - 1, 0,
                new QTableWidgetItem(QFileInfo(FileName[j]).fileName() +
                                     "/Contents/PlugIns/" + kext_file[i + 2]));
            t->setItem(row - 1, 1, new QTableWidgetItem(""));

            t->setItem(row - 1, 2,
                       new QTableWidgetItem("Contents/MacOS/" +
                                            fileInfoList.fileName()));

            t->setItem(row - 1, 3, new QTableWidgetItem("Contents/Info.plist"));
            t->setItem(row - 1, 4, new QTableWidgetItem(""));
            t->setItem(row - 1, 5, new QTableWidgetItem(""));
            init_enabled_data(t, row - 1, 6, "true");

            QTableWidgetItem* newItem1 = new QTableWidgetItem("x86_64");
            newItem1->setTextAlignment(Qt::AlignCenter);
            t->setItem(row - 1, 7, newItem1);

          } else {  //不存在二进制文件，只存在一个Info.plist文件的情况

            QDir fileDir(filePath + "/" + fileInfo.fileName() +
                         "/Contents/PlugIns/" + kext_file[i + 2] +
                         "/Contents/");
            if (fileDir.exists()) {
              //写入到表里
              int row = t->rowCount() + 1;

              t->setRowCount(row);
              t->setItem(row - 1, 0,
                         new QTableWidgetItem(
                             QFileInfo(FileName[j]).fileName() +
                             "/Contents/PlugIns/" + kext_file[i + 2]));
              t->setItem(row - 1, 1, new QTableWidgetItem(""));
              t->setItem(row - 1, 2, new QTableWidgetItem(""));
              t->setItem(row - 1, 3,
                         new QTableWidgetItem("Contents/Info.plist"));
              t->setItem(row - 1, 4, new QTableWidgetItem(""));
              t->setItem(row - 1, 5, new QTableWidgetItem(""));
              init_enabled_data(t, row - 1, 6, "true");

              QTableWidgetItem* newItem1 = new QTableWidgetItem("x86_64");
              newItem1->setTextAlignment(Qt::AlignCenter);
              t->setItem(row - 1, 7, newItem1);
            }
          }
        }
      }
    }

    t->setFocus();
    t->setCurrentCell(row - 1, 0);
  }

  this->setWindowModified(true);
}

void MainWindow::on_btnKernelBlock_Add_clicked() {
  add_item(ui->table_kernel_block, 4);
  init_enabled_data(ui->table_kernel_block,
                    ui->table_kernel_block->currentRow(), 4, "true");

  QTableWidgetItem* newItem1 = new QTableWidgetItem("Any");
  newItem1->setTextAlignment(Qt::AlignCenter);
  ui->table_kernel_block->setItem(ui->table_kernel_block->currentRow(), 5,
                                  newItem1);

  this->setWindowModified(true);
}

void MainWindow::on_btnKernelBlock_Del_clicked() {
  del_item(ui->table_kernel_block);
}

void MainWindow::on_btnMiscBO_Add_clicked() {
  add_item(ui->tableBlessOverride, 1);
  ui->tableBlessOverride->setItem(ui->tableBlessOverride->currentRow(), 0,
                                  new QTableWidgetItem(""));

  this->setWindowModified(true);
}

void MainWindow::on_btnMiscBO_Del_clicked() {
  del_item(ui->tableBlessOverride);
}

void MainWindow::on_btnMiscEntries_Add_clicked() {
  add_item(ui->tableEntries, 7);

  int row = ui->tableEntries->rowCount() - 1;

  init_enabled_data(ui->tableEntries, row, 4, "false");
  init_enabled_data(ui->tableEntries, row, 5, "true");
  init_enabled_data(ui->tableEntries, row, 6, "false");

  ui->tableEntries->setItem(row, 7, new QTableWidgetItem("Auto"));

  this->setWindowModified(true);
}

void MainWindow::on_btnMiscTools_Add_clicked() {
  QFileDialog fd;

  QStringList FileName = fd.getOpenFileNames(this, "tools efi file", "",
                                             "efi file(*.efi);;all files(*.*)");

  addEFITools(FileName);
}

void MainWindow::addEFITools(QStringList FileName) {
  if (FileName.isEmpty()) return;

  for (int i = 0; i < FileName.count(); i++) {
    add_item(ui->tableTools, 9);

    int row = ui->tableTools->rowCount() - 1;

    ui->tableTools->setItem(
        row, 0, new QTableWidgetItem(QFileInfo(FileName.at(i)).fileName()));

    ui->tableTools->setItem(
        row, 2, new QTableWidgetItem(QFileInfo(FileName.at(i)).baseName()));

    ui->tableTools->setFocus();
    ui->tableTools->setCurrentCell(row, 0);

    init_enabled_data(ui->tableTools, row, 4, "false");
    init_enabled_data(ui->tableTools, row, 5, "true");
    init_enabled_data(ui->tableTools, row, 6, "false");
    init_enabled_data(ui->tableTools, row, 7, "false");

    ui->tableTools->setItem(row, 8, new QTableWidgetItem("Auto"));
  }

  this->setWindowModified(true);
}

void MainWindow::on_btnMiscEntries_Del_clicked() { del_item(ui->tableEntries); }

void MainWindow::on_btnMiscTools_Del_clicked() { del_item(ui->tableTools); }

void MainWindow::on_btnNVRAMAdd_Add0_clicked() {
  add_item(ui->table_nv_add0, 1);
  ui->table_nv_add->setRowCount(0);  //先清除右边表中的所有条目
  on_btnNVRAMAdd_Add_clicked();      //同时右边增加一个新条目

  write_ini(ui->table_nv_add0, ui->table_nv_add,
            ui->table_nv_add0->rowCount() - 1);

  this->setWindowModified(true);
}

void MainWindow::on_btnNVRAMAdd_Add_clicked() {
  if (ui->table_nv_add0->rowCount() == 0) return;

  loading = true;
  add_item(ui->table_nv_add, 3);
  loading = false;

  //保存数据
  write_ini(ui->table_nv_add0, ui->table_nv_add,
            ui->table_nv_add0->currentRow());

  this->setWindowModified(true);
}

void MainWindow::on_btnNVRAMAdd_Del0_clicked() {
  int count = ui->table_nv_add0->rowCount();

  if (count == 0) return;

  //先记住被删的条目位置
  int delindex = ui->table_nv_add0->currentRow();

  QString qz = QDir::homePath() + "/.config/QtOCC/" + CurrentDateTime +
               ui->table_nv_add0->objectName();
  QFile file(qz + QString::number(delindex + 1) + ".ini");
  del_item(ui->table_nv_add0);
  if (file.exists()) file.remove();

  //改名，以适应新的索引
  if (delindex < count) {
    for (int i = delindex; i < ui->table_nv_add0->rowCount(); i++) {
      QFile file(qz + QString::number(i + 2) + ".ini");
      file.rename(qz + QString::number(i + 1) + ".ini");
    }
  }

  if (ui->table_nv_add0->rowCount() == 0) {
    ui->table_nv_add->setRowCount(0);
  }

  if (ui->table_nv_add0->rowCount() > 0)
    on_table_nv_add0_cellClicked(ui->table_nv_add0->currentRow(), 0);

  this->setWindowModified(true);
}

void MainWindow::on_btnNVRAMAdd_Del_clicked() {
  del_item(ui->table_nv_add);
  write_ini(ui->table_nv_add0, ui->table_nv_add,
            ui->table_nv_add0->currentRow());
}

void MainWindow::on_btnNVRAMDel_Add0_clicked() {
  add_item(ui->table_nv_del0, 1);
  ui->table_nv_del->setRowCount(0);  //先清除右边表中的所有条目
  on_btnNVRAMDel_Add_clicked();      //同时右边增加一个新条目

  write_value_ini(ui->table_nv_del0, ui->table_nv_del,
                  ui->table_nv_del0->rowCount() - 1);

  this->setWindowModified(true);
}

void MainWindow::on_btnNVRAMDel_Add_clicked() {
  if (ui->table_nv_del0->rowCount() == 0) return;

  loading = true;
  add_item(ui->table_nv_del, 1);
  loading = false;

  //保存数据
  write_value_ini(ui->table_nv_del0, ui->table_nv_del,
                  ui->table_nv_del0->currentRow());

  this->setWindowModified(true);
}

void MainWindow::on_btnNVRAMLS_Add0_clicked() {
  add_item(ui->table_nv_ls0, 1);
  ui->table_nv_ls->setRowCount(0);  //先清除右边表中的所有条目
  on_btnNVRAMLS_Add_clicked();      //同时右边增加一个新条目

  write_value_ini(ui->table_nv_ls0, ui->table_nv_ls,
                  ui->table_nv_ls0->rowCount() - 1);

  this->setWindowModified(true);
}

void MainWindow::on_btnNVRAMLS_Add_clicked() {
  if (ui->table_nv_ls0->rowCount() == 0) return;

  loading = true;
  add_item(ui->table_nv_ls, 1);
  loading = false;

  //保存数据
  write_value_ini(ui->table_nv_ls0, ui->table_nv_ls,
                  ui->table_nv_ls0->currentRow());

  this->setWindowModified(true);
}

void MainWindow::on_btnNVRAMDel_Del0_clicked() {
  if (ui->table_nv_del0->rowCount() == 0) return;

  //先记住被删的条目位置
  int delindex = ui->table_nv_del0->currentRow();
  int count = ui->table_nv_del0->rowCount();

  QString qz = QDir::homePath() + "/.config/QtOCC/" + CurrentDateTime +
               ui->table_nv_del0->objectName();
  QFile file(qz + QString::number(delindex + 1) + ".ini");
  del_item(ui->table_nv_del0);
  if (file.exists()) file.remove();

  //改名，以适应新的索引
  if (delindex < count) {
    for (int i = delindex; i < ui->table_nv_del0->rowCount(); i++) {
      QFile file(qz + QString::number(i + 2) + ".ini");
      file.rename(qz + QString::number(i + 1) + ".ini");
    }
  }

  if (ui->table_nv_del0->rowCount() == 0) {
    ui->table_nv_del->setRowCount(0);
  }

  if (ui->table_nv_del0->rowCount() > 0)
    on_table_nv_del0_cellClicked(ui->table_nv_del0->currentRow(), 0);

  this->setWindowModified(true);
}

void MainWindow::on_btnNVRAMLS_Del0_clicked() {
  if (ui->table_nv_ls0->rowCount() == 0) return;

  //先记住被删的条目位置
  int delindex = ui->table_nv_ls0->currentRow();
  int count = ui->table_nv_ls0->rowCount();

  QString qz = QDir::homePath() + "/.config/QtOCC/" + CurrentDateTime +
               ui->table_nv_ls0->objectName();
  QFile file(qz + QString::number(delindex + 1) + ".ini");
  del_item(ui->table_nv_ls0);
  if (file.exists()) file.remove();

  //改名，以适应新的索引
  if (delindex < count) {
    for (int i = delindex; i < ui->table_nv_ls0->rowCount(); i++) {
      QFile file(qz + QString::number(i + 2) + ".ini");
      file.rename(qz + QString::number(i + 1) + ".ini");
    }
  }

  if (ui->table_nv_ls0->rowCount() == 0) {
    ui->table_nv_ls->setRowCount(0);
  }

  if (ui->table_nv_ls0->rowCount() > 0)
    on_table_nv_ls0_cellClicked(ui->table_nv_ls0->currentRow(), 0);

  this->setWindowModified(true);
}

void MainWindow::on_btnNVRAMDel_Del_clicked() {
  del_item(ui->table_nv_del);

  //保存数据
  write_value_ini(ui->table_nv_del0, ui->table_nv_del,
                  ui->table_nv_del0->currentRow());
}

void MainWindow::on_btnNVRAMLS_Del_clicked() {
  del_item(ui->table_nv_ls);

  //保存数据
  write_value_ini(ui->table_nv_ls0, ui->table_nv_ls,
                  ui->table_nv_ls0->currentRow());
}

void MainWindow::on_btnUEFIRM_Add_clicked() {
  add_item(ui->table_uefi_ReservedMemory, 5);

  int row = ui->table_uefi_ReservedMemory->currentRow();
  ui->table_uefi_ReservedMemory->item(row, 3)->setText("Reserved");
  init_enabled_data(ui->table_uefi_ReservedMemory,
                    ui->table_uefi_ReservedMemory->rowCount() - 1, 4, "true");

  this->setWindowModified(true);
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

  for (int i = 0; i < FileName.count(); i++) {
    int row = ui->table_uefi_drivers->rowCount() + 1;

    ui->table_uefi_drivers->setRowCount(row);
    ui->table_uefi_drivers->setItem(
        row - 1, 0, new QTableWidgetItem(QFileInfo(FileName.at(i)).fileName()));
    init_enabled_data(ui->table_uefi_drivers, row - 1, 1, "true");
    ui->table_uefi_drivers->setItem(row - 1, 2, new QTableWidgetItem(""));

    ui->table_uefi_drivers->setItem(row - 1, 3, new QTableWidgetItem(""));

    ui->table_uefi_drivers->setFocus();
    ui->table_uefi_drivers->setCurrentCell(row - 1, 0);
  }

  this->setWindowModified(true);
}

void MainWindow::on_btnUEFIDrivers_Del_clicked() {
  del_item(ui->table_uefi_drivers);
}

void MainWindow::MoveItem(QTableWidget* t, bool up) {
  t->setFocus();

  if (up) {
    if (t->rowCount() == 0 || t->currentRow() == 0 || t->currentRow() < 0)
      return;
  } else {
    if (t->currentRow() == t->rowCount() - 1 || t->currentRow() < 0) return;
  }

  QStringList items;
  int cr = t->currentRow();
  int cols = t->columnCount();
  QList<int> enabledList;
  for (int i = 0; i < cols; i++) {
    if (t->horizontalHeaderItem(i)->text() == tr("Enabled"))
      enabledList.append(i);
  }

  if (up) {
    //先将上面的内容进行备份
    for (int i = 0; i < cols; i++) {
      items.append(t->item(cr - 1, i)->text());
    }

    //将下面的内容移到上面
    for (int i = 0; i < cols; i++) {
      t->item(cr - 1, i)->setText(t->item(cr, i)->text());
    }

    for (int i = 0; i < enabledList.count(); i++) {
      if (t->item(cr, enabledList.at(i))->text() == "true")
        t->item(cr - 1, enabledList.at(i))->setCheckState(Qt::Checked);
      else
        t->item(cr - 1, enabledList.at(i))->setCheckState(Qt::Unchecked);
    }

    //最后将之前的备份恢复到下面
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
    //先将下面的内容进行备份
    for (int i = 0; i < cols; i++) {
      items.append(t->item(cr + 1, i)->text());
    }

    //将上面的内容移到下面
    for (int i = 0; i < cols; i++) {
      t->item(cr + 1, i)->setText(t->item(cr, i)->text());
    }

    for (int i = 0; i < enabledList.count(); i++) {
      if (t->item(cr, enabledList.at(i))->text() == "true")
        t->item(cr + 1, enabledList.at(i))->setCheckState(Qt::Checked);
      else
        t->item(cr + 1, enabledList.at(i))->setCheckState(Qt::Unchecked);
    }

    //最后将之前的备份恢复到上面
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

  this->setWindowModified(true);
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
    SaveFileName = PlistFileName;
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
  ui->table_kernel_add->item(c_row, 7)->setText(cboxArch->currentText());
}

void MainWindow::arch_ForceChange() {
  ui->table_kernel_Force->item(c_row, 8)->setText(cboxArch->currentText());
}

void MainWindow::arch_blockChange() {
  ui->table_kernel_block->item(c_row, 5)->setText(cboxArch->currentText());
}

void MainWindow::arch_patchChange() {
  ui->table_kernel_patch->item(c_row, 13)->setText(cboxArch->currentText());
}

void MainWindow::arch_Booter_patchChange() {
  ui->table_Booter_patch->item(c_row, 10)->setText(cboxArch->currentText());
}

void MainWindow::ReservedMemoryTypeChange() {
  ui->table_uefi_ReservedMemory->item(c_row, 3)->setText(
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
    cboxDataClass->addItem("");
    connect(cboxDataClass, SIGNAL(currentTextChanged(QString)), this,
            SLOT(dataClassChange_nv()));
    c_row = row;

    ui->table_nv_add->setCellWidget(row, column, cboxDataClass);
    cboxDataClass->setCurrentText(ui->table_nv_add->item(row, 1)->text());
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
  if (!loading) {
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
      QValidator* validator = new QRegExpValidator(regxNumber, lineEdit);
      lineEdit->setValidator(validator);
      lineEdit->setPlaceholderText(tr("Integer"));
      lineEdit->setToolTip(tr("Integer"));
    }

    if (getTableFieldDataType(myTable) == "Data") {
      QValidator* validator = new QRegExpValidator(regx, lineEdit);
      lineEdit->setValidator(validator);
      lineEdit->setPlaceholderText(tr("Hexadecimal"));
      lineEdit->setToolTip(tr("Hexadecimal"));
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
  QStringList strHeaderList = strHeader.split("\n");

  if (strHeaderList.count() == 2 && strHeaderList.at(1) != "Base") {
    if (stringInt.contains(strHeaderList.at(1))) {
      return "Int";
    }

    if (stringData.contains(strHeaderList.at(1))) {
      return "Data";
    }
  }

  if (strHeaderList.count() == 1 && strHeaderList.at(0) != "Base") {
    if (stringInt.contains(strHeaderList.at(0))) {
      return "Int";
    }

    if (stringData.contains(strHeaderList.at(0))) {
      return "Data";
    }
  }

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
  QSettings* regShell =
      new QSettings("HKEY_CLASSES_ROOT\\QtOpenCoreConfig\\shell\\open\\command",
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

      break;
    }
  }

  return str;
}

void MainWindow::on_cboxSystemProductName_currentIndexChanged(
    const QString& arg1) {
  if (!loading && arg1 != "") {
    QFileInfo appInfo(qApp->applicationDirPath());
    gs = new QProcess;

    QString str = getSystemProductName(arg1);
    ui->editSystemProductName->setText(str);
    ui->editSystemProductName_2->setText(str);

#ifdef Q_OS_WIN32

    gs->start(appInfo.filePath() + "/Database/win/macserial.exe",
              QStringList() << "-m" << str);  //阻塞为execute

#endif

#ifdef Q_OS_LINUX

    gs->start(appInfo.filePath() + "/Database/linux/macserial",
              QStringList() << "-m" << str);

#endif

#ifdef Q_OS_MAC

    gs->start(appInfo.filePath() + "/Database/mac/macserial",
              QStringList() << "-m" << str);

#endif

    connect(gs, SIGNAL(finished(int)), this, SLOT(readResult()));
    // connect(gs , SIGNAL(readyRead()) , this , SLOT(readResult()));
  }

  this->setWindowModified(true);
}

void MainWindow::readResult() {
  QTextEdit* textMacInfo = new QTextEdit;
  QTextCodec* gbkCodec = QTextCodec::codecForName("UTF-8");
  textMacInfo->clear();
  QString result = gbkCodec->toUnicode(gs->readAll());
  textMacInfo->append(result);
  //取第三行的数据，第一行留给提示用
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

void MainWindow::readResultSystemInfo() {
  ui->textMacInfo->clear();
  QTextCodec* gbkCodec = QTextCodec::codecForName("UTF-8");
  QString result = gbkCodec->toUnicode(si->readAll());
  ui->textMacInfo->append(result);
}

void MainWindow::on_btnGenerate_clicked() {
  on_cboxSystemProductName_currentIndexChanged(
      ui->cboxSystemProductName->currentText());
}

void MainWindow::on_btnSystemUUID_clicked() {
  QUuid id = QUuid::createUuid();
  QString strTemp = id.toString();
  QString strId = strTemp.mid(1, strTemp.count() - 2).toUpper();

  ui->editSystemUUID->setText(strId);
  ui->editSystemUUID_data->setText(strId);
  ui->editSystemUUID_2->setText(strId);
  ui->editSystemUUID_PNVRAM->setText(strId);
}

void MainWindow::on_table_kernel_Force_cellClicked(int row, int column) {
  if (!ui->table_kernel_Force->currentIndex().isValid()) return;

  enabled_change(ui->table_kernel_Force, row, column, 7);

  if (column == 8) {
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
    cboxArch->setCurrentText(ui->table_kernel_Force->item(row, 8)->text());
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
  QTableWidget* t = new QTableWidget;
  t = ui->table_kernel_Force;
  int row = t->rowCount() + 1;

  t->setRowCount(row);
  t->setItem(row - 1, 0, new QTableWidgetItem(""));
  t->setItem(row - 1, 1, new QTableWidgetItem(""));
  t->setItem(row - 1, 2, new QTableWidgetItem(""));
  t->setItem(row - 1, 3, new QTableWidgetItem(""));
  t->setItem(row - 1, 4, new QTableWidgetItem("Contents/Info.plist"));
  t->setItem(row - 1, 5, new QTableWidgetItem(""));
  t->setItem(row - 1, 6, new QTableWidgetItem(""));
  init_enabled_data(t, row - 1, 7, "false");

  QTableWidgetItem* newItem1 = new QTableWidgetItem("Any");
  newItem1->setTextAlignment(Qt::AlignCenter);
  t->setItem(row - 1, 8, newItem1);

  t->setFocus();
  t->setCurrentCell(row - 1, 0);

  this->setWindowModified(true);
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
  runAdmin(exec, "-mount:*");  //可选参数-Updater

  QString exec2 =
      QCoreApplication::applicationDirPath() + "/Database/win/winfile.exe";

  runAdmin(exec2, NULL);  //此时参数为空

#endif

#ifdef Q_OS_LINUX

#endif

#ifdef Q_OS_MAC

  di = new QProcess;
  di->start("diskutil", QStringList() << "list");
  connect(di, SIGNAL(finished(int)), this, SLOT(readResultDiskInfo()));

#endif
}

QString MainWindow::getDriverInfo(QString strDisk) {
  processDriverInfo = new QProcess;
  processDriverInfo->start("diskutil", QStringList() << "info" << strDisk);
  processDriverInfo->waitForFinished();

  QTextEdit* textEdit = new QTextEdit;
  QTextCodec* gbkCodec = QTextCodec::codecForName("UTF-8");
  QString result = gbkCodec->toUnicode(processDriverInfo->readAll());
  textEdit->append(result);

  QString str0, str1;
  QStringList strList;
  int count = textEdit->document()->lineCount();
  for (int i = 0; i < count; i++) {
    str0 = textEdit->document()->findBlockByNumber(i).text().trimmed();

    if (str0.contains("Media Name")) {
      strList = str0.split(":");

      break;
    }
  }

  if (strList.count() > 0)
    str1 = strList.at(1);
  else
    str1 = "";

  str1 = str1.trimmed();

  return str1;
}

void MainWindow::readResultDiskInfo() {
  ui->textDiskInfo->clear();
  ui->textDiskInfo->setReadOnly(true);
  QTextCodec* gbkCodec = QTextCodec::codecForName("UTF-8");
  QString result = gbkCodec->toUnicode(di->readAll());
  ui->textDiskInfo->append(result);

  dlgMountESP* dlgMESP = new dlgMountESP(this);

  QString str0;
  int count = ui->textDiskInfo->document()->lineCount();
  for (int i = 0; i < count; i++) {
    str0 = ui->textDiskInfo->document()->findBlockByNumber(i).text().trimmed();

    QStringList strList = str0.simplified().split(" ");
    if (strList.count() == 6) {
      if (strList.at(1).toUpper() == "EFI") {
        QString strDisk = strList.at(5).mid(0, 5);
        str0 = str0 + "    " + getDriverInfo(strDisk);

        dlgMESP->ui->listWidget->setIconSize(QSize(32, 32));
        dlgMESP->ui->listWidget->addItem(
            new QListWidgetItem(QIcon(":/icon/esp.png"), str0));
      }
    }
  }

  dlgMESP->setModal(true);
  if (dlgMESP->ui->listWidget->count() > 0)
    dlgMESP->ui->listWidget->setCurrentRow(0);
  dlgMESP->show();
}

void MainWindow::mount_esp_mac(QString strEfiDisk) {
  QString str5 = "diskutil mount " + strEfiDisk;
  QString str_ex = "do shell script " +
                   QString::fromLatin1("\"%1\"").arg(str5) +
                   " with administrator privileges";

  QString fileName = QDir::homePath() + "/.config/QtOCC/qtocc.applescript";
  QFile fi(fileName);
  if (fi.exists()) fi.remove();

  QSaveFile file(fileName);
  QString errorMessage;
  if (file.open(QFile::WriteOnly | QFile::Text)) {
    QTextStream out(&file);
    out << str_ex;
    if (!file.commit()) {
      errorMessage =
          tr("Cannot write file %1:\n%2.")
              .arg(QDir::toNativeSeparators(fileName), file.errorString());
    }
  } else {
    errorMessage =
        tr("Cannot open file %1 for writing:\n%2.")
            .arg(QDir::toNativeSeparators(fileName), file.errorString());
  }

  QProcess* dm = new QProcess;
  dm->execute("osascript", QStringList() << fileName);
}

void MainWindow::closeEvent(QCloseEvent* event) {
  QString qfile = QDir::homePath() + "/.config/QtOCC/QtOCC.ini";
  QFile file(qfile);
  QSettings Reg(qfile, QSettings::IniFormat);
  Reg.setValue("SaveDataHub", ui->chkSaveDataHub->isChecked());

  //搜索历史记录保存
  int textTotal = ui->cboxFind->count();
  Reg.setValue("textTotal", textTotal);
  for (int i = 0; i < textTotal; i++) {
    Reg.setValue(QString::number(i), ui->cboxFind->itemText(i));
  }

  // 窗口位置和大小
  Reg.setValue("x", this->x());
  Reg.setValue("y", this->y());
  Reg.setValue("width", this->width());
  Reg.setValue("height", this->height());

  file.close();

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

        ui->cboxFind->setFocus();
        break;
    }
  } else {
    clear_temp_data();

    event->accept();
  }
}

void MainWindow::on_table_uefi_ReservedMemory_currentCellChanged(
    int currentRow, int currentColumn, int previousRow, int previousColumn) {
  currentCellChanged(ui->table_uefi_ReservedMemory, previousRow, previousColumn,
                     currentRow, currentColumn);
}

void MainWindow::loadLocal() {
  QTextCodec* codec = QTextCodec::codecForName("System");
  QTextCodec::setCodecForLocale(codec);

  static QTranslator translator;  //该对象要一直存在，注意用static
  static QTranslator translator1;
  static QTranslator translator2;

  QLocale locale;
  if (locale.language() == QLocale::English)  //获取系统语言环境
  {
    zh_cn = false;

  } else if (locale.language() == QLocale::Chinese) {
    bool tr = false;
    tr = translator.load(":/cn.qm");
    if (tr) {
      qApp->installTranslator(&translator);
      zh_cn = true;
    }

    bool tr1 = false;
    tr1 = translator1.load(":/qt_zh_CN.qm");
    if (tr1) {
      qApp->installTranslator(&translator1);
      zh_cn = true;
    }

    bool tr2 = false;
    tr2 = translator2.load(":/widgets_zh_cn.qm");
    if (tr2) {
      qApp->installTranslator(&translator2);
      zh_cn = true;
    }

    ui->retranslateUi(this);
  }
}

void MainWindow::on_btnHelp() {
  QFileInfo appInfo(qApp->applicationDirPath());
  QString qtManulFile = appInfo.filePath() + "/Database/doc/Configuration.pdf";

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
  add_item(ui->tableDevices, 8);

  this->setWindowModified(true);
}

void MainWindow::on_btnDevices_del_clicked() { del_item(ui->tableDevices); }

void MainWindow::on_cboxUpdateSMBIOSMode_currentIndexChanged(
    const QString& arg1) {
  if (arg1 == "Custom")
    ui->chkCustomSMBIOSGuid->setChecked(true);  //联动
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
    for (int val : list) {
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
    //设置菜单选项
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
    int row = index.row();  //获得QTableWidget列表点击的行数
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
  //设置菜单选项
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
  //获得鼠标点击的x，y坐标点
  int x = pos.x();
  int y = pos.y();
  QModelIndex index = ui->table_nv_add0->indexAt(QPoint(x, y));
  int row = index.row();  //获得QTableWidget列表点击的行数
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

    // ui->listHardwareInfo->addItem(tr("CPU") + "  :  ");
    ui->listHardwareInfo->addItem(tr("CPU") + "  :  \n" +
                                  getMacInfo("sysctl machdep.cpu"));

    // ui->listHardwareInfo->addItem("");

    // ui->listHardwareInfo->addItem(getMacInfo("system_profiler
    // SPEthernetDataType"));

    // ui->listHardwareInfo->addItem("");

    // ui->listHardwareInfo->addItem(getMacInfo("system_profiler
    // SPAudioDataType"));

    // ui->listHardwareInfo->addItem("");

    // ui->listHardwareInfo->addItem(getMacInfo("system_profiler
    // SPCameraDataType"));

    // ui->listHardwareInfo->addItem("");

    // ui->listHardwareInfo->addItem(getMacInfo("system_profiler
    // SPSerialATADataType"));

    // ui->listHardwareInfo->addItem("");

    // ui->listHardwareInfo->addItem(getMacInfo("system_profiler
    // SPUSBDataType"));
  }

  if (linuxOS) {
    ui->tabTotal->removeTab(8);
  }
}

void MainWindow::setListMainIcon() {
  ui->listMain->setViewMode(QListWidget::IconMode);

  ui->listMain->clear();
  QStringList strIconList, strItemList, strItemList1;
  strIconList << ":/icon/m1.png"
              << ":/icon/m2.png"
              << ":/icon/m3.png"
              << ":/icon/m4.png"
              << ":/icon/m5.png"
              << ":/icon/m6.png"
              << ":/icon/m7.png"
              << ":/icon/m8.png";
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
  ui->listMain->setFixedWidth(68);
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

void MainWindow::init_listMainSub() {
  QString listStyle;
  listStyle =
      "QListWidget::item:selected{background:lightblue; border:0px blue; "
      "color:black}";
  ui->listMain->setStyleSheet(listStyle);
  ui->listSub->setStyleSheet(listStyle);

  QFont myFont(this->font().family(), this->font().pixelSize());
  QFontMetrics fm(myFont);
  int fontHeight = fm.height() + 4;

  if (zh_cn)
    ui->listSub->setFixedHeight(fontHeight * 2);
  else
    ui->listSub->setFixedHeight(fontHeight);

  ui->listMain->setViewMode(QListView::ListMode);
  ui->listSub->setViewMode(QListView::ListMode);
  ui->listMain->setMovement(QListView::Static);  //禁止拖动
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
  ui->textDiskInfo->setVisible(false);
}

void MainWindow::init_FileMenu() {
  // New
  if (mac || osx1012) ui->actionNewWindow->setIconVisibleInMenu(false);
  ui->actionNewWindow->setIcon(QIcon(":/icon/new.png"));
  ui->toolBar->addAction(ui->actionNewWindow);

  // Open
  if (mac || osx1012) ui->actionOpen->setIconVisibleInMenu(false);
  connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::on_btnOpen);
  ui->actionOpen->setShortcut(tr("ctrl+o"));
  ui->actionOpen->setIcon(QIcon(":/icon/open.png"));
  ui->toolBar->addAction(ui->actionOpen);

  //最近打开的文件快捷通道
  QToolButton* btn0 = new QToolButton(this);
  btn0->setToolTip(tr("Open Recent..."));
  btn0->setIcon(QIcon(":/icon/rp.png"));
  btn0->setPopupMode(QToolButton::InstantPopup);
  ui->toolBar->addWidget(btn0);
  reFileMenu = new QMenu(this);
  btn0->setMenu(reFileMenu);

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
  ui->actionSave_As->setIcon(QIcon(":/icon/saveas.png"));
  ui->toolBar->addAction(ui->actionSave_As);

  // Quit
  ui->actionQuit->setMenuRole(QAction::QuitRole);

  ui->toolBar->addSeparator();
}

void MainWindow::init_EditMenu() {
  // Edit
  // OC Validate
  if (mac || osx1012) ui->actionOcvalidate->setIconVisibleInMenu(false);

  ui->actionOcvalidate->setShortcut(tr("ctrl+l"));
  ui->actionOcvalidate->setIcon(QIcon(":/icon/ov.png"));
  ui->toolBar->addAction(ui->actionOcvalidate);

  ui->toolBar->addSeparator();

  // MountESP
  if (mac || osx1012) ui->actionMountEsp->setIconVisibleInMenu(false);

  ui->actionMountEsp->setShortcut(tr("ctrl+m"));
  ui->actionMountEsp->setIcon(QIcon(":/icon/esp.png"));
  ui->toolBar->addAction(ui->actionMountEsp);

  // GenerateEFI
  if (mac || osx1012) ui->actionGenerateEFI->setIconVisibleInMenu(false);

  ui->actionGenerateEFI->setIcon(QIcon(":/icon/efi.png"));
  ui->toolBar->addAction(ui->actionGenerateEFI);

  // Update OC Main Program
  if (mac || osx1012) ui->actionUpgrade_OC->setIconVisibleInMenu(false);

  ui->actionUpgrade_OC->setIcon(QIcon(":/icon/um.png"));
  ui->actionUpgrade_OC->setEnabled(false);
  ui->toolBar->addAction(ui->actionUpgrade_OC);

  // Open DataBase
  if (mac || osx1012) ui->actionDatabase->setIconVisibleInMenu(false);

  ui->actionDatabase->setShortcut(tr("ctrl+d"));
  ui->actionDatabase->setIcon(QIcon(":/icon/db.png"));

  // Open DataBase Dir
  if (mac || osx1012)
    ui->actionOpen_database_directory->setIconVisibleInMenu(false);
  ui->actionOpen_database_directory->setIcon(QIcon(":/icon/opendb.png"));
  connect(ui->actionOpen_database_directory, &QAction::triggered, this,
          &MainWindow::OpenDir_clicked);
}

void MainWindow::init_HelpMenu() {
  // Help
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
  connect(ui->actionOpenCore_Forum, &QAction::triggered, this,
          &MainWindow::on_line3);
  connect(ui->actionOpenCanopyIcons, &QAction::triggered, this,
          &MainWindow::on_line5);

  // OC工厂
  ui->toolBar->addSeparator();
  if (mac || osx1012) ui->actionOpenCore_Factory->setIconVisibleInMenu(false);
  ui->actionOpenCore_Factory->setIcon(QIcon(":/icon/ocf.png"));
  ui->toolBar->addAction(ui->actionOpenCore_Factory);

  //检查更新
  if (mac || osx1012) ui->btnCheckUpdate->setIconVisibleInMenu(false);
  ui->btnCheckUpdate->setIcon(QIcon(":/icon/cu.png"));
  ui->toolBar->addAction(ui->btnCheckUpdate);

  //文档
  if (mac || osx1012) ui->btnHelp->setIconVisibleInMenu(false);
  ui->btnHelp->setIcon(QIcon(":/icon/doc.png"));
  ui->toolBar->addAction(ui->btnHelp);

  // Bug Report
  if (mac || osx1012) ui->actionBug_Report->setIconVisibleInMenu(false);
  ui->actionBug_Report->setIcon(QIcon(":/icon/about.png"));
}

void MainWindow::init_UndoRedo() {
  // Undo/Redo
  undoStack = new QUndoStack(this);

  undoView = new QUndoView(undoStack);
  undoView->setWindowTitle(tr("Command List"));
  // undoView->show();
  undoView->setAttribute(Qt::WA_QuitOnClose, false);

  undoAction = undoStack->createUndoAction(this, tr("Undo"));
  // undoAction->setShortcuts(QKeySequence::Undo);
  if (mac || osx1012) undoAction->setIconVisibleInMenu(false);

  redoAction = undoStack->createRedoAction(this, tr("Redo"));
  // redoAction->setShortcuts(QKeySequence::Redo);
  if (mac || osx1012) redoAction->setIconVisibleInMenu(false);

  ui->menuTools->addSeparator();
  ui->menuTools->addAction(undoAction);
  ui->menuTools->addAction(redoAction);

  // Undo
  undoAction->setShortcut(tr("ctrl+1"));
  undoAction->setIcon(QIcon(":/icon/undo.png"));
  ui->toolBar->addAction(undoAction);

  // Redo
  redoAction->setShortcut(tr("ctrl+2"));
  redoAction->setIcon(QIcon(":/icon/redo.png"));
  ui->toolBar->addAction(redoAction);

  ui->toolBar->addSeparator();
}

void MainWindow::init_MainUI() {
  orgComboBoxStyle = ui->cboxKernelArch->styleSheet();
  orgLineEditStyle = ui->editBID->styleSheet();
  orgLabelStyle = ui->label->styleSheet();
  orgCheckBoxStyle = ui->chkFadtEnableReset->styleSheet();

  int iSize = 25;
  ui->toolBar->setIconSize(QSize(iSize, iSize));

  init_FileMenu();

  init_EditMenu();
  ui->toolBar->addSeparator();
  init_UndoRedo();

  ui->toolBar->addWidget(ui->lblCount);
  ui->toolBar->addWidget(ui->cboxFind);
  ui->toolBar->addAction(ui->actionFind);
  // ui->toolBar->addAction(ui->actionGo_to_the_previous);
  // ui->toolBar->addAction(ui->actionGo_to_the_next);

  ui->toolBar->addSeparator();
  ui->toolBar->addAction(ui->actionDatabase);
  ui->toolBar->addAction(ui->actionOpen_database_directory);

  init_HelpMenu();

  //搜索框

  ui->cboxFind->lineEdit()->setClearButtonEnabled(true);
  ui->cboxFind->lineEdit()->setPlaceholderText(tr("Search"));
  connect(ui->cboxFind->lineEdit(), &QLineEdit::returnPressed, this,
          &MainWindow::on_actionFind_triggered);

  // 清除搜索历史
  clearTextsAction = new QAction(this);
  clearTextsAction->setToolTip(tr("Clear search history"));
  clearTextsAction->setIcon(QIcon(":/icon/clear.png"));
  ui->cboxFind->lineEdit()->addAction(clearTextsAction,
                                      QLineEdit::LeadingPosition);
  connect(clearTextsAction, SIGNAL(triggered()), this, SLOT(clearFindTexts()));

  // 读取搜索历史
  QString qfile = QDir::homePath() + "/.config/QtOCC/QtOCC.ini";
  QSettings Reg(qfile, QSettings::IniFormat);
  int textTotal = Reg.value("textTotal").toInt();
  for (int i = 0; i < textTotal; i++) {
    ui->cboxFind->addItem(Reg.value(QString::number(i)).toString());
  }

  // 读取窗口的位置和大小
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

  ui->cboxFind->setCurrentText("");
  if (textTotal > 0)
    clearTextsAction->setEnabled(true);
  else
    clearTextsAction->setEnabled(false);

  ui->dockWidgetContents->layout()->setMargin(1);
  ui->dockFind->close();

  //查找
  if (mac || osx1012) ui->actionFind->setIconVisibleInMenu(false);
  ui->actionFind->setShortcut(tr("ctrl+f"));
  ui->actionFind->setIcon(QIcon(":/icon/find.png"));

  //转到上一个
  if (mac || osx1012) ui->actionGo_to_the_previous->setIconVisibleInMenu(false);
  ui->actionGo_to_the_previous->setShortcut(tr("ctrl+3"));
  ui->actionGo_to_the_previous->setIcon(QIcon(":/icon/1.png"));

  //转到下一个
  if (mac || osx1012) ui->actionGo_to_the_next->setIconVisibleInMenu(false);
  ui->actionGo_to_the_next->setShortcut(tr("ctrl+4"));
  ui->actionGo_to_the_next->setIcon(QIcon(":/icon/2.png"));

  // StatusBar
  ui->statusbar->addPermanentWidget(ui->btnUpdateHex, 0);

  ui->txtEditHex->setPlaceholderText(tr("Hexadecimal"));
  ui->statusbar->addPermanentWidget(ui->txtEditHex, 0);

  labelConversion = new QLabel(this);
  labelConversion->setText("Hex <==> ASCII");
  ui->statusbar->addPermanentWidget(labelConversion, 0);

  ui->txtEditASCII->setPlaceholderText(tr("ASCII"));
  ui->statusbar->addPermanentWidget(ui->txtEditASCII, 0);

  setConversionWidgetVisible(false);

  CopyLabel();

  CopyCheckbox();

  LineEditDataCheck();

  // Copy list
  copyText(ui->listFind);
  copyText(ui->listMain);
  copyText(ui->listSub);

  init_InitialValue();
  init_TableStyle();
}

void MainWindow::init_TableStyle() {
  listOfTableWidget.clear();
  listOfTableWidget = getAllTableWidget(getAllUIControls(ui->tabTotal));
  for (int i = 0; i < listOfTableWidget.count(); i++) {
    QTableWidget* t;
    t = (QTableWidget*)listOfTableWidget.at(i);
    QString strStyle =
        "QTableWidget::item::selected{color : white;background : rgb(0, 124, "
        "221);}";

    t->setStyleSheet(strStyle);
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
  chk_Target.append(ui->chkT1);
  chk_Target.append(ui->chkT2);
  chk_Target.append(ui->chkT3);
  chk_Target.append(ui->chkT4);
  chk_Target.append(ui->chkT5);
  chk_Target.append(ui->chkT6);
  chk_Target.append(ui->chkT7);
  for (int i = 0; i < chk_Target.count(); i++) {
    connect(chk_Target.at(i), &QCheckBox::clicked, this, &MainWindow::Target);
  }
}

void MainWindow::LineEditDataCheck() {
  // LineEdit data check
  listOfLineEdit.clear();
  listOfLineEdit = getAllLineEdit(getAllUIControls(ui->tabTotal));
  for (int i = 0; i < listOfLineEdit.count(); i++) {
    QLineEdit* w = (QLineEdit*)listOfLineEdit.at(i);

    if (w->objectName().mid(0, 7) == "editInt") {
      QValidator* validator = new QRegExpValidator(regxNumber, w);
      w->setValidator(validator);
      w->setPlaceholderText(tr("Integer"));
    }

    if (w->objectName().mid(0, 7) == "editDat") {
      QValidator* validator = new QRegExpValidator(regx, w);
      w->setValidator(validator);
      w->setPlaceholderText(tr("Hexadecimal"));
    }
  }
}

void MainWindow::CopyCheckbox() {
  // Copy CheckBox
  listOfCheckBox.clear();
  listOfCheckBox = getAllCheckBox(getAllUIControls(ui->tabTotal));
  for (int i = 0; i < listOfCheckBox.count(); i++) {
    QCheckBox* w = (QCheckBox*)listOfCheckBox.at(i);
    setCheckBoxWidth(w);
    w->setContextMenuPolicy(Qt::CustomContextMenu);

    w->installEventFilter(this);

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
      QString str2;
      for (int x = 0; x < str1.count(); x++) {
        str2 = str2 + "=";
      }

      myToolTip->popup(QCursor::pos(), str1 + "\n" + str2 + "\n\n",
                       w->toolTip());
    });

    connect(w, &QCheckBox::customContextMenuRequested, [=](const QPoint& pos) {
      Q_UNUSED(pos);

      if (w->toolTip().trimmed() == "")
        showTipsAction->setVisible(false);
      else
        showTipsAction->setVisible(true);

      copyMenu->exec(QCursor::pos());
    });
  }
}

void MainWindow::CopyLabel() {
  // Copy Label
  listOfLabel.clear();
  listOfLabel = getAllLabel(getAllUIControls(ui->tabTotal));
  for (int i = 0; i < listOfLabel.count(); i++) {
    QLabel* w = (QLabel*)listOfLabel.at(i);
    if (w == ui->lblPickerAttributes || w == ui->lblDisplayLevel ||
        w == ui->lblExposeSensitiveData || w == ui->lblHaltLevel ||
        w == ui->lblScanPolicy || w == ui->lblTargetHex) {
      w->setTextInteractionFlags(Qt::TextSelectableByMouse);

    } else {
      w->setContextMenuPolicy(Qt::CustomContextMenu);

      w->installEventFilter(this);

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
        QString str2;
        for (int x = 0; x < str1.count(); x++) {
          str2 = str2 + "=";
        }

        myToolTip->popup(QCursor::pos(), str1 + "\n" + str2 + "\n\n",
                         w->toolTip());
      });

      connect(w, &QLabel::customContextMenuRequested, [=](const QPoint& pos) {
        Q_UNUSED(pos);

        if (w->toolTip().trimmed() == "")
          showTipsAction->setVisible(false);
        else
          showTipsAction->setVisible(true);

        copyMenu->exec(QCursor::pos());
      });
    }
  }
}

void MainWindow::setCheckBoxWidth(QCheckBox* cbox) {
  QFont myFont(cbox->font().family(), cbox->font().pointSize());
  QString str = cbox->text() + "        ";
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
  ui->cboxFind->clear();
  clearTextsAction->setEnabled(false);
}

QString MainWindow::getDatabaseVer() {
  // Read database version information
  QFileInfo appInfo(qApp->applicationDirPath());
  QString strLastModify =
      QFileInfo(appInfo.filePath() + "/Database/EFI/OC/OpenCore.efi")
          .lastModified()
          .toString();

  QString DatabaseVer = ocVer + "    " + strLastModify;

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

void MainWindow::on_line3() {
  QUrl url(QString(
      "https://www.insanelymac.com/forum/topic/338516-opencore-discussion/"));
  QDesktopServices::openUrl(url);
}

void MainWindow::on_line5() {
  QUrl url(QString("https://github.com/blackosx/OpenCanopyIcons"));
  QDesktopServices::openUrl(url);
}

void MainWindow::on_table_dp_add0_itemChanged(QTableWidgetItem* item) {
  Q_UNUSED(item);

  if (writeINI) {
    write_ini(ui->table_dp_add0, ui->table_dp_add,
              ui->table_dp_add0->currentRow());
    this->setWindowModified(true);
  }
}

void MainWindow::on_table_dp_del0_itemChanged(QTableWidgetItem* item) {
  Q_UNUSED(item);

  if (writeINI) {
    write_value_ini(ui->table_dp_del0, ui->table_dp_del,
                    ui->table_dp_del0->currentRow());
    this->setWindowModified(true);
  }
}

void MainWindow::on_table_nv_add0_itemChanged(QTableWidgetItem* item) {
  Q_UNUSED(item);

  if (writeINI) {
    write_ini(ui->table_nv_add0, ui->table_nv_add,
              ui->table_nv_add0->currentRow());
    this->setWindowModified(true);
  }
}

void MainWindow::on_table_nv_del0_itemChanged(QTableWidgetItem* item) {
  Q_UNUSED(item);

  if (writeINI) {
    write_value_ini(ui->table_nv_del0, ui->table_nv_del,
                    ui->table_nv_del0->currentRow());
    this->setWindowModified(true);
  }
}

void MainWindow::on_table_nv_ls0_itemChanged(QTableWidgetItem* item) {
  Q_UNUSED(item);

  if (writeINI) {
    write_value_ini(ui->table_nv_ls0, ui->table_nv_ls,
                    ui->table_nv_ls0->currentRow());
    this->setWindowModified(true);
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

  ui->statusbar->showMessage(ui->table_nv_del->currentItem()->text());
}

void MainWindow::on_table_nv_ls_cellClicked(int row, int column) {
  Q_UNUSED(row);
  Q_UNUSED(column);

  ui->statusbar->showMessage(ui->table_nv_ls->currentItem()->text());
}

void MainWindow::on_tableDevices_cellClicked(int row, int column) {
  Q_UNUSED(row);
  Q_UNUSED(column);

  ui->statusbar->showMessage(ui->tableDevices->currentItem()->text());
}

void MainWindow::on_table_uefi_drivers_cellClicked(int row, int column) {
  if (!ui->table_uefi_drivers->currentIndex().isValid()) return;

  enabled_change(ui->table_uefi_drivers, row, column, 1);

  setStatusBarText(ui->table_uefi_drivers);
}

void MainWindow::readResultCheckData() {
  QString result =
      QString::fromLocal8Bit(chkdata->readAll());  //与保存文件的格式一致
  QString str;
  QString strMsg;

  if (result.trimmed() == "Failed to read") return;

  if (result.contains("No issues found") || result.contains("No is")) {
    str = tr("OK !");
    strMsg = result + "\n\n" + str;

    ui->actionOcvalidate->setIcon(QIcon(":/icon/ov.png"));
    ui->actionOcvalidate->setToolTip(ui->actionOcvalidate->text());

    dlgOCV->setGoEnabled(false);

  } else {
    strMsg = result;
    dlgOCV->setGoEnabled(true);

    ui->actionOcvalidate->setIcon(QIcon(":/icon/overror.png"));
    ui->actionOcvalidate->setToolTip(
        tr("There is a issue with the configuration file."));
  }

  if (OpenFileValidate && !dlgOCV->isVisible()) {
    OpenFileValidate = false;
    return;
  }

  dlgOCV->setTextOCV(strMsg);
  dlgOCV->setWindowFlags(dlgOCV->windowFlags() | Qt::WindowStaysOnTopHint);
  dlgOCV->show();
}

void MainWindow::on_btnBooterPatchAdd_clicked() {
  add_item(ui->table_Booter_patch, 10);
  init_enabled_data(ui->table_Booter_patch,
                    ui->table_Booter_patch->rowCount() - 1, 9, "true");

  QTableWidgetItem* newItem1 = new QTableWidgetItem("Any");
  newItem1->setTextAlignment(Qt::AlignCenter);
  ui->table_Booter_patch->setItem(ui->table_Booter_patch->currentRow(), 10,
                                  newItem1);

  this->setWindowModified(true);
}

void MainWindow::on_btnBooterPatchDel_clicked() {
  del_item(ui->table_Booter_patch);
}

void MainWindow::on_table_Booter_patch_cellClicked(int row, int column) {
  if (!ui->table_Booter_patch->currentIndex().isValid()) return;

  enabled_change(ui->table_Booter_patch, row, column, 9);

  if (column == 10) {
    cboxArch = new QComboBox;
    cboxArch->setEditable(true);
    cboxArch->addItem("Any");
    cboxArch->addItem("i386");
    cboxArch->addItem("x86_64");
    cboxArch->addItem("");

    connect(cboxArch, SIGNAL(currentTextChanged(QString)), this,
            SLOT(arch_Booter_patchChange()));
    c_row = row;

    ui->table_Booter_patch->setCellWidget(row, column, cboxArch);
    cboxArch->setCurrentText(ui->table_Booter_patch->item(row, 10)->text());
  }

  setStatusBarText(ui->table_Booter_patch);
}

void MainWindow::on_table_Booter_patch_currentCellChanged(int currentRow,
                                                          int currentColumn,
                                                          int previousRow,
                                                          int previousColumn) {
  ui->table_Booter_patch->removeCellWidget(previousRow, 10);

  currentCellChanged(ui->table_Booter_patch, previousRow, previousColumn,
                     currentRow, currentColumn);
}

void MainWindow::on_btnCheckUpdate() {
  QNetworkRequest quest;
  quest.setUrl(QUrl(
      "https://api.github.com/repos/ic005k/QtOpenCoreConfig/releases/latest"));
  quest.setHeader(QNetworkRequest::UserAgentHeader, "RT-Thread ART");
  manager->get(quest);
}

void MainWindow::replyFinished(QNetworkReply* reply) {
  QString str = reply->readAll();
  QMessageBox box;
  box.setText(str);
  // box.exec();
  // qDebug() << QSslSocket::supportsSsl() <<
  // QSslSocket::sslLibraryBuildVersionString() <<
  // QSslSocket::sslLibraryVersionString();

  parse_UpdateJSON(str);

  reply->deleteLater();
}

QString MainWindow::getUrl(QVariantList list) {
  QString macUrl, winUrl, linuxUrl, osx1012Url;
  for (int i = 0; i < list.count(); i++) {
    QVariantMap map = list[i].toMap();
    QString fName = map["name"].toString();
    // qDebug() << map["browser_download_url"].toString();
    if (fName.contains("OCAT_Mac.dmg"))
      macUrl = map["browser_download_url"].toString();

    if (fName.contains("Win")) winUrl = map["browser_download_url"].toString();

    if (fName.contains("Linux"))
      linuxUrl = map["browser_download_url"].toString();

    if (fName.contains("below"))
      osx1012Url = map["browser_download_url"].toString();
  }

  QString Url;
  if (mac) Url = macUrl;
  if (win) Url = winUrl;
  if (linuxOS) Url = linuxUrl;
  if (osx1012) Url = osx1012Url;
  if (Url == "")
    Url = "https://github.com/ic005k/QtOpenCoreConfig/releases/latest";

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

    QString UpdateTime = root_Obj.value("published_at").toString();
    QString ReleaseNote = root_Obj.value("body").toString();
    // QJsonObject PulseValue = root_Obj.value("assets").toObject();
    QString Verison = root_Obj.value("tag_name").toString();

    this->setFocus();

    if (Verison > CurVerison) {
      ui->btnCheckUpdate->setIcon(QIcon(":/icon/newver.png"));
      ui->btnCheckUpdate->setToolTip(tr("There is a new version"));

      if (!autoCheckUpdate) {
        QString warningStr = tr("New version detected!") + "\n" +
                             tr("Version: ") + "V" + Verison + "\n" +
                             tr("Published at: ") + UpdateTime + "\n" +
                             tr("Release Notes: ") + "\n" + ReleaseNote;
        int ret = QMessageBox::warning(this, "", warningStr, tr("Download"),
                                       tr("Cancel"));
        if (ret == 0) {
          // Url = "https://github.com/ic005k/QtOpenCoreConfig/releases/latest";
          // QDesktopServices::openUrl(QUrl(Url));

          ShowAutoUpdateDlg(false);
        }
      }

      autoCheckUpdate = false;

    } else {
      if (!autoCheckUpdate) {
        QMessageBox::information(this, "",
                                 tr("It is currently the latest version!"));
      }

      autoCheckUpdate = false;
    }

    ui->cboxFind->setFocus();
  }
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

//拷贝文件：
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

//拷贝文件夹：
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

void MainWindow::OpenDir_clicked() {
  QFileInfo appInfo(qApp->applicationDirPath());
  QString dirpath = appInfo.filePath() + "/Database/";
  QString dir = "file:" + dirpath;
  QDesktopServices::openUrl(QUrl(dir, QUrl::TolerantMode));
}

void MainWindow::on_listMain_itemSelectionChanged() {
  int cu = ui->listMain->currentRow();
  ui->tabTotal->setCurrentIndex(cu);
  int index;
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
            << tr("BlessOverride") << tr("Entries") << tr("Tools");
  }

  if (ui->listMain->currentRow() == 5) {
    strList << tr("Add") << tr("Delete") << tr("LegacySchema");
  }

  if (ui->listMain->currentRow() == 6) {
    strList << tr("Generic") << tr("DataHub") << tr("Memory")
            << tr("PlatformNVRAM") << tr("SMBIOS");

    if (mac || osx1012) strList.append(tr("SystemInfo"));
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
  }

  if (ui->listMain->currentRow() == 8) {
    index = 0;
    strList << tr("Hardware Information");
  }

  QStringList str;
  int w1, w2;
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
        QSize(w1, ui->listSub->maximumHeight() - 4));
  }

  ui->listSub->setCurrentRow(index);
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
  QProcess p;

  p.start(cmd);
  p.waitForFinished();
  QString result = QString::fromLocal8Bit(p.readAllStandardOutput());
  QStringList list = cmd.split(" ");
  result = result.remove(list.last(), Qt::CaseInsensitive);
  result = result.replace("\r", "");
  result = result.replace("\n", "");
  result = result.simplified();
  return result;
}

QString MainWindow::getCpuName()  //获取cpu名称：wmic cpu get Name
{
  return getWMIC("wmic cpu get name");
}

QString MainWindow::getCpuId()  //查询cpu序列号：wmic cpu get processorid
{
  return getWMIC("wmic cpu get processorid");
}

QString
MainWindow::getCpuCoresNum()  //获取cpu核心数：wmic cpu get NumberOfCores
{
  return getWMIC("wmic cpu get NumberOfCores");
}

QString MainWindow::getCpuLogicalProcessorsNum()  //获取cpu线程数：wmic cpu get
                                                  // NumberOfLogicalProcessors
{
  return getWMIC("wmic cpu get NumberOfLogicalProcessors");
}

QString MainWindow::getDiskNum()  //查看硬盘：wmic diskdrive get serialnumber
{
  return getWMIC("wmic diskdrive where index=0 get serialnumber");
}

QString
MainWindow::getBaseBordNum()  //查询主板序列号：wmic baseboard get serialnumber
{
  return getWMIC("wmic baseboard get serialnumber");
}

QString MainWindow::getBiosNum()  //查询BIOS序列号：wmic bios get serialnumber
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
  //读取ini数据并加载到table_dp_add中

  loadRightTable(ui->table_dp_add0, ui->table_dp_add);
}

void MainWindow::on_table_dp_del0_itemSelectionChanged() {
  loadRightTable(ui->table_dp_del0, ui->table_dp_del);
}

void MainWindow::on_table_nv_add0_itemSelectionChanged() {
  loadRightTable(ui->table_nv_add0, ui->table_nv_add);
}

void MainWindow::on_table_nv_del0_itemSelectionChanged() {
  loadRightTable(ui->table_nv_del0, ui->table_nv_del);
}

void MainWindow::on_table_nv_ls0_itemSelectionChanged() {
  loadRightTable(ui->table_nv_ls0, ui->table_nv_ls);
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
  if (!loading) {
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
        "CountLimitSkipBaseSkipMaxKernelMinKernelAssetTagBankLocatorDeviceLocat"
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
  if (column != 1) {
    myTable = new QTableWidget;
    myTable = ui->table_acpi_add;

    initLineEdit(myTable, row, column, row, column);
  }
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
  myTable = new QTableWidget;
  myTable = ui->table_acpi_del;

  if (column != 3 && column != 4)
    initLineEdit(myTable, row, column, row, column);
}

void MainWindow::on_table_acpi_patch_cellDoubleClicked(int row, int column) {
  myTable = new QTableWidget;
  myTable = ui->table_acpi_patch;

  if (column != 11) initLineEdit(myTable, row, column, row, column);
}

void MainWindow::on_table_booter_cellDoubleClicked(int row, int column) {
  myTable = new QTableWidget;
  myTable = ui->table_booter;

  if (column != 1) initLineEdit(myTable, row, column, row, column);
}

void MainWindow::on_table_Booter_patch_cellDoubleClicked(int row, int column) {
  myTable = new QTableWidget;
  myTable = ui->table_Booter_patch;

  if (column != 9 && column != 10)
    initLineEdit(myTable, row, column, row, column);
}

void MainWindow::on_table_kernel_add_cellDoubleClicked(int row, int column) {
  myTable = new QTableWidget;
  myTable = ui->table_kernel_add;

  if (column != 6 && column != 7)
    initLineEdit(myTable, row, column, row, column);
}

void MainWindow::on_table_kernel_block_cellDoubleClicked(int row, int column) {
  myTable = new QTableWidget;
  myTable = ui->table_kernel_block;

  if (column != 4 && column != 5)
    initLineEdit(myTable, row, column, row, column);
}

void MainWindow::on_table_kernel_Force_cellDoubleClicked(int row, int column) {
  myTable = new QTableWidget;
  myTable = ui->table_kernel_Force;

  if (column != 7 && column != 8)
    initLineEdit(myTable, row, column, row, column);
}

void MainWindow::on_table_kernel_patch_cellDoubleClicked(int row, int column) {
  myTable = new QTableWidget;
  myTable = ui->table_kernel_patch;

  if (column != 12 && column != 13)
    initLineEdit(myTable, row, column, row, column);
}

void MainWindow::on_tableBlessOverride_cellDoubleClicked(int row, int column) {
  myTable = new QTableWidget;
  myTable = ui->tableBlessOverride;

  initLineEdit(myTable, row, column, row, column);
}

void MainWindow::on_tableEntries_cellDoubleClicked(int row, int column) {
  myTable = new QTableWidget;
  myTable = ui->tableEntries;

  if (column != 4 && column != 5 && column != 6)
    initLineEdit(myTable, row, column, row, column);
}

void MainWindow::on_tableTools_cellDoubleClicked(int row, int column) {
  myTable = new QTableWidget;
  myTable = ui->tableTools;

  if (column != 4 && column != 5 && column != 6 && column != 7)
    initLineEdit(myTable, row, column, row, column);
}

void MainWindow::on_table_nv_add0_cellDoubleClicked(int row, int column) {
  myTable = new QTableWidget;
  myTable = ui->table_nv_add0;

  initLineEdit(myTable, row, column, row, column);
}

void MainWindow::on_table_nv_add_cellDoubleClicked(int row, int column) {
  myTable = new QTableWidget;
  myTable = ui->table_nv_add;

  if (column != 1) initLineEdit(myTable, row, column, row, column);
}

void MainWindow::on_table_nv_del0_cellDoubleClicked(int row, int column) {
  myTable = new QTableWidget;
  myTable = ui->table_nv_del0;

  initLineEdit(myTable, row, column, row, column);
}

void MainWindow::on_table_nv_del_cellDoubleClicked(int row, int column) {
  myTable = new QTableWidget;
  myTable = ui->table_nv_del;

  initLineEdit(myTable, row, column, row, column);
}

void MainWindow::on_table_nv_ls0_cellDoubleClicked(int row, int column) {
  myTable = new QTableWidget;
  myTable = ui->table_nv_ls0;

  initLineEdit(myTable, row, column, row, column);
}

void MainWindow::on_table_nv_ls_cellDoubleClicked(int row, int column) {
  myTable = new QTableWidget;
  myTable = ui->table_nv_ls;

  initLineEdit(myTable, row, column, row, column);
}

void MainWindow::on_tableDevices_cellDoubleClicked(int row, int column) {
  myTable = new QTableWidget;
  myTable = ui->tableDevices;

  initLineEdit(myTable, row, column, row, column);
}

void MainWindow::on_table_uefi_drivers_cellDoubleClicked(int row, int column) {
  myTable = new QTableWidget;
  myTable = ui->table_uefi_drivers;

  initLineEdit(myTable, row, column, row, column);
}

void MainWindow::on_table_uefi_ReservedMemory_cellDoubleClicked(int row,
                                                                int column) {
  myTable = new QTableWidget;
  myTable = ui->table_uefi_ReservedMemory;

  if (column != 3 && column != 4)
    initLineEdit(myTable, row, column, row, column);
}

void MainWindow::on_table_dp_del_cellDoubleClicked(int row, int column) {
  myTable = new QTableWidget;
  myTable = ui->table_dp_del;

  initLineEdit(myTable, row, column, row, column);
}

void MainWindow::on_actionNewWindow_triggered() {
  QFileInfo appInfo(qApp->applicationDirPath());
  QString pathSource = appInfo.filePath() + "/OCAuxiliaryTools";
  QStringList arguments;
  QString fn = "";
  arguments << fn;
  QProcess* process = new QProcess;
  process->setEnvironment(process->environment());
  process->start(pathSource, arguments);
}

/* 获取所有控件 */
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

// 获取所有的chkbox
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

QObjectList MainWindow::getAllComboBox(QObjectList lstUIControls) {
  QObjectList lstOfComboBox;
  foreach (QObject* obj, lstUIControls) {
    if (obj->metaObject()->className() == QStringLiteral("QComboBox")) {
      lstOfComboBox.append(obj);
    }
  }
  return lstOfComboBox;
}

void MainWindow::findCheckBox(QString findText) {
  // CheckBox  1
  listOfCheckBox.clear();
  listOfCheckBox = getAllCheckBox(getAllUIControls(ui->tabTotal));

  listOfCheckBoxResults.clear();

  for (int i = 0; i < listOfCheckBox.count(); i++) {
    QCheckBox* chkbox = (QCheckBox*)listOfCheckBox.at(i);
    if (chkbox->text().toLower().contains(findText.trimmed().toLower())) {
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
      findCount++;
      listOfLabelResults.append(lbl);
      listNameResults.append("3" + lbl->objectName());

      ui->listFind->addItem(lbl->text());
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
      if (edit->text().toLower().contains(findText.trimmed().toLower())) {
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
    if (cbox != ui->cboxFind && cbox != ui->cboxTextColor &&
        cbox != ui->cboxBackColor) {
      if (cbox->currentText().toLower().contains(
              findText.trimmed().toLower())) {
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
  find = true;

  if (!FindTextChange) {
    on_actionGo_to_the_next_triggered();

    return;
  }

  bool curWinModi = this->isWindowModified();

  QString findText = ui->cboxFind->currentText().trimmed().toLower();

  if (findText == "") return;

  if (ui->cboxFind->count() > 0) clearTextsAction->setEnabled(true);

  findCount = 0;
  listNameResults.clear();
  loading = true;
  ui->listFind->clear();
  loading = false;
  indexOfResults = -1;

  //清理标记
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

  findCheckBox(findText);

  mymethod->findTable(findText);

  findLabel(findText);

  findLineEdit(findText);

  findComboBox(findText);

  findTableHeader(findText);

  findTabText(findText);

  ui->lblCount->setText(QString::number(findCount));

  if (listNameResults.count() > 0) {
    ui->dockFind->show();

    ui->listFind->setCurrentRow(0);

    int index = ui->listMain->currentRow();
    setListMainIcon();
    ui->listMain->setCurrentRow(index);

    // 刷新搜索历史
    QStringList strList;
    for (int i = 0; i < ui->cboxFind->count(); i++) {
      strList.append(ui->cboxFind->itemText(i));
    }

    for (int i = 0; i < strList.count(); i++) {
      if (findText == strList.at(i)) {
        strList.removeAt(i);
      }
    }

    strList.insert(0, findText);
    AddCboxFindItem = true;
    ui->cboxFind->clear();
    ui->cboxFind->addItems(strList);
    AddCboxFindItem = false;

    if (red < 55) {
      setPalette(ui->cboxFind, QColor(50, 50, 50), Qt::white);

    } else {
      setPalette(ui->cboxFind, Qt::white, Qt::black);
    }

    FindTextChange = false;

  } else {
    setPalette(ui->cboxFind, QColor(255, 70, 70), Qt::white);
  }

  this->setWindowModified(curWinModi);

  find = false;
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

        //命名规则：当前位置+对象名称
        QString name =
            QString::number(listNameResults.count() - 1) + t->objectName();

        QString plistPath = QDir::homePath() + "/.config/QtOCC/" +
                            CurrentDateTime + name + ".ini";
        // qDebug() << plistPath;

        QFile file(plistPath);
        if (file.exists())  //如果文件存在，则先删除它
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
        currentTabWidget = mymethod->getSubTabWidget(i, j);
        listOfCheckBox.clear();
        listOfCheckBox = getAllCheckBox(getAllUIControls(currentTabWidget));
        for (int k = 0; k < listOfCheckBox.count(); k++) {
          if (listOfCheckBox.at(k)->objectName() == name) {
            orgCheckBoxStyle = ui->chkFadtEnableReset->styleSheet();
            QString style =
                "QCheckBox{background-color:rgb(255,0,0);color:rgb(255,255,255)"
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
          if (listOfCheckBox.at(k)->objectName() == name) {
            QString style =
                "QCheckBox{background-color:rgb(255,0,0);color:rgb(255,255,255)"
                ";}";
            QCheckBox* w = (QCheckBox*)listOfCheckBox.at(k);
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
        currentTabWidget = mymethod->getSubTabWidget(i, j);
        listOfTableWidget.clear();
        listOfTableWidget =
            getAllTableWidget(getAllUIControls(currentTabWidget));
        for (int k = 0; k < listOfTableWidget.count(); k++) {
          if (listOfTableWidget.at(k)->objectName() == name) {
            QString nameINI = QString::number(index) + name;

            QString plistPath = QDir::homePath() + "/.config/QtOCC/" +
                                CurrentDateTime + nameINI + ".ini";

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
        currentTabWidget = mymethod->getSubTabWidget(i, j);
        listOfLabel.clear();
        listOfLabel = getAllLabel(getAllUIControls(currentTabWidget));
        for (int k = 0; k < listOfLabel.count(); k++) {
          if (listOfLabel.at(k)->objectName() == name) {
            orgLabelStyle = ui->label->styleSheet();
            QString style =
                "QLabel{background-color:rgb(255,0,0);color:rgb(255,255,255);}";
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
        currentTabWidget = mymethod->getSubTabWidget(i, j);
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
        currentTabWidget = mymethod->getSubTabWidget(i, j);
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
        currentTabWidget = mymethod->getSubTabWidget(i, j);
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

  //获取背景色
  QPalette pal = this->palette();
  QBrush brush = pal.window();
  red = brush.color().red();

  //清理之前的标记
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

void MainWindow::on_cboxFind_currentTextChanged(const QString& arg1) {
  if (AddCboxFindItem) {
    return;
  }

  FindTextChange = true;

  if (arg1.trimmed() == "") {
    ui->lblCount->setText("0");
    listNameResults.clear();

    loading = true;
    ui->listFind->clear();
    loading = false;

    clearCheckBoxMarker();
    clearComboBoxMarker();
    clearLabelMarker();
    clearLineEditMarker();
    clearTableHeaderMarker();

    //获取背景色
    QPalette pal = this->palette();
    QBrush brush = pal.window();
    red = brush.color().red();

    if (red < 55) {
      setPalette(ui->cboxFind, QColor(50, 50, 50), Qt::white);

    } else {
      setPalette(ui->cboxFind, Qt::white, Qt::black);
    }
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
  if (t->hasFocus()) {
    row = t->currentRow();
    col = t->currentColumn();
    on_table_dp_add0_cellDoubleClicked(row, col);
  }

  t = ui->table_dp_add;
  if (t->hasFocus()) {
    row = t->currentRow();
    col = t->currentColumn();
    on_table_dp_add_cellDoubleClicked(row, col);
  }

  t = ui->table_dp_del0;
  if (t->hasFocus()) {
    row = t->currentRow();
    col = t->currentColumn();
    on_table_dp_del0_cellDoubleClicked(row, col);
  }

  t = ui->table_dp_del;
  if (t->hasFocus()) {
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
  if (t->hasFocus()) {
    row = t->currentRow();
    col = t->currentColumn();
    on_table_nv_add0_cellDoubleClicked(row, col);
  }

  t = ui->table_nv_add;
  if (t->hasFocus()) {
    row = t->currentRow();
    col = t->currentColumn();
    on_table_nv_add_cellDoubleClicked(row, col);
  }

  t = ui->table_nv_del0;
  if (t->hasFocus()) {
    row = t->currentRow();
    col = t->currentColumn();
    on_table_nv_del0_cellDoubleClicked(row, col);
  }

  t = ui->table_nv_del;
  if (t->hasFocus()) {
    row = t->currentRow();
    col = t->currentColumn();
    on_table_nv_del_cellDoubleClicked(row, col);
  }

  t = ui->table_nv_ls0;
  if (t->hasFocus()) {
    row = t->currentRow();
    col = t->currentColumn();
    on_table_nv_ls0_cellDoubleClicked(row, col);
  }

  t = ui->table_nv_ls;
  if (t->hasFocus()) {
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
  if (!LoadRightTable) this->setWindowModified(true);
}

void MainWindow::setWM() { this->setWindowModified(true); }

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
    QString qfile = QDir::homePath() + "/.config/QtOCC/" + name + ".ini";
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
    QString qfile = QDir::homePath() + "/.config/QtOCC/" + name + ".ini";
    QFile file(qfile);

    QItemSelectionModel* selections = w->selectionModel();  //返回当前的选择模式
    QModelIndexList selectedsList =
        selections->selectedIndexes();  //返回所有选定的模型项目索引列表

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
    }
    file.close();
  });
}

void MainWindow::cutLine(QTableWidget* w, QAction* cutAction,
                         QAction* copyAction) {
  connect(cutAction, &QAction::triggered, [=]() {
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
  });
}

void MainWindow::setPopMenuEnabled(QString qfile, QTableWidget* w,
                                   QAction* cutAction, QAction* pasteAction,
                                   QAction* copyAction) {
  if (w == ui->table_dp_add0 || w == ui->table_dp_del0 ||
      w == ui->table_nv_add0 || w == ui->table_nv_del0 ||
      w == ui->table_nv_ls0) {
    QString dirpath = QDir::homePath() + "/.config/QtOCC/";

    QSettings Reg(qfile, QSettings::IniFormat);
    QString text = Reg.value("0/col" + QString::number(0)).toString().trimmed();

    text = text.replace("/", "-");
    QString oldRightTable = Reg.value("CurrentDateTime").toString() +
                            w->objectName() + text + ".ini";
    QFileInfo fi(dirpath + oldRightTable);

    if (!fi.exists())
      pasteAction->setEnabled(false);
    else {
      copyAction->setEnabled(true);
      cutAction->setEnabled(true);
    }
  }
}

void MainWindow::setPopMenuEnabled(QTableWidget* w, QAction* pasteAction) {
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

    QString qfile = QDir::homePath() + "/.config/QtOCC/" + name + ".ini";
    QFile file(qfile);
    if (file.exists()) {
      QSettings Reg(qfile, QSettings::IniFormat);

      pasteAction->setEnabled(true);

      setPopMenuEnabled(qfile, w, cutAction, pasteAction, copyAction);

      setPopMenuEnabled(w, pasteAction);

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

    if (ui->listMain->currentRow() == 0 && ui->listSub->currentRow() == 0 &&
        ui->checkACPIAdd->isChecked()) {
      copyAction->setEnabled(false);
      cutAction->setEnabled(false);
      pasteAction->setEnabled(false);
    }

    if (ui->listMain->currentRow() == 3 && ui->listSub->currentRow() == 0 &&
        ui->checkKernelAdd->isChecked()) {
      copyAction->setEnabled(false);
      cutAction->setEnabled(false);
      pasteAction->setEnabled(false);
    }

    popMenu->exec(QCursor::pos());
  });
}

void MainWindow::init_CopyPasteLine() {
  listOfTableWidget.clear();
  listOfTableWidget = getAllTableWidget(getAllUIControls(ui->tabTotal));
  for (int i = 0; i < listOfTableWidget.count(); i++) {
    QTableWidget* w = (QTableWidget*)listOfTableWidget.at(i);
    for (int y = 0; y < w->columnCount(); y++) {
      QString item = w->horizontalHeaderItem(y)->text();
      if (item != tr("Enabled") && item != tr("Arch") && item != tr("All") &&
          item != tr("Type") && item != tr("TextMode") &&
          item != tr("Auxiliary") && item != tr("RealPath") &&
          item != tr("Class"))
        w->horizontalHeader()->setSectionResizeMode(
            y, QHeaderView::ResizeToContents);
    }
    // w->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);

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

    QAction* cutAction = new QAction(tr("Cut Line"));
    QAction* copyAction = new QAction(tr("Copy Line"));
    QAction* pasteAction = new QAction(tr("Paste Line"));
    QAction* showtipAction = new QAction(tr("Show Tips"));
    QMenu* popMenu = new QMenu(this);

    popMenu->addAction(cutAction);
    popMenu->addAction(copyAction);
    popMenu->addAction(pasteAction);
    popMenu->addSeparator();
    popMenu->addAction(showtipAction);

    // 显示提示
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

  if (!loading) {
    loading = true;
    LoadRightTable = true;
    read_ini(t0, t, t0->currentRow());
    LoadRightTable = false;
    loading = false;
    ui->statusbar->showMessage(t0->currentItem()->text());
  }
}

void MainWindow::endPasteLine(QTableWidget* w, int row, QString colText0) {
  w->setCurrentCell(row, 0);

  if (w == ui->table_dp_add0 || w == ui->table_dp_del0 ||
      w == ui->table_nv_add0 || w == ui->table_nv_del0 ||
      w == ui->table_nv_ls0) {
    QString name = w->objectName();
    QString qfile = QDir::homePath() + "/.config/QtOCC/" + name + ".ini";
    QSettings Reg(qfile, QSettings::IniFormat);

    QString dirpath = QDir::homePath() + "/.config/QtOCC/";

    colText0 = colText0.replace("/", "-");
    QString oldRightTable = Reg.value("CurrentDateTime").toString() +
                            w->objectName() + colText0 + ".ini";

    QString newText = w->item(row, 0)->text().trimmed();
    newText = newText.replace("/", "-");
    QString newReghtTable =
        CurrentDateTime + w->objectName() + newText + ".ini";

    QFileInfo fi(dirpath + oldRightTable);
    if (fi.exists()) {
      QFile::copy(dirpath + oldRightTable, dirpath + newReghtTable);
      IniFile.push_back(dirpath + newReghtTable);
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
  }
}

void MainWindow::endDelLeftTable(QTableWidget* t0) {
  QTableWidget* t;

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

  //获取背景色
  QPalette pal = this->palette();
  QBrush brush = pal.window();
  int c_red = brush.color().red();

  if (c_red != red) {
    red = c_red;

    if (red < 55) {
      setPalette(ui->cboxFind, QColor(50, 50, 50), Qt::white);

    } else {
      setPalette(ui->cboxFind, Qt::white, Qt::black);
    }
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

      return true;  //不让事件继续传播
    }

    return false;
  }

  return MainWindow::eventFilter(obj, event);
}

void MainWindow::on_actionBug_Report_triggered() {
  QUrl url(QString("https://github.com/ic005k/QtOpenCoreConfig/issues"));
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

    bool t = false;
    for (int j = 0; j < chk_Target.count(); j++) {
      if (chkbox == chk_Target.at(j)) t = true;
    }
    if (chkbox->text().mid(0, 3) != "OC_" && chkbox->text().mid(0, 5) != "DEBUG"

        && !t)

      chkbox->setChecked(map[name].toBool());
  }
}

void MainWindow::getComboBoxValue(QVariantMap map, QWidget* tab) {
  QObjectList listComboBox;
  listComboBox = getAllComboBox(getAllUIControls(tab));
  for (int i = 0; i < listComboBox.count(); i++) {
    QComboBox* w = (QComboBox*)listComboBox.at(i);
    QString name = w->objectName().mid(4, w->objectName().count() - 3);

    QString cu_text = map[name].toString().trimmed();
    if (w != ui->cboxFind) {
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
          w->setText(ByteToHexStr(map[name].toByteArray()));  // 为data类型
      } else {
        if (strList.count() > 0)
          w->setText(map[strList.at(0)].toString());
        else {
          w->setText(map[name].toString());
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
  if (name != "" && w != ui->editPassInput && name != "pinbox_lineedit")
    return true;

  return false;
}

QVariantMap MainWindow::setEditValue(QVariantMap map, QWidget* tab) {
  // edit
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

    if (editExclusion(w, name)) {
      QStringList strList = name.split("_");

      if (str0.mid(0, 3) == "Dat") {
        if (strList.count() > 0)
          map.insert(strList.at(0), HexStrToByte(w->text().trimmed()));
        else
          map.insert(name, HexStrToByte(w->text().trimmed()));
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

  return map;
}

bool MainWindow::ExclusionCheckBox(QCheckBox* chkbox) {
  if (chkbox->text().trimmed().mid(0, 3) != "OC_" &&
      chkbox->text().trimmed().mid(0, 5) != "DEBUG"

      && chkbox != ui->chkT1 && chkbox != ui->chkT2 && chkbox != ui->chkT3 &&
      chkbox != ui->chkT4 && chkbox != ui->chkT5 && chkbox != ui->chkT6 &&
      chkbox != ui->chkT7)

    return true;

  return false;
}

QVariantMap MainWindow::setCheckBoxValue(QVariantMap map, QWidget* tab) {
  // chk
  QObjectList listCheckBox;
  listCheckBox = getAllCheckBox(getAllUIControls(tab));
  for (int i = 0; i < listCheckBox.count(); i++) {
    QCheckBox* chkbox = (QCheckBox*)listCheckBox.at(i);
    QString strObjName = chkbox->objectName();
    QString name = strObjName.mid(3, strObjName.count() - 2);

    if (ExclusionCheckBox(chkbox)) {
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
    QString name = w->objectName().mid(4, w->objectName().count() - 3);

    if (w != ui->cboxFind && w != ui->cboxTextColor && w != ui->cboxBackColor) {
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
  QStringList sourceFiles, targetFiles;

  QString DirName;
  QMessageBox box;

  QFileInfo fi(SaveFileName);
  DirName = fi.path().mid(0, fi.path().count() - 3);

  if (DirName.isEmpty()) return;

  QFileInfo appInfo(qApp->applicationDirPath());
  QString pathSource = appInfo.filePath() + "/Database/";

  QString file1, file2, file3, file4;
  QString targetFile1, targetFile2, targetFile3, targetFile4;
  file1 = pathSource + "EFI/OC/OpenCore.efi";
  file2 = pathSource + "EFI/BOOT/BOOTx64.efi";
  file3 = pathSource + "EFI/OC/Drivers/OpenRuntime.efi";
  file4 = pathSource + "EFI/OC/Drivers/OpenCanopy.efi";
  sourceFiles.append(file1);
  sourceFiles.append(file2);
  sourceFiles.append(file3);
  sourceFiles.append(file4);

  targetFile1 = DirName + "/OC/OpenCore.efi";
  targetFile2 = DirName + "/BOOT/BOOTx64.efi";
  targetFile3 = DirName + "/OC/Drivers/OpenRuntime.efi";
  targetFile4 = DirName + "/OC/Drivers/OpenCanopy.efi";
  targetFiles.append(targetFile1);
  targetFiles.append(targetFile2);
  targetFiles.append(targetFile3);
  targetFiles.append(targetFile4);

  // Drivers
  for (int i = 0; i < ui->table_uefi_drivers->rowCount(); i++) {
    QString str1 = ui->table_uefi_drivers->item(i, 0)->text();
    QString str2 = pathSource + "EFI/OC/Drivers/" + str1;
    bool re = false;
    for (int j = 0; j < sourceFiles.count(); j++) {
      if (sourceFiles.at(j) == str2) re = true;
    }
    if (!re) {
      sourceFiles.append(str2);
      targetFiles.append(DirName + "/OC/Drivers/" + str1);
    }
  }

  QFileInfo f1(file1);
  QFileInfo f2(file2);
  QFileInfo f3(file3);
  QFileInfo f4(file4);

  this->setFocus();
  if (!f1.exists() || !f2.exists() || !f3.exists() || !f4.exists()) {
    box.setText(
        tr("The database file is incomplete and the upgrade cannot be "
           "completed."));
    box.exec();
    ui->cboxFind->setFocus();
    return;
  }

  dlgSyncOC->ui->listSource->clear();
  dlgSyncOC->ui->listTarget->clear();
  dlgSyncOC->ui->listSource->addItems(sourceFiles);
  dlgSyncOC->ui->listTarget->addItems(targetFiles);
  dlgSyncOC->setModal(true);
  dlgSyncOC->show();
  for (int i = 0; i < dlgSyncOC->ui->listSource->count(); i++) {
    dlgSyncOC->ui->listSource->setCurrentRow(i);
  }
  dlgSyncOC->ui->listSource->setCurrentRow(0);
  // Resources
  dlgSyncOC->sourceResourcesDir = pathSource + "EFI/OC/Resources/";
  dlgSyncOC->targetResourcesDir = DirName + "/OC/Resources/";
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

void MainWindow::on_cboxTextColor_currentIndexChanged(int index) {
  Q_UNUSED(index);

  initColorValue();

  int bcIndex = ui->cboxBackColor->currentIndex();
  int tcIndex = ui->cboxTextColor->currentIndex();
  int total = backColorInt.at(bcIndex) + textColorInt.at(tcIndex);

  ui->editIntConsoleAttributes->setText(QString::number(total));

  if (bcIndex >= 0 && tcIndex >= 0 && !Initialization) {
    QPalette pe;
    pe = ui->lblColorEffect->palette();
    pe.setColor(QPalette::Background, QColor(backColor.at(bcIndex)));
    ui->lblColorEffect->setAutoFillBackground(true);
    if (total != 0)
      pe.setColor(QPalette::WindowText, QColor(textColor.at(tcIndex)));
    else
      pe.setColor(QPalette::WindowText, Qt::white);
    ui->lblColorEffect->setPalette(pe);
  }
}

void MainWindow::on_cboxBackColor_currentIndexChanged(int index) {
  on_cboxTextColor_currentIndexChanged(index);
}

void MainWindow::on_editIntConsoleAttributes_textChanged(const QString& arg1) {
  int total = arg1.toInt();

  initColorValue();

  for (int i = 0; i < textColorInt.count(); i++) {
    for (int j = 0; j < backColorInt.count(); j++) {
      if (total == textColorInt.at(i) + backColorInt.at(j)) {
        ui->cboxTextColor->setCurrentIndex(i);
        ui->cboxBackColor->setCurrentIndex(j);
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
  ui->btnGetPassHash->setEnabled(false);
  ui->progressBar->setMaximum(0);
  this->repaint();

  QFileInfo appInfo(qApp->applicationDirPath());
  QString strPass = "";
  chkdata = new QProcess;

#ifdef Q_OS_WIN32
  chkdata->start(appInfo.filePath() + "/Database/win/ocpasswordgen.exe",
                 QStringList() << strPass);

#endif

#ifdef Q_OS_LINUX
  chkdata->start(appInfo.filePath() + "/Database/linux/ocpasswordgen",
                 QStringList() << strPass);

#endif

#ifdef Q_OS_MAC

  chkdata->start(appInfo.filePath() + "/Database/mac/ocpasswordgen",
                 QStringList() << strPass);
#endif

  chkdata->waitForStarted();  //等待启动完成
  QString strData = ui->editPassInput->text().trimmed() + "\n";
  const char* cstr;  // = strData.toLocal8Bit().constData();
  strData = strData.toLocal8Bit();
  string strStd = strData.toStdString();
  cstr = strStd.c_str();

  chkdata->write(cstr);

  connect(chkdata, SIGNAL(finished(int)), this, SLOT(readResultPassHash()));
}

void MainWindow::readResultPassHash() {
  QString result = chkdata->readAll();

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

void MainWindow::on_editPassInput_textChanged(const QString& arg1) {
  if (ui->progressBar->maximum() == 0) return;

  if (arg1.trimmed().count() > 0) {
    ui->btnGetPassHash->setEnabled(true);
    this->repaint();
  } else {
    ui->btnGetPassHash->setEnabled(false);
    this->repaint();
  }
}

void MainWindow::on_editPassInput_returnPressed() {
  if (ui->btnGetPassHash->isEnabled()) on_btnGetPassHash_clicked();
}

void MainWindow::on_actionDatabase_triggered() {
  myDatabase->setModal(true);
  myDatabase->show();

  QFileInfo appInfo(qApp->applicationDirPath());

  QString dirpath = appInfo.filePath() + "/Database/";
  //设置要遍历的目录
  QDir dir(dirpath);
  //设置文件过滤器
  QStringList nameFilters;
  //设置文件过滤格式
  nameFilters << "*.plist";
  //将过滤后的文件名称存入到files列表中
  QStringList filesTemp =
      dir.entryList(nameFilters, QDir::Files | QDir::Readable, QDir::Name);
  QStringList files;
  for (int j = 0; j < filesTemp.count(); j++) {
    if (filesTemp.at(j).mid(0, 1) != ".") files.append(filesTemp.at(j));
  }

  tableDatabase->setRowCount(0);
  tableDatabase->setRowCount(files.count());
  for (int i = 0; i < files.count(); i++) {
    QTableWidgetItem* newItem1;
    newItem1 = new QTableWidgetItem(files.at(i));
    tableDatabase->setItem(i, 0, newItem1);
  }

  myDatabase->setWindowTitle(tr("Configuration file database") + " : " +
                             getDatabaseVer());
}

void MainWindow::on_actionOcvalidate_triggered() {
  QFileInfo appInfo(qApp->applicationDirPath());
  chkdata = new QProcess;

#ifdef Q_OS_WIN32
  chkdata->start(appInfo.filePath() + "/Database/win/ocvalidate.exe",
                 QStringList() << SaveFileName);
#endif

#ifdef Q_OS_LINUX
  chkdata->start(appInfo.filePath() + "/Database/linux/ocvalidate",
                 QStringList() << SaveFileName);
#endif

#ifdef Q_OS_MAC
  chkdata->start(appInfo.filePath() + "/Database/mac/ocvalidate",
                 QStringList() << SaveFileName);
#endif

  connect(chkdata, SIGNAL(finished(int)), this, SLOT(readResultCheckData()));
}

void MainWindow::on_actionMountEsp_triggered() { mount_esp(); }

void MainWindow::on_actionGenerateEFI_triggered() {
  mymethod->on_GenerateEFI();
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
  QFileInfo appInfo(qApp->applicationDirPath());
  QString dirpath = appInfo.filePath() + strDir;
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
  QUrl url(QString("https://github.com/ic005k/PlistEDPlus"));
  QDesktopServices::openUrl(url);
}

void MainWindow::on_actionDSDT_SSDT_editor_triggered() {
  QUrl url(QString("https://github.com/ic005k/QtiASL"));
  QDesktopServices::openUrl(url);
}

void MainWindow::on_actionDifferences_triggered() {
  QFileInfo appInfo(qApp->applicationDirPath());
  QString qtManulFile = appInfo.filePath() + "/Database/doc/Differences.pdf";

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

void MainWindow::on_checkACPIAdd_stateChanged(int arg1) {
  CheckChange(ui->table_acpi_add, arg1, ui->btnACPIAdd_Del);
}

void MainWindow::on_checkKernelAdd_stateChanged(int arg1) {
  CheckChange(ui->table_kernel_add, arg1, ui->btnKernelAdd_Del);
}

void MainWindow::on_btnUp_UEFI_Drivers_clicked() {
  MoveItem(ui->table_uefi_drivers, true);
}

void MainWindow::on_btnDown_UEFI_Drivers_clicked() {
  MoveItem(ui->table_uefi_drivers, false);
}

void MainWindow::on_actionLatest_Release_triggered() {
  QUrl url(
      QString("https://github.com/ic005k/QtOpenCoreConfig/releases/latest"));
  QDesktopServices::openUrl(url);
}

void MainWindow::on_actionOnline_Download_Updates_triggered() {
  ShowAutoUpdateDlg(false);
}

void MainWindow::on_actionUpgrade_Database_triggered() {
  if (mac || win || osx1012) {
    ShowAutoUpdateDlg(true);
  }
}

void MainWindow::ShowAutoUpdateDlg(bool Database) {
  if (dlgAutoUpdate->isVisible()) return;

  dlgAutoUpdate->setWindowFlags(dlgAutoUpdate->windowFlags() |
                                Qt::WindowStaysOnTopHint);
  dlgAutoUpdate->show();
  dlgAutoUpdate->startDownload(Database);
}

void MainWindow::on_txtEditHex_textChanged(const QString& arg1) {
  if (txtEditASCIITextChanged) {
    txtEditASCIITextChanged = false;
    return;
  }

  txtEditHexTextChanged = true;
  QString str0, str;
  str0 = arg1;
  str = str0.remove(QRegExp("\\s"));  // 16进制去除所有空格

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
  if (find) return;
  Q_UNUSED(currentRow);
  setConversionWidgetVisible(false);
  mymethod->UpdateStatusBarInfo();
}

void MainWindow::on_listMain_currentRowChanged(int currentRow) {
  if (find) return;
  Q_UNUSED(currentRow);
  setConversionWidgetVisible(false);
  mymethod->UpdateStatusBarInfo();
}

void MainWindow::setConversionWidgetVisible(bool v) {
  if (!v) {
    ui->txtEditHex->setVisible(false);
    labelConversion->setVisible(false);
    ui->txtEditASCII->setVisible(false);
    ui->btnUpdateHex->setVisible(false);
  } else {
    ui->txtEditHex->setVisible(true);
    labelConversion->setVisible(true);
    ui->txtEditASCII->setVisible(true);
    ui->btnUpdateHex->setVisible(true);
  }
}

void MainWindow::on_btnUpdateHex_triggered(QAction* arg1) { Q_UNUSED(arg1); }

void MainWindow::on_btnUpdateHex_clicked() {
  if (ui->txtEditHex->text().length() % 2 != 0) return;

  QObjectList listTable;
  QTableWidget* t;
  bool txtHexFocus = ui->txtEditHex->hasFocus();
  bool txtASCIIFocus = ui->txtEditASCII->hasFocus();

  QWidget* w = mymethod->getSubTabWidget(ui->listMain->currentRow(),
                                         ui->listSub->currentRow());
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

  lineEdit->setText(
      ui->txtEditHex->text().remove(QRegExp("\\s")));  // 16进制去除所有空格);

  QCoreApplication::sendEvent(lineEdit, tabKey);

  if (txtHexFocus) ui->txtEditHex->setFocus();
  if (txtASCIIFocus) ui->txtEditASCII->setFocus();
}
