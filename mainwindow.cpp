#include "mainwindow.h"
#include "plistparser.h"
#include "plistserializer.h"
#include "ui_mainwindow.h"

#include <QBuffer>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>

QString PlistFileName;
QVector<QString> filelist;
QWidgetList wdlist;
QTableWidget* tableDatabase;

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    loadLocal();

    test(false);
    CurVerison = "20210118";
    title = "QtOpenCoreConfigurator   V0.6.6-" + CurVerison + "        [*] ";
    setWindowTitle(title);

    aboutDlg = new aboutDialog(this);
    myDatabase = new dlgDatabase(this);

    QDir dir;
    if (dir.mkpath(QDir::homePath() + "/.config/QtOCC/")) { }

    ui->tabTotal->setCurrentIndex(0);
    ui->tabACPI->setCurrentIndex(0);
    ui->tabBooter->setCurrentIndex(0);
    ui->tabDP->setCurrentIndex(0);
    ui->tabKernel->setCurrentIndex(0);
    ui->tabMisc->setCurrentIndex(0);
    ui->tabNVRAM->setCurrentIndex(0);
    ui->tabPlatformInfo->setCurrentIndex(0);
    ui->tabUEFI->setCurrentIndex(0);

    //ui->tabTotal->tabBar()->setTabToolTip(0, tr("ACPI"));
    //ui->tabTotal->tabBar()->setTabToolTip(1, tr("Booter"));
    //ui->tabTotal->tabBar()->setTabToolTip(2, tr("DeviceProperties"));
    //ui->tabTotal->tabBar()->setTabToolTip(3, tr("Kernel"));
    //ui->tabTotal->tabBar()->setTabToolTip(4, tr("Misc"));
    //ui->tabTotal->tabBar()->setTabToolTip(5, tr("NVRAM"));
    //ui->tabTotal->tabBar()->setTabToolTip(6, tr("PlatformInfo"));
    //ui->tabTotal->tabBar()->setTabToolTip(7, tr("UEFI"));

    QString tabBarStyle0 = "QTabBar::tab {min-width:100px;border: 1px solid;border-top-left-radius: 5px;border-top-right-radius: 5px;padding:2px;}\
            QTabBar::tab:!selected {margin-top: 2px;}\
            QTabWidget::pane{border:none;}\
            QTabBar::tab{background:transparent;color:black;}\
            QTabBar::tab:hover{background:rgba(0, 0, 255, 80);}\
            QTabBar::tab:selected{border-color: white;background:rgba(0, 0, 255, 255);color:white;}";

    QString tabBarStyle1 = "QTabBar::tab {min-width:100px;color: rgba(120,120,120);border: 1px solid;border-top-left-radius: 6px;border-top-right-radius: 6px;padding:1px;}\
            QTabBar::tab:!selected {margin-top: 2px;} \
            QTabBar::tab:selected {color: rgba(0,0,255);}";

    QString tabBarStyle2 = "QTabBar::tab {min-width:100px;color: black;background-color:qlineargradient(x1:0, y1:0, x2:0, y2:1, stop: 0 #eeeeee, stop: 1 gray);border: 0px solid;border-top-left-radius: 10px;border-top-right-radius: 10px;padding:5px;}\
            QTabBar::tab:!selected {margin-top: 5px;} \
            QTabBar::tab:selected {color: blue;}";

    QString tabBarStyle3 = "QTabWidget::pane{border:none;}\
            QTabWidget::tab-bar{alignment:left;}\
            QTabBar::tab{background:transparent;color:black;}\
            QTabBar::tab:hover{background:rgba(0, 0, 255, 80);}\
            QTabBar::tab:selected{border-color: white;background:rgba(0, 0, 255, 255);color:white;}";

    QString tabBarStyle4 = "QTabBar::tab:selected{background:rgba(30, 160, 255, 255);}";

    //ui->tabTotal->setStyleSheet(tabBarStyle4);
    //ui->tabTotal->tabBar()->setStyle(new CustomTabStyle2);

    init_tr_str();

    init_menu();

    initui_booter();
    initui_dp();
    initui_kernel();
    initui_misc();
    initui_nvram();
    initui_PlatformInfo();
    initui_UEFI();
    initui_acpi();

    //接受文件拖放打开
    this->setAcceptDrops(true);

    ui->tabTotal->removeTab(8);

    QFont font;

#ifdef Q_OS_WIN32
    reg_win();
    font.setPixelSize(18);
    ui->tabTotal->setDocumentMode(false);
    ui->btnOcvalidate->setEnabled(true);

    win = true;

#endif

#ifdef Q_OS_LINUX
    ui->btnMountEsp->setEnabled(false);
    font.setPixelSize(12);
    ui->btnOcvalidate->setEnabled(true);

    linuxOS = true;
#endif

#ifdef Q_OS_MAC
    font.setPixelSize(12);
    mac = true;
    ui->tabTotal->setDocumentMode(false);
    //ui->btnCheckUpdate->setVisible(false);

#endif

    //设置QToolTip颜色
    /*QPalette palette = QToolTip::palette();
    palette.setColor(QPalette::Inactive, QPalette::ToolTipBase, Qt::white);*/
    //设置ToolTip背景色
    /*palette.setColor(QPalette::Inactive, QPalette::ToolTipText, QColor(50, 50, 255, 255));
    QToolTip::setFont(font);*/

    //最近打开的文件
    QCoreApplication::setOrganizationName("ic005k");
    QCoreApplication::setOrganizationDomain("github.com/ic005k");
    QCoreApplication::setApplicationName("QtOpenCoreConfig");

    m_recentFiles = new RecentFiles(this);
    if (!zh_cn)
        m_recentFiles->attachToMenuAfterItem(
            ui->menuFile, "Save As...", SLOT(recentOpen(QString)));
    else
        m_recentFiles->attachToMenuAfterItem(
            ui->menuFile, "另存...", SLOT(recentOpen(QString)));

    m_recentFiles->setNumOfRecentFiles(10);

    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));

    this->setWindowModified(false);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::recentOpen(QString filename) { openFile(filename); }

void MainWindow::openFile(QString PlistFileName)
{
    if (!PlistFileName.isEmpty()) {
        setWindowTitle(title + PlistFileName);
        SaveFileName = PlistFileName;
    } else
        return;

    QSettings settings;
    QFileInfo fInfo(PlistFileName);

    settings.setValue("currentDirectory", fInfo.absolutePath());
    // qDebug() << settings.fileName(); //最近打开的文件所保存的位置
    m_recentFiles->setMostRecentFile(PlistFileName);

    //初始化
    // ACPI
    ui->table_acpi_add->setRowCount(0);
    ui->table_acpi_del->setRowCount(0);
    ui->table_acpi_patch->setRowCount(0);

    // Booter
    ui->table_booter->setRowCount(0);

    // DP
    ui->table_dp_add0->setRowCount(0);
    ui->table_dp_add->setRowCount(0);
    ui->table_dp_del0->setRowCount(0);
    ui->table_dp_del->setRowCount(0);

    // Kernel
    ui->table_kernel_add->setRowCount(0);
    ui->table_kernel_block->setRowCount(0);
    ui->table_kernel_Force->setRowCount(0);
    ui->table_kernel_patch->setRowCount(0);

    // Misc
    ui->tableBlessOverride->setRowCount(0);
    ui->tableEntries->setRowCount(0);
    ui->tableTools->setRowCount(0);

    // UEFI
    ui->table_uefi_drivers->setRowCount(0);
    ui->table_uefi_ReservedMemory->setRowCount(0);

    // NVRAM
    ui->table_nv_add0->setRowCount(0);
    ui->table_nv_add->setRowCount(0);
    ui->table_nv_del0->setRowCount(0);
    ui->table_nv_del->setRowCount(0);
    ui->table_nv_ls0->setRowCount(0);
    ui->table_nv_ls->setRowCount(0);

    QFile file(PlistFileName);
    QVariantMap map = PListParser::parsePList(&file).toMap();

    loading = true;

    ParserACPI(map);
    ParserBooter(map);
    ParserDP(map);
    ParserKernel(map);
    ParserMisc(map);
    ParserNvram(map);
    ParserPlatformInfo(map);
    ParserUEFI(map);

    loading = false;

    ui->actionSave->setEnabled(true);

    this->setWindowModified(false);
}

void MainWindow::on_btnOpen()
{
    QFileDialog fd;

    PlistFileName = fd.getOpenFileName(this, "Config file", "",
        "Config file(*.plist);;All files(*.*)");

    openFile(PlistFileName);
}

void MainWindow::on_btnTestWrite_clicked()
{
    QString plistPath = QDir::homePath() + "/xx.plist";
    qDebug() << plistPath;

    // QString  plistPath = "E:/xxx.plist";

    QSettings Reg(plistPath, QSettings::NativeFormat);

    Reg.setValue("ACPI", true);
    qDebug() << Reg.value("ACPI");

    int i = 1;
    QString str_key = "/key";
    QString str_class = "/class";
    QString str_value = "/value";

    Reg.setValue(QString::number(i) + str_key, "key1");
    Reg.setValue("1/class", "class1");
    Reg.setValue("1/value", "value1");

    qDebug() << Reg.value("1/key").toString();
    qDebug() << Reg.value("1/class").toString();
    qDebug() << Reg.value("1/value").toString();
}

void MainWindow::on_btnParse_clicked()
{
    QByteArray sample = "\
            <?xml version=\"1.0\" encoding=\"UTF-8\"?>\
            <!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\
            <plist version=\"1.0\">\
            <dict>\
            <key>SomeKey</key>\
            <string>Value1</string>\
            <key>MyOtherKey</key>\
            <string>Value2</string>\
            </dict>\
            </plist>";
    QBuffer buffer(&sample);
    // QVariantMap map = PListParser::parsePList(&buffer).toMap();
    QFile file(QDir::homePath() + "/com.xx.plist");
    QVariantMap map = PListParser::parsePList(&file).toMap();
    // QString val = map["test"].toString();
    qDebug() << map.firstKey();
    qDebug() << map.lastKey();
    qDebug() << map.keys();
    qDebug() << map.count();

    map = map["ACPI"].toMap();
    QVariantList map2 = map["Add"].toList();
    qDebug() << map2.at(0) << map2.count();
    QVariantMap map3 = map2.at(0).toMap();
    qDebug() << map3["Comment"].toString();
    qDebug() << map3["Enabled"].toBool();
    qDebug() << map3["Path"].toString();
}

void MainWindow::on_btnSerialize_clicked()
{
    QFile file(QDir::homePath() + "/com.xx.plist");
    QVariantMap map = PListParser::parsePList(&file).toMap();

    QVariantMap foo;
    QVariantMap sub;
    // sub = foo["SomeKey"].toMap();
    QString data = "2147483648";
    sub["test"] = 12345;
    qDebug() << data.toLongLong();
    sub["A"] = 1;

    foo["SomeOtherKey"] = sub;

    foo["test2"] = true;
    foo["test3"] = data.toLongLong();

    // qDebug() << "PList:\n" << PListSerializer::toPList(foo).toNSString();

    PListSerializer::toPList(foo, QDir::homePath() + "/test.plist");
}

void MainWindow::ParserACPI(QVariantMap map)
{
    map = map["ACPI"].toMap();
    if (map.isEmpty())
        return;

    //分析"Add"
    QVariantList map_add = map["Add"].toList();
    // qDebug() << map_add;
    ui->table_acpi_add->setRowCount(map_add.count());
    for (int i = 0; i < map_add.count(); i++) {
        QVariantMap map3 = map_add.at(i).toMap();

        QTableWidgetItem* newItem1;
        newItem1 = new QTableWidgetItem(map3["Path"].toString());
        ui->table_acpi_add->setItem(i, 0, newItem1);
        newItem1 = new QTableWidgetItem(map3["Comment"].toString());
        ui->table_acpi_add->setItem(i, 1, newItem1);

        init_enabled_data(ui->table_acpi_add, i, 2, map3["Enabled"].toString());
    }

    //分析Delete
    QVariantList map_del = map["Delete"].toList();
    // qDebug() << map_del;
    ui->table_acpi_del->setRowCount(map_del.count());
    for (int i = 0; i < map_del.count(); i++) {
        QVariantMap map3 = map_del.at(i).toMap();

        QTableWidgetItem* newItem1;
        newItem1 = new QTableWidgetItem(
            ByteToHexStr(map3["TableSignature"].toByteArray()));
        ui->table_acpi_del->setItem(i, 0, newItem1);

        newItem1 = new QTableWidgetItem(ByteToHexStr(map3["OemTableId"].toByteArray()));
        ui->table_acpi_del->setItem(i, 1, newItem1);

        newItem1 = new QTableWidgetItem(map3["TableLength"].toString());
        ui->table_acpi_del->setItem(i, 2, newItem1);

        newItem1 = new QTableWidgetItem(map3["Comment"].toString());
        ui->table_acpi_del->setItem(i, 3, newItem1);

        init_enabled_data(ui->table_acpi_del, i, 4, map3["All"].toString());

        init_enabled_data(ui->table_acpi_del, i, 5, map3["Enabled"].toString());
    }

    //分析Patch
    QVariantList map_patch = map["Patch"].toList();
    ui->table_acpi_patch->setRowCount(map_patch.count());
    for (int i = 0; i < map_patch.count(); i++) {
        QVariantMap map3 = map_patch.at(i).toMap();

        QTableWidgetItem* newItem1;
        newItem1 = new QTableWidgetItem(map3["TableSignature"].toString());
        ui->table_acpi_patch->setItem(i, 0, newItem1);

        newItem1 = new QTableWidgetItem(map3["OemTableId"].toString());
        ui->table_acpi_patch->setItem(i, 1, newItem1);

        newItem1 = new QTableWidgetItem(map3["TableLength"].toString());
        ui->table_acpi_patch->setItem(i, 2, newItem1);

        // newItem1 = new QTableWidgetItem(map3["Find"].toString());
        ui->table_acpi_patch->setItem(
            i, 3, new QTableWidgetItem(ByteToHexStr(map3["Find"].toByteArray())));

        newItem1 = new QTableWidgetItem(ByteToHexStr(map3["Replace"].toByteArray()));
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
    }

    //分析Quirks
    QVariantMap map_quirks = map["Quirks"].toMap();

    ui->chkFadtEnableReset->setChecked(map_quirks["FadtEnableReset"].toBool());
    ui->chkNormalizeHeaders->setChecked(map_quirks["NormalizeHeaders"].toBool());
    ui->chkRebaseRegions->setChecked(map_quirks["RebaseRegions"].toBool());
    ui->chkResetHwSig->setChecked(map_quirks["ResetHwSig"].toBool());
    ui->chkResetLogoStatus->setChecked(map_quirks["ResetLogoStatus"].toBool());
}

void MainWindow::initui_acpi()
{

    ui->btnExportMaster->setText(tr("Export") + "  ACPI");
    ui->btnImportMaster->setText(tr("Import") + "  ACPI");

    QTableWidgetItem* id0;
    // ACPI-Add
    ui->table_acpi_add->setColumnWidth(0, 550);
    id0 = new QTableWidgetItem(tr("Path"));
    ui->table_acpi_add->setHorizontalHeaderItem(0, id0);

    ui->table_acpi_add->setColumnWidth(1, 350);
    id0 = new QTableWidgetItem(tr("Comment"));
    ui->table_acpi_add->setHorizontalHeaderItem(1, id0);

    id0 = new QTableWidgetItem(tr("Enabled"));
    ui->table_acpi_add->setHorizontalHeaderItem(2, id0);

    ui->table_acpi_add->setAlternatingRowColors(true); //底色交替显示
    //ui->table_acpi_add->horizontalHeader()->setStretchLastSection(true); //设置充满表宽度

    // ACPI-Delete
    id0 = new QTableWidgetItem(tr("TableSignature"));
    ui->table_acpi_del->setHorizontalHeaderItem(0, id0);

    ui->table_acpi_del->setColumnWidth(1, 350);
    id0 = new QTableWidgetItem(tr("OemTableId"));
    ui->table_acpi_del->setHorizontalHeaderItem(1, id0);

    id0 = new QTableWidgetItem(tr("TableLength"));
    ui->table_acpi_del->setHorizontalHeaderItem(2, id0);

    ui->table_acpi_del->setColumnWidth(3, 250);
    id0 = new QTableWidgetItem(tr("Comment"));
    ui->table_acpi_del->setHorizontalHeaderItem(3, id0);

    id0 = new QTableWidgetItem(tr("All"));
    ui->table_acpi_del->setHorizontalHeaderItem(4, id0);

    id0 = new QTableWidgetItem(tr("Enabled"));
    ui->table_acpi_del->setHorizontalHeaderItem(5, id0);

    ui->table_acpi_del->setAlternatingRowColors(true);
    //ui->table_acpi_del->horizontalHeader()->setStretchLastSection(true);

    // ACPI-Patch
    ui->table_acpi_patch->setColumnWidth(0, 150);
    id0 = new QTableWidgetItem(tr("TableSignature"));
    ui->table_acpi_patch->setHorizontalHeaderItem(0, id0);

    id0 = new QTableWidgetItem(tr("OemTableId"));
    ui->table_acpi_patch->setHorizontalHeaderItem(1, id0);

    id0 = new QTableWidgetItem(tr("TableLength"));
    ui->table_acpi_patch->setHorizontalHeaderItem(2, id0);

    ui->table_acpi_patch->setColumnWidth(3, 200);
    id0 = new QTableWidgetItem(tr("Find"));
    ui->table_acpi_patch->setHorizontalHeaderItem(3, id0);

    ui->table_acpi_patch->setColumnWidth(4, 200);
    id0 = new QTableWidgetItem(tr("Replace"));
    ui->table_acpi_patch->setHorizontalHeaderItem(4, id0);

    ui->table_acpi_patch->setColumnWidth(5, 300);
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

    ui->table_acpi_patch->setAlternatingRowColors(true);
    ui->table_acpi_patch->horizontalHeader()->setStretchLastSection(true);

    if (zh_cn) {
        ui->table_acpi_patch->horizontalHeaderItem(0)->setToolTip(
            "匹配表签名等于该值，除非全为零。");
        ui->table_acpi_patch->horizontalHeaderItem(1)->setToolTip(
            "匹配表OEM ID等于此值，除非全为零。");
        ui->table_acpi_patch->horizontalHeaderItem(2)->setToolTip(
            "匹配表大小等于此值，除非为0。");
        ui->table_acpi_patch->horizontalHeaderItem(3)->setToolTip("查找");
        ui->table_acpi_patch->horizontalHeaderItem(4)->setToolTip("替换");
        ui->table_acpi_patch->horizontalHeaderItem(5)->setToolTip("注释");
        ui->table_acpi_patch->horizontalHeaderItem(6)->setToolTip(
            "查找比较期间使用的数据按位掩码。 通过忽略未屏蔽（设置到零）位。 "
            "可以设置为空数据以忽略。 否则必须等于“替换大小”。");
        ui->table_acpi_patch->horizontalHeaderItem(7)->setToolTip(
            "替换期间使用的数据按位掩码。 通过更新遮罩（设置到非零）位。 "
            "可以设置为空数据以忽略。 否则必须等于“替换大小”。");
        ui->table_acpi_patch->horizontalHeaderItem(8)->setToolTip(
            "要应用的补丁出现次数。 0将补丁应用于所有发现的事件。");
        ui->table_acpi_patch->horizontalHeaderItem(9)->setToolTip(
            "要搜索的最大字节数。 可以设置为0以浏览整个ACPI表。");
        ui->table_acpi_patch->horizontalHeaderItem(10)->setToolTip(
            "完成替换之前要跳过的发现事件数。");
        ui->table_acpi_patch->horizontalHeaderItem(11)->setToolTip("是否启用");
    }
}

void MainWindow::initui_booter()
{
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
    ui->table_Booter_patch->horizontalHeader()->setStretchLastSection(true);

    //MmioWhitelist

    ui->table_booter->setColumnWidth(0, 450);
    id0 = new QTableWidgetItem(tr("Address"));
    ui->table_booter->setHorizontalHeaderItem(0, id0);

    ui->table_booter->setColumnWidth(1, 400);
    id0 = new QTableWidgetItem(tr("Comment"));
    ui->table_booter->setHorizontalHeaderItem(1, id0);

    id0 = new QTableWidgetItem(tr("Enabled"));
    ui->table_booter->setHorizontalHeaderItem(2, id0);

    ui->table_booter->setAlternatingRowColors(true);
    //ui->table_booter->horizontalHeader()->setStretchLastSection(true);

    if (zh_cn) {
        ui->table_booter->horizontalHeaderItem(0)->setToolTip("地址");
        ui->table_booter->horizontalHeaderItem(1)->setToolTip("注释");
        ui->table_booter->horizontalHeaderItem(2)->setToolTip("是否启用");
    }
}

void MainWindow::ParserBooter(QVariantMap map)
{
    map = map["Booter"].toMap();
    if (map.isEmpty())
        return;

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

    //MmioWhitelist

    QVariantList map_add = map["MmioWhitelist"].toList();

    ui->table_booter->setRowCount(map_add.count());
    for (int i = 0; i < map_add.count(); i++) {
        QVariantMap map3 = map_add.at(i).toMap();

        QTableWidgetItem* newItem1;
        newItem1 = new QTableWidgetItem(map3["Address"].toString());
        ui->table_booter->setItem(i, 0, newItem1);

        newItem1 = new QTableWidgetItem(map3["Comment"].toString());
        ui->table_booter->setItem(i, 1, newItem1);

        init_enabled_data(ui->table_booter, i, 2, map3["Enabled"].toString());
    }

    // Quirks
    QVariantMap map_quirks = map["Quirks"].toMap();

    ui->chkAvoidRuntimeDefrag->setChecked(
        map_quirks["AvoidRuntimeDefrag"].toBool());
    ui->chkDevirtualiseMmio->setChecked(map_quirks["DevirtualiseMmio"].toBool());
    ui->chkDisableSingleUser->setChecked(
        map_quirks["DisableSingleUser"].toBool());
    ui->chkDisableVariableWrite->setChecked(
        map_quirks["DisableVariableWrite"].toBool());
    ui->chkDiscardHibernateMap->setChecked(
        map_quirks["DiscardHibernateMap"].toBool());
    ui->chkEnableSafeModeSlide->setChecked(
        map_quirks["EnableSafeModeSlide"].toBool());
    ui->chkEnableWriteUnprotector->setChecked(
        map_quirks["EnableWriteUnprotector"].toBool());
    ui->chkForceExitBootServices->setChecked(
        map_quirks["ForceExitBootServices"].toBool());
    ui->chkProtectMemoryRegions->setChecked(
        map_quirks["ProtectMemoryRegions"].toBool());
    ui->chkProtectSecureBoot->setChecked(
        map_quirks["ProtectSecureBoot"].toBool());
    ui->chkProtectUefiServices->setChecked(
        map_quirks["ProtectUefiServices"].toBool());
    ui->chkProvideCustomSlide->setChecked(
        map_quirks["ProvideCustomSlide"].toBool());
    ui->editProvideMaxSlide->setText(map_quirks["ProvideMaxSlide"].toString());
    ui->chkRebuildAppleMemoryMap->setChecked(
        map_quirks["RebuildAppleMemoryMap"].toBool());
    ui->chkSetupVirtualMap->setChecked(map_quirks["SetupVirtualMap"].toBool());
    ui->chkSignalAppleOS->setChecked(map_quirks["SignalAppleOS"].toBool());
    ui->chkSyncRuntimePermissions->setChecked(
        map_quirks["SyncRuntimePermissions"].toBool());

    ui->chkAllowRelocationBlock->setChecked(
        map_quirks["AllowRelocationBlock"].toBool());
}

void MainWindow::initui_dp()
{
    QTableWidgetItem* id0;

    // Add
    ui->table_dp_add0->setColumnWidth(0, 475);
    ui->table_dp_add0->setMinimumWidth(500);

    id0 = new QTableWidgetItem(tr("PCILists"));
    ui->table_dp_add0->setHorizontalHeaderItem(0, id0);

    ui->table_dp_add0->setAlternatingRowColors(true);
    ui->table_dp_add0->horizontalHeader()->setStretchLastSection(true);

    ui->table_dp_add->setColumnWidth(0, 300);
    ui->table_dp_add->setMinimumWidth(700);

    id0 = new QTableWidgetItem(tr("Key"));
    ui->table_dp_add->setHorizontalHeaderItem(0, id0);

    id0 = new QTableWidgetItem(tr("Class"));
    ui->table_dp_add->setHorizontalHeaderItem(1, id0);

    ui->table_dp_add->setColumnWidth(2, 260);
    id0 = new QTableWidgetItem(tr("Value"));
    ui->table_dp_add->setHorizontalHeaderItem(2, id0);

    ui->table_dp_add->setAlternatingRowColors(true);
    ui->table_dp_add->horizontalHeader()->setStretchLastSection(true);

    // Delete

    ui->table_dp_del0->setColumnWidth(0, 500);
    ui->table_dp_del0->setMinimumWidth(400);
    id0 = new QTableWidgetItem(tr("PCILists"));
    ui->table_dp_del0->setHorizontalHeaderItem(0, id0);

    ui->table_dp_del0->setAlternatingRowColors(true);
    ui->table_dp_del0->horizontalHeader()->setStretchLastSection(true);

    ui->table_dp_del->setColumnWidth(0, 500);
    id0 = new QTableWidgetItem(tr("Value"));
    ui->table_dp_del->setHorizontalHeaderItem(0, id0);

    ui->table_dp_del->setAlternatingRowColors(true);
    ui->table_dp_del->horizontalHeader()->setStretchLastSection(true);
}

void MainWindow::ParserDP(QVariantMap map)
{
    map = map["DeviceProperties"].toMap();
    if (map.isEmpty())
        return;

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
        ui->table_dp_add->setRowCount(map_sub.keys().count()); //子键的个数

        for (int j = 0; j < map_sub.keys().count(); j++) {

            // QTableWidgetItem *newItem1;
            newItem1 = new QTableWidgetItem(map_sub.keys().at(j)); //键
            ui->table_dp_add->setItem(j, 0, newItem1);

            QString dtype = map_sub[map_sub.keys().at(j)].typeName();
            QString ztype;
            if (dtype == "QByteArray")
                ztype = "Data";
            if (dtype == "QString")
                ztype = "String";
            if (dtype == "qlonglong")
                ztype = "Number";

            newItem1 = new QTableWidgetItem(ztype); //数据类型
            newItem1->setTextAlignment(Qt::AlignCenter);
            ui->table_dp_add->setItem(j, 1, newItem1);

            QString type_name = map_sub[map_sub.keys().at(j)].typeName();
            if (type_name == "QByteArray") {
                QByteArray tohex = map_sub[map_sub.keys().at(j)].toByteArray();
                QString va = tohex.toHex().toUpper();
                newItem1 = new QTableWidgetItem(va);

            } else
                newItem1 = new QTableWidgetItem(map_sub[map_sub.keys().at(j)].toString());
            ui->table_dp_add->setItem(j, 2, newItem1);
        }

        //保存子条目里面的数据，以便以后加载

        write_ini("table_dp_add0", ui->table_dp_add, i);
    }

    int last = ui->table_dp_add0->rowCount();
    ui->table_dp_add0->setCurrentCell(last - 1, 0);

    // Delete
    init_value(map["Delete"].toMap(), ui->table_dp_del0, ui->table_dp_del);
}

void MainWindow::initui_kernel()
{

    QTableWidgetItem* id0;
    // Add
    ui->table_kernel_add->setColumnCount(8);

    ui->table_kernel_add->setColumnWidth(0, 250);
    id0 = new QTableWidgetItem(tr("BundlePath"));
    ui->table_kernel_add->setHorizontalHeaderItem(0, id0);
    ui->table_kernel_add->horizontalHeaderItem(0)->setToolTip(strBundlePath);

    ui->table_kernel_add->setColumnWidth(1, 250);
    id0 = new QTableWidgetItem(tr("Comment"));
    ui->table_kernel_add->setHorizontalHeaderItem(1, id0);
    ui->table_kernel_add->horizontalHeaderItem(1)->setToolTip(strComment);

    ui->table_kernel_add->setColumnWidth(2, 250);
    id0 = new QTableWidgetItem(tr("ExecutablePath"));
    ui->table_kernel_add->setHorizontalHeaderItem(2, id0);
    ui->table_kernel_add->horizontalHeaderItem(2)->setToolTip(strExecutablePath);

    ui->table_kernel_add->setColumnWidth(0, 250);
    id0 = new QTableWidgetItem(tr("PlistPath"));
    ui->table_kernel_add->setHorizontalHeaderItem(3, id0);
    ui->table_kernel_add->horizontalHeaderItem(3)->setToolTip(strPlistPath);

    ui->table_kernel_add->setColumnWidth(0, 250);
    id0 = new QTableWidgetItem(tr("MinKernel"));
    ui->table_kernel_add->setHorizontalHeaderItem(4, id0);
    ui->table_kernel_add->horizontalHeaderItem(4)->setToolTip(strMinKernel);

    ui->table_kernel_add->setColumnWidth(0, 250);
    id0 = new QTableWidgetItem(tr("MaxKernel"));
    ui->table_kernel_add->setHorizontalHeaderItem(5, id0);
    ui->table_kernel_add->horizontalHeaderItem(5)->setToolTip(strMaxKernel);

    ui->table_kernel_add->setColumnWidth(0, 250);
    id0 = new QTableWidgetItem(tr("Enabled"));
    ui->table_kernel_add->setHorizontalHeaderItem(6, id0);
    ui->table_kernel_add->horizontalHeaderItem(6)->setToolTip(strEnabled);

    ui->table_kernel_add->setColumnWidth(0, 250);
    id0 = new QTableWidgetItem(tr("Arch"));
    ui->table_kernel_add->setHorizontalHeaderItem(7, id0);
    ui->table_kernel_add->horizontalHeaderItem(7)->setToolTip(strArch);

    ui->table_kernel_add->setAlternatingRowColors(true);

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

void MainWindow::ParserKernel(QVariantMap map)
{
    map = map["Kernel"].toMap();
    if (map.isEmpty())
        return;

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
        newItem1->setTextAlignment(Qt::AlignCenter);
        ui->table_kernel_patch->setItem(i, 13, newItem1);
    }

    // Emulate
    QVariantMap map_Emulate = map["Emulate"].toMap();
    ui->editCpuid1Data->setText(
        ByteToHexStr(map_Emulate["Cpuid1Data"].toByteArray()));
    ui->editCpuid1Mask->setText(
        ByteToHexStr(map_Emulate["Cpuid1Mask"].toByteArray()));
    ui->chkDummyPowerManagement->setChecked(
        map_Emulate["DummyPowerManagement"].toBool());
    ui->editMaxKernel->setText(map_Emulate["MaxKernel"].toString());
    ui->editMinKernel->setText(map_Emulate["MinKernel"].toString());

    // Quirks
    QVariantMap map_quirks = map["Quirks"].toMap();

    ui->chkAppleCpuPmCfgLock->setChecked(
        map_quirks["AppleCpuPmCfgLock"].toBool());
    ui->chkAppleXcpmCfgLock->setChecked(map_quirks["AppleXcpmCfgLock"].toBool());
    ui->chkAppleXcpmExtraMsrs->setChecked(
        map_quirks["AppleXcpmExtraMsrs"].toBool());
    ui->chkAppleXcpmForceBoost->setChecked(
        map_quirks["AppleXcpmForceBoost"].toBool());

    ui->chkCustomSMBIOSGuid->setChecked(map_quirks["CustomSMBIOSGuid"].toBool());

    ui->chkDisableIoMapper->setChecked(map_quirks["DisableIoMapper"].toBool());
    ui->chkDisableRtcChecksum->setChecked(
        map_quirks["DisableRtcChecksum"].toBool());
    ui->chkExternalDiskIcons->setChecked(
        map_quirks["ExternalDiskIcons"].toBool());
    ui->chkIncreasePciBarSize->setChecked(
        map_quirks["IncreasePciBarSize"].toBool());
    ui->chkLapicKernelPanic->setChecked(map_quirks["LapicKernelPanic"].toBool());
    ui->chkPanicNoKextDump->setChecked(map_quirks["PanicNoKextDump"].toBool());
    ui->chkPowerTimeoutKernelPanic->setChecked(
        map_quirks["PowerTimeoutKernelPanic"].toBool());
    ui->chkThirdPartyDrives->setChecked(map_quirks["ThirdPartyDrives"].toBool());
    ui->chkXhciPortLimit->setChecked(map_quirks["XhciPortLimit"].toBool());
    ui->chkDisableLinkeditJettison->setChecked(
        map_quirks["DisableLinkeditJettison"].toBool());

    ui->chkExtendBTFeatureFlags->setChecked(
        map_quirks["ExtendBTFeatureFlags"].toBool());
    ui->chkLegacyCommpage->setChecked(map_quirks["LegacyCommpage"].toBool());

    ui->chkForceSecureBootScheme->setChecked(
        map_quirks["ForceSecureBootScheme"].toBool());

    // Scheme
    QVariantMap map_Scheme = map["Scheme"].toMap();
    ui->chkFuzzyMatch->setChecked(map_Scheme["FuzzyMatch"].toBool());

    QString hm = map_Scheme["KernelArch"].toString();
    ui->cboxKernelArch->setCurrentText(hm.trimmed());

    hm = map_Scheme["KernelCache"].toString();
    ui->cboxKernelCache->setCurrentText(hm.trimmed());
}

void MainWindow::initui_misc()
{
    // Boot
    ui->cboxHibernateMode->addItem("None");
    ui->cboxHibernateMode->addItem("Auto");
    ui->cboxHibernateMode->addItem("RTC");
    ui->cboxHibernateMode->addItem("NVRAM");

    ui->cboxPickerMode->addItem("Builtin");
    ui->cboxPickerMode->addItem("External");
    ui->cboxPickerMode->addItem("Apple");

    ui->cboxPickerVariant->addItem("Auto");
    ui->cboxPickerVariant->addItem("Default");
    ui->cboxPickerVariant->addItem("Old");
    ui->cboxPickerVariant->addItem("Modern");
    ui->cboxPickerVariant->addItem("Other value");

    // Security
    ui->cboxBootProtect->addItem("None");
    ui->cboxBootProtect->addItem("Bootstrap");

    ui->cboxDmgLoading->addItem("Disabled");
    ui->cboxDmgLoading->addItem("Signed");
    ui->cboxDmgLoading->addItem("Any");

    ui->cboxVault->addItem("Optional");
    ui->cboxVault->addItem("Basic");
    ui->cboxVault->addItem("Secure");

    ui->cboxSecureBootModel->setEditable(true);
    ui->cboxSecureBootModel->addItem("Disabled");
    ui->cboxSecureBootModel->addItem("Default");

    // BlessOverride
    QTableWidgetItem* id0;
    ui->tableBlessOverride->setColumnWidth(0, 1150);
    id0 = new QTableWidgetItem(tr("BlessOverride"));
    ui->tableBlessOverride->setHorizontalHeaderItem(0, id0);

    ui->tableBlessOverride->setAlternatingRowColors(true);
    ui->tableBlessOverride->horizontalHeader()->setStretchLastSection(true);

    // Entries
    ui->tableEntries->setColumnCount(7);

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

    ui->tableEntries->setAlternatingRowColors(true);

    // Tools
    ui->tableTools->setColumnCount(8);

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

    ui->tableTools->setAlternatingRowColors(true);
}

void MainWindow::ParserMisc(QVariantMap map)
{
    map = map["Misc"].toMap();
    if (map.isEmpty())
        return;

    //分析"Boot"
    QVariantMap map_boot = map["Boot"].toMap();

    ui->editConsoleAttributes->setText(map_boot["ConsoleAttributes"].toString());

    QString hm = map_boot["HibernateMode"].toString();
    ui->cboxHibernateMode->setCurrentText(hm.trimmed());

    ui->chkHideAuxiliary->setChecked(map_boot["HideAuxiliary"].toBool());
    ui->editPickerAttributes->setText(map_boot["PickerAttributes"].toString());
    ui->chkPickerAudioAssist->setChecked(map_boot["PickerAudioAssist"].toBool());

    hm = map_boot["PickerMode"].toString();
    ui->cboxPickerMode->setCurrentText(hm.trimmed());

    hm = map_boot["PickerVariant"].toString();
    if (hm == "")
        hm = "Auto";
    ui->cboxPickerVariant->setCurrentText(hm.trimmed());

    ui->chkPollAppleHotKeys->setChecked(map_boot["PollAppleHotKeys"].toBool());
    ui->chkShowPicker->setChecked(map_boot["ShowPicker"].toBool());
    ui->editTakeoffDelay->setText(map_boot["TakeoffDelay"].toString());
    ui->editTimeout->setText(map_boot["Timeout"].toString());

    // Debug
    QVariantMap map_debug = map["Debug"].toMap();
    ui->chkAppleDebug->setChecked(map_debug["AppleDebug"].toBool());
    ui->chkApplePanic->setChecked(map_debug["ApplePanic"].toBool());
    ui->chkDisableWatchDog->setChecked(map_debug["DisableWatchDog"].toBool());

    ui->editDisplayDelay->setText(map_debug["DisplayDelay"].toString());

    ui->editDisplayLevel->setText(map_debug["DisplayLevel"].toString());

    ui->chkSerialInit->setChecked(map_debug["SerialInit"].toBool());
    ui->chkSysReport->setChecked(map_debug["SysReport"].toBool());

    ui->editTarget->setText(map_debug["Target"].toString());

    // Security
    QVariantMap map_security = map["Security"].toMap();
    ui->chkAllowNvramReset->setChecked(map_security["AllowNvramReset"].toBool());
    ui->chkAllowSetDefault->setChecked(map_security["AllowSetDefault"].toBool());
    ui->chkAuthRestart->setChecked(map_security["AuthRestart"].toBool());
    ui->chkBlacklistAppleUpdate->setChecked(
        map_security["BlacklistAppleUpdate"].toBool());

    hm = map_security["BootProtect"].toString();
    ui->cboxBootProtect->setCurrentText(hm.trimmed());

    ui->editExposeSensitiveData->setText(
        map_security["ExposeSensitiveData"].toString());

    ui->editHaltLevel->setText(map_security["HaltLevel"].toString());
    ui->editScanPolicy->setText(map_security["ScanPolicy"].toString());

    hm = map_security["Vault"].toString();
    ui->cboxVault->setCurrentText(hm.trimmed());

    hm = map_security["DmgLoading"].toString();
    ui->cboxDmgLoading->setCurrentText(hm.trimmed());

    ui->editApECID->setText(map_security["ApECID"].toString());

    hm = map_security["SecureBootModel"].toString().trimmed();
    if (hm == "")
        hm = "Disabled";
    ui->cboxSecureBootModel->setCurrentText(hm.trimmed());

    ui->chkEnablePassword->setChecked(map_security["EnablePassword"].toBool());
    ui->editPasswordHash->setText(
        ByteToHexStr(map_security["PasswordHash"].toByteArray()));
    ui->editPasswordSalt->setText(
        ByteToHexStr(map_security["PasswordSalt"].toByteArray()));

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
    }
}

void MainWindow::initui_nvram()
{

    QString y = QString::number(QDate::currentDate().year());
    QString m = QString::number(QDate::currentDate().month());
    QString d = QString::number(QDate::currentDate().day());

    QString h = QString::number(QTime::currentTime().hour());
    QString mm = QString::number(QTime::currentTime().minute());
    QString s = QString::number(QTime::currentTime().second());

    CurrentDateTime = y + m + d + h + mm + s;

    // qDebug() << CurrentDateTime;

    QTableWidgetItem* id0;

    // Add
    ui->table_nv_add0->setColumnWidth(0, 450);
    ui->table_nv_add0->setMinimumWidth(300);
    ui->table_nv_add0->setMaximumWidth(470);
    id0 = new QTableWidgetItem(tr("UUID"));
    ui->table_nv_add0->setHorizontalHeaderItem(0, id0);
    ui->table_nv_add0->setAlternatingRowColors(true);
    ui->table_nv_add0->horizontalHeader()->setStretchLastSection(true);

    ui->table_nv_add0->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->table_nv_add0, SIGNAL(customContextMenuRequested(QPoint)), this,
        SLOT(show_menu0(QPoint)));

    ui->table_nv_add->setColumnWidth(0, 200);
    id0 = new QTableWidgetItem(tr("Key"));
    ui->table_nv_add->setHorizontalHeaderItem(0, id0);

    ui->table_nv_add->setColumnWidth(2, 350);
    id0 = new QTableWidgetItem(tr("Value"));
    ui->table_nv_add->setHorizontalHeaderItem(2, id0);

    id0 = new QTableWidgetItem(tr("Class"));
    ui->table_nv_add->setHorizontalHeaderItem(1, id0);

    ui->table_nv_add->setAlternatingRowColors(true);
    ui->table_nv_add->horizontalHeader()->setStretchLastSection(true);

    ui->table_nv_add->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->table_nv_add, SIGNAL(customContextMenuRequested(QPoint)), this,
        SLOT(show_menu(QPoint)));

    // Delete

    ui->table_nv_del0->setColumnWidth(0, 500);
    id0 = new QTableWidgetItem(tr("UUID"));
    ui->table_nv_del0->setHorizontalHeaderItem(0, id0);

    ui->table_nv_del0->setAlternatingRowColors(true);
    ui->table_nv_del0->horizontalHeader()->setStretchLastSection(true);

    ui->table_nv_del->setColumnWidth(0, 500);
    id0 = new QTableWidgetItem(tr("Value"));
    ui->table_nv_del->setHorizontalHeaderItem(0, id0);

    ui->table_nv_del->setAlternatingRowColors(true);
    ui->table_nv_del->horizontalHeader()->setStretchLastSection(true);

    // LegacySchema

    ui->table_nv_ls0->setColumnWidth(0, 500);
    id0 = new QTableWidgetItem(tr("UUID"));
    ui->table_nv_ls0->setHorizontalHeaderItem(0, id0);
    ui->table_nv_ls0->setAlternatingRowColors(true);
    ui->table_nv_ls0->horizontalHeader()->setStretchLastSection(true);

    ui->table_nv_ls->setColumnWidth(0, 500);
    id0 = new QTableWidgetItem(tr("Value"));
    ui->table_nv_ls->setHorizontalHeaderItem(0, id0);
    ui->table_nv_ls->setAlternatingRowColors(true);
    ui->table_nv_ls->horizontalHeader()->setStretchLastSection(true);

    QSplitter* splitterMain = new QSplitter(Qt::Horizontal, this);
    splitterMain->addWidget(ui->table_nv_add0);
    splitterMain->addWidget(ui->table_nv_add);
    ui->gridLayout_nv_add->addWidget(splitterMain);
}

void MainWindow::ParserNvram(QVariantMap map)
{
    map = map["NVRAM"].toMap();
    if (map.isEmpty())
        return;

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
        ui->table_nv_add->setRowCount(map_sub.keys().count()); //子键的个数

        for (int j = 0; j < map_sub.keys().count(); j++) {

            // QTableWidgetItem *newItem1;
            newItem1 = new QTableWidgetItem(map_sub.keys().at(j)); //键
            ui->table_nv_add->setItem(j, 0, newItem1);

            QString dtype = map_sub[map_sub.keys().at(j)].typeName();
            QString ztype;
            if (dtype == "QByteArray")
                ztype = "Data";
            if (dtype == "QString")
                ztype = "String";
            if (dtype == "qlonglong")
                ztype = "Number";
            newItem1 = new QTableWidgetItem(ztype); //数据类型
            newItem1->setTextAlignment(Qt::AlignCenter);
            ui->table_nv_add->setItem(j, 1, newItem1);

            QString type_name = map_sub[map_sub.keys().at(j)].typeName();
            if (type_name == "QByteArray") {
                QByteArray tohex = map_sub[map_sub.keys().at(j)].toByteArray();
                QString va = tohex.toHex().toUpper();
                newItem1 = new QTableWidgetItem(va);
            } else
                newItem1 = new QTableWidgetItem(map_sub[map_sub.keys().at(j)].toString());
            ui->table_nv_add->setItem(j, 2, newItem1);
        }

        //保存子条目里面的数据，以便以后加载

        write_ini("table_nv_add0", ui->table_nv_add, i);
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

void MainWindow::write_ini(QString table_name, QTableWidget* mytable, int i)
{
    QString plistPath = QDir::homePath() + "/.config/QtOCC/" + CurrentDateTime + table_name + QString::number(i + 1) + ".ini";
    // qDebug() << plistPath;
    QFile file(plistPath);
    if (file.exists()) //如果文件存在，则先删除它
        file.remove();

    // QSettings Reg(plistPath, QSettings::NativeFormat);
    QSettings Reg(plistPath, QSettings::IniFormat); //全平台都采用ini格式

    for (int k = 0; k < mytable->rowCount(); k++) {
        Reg.setValue(QString::number(k + 1) + "/key", mytable->item(k, 0)->text());
        Reg.setValue(QString::number(k + 1) + "/class",
            mytable->item(k, 1)->text());
        Reg.setValue(QString::number(k + 1) + "/value",
            mytable->item(k, 2)->text());
    }

    //记录总数
    Reg.setValue("total", mytable->rowCount());

    IniFile.push_back(plistPath);
}

void MainWindow::read_ini(QString table_name, QTableWidget* mytable, int i)
{
    QString plistPath = QDir::homePath() + "/.config/QtOCC/" + CurrentDateTime + table_name + QString::number(i + 1) + ".ini";
    // qDebug() << plistPath;
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

void MainWindow::read_value_ini(QString table_name, QTableWidget* mytable,
    int i)
{
    QString plistPath = QDir::homePath() + "/.config/QtOCC/" + CurrentDateTime + table_name + QString::number(i + 1) + ".ini";
    // qDebug() << plistPath;
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

void MainWindow::on_table_dp_add0_cellClicked(int row, int column)
{
    //读取ini数据并加载到table_dp_add中
    // qDebug() << row;

    loading = true;

    if (column == 0)
        read_ini("table_dp_add0", ui->table_dp_add, row);

    loading = false;

    ui->statusbar->showMessage(ui->table_dp_add0->currentItem()->text());
}

void MainWindow::on_table_dp_add_itemSelectionChanged() { }

void MainWindow::on_table_dp_add_itemChanged(QTableWidgetItem* item)
{
    if (item->text().isEmpty()) {
    }
    //当条目有修改时，重新写入数据
    if (!loading) //数据已经加载完成后
        write_ini("table_dp_add0", ui->table_dp_add,
            ui->table_dp_add0->currentRow());

    this->setWindowModified(true);
}

void MainWindow::on_table_nv_add0_cellClicked(int row, int column)
{
    loading = true;

    if (column == 0)
        read_ini("table_nv_add0", ui->table_nv_add, row);

    loading = false;

    ui->statusbar->showMessage(ui->table_nv_add0->currentItem()->text());
}

void MainWindow::on_table_nv_add_itemChanged(QTableWidgetItem* item)
{
    if (item->text().isEmpty()) {
    }

    //当条目有修改时，重新写入数据
    if (!loading) //数据已经加载完成后
        write_ini("table_nv_add0", ui->table_nv_add,
            ui->table_nv_add0->currentRow());

    this->setWindowModified(true);
}

void MainWindow::init_value(QVariantMap map_fun, QTableWidget* table,
    QTableWidget* subtable)
{
    // QVariantMap map_del = map["Delete"].toMap();

    table->setRowCount(map_fun.count());
    subtable->setRowCount(0); //归零，清除之前的残留
    QTableWidgetItem* newItem1;
    for (int i = 0; i < map_fun.count(); i++) {
        newItem1 = new QTableWidgetItem(map_fun.keys().at(i));
        table->setItem(i, 0, newItem1);

        //加载子条目
        QVariantList map_sub_list = map_fun[map_fun.keys().at(i)].toList(); //是个数组
        subtable->setRowCount(map_sub_list.count()); //子键的个数
        // qDebug() << map_sub_list.count();

        for (int j = 0; j < map_sub_list.count(); j++) {

            // qDebug() << map_sub_list.at(j).toString();
            newItem1 = new QTableWidgetItem(map_sub_list.at(j).toString()); //键
            subtable->setItem(j, 0, newItem1);
        }

        //保存子条目里面的数据，以便以后加载
        write_value_ini(table->objectName(), subtable, i);
    }

    int last = table->rowCount();
    table->setCurrentCell(last - 1, 0);
}

void MainWindow::write_value_ini(QString tablename, QTableWidget* subtable,
    int i)
{

    QString plistPath = QDir::homePath() + "/.config/QtOCC/" + CurrentDateTime + tablename + QString::number(i + 1) + ".ini";
    // qDebug() << plistPath;
    QFile file(plistPath);
    if (file.exists())
        file.remove();
    // QSettings Reg(plistPath, QSettings::NativeFormat);
    QSettings Reg(plistPath, QSettings::IniFormat);

    for (int k = 0; k < subtable->rowCount(); k++) {
        Reg.setValue(QString::number(k + 1) + "/key", subtable->item(k, 0)->text());
    }

    //记录总数
    Reg.setValue("total", subtable->rowCount());

    IniFile.push_back(plistPath);
}

void MainWindow::on_table_nv_del0_cellClicked(int row, int column)
{
    loading = true;

    if (column == 0)
        read_value_ini(ui->table_nv_del0->objectName(), ui->table_nv_del, row);

    loading = false;

    ui->statusbar->showMessage(ui->table_nv_del0->currentItem()->text());
}

void MainWindow::on_table_nv_ls0_cellClicked(int row, int column)
{
    loading = true;

    if (column == 0)
        read_value_ini(ui->table_nv_ls0->objectName(), ui->table_nv_ls, row);

    loading = false;

    ui->statusbar->showMessage(ui->table_nv_ls0->currentItem()->text());
}

void MainWindow::on_table_nv_del_itemChanged(QTableWidgetItem* item)
{
    if (item->text().isEmpty()) {
    }

    if (!loading)
        write_value_ini(ui->table_nv_del0->objectName(), ui->table_nv_del,
            ui->table_nv_del0->currentRow());

    this->setWindowModified(true);
}

void MainWindow::on_table_nv_ls_itemChanged(QTableWidgetItem* item)
{
    if (item->text().isEmpty()) {
    }

    if (!loading)
        write_value_ini(ui->table_nv_ls0->objectName(), ui->table_nv_ls,
            ui->table_nv_ls0->currentRow());

    this->setWindowModified(true);
}

void MainWindow::on_table_dp_del0_cellClicked(int row, int column)
{
    loading = true;

    if (column == 0)
        read_value_ini(ui->table_dp_del0->objectName(), ui->table_dp_del, row);

    loading = false;

    ui->statusbar->showMessage(ui->table_dp_del0->currentItem()->text());
}

void MainWindow::on_table_dp_del_itemChanged(QTableWidgetItem* item)
{
    if (item->text().isEmpty()) {
    }

    if (!loading)
        write_value_ini(ui->table_dp_del0->objectName(), ui->table_dp_del,
            ui->table_dp_del0->currentRow());

    this->setWindowModified(true);
}

void MainWindow::initui_PlatformInfo()
{
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

    ui->cboxSystemMemoryStatus->addItem("");
    ui->cboxSystemMemoryStatus->addItem("Auto");
    ui->cboxSystemMemoryStatus->addItem("Upgradable");
    ui->cboxSystemMemoryStatus->addItem("Soldered");

    // Memory-Devices
    QTableWidgetItem* id0;
    ui->tableDevices->setColumnWidth(0, 150);
    id0 = new QTableWidgetItem(tr("AssetTag"));
    ui->tableDevices->setHorizontalHeaderItem(0, id0);

    id0 = new QTableWidgetItem(tr("BankLocator"));
    ui->tableDevices->setHorizontalHeaderItem(1, id0);

    ui->tableDevices->setColumnWidth(2, 200);
    id0 = new QTableWidgetItem(tr("DeviceLocator"));
    ui->tableDevices->setHorizontalHeaderItem(2, id0);

    id0 = new QTableWidgetItem(tr("Manufacturer"));
    ui->tableDevices->setHorizontalHeaderItem(3, id0);

    id0 = new QTableWidgetItem(tr("PartNumber"));
    ui->tableDevices->setHorizontalHeaderItem(4, id0);

    ui->tableDevices->setColumnWidth(5, 200);
    id0 = new QTableWidgetItem(tr("SerialNumber"));
    ui->tableDevices->setHorizontalHeaderItem(5, id0);

    id0 = new QTableWidgetItem(tr("Size"));
    ui->tableDevices->setHorizontalHeaderItem(6, id0);

    id0 = new QTableWidgetItem(tr("Speed"));
    ui->tableDevices->setHorizontalHeaderItem(7, id0);

    ui->tableDevices->setAlternatingRowColors(true); //底色交替显示
    //ui->tableDevices->horizontalHeader()->setStretchLastSection(true);

    // ui->cboxSystemProductName->setEditable(true);
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
    QFile file(appInfo.filePath() + "/macserial.exe");
    if (file.exists())
        gs->execute(appInfo.filePath() + "/macserial.exe",
            QStringList() << "-s"); //阻塞execute
    else {

        ui->btnGenerate->setEnabled(false);
        ui->btnSystemUUID->setEnabled(false);
    }

    ui->tabPlatformInfo->removeTab(5);

#endif

#ifdef Q_OS_LINUX
    gs->execute(appInfo.filePath() + "/macserial", QStringList() << "-s");
    /*暂时屏蔽*/
    ui->tabPlatformInfo->removeTab(5);
    ui->btnGenerate->setEnabled(true);

#endif

#ifdef Q_OS_MAC
    si->start(appInfo.filePath() + "/macserial", QStringList() << "-s");
#endif
    connect(si, SIGNAL(finished(int)), this, SLOT(readResultSystemInfo()));
}

void MainWindow::ParserPlatformInfo(QVariantMap map)
{
    map = map["PlatformInfo"].toMap();
    if (map.isEmpty())
        return;

    ui->chkAutomatic->setChecked(map["Automatic"].toBool());
    ui->chkCustomMemory->setChecked(map["CustomMemory"].toBool());
    ui->chkUpdateDataHub->setChecked(map["UpdateDataHub"].toBool());
    ui->chkUpdateNVRAM->setChecked(map["UpdateNVRAM"].toBool());
    ui->chkUpdateSMBIOS->setChecked(map["UpdateSMBIOS"].toBool());

    QString usm = map["UpdateSMBIOSMode"].toString();
    ui->cboxUpdateSMBIOSMode->setCurrentText(usm.trimmed());

    if (usm.trimmed() == "Custom") {
        ui->cboxUpdateSMBIOSMode->setCurrentText(usm.trimmed());
        ui->chkCustomSMBIOSGuid->setChecked(true); //联动
    } else
        ui->chkCustomSMBIOSGuid->setChecked(false);

    // Generic
    QVariantMap mapGeneric = map["Generic"].toMap();
    ui->chkAdviseWindows->setChecked(mapGeneric["AdviseWindows"].toBool());
    ui->chkMaxBIOSVersion->setChecked(mapGeneric["MaxBIOSVersion"].toBool());

    ui->editMLB->setText(mapGeneric["MLB"].toString());
    if (ui->editMLB_2->text().trimmed() == "")
        ui->editMLB_2->setText(mapGeneric["MLB"].toString());

    QByteArray ba = mapGeneric["ROM"].toByteArray();
    QString va = ba.toHex().toUpper();
    ui->editROM->setText(va);
    if (ui->editROM_2->text().trimmed() == "")
        ui->editROM_2->setText(va);

    ui->chkSpoofVendor->setChecked(mapGeneric["SpoofVendor"].toBool());

    ui->editProcessorTypeGeneric->setText(mapGeneric["ProcessorType"].toString());

    ui->cboxSystemMemoryStatus->setCurrentText(
        mapGeneric["SystemMemoryStatus"].toString().trimmed());

    //机型
    QString spn = mapGeneric["SystemProductName"].toString();
    ui->cboxSystemProductName->setCurrentText(spn);
    for (int i = 0; i < ui->cboxSystemProductName->count(); i++) {
        if (getSystemProductName(ui->cboxSystemProductName->itemText(i)) == spn) {
            ui->cboxSystemProductName->setCurrentIndex(i);

            break;
        }
    }

    if (ui->editSystemProductName->text().trimmed() == "")
        ui->editSystemProductName->setText(
            mapGeneric["SystemProductName"].toString());
    if (ui->editSystemProductName_2->text().trimmed() == "")
        ui->editSystemProductName_2->setText(
            mapGeneric["SystemProductName"].toString());

    ui->editSystemSerialNumber->setText(
        mapGeneric["SystemSerialNumber"].toString());
    if (ui->editSystemSerialNumber_data->text().trimmed() == "")
        ui->editSystemSerialNumber_data->setText(
            mapGeneric["SystemSerialNumber"].toString());
    if (ui->editSystemSerialNumber_2->text().trimmed() == "")
        ui->editSystemSerialNumber_2->setText(
            mapGeneric["SystemSerialNumber"].toString());

    ui->editSystemUUID->setText(mapGeneric["SystemUUID"].toString());
    if (ui->editSystemUUID_data->text().trimmed() == "")
        ui->editSystemUUID_data->setText(mapGeneric["SystemUUID"].toString());
    if (ui->editSystemUUID_2->text().trimmed() == "")
        ui->editSystemUUID_2->setText(mapGeneric["SystemUUID"].toString());

    // DataHub
    QVariantMap mapDataHub = map["DataHub"].toMap();
    ui->editARTFrequency->setText(mapDataHub["ARTFrequency"].toString());
    ui->editBoardProduct->setText(mapDataHub["BoardProduct"].toString());
    ui->editBoardRevision->setText(
        ByteToHexStr(mapDataHub["BoardRevision"].toByteArray()));
    ui->editDevicePathsSupported->setText(
        mapDataHub["DevicePathsSupported"].toString());
    ui->editFSBFrequency->setText(mapDataHub["FSBFrequency"].toString());
    ui->editInitialTSC->setText(mapDataHub["InitialTSC"].toString());
    ui->editPlatformName->setText(mapDataHub["PlatformName"].toString());
    ui->editSmcBranch->setText(
        ByteToHexStr(mapDataHub["SmcBranch"].toByteArray()));
    ui->editSmcPlatform->setText(
        ByteToHexStr(mapDataHub["SmcPlatform"].toByteArray()));
    ui->editSmcRevision->setText(
        ByteToHexStr(mapDataHub["SmcRevision"].toByteArray()));
    ui->editStartupPowerEvents->setText(
        mapDataHub["StartupPowerEvents"].toString());

    ui->editSystemProductName->setText(
        mapDataHub["SystemProductName"].toString());
    if (ui->cboxSystemProductName->currentText() == "") {
        for (int i = 0; i < ui->cboxSystemProductName->count(); i++) {
            if (getSystemProductName(ui->cboxSystemProductName->itemText(i)) == mapDataHub["SystemProductName"].toString()) {
                ui->cboxSystemProductName->setCurrentIndex(i);
                break;
            }
        }
    }

    ui->editSystemSerialNumber_data->setText(
        mapDataHub["SystemSerialNumber"].toString());
    if (ui->editSystemSerialNumber->text().trimmed() == "")
        ui->editSystemSerialNumber->setText(
            mapDataHub["SystemSerialNumber"].toString());

    ui->editSystemUUID_data->setText(mapDataHub["SystemUUID"].toString());
    if (ui->editSystemUUID->text().trimmed() == "")
        ui->editSystemUUID->setText(mapDataHub["SystemUUID"].toString());

    // Memory
    QVariantMap mapMemory = map["Memory"].toMap();
    ui->editDataWidth->setText(mapMemory["DataWidth"].toString());
    ui->editErrorCorrection->setText(mapMemory["ErrorCorrection"].toString());
    ui->editFormFactor->setText(mapMemory["FormFactor"].toString());
    ui->editMaxCapacity->setText(mapMemory["MaxCapacity"].toString());
    ui->editTotalWidth->setText(mapMemory["TotalWidth"].toString());
    ui->editType->setText(mapMemory["Type"].toString());
    ui->editTypeDetail->setText(mapMemory["TypeDetail"].toString());

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

    ui->editBID->setText(mapPlatformNVRAM["BID"].toString());

    ui->editMLB_2->setText(mapPlatformNVRAM["MLB"].toString());
    if (ui->editMLB->text().trimmed() == "")
        ui->editMLB->setText(mapPlatformNVRAM["MLB"].toString());

    ui->editFirmwareFeatures->setText(
        ByteToHexStr(mapPlatformNVRAM["FirmwareFeatures"].toByteArray()));
    ui->editFirmwareFeaturesMask->setText(
        ByteToHexStr(mapPlatformNVRAM["FirmwareFeaturesMask"].toByteArray()));

    ui->editROM_2->setText(ByteToHexStr(mapPlatformNVRAM["ROM"].toByteArray()));
    if (ui->editROM->text().trimmed() == "")
        ui->editROM->setText(ByteToHexStr(mapPlatformNVRAM["ROM"].toByteArray()));

    ui->editSystemUUID_PNVRAM->setText(mapPlatformNVRAM["SystemUUID"].toString());

    // SMBIOS
    QVariantMap mapSMBIOS = map["SMBIOS"].toMap();

    ui->editBIOSReleaseDate->setText(mapSMBIOS["BIOSReleaseDate"].toString());
    ui->editBIOSVendor->setText(mapSMBIOS["BIOSVendor"].toString());
    ui->editBIOSVersion->setText(mapSMBIOS["BIOSVersion"].toString());
    ui->editBoardAssetTag->setText(mapSMBIOS["BoardAssetTag"].toString());
    ui->editBoardLocationInChassis->setText(
        mapSMBIOS["BoardLocationInChassis"].toString());
    ui->editBoardManufacturer->setText(mapSMBIOS["BoardManufacturer"].toString());
    ui->editBoardProduct_2->setText(mapSMBIOS["BoardProduct"].toString());
    ui->editBoardSerialNumber->setText(mapSMBIOS["BoardSerialNumber"].toString());
    ui->editBoardType->setText(mapSMBIOS["BoardType"].toString());
    ui->editBoardVersion->setText(mapSMBIOS["BoardVersion"].toString());
    ui->editChassisAssetTag->setText(mapSMBIOS["ChassisAssetTag"].toString());
    ui->editChassisManufacturer->setText(
        mapSMBIOS["ChassisManufacturer"].toString());
    ui->editChassisSerialNumber->setText(
        mapSMBIOS["ChassisSerialNumber"].toString());
    ui->editChassisType->setText(mapSMBIOS["ChassisType"].toString());
    ui->editChassisVersion->setText(mapSMBIOS["ChassisVersion"].toString());
    ui->editFirmwareFeatures_2->setText(
        ByteToHexStr(mapSMBIOS["FirmwareFeatures"].toByteArray()));
    ui->editFirmwareFeaturesMask_2->setText(
        ByteToHexStr(mapSMBIOS["FirmwareFeaturesMask"].toByteArray()));
    // ui->editMemoryFormFactor->setText(mapSMBIOS["MemoryFormFactor"].toString());
    ui->editPlatformFeature->setText(mapSMBIOS["PlatformFeature"].toString());
    ui->editProcessorType->setText(mapSMBIOS["ProcessorType"].toString());
    ui->editSmcVersion->setText(
        ByteToHexStr(mapSMBIOS["SmcVersion"].toByteArray()));
    ui->editSystemFamily->setText(mapSMBIOS["SystemFamily"].toString());
    ui->editSystemManufacturer->setText(
        mapSMBIOS["SystemManufacturer"].toString());
    ui->editSystemProductName_2->setText(
        mapSMBIOS["SystemProductName"].toString());
    ui->editSystemSKUNumber->setText(mapSMBIOS["SystemSKUNumber"].toString());
    ui->editSystemSerialNumber_2->setText(
        mapSMBIOS["SystemSerialNumber"].toString());
    ui->editSystemUUID_2->setText(mapSMBIOS["SystemUUID"].toString());
    ui->editSystemVersion->setText(mapSMBIOS["SystemVersion"].toString());
}

void MainWindow::initui_UEFI()
{
    //Audio
    QStringList list;
    list.append("Auto");
    list.append("Enabled");
    list.append("Disabled");
    ui->cboxPlayChime->addItems(list);

    // Drivers
    QTableWidgetItem* id0;

    ui->table_uefi_drivers->setColumnWidth(0, 1000);
    id0 = new QTableWidgetItem(tr("Drivers"));
    ui->table_uefi_drivers->setHorizontalHeaderItem(0, id0);
    ui->table_uefi_drivers->setAlternatingRowColors(true);
    ui->table_uefi_drivers->horizontalHeader()->setStretchLastSection(true);

    // KeySupportMode
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

    ui->table_uefi_ReservedMemory->setColumnWidth(3, 210);
    id0 = new QTableWidgetItem(tr("Type"));
    ui->table_uefi_ReservedMemory->setHorizontalHeaderItem(3, id0);

    id0 = new QTableWidgetItem(tr("Enabled"));
    ui->table_uefi_ReservedMemory->setHorizontalHeaderItem(4, id0);

    ui->table_uefi_ReservedMemory->setAlternatingRowColors(true);
    //ui->table_uefi_ReservedMemory->horizontalHeader()->setStretchLastSection(true);
}

void MainWindow::ParserUEFI(QVariantMap map)
{
    map = map["UEFI"].toMap();
    if (map.isEmpty())
        return;

    //分析APFS
    QVariantMap map_apfs = map["APFS"].toMap();
    ui->chkEnableJumpstart->setChecked(map_apfs["EnableJumpstart"].toBool());
    ui->chkGlobalConnect->setChecked(map_apfs["GlobalConnect"].toBool());
    ui->chkHideVerbose->setChecked(map_apfs["HideVerbose"].toBool());
    ui->chkJumpstartHotPlug->setChecked(map_apfs["JumpstartHotPlug"].toBool());
    ui->editMinDate->setText(map_apfs["MinDate"].toString());
    ui->editMinVersion->setText(map_apfs["MinVersion"].toString());

    //分析Audio
    QVariantMap map_audio = map["Audio"].toMap();
    ui->chkAudioSupport->setChecked(map_audio["AudioSupport"].toBool());

    QString strPlayChime = map_audio["PlayChime"].toString();
    if (strPlayChime == "true" || strPlayChime == "false")
        ui->cboxPlayChime->setCurrentText("Auto");
    else
        ui->cboxPlayChime->setCurrentText(map_audio["PlayChime"].toString());

    ui->editAudioCodec->setText(map_audio["AudioCodec"].toString());
    ui->editAudioDevice->setText(map_audio["AudioDevice"].toString());
    ui->editAudioOut->setText(map_audio["AudioOut"].toString());
    ui->editMinimumVolume->setText(map_audio["MinimumVolume"].toString());
    ui->editSetupDelay->setText(map_audio["SetupDelay"].toString());
    ui->editVolumeAmplifier->setText(map_audio["VolumeAmplifier"].toString());

    // Drivers
    QTableWidgetItem* id0;
    QVariantList map_Drivers = map["Drivers"].toList(); //数组
    ui->table_uefi_drivers->setRowCount(map_Drivers.count());
    for (int i = 0; i < map_Drivers.count(); i++) {

        id0 = new QTableWidgetItem(map_Drivers.at(i).toString());
        ui->table_uefi_drivers->setItem(i, 0, id0);
    }

    ui->chkConnectDrivers->setChecked(map["ConnectDrivers"].toBool());

    //分析Input
    QVariantMap map_input = map["Input"].toMap();
    ui->chkKeyFiltering->setChecked(map_input["KeyFiltering"].toBool());
    ui->chkKeySupport->setChecked(map_input["KeySupport"].toBool());
    ui->chkKeySwap->setChecked(map_input["KeySwap"].toBool());
    ui->chkPointerSupport->setChecked(map_input["PointerSupport"].toBool());

    ui->editKeyForgetThreshold->setText(
        map_input["KeyForgetThreshold"].toString());
    ui->editKeyMergeThreshold->setText(map_input["KeyMergeThreshold"].toString());
    ui->editPointerSupportMode->setText(
        map_input["PointerSupportMode"].toString());
    ui->editTimerResolution->setText(map_input["TimerResolution"].toString());

    QString ksm = map_input["KeySupportMode"].toString();
    ui->cboxKeySupportMode->setCurrentText(ksm.trimmed());

    // Output
    QVariantMap map_output = map["Output"].toMap();
    ui->chkClearScreenOnModeSwitch->setChecked(
        map_output["ClearScreenOnModeSwitch"].toBool());
    ui->chkDirectGopRendering->setChecked(
        map_output["DirectGopRendering"].toBool());
    ui->chkIgnoreTextInGraphics->setChecked(
        map_output["IgnoreTextInGraphics"].toBool());
    ui->chkProvideConsoleGop->setChecked(
        map_output["ProvideConsoleGop"].toBool());
    ui->chkReconnectOnResChange->setChecked(
        map_output["ReconnectOnResChange"].toBool());
    ui->chkReplaceTabWithSpace->setChecked(
        map_output["ReplaceTabWithSpace"].toBool());
    ui->chkSanitiseClearScreen->setChecked(
        map_output["SanitiseClearScreen"].toBool());
    ui->chkUgaPassThrough->setChecked(map_output["UgaPassThrough"].toBool());
    ui->chkForceResolution->setChecked(map_output["ForceResolution"].toBool());

    ui->cboxConsoleMode->setCurrentText(map_output["ConsoleMode"].toString());
    ui->cboxResolution->setCurrentText(map_output["Resolution"].toString());
    ui->cboxTextRenderer->setCurrentText(map_output["TextRenderer"].toString());

    // ProtocolOverrides
    QVariantMap map_po = map["ProtocolOverrides"].toMap();
    ui->chkAppleAudio->setChecked(map_po["AppleAudio"].toBool());
    ui->chkAppleBootPolicy->setChecked(map_po["AppleBootPolicy"].toBool());
    ui->chkAppleDebugLog->setChecked(map_po["AppleDebugLog"].toBool());
    ui->chkAppleEvent->setChecked(map_po["AppleEvent"].toBool());
    ui->chkAppleFramebufferInfo->setChecked(
        map_po["AppleFramebufferInfo"].toBool());
    ui->chkAppleImageConversion->setChecked(
        map_po["AppleImageConversion"].toBool());
    ui->chkAppleKeyMap->setChecked(map_po["AppleKeyMap"].toBool());
    ui->chkAppleRtcRam->setChecked(map_po["AppleRtcRam"].toBool());
    ui->chkAppleSmcIo->setChecked(map_po["AppleSmcIo"].toBool());
    ui->chkAppleUserInterfaceTheme->setChecked(
        map_po["AppleUserInterfaceTheme"].toBool());
    ui->chkDataHub->setChecked(map_po["DataHub"].toBool());
    ui->chkDeviceProperties->setChecked(map_po["DeviceProperties"].toBool());
    ui->chkFirmwareVolume->setChecked(map_po["FirmwareVolume"].toBool());
    ui->chkHashServices->setChecked(map_po["HashServices"].toBool());
    ui->chkOSInfo->setChecked(map_po["OSInfo"].toBool());
    ui->chkUnicodeCollation->setChecked(map_po["UnicodeCollation"].toBool());
    ui->chkAppleImg4Verification->setChecked(
        map_po["AppleImg4Verification"].toBool());
    ui->chkAppleSecureBoot->setChecked(map_po["AppleSecureBoot"].toBool());

    // Quirks
    QVariantMap map_uefi_Quirks = map["Quirks"].toMap();
    //ui->chkDeduplicateBootOrder->setChecked(
    //    map_uefi_Quirks["DeduplicateBootOrder"].toBool());
    ui->chkIgnoreInvalidFlexRatio->setChecked(
        map_uefi_Quirks["IgnoreInvalidFlexRatio"].toBool());
    ui->chkReleaseUsbOwnership->setChecked(
        map_uefi_Quirks["ReleaseUsbOwnership"].toBool());
    ui->chkRequestBootVarRouting->setChecked(
        map_uefi_Quirks["RequestBootVarRouting"].toBool());
    ui->chkUnblockFsConnect->setChecked(
        map_uefi_Quirks["UnblockFsConnect"].toBool());

    ui->editExitBootServicesDelay->setText(
        map_uefi_Quirks["ExitBootServicesDelay"].toString());
    ui->editTscSyncTimeout->setText(map_uefi_Quirks["TscSyncTimeout"].toString());

    // ReservedMemory
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

        newItem1 = new QTableWidgetItem(map_sub["Type"].toString());
        ui->table_uefi_ReservedMemory->setItem(i, 3, newItem1);

        init_enabled_data(ui->table_uefi_ReservedMemory, i, 4,
            map_sub["Enabled"].toString());
    }
}

void MainWindow::on_btnSave()
{
    SavePlist(SaveFileName);
}

bool MainWindow::getBool(QTableWidget* table, int row, int column)
{
    QString be = table->item(row, column)->text();
    if (be.trimmed() == "true")
        return true;
    if (be.trimmed() == "false")
        return false;

    return false;
}

void MainWindow::SavePlist(QString FileName)
{

    QVariantMap OpenCore;

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
}

QVariantMap MainWindow::SaveACPI()
{
    // ACPI

    // Add
    QVariantMap acpiMap;

    QVariantList acpiAdd;
    QVariantMap acpiAddSub;

    for (int i = 0; i < ui->table_acpi_add->rowCount(); i++) {
        acpiAddSub["Path"] = ui->table_acpi_add->item(i, 0)->text();
        acpiAddSub["Comment"] = ui->table_acpi_add->item(i, 1)->text();
        acpiAddSub["Enabled"] = getBool(ui->table_acpi_add, i, 2);

        acpiAdd.append(acpiAddSub); //最后一层
    }

    acpiMap["Add"] = acpiAdd; //第二层

    // Delete

    QVariantList acpiDel;
    QVariantMap acpiDelSub;
    QString str;

    for (int i = 0; i < ui->table_acpi_del->rowCount(); i++) {

        str = ui->table_acpi_del->item(i, 0)->text();
        acpiDelSub["TableSignature"] = HexStrToByte(str);

        str = ui->table_acpi_del->item(i, 1)->text();
        acpiDelSub["OemTableId"] = HexStrToByte(str);

        acpiDelSub["TableLength"] = ui->table_acpi_del->item(i, 2)->text().toLongLong();
        acpiDelSub["Comment"] = ui->table_acpi_del->item(i, 3)->text();
        acpiDelSub["All"] = getBool(ui->table_acpi_del, i, 4);
        acpiDelSub["Enabled"] = getBool(ui->table_acpi_del, i, 5);

        acpiDel.append(acpiDelSub); //最后一层
    }

    acpiMap["Delete"] = acpiDel; //第二层

    // Patch

    QVariantList acpiPatch;
    QVariantMap acpiPatchSub;

    for (int i = 0; i < ui->table_acpi_patch->rowCount(); i++) {

        str = ui->table_acpi_patch->item(i, 0)->text();
        acpiPatchSub["TableSignature"] = HexStrToByte(str);

        str = ui->table_acpi_patch->item(i, 1)->text();
        acpiPatchSub["OemTableId"] = HexStrToByte(str);

        acpiPatchSub["TableLength"] = ui->table_acpi_patch->item(i, 2)->text().toLongLong();
        acpiPatchSub["Find"] = HexStrToByte(ui->table_acpi_patch->item(i, 3)->text());
        acpiPatchSub["Replace"] = HexStrToByte(ui->table_acpi_patch->item(i, 4)->text());
        acpiPatchSub["Comment"] = ui->table_acpi_patch->item(i, 5)->text();
        acpiPatchSub["Mask"] = ui->table_acpi_patch->item(i, 6)->text().toLatin1();
        acpiPatchSub["ReplaceMask"] = ui->table_acpi_patch->item(i, 7)->text().toLatin1();
        acpiPatchSub["Count"] = ui->table_acpi_patch->item(i, 8)->text().toLongLong();
        acpiPatchSub["Limit"] = ui->table_acpi_patch->item(i, 9)->text().toLongLong();
        acpiPatchSub["Skip"] = ui->table_acpi_patch->item(i, 10)->text().toLongLong();
        acpiPatchSub["Enabled"] = getBool(ui->table_acpi_patch, i, 11);

        acpiPatch.append(acpiPatchSub); //最后一层
    }

    acpiMap["Patch"] = acpiPatch; //第二层

    // Quirks
    QVariantMap acpiQuirks;

    acpiQuirks["FadtEnableReset"] = getChkBool(ui->chkFadtEnableReset);
    acpiQuirks["NormalizeHeaders"] = getChkBool(ui->chkNormalizeHeaders);
    acpiQuirks["RebaseRegions"] = getChkBool(ui->chkRebaseRegions);
    acpiQuirks["ResetHwSig"] = getChkBool(ui->chkResetHwSig);
    acpiQuirks["ResetLogoStatus"] = getChkBool(ui->chkResetLogoStatus);

    acpiMap["Quirks"] = acpiQuirks;

    return acpiMap;
}

QVariantMap MainWindow::SaveBooter()
{

    QVariantMap subMap;
    QVariantList arrayList;
    QVariantMap valueList;

    // Patch
    arrayList.clear();
    valueList.clear();
    for (int i = 0; i < ui->table_Booter_patch->rowCount(); i++) {
        valueList["Identifier"] = ui->table_Booter_patch->item(i, 0)->text();

        valueList["Comment"] = ui->table_Booter_patch->item(i, 1)->text();
        valueList["Find"] = HexStrToByte(ui->table_Booter_patch->item(i, 2)->text());
        valueList["Replace"] = HexStrToByte(ui->table_Booter_patch->item(i, 3)->text());
        valueList["Mask"] = HexStrToByte(ui->table_Booter_patch->item(i, 4)->text());
        valueList["ReplaceMask"] = HexStrToByte(ui->table_Booter_patch->item(i, 5)->text());

        valueList["Count"] = ui->table_Booter_patch->item(i, 6)->text().toLongLong();
        valueList["Limit"] = ui->table_Booter_patch->item(i, 7)->text().toLongLong();
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

        valueList["Comment"] = ui->table_booter->item(i, 1)->text();

        valueList["Enabled"] = getBool(ui->table_booter, i, 2);

        arrayList.append(valueList); //最后一层
    }

    subMap["MmioWhitelist"] = arrayList; //第二层

    // Quirks
    QVariantMap mapQuirks;
    mapQuirks["AvoidRuntimeDefrag"] = getChkBool(ui->chkAvoidRuntimeDefrag);
    mapQuirks["AllowRelocationBlock"] = getChkBool(ui->chkAllowRelocationBlock);
    mapQuirks["DevirtualiseMmio"] = getChkBool(ui->chkDevirtualiseMmio);
    mapQuirks["DisableSingleUser"] = getChkBool(ui->chkDisableSingleUser);
    mapQuirks["DisableVariableWrite"] = getChkBool(ui->chkDisableVariableWrite);
    mapQuirks["DiscardHibernateMap"] = getChkBool(ui->chkDiscardHibernateMap);
    mapQuirks["EnableSafeModeSlide"] = getChkBool(ui->chkEnableSafeModeSlide);
    mapQuirks["EnableWriteUnprotector"] = getChkBool(ui->chkEnableWriteUnprotector);
    mapQuirks["ForceExitBootServices"] = getChkBool(ui->chkForceExitBootServices);
    mapQuirks["ProtectMemoryRegions"] = getChkBool(ui->chkProtectMemoryRegions);
    mapQuirks["ProtectSecureBoot"] = getChkBool(ui->chkProtectSecureBoot);
    mapQuirks["ProtectUefiServices"] = getChkBool(ui->chkProtectUefiServices);
    mapQuirks["ProvideCustomSlide"] = getChkBool(ui->chkProvideCustomSlide);
    mapQuirks["ProvideMaxSlide"] = ui->editProvideMaxSlide->text().toLongLong();
    mapQuirks["RebuildAppleMemoryMap"] = getChkBool(ui->chkRebuildAppleMemoryMap);
    mapQuirks["SetupVirtualMap"] = getChkBool(ui->chkSetupVirtualMap);
    mapQuirks["SignalAppleOS"] = getChkBool(ui->chkSignalAppleOS);
    mapQuirks["SyncRuntimePermissions"] = getChkBool(ui->chkSyncRuntimePermissions);

    subMap["Quirks"] = mapQuirks;

    return subMap;
}

QVariantMap MainWindow::SaveDeviceProperties()
{
    // Add
    QVariantMap subMap;
    QVariantMap dictList;
    QVariantMap valueList;
    QVariantList arrayList;
    for (int i = 0; i < ui->table_dp_add0->rowCount(); i++) {

        valueList.clear(); //先必须清理下列表，很重要
        //先加载表中的数据
        ui->table_dp_add0->setCurrentCell(i, 0);
        on_table_dp_add0_cellClicked(i, 0);

        for (int k = 0; k < ui->table_dp_add->rowCount(); k++) {
            QString dataType = ui->table_dp_add->item(k, 1)->text(); //数据类型
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
    subMap["Add"] = dictList;

    // Delete
    dictList.clear(); //先清理之前的数据
    for (int i = 0; i < ui->table_dp_del0->rowCount(); i++) {

        valueList.clear(); //先必须清理下列表，很重要
        arrayList.clear();

        //先加载表中的数据
        ui->table_dp_del0->setCurrentCell(i, 0);
        on_table_dp_del0_cellClicked(i, 0);

        for (int k = 0; k < ui->table_dp_del->rowCount(); k++) {
            arrayList.append(ui->table_dp_del->item(k, 0)->text());
        }
        dictList[ui->table_dp_del0->item(i, 0)->text()] = arrayList;
    }
    subMap["Delete"] = dictList;

    return subMap;
}

QVariantMap MainWindow::SaveKernel()
{
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
    dictList.clear(); //必须先清理之前的数据，否则之前的数据会加到当前数据里面来
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
        valueList["Find"] = HexStrToByte(ui->table_kernel_patch->item(i, 3)->text());
        valueList["Replace"] = HexStrToByte(ui->table_kernel_patch->item(i, 4)->text());
        valueList["Mask"] = HexStrToByte(ui->table_kernel_patch->item(i, 5)->text());
        valueList["ReplaceMask"] = HexStrToByte(ui->table_kernel_patch->item(i, 6)->text());
        valueList["MinKernel"] = ui->table_kernel_patch->item(i, 7)->text();
        valueList["MaxKernel"] = ui->table_kernel_patch->item(i, 8)->text();
        valueList["Count"] = ui->table_kernel_patch->item(i, 9)->text().toLongLong();
        valueList["Limit"] = ui->table_kernel_patch->item(i, 10)->text().toLongLong();
        valueList["Skip"] = ui->table_kernel_patch->item(i, 11)->text().toLongLong();
        valueList["Enabled"] = getBool(ui->table_kernel_patch, i, 12);
        valueList["Arch"] = ui->table_kernel_patch->item(i, 13)->text();

        dictList.append(valueList);
    }

    subMap["Patch"] = dictList;

    // Emulate
    QVariantMap mapValue;
    mapValue["Cpuid1Data"] = HexStrToByte(ui->editCpuid1Data->text());
    mapValue["Cpuid1Mask"] = HexStrToByte(ui->editCpuid1Mask->text());
    mapValue["DummyPowerManagement"] = getChkBool(ui->chkDummyPowerManagement);
    mapValue["MaxKernel"] = ui->editMaxKernel->text().trimmed();
    mapValue["MinKernel"] = ui->editMinKernel->text().trimmed();

    subMap["Emulate"] = mapValue;

    // Quirks
    QVariantMap mapQuirks;
    mapQuirks["AppleCpuPmCfgLock"] = getChkBool(ui->chkAppleCpuPmCfgLock);
    mapQuirks["AppleXcpmCfgLock"] = getChkBool(ui->chkAppleXcpmCfgLock);
    mapQuirks["AppleXcpmExtraMsrs"] = getChkBool(ui->chkAppleXcpmExtraMsrs);
    mapQuirks["AppleXcpmForceBoost"] = getChkBool(ui->chkAppleXcpmForceBoost);
    mapQuirks["CustomSMBIOSGuid"] = getChkBool(ui->chkCustomSMBIOSGuid);
    mapQuirks["DisableIoMapper"] = getChkBool(ui->chkDisableIoMapper);
    mapQuirks["DisableRtcChecksum"] = getChkBool(ui->chkDisableRtcChecksum);
    mapQuirks["ExternalDiskIcons"] = getChkBool(ui->chkExternalDiskIcons);
    mapQuirks["IncreasePciBarSize"] = getChkBool(ui->chkIncreasePciBarSize);
    mapQuirks["LapicKernelPanic"] = getChkBool(ui->chkLapicKernelPanic);
    mapQuirks["PanicNoKextDump"] = getChkBool(ui->chkPanicNoKextDump);
    mapQuirks["PowerTimeoutKernelPanic"] = getChkBool(ui->chkPowerTimeoutKernelPanic);
    mapQuirks["ThirdPartyDrives"] = getChkBool(ui->chkThirdPartyDrives);
    mapQuirks["XhciPortLimit"] = getChkBool(ui->chkXhciPortLimit);
    mapQuirks["DisableLinkeditJettison"] = getChkBool(ui->chkDisableLinkeditJettison);

    mapQuirks["LegacyCommpage"] = getChkBool(ui->chkLegacyCommpage);
    mapQuirks["ExtendBTFeatureFlags"] = getChkBool(ui->chkExtendBTFeatureFlags);

    mapQuirks["ForceSecureBootScheme"] = getChkBool(ui->chkForceSecureBootScheme);

    subMap["Quirks"] = mapQuirks;

    // Scheme
    QVariantMap mapScheme;
    mapScheme["FuzzyMatch"] = getChkBool(ui->chkFuzzyMatch);
    mapScheme["KernelArch"] = ui->cboxKernelArch->currentText();
    mapScheme["KernelCache"] = ui->cboxKernelCache->currentText();
    subMap["Scheme"] = mapScheme;

    return subMap;
}

QVariantMap MainWindow::SaveMisc()
{
    QVariantMap subMap;
    QVariantList dictList;
    QVariantMap valueList;

    // BlessOverride
    for (int i = 0; i < ui->tableBlessOverride->rowCount(); i++) {
        dictList.append(ui->tableBlessOverride->item(i, 0)->text());
    }
    subMap["BlessOverride"] = dictList;

    // Boot
    valueList["ConsoleAttributes"] = ui->editConsoleAttributes->text().toLongLong();
    valueList["HibernateMode"] = ui->cboxHibernateMode->currentText();
    valueList["HideAuxiliary"] = getChkBool(ui->chkHideAuxiliary);
    valueList["PickerAttributes"] = ui->editPickerAttributes->text().toLongLong();
    valueList["PickerAudioAssist"] = getChkBool(ui->chkPickerAudioAssist);
    valueList["PickerMode"] = ui->cboxPickerMode->currentText();
    valueList["PickerVariant"] = ui->cboxPickerVariant->currentText();
    valueList["PollAppleHotKeys"] = getChkBool(ui->chkPollAppleHotKeys);
    valueList["ShowPicker"] = getChkBool(ui->chkShowPicker);
    valueList["TakeoffDelay"] = ui->editTakeoffDelay->text().toLongLong();
    valueList["Timeout"] = ui->editTimeout->text().toLongLong();

    subMap["Boot"] = valueList;

    // Debug
    valueList.clear();

    valueList["AppleDebug"] = getChkBool(ui->chkAppleDebug);
    valueList["ApplePanic"] = getChkBool(ui->chkApplePanic);
    valueList["DisableWatchDog"] = getChkBool(ui->chkDisableWatchDog);
    valueList["DisplayDelay"] = ui->editDisplayDelay->text().toLongLong();
    valueList["DisplayLevel"] = ui->editDisplayLevel->text().toLongLong();
    valueList["SerialInit"] = getChkBool(ui->chkSerialInit);
    valueList["SysReport"] = getChkBool(ui->chkSysReport);
    valueList["Target"] = ui->editTarget->text().toLongLong();

    subMap["Debug"] = valueList;

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

        dictList.append(valueList);
    }
    subMap["Entries"] = dictList;

    // Security
    valueList.clear();

    valueList["AllowNvramReset"] = getChkBool(ui->chkAllowNvramReset);
    valueList["AllowSetDefault"] = getChkBool(ui->chkAllowSetDefault);
    valueList["AuthRestart"] = getChkBool(ui->chkAuthRestart);
    valueList["BlacklistAppleUpdate"] = getChkBool(ui->chkBlacklistAppleUpdate);
    valueList["EnablePassword"] = getChkBool(ui->chkEnablePassword);

    valueList["BootProtect"] = ui->cboxBootProtect->currentText();
    valueList["DmgLoading"] = ui->cboxDmgLoading->currentText();
    valueList["Vault"] = ui->cboxVault->currentText();
    valueList["PasswordHash"] = HexStrToByte(ui->editPasswordHash->text());
    valueList["PasswordSalt"] = HexStrToByte(ui->editPasswordSalt->text());

    valueList["ExposeSensitiveData"] = ui->editExposeSensitiveData->text().toLongLong();

    valueList["HaltLevel"] = ui->editHaltLevel->text().toLongLong(nullptr, 10);
    qDebug() << ui->editHaltLevel->text().toLongLong(nullptr, 16);

    valueList["ScanPolicy"] = ui->editScanPolicy->text().toLongLong();

    valueList["ApECID"] = ui->editApECID->text().toLongLong();

    QString hm = ui->cboxSecureBootModel->currentText().trimmed();
    if (hm == "")
        hm = "Disabled";
    valueList["SecureBootModel"] = hm;

    subMap["Security"] = valueList;

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

        dictList.append(valueList);
    }
    subMap["Tools"] = dictList;

    return subMap;
}

QVariantMap MainWindow::SaveNVRAM()
{
    // Add
    QVariantMap subMap;
    QVariantMap dictList;
    QVariantList arrayList;
    QVariantMap valueList;

    for (int i = 0; i < ui->table_nv_add0->rowCount(); i++) {

        valueList.clear(); //先必须清理下列表，很重要
        //先加载表中的数据
        ui->table_nv_add0->setCurrentCell(i, 0);
        on_table_nv_add0_cellClicked(i, 0);

        for (int k = 0; k < ui->table_nv_add->rowCount(); k++) {
            QString dataType = ui->table_nv_add->item(k, 1)->text(); //数据类型
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
    subMap["Add"] = dictList;

    // Delete
    dictList.clear(); //先清理之前的数据
    for (int i = 0; i < ui->table_nv_del0->rowCount(); i++) {

        valueList.clear(); //先必须清理下列表，很重要
        arrayList.clear();

        //先加载表中的数据
        ui->table_nv_del0->setCurrentCell(i, 0);
        on_table_nv_del0_cellClicked(i, 0);

        for (int k = 0; k < ui->table_nv_del->rowCount(); k++) {
            arrayList.append(ui->table_nv_del->item(k, 0)->text());
        }
        dictList[ui->table_nv_del0->item(i, 0)->text()] = arrayList;
    }
    subMap["Delete"] = dictList;

    // LegacySchema
    dictList.clear(); //先清理之前的数据
    for (int i = 0; i < ui->table_nv_ls0->rowCount(); i++) {

        valueList.clear(); //先必须清理下列表，很重要
        arrayList.clear();

        //先加载表中的数据
        ui->table_nv_ls0->setCurrentCell(i, 0);
        on_table_nv_ls0_cellClicked(i, 0);

        for (int k = 0; k < ui->table_nv_ls->rowCount(); k++) {
            arrayList.append(ui->table_nv_ls->item(k, 0)->text());
        }
        dictList[ui->table_nv_ls0->item(i, 0)->text()] = arrayList;
    }
    subMap["LegacySchema"] = dictList;

    subMap["LegacyEnable"] = getChkBool(ui->chkLegacyEnable);
    subMap["LegacyOverwrite"] = getChkBool(ui->chkLegacyOverwrite);
    subMap["WriteFlash"] = getChkBool(ui->chkWriteFlash);

    return subMap;
}

QVariantMap MainWindow::SavePlatformInfo()
{
    QVariantMap subMap;
    QVariantMap valueList;

    // DataHub
    valueList.clear();
    valueList["ARTFrequency"] = ui->editARTFrequency->text().toLongLong();
    valueList["BoardProduct"] = ui->editBoardProduct->text();
    valueList["BoardRevision"] = HexStrToByte(ui->editBoardRevision->text());
    valueList["DevicePathsSupported"] = ui->editDevicePathsSupported->text().toLongLong();
    valueList["FSBFrequency"] = ui->editFSBFrequency->text().toLongLong();
    valueList["InitialTSC"] = ui->editInitialTSC->text().toLongLong();
    valueList["PlatformName"] = ui->editPlatformName->text();
    valueList["SmcBranch"] = HexStrToByte(ui->editSmcBranch->text());
    valueList["SmcPlatform"] = HexStrToByte(ui->editSmcPlatform->text());
    valueList["SmcRevision"] = HexStrToByte(ui->editSmcRevision->text());
    valueList["StartupPowerEvents"] = ui->editStartupPowerEvents->text().toLongLong();
    valueList["SystemProductName"] = ui->editSystemProductName->text();
    valueList["SystemSerialNumber"] = ui->editSystemSerialNumber_data->text();
    valueList["SystemUUID"] = ui->editSystemUUID_data->text();

    if (ui->chkSaveDataHub->isChecked() || !ui->chkAutomatic->isChecked())
        subMap["DataHub"] = valueList;

    // Memory
    valueList.clear();
    valueList["DataWidth"] = ui->editDataWidth->text().toLongLong();
    valueList["ErrorCorrection"] = ui->editErrorCorrection->text().toLongLong();
    valueList["FormFactor"] = ui->editFormFactor->text().toLongLong();
    valueList["MaxCapacity"] = ui->editMaxCapacity->text().toLongLong();
    valueList["TotalWidth"] = ui->editTotalWidth->text().toLongLong();
    valueList["Type"] = ui->editType->text().toLongLong();
    valueList["TypeDetail"] = ui->editTypeDetail->text().toLongLong();

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

        Array.append(AddSub); //最后一层
    }

    Map["Devices"] = Array; //第二层

    // if(ui->tableDevices->rowCount() > 0) //里面有数据才进行保存的动作
    //{
    valueList["Devices"] = Map["Devices"];
    //}

    if (ui->chkCustomMemory->isChecked()) {
        if (ui->chkSaveDataHub->isChecked() || !ui->chkAutomatic->isChecked())
            subMap["Memory"] = valueList;
    }

    // Generic
    valueList.clear();
    valueList["AdviseWindows"] = getChkBool(ui->chkAdviseWindows);
    valueList["MaxBIOSVersion"] = getChkBool(ui->chkMaxBIOSVersion);
    valueList["MLB"] = ui->editMLB->text();
    valueList["SystemMemoryStatus"] = ui->cboxSystemMemoryStatus->currentText().trimmed();
    valueList["ProcessorType"] = ui->editProcessorTypeGeneric->text().toLongLong();
    valueList["ROM"] = HexStrToByte(ui->editROM->text());
    valueList["SpoofVendor"] = getChkBool(ui->chkSpoofVendor);

    if (getSystemProductName(ui->cboxSystemProductName->currentText()) != "")
        valueList["SystemProductName"] = getSystemProductName(ui->cboxSystemProductName->currentText());
    else
        valueList["SystemProductName"] = ui->cboxSystemProductName->currentText();

    valueList["SystemSerialNumber"] = ui->editSystemSerialNumber->text();
    valueList["SystemUUID"] = ui->editSystemUUID->text();

    subMap["Generic"] = valueList;

    // PlatformNVRAM
    valueList.clear();
    valueList["BID"] = ui->editBID->text();
    valueList["MLB"] = ui->editMLB_2->text();

    valueList["FirmwareFeatures"] = HexStrToByte(ui->editFirmwareFeatures->text());
    valueList["FirmwareFeaturesMask"] = HexStrToByte(ui->editFirmwareFeaturesMask->text());
    valueList["ROM"] = HexStrToByte(ui->editROM_2->text());

    valueList["SystemUUID"] = ui->editSystemUUID_PNVRAM->text();

    if (ui->chkSaveDataHub->isChecked() || !ui->chkAutomatic->isChecked())
        subMap["PlatformNVRAM"] = valueList;

    // SMBIOS
    valueList.clear();
    valueList["BIOSReleaseDate"] = ui->editBIOSReleaseDate->text();
    valueList["BIOSVendor"] = ui->editBIOSVendor->text();
    valueList["BIOSVersion"] = ui->editBIOSVersion->text();
    valueList["BoardAssetTag"] = ui->editBoardAssetTag->text();
    valueList["BoardLocationInChassis"] = ui->editBoardLocationInChassis->text();
    valueList["BoardManufacturer"] = ui->editBoardManufacturer->text();
    valueList["BoardProduct"] = ui->editBoardProduct_2->text();
    valueList["BoardSerialNumber"] = ui->editBoardSerialNumber->text();
    valueList["BoardType"] = ui->editBoardType->text().toLongLong();
    valueList["BoardVersion"] = ui->editBoardVersion->text();
    valueList["ChassisAssetTag"] = ui->editChassisAssetTag->text();
    valueList["ChassisManufacturer"] = ui->editChassisManufacturer->text();
    valueList["ChassisSerialNumber"] = ui->editChassisSerialNumber->text();
    valueList["ChassisType"] = ui->editChassisType->text().toLongLong();
    valueList["ChassisVersion"] = ui->editChassisVersion->text();
    valueList["FirmwareFeatures"] = HexStrToByte(ui->editFirmwareFeatures_2->text());
    valueList["FirmwareFeaturesMask"] = HexStrToByte(ui->editFirmwareFeaturesMask_2->text());
    // valueList["MemoryFormFactor"] =
    // ui->editMemoryFormFactor->text().toLongLong();
    valueList["PlatformFeature"] = ui->editPlatformFeature->text().toLongLong();
    valueList["ProcessorType"] = ui->editProcessorType->text().toLongLong();
    valueList["SmcVersion"] = HexStrToByte(ui->editSmcVersion->text());
    valueList["SystemFamily"] = ui->editSystemFamily->text();
    valueList["SystemManufacturer"] = ui->editSystemManufacturer->text();
    valueList["SystemProductName"] = ui->editSystemProductName_2->text();
    valueList["SystemSKUNumber"] = ui->editSystemSKUNumber->text();
    valueList["SystemSerialNumber"] = ui->editSystemSerialNumber_2->text();
    valueList["SystemUUID"] = ui->editSystemUUID_2->text();
    valueList["SystemVersion"] = ui->editSystemVersion->text();

    if (ui->chkSaveDataHub->isChecked() || !ui->chkAutomatic->isChecked())
        subMap["SMBIOS"] = valueList;

    subMap["Automatic"] = getChkBool(ui->chkAutomatic);
    subMap["CustomMemory"] = getChkBool(ui->chkCustomMemory);
    subMap["UpdateDataHub"] = getChkBool(ui->chkUpdateDataHub);
    subMap["UpdateNVRAM"] = getChkBool(ui->chkUpdateNVRAM);
    subMap["UpdateSMBIOS"] = getChkBool(ui->chkUpdateSMBIOS);
    subMap["UpdateSMBIOSMode"] = ui->cboxUpdateSMBIOSMode->currentText();

    return subMap;
}

QVariantMap MainWindow::SaveUEFI()
{
    QVariantMap subMap;
    QVariantMap dictList;
    QVariantList arrayList;
    QVariantMap valueList;

    // APFS
    dictList["EnableJumpstart"] = getChkBool(ui->chkEnableJumpstart);
    dictList["GlobalConnect"] = getChkBool(ui->chkGlobalConnect);
    dictList["HideVerbose"] = getChkBool(ui->chkHideVerbose);
    dictList["JumpstartHotPlug"] = getChkBool(ui->chkJumpstartHotPlug);
    dictList["MinDate"] = ui->editMinDate->text().toLongLong();
    dictList["MinVersion"] = ui->editMinVersion->text().toLongLong();
    subMap["APFS"] = dictList;

    // Audio
    dictList.clear();
    dictList["AudioCodec"] = ui->editAudioCodec->text().toLongLong();
    dictList["AudioDevice"] = ui->editAudioDevice->text();
    dictList["AudioOut"] = ui->editAudioOut->text().toLongLong();
    dictList["AudioSupport"] = getChkBool(ui->chkAudioSupport);
    dictList["MinimumVolume"] = ui->editMinimumVolume->text().toLongLong();
    dictList["SetupDelay"] = ui->editSetupDelay->text().toLongLong();
    dictList["PlayChime"] = ui->cboxPlayChime->currentText();
    dictList["VolumeAmplifier"] = ui->editVolumeAmplifier->text().toLongLong();
    subMap["Audio"] = dictList;

    // Drivers
    arrayList.clear();
    for (int i = 0; i < ui->table_uefi_drivers->rowCount(); i++) {
        arrayList.append(ui->table_uefi_drivers->item(i, 0)->text());
    }
    subMap["Drivers"] = arrayList;
    subMap["ConnectDrivers"] = getChkBool(ui->chkConnectDrivers);

    // Input
    dictList.clear();
    dictList["KeyFiltering"] = getChkBool(ui->chkKeyFiltering);
    dictList["KeySupport"] = getChkBool(ui->chkKeySupport);
    dictList["KeySwap"] = getChkBool(ui->chkKeySwap);
    dictList["PointerSupport"] = getChkBool(ui->chkPointerSupport);

    dictList["KeyForgetThreshold"] = ui->editKeyForgetThreshold->text().toLongLong();
    dictList["KeyMergeThreshold"] = ui->editKeyMergeThreshold->text().toLongLong();
    dictList["PointerSupportMode"] = ui->editPointerSupportMode->text();
    dictList["TimerResolution"] = ui->editTimerResolution->text().toLongLong();

    dictList["KeySupportMode"] = ui->cboxKeySupportMode->currentText();

    subMap["Input"] = dictList;

    // Output
    dictList.clear();
    dictList["ClearScreenOnModeSwitch"] = getChkBool(ui->chkClearScreenOnModeSwitch);
    dictList["DirectGopRendering"] = getChkBool(ui->chkDirectGopRendering);
    dictList["IgnoreTextInGraphics"] = getChkBool(ui->chkIgnoreTextInGraphics);
    dictList["ProvideConsoleGop"] = getChkBool(ui->chkProvideConsoleGop);
    dictList["ReconnectOnResChange"] = getChkBool(ui->chkReconnectOnResChange);
    dictList["ReplaceTabWithSpace"] = getChkBool(ui->chkReplaceTabWithSpace);
    dictList["SanitiseClearScreen"] = getChkBool(ui->chkSanitiseClearScreen);
    dictList["UgaPassThrough"] = getChkBool(ui->chkUgaPassThrough);
    dictList["ForceResolution"] = getChkBool(ui->chkForceResolution);

    dictList["ConsoleMode"] = ui->cboxConsoleMode->currentText();
    dictList["Resolution"] = ui->cboxResolution->currentText();
    dictList["TextRenderer"] = ui->cboxTextRenderer->currentText();

    subMap["Output"] = dictList;

    // ProtocolOverrides
    dictList.clear();
    dictList["AppleAudio"] = getChkBool(ui->chkAppleAudio);
    dictList["AppleBootPolicy"] = getChkBool(ui->chkAppleBootPolicy);
    dictList["AppleDebugLog"] = getChkBool(ui->chkAppleDebugLog);
    dictList["AppleEvent"] = getChkBool(ui->chkAppleEvent);
    dictList["AppleFramebufferInfo"] = getChkBool(ui->chkAppleFramebufferInfo);
    dictList["AppleImageConversion"] = getChkBool(ui->chkAppleImageConversion);
    dictList["AppleKeyMap"] = getChkBool(ui->chkAppleKeyMap);
    dictList["AppleRtcRam"] = getChkBool(ui->chkAppleRtcRam);
    dictList["AppleSmcIo"] = getChkBool(ui->chkAppleSmcIo);
    dictList["AppleUserInterfaceTheme"] = getChkBool(ui->chkAppleUserInterfaceTheme);
    dictList["DataHub"] = getChkBool(ui->chkDataHub);
    dictList["DeviceProperties"] = getChkBool(ui->chkDeviceProperties);
    dictList["FirmwareVolume"] = getChkBool(ui->chkFirmwareVolume);
    dictList["HashServices"] = getChkBool(ui->chkHashServices);
    dictList["OSInfo"] = getChkBool(ui->chkOSInfo);
    dictList["UnicodeCollation"] = getChkBool(ui->chkUnicodeCollation);
    dictList["AppleImg4Verification"] = getChkBool(ui->chkAppleImg4Verification);
    dictList["AppleSecureBoot"] = getChkBool(ui->chkAppleSecureBoot);

    subMap["ProtocolOverrides"] = dictList;

    // Quirks
    dictList.clear();

    //dictList["DeduplicateBootOrder"] = getChkBool(ui->chkDeduplicateBootOrder);

    dictList["IgnoreInvalidFlexRatio"] = getChkBool(ui->chkIgnoreInvalidFlexRatio);
    dictList["ReleaseUsbOwnership"] = getChkBool(ui->chkReleaseUsbOwnership);
    dictList["RequestBootVarRouting"] = getChkBool(ui->chkRequestBootVarRouting);
    dictList["UnblockFsConnect"] = getChkBool(ui->chkUnblockFsConnect);

    dictList["ExitBootServicesDelay"] = ui->editExitBootServicesDelay->text().toLongLong();
    dictList["TscSyncTimeout"] = ui->editTscSyncTimeout->text().toLongLong();

    subMap["Quirks"] = dictList;

    // ReservedMemory
    arrayList.clear();
    valueList.clear();
    for (int i = 0; i < ui->table_uefi_ReservedMemory->rowCount(); i++) {
        valueList["Address"] = ui->table_uefi_ReservedMemory->item(i, 0)->text().toLongLong();
        valueList["Comment"] = ui->table_uefi_ReservedMemory->item(i, 1)->text();
        valueList["Size"] = ui->table_uefi_ReservedMemory->item(i, 2)->text().toLongLong();
        valueList["Type"] = ui->table_uefi_ReservedMemory->item(i, 3)->text();
        valueList["Enabled"] = getBool(ui->table_uefi_ReservedMemory, i, 4);

        arrayList.append(valueList);
    }
    subMap["ReservedMemory"] = arrayList;

    return subMap;
}

bool MainWindow::getChkBool(QCheckBox* chkbox)
{
    if (chkbox->isChecked())
        return true;
    else
        return false;

    return false;
}

QString MainWindow::ByteToHexStr(QByteArray ba)
{
    QString str = ba.toHex().toUpper();

    return str;
}

QByteArray MainWindow::HexStrToByte(QString value)
{
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

    /*QString c1, c2, c3, c4;
    c1 = value.mid(0, 2);
    c2 = value.mid(2, 2);
    c3 = value.mid(4, 2);
    c4 = value.mid(6, 2);

    ba.resize(4);
    ba[0] = c1.toUInt(nullptr, 16);
    ba[1] = c2.toUInt(nullptr, 16);
    ba[2] = c3.toUInt(nullptr, 16);
    ba[3] = c4.toUInt(nullptr, 16);*/

    return ba;
}

QByteArray MainWindow::HexStringToByteArray(QString HexString)
{
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

void MainWindow::on_table_acpi_add_cellClicked(int row, int column)
{
    if (!ui->table_acpi_add->currentIndex().isValid())
        return;

    enabled_change(ui->table_acpi_add, row, column, 2);

    ui->statusbar->showMessage(ui->table_acpi_add->currentItem()->text());
}

void MainWindow::init_enabled_data(QTableWidget* table, int row, int column,
    QString str)
{

    QTableWidgetItem* chkbox = new QTableWidgetItem(str);

    table->setItem(row, column, chkbox);
    table->item(row, column)->setTextAlignment(Qt::AlignCenter);
    if (str == "true")

        chkbox->setCheckState(Qt::Checked);
    else

        chkbox->setCheckState(Qt::Unchecked);
}

void MainWindow::enabled_change(QTableWidget* table, int row, int column, int cc)
{

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

void MainWindow::on_table_acpi_del_cellClicked(int row, int column)
{
    if (!ui->table_acpi_del->currentIndex().isValid())
        return;

    enabled_change(ui->table_acpi_del, row, column, 4);
    enabled_change(ui->table_acpi_del, row, column, 5);

    ui->statusbar->showMessage(ui->table_acpi_del->currentItem()->text());
}

void MainWindow::on_table_acpi_patch_cellClicked(int row, int column)
{
    if (!ui->table_acpi_patch->currentIndex().isValid())
        return;

    enabled_change(ui->table_acpi_patch, row, column, 11);

    ui->statusbar->showMessage(ui->table_acpi_patch->currentItem()->text());
}

void MainWindow::on_table_booter_cellClicked(int row, int column)
{
    if (!ui->table_booter->currentIndex().isValid())
        return;

    enabled_change(ui->table_booter, row, column, 2);

    ui->statusbar->showMessage(ui->table_booter->currentItem()->text());
}

void MainWindow::on_table_kernel_add_cellClicked(int row, int column)
{
    if (!ui->table_kernel_add->currentIndex().isValid())
        return;

    enabled_change(ui->table_kernel_add, row, column, 6);

    if (column == 7) {

        cboxArch = new QComboBox;
        cboxArch->addItem("Any");
        cboxArch->addItem("i386");
        cboxArch->addItem("x86_64");
        cboxArch->addItem("");

        connect(cboxArch, SIGNAL(currentIndexChanged(QString)), this,
            SLOT(arch_addChange()));
        c_row = row;

        ui->table_kernel_add->setCellWidget(row, column, cboxArch);
        cboxArch->setCurrentText(ui->table_kernel_add->item(row, 7)->text());
    }

    ui->statusbar->showMessage(ui->table_kernel_add->currentItem()->text());
}

void MainWindow::on_table_kernel_block_cellClicked(int row, int column)
{
    if (!ui->table_kernel_block->currentIndex().isValid())
        return;

    enabled_change(ui->table_kernel_block, row, column, 4);

    if (column == 5) {

        cboxArch = new QComboBox;
        cboxArch->addItem("Any");
        cboxArch->addItem("i386");
        cboxArch->addItem("x86_64");
        cboxArch->addItem("");

        connect(cboxArch, SIGNAL(currentIndexChanged(QString)), this,
            SLOT(arch_blockChange()));
        c_row = row;

        ui->table_kernel_block->setCellWidget(row, column, cboxArch);
        cboxArch->setCurrentText(ui->table_kernel_block->item(row, 5)->text());
    }

    ui->statusbar->showMessage(ui->table_kernel_block->currentItem()->text());
}

void MainWindow::on_table_kernel_patch_cellClicked(int row, int column)
{
    if (!ui->table_kernel_patch->currentIndex().isValid())
        return;

    enabled_change(ui->table_kernel_patch, row, column, 12);

    if (column == 13) {

        cboxArch = new QComboBox;
        cboxArch->addItem("Any");
        cboxArch->addItem("i386");
        cboxArch->addItem("x86_64");
        cboxArch->addItem("");

        connect(cboxArch, SIGNAL(currentIndexChanged(QString)), this,
            SLOT(arch_patchChange()));
        c_row = row;

        ui->table_kernel_patch->setCellWidget(row, column, cboxArch);
        cboxArch->setCurrentText(ui->table_kernel_patch->item(row, 13)->text());
    }

    ui->statusbar->showMessage(ui->table_kernel_patch->currentItem()->text());
}

void MainWindow::on_tableEntries_cellClicked(int row, int column)
{
    if (!ui->tableEntries->currentIndex().isValid())
        return;

    enabled_change(ui->tableEntries, row, column, 5);

    enabled_change(ui->tableEntries, row, column, 4);

    enabled_change(ui->tableEntries, row, column, 6);

    ui->statusbar->showMessage(ui->tableEntries->currentItem()->text());
}

void MainWindow::on_tableTools_cellClicked(int row, int column)
{
    if (!ui->tableTools->currentIndex().isValid())
        return;

    enabled_change(ui->tableTools, row, column, 5);

    enabled_change(ui->tableTools, row, column, 4);

    enabled_change(ui->tableTools, row, column, 6);

    enabled_change(ui->tableTools, row, column, 7);

    ui->statusbar->showMessage(ui->tableTools->currentItem()->text());
}

void MainWindow::on_table_uefi_ReservedMemory_cellClicked(int row, int column)
{

    if (!ui->table_uefi_ReservedMemory->currentIndex().isValid())
        return;

    enabled_change(ui->table_uefi_ReservedMemory, row, column, 4);

    if (column == 3) {

        cboxReservedMemoryType = new QComboBox(this);
        QStringList sl_type;
        sl_type.append("");
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

        connect(cboxReservedMemoryType, SIGNAL(currentIndexChanged(QString)), this,
            SLOT(ReservedMemoryTypeChange()));
        c_row = row;

        ui->table_uefi_ReservedMemory->setCellWidget(row, column,
            cboxReservedMemoryType);
        cboxReservedMemoryType->setCurrentText(
            ui->table_uefi_ReservedMemory->item(row, 3)->text());
    }

    ui->statusbar->showMessage(
        ui->table_uefi_ReservedMemory->currentItem()->text());
}

void MainWindow::on_btnKernelPatchAdd_clicked()
{
    add_item(ui->table_kernel_patch, 12);
    init_enabled_data(ui->table_kernel_patch,
        ui->table_kernel_patch->rowCount() - 1, 12, "true");

    QTableWidgetItem* newItem1 = new QTableWidgetItem("Any");
    newItem1->setTextAlignment(Qt::AlignCenter);
    ui->table_kernel_patch->setItem(ui->table_kernel_patch->currentRow(), 13,
        newItem1);

    this->setWindowModified(true);
}

void MainWindow::on_btnKernelPatchDel_clicked()
{
    del_item(ui->table_kernel_patch);
}

void MainWindow::on_btnQuickOpen1_clicked()
{

    QString FileName = QDir::homePath() + "/Sample.plist";

    QFile file(FileName);
    QVariantMap map = PListParser::parsePList(&file).toMap();

    loading = true;

    ParserACPI(map);
    ParserBooter(map);
    ParserDP(map);
    ParserKernel(map);
    ParserMisc(map);
    ParserNvram(map);
    ParserPlatformInfo(map);
    ParserUEFI(map);

    loading = false;
}

void MainWindow::on_btnQuickOpen2_clicked()
{
    QString FileName = QDir::homePath() + "/SampleFull.plist";

    QFile file(FileName);
    QVariantMap map = PListParser::parsePList(&file).toMap();

    loading = true;

    ParserACPI(map);
    ParserBooter(map);
    ParserDP(map);
    ParserKernel(map);
    ParserMisc(map);
    ParserNvram(map);
    ParserPlatformInfo(map);
    ParserUEFI(map);

    loading = false;
}

void MainWindow::add_item(QTableWidget* table, int total_column)
{
    int t = table->rowCount();
    table->setRowCount(t + 1);

    //用""初始化各项值
    for (int i = 0; i < total_column; i++) {
        table->setItem(t, i, new QTableWidgetItem(""));
    }
    table->setFocus();
    table->setCurrentCell(t, 0);
}

void MainWindow::del_item(QTableWidget* table)
{

    if (table->rowCount() == 0)
        return;

    int row = table->currentRow();

    //std::vector<int> vecItemIndex; //保存选中行的索引
    QItemSelectionModel* selections = table->selectionModel(); //返回当前的选择模式
    QModelIndexList selectedsList = selections->selectedIndexes(); //返回所有选定的模型项目索引列表

    for (int i = 0; i < selectedsList.count(); i++) {

        //vecItemIndex.push_back(selectedsList.at(i).row());
        int t = selectedsList.at(i).row();
        table->removeRow(t);

        selections = table->selectionModel();
        selectedsList = selections->selectedIndexes();

        i = -1;
        qDebug() << t;
    }

    if (row > table->rowCount()) {

        row = table->rowCount();
    }

    table->setFocus();
    table->setCurrentCell(row - 1, 0);
    if (row == 0)
        table->setCurrentCell(0, 0);

    this->setWindowModified(true);
}

void MainWindow::on_btnACPIAdd_Del_clicked()
{
    del_item(ui->table_acpi_add);
}

void MainWindow::on_btnACPIDel_Add_clicked()
{
    add_item(ui->table_acpi_del, 5);

    init_enabled_data(ui->table_acpi_del, ui->table_acpi_del->rowCount() - 1, 4,
        "false");
    init_enabled_data(ui->table_acpi_del, ui->table_acpi_del->rowCount() - 1, 5,
        "true");

    this->setWindowModified(true);
}

void MainWindow::on_btnACPIDel_Del_clicked()
{
    del_item(ui->table_acpi_del);
}

void MainWindow::on_btnACPIPatch_Add_clicked()
{
    add_item(ui->table_acpi_patch, 11);
    init_enabled_data(ui->table_acpi_patch, ui->table_acpi_patch->rowCount() - 1,
        11, "true");

    this->setWindowModified(true);
}

void MainWindow::on_btnACPIPatch_Del_clicked()
{
    del_item(ui->table_acpi_patch);
}

void MainWindow::on_btnBooter_Add_clicked()
{
    add_item(ui->table_booter, 2);
    init_enabled_data(ui->table_booter, ui->table_booter->rowCount() - 1, 2,
        "true");

    this->setWindowModified(true);
}

void MainWindow::on_btnBooter_Del_clicked()
{
    del_item(ui->table_booter);
}

void MainWindow::on_btnDPDel_Add0_clicked()
{
    add_item(ui->table_dp_del0, 1);
    ui->table_dp_del->setRowCount(0); //先清除右边表中的所有条目
    on_btnDPDel_Add_clicked(); //同时右边增加一个新条目

    write_value_ini(ui->table_dp_del0->objectName(), ui->table_dp_del,
        ui->table_dp_del0->rowCount() - 1);

    this->setWindowModified(true);
}

void MainWindow::on_btnDPDel_Del0_clicked()
{
    if (ui->table_dp_del0->rowCount() == 0)
        return;

    //先记住被删的条目位置
    int delindex = ui->table_dp_del0->currentRow();
    int count = ui->table_dp_del0->rowCount();

    QString qz = QDir::homePath() + "/.config/QtOCC/" + CurrentDateTime + ui->table_dp_del0->objectName();
    QFile file(qz + QString::number(delindex + 1) + ".ini");
    del_item(ui->table_dp_del0);
    if (file.exists())
        file.remove();

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

void MainWindow::on_btnDPDel_Add_clicked()
{
    if (ui->table_dp_del0->rowCount() == 0)
        return;

    loading = true;
    add_item(ui->table_dp_del, 1);
    loading = false;

    //保存数据
    write_value_ini(ui->table_dp_del0->objectName(), ui->table_dp_del,
        ui->table_dp_del0->currentRow());

    this->setWindowModified(true);
}

void MainWindow::on_btnDPDel_Del_clicked()
{
    del_item(ui->table_dp_del);

    //保存数据
    write_value_ini(ui->table_dp_del0->objectName(), ui->table_dp_del,
        ui->table_dp_del0->currentRow());
}

void MainWindow::on_btnACPIAdd_Add_clicked()
{
    QFileDialog fd;

    QStringList FileName = fd.getOpenFileNames(this, "file", "", "acpi file(*.aml);;all(*.*)");

    addACPIItem(FileName);
}

void MainWindow::addACPIItem(QStringList FileName)
{
    if (FileName.isEmpty())

        return;

    for (int i = 0; i < FileName.count(); i++) {
        int row = ui->table_acpi_add->rowCount() + 1;

        ui->table_acpi_add->setRowCount(row);
        ui->table_acpi_add->setItem(
            row - 1, 0, new QTableWidgetItem(QFileInfo(FileName.at(i)).fileName()));
        ui->table_acpi_add->setItem(row - 1, 1, new QTableWidgetItem(""));
        init_enabled_data(ui->table_acpi_add, row - 1, 2, "true");

        ui->table_acpi_add->setFocus();
        ui->table_acpi_add->setCurrentCell(row - 1, 0);
    }

    this->setWindowModified(true);
}

void MainWindow::on_btnDPAdd_Add0_clicked()
{
    add_item(ui->table_dp_add0, 1);
    ui->table_dp_add->setRowCount(0); //先清除右边表中的所有条目
    on_btnDPAdd_Add_clicked(); //同时右边增加一个新条目
    write_ini(ui->table_dp_add0->objectName(), ui->table_dp_add,
        ui->table_dp_add0->rowCount() - 1);

    this->setWindowModified(true);
}

void MainWindow::on_btnDPAdd_Del0_clicked()
{

    if (ui->table_dp_add0->rowCount() == 0)
        return;

    //先记住被删的条目位置
    int delindex = ui->table_dp_add0->currentRow();
    int count = ui->table_dp_add0->rowCount();

    QString qz = QDir::homePath() + "/.config/QtOCC/" + CurrentDateTime + ui->table_dp_add0->objectName();
    QFile file(qz + QString::number(delindex + 1) + ".ini");
    del_item(ui->table_dp_add0);
    if (file.exists())
        file.remove();

    //改名，以适应新的索引
    if (delindex < count) {
        for (int i = delindex; i < ui->table_dp_add0->rowCount(); i++) {
            QFile file(qz + QString::number(i + 2) + ".ini");
            file.rename(qz + QString::number(i + 1) + ".ini");
        }
    }

    if (ui->table_dp_add0->rowCount() == 0) {
        ui->table_dp_add->setRowCount(0);
    }

    if (ui->table_dp_add0->rowCount() > 0)
        on_table_dp_add0_cellClicked(ui->table_dp_add0->currentRow(), 0);

    this->setWindowModified(true);
}

void MainWindow::on_btnDPAdd_Add_clicked()
{
    if (ui->table_dp_add0->rowCount() == 0)
        return;

    loading = true;
    add_item(ui->table_dp_add, 3);
    loading = false;

    //保存数据
    write_ini(ui->table_dp_add0->objectName(), ui->table_dp_add,
        ui->table_dp_add0->currentRow());

    this->setWindowModified(true);
}

void MainWindow::on_btnDPAdd_Del_clicked()
{
    del_item(ui->table_dp_add);
    write_ini(ui->table_dp_add0->objectName(), ui->table_dp_add,
        ui->table_dp_add0->currentRow());
}

void MainWindow::on_btnKernelAdd_Add_clicked()
{
    QFileDialog fd;
    QStringList FileName;

#ifdef Q_OS_WIN32

    FileName.append(fd.getExistingDirectory());
    // qDebug() << FileName[0];
#endif

#ifdef Q_OS_LINUX

    FileName.append(fd.getExistingDirectory());
#endif

#ifdef Q_OS_MAC

    FileName = fd.getOpenFileNames(this, "kext", "",
        "kext(*.kext);;all(*.*)");
#endif
    //qDebug() << FileName[0];
    addKexts(FileName);
}

void MainWindow::addKexts(QStringList FileName)
{
    int file_count = FileName.count();

    if (file_count == 0 || FileName[0] == "")

        return;

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
        // qDebug() << filePath;
        QDir fileDir(filePath + "/" + fileInfo.fileName() + "/Contents/MacOS/");
        // qDebug() << fileDir;
        if (fileDir.exists()) //如果目录存在，则遍历里面的文件
        {
            fileDir.setFilter(QDir::Files); //只遍历本目录
            QFileInfoList fileList = fileDir.entryInfoList();
            int fileCount = fileList.count();
            for (int i = 0; i < fileCount; i++) //一般只有一个二进制文件
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
        t->setItem(
            row - 1, 2,
            new QTableWidgetItem("Contents/MacOS/" + fileInfoList.fileName()));
        t->setItem(row - 1, 3, new QTableWidgetItem("Contents/Info.plist"));
        t->setItem(row - 1, 4, new QTableWidgetItem(""));
        t->setItem(row - 1, 5, new QTableWidgetItem(""));
        init_enabled_data(t, row - 1, 6, "true");

        QTableWidgetItem* newItem1 = new QTableWidgetItem("x86_64");
        newItem1->setTextAlignment(Qt::AlignCenter);
        t->setItem(row - 1, 7, newItem1);

        //如果里面还有PlugIns目录，则需要继续遍历插件目录
        QDir piDir(filePath + "/" + fileInfo.fileName() + "/Contents/PlugIns/");
        // qDebug() << piDir;
        if (piDir.exists()) {

            piDir.setFilter(QDir::Dirs); //过滤器：只遍历里面的目录
            QFileInfoList fileList = piDir.entryInfoList();
            int fileCount = fileList.count();
            QVector<QString> kext_file;
            qDebug() << fileCount;
            for (int i = 0; i < fileCount; i++) //找出里面的kext文件(目录）
            {
                kext_file.push_back(fileList[i].fileName());
                qDebug() << kext_file.at(i);
            }

            if (fileCount >= 3) //里面有目录
            {
                for (int i = 0; i < fileCount - 2; i++) {
                    QDir fileDir(filePath + "/" + fileInfo.fileName() + "/Contents/PlugIns/" + kext_file[i + 2] + "/Contents/MacOS/");
                    if (fileDir.exists()) {
                        // qDebug() << fileDir;
                        fileDir.setFilter(QDir::Files); //只遍历本目录里面的文件
                        QFileInfoList fileList = fileDir.entryInfoList();
                        int fileCount = fileList.count();
                        for (int i = 0; i < fileCount; i++) //一般只有一个二进制文件
                        {
                            fileInfoList = fileList[i];
                        }

                        //写入到表里
                        int row = t->rowCount() + 1;

                        t->setRowCount(row);
                        t->setItem(row - 1, 0, new QTableWidgetItem(QFileInfo(FileName[j]).fileName() + "/Contents/PlugIns/" + kext_file[i + 2]));
                        t->setItem(row - 1, 1, new QTableWidgetItem(""));
                        t->setItem(row - 1, 2, new QTableWidgetItem("Contents/MacOS/" + fileInfoList.fileName()));
                        t->setItem(row - 1, 3, new QTableWidgetItem("Contents/Info.plist"));
                        t->setItem(row - 1, 4, new QTableWidgetItem(""));
                        t->setItem(row - 1, 5, new QTableWidgetItem(""));
                        init_enabled_data(t, row - 1, 6, "true");

                        QTableWidgetItem* newItem1 = new QTableWidgetItem("x86_64");
                        newItem1->setTextAlignment(Qt::AlignCenter);
                        t->setItem(row - 1, 7, newItem1);

                    } else { //不存在二进制文件，只存在一个Info.plist文件的情况

                        QDir fileDir(filePath + "/" + fileInfo.fileName() + "/Contents/PlugIns/" + kext_file[i + 2] + "/Contents/");
                        if (fileDir.exists()) {
                            //写入到表里
                            int row = t->rowCount() + 1;

                            t->setRowCount(row);
                            t->setItem(row - 1, 0, new QTableWidgetItem(QFileInfo(FileName[j]).fileName() + "/Contents/PlugIns/" + kext_file[i + 2]));
                            t->setItem(row - 1, 1, new QTableWidgetItem(""));
                            t->setItem(row - 1, 2, new QTableWidgetItem(""));
                            t->setItem(row - 1, 3, new QTableWidgetItem("Contents/Info.plist"));
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

void MainWindow::on_btnKernelBlock_Add_clicked()
{
    add_item(ui->table_kernel_block, 4);
    init_enabled_data(ui->table_kernel_block,
        ui->table_kernel_block->currentRow(), 4, "true");

    QTableWidgetItem* newItem1 = new QTableWidgetItem("Any");
    newItem1->setTextAlignment(Qt::AlignCenter);
    ui->table_kernel_block->setItem(ui->table_kernel_block->currentRow(), 5,
        newItem1);

    this->setWindowModified(true);
}

void MainWindow::on_btnKernelBlock_Del_clicked()
{
    del_item(ui->table_kernel_block);
}

void MainWindow::on_btnMiscBO_Add_clicked()
{
    add_item(ui->tableBlessOverride, 1);
    ui->tableBlessOverride->setItem(ui->tableBlessOverride->currentRow(), 0,
        new QTableWidgetItem(""));

    this->setWindowModified(true);
}

void MainWindow::on_btnMiscBO_Del_clicked()
{
    del_item(ui->tableBlessOverride);
}

void MainWindow::on_btnMiscEntries_Add_clicked()
{
    add_item(ui->tableEntries, 6);

    init_enabled_data(ui->tableEntries, ui->tableEntries->rowCount() - 1, 4,
        "false");
    init_enabled_data(ui->tableEntries, ui->tableEntries->rowCount() - 1, 5,
        "true");
    init_enabled_data(ui->tableEntries, ui->tableEntries->rowCount() - 1, 6,
        "false");

    this->setWindowModified(true);
}

void MainWindow::on_btnMiscTools_Add_clicked()
{

    QFileDialog fd;

    QStringList FileName = fd.getOpenFileNames(this, "tools efi file", "",
        "efi file(*.efi);;all files(*.*)");

    addEFITools(FileName);
}

void MainWindow::addEFITools(QStringList FileName)
{
    if (FileName.isEmpty())

        return;

    for (int i = 0; i < FileName.count(); i++) {
        add_item(ui->tableTools, 7);

        int row = ui->tableTools->rowCount();

        ui->tableTools->setItem(
            row - 1, 0, new QTableWidgetItem(QFileInfo(FileName.at(i)).fileName()));

        ui->tableTools->setFocus();
        ui->tableTools->setCurrentCell(row - 1, 0);

        init_enabled_data(ui->tableTools, ui->tableTools->rowCount() - 1, 4,
            "false");
        init_enabled_data(ui->tableTools, ui->tableTools->rowCount() - 1, 5,
            "true");
        init_enabled_data(ui->tableTools, ui->tableTools->rowCount() - 1, 6,
            "false");
        init_enabled_data(ui->tableTools, ui->tableTools->rowCount() - 1, 7,
            "false");
    }

    this->setWindowModified(true);
}

void MainWindow::on_btnMiscEntries_Del_clicked()
{
    del_item(ui->tableEntries);
}

void MainWindow::on_btnMiscTools_Del_clicked()
{
    del_item(ui->tableTools);
}

void MainWindow::on_btnNVRAMAdd_Add0_clicked()
{
    add_item(ui->table_nv_add0, 1);
    ui->table_nv_add->setRowCount(0); //先清除右边表中的所有条目
    on_btnNVRAMAdd_Add_clicked(); //同时右边增加一个新条目

    write_ini(ui->table_nv_add0->objectName(), ui->table_nv_add,
        ui->table_nv_add0->rowCount() - 1);

    this->setWindowModified(true);
}

void MainWindow::on_btnNVRAMAdd_Add_clicked()
{
    if (ui->table_nv_add0->rowCount() == 0)
        return;

    loading = true;
    add_item(ui->table_nv_add, 3);
    loading = false;

    //保存数据
    write_ini(ui->table_nv_add0->objectName(), ui->table_nv_add,
        ui->table_nv_add0->currentRow());

    this->setWindowModified(true);
}

void MainWindow::on_btnNVRAMAdd_Del0_clicked()
{
    int count = ui->table_nv_add0->rowCount();

    if (count == 0)
        return;

    //先记住被删的条目位置
    int delindex = ui->table_nv_add0->currentRow();

    QString qz = QDir::homePath() + "/.config/QtOCC/" + CurrentDateTime + ui->table_nv_add0->objectName();
    QFile file(qz + QString::number(delindex + 1) + ".ini");
    del_item(ui->table_nv_add0);
    if (file.exists())
        file.remove();

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

void MainWindow::on_btnNVRAMAdd_Del_clicked()
{
    del_item(ui->table_nv_add);
    write_ini(ui->table_nv_add0->objectName(), ui->table_nv_add,
        ui->table_nv_add0->currentRow());
}

void MainWindow::on_btnNVRAMDel_Add0_clicked()
{
    add_item(ui->table_nv_del0, 1);
    ui->table_nv_del->setRowCount(0); //先清除右边表中的所有条目
    on_btnNVRAMDel_Add_clicked(); //同时右边增加一个新条目

    write_value_ini(ui->table_nv_del0->objectName(), ui->table_nv_del,
        ui->table_nv_del0->rowCount() - 1);

    this->setWindowModified(true);
}

void MainWindow::on_btnNVRAMDel_Add_clicked()
{
    if (ui->table_nv_del0->rowCount() == 0)
        return;

    loading = true;
    add_item(ui->table_nv_del, 1);
    loading = false;

    //保存数据
    write_value_ini(ui->table_nv_del0->objectName(), ui->table_nv_del,
        ui->table_nv_del0->currentRow());

    this->setWindowModified(true);
}

void MainWindow::on_btnNVRAMLS_Add0_clicked()
{
    add_item(ui->table_nv_ls0, 1);
    ui->table_nv_ls->setRowCount(0); //先清除右边表中的所有条目
    on_btnNVRAMLS_Add_clicked(); //同时右边增加一个新条目

    write_value_ini(ui->table_nv_ls0->objectName(), ui->table_nv_ls,
        ui->table_nv_ls0->rowCount() - 1);

    this->setWindowModified(true);
}

void MainWindow::on_btnNVRAMLS_Add_clicked()
{
    if (ui->table_nv_ls0->rowCount() == 0)
        return;

    loading = true;
    add_item(ui->table_nv_ls, 1);
    loading = false;

    //保存数据
    write_value_ini(ui->table_nv_ls0->objectName(), ui->table_nv_ls,
        ui->table_nv_ls0->currentRow());

    this->setWindowModified(true);
}

void MainWindow::on_btnNVRAMDel_Del0_clicked()
{
    if (ui->table_nv_del0->rowCount() == 0)
        return;

    //先记住被删的条目位置
    int delindex = ui->table_nv_del0->currentRow();
    int count = ui->table_nv_del0->rowCount();

    QString qz = QDir::homePath() + "/.config/QtOCC/" + CurrentDateTime + ui->table_nv_del0->objectName();
    QFile file(qz + QString::number(delindex + 1) + ".ini");
    del_item(ui->table_nv_del0);
    if (file.exists())
        file.remove();

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

void MainWindow::on_btnNVRAMLS_Del0_clicked()
{
    if (ui->table_nv_ls0->rowCount() == 0)
        return;

    //先记住被删的条目位置
    int delindex = ui->table_nv_ls0->currentRow();
    int count = ui->table_nv_ls0->rowCount();

    QString qz = QDir::homePath() + "/.config/QtOCC/" + CurrentDateTime + ui->table_nv_ls0->objectName();
    QFile file(qz + QString::number(delindex + 1) + ".ini");
    del_item(ui->table_nv_ls0);
    if (file.exists())
        file.remove();

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

void MainWindow::on_btnNVRAMDel_Del_clicked()
{
    del_item(ui->table_nv_del);

    //保存数据
    write_value_ini(ui->table_nv_del0->objectName(), ui->table_nv_del,
        ui->table_nv_del0->currentRow());
}

void MainWindow::on_btnNVRAMLS_Del_clicked()
{
    del_item(ui->table_nv_ls);

    //保存数据
    write_value_ini(ui->table_nv_ls0->objectName(), ui->table_nv_ls,
        ui->table_nv_ls0->currentRow());
}

void MainWindow::on_btnUEFIRM_Add_clicked()
{
    add_item(ui->table_uefi_ReservedMemory, 4);
    init_enabled_data(ui->table_uefi_ReservedMemory,
        ui->table_uefi_ReservedMemory->rowCount() - 1, 4, "true");

    this->setWindowModified(true);
}

void MainWindow::on_btnUEFIRM_Del_clicked()
{
    del_item(ui->table_uefi_ReservedMemory);
}

void MainWindow::on_btnUEFIDrivers_Add_clicked()
{
    QFileDialog fd;

    QStringList FileName = fd.getOpenFileNames(this, "file", "", "efi file(*.efi);;all(*.*)");

    addEFIDrivers(FileName);
}

void MainWindow::addEFIDrivers(QStringList FileName)
{
    if (FileName.isEmpty())

        return;

    for (int i = 0; i < FileName.count(); i++) {
        int row = ui->table_uefi_drivers->rowCount() + 1;

        ui->table_uefi_drivers->setRowCount(row);
        ui->table_uefi_drivers->setItem(
            row - 1, 0, new QTableWidgetItem(QFileInfo(FileName.at(i)).fileName()));

        ui->table_uefi_drivers->setFocus();
        ui->table_uefi_drivers->setCurrentCell(row - 1, 0);
    }

    this->setWindowModified(true);
}

void MainWindow::on_btnUEFIDrivers_Del_clicked()
{
    del_item(ui->table_uefi_drivers);
}

void MainWindow::on_btnKernelAdd_Up_clicked()
{

    QTableWidget* t = new QTableWidget;
    t = ui->table_kernel_add;

    t->setFocus();

    if (t->rowCount() == 0 || t->currentRow() == 0 || t->currentRow() < 0)
        return;

    int cr = t->currentRow();
    //先将上面的内容进行备份
    QString item[8];
    item[0] = t->item(cr - 1, 0)->text();
    item[1] = t->item(cr - 1, 1)->text();
    item[2] = t->item(cr - 1, 2)->text();
    item[3] = t->item(cr - 1, 3)->text();
    item[4] = t->item(cr - 1, 4)->text();
    item[5] = t->item(cr - 1, 5)->text();
    item[6] = t->item(cr - 1, 6)->text();
    item[7] = t->item(cr - 1, 7)->text();
    //将下面的内容移到上面
    t->item(cr - 1, 0)->setText(t->item(cr, 0)->text());
    t->item(cr - 1, 1)->setText(t->item(cr, 1)->text());
    t->item(cr - 1, 2)->setText(t->item(cr, 2)->text());
    t->item(cr - 1, 3)->setText(t->item(cr, 3)->text());
    t->item(cr - 1, 4)->setText(t->item(cr, 4)->text());
    t->item(cr - 1, 5)->setText(t->item(cr, 5)->text());
    t->item(cr - 1, 6)->setText(t->item(cr, 6)->text());
    if (t->item(cr, 6)->text() == "true")
        t->item(cr - 1, 6)->setCheckState(Qt::Checked);
    else
        t->item(cr - 1, 6)->setCheckState(Qt::Unchecked);

    t->item(cr - 1, 7)->setText(t->item(cr, 7)->text());

    //最后将之前的备份恢复到下面
    t->item(cr, 0)->setText(item[0]);
    t->item(cr, 1)->setText(item[1]);
    t->item(cr, 2)->setText(item[2]);
    t->item(cr, 3)->setText(item[3]);
    t->item(cr, 4)->setText(item[4]);
    t->item(cr, 5)->setText(item[5]);
    t->item(cr, 6)->setText(item[6]);
    if (item[6] == "true")
        t->item(cr, 6)->setCheckState(Qt::Checked);
    else
        t->item(cr, 6)->setCheckState(Qt::Unchecked);

    t->item(cr, 7)->setText(item[7]);

    t->setCurrentCell(cr - 1, t->currentColumn());

    this->setWindowModified(true);
}

void MainWindow::on_btnKernelAdd_Down_clicked()
{
    QTableWidget* t = new QTableWidget;
    t = ui->table_kernel_add;

    if (t->currentRow() == t->rowCount() - 1 || t->currentRow() < 0)
        return;

    int cr = t->currentRow();
    //先将下面的内容进行备份
    QString item[8];
    item[0] = t->item(cr + 1, 0)->text();
    item[1] = t->item(cr + 1, 1)->text();
    item[2] = t->item(cr + 1, 2)->text();
    item[3] = t->item(cr + 1, 3)->text();
    item[4] = t->item(cr + 1, 4)->text();
    item[5] = t->item(cr + 1, 5)->text();
    item[6] = t->item(cr + 1, 6)->text();
    item[7] = t->item(cr + 1, 7)->text();
    //将上面的内容移到下面
    t->item(cr + 1, 0)->setText(t->item(cr, 0)->text());
    t->item(cr + 1, 1)->setText(t->item(cr, 1)->text());
    t->item(cr + 1, 2)->setText(t->item(cr, 2)->text());
    t->item(cr + 1, 3)->setText(t->item(cr, 3)->text());
    t->item(cr + 1, 4)->setText(t->item(cr, 4)->text());
    t->item(cr + 1, 5)->setText(t->item(cr, 5)->text());
    t->item(cr + 1, 6)->setText(t->item(cr, 6)->text());
    if (t->item(cr, 6)->text() == "true")
        t->item(cr + 1, 6)->setCheckState(Qt::Checked);
    else
        t->item(cr + 1, 6)->setCheckState(Qt::Unchecked);

    t->item(cr + 1, 7)->setText(t->item(cr, 7)->text());

    //最后将之前的备份恢复到上面
    t->item(cr, 0)->setText(item[0]);
    t->item(cr, 1)->setText(item[1]);
    t->item(cr, 2)->setText(item[2]);
    t->item(cr, 3)->setText(item[3]);
    t->item(cr, 4)->setText(item[4]);
    t->item(cr, 5)->setText(item[5]);
    t->item(cr, 6)->setText(item[6]);
    if (item[6] == "true")
        t->item(cr, 6)->setCheckState(Qt::Checked);
    else
        t->item(cr, 6)->setCheckState(Qt::Unchecked);

    t->item(cr, 7)->setText(item[7]);

    t->setCurrentCell(cr + 1, t->currentColumn());

    this->setWindowModified(true);
}

void MainWindow::test(bool test)
{
    if (test) {

    } else {
    }
}

void MainWindow::on_btnSaveAs()
{
    QFileDialog fd;

    PlistFileName = fd.getSaveFileName(this, "plist", "",
        "plist(*.plist);;all(*.*)");
    if (!PlistFileName.isEmpty()) {
        setWindowTitle(title + "      [*]" + PlistFileName);
        SaveFileName = PlistFileName;
    } else {
        if (closeSave)
            clear_temp_data();

        return;
    }

    SavePlist(PlistFileName);

    ui->actionSave->setEnabled(true);
}

void MainWindow::about()
{
    //QString strIcon = tr("Icon designer:  Mirone(Brazil)");
    //QString strUrl = "<a style='color: blue;' href = "
    //                 "https://github.com/ic005k/QtOpenCoreConfig>QtOpenCoreConfigurator</"
    //                 "a><br><a style='color: blue;'<\n><br>";

    //QMessageBox::about(this, tr("About"), strUrl + "\n" + strIcon);

    aboutDlg->setModal(true);
    aboutDlg->show();
}

void MainWindow::on_btnKernelAdd_Del_clicked()
{
    del_item(ui->table_kernel_add);
}

void MainWindow::on_table_dp_add_cellClicked(int row, int column)
{

    if (column == 1) {

        cboxDataClass = new QComboBox;
        cboxDataClass->addItem("Data");
        cboxDataClass->addItem("String");
        cboxDataClass->addItem("Number");
        cboxDataClass->addItem("");
        connect(cboxDataClass, SIGNAL(currentIndexChanged(QString)), this,
            SLOT(dataClassChange_dp()));
        c_row = row;

        ui->table_dp_add->setCellWidget(row, column, cboxDataClass);
        cboxDataClass->setCurrentText(ui->table_dp_add->item(row, 1)->text());
    }

    ui->statusbar->showMessage(ui->table_dp_add->currentItem()->text());
}

void MainWindow::on_table_dp_add_currentCellChanged(int currentRow,
    int currentColumn,
    int previousRow,
    int previousColumn)
{
    if (currentRow == 0 && currentColumn == 0 && previousColumn == 0) {
    }

    ui->table_dp_add->removeCellWidget(previousRow, 1);
}

void MainWindow::arch_addChange()
{
    ui->table_kernel_add->item(c_row, 7)->setText(cboxArch->currentText());
}

void MainWindow::arch_ForceChange()
{
    ui->table_kernel_Force->item(c_row, 8)->setText(cboxArch->currentText());
}

void MainWindow::arch_blockChange()
{
    ui->table_kernel_block->item(c_row, 5)->setText(cboxArch->currentText());
}

void MainWindow::arch_patchChange()
{
    ui->table_kernel_patch->item(c_row, 13)->setText(cboxArch->currentText());
}

void MainWindow::arch_Booter_patchChange()
{
    ui->table_Booter_patch->item(c_row, 10)->setText(cboxArch->currentText());
}

void MainWindow::ReservedMemoryTypeChange()
{
    ui->table_uefi_ReservedMemory->item(c_row, 3)->setText(
        cboxReservedMemoryType->currentText());
}

void MainWindow::dataClassChange_dp()
{
    ui->table_dp_add->item(c_row, 1)->setTextAlignment(Qt::AlignCenter);
    ui->table_dp_add->item(c_row, 1)->setText(cboxDataClass->currentText());
}

void MainWindow::dataClassChange_nv()
{
    ui->table_nv_add->item(c_row, 1)->setTextAlignment(Qt::AlignCenter);
    ui->table_nv_add->item(c_row, 1)->setText(cboxDataClass->currentText());
}

void MainWindow::on_table_nv_add_cellClicked(int row, int column)
{

    if (column == 1) {
        cboxDataClass = new QComboBox;
        cboxDataClass->addItem("Data");
        cboxDataClass->addItem("String");
        cboxDataClass->addItem("Number");
        cboxDataClass->addItem("");
        connect(cboxDataClass, SIGNAL(currentIndexChanged(QString)), this,
            SLOT(dataClassChange_nv()));
        c_row = row;

        ui->table_nv_add->setCellWidget(row, column, cboxDataClass);
        cboxDataClass->setCurrentText(ui->table_nv_add->item(row, 1)->text());
    }

    ui->statusbar->showMessage(ui->table_nv_add->currentItem()->text());
}

void MainWindow::on_table_nv_add_currentCellChanged(int currentRow,
    int currentColumn,
    int previousRow,
    int previousColumn)
{
    if (currentRow == 0 && currentColumn == 0 && previousColumn == 0) {
    }

    ui->table_nv_add->removeCellWidget(previousRow, 1);
}

void MainWindow::reg_win()
{
    QString appPath = qApp->applicationFilePath();

    QString dir = qApp->applicationDirPath();
    // 注意路径的替换
    appPath.replace("/", "\\");
    QString type = "QtiASL";
    QSettings* regType = new QSettings("HKEY_CLASSES_ROOT\\.plist", QSettings::NativeFormat);
    QSettings* regIcon = new QSettings("HKEY_CLASSES_ROOT\\.plist\\DefaultIcon",
        QSettings::NativeFormat);
    QSettings* regShell = new QSettings("HKEY_CLASSES_ROOT\\QtOpenCoreConfig\\shell\\open\\command",
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
    int previousColumn)
{
    if (currentRow == 0 && currentColumn == 0 && previousColumn == 0) {
    }

    ui->table_kernel_add->removeCellWidget(previousRow, 7);
}

void MainWindow::on_table_kernel_block_currentCellChanged(int currentRow,
    int currentColumn,
    int previousRow,
    int previousColumn)
{
    if (currentRow == 0 && currentColumn == 0 && previousColumn == 0) {
    }

    ui->table_kernel_block->removeCellWidget(previousRow, 5);
}

void MainWindow::on_table_kernel_patch_currentCellChanged(int currentRow,
    int currentColumn,
    int previousRow,
    int previousColumn)
{
    if (currentRow == 0 && currentColumn == 0 && previousColumn == 0) {
    }

    ui->table_kernel_patch->removeCellWidget(previousRow, 13);
}

QString MainWindow::getSystemProductName(QString arg1)
{
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
    const QString& arg1)
{
    if (!loading && arg1 != "") {
        QFileInfo appInfo(qApp->applicationDirPath());
        gs = new QProcess;

        QString str = getSystemProductName(arg1);
        ui->editSystemProductName->setText(str);
        ui->editSystemProductName_2->setText(str);

#ifdef Q_OS_WIN32
        // win
        // QFile file(appInfo.filePath() + "/macserial.exe");

        gs->start(appInfo.filePath() + "/macserial.exe",
            QStringList() << "-m" << str); //阻塞为execute

#endif

#ifdef Q_OS_LINUX
        // linux
        gs->start(appInfo.filePath() + "/macserial", QStringList() << "-m" << str);

#endif

#ifdef Q_OS_MAC
        // mac
        gs->start(appInfo.filePath() + "/macserial", QStringList() << "-m" << str);

#endif

        connect(gs, SIGNAL(finished(int)), this, SLOT(readResult()));
        // connect(gs , SIGNAL(readyRead()) , this , SLOT(readResult()));
    }

    this->setWindowModified(true);
}

void MainWindow::readResult()
{

    QTextEdit* textMacInfo = new QTextEdit;
    QTextCodec* gbkCodec = QTextCodec::codecForName("UTF-8");
    textMacInfo->clear();
    QString result = gbkCodec->toUnicode(gs->readAll());
    textMacInfo->append(result);
    //取第三行的数据，第一行留给提示用
    QString str = textMacInfo->document()->findBlockByNumber(2).text().trimmed();

    // qDebug() << result;

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

    ui->editMLB->setText(str2);
    ui->editMLB_2->setText(str2);

    on_btnSystemUUID_clicked();
}

void MainWindow::readResultSystemInfo()
{
    ui->textMacInfo->clear();
    QTextCodec* gbkCodec = QTextCodec::codecForName("UTF-8");
    QString result = gbkCodec->toUnicode(si->readAll());
    ui->textMacInfo->append(result);
    // qDebug() << result;
}

void MainWindow::on_btnGenerate_clicked()
{
    on_cboxSystemProductName_currentIndexChanged(
        ui->cboxSystemProductName->currentText());
}

void MainWindow::on_btnSystemUUID_clicked()
{
    QUuid id = QUuid::createUuid();
    QString strTemp = id.toString();
    QString strId = strTemp.mid(1, strTemp.count() - 2).toUpper();

    ui->editSystemUUID->setText(strId);
    ui->editSystemUUID_data->setText(strId);
    ui->editSystemUUID_2->setText(strId);
}

void MainWindow::on_table_kernel_Force_cellClicked(int row, int column)
{
    if (!ui->table_kernel_Force->currentIndex().isValid())
        return;

    enabled_change(ui->table_kernel_Force, row, column, 7);

    if (column == 8) {

        cboxArch = new QComboBox;
        cboxArch->addItem("Any");
        cboxArch->addItem("i386");
        cboxArch->addItem("x86_64");
        cboxArch->addItem("");

        connect(cboxArch, SIGNAL(currentIndexChanged(QString)), this,
            SLOT(arch_ForceChange()));
        c_row = row;

        ui->table_kernel_Force->setCellWidget(row, column, cboxArch);
        cboxArch->setCurrentText(ui->table_kernel_Force->item(row, 8)->text());
    }

    ui->statusbar->showMessage(ui->table_kernel_Force->currentItem()->text());
}

void MainWindow::on_table_kernel_Force_currentCellChanged(int currentRow,
    int currentColumn,
    int previousRow,
    int previousColumn)
{
    if (currentRow == 0 && currentColumn == 0 && previousColumn == 0) {
    }

    ui->table_kernel_Force->removeCellWidget(previousRow, 8);
}

void MainWindow::on_btnKernelForce_Add_clicked()
{
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

void MainWindow::on_btnKernelForce_Del_clicked()
{
    del_item(ui->table_kernel_Force);
}

void MainWindow::dragEnterEvent(QDragEnterEvent* e)
{

    if (e->mimeData()->hasFormat("text/uri-list")) {
        e->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent* e)
{
    QList<QUrl> urls = e->mimeData()->urls();
    if (urls.isEmpty()) {
        return;
    }

    QStringList fileList;
    for (int i = 0; i < urls.count(); i++) {
        fileList.append(urls.at(i).toLocalFile());
    }

    //QString fileName = urls.first().toLocalFile();
    if (fileList.isEmpty()) {
        return;
    }

    //plist
    QFileInfo fi(fileList.at(0));
    if (fi.suffix().toLower() == "plist") {

        PlistFileName = fileList.at(0);
        openFile(PlistFileName);
    }

    //aml
    if (fi.suffix().toLower() == "aml") {

        if (ui->tabTotal->currentIndex() == 0 && ui->tabACPI->currentIndex() == 0) {

            addACPIItem(fileList);
        }
    }

    //kext
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

    //qDebug() << fi.suffix().toLower() << fileList.at(0) << fileSuffix;
    if (fileSuffix == "kext") {
        if (ui->tabTotal->currentIndex() == 3 && ui->tabKernel->currentIndex() == 0) {
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
                //qDebug() << str4;
            }

            addKexts(kextList);
        }
    }

    //efi tools
    if (fi.suffix().toLower() == "efi") {
        if (ui->tabTotal->currentIndex() == 4 && ui->tabMisc->currentIndex() == 5) {
            addEFITools(fileList);
        }
    }

    //efi drivers
    if (fi.suffix().toLower() == "efi") {
        if (ui->tabTotal->currentIndex() == 7 && ui->tabUEFI->currentIndex() == 2) {
            addEFIDrivers(fileList);
        }
    }
}

#ifdef Q_OS_WIN32
void MainWindow::runAdmin(QString file, QString arg)
{

    QString exePath = file;
    WCHAR exePathArray[1024] = { 0 };
    exePath.toWCharArray(exePathArray);

    QString command = arg; //"-mount:*";//带参数运行
    WCHAR commandArr[1024] = { 0 };
    command.toWCharArray(commandArr);
    HINSTANCE hNewExe = ShellExecute(NULL, L"runas", exePathArray, commandArr, NULL,
        SW_SHOWMAXIMIZED); // SW_NORMAL SW_SHOWMAXIMIZED
    if (hNewExe) {
    };
}
#endif

void MainWindow::mount_esp()
{

#ifdef Q_OS_WIN32
    // di = new QProcess;
    // di->execute("mountvol.exe", QStringList() << "x:" << "/s");//阻塞

    QString exec = QCoreApplication::applicationDirPath() + "/FindESP.exe";

    // runAdmin(exec, "-unmount:*");
    runAdmin(exec, "-mount:*"); //可选参数-Updater

    QString exec2 = QCoreApplication::applicationDirPath() + "/winfile.exe";

    runAdmin(exec2, NULL); //此时参数为空

#endif

#ifdef Q_OS_LINUX

#endif

#ifdef Q_OS_MAC
    di = new QProcess;
    di->start("diskutil", QStringList() << "list");
    connect(di, SIGNAL(finished(int)), this, SLOT(readResultDiskInfo()));

#endif
}

void MainWindow::readResultDiskInfo()
{
    ui->textDiskInfo->clear();
    ui->textDiskInfo->setReadOnly(true);
    QTextCodec* gbkCodec = QTextCodec::codecForName("UTF-8");
    QString result = gbkCodec->toUnicode(di->readAll());
    ui->textDiskInfo->append(result);

    QString str0, str1, strEfiDisk;
    int count = ui->textDiskInfo->document()->lineCount();
    for (int i = 0; i < count; i++) {
        str0 = ui->textDiskInfo->document()->findBlockByNumber(i).text().trimmed();
        str1 = str0.mid(3, str0.count() - 3).trimmed();

        if (str1.mid(0, 3).toUpper() == "EFI") {

            strEfiDisk = str1.mid(str1.count() - 7, 7);
            mount_esp_mac(strEfiDisk);
        }

        // qDebug() << strEfiDisk << str1;
    }
}

void MainWindow::mount_esp_mac(QString strEfiDisk)
{
    QString str5 = "diskutil mount " + strEfiDisk;
    QString str_ex = "do shell script " + QString::fromLatin1("\"%1\"").arg(str5) + " with administrator privileges";

    QString fileName = QDir::homePath() + "/.config/QtOCC/qtocc.applescript";
    QFile fi(fileName);
    if (fi.exists())
        fi.remove();

    QSaveFile file(fileName);
    QString errorMessage;
    if (file.open(QFile::WriteOnly | QFile::Text)) {

        QTextStream out(&file);
        out << str_ex;
        if (!file.commit()) {
            errorMessage = tr("Cannot write file %1:\n%2.")
                               .arg(QDir::toNativeSeparators(fileName), file.errorString());
        }
    } else {
        errorMessage = tr("Cannot open file %1 for writing:\n%2.")
                           .arg(QDir::toNativeSeparators(fileName), file.errorString());
    }

    QProcess* dm = new QProcess;
    dm->execute("osascript", QStringList() << fileName);
}

void MainWindow::on_btnMountEsp()
{
    mount_esp();
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    if (event) {
    };

    QString qfile = QDir::homePath() + "/.config/QtOCC/QtOCC.ini";
    QFile file(qfile);
    // QSettings Reg(qfile, QSettings::NativeFormat);
    QSettings Reg(qfile, QSettings::IniFormat);
    Reg.setValue("SaveDataHub", ui->chkSaveDataHub->isChecked());

    if (this->isWindowModified()) {
        int choice;
        if (!zh_cn) {

            choice = QMessageBox::warning(
                this, tr("Application"),
                tr("The document has been modified.\n"
                   "Do you want to save your changes?\n\n")
                    + SaveFileName,
                QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

        } else {
            QMessageBox message(QMessageBox::Warning, "QtiASL",
                "文件内容已修改，是否保存？\n\n" + SaveFileName);
            message.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
            message.setButtonText(QMessageBox::Save, QString("保 存"));
            message.setButtonText(QMessageBox::Cancel, QString("取 消"));
            message.setButtonText(QMessageBox::Discard, QString("不保存"));
            message.setDefaultButton(QMessageBox::Save);
            choice = message.exec();
        }

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
            event->ignore();
            break;
        }
    } else {
        clear_temp_data();

        event->accept();
    }
}

void MainWindow::on_table_uefi_ReservedMemory_currentCellChanged(
    int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    if (currentRow == 0 && currentColumn == 0 && previousColumn == 0) {
    }

    ui->table_uefi_ReservedMemory->removeCellWidget(previousRow, 3);
}

void MainWindow::loadLocal()
{
    QTextCodec* codec = QTextCodec::codecForName("System");
    QTextCodec::setCodecForLocale(codec);

    static QTranslator translator; //该对象要一直存在，注意用static
    QLocale locale;
    if (locale.language() == QLocale::English) //获取系统语言环境
    {

        zh_cn = false;

    } else if (locale.language() == QLocale::Chinese) {

        bool tr = false;
        tr = translator.load(":/cn.qm");
        if (tr) {
            qApp->installTranslator(&translator);
            zh_cn = true;
        }

        ui->retranslateUi(this);
    }
}

void MainWindow::on_btnHelp()
{

    QFileInfo appInfo(qApp->applicationDirPath());
    QString qtManulFile = appInfo.filePath() + "/Configuration.pdf";

    QDesktopServices::openUrl(QUrl::fromLocalFile(qtManulFile));
}

void MainWindow::init_tr_str()
{
    strArch = tr("Kext architecture (Any, i386, x86_64).");
    strBundlePath = tr("Kext bundle path (e.g. Lilu.kext or "
                       "MyKext.kext/Contents/PlugIns/MySubKext.kext).");
    strComment = tr("Comment.");
    strEnabled = tr("This kernel driver will not be added unless set to true.");
    strExecutablePath = tr("Kext executable path relative to bundle (e.g. Contents/MacOS/Lilu).");
    strMaxKernel = tr("Adds kernel driver on specified macOS version or older.");
    strMinKernel = tr("Adds kernel driver on specified macOS version or newer.");
    strPlistPath = tr(
        " Kext Info.plist path relative to bundle (e.g. Contents/Info.plist).");
}

void MainWindow::on_btnExportMaster()
{
    QFileDialog fd;
    QString defname;
    int index = ui->tabTotal->currentIndex();

    switch (index) {
    case 0:
        defname = "ACPI";
        break;
    case 1:
        defname = "Booter";
        break;
    case 2:
        defname = "DeviceProperties";
        break;
    case 3:
        defname = "Kernel";
        break;
    case 4:
        defname = "Misc";
        break;
    case 5:
        defname = "NVRAM";
        break;
    case 6:
        defname = "PlatformInfo";
        break;
    case 7:
        defname = "UEFI";
    }

    QString FileName = fd.getSaveFileName(this, tr("Save File"), defname,
        tr("Config file(*.plist);;All files(*.*)"));
    if (FileName.isEmpty())
        return;

    QVariantMap OpenCore;

    switch (index) {
    case 0:
        OpenCore["ACPI"] = SaveACPI();

        break;

    case 1:
        OpenCore["Booter"] = SaveBooter();
        break;

    case 2:
        OpenCore["DeviceProperties"] = SaveDeviceProperties();
        break;

    case 3:
        OpenCore["Kernel"] = SaveKernel();
        break;

    case 4:
        OpenCore["Misc"] = SaveMisc();
        break;

    case 5:
        OpenCore["NVRAM"] = SaveNVRAM();
        break;

    case 6:
        OpenCore["PlatformInfo"] = SavePlatformInfo();
        break;

    case 7:
        OpenCore["UEFI"] = SaveUEFI();
        break;
    }

    PListSerializer::toPList(OpenCore, FileName);
}

void MainWindow::on_btnImportMaster()
{
    QFileDialog fd;
    QString defname;
    int index = ui->tabTotal->currentIndex();

    switch (index) {
    case 0:
        defname = "ACPI.plist";
        break;
    case 1:
        defname = "Booter.plist";
        break;
    case 2:
        defname = "DeviceProperties.plist";
        break;
    case 3:
        defname = "Kernel.plist";
        break;
    case 4:
        defname = "Misc.plist";
        break;
    case 5:
        defname = "NVRAM.plist";
        break;
    case 6:
        defname = "PlatformInfo.plist";
        break;
    case 7:
        defname = "UEFI.plist";
    }

    QString FileName = fd.getOpenFileName(this, tr("Open File"), defname,
        tr("Config file(*.plist);;All files(*.*)"));
    if (FileName.isEmpty())
        return;

    loading = true;

    QFile file(FileName);
    QVariantMap map = PListParser::parsePList(&file).toMap();

    switch (index) {
    case 0:
        // ACPI
        ui->table_acpi_add->setRowCount(0);
        ui->table_acpi_del->setRowCount(0);
        ui->table_acpi_patch->setRowCount(0);
        ParserACPI(map);

        break;

    case 1:
        // Booter
        ui->table_booter->setRowCount(0);
        ParserBooter(map);
        break;

    case 2:
        // DP
        ui->table_dp_add0->setRowCount(0);
        ui->table_dp_add->setRowCount(0);
        ui->table_dp_del0->setRowCount(0);
        ui->table_dp_del->setRowCount(0);
        ParserDP(map);
        break;

    case 3:
        // Kernel
        ui->table_kernel_add->setRowCount(0);
        ui->table_kernel_block->setRowCount(0);
        ui->table_kernel_Force->setRowCount(0);
        ui->table_kernel_patch->setRowCount(0);
        ParserKernel(map);
        break;

    case 4:
        // Misc
        ui->tableBlessOverride->setRowCount(0);
        ui->tableEntries->setRowCount(0);
        ui->tableTools->setRowCount(0);
        ParserMisc(map);
        break;

    case 5:
        // NVRAM
        ui->table_nv_add0->setRowCount(0);
        ui->table_nv_add->setRowCount(0);
        ui->table_nv_del0->setRowCount(0);
        ui->table_nv_del->setRowCount(0);
        ui->table_nv_ls0->setRowCount(0);
        ui->table_nv_ls->setRowCount(0);
        ParserNvram(map);
        break;

    case 6:
        ParserPlatformInfo(map);
        break;

    case 7:
        // UEFI
        ui->table_uefi_drivers->setRowCount(0);
        ui->table_uefi_ReservedMemory->setRowCount(0);
        ParserUEFI(map);
        break;
    }

    loading = false;
}

void MainWindow::on_tabTotal_tabBarClicked(int index)
{

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

void MainWindow::on_tabTotal_currentChanged(int index)
{
    on_tabTotal_tabBarClicked(index);
}

void MainWindow::on_btnDevices_add_clicked()
{
    add_item(ui->tableDevices, 8);

    this->setWindowModified(true);
}

void MainWindow::on_btnDevices_del_clicked()
{
    del_item(ui->tableDevices);
}

void MainWindow::on_cboxUpdateSMBIOSMode_currentIndexChanged(
    const QString& arg1)
{
    if (arg1 == "Custom")
        ui->chkCustomSMBIOSGuid->setChecked(true); //联动
    else
        ui->chkCustomSMBIOSGuid->setChecked(false);
}

int MainWindow::ExposeSensitiveData()
{
    int a, b, c, d;
    if (ui->chk01->isChecked())
        a = 1;
    else
        a = 0;

    if (ui->chk02->isChecked())
        b = 2;
    else
        b = 0;

    if (ui->chk04->isChecked())
        c = 4;
    else
        c = 0;

    if (ui->chk08->isChecked())
        d = 8;
    else
        d = 0;

    ui->editExposeSensitiveData->setText(QString::number(a + b + c + d));

    return a + b + c + d;
}

void MainWindow::on_chk01_clicked() { ExposeSensitiveData(); }

void MainWindow::on_chk02_clicked() { ExposeSensitiveData(); }

void MainWindow::on_chk04_clicked() { ExposeSensitiveData(); }

void MainWindow::on_chk08_clicked() { ExposeSensitiveData(); }

void MainWindow::on_editExposeSensitiveData_textChanged(const QString& arg1)
{
    int val = arg1.toInt();
    if (val == 0) {
        ui->chk01->setChecked(false);
        ui->chk02->setChecked(false);
        ui->chk04->setChecked(false);
        ui->chk08->setChecked(false);
    }

    if (val == 1) {
        ui->chk01->setChecked(true);
        ui->chk02->setChecked(false);
        ui->chk04->setChecked(false);
        ui->chk08->setChecked(false);
    }

    if (val == 2) {
        ui->chk01->setChecked(false);
        ui->chk02->setChecked(true);
        ui->chk04->setChecked(false);
        ui->chk08->setChecked(false);
    }

    if (val == 3) {
        ui->chk01->setChecked(1);
        ui->chk02->setChecked(1);
        ui->chk04->setChecked(0);
        ui->chk08->setChecked(0);
    }

    if (val == 4) {
        ui->chk01->setChecked(0);
        ui->chk02->setChecked(0);
        ui->chk04->setChecked(1);
        ui->chk08->setChecked(0);
    }

    if (val == 5) {
        ui->chk01->setChecked(1);
        ui->chk02->setChecked(0);
        ui->chk04->setChecked(1);
        ui->chk08->setChecked(0);
    }

    if (val == 6) {
        ui->chk01->setChecked(0);
        ui->chk02->setChecked(1);
        ui->chk04->setChecked(1);
        ui->chk08->setChecked(0);
    }

    if (val == 7) {
        ui->chk01->setChecked(1);
        ui->chk02->setChecked(1);
        ui->chk04->setChecked(1);
        ui->chk08->setChecked(0);
    }

    if (val == 8) {
        ui->chk01->setChecked(0);
        ui->chk02->setChecked(0);
        ui->chk04->setChecked(0);
        ui->chk08->setChecked(1);
    }

    if (val == 9) {
        ui->chk01->setChecked(1);
        ui->chk02->setChecked(0);
        ui->chk04->setChecked(0);
        ui->chk08->setChecked(1);
    }

    if (val == 10) {
        ui->chk01->setChecked(0);
        ui->chk02->setChecked(1);
        ui->chk04->setChecked(0);
        ui->chk08->setChecked(1);
    }

    if (val == 11) {
        ui->chk01->setChecked(1);
        ui->chk02->setChecked(1);
        ui->chk04->setChecked(0);
        ui->chk08->setChecked(1);
    }

    if (val == 12) {
        ui->chk01->setChecked(0);
        ui->chk02->setChecked(0);
        ui->chk04->setChecked(1);
        ui->chk08->setChecked(1);
    }

    if (val == 13) {
        ui->chk01->setChecked(1);
        ui->chk02->setChecked(0);
        ui->chk04->setChecked(1);
        ui->chk08->setChecked(1);
    }

    if (val == 14) {
        ui->chk01->setChecked(0);
        ui->chk02->setChecked(1);
        ui->chk04->setChecked(1);
        ui->chk08->setChecked(1);
    }

    if (val == 15) {
        ui->chk01->setChecked(1);
        ui->chk02->setChecked(1);
        ui->chk04->setChecked(1);
        ui->chk08->setChecked(1);
    }

    this->setWindowModified(true);
}

void MainWindow::ScanPolicy()
{

    if (ui->chk1->isChecked())
        v1 = 1;
    else
        v1 = 0;
    if (ui->chk2->isChecked())
        v2 = 2;
    else
        v2 = 0;
    if (ui->chk3->isChecked())
        v3 = 256;
    else
        v3 = 0;
    if (ui->chk4->isChecked())
        v4 = 512;
    else
        v4 = 0;
    if (ui->chk5->isChecked())
        v5 = 1024;
    else
        v5 = 0;
    if (ui->chk6->isChecked())
        v6 = 2048;
    else
        v6 = 0;
    if (ui->chk7->isChecked())
        v7 = 4096;
    else
        v7 = 0;
    if (ui->chk8->isChecked())
        v8 = 65536;
    else
        v8 = 0;
    if (ui->chk9->isChecked())
        v9 = 131072;
    else
        v9 = 0;
    if (ui->chk10->isChecked())
        v10 = 262144;
    else
        v10 = 0;
    if (ui->chk11->isChecked())
        v11 = 524288;
    else
        v11 = 0;
    if (ui->chk12->isChecked())
        v12 = 1048576;
    else
        v12 = 0;
    if (ui->chk13->isChecked())
        v13 = 2097152;
    else
        v13 = 0;
    if (ui->chk14->isChecked())
        v14 = 4194304;
    else
        v14 = 0;
    if (ui->chk15->isChecked())
        v15 = 8388608;
    else
        v15 = 0;
    if (ui->chk16->isChecked())
        v16 = 16777216;
    else
        v16 = 0;

    int total = v1 + v2 + v3 + v4 + v5 + v6 + v7 + v8 + v9 + v10 + v11 + v12 + v13 + v14 + v15 + v16;

    ui->editScanPolicy->setText(QString::number(total));
}

void MainWindow::on_chk1_clicked() { ScanPolicy(); }

void MainWindow::on_chk2_clicked() { ScanPolicy(); }

void MainWindow::on_chk3_clicked() { ScanPolicy(); }

void MainWindow::on_chk4_clicked() { ScanPolicy(); }

void MainWindow::on_chk5_clicked() { ScanPolicy(); }

void MainWindow::on_chk6_clicked() { ScanPolicy(); }

void MainWindow::on_chk7_clicked() { ScanPolicy(); }

void MainWindow::on_chk8_clicked() { ScanPolicy(); }

void MainWindow::on_chk9_clicked() { ScanPolicy(); }

void MainWindow::on_chk10_clicked() { ScanPolicy(); }

void MainWindow::on_chk11_clicked() { ScanPolicy(); }

void MainWindow::on_chk12_clicked() { ScanPolicy(); }

void MainWindow::on_chk13_clicked() { ScanPolicy(); }

void MainWindow::on_chk14_clicked() { ScanPolicy(); }

void MainWindow::on_chk15_clicked() { ScanPolicy(); }

void MainWindow::on_chk16_clicked() { ScanPolicy(); }

void MainWindow::on_editScanPolicy_textChanged(const QString& arg1)
{

    int total = arg1.toInt();

    chk.clear();
    chk.append(ui->chk1);
    chk.append(ui->chk2);
    chk.append(ui->chk3);
    chk.append(ui->chk4);
    chk.append(ui->chk5);
    chk.append(ui->chk6);
    chk.append(ui->chk7);
    chk.append(ui->chk8);
    chk.append(ui->chk9);
    chk.append(ui->chk10);
    chk.append(ui->chk11);
    chk.append(ui->chk12);
    chk.append(ui->chk13);
    chk.append(ui->chk14);
    chk.append(ui->chk15);
    chk.append(ui->chk16);

    v1 = 1;
    v2 = 2;
    v3 = 256;
    v4 = 512;
    v5 = 1024;
    v6 = 2048;
    v7 = 4096;
    v8 = 65536;
    v9 = 131072;
    v10 = 262144;
    v11 = 524288;
    v12 = 1048576;
    v13 = 2097152;
    v14 = 4194304;
    v15 = 8388608;
    v16 = 16777216;

    v.clear();
    v.append(v1);
    v.append(v2);
    v.append(v3);
    v.append(v4);
    v.append(v5);
    v.append(v6);
    v.append(v7);
    v.append(v8);
    v.append(v9);
    v.append(v10);
    v.append(v11);
    v.append(v12);
    v.append(v13);
    v.append(v14);
    v.append(v15);
    v.append(v16);

    scanPolicy = true;
    pickerAttributes = false;

    for (int i = 0; i < 16; i++)
        chk.at(i)->setChecked(false);

    method(v, total);

    this->setWindowModified(true);
}

void MainWindow::method(QVector<int> nums, int sum)
{

    QVector<int> list;

    method(nums, sum, list, -1);
}

void MainWindow::method(QVector<int> nums, int sum, QVector<int> list,
    int index)
{
    if (sum == 0) {
        for (int val : list) {
            // qDebug() << val;
            for (int i = 0; i < nums.count(); i++) {

                if (scanPolicy) {
                    if (val == v.at(i)) {

                        chk.at(i)->setChecked(true);
                    }
                }

                if (pickerAttributes) {
                    if (val == v_pa.at(i)) {
                        chk_pa.at(i)->setChecked(true);
                    }
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

void MainWindow::methodDisplayLevel(QVector<unsigned int> nums,
    unsigned int sum)
{

    QVector<unsigned int> list;

    methodDisplayLevel(nums, sum, list, -1);
}

void MainWindow::methodDisplayLevel(QVector<unsigned int> nums,
    unsigned int sum,
    QVector<unsigned int> list, int index)
{
    if (sum == 0) {
        for (unsigned int val : list) {
            qDebug() << val;
            for (int i = 0; i < 19; i++) {

                if (val == vDisplayLevel.at(i)) {
                    chkDisplayLevel.at(i)->setChecked(true);
                }
            }
        }
    } else if (sum > 0) {
        for (int i = index + 1; i < nums.count(); i++) {
            list.append(nums.at(i));
            methodDisplayLevel(nums, sum - nums.at(i), list, i);
            list.remove(list.size() - 1);
        }
    }
}

void MainWindow::DisplayLevel()
{

    click = true;

    vd1 = 1;
    vd2 = 2;
    vd3 = 4;
    vd4 = 8;
    vd5 = 16;
    vd6 = 32;
    vd7 = 64;
    vd8 = 128;
    vd9 = 256;
    vd10 = 1024;
    vd11 = 4096;
    vd12 = 16384;
    vd13 = 65536;
    vd14 = 131072;
    vd15 = 524288;
    vd16 = 1048576;
    vd17 = 2097152;
    vd18 = 4194304;
    vd19 = 2147483648;

    vDisplayLevel.clear();
    vDisplayLevel.append(vd1);
    vDisplayLevel.append(vd2);
    vDisplayLevel.append(vd3);
    vDisplayLevel.append(vd4);
    vDisplayLevel.append(vd5);
    vDisplayLevel.append(vd6);
    vDisplayLevel.append(vd7);
    vDisplayLevel.append(vd8);
    vDisplayLevel.append(vd9);
    vDisplayLevel.append(vd10);
    vDisplayLevel.append(vd11);
    vDisplayLevel.append(vd12);
    vDisplayLevel.append(vd13);
    vDisplayLevel.append(vd14);
    vDisplayLevel.append(vd15);
    vDisplayLevel.append(vd16);
    vDisplayLevel.append(vd17);
    vDisplayLevel.append(vd18);
    vDisplayLevel.append(vd19);

    chkDisplayLevel.clear();
    chkDisplayLevel.append(ui->chkD1);
    chkDisplayLevel.append(ui->chkD2);
    chkDisplayLevel.append(ui->chkD3);
    chkDisplayLevel.append(ui->chkD4);
    chkDisplayLevel.append(ui->chkD5);
    chkDisplayLevel.append(ui->chkD6);
    chkDisplayLevel.append(ui->chkD7);
    chkDisplayLevel.append(ui->chkD8);
    chkDisplayLevel.append(ui->chkD9);
    chkDisplayLevel.append(ui->chkD10);
    chkDisplayLevel.append(ui->chkD11);
    chkDisplayLevel.append(ui->chkD12);
    chkDisplayLevel.append(ui->chkD13);
    chkDisplayLevel.append(ui->chkD14);
    chkDisplayLevel.append(ui->chkD15);
    chkDisplayLevel.append(ui->chkD16);
    chkDisplayLevel.append(ui->chkD17);
    chkDisplayLevel.append(ui->chkD18);
    chkDisplayLevel.append(ui->chkD19);

    unsigned int total = 0;
    for (int i = 0; i < 19; i++) {
        if (!chkDisplayLevel.at(i)->isChecked()) {
            vDisplayLevel.remove(i);
            vDisplayLevel.insert(i, 0);
        }
    }

    for (int i = 0; i < 19; i++) {
        total = total + vDisplayLevel.at(i);
    }

    ui->editDisplayLevel->setText(QString::number(total));

    click = false;
}

void MainWindow::on_chkD1_clicked() { DisplayLevel(); }

void MainWindow::on_chkD2_clicked() { DisplayLevel(); }

void MainWindow::on_chkD3_clicked() { DisplayLevel(); }

void MainWindow::on_chkD4_clicked() { DisplayLevel(); }

void MainWindow::on_chkD5_clicked() { DisplayLevel(); }

void MainWindow::on_chkD6_clicked() { DisplayLevel(); }

void MainWindow::on_chkD7_clicked() { DisplayLevel(); }

void MainWindow::on_chkD8_clicked() { DisplayLevel(); }

void MainWindow::on_chkD9_clicked() { DisplayLevel(); }

void MainWindow::on_chkD10_clicked() { DisplayLevel(); }

void MainWindow::on_chkD11_clicked() { DisplayLevel(); }

void MainWindow::on_chkD12_clicked() { DisplayLevel(); }

void MainWindow::on_chkD13_clicked() { DisplayLevel(); }

void MainWindow::on_chkD14_clicked() { DisplayLevel(); }

void MainWindow::on_chkD15_clicked() { DisplayLevel(); }

void MainWindow::on_chkD16_clicked() { DisplayLevel(); }

void MainWindow::on_chkD17_clicked() { DisplayLevel(); }

void MainWindow::on_chkD18_clicked() { DisplayLevel(); }

void MainWindow::on_chkD19_clicked() { DisplayLevel(); }

void MainWindow::on_editDisplayLevel_textChanged(const QString& arg1)
{
    if (click)
        return;

    unsigned int total = arg1.toULongLong();

    vd1 = 1;
    vd2 = 2;
    vd3 = 4;
    vd4 = 8;
    vd5 = 16;
    vd6 = 32;
    vd7 = 64;
    vd8 = 128;
    vd9 = 256;
    vd10 = 1024;
    vd11 = 4096;
    vd12 = 16384;
    vd13 = 65536;
    vd14 = 131072;
    vd15 = 524288;
    vd16 = 1048576;
    vd17 = 2097152;
    vd18 = 4194304;
    vd19 = 2147483648;

    vDisplayLevel.clear();
    vDisplayLevel.append(vd1);
    vDisplayLevel.append(vd2);
    vDisplayLevel.append(vd3);
    vDisplayLevel.append(vd4);
    vDisplayLevel.append(vd5);
    vDisplayLevel.append(vd6);
    vDisplayLevel.append(vd7);
    vDisplayLevel.append(vd8);
    vDisplayLevel.append(vd9);
    vDisplayLevel.append(vd10);
    vDisplayLevel.append(vd11);
    vDisplayLevel.append(vd12);
    vDisplayLevel.append(vd13);
    vDisplayLevel.append(vd14);
    vDisplayLevel.append(vd15);
    vDisplayLevel.append(vd16);
    vDisplayLevel.append(vd17);
    vDisplayLevel.append(vd18);
    vDisplayLevel.append(vd19);

    chkDisplayLevel.clear();
    chkDisplayLevel.append(ui->chkD1);
    chkDisplayLevel.append(ui->chkD2);
    chkDisplayLevel.append(ui->chkD3);
    chkDisplayLevel.append(ui->chkD4);
    chkDisplayLevel.append(ui->chkD5);
    chkDisplayLevel.append(ui->chkD6);
    chkDisplayLevel.append(ui->chkD7);
    chkDisplayLevel.append(ui->chkD8);
    chkDisplayLevel.append(ui->chkD9);
    chkDisplayLevel.append(ui->chkD10);
    chkDisplayLevel.append(ui->chkD11);
    chkDisplayLevel.append(ui->chkD12);
    chkDisplayLevel.append(ui->chkD13);
    chkDisplayLevel.append(ui->chkD14);
    chkDisplayLevel.append(ui->chkD15);
    chkDisplayLevel.append(ui->chkD16);
    chkDisplayLevel.append(ui->chkD17);
    chkDisplayLevel.append(ui->chkD18);
    chkDisplayLevel.append(ui->chkD19);

    for (int i = 0; i < 19; i++) {
        chkDisplayLevel.at(i)->setChecked(false);
    }

    methodDisplayLevel(vDisplayLevel, total);

    this->setWindowModified(true);
}

void MainWindow::on_btnDLSetAll_clicked()
{
    chkDisplayLevel.clear();
    chkDisplayLevel.append(ui->chkD1);
    chkDisplayLevel.append(ui->chkD2);
    chkDisplayLevel.append(ui->chkD3);
    chkDisplayLevel.append(ui->chkD4);
    chkDisplayLevel.append(ui->chkD5);
    chkDisplayLevel.append(ui->chkD6);
    chkDisplayLevel.append(ui->chkD7);
    chkDisplayLevel.append(ui->chkD8);
    chkDisplayLevel.append(ui->chkD9);
    chkDisplayLevel.append(ui->chkD10);
    chkDisplayLevel.append(ui->chkD11);
    chkDisplayLevel.append(ui->chkD12);
    chkDisplayLevel.append(ui->chkD13);
    chkDisplayLevel.append(ui->chkD14);
    chkDisplayLevel.append(ui->chkD15);
    chkDisplayLevel.append(ui->chkD16);
    chkDisplayLevel.append(ui->chkD17);
    chkDisplayLevel.append(ui->chkD18);
    chkDisplayLevel.append(ui->chkD19);

    for (int i = 0; i < 19; i++) {
        chkDisplayLevel.at(i)->setChecked(true);
    }

    DisplayLevel();
}

void MainWindow::on_btnDLClear_clicked()
{

    chkDisplayLevel.clear();
    chkDisplayLevel.append(ui->chkD1);
    chkDisplayLevel.append(ui->chkD2);
    chkDisplayLevel.append(ui->chkD3);
    chkDisplayLevel.append(ui->chkD4);
    chkDisplayLevel.append(ui->chkD5);
    chkDisplayLevel.append(ui->chkD6);
    chkDisplayLevel.append(ui->chkD7);
    chkDisplayLevel.append(ui->chkD8);
    chkDisplayLevel.append(ui->chkD9);
    chkDisplayLevel.append(ui->chkD10);
    chkDisplayLevel.append(ui->chkD11);
    chkDisplayLevel.append(ui->chkD12);
    chkDisplayLevel.append(ui->chkD13);
    chkDisplayLevel.append(ui->chkD14);
    chkDisplayLevel.append(ui->chkD15);
    chkDisplayLevel.append(ui->chkD16);
    chkDisplayLevel.append(ui->chkD17);
    chkDisplayLevel.append(ui->chkD18);
    chkDisplayLevel.append(ui->chkD19);

    for (int i = 0; i < 19; i++) {
        chkDisplayLevel.at(i)->setChecked(false);
    }

    DisplayLevel();
}

void MainWindow::PickerAttributes()
{
    pav1 = 1;
    pav2 = 2;
    pav3 = 4;
    pav4 = 8;
    pav5 = 16;

    chk_pa.clear();
    chk_pa.append(ui->chkPA1);
    chk_pa.append(ui->chkPA2);
    chk_pa.append(ui->chkPA3);
    chk_pa.append(ui->chkPA4);
    chk_pa.append(ui->chkPA5);

    v_pa.clear();
    v_pa.append(pav1);
    v_pa.append(pav2);
    v_pa.append(pav3);
    v_pa.append(pav4);
    v_pa.append(pav5);

    for (int i = 0; i < v_pa.count(); i++) {
        if (!chk_pa.at(i)->isChecked()) {
            v_pa.remove(i);
            v_pa.insert(i, 0);
        }
    }

    int total = 0;
    for (int i = 0; i < v_pa.count(); i++) {
        total = total + v_pa.at(i);
    }

    ui->editPickerAttributes->setText(QString::number(total));
}

void MainWindow::on_chkPA1_clicked() { PickerAttributes(); }

void MainWindow::on_chkPA2_clicked() { PickerAttributes(); }

void MainWindow::on_chkPA3_clicked() { PickerAttributes(); }

void MainWindow::on_chkPA4_clicked() { PickerAttributes(); }

void MainWindow::on_chkPA5_clicked()
{
    PickerAttributes();
}

void MainWindow::on_editPickerAttributes_textChanged(const QString& arg1)
{
    int total = arg1.toInt();

    pav1 = 1;
    pav2 = 2;
    pav3 = 4;
    pav4 = 8;
    pav5 = 16;

    chk_pa.clear();
    chk_pa.append(ui->chkPA1);
    chk_pa.append(ui->chkPA2);
    chk_pa.append(ui->chkPA3);
    chk_pa.append(ui->chkPA4);
    chk_pa.append(ui->chkPA5);

    v_pa.clear();
    v_pa.append(pav1);
    v_pa.append(pav2);
    v_pa.append(pav3);
    v_pa.append(pav4);
    v_pa.append(pav5);

    scanPolicy = false;
    pickerAttributes = true;

    for (int i = 0; i < v_pa.count(); i++)
        chk_pa.at(i)->setChecked(false);

    method(v_pa, total);

    this->setWindowModified(true);
}

void MainWindow::contextMenuEvent(QContextMenuEvent* event)
{

    Q_UNUSED(event);

    QMenu menu(ui->table_nv_add);

    QAction* actionbootargs = new QAction("boot-args");

    menu.addAction(actionbootargs);

    // menu.exec();
}

void MainWindow::show_menu(const QPoint pos)
{

    if (ui->table_nv_add0->currentIndex().data().toString() == "7C436110-AB2A-4BBB-A880-FE41995C9F82") {

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

        menu->move(cursor().pos());
        menu->show();
        //获得鼠标点击的x，y坐标点
        int x = pos.x();
        int y = pos.y();
        QModelIndex index = ui->table_nv_add->indexAt(QPoint(x, y));
        int row = index.row(); //获得QTableWidget列表点击的行数
        QMessageBox box;
        box.setText(QString::number(row));
        // box.exec();
    }
}

void MainWindow::on_nv1()
{

    bool re = false;

    for (int i = 0; i < ui->table_nv_add->rowCount(); i++) {
        QString str;
        str = ui->table_nv_add->item(i, 0)->text();
        if (str == "boot-args") {
            ui->table_nv_add->setCurrentCell(i, 0);
            re = true;
        }
    }

    if (!re) {
        on_btnNVRAMAdd_Add_clicked();

        ui->table_nv_add->setItem(ui->table_nv_add->rowCount() - 1, 0,
            new QTableWidgetItem("boot-args"));

        QTableWidgetItem* newItem1 = new QTableWidgetItem("String");
        newItem1->setTextAlignment(Qt::AlignCenter);
        ui->table_nv_add->setItem(ui->table_nv_add->rowCount() - 1, 1, newItem1);
    }
}

void MainWindow::on_nv2()
{

    bool re = false;

    for (int i = 0; i < ui->table_nv_add->rowCount(); i++) {
        QString str;
        str = ui->table_nv_add->item(i, 0)->text();
        if (str == "bootercfg") {
            ui->table_nv_add->setCurrentCell(i, 0);
            re = true;
        }
    }

    if (!re) {
        on_btnNVRAMAdd_Add_clicked();

        ui->table_nv_add->setItem(ui->table_nv_add->rowCount() - 1, 0,
            new QTableWidgetItem("bootercfg"));

        QTableWidgetItem* newItem1 = new QTableWidgetItem("Data");
        newItem1->setTextAlignment(Qt::AlignCenter);
        ui->table_nv_add->setItem(ui->table_nv_add->rowCount() - 1, 1, newItem1);
    }
}

void MainWindow::on_nv3()
{

    bool re = false;

    for (int i = 0; i < ui->table_nv_add->rowCount(); i++) {
        QString str;
        str = ui->table_nv_add->item(i, 0)->text();
        if (str == "bootercfg-once") {
            ui->table_nv_add->setCurrentCell(i, 0);
            re = true;
        }
    }

    if (!re) {
        on_btnNVRAMAdd_Add_clicked();

        ui->table_nv_add->setItem(ui->table_nv_add->rowCount() - 1, 0,
            new QTableWidgetItem("bootercfg-once"));

        QTableWidgetItem* newItem1 = new QTableWidgetItem("Data");
        newItem1->setTextAlignment(Qt::AlignCenter);
        ui->table_nv_add->setItem(ui->table_nv_add->rowCount() - 1, 1, newItem1);
    }
}

void MainWindow::on_nv4()
{

    bool re = false;

    for (int i = 0; i < ui->table_nv_add->rowCount(); i++) {
        QString str;
        str = ui->table_nv_add->item(i, 0)->text();
        if (str == "efiboot-perf-record") {
            ui->table_nv_add->setCurrentCell(i, 0);
            re = true;
        }
    }

    if (!re) {
        on_btnNVRAMAdd_Add_clicked();

        ui->table_nv_add->setItem(ui->table_nv_add->rowCount() - 1, 0,
            new QTableWidgetItem("efiboot-perf-record"));

        QTableWidgetItem* newItem1 = new QTableWidgetItem("Data");
        newItem1->setTextAlignment(Qt::AlignCenter);
        ui->table_nv_add->setItem(ui->table_nv_add->rowCount() - 1, 1, newItem1);
    }
}

void MainWindow::on_nv5()
{

    bool re = false;

    for (int i = 0; i < ui->table_nv_add->rowCount(); i++) {
        QString str;
        str = ui->table_nv_add->item(i, 0)->text();
        if (str == "fmm-computer-name") {
            ui->table_nv_add->setCurrentCell(i, 0);
            re = true;
        }
    }

    if (!re) {
        on_btnNVRAMAdd_Add_clicked();

        ui->table_nv_add->setItem(ui->table_nv_add->rowCount() - 1, 0,
            new QTableWidgetItem("fmm-computer-name"));

        QTableWidgetItem* newItem1 = new QTableWidgetItem("String");
        newItem1->setTextAlignment(Qt::AlignCenter);
        ui->table_nv_add->setItem(ui->table_nv_add->rowCount() - 1, 1, newItem1);
    }
}

void MainWindow::on_nv6()
{

    bool re = false;

    for (int i = 0; i < ui->table_nv_add->rowCount(); i++) {
        QString str;
        str = ui->table_nv_add->item(i, 0)->text();
        if (str == "nvda_drv") {
            ui->table_nv_add->setCurrentCell(i, 0);
            re = true;
        }
    }

    if (!re) {
        on_btnNVRAMAdd_Add_clicked();

        ui->table_nv_add->setItem(ui->table_nv_add->rowCount() - 1, 0,
            new QTableWidgetItem("nvda_drv"));

        QTableWidgetItem* newItem1 = new QTableWidgetItem("String");
        newItem1->setTextAlignment(Qt::AlignCenter);
        ui->table_nv_add->setItem(ui->table_nv_add->rowCount() - 1, 1, newItem1);
    }
}

void MainWindow::on_nv7()
{

    bool re = false;

    for (int i = 0; i < ui->table_nv_add->rowCount(); i++) {
        QString str;
        str = ui->table_nv_add->item(i, 0)->text();
        if (str == "run-efi-updater") {
            ui->table_nv_add->setCurrentCell(i, 0);
            re = true;
        }
    }

    if (!re) {
        on_btnNVRAMAdd_Add_clicked();

        ui->table_nv_add->setItem(ui->table_nv_add->rowCount() - 1, 0,
            new QTableWidgetItem("run-efi-updater"));

        QTableWidgetItem* newItem1 = new QTableWidgetItem("String");
        newItem1->setTextAlignment(Qt::AlignCenter);
        ui->table_nv_add->setItem(ui->table_nv_add->rowCount() - 1, 1, newItem1);
    }
}

void MainWindow::on_nv8()
{

    bool re = false;

    for (int i = 0; i < ui->table_nv_add->rowCount(); i++) {
        QString str;
        str = ui->table_nv_add->item(i, 0)->text();
        if (str == "StartupMute") {
            ui->table_nv_add->setCurrentCell(i, 0);
            re = true;
        }
    }

    if (!re) {
        on_btnNVRAMAdd_Add_clicked();

        ui->table_nv_add->setItem(ui->table_nv_add->rowCount() - 1, 0,
            new QTableWidgetItem("StartupMute"));

        QTableWidgetItem* newItem1 = new QTableWidgetItem("Data");
        newItem1->setTextAlignment(Qt::AlignCenter);
        ui->table_nv_add->setItem(ui->table_nv_add->rowCount() - 1, 1, newItem1);
    }
}

void MainWindow::on_nv9()
{

    bool re = false;

    for (int i = 0; i < ui->table_nv_add->rowCount(); i++) {
        QString str;
        str = ui->table_nv_add->item(i, 0)->text();
        if (str == "SystemAudioVolume") {
            ui->table_nv_add->setCurrentCell(i, 0);
            re = true;
        }
    }

    if (!re) {
        on_btnNVRAMAdd_Add_clicked();

        ui->table_nv_add->setItem(ui->table_nv_add->rowCount() - 1, 0,
            new QTableWidgetItem("SystemAudioVolume"));

        QTableWidgetItem* newItem1 = new QTableWidgetItem("Data");
        newItem1->setTextAlignment(Qt::AlignCenter);
        ui->table_nv_add->setItem(ui->table_nv_add->rowCount() - 1, 1, newItem1);
    }
}

void MainWindow::on_nv10()
{

    bool re = false;

    for (int i = 0; i < ui->table_nv_add->rowCount(); i++) {
        QString str;
        str = ui->table_nv_add->item(i, 0)->text();
        if (str == "csr-active-config") {
            ui->table_nv_add->setCurrentCell(i, 0);
            re = true;
        }
    }

    if (!re) {
        on_btnNVRAMAdd_Add_clicked();

        ui->table_nv_add->setItem(ui->table_nv_add->rowCount() - 1, 0,
            new QTableWidgetItem("csr-active-config"));

        QTableWidgetItem* newItem1 = new QTableWidgetItem("Data");
        newItem1->setTextAlignment(Qt::AlignCenter);
        ui->table_nv_add->setItem(ui->table_nv_add->rowCount() - 1, 1, newItem1);
    }
}

void MainWindow::on_nv11()
{

    bool re = false;

    for (int i = 0; i < ui->table_nv_add->rowCount(); i++) {
        QString str;
        str = ui->table_nv_add->item(i, 0)->text();
        if (str == "prev-lang:kbd") {
            ui->table_nv_add->setCurrentCell(i, 0);
            re = true;
        }
    }

    if (!re) {
        on_btnNVRAMAdd_Add_clicked();

        ui->table_nv_add->setItem(ui->table_nv_add->rowCount() - 1, 0,
            new QTableWidgetItem("prev-lang:kbd"));

        QTableWidgetItem* newItem1 = new QTableWidgetItem("Data");
        newItem1->setTextAlignment(Qt::AlignCenter);
        ui->table_nv_add->setItem(ui->table_nv_add->rowCount() - 1, 1, newItem1);
    }
}

void MainWindow::on_nv12()
{

    bool re = false;

    for (int i = 0; i < ui->table_nv_add->rowCount(); i++) {
        QString str;
        str = ui->table_nv_add->item(i, 0)->text();
        if (str == "security-mode") {
            ui->table_nv_add->setCurrentCell(i, 0);
            re = true;
        }
    }

    if (!re) {
        on_btnNVRAMAdd_Add_clicked();

        ui->table_nv_add->setItem(ui->table_nv_add->rowCount() - 1, 0,
            new QTableWidgetItem("security-mode"));

        QTableWidgetItem* newItem1 = new QTableWidgetItem("Data");
        newItem1->setTextAlignment(Qt::AlignCenter);
        ui->table_nv_add->setItem(ui->table_nv_add->rowCount() - 1, 1, newItem1);
    }
}

void MainWindow::show_menu0(const QPoint pos)
{
    //设置菜单选项
    QMenu* menu = new QMenu(ui->table_nv_add0);

    QAction* act1 = new QAction("+  4D1EDE05-38C7-4A6A-9CC6-4BCCA8B38C14", ui->table_nv_add0);
    connect(act1, SIGNAL(triggered()), this, SLOT(on_nv01()));

    QAction* act2 = new QAction("+  7C436110-AB2A-4BBB-A880-FE41995C9F82", ui->table_nv_add0);
    connect(act2, SIGNAL(triggered()), this, SLOT(on_nv02()));

    QAction* act3 = new QAction("+  8BE4DF61-93CA-11D2-AA0D-00E098032B8C", ui->table_nv_add0);
    connect(act3, SIGNAL(triggered()), this, SLOT(on_nv03()));

    QAction* act4 = new QAction("+  4D1FDA02-38C7-4A6A-9CC6-4BCCA8B30102", ui->table_nv_add0);
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
    int row = index.row(); //获得QTableWidget列表点击的行数
    QMessageBox box;
    box.setText(QString::number(row));
    // box.exec();
}

void MainWindow::on_nv01()
{

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

void MainWindow::on_nv02()
{

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

void MainWindow::on_nv03()
{

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

void MainWindow::on_nv04()
{

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

void MainWindow::init_menu()
{
    //File
    connect(ui->actionOpen, &QAction::triggered, this,
        &MainWindow::on_btnOpen);
    ui->actionOpen->setShortcut(tr("ctrl+o"));

    connect(ui->actionSave, &QAction::triggered, this,
        &MainWindow::on_btnSave);
    ui->actionSave->setShortcut(tr("ctrl+s"));

    connect(ui->actionSave_As, &QAction::triggered, this,
        &MainWindow::on_btnSaveAs);
    ui->actionSave_As->setShortcut(tr("ctrl+shift+s"));

    //Tools
    connect(ui->btnMountEsp, &QAction::triggered, this, &MainWindow::on_btnMountEsp);
    ui->btnMountEsp->setShortcut(tr("ctrl+m"));

    connect(ui->btnOcvalidate, &QAction::triggered, this, &MainWindow::on_btnOcvalidate);
    ui->btnOcvalidate->setShortcut(tr("ctrl+l"));

    connect(ui->actionGenerateEFI, &QAction::triggered, this, &MainWindow::on_GenerateEFI);
    //ui->actionGenerateEFI->setShortcut(tr("ctrl+e"));

    connect(ui->actionDatabase, &QAction::triggered, this, &MainWindow::on_Database);
    ui->actionDatabase->setShortcut(tr("ctrl+d"));

    connect(ui->actionOpen_database_directory, &QAction::triggered, this, &MainWindow::on_btnOpenDir_clicked);

    connect(ui->actionShareConfig, &QAction::triggered, this, &MainWindow::on_ShareConfig);
    ui->actionShareConfig->setShortcut(tr("ctrl+r"));

    connect(ui->btnExportMaster, &QAction::triggered, this, &MainWindow::on_btnExportMaster);

    connect(ui->btnImportMaster, &QAction::triggered, this, &MainWindow::on_btnImportMaster);

    //Help
    connect(ui->btnHelp, &QAction::triggered, this, &MainWindow::on_btnHelp);
    ui->btnHelp->setShortcut(tr("ctrl+p"));

    connect(ui->btnCheckUpdate, &QAction::triggered, this, &MainWindow::on_btnCheckUpdate);
    ui->btnCheckUpdate->setShortcut(tr("ctrl+u"));

    connect(ui->actionAbout_2, &QAction::triggered, this, &MainWindow::about);

    connect(ui->actionOpenCore, &QAction::triggered, this, &MainWindow::on_line1);
    connect(ui->actionOpenCore_Factory, &QAction::triggered, this, &MainWindow::on_line2);
    connect(ui->actionOpenCore_Forum, &QAction::triggered, this, &MainWindow::on_line3);

    connect(ui->actionSimplified_Chinese_Manual, &QAction::triggered, this, &MainWindow::on_line4);
    if (!zh_cn)
        ui->actionSimplified_Chinese_Manual->setVisible(false);

    connect(ui->actionOpenCanopyIcons, &QAction::triggered, this,
        &MainWindow::on_line5);

    connect(ui->actionPlist_editor, &QAction::triggered, this,
        &MainWindow::on_line20);
    connect(ui->actionDSDT_SSDT_editor, &QAction::triggered, this,
        &MainWindow::on_line21);

    ui->actionSave->setEnabled(false);
}

void MainWindow::on_Database()
{
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
    QStringList files = dir.entryList(nameFilters, QDir::Files | QDir::Readable, QDir::Name);
    tableDatabase->setRowCount(0);
    tableDatabase->setRowCount(files.count());
    for (int i = 0; i < files.count(); i++) {
        QTableWidgetItem* newItem1;
        newItem1 = new QTableWidgetItem(files.at(i));
        tableDatabase->setItem(i, 0, newItem1);
    }
}

void MainWindow::on_line1()
{
    QUrl url(QString("https://github.com/acidanthera/OpenCorePkg/releases"));
    QDesktopServices::openUrl(url);
}

void MainWindow::on_line2()
{
    QUrl url(QString("https://github.com/acidanthera/OpenCorePkg/actions"));
    QDesktopServices::openUrl(url);
}

void MainWindow::on_line3()
{
    QUrl url(QString(
        "https://www.insanelymac.com/forum/topic/338516-opencore-discussion/"));
    QDesktopServices::openUrl(url);
}

void MainWindow::on_line4()
{
    QUrl url(QString("https://oc.skk.moe/"));
    QDesktopServices::openUrl(url);
}

void MainWindow::on_line5()
{
    QUrl url(QString("https://github.com/blackosx/OpenCanopyIcons"));
    QDesktopServices::openUrl(url);
}

void MainWindow::on_line20()
{
    QUrl url(QString(
        "https://www.insanelymac.com/forum/topic/"
        "345512-open-source-cross-platform-plist-file-editor-plistedplus/"));
    QDesktopServices::openUrl(url);
}

void MainWindow::on_line21()
{
    QUrl url(
        QString("https://www.insanelymac.com/forum/topic/"
                "344860-open-source-cross-platform-dsdtssdt-analysis-editor/"));
    QDesktopServices::openUrl(url);
}

void MainWindow::on_table_acpi_add_itemChanged(QTableWidgetItem* item)
{
    Q_UNUSED(item);
    this->setWindowModified(true);
    // qDebug() << item->text();
}

void MainWindow::on_table_acpi_add_currentItemChanged(
    QTableWidgetItem* current, QTableWidgetItem* previous)
{
    Q_UNUSED(current);
    Q_UNUSED(previous);
}

void MainWindow::on_table_acpi_del_itemChanged(QTableWidgetItem* item)
{
    Q_UNUSED(item);
    this->setWindowModified(true);
}

void MainWindow::on_table_acpi_patch_itemChanged(QTableWidgetItem* item)
{
    Q_UNUSED(item);
    this->setWindowModified(true);
}

void MainWindow::on_table_booter_itemChanged(QTableWidgetItem* item)
{
    Q_UNUSED(item);
    this->setWindowModified(true);
}

void MainWindow::on_table_dp_add0_itemChanged(QTableWidgetItem* item)
{
    Q_UNUSED(item);
    this->setWindowModified(true);
}

void MainWindow::on_table_dp_del0_itemChanged(QTableWidgetItem* item)
{
    Q_UNUSED(item);
    this->setWindowModified(true);
}

void MainWindow::on_table_kernel_add_itemChanged(QTableWidgetItem* item)
{
    Q_UNUSED(item);
    this->setWindowModified(true);
}

void MainWindow::on_table_kernel_block_itemChanged(QTableWidgetItem* item)
{
    Q_UNUSED(item);
    this->setWindowModified(true);
}

void MainWindow::on_table_kernel_Force_itemChanged(QTableWidgetItem* item)
{
    Q_UNUSED(item);
    this->setWindowModified(true);
}

void MainWindow::on_table_kernel_patch_itemChanged(QTableWidgetItem* item)
{
    Q_UNUSED(item);
    this->setWindowModified(true);
}

void MainWindow::on_tableBlessOverride_itemChanged(QTableWidgetItem* item)
{
    Q_UNUSED(item);
    this->setWindowModified(true);
}

void MainWindow::on_tableEntries_itemChanged(QTableWidgetItem* item)
{
    Q_UNUSED(item);
    this->setWindowModified(true);
}

void MainWindow::on_tableTools_itemChanged(QTableWidgetItem* item)
{
    Q_UNUSED(item);
    this->setWindowModified(true);
}

void MainWindow::on_table_nv_add0_itemChanged(QTableWidgetItem* item)
{
    Q_UNUSED(item);
    this->setWindowModified(true);
}

void MainWindow::on_table_nv_del0_itemChanged(QTableWidgetItem* item)
{
    Q_UNUSED(item);
    this->setWindowModified(true);
}

void MainWindow::on_table_nv_ls0_itemChanged(QTableWidgetItem* item)
{
    Q_UNUSED(item);
    this->setWindowModified(true);
}

void MainWindow::on_tableDevices_itemChanged(QTableWidgetItem* item)
{
    Q_UNUSED(item);
    this->setWindowModified(true);
}

void MainWindow::on_table_uefi_drivers_itemChanged(QTableWidgetItem* item)
{
    Q_UNUSED(item);
    this->setWindowModified(true);
}

void MainWindow::on_table_uefi_ReservedMemory_itemChanged(
    QTableWidgetItem* item)
{
    Q_UNUSED(item);
    this->setWindowModified(true);
}

void MainWindow::on_chkFadtEnableReset_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkNormalizeHeaders_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkRebaseRegions_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkResetHwSig_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkResetLogoStatus_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkAvoidRuntimeDefrag_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkEnableWriteUnprotector_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkRebuildAppleMemoryMap_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkEnableSafeModeSlide_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkDevirtualiseMmio_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkForceExitBootServices_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkSetupVirtualMap_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkAllowRelocationBlock_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkDisableSingleUser_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkProtectMemoryRegions_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkSignalAppleOS_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkDisableVariableWrite_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkProtectSecureBoot_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkSyncRuntimePermissions_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkDiscardHibernateMap_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkProtectUefiServices_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkProvideCustomSlide_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkAppleCpuPmCfgLock_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkDisableIoMapper_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkLapicKernelPanic_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkExternalDiskIcons_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkExtendBTFeatureFlags_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkForceSecureBootScheme_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkPowerTimeoutKernelPanic_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkLegacyCommpage_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkAppleXcpmForceBoost_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkAppleXcpmExtraMsrs_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkIncreasePciBarSize_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkCustomSMBIOSGuid_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkXhciPortLimit_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkThirdPartyDrives_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkDisableLinkeditJettison_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkPanicNoKextDump_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkDisableRtcChecksum_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkAppleXcpmCfgLock_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkFuzzyMatch_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkHideAuxiliary_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkPickerAudioAssist_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkPollAppleHotKeys_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkShowPicker_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkAppleDebug_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkApplePanic_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkDisableWatchDog_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkSysReport_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkSerialInit_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkAllowNvramReset_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkAllowSetDefault_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkAuthRestart_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkBlacklistAppleUpdate_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkEnablePassword_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkLegacyEnable_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkLegacyOverwrite_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkWriteFlash_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkAutomatic_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkCustomMemory_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkUpdateDataHub_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkUpdateNVRAM_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkUpdateSMBIOS_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkAdviseWindows_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkSpoofVendor_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkEnableJumpstart_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkJumpstartHotPlug_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkHideVerbose_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkGlobalConnect_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkIgnoreInvalidFlexRatio_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkReleaseUsbOwnership_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkRequestBootVarRouting_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkUnblockFsConnect_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkAudioSupport_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkConnectDrivers_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkKeyFiltering_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkKeySwap_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkKeySupport_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkPointerSupport_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkClearScreenOnModeSwitch_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkDirectGopRendering_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkReconnectOnResChange_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkReplaceTabWithSpace_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkForceResolution_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkProvideConsoleGop_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkIgnoreTextInGraphics_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkUgaPassThrough_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkSanitiseClearScreen_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkAppleAudio_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkAppleBootPolicy_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkAppleDebugLog_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkAppleEvent_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkAppleImg4Verification_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkAppleSecureBoot_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkAppleRtcRam_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkAppleKeyMap_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkAppleImageConversion_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkAppleFramebufferInfo_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkAppleSmcIo_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkAppleUserInterfaceTheme_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkDataHub_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkOSInfo_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkHashServices_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkFirmwareVolume_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkDeviceProperties_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkUnicodeCollation_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editProvideMaxSlide_textChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editCpuid1Data_textChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editCpuid1Mask_textChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_chkDummyPowerManagement_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editMaxKernel_textChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editMinKernel_textChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_cboxKernelArch_currentIndexChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_cboxKernelCache_currentIndexChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editConsoleAttributes_textChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editTakeoffDelay_textChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editTimeout_textChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_cboxHibernateMode_currentIndexChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_cboxPickerMode_currentIndexChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editDisplayDelay_textChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editTarget_textChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editHaltLevel_textChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editApECID_textChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_cboxBootProtect_currentIndexChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editPasswordHash_textChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editPasswordSalt_textChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_cboxDmgLoading_currentIndexChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_cboxVault_currentIndexChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_cboxSecureBootModel_currentIndexChanged(
    const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editSystemSerialNumber_textChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editMLB_textChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editSystemUUID_textChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_cboxSystemMemoryStatus_currentIndexChanged(
    const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editProcessorTypeGeneric_textChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editROM_textChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editARTFrequency_textChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editBoardProduct_textChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editBoardRevision_textChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editDevicePathsSupported_textChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editFSBFrequency_textChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editInitialTSC_textChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editPlatformName_textChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editSmcBranch_textChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editSmcPlatform_textChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editSmcRevision_textChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editStartupPowerEvents_textChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editSystemProductName_textChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editSystemSerialNumber_data_textChanged(
    const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editSystemUUID_data_textChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editDataWidth_textChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editTotalWidth_textChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editErrorCorrection_textChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editType_textChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editFormFactor_textChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editTypeDetail_textChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editMaxCapacity_textChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editBID_textChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editFirmwareFeatures_textChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editFirmwareFeaturesMask_textChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editMLB_2_textChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editROM_2_textChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editSystemUUID_PNVRAM_textChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editBIOSReleaseDate_textChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editBIOSVendor_textChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editBIOSVersion_textChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editBoardAssetTag_textChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editBoardLocationInChassis_textChanged(
    const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editBoardManufacturer_textChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editBoardProduct_2_textChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editBoardSerialNumber_textChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editBoardType_textChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editBoardVersion_textEdited(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editChassisAssetTag_textEdited(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editChassisManufacturer_textEdited(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editChassisSerialNumber_textEdited(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editChassisType_textEdited(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editSystemVersion_textEdited(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editSystemUUID_2_textEdited(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editSystemSerialNumber_2_textEdited(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editSystemSKUNumber_textEdited(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editSystemProductName_2_textEdited(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editSystemManufacturer_textEdited(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editSystemFamily_textEdited(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editSmcVersion_textEdited(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editProcessorType_textEdited(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editPlatformFeature_textEdited(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editFirmwareFeaturesMask_2_textEdited(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editFirmwareFeatures_2_textEdited(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editChassisVersion_textEdited(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editExitBootServicesDelay_textEdited(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editTscSyncTimeout_textEdited(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editMinDate_textEdited(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editMinVersion_textEdited(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editAudioCodec_textEdited(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editAudioDevice_textEdited(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editAudioOut_textEdited(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editMinimumVolume_textEdited(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editVolumeAmplifier_textEdited(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editKeyForgetThreshold_textEdited(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editKeyMergeThreshold_textEdited(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_cboxKeySupportMode_currentIndexChanged(
    const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editPointerSupportMode_textEdited(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_editTimerResolution_textEdited(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_cboxConsoleMode_currentIndexChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_cboxResolution_currentIndexChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_cboxTextRenderer_currentIndexChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::clear_temp_data()
{
    for (int i = 0; i < IniFile.count(); i++) {
        QFile file(IniFile.at(i));
        // qDebug() << IniFile.at(i);
        if (file.exists()) {
            file.remove();
            i = -1;
        }
    }
}

void MainWindow::on_table_dp_del_cellClicked(int row, int column)
{
    Q_UNUSED(row);
    Q_UNUSED(column);

    ui->statusbar->showMessage(ui->table_dp_del->currentItem()->text());
}

void MainWindow::on_tableBlessOverride_cellClicked(int row, int column)
{
    Q_UNUSED(row);
    Q_UNUSED(column);

    ui->statusbar->showMessage(ui->tableBlessOverride->currentItem()->text());
}

void MainWindow::on_table_nv_del_cellClicked(int row, int column)
{
    Q_UNUSED(row);
    Q_UNUSED(column);

    ui->statusbar->showMessage(ui->table_nv_del->currentItem()->text());
}

void MainWindow::on_table_nv_ls_cellClicked(int row, int column)
{
    Q_UNUSED(row);
    Q_UNUSED(column);

    ui->statusbar->showMessage(ui->table_nv_ls->currentItem()->text());
}

void MainWindow::on_tableDevices_cellClicked(int row, int column)
{
    Q_UNUSED(row);
    Q_UNUSED(column);

    ui->statusbar->showMessage(ui->tableDevices->currentItem()->text());
}

void MainWindow::on_table_uefi_drivers_cellClicked(int row, int column)
{
    Q_UNUSED(row);
    Q_UNUSED(column);

    ui->statusbar->showMessage(ui->table_uefi_drivers->currentItem()->text());
}

void MainWindow::on_btnOcvalidate()
{
    QFileInfo appInfo(qApp->applicationDirPath());
    chkdata = new QProcess;
#ifdef Q_OS_WIN32
    chkdata->start(appInfo.filePath() + "/ocvalidate.exe", QStringList() << SaveFileName);

#endif

#ifdef Q_OS_LINUX
    chkdata->start(appInfo.filePath() + "/ocvalidate", QStringList() << SaveFileName);

#endif

#ifdef Q_OS_MAC

    chkdata->start(appInfo.filePath() + "/ocvalidate", QStringList() << SaveFileName);
#endif
    connect(chkdata, SIGNAL(finished(int)), this, SLOT(readResultCheckData()));
}

void MainWindow::readResultCheckData()
{
    QString result = chkdata->readAll();
    QString str;

    if (result.trimmed() == "Failed to read")
        return;
    QMessageBox box;
    if (result.trimmed().mid(0, 4) == "Done") {
        str = tr("OK!");
        box.setText(result + "\n" + str);
    } else
        box.setText(result);

    box.exec();
}

void MainWindow::on_btnBooterPatchAdd_clicked()
{
    add_item(ui->table_Booter_patch, 10);
    init_enabled_data(ui->table_Booter_patch,
        ui->table_Booter_patch->rowCount() - 1, 9, "true");

    QTableWidgetItem* newItem1 = new QTableWidgetItem("Any");
    newItem1->setTextAlignment(Qt::AlignCenter);
    ui->table_Booter_patch->setItem(ui->table_Booter_patch->currentRow(), 10,
        newItem1);

    this->setWindowModified(true);
}

void MainWindow::on_btnBooterPatchDel_clicked()
{
    del_item(ui->table_Booter_patch);
}

void MainWindow::on_table_Booter_patch_cellClicked(int row, int column)
{
    if (!ui->table_Booter_patch->currentIndex().isValid())
        return;

    enabled_change(ui->table_Booter_patch, row, column, 9);

    if (column == 10) {

        cboxArch = new QComboBox;
        cboxArch->addItem("Any");
        cboxArch->addItem("i386");
        cboxArch->addItem("x86_64");
        cboxArch->addItem("");

        connect(cboxArch, SIGNAL(currentIndexChanged(QString)), this, SLOT(arch_Booter_patchChange()));
        c_row = row;

        ui->table_Booter_patch->setCellWidget(row, column, cboxArch);
        cboxArch->setCurrentText(ui->table_Booter_patch->item(row, 10)->text());
    }

    ui->statusbar->showMessage(ui->table_Booter_patch->currentItem()->text());
}

void MainWindow::on_table_Booter_patch_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    if (currentRow == 0 && currentColumn == 0 && previousColumn == 0) {
    }

    ui->table_Booter_patch->removeCellWidget(previousRow, 10);
}

void MainWindow::on_cboxPlayChime_currentTextChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_btnCheckUpdate()
{

    QNetworkRequest quest;
    quest.setUrl(QUrl("https://api.github.com/repos/ic005k/QtOpenCoreConfig/releases/latest"));
    quest.setHeader(QNetworkRequest::UserAgentHeader, "RT-Thread ART");
    manager->get(quest);
}

void MainWindow::replyFinished(QNetworkReply* reply)
{
    QString str = reply->readAll();
    QMessageBox box;
    box.setText(str);
    //box.exec();
    //qDebug() << QSslSocket::supportsSsl() << QSslSocket::sslLibraryBuildVersionString() << QSslSocket::sslLibraryVersionString();

    parse_UpdateJSON(str);

    reply->deleteLater();
}

int MainWindow::parse_UpdateJSON(QString str)
{

    QJsonParseError err_rpt;
    QJsonDocument root_Doc = QJsonDocument::fromJson(str.toUtf8(), &err_rpt);

    if (err_rpt.error != QJsonParseError::NoError) {
        QMessageBox::critical(this, "", tr("Network error!"));
        return -1;
    }
    if (root_Doc.isObject()) {
        QJsonObject root_Obj = root_Doc.object();

        QString macUrl, winUrl, linuxUrl;
        QVariantList list = root_Obj.value("assets").toArray().toVariantList();
        for (int i = 0; i < list.count(); i++) {
            QVariantMap map = list[i].toMap();
            QFileInfo file(map["name"].toString());
            if (file.suffix().toLower() == "zip")
                macUrl = map["browser_download_url"].toString();

            if (file.suffix().toLower() == "7z")
                winUrl = map["browser_download_url"].toString();

            if (file.suffix() == "AppImage")
                linuxUrl = map["browser_download_url"].toString();
        }

        QJsonObject PulseValue = root_Obj.value("assets").toObject();
        QString Verison = root_Obj.value("tag_name").toString();
        QString Url;
        if (mac)
            Url = macUrl;
        if (win)
            Url = winUrl;
        if (linuxOS)
            Url = linuxUrl;

        QString UpdateTime = root_Obj.value("published_at").toString();
        QString ReleaseNote = root_Obj.value("body").toString();

        if (Verison > CurVerison) {
            QString warningStr = tr("New version detected!") + "\n" + tr("Version: ") + "V" + Verison + "\n" + tr("Published at: ") + UpdateTime + "\n" + tr("Release Notes: ") + "\n" + ReleaseNote;
            int ret = QMessageBox::warning(this, "", warningStr, tr("Download"), tr("Cancel"));
            if (ret == 0) {
                QDesktopServices::openUrl(QUrl(Url));
            }
        } else
            QMessageBox::information(this, "", tr("It is currently the latest version!"));
    }
    return 0;
}

void MainWindow::on_chkSaveDataHub_clicked()
{
    this->setWindowModified(true);
}

void MainWindow::on_editSetupDelay_textChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_GenerateEFI()
{

    QDir dir;
    QString strDatabase;

    QFileInfo appInfo(qApp->applicationDirPath());
    QString pathSource = appInfo.filePath() + "/Database/";

    QString pathTarget = QDir::homePath() + "/Desktop/EFI/";

    deleteDirfile(pathTarget);

    if (dir.mkpath(pathTarget)) { }

    //BOOT
    QString pathBoot = pathTarget + "BOOT/";
    if (dir.mkpath(pathBoot)) { }
    QFile::copy(pathSource + "EFI/BOOT/BOOTx64.efi", pathBoot + "BOOTx64.efi");

    //OC/ACPI
    QString pathOCACPI = pathTarget + "OC/ACPI/";
    if (dir.mkpath(pathOCACPI)) { }
    for (int i = 0; i < ui->table_acpi_add->rowCount(); i++) {
        ui->table_acpi_add->setCurrentCell(i, 0);
        QString file = ui->table_acpi_add->currentItem()->text();
        QFileInfo fi(pathSource + "EFI/OC/ACPI/" + file);
        if (fi.exists())
            QFile::copy(pathSource + "EFI/OC/ACPI/" + file, pathOCACPI + file);
        else
            strDatabase = strDatabase + "EFI/OC/ACPI/" + file + "\n";
    }

    //OC/Bootstrap
    QString pathOCBootstrap = pathTarget + "OC/Bootstrap/";
    if (dir.mkpath(pathOCBootstrap)) { }
    QFile::copy(pathSource + "EFI/OC/Bootstrap/Bootstrap.efi", pathOCBootstrap + "Bootstrap.efi");

    //OC/Drivers
    QString pathOCDrivers = pathTarget + "OC/Drivers/";
    if (dir.mkpath(pathOCDrivers)) { }
    for (int i = 0; i < ui->table_uefi_drivers->rowCount(); i++) {
        ui->table_uefi_drivers->setCurrentCell(i, 0);
        QString file = ui->table_uefi_drivers->currentItem()->text();
        QString str0 = pathSource + "EFI/OC/Drivers/" + file;
        if (!str0.contains("#")) {
            QFileInfo fi(str0);
            if (fi.exists())
                QFile::copy(str0, pathOCDrivers + file);
            else
                strDatabase = strDatabase + "EFI/OC/Drivers/" + file + "\n";
        }
    }

    //OC/Kexts
    QString pathOCKexts = pathTarget + "OC/Kexts/";
    if (dir.mkpath(pathOCKexts)) { }
    for (int i = 0; i < ui->table_kernel_add->rowCount(); i++) {
        ui->table_kernel_add->setCurrentCell(i, 0);
        QString file = ui->table_kernel_add->currentItem()->text();
        QString str0 = pathSource + "EFI/OC/Kexts/" + file;
        QDir kextDir(str0);

        if (!str0.contains("#")) {

            if (kextDir.exists())
                copyDirectoryFiles(str0, pathOCKexts + file, true);
            else
                strDatabase = strDatabase + "EFI/OC/Kexts/" + file + "\n";
        }
    }

    //OC/Resources
    QString pathOCResources = pathTarget + "OC/Resources/";
    copyDirectoryFiles(pathSource + "EFI/OC/Resources/", pathOCResources, true);

    //OC/Tools
    QString pathOCTools = pathTarget + "OC/Tools/";
    if (dir.mkpath(pathOCTools)) { }
    for (int i = 0; i < ui->tableTools->rowCount(); i++) {
        ui->tableTools->setCurrentCell(i, 0);
        QString file = ui->tableTools->currentItem()->text();
        QString str0 = pathSource + "EFI/OC/Tools/" + file;
        if (!str0.contains("#")) {
            QFileInfo fi(str0);
            if (fi.exists())
                QFile::copy(str0, pathOCTools + file);
            else
                strDatabase = strDatabase + "EFI/OC/Tools/" + file + "\n";
        }
    }

    //OC/OpenCore.efi
    QFile::copy(pathSource + "EFI/OC/OpenCore.efi", pathTarget + "OC/OpenCore.efi");

    //OC/Config.plist
    SavePlist(pathTarget + "OC/Config.plist");

    QMessageBox box;
    if (strDatabase != "")
        box.setText(tr("Finished generating the EFI folder on the desktop.") + "\n" + tr("The following files do not exist in the database at the moment, please add them yourself:") + "\n" + strDatabase);
    else
        box.setText(tr("Finished generating the EFI folder on the desktop."));
    box.exec();
}

int MainWindow::deleteDirfile(QString dirName)
{
    QDir directory(dirName);
    if (!directory.exists()) {
        return true;
    }

    QString srcPath = QDir::toNativeSeparators(dirName);
    if (!srcPath.endsWith(QDir::separator()))
        srcPath += QDir::separator();

    QStringList fileNames = directory.entryList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);
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

bool MainWindow::DeleteDirectory(const QString& path)
{
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
bool MainWindow::copyFileToPath(QString sourceDir, QString toDir, bool coverFileIfExist)
{
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
    } //end if

    if (!QFile::copy(sourceDir, toDir)) {
        return false;
    }
    return true;
}

//拷贝文件夹：
bool MainWindow::copyDirectoryFiles(const QString& fromDir, const QString& toDir, bool coverFileIfExist)
{
    QDir sourceDir(fromDir);
    QDir targetDir(toDir);
    if (!targetDir.exists()) { /**< 如果目标目录不存在，则进行创建 */
        if (!targetDir.mkdir(targetDir.absolutePath()))
            return false;
    }

    QFileInfoList fileInfoList = sourceDir.entryInfoList();
    foreach (QFileInfo fileInfo, fileInfoList) {
        if (fileInfo.fileName() == "." || fileInfo.fileName() == "..")
            continue;

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

void MainWindow::on_ShareConfig()
{
    QUrl url(QString("https://github.com/ic005k/QtOpenCoreConfigDatabase/issues"));
    QDesktopServices::openUrl(url);
}

void MainWindow::on_cboxPickerVariant_currentTextChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    this->setWindowModified(true);
}

void MainWindow::on_tabACPI_currentChanged(int index)
{
    Q_UNUSED(index);
}

void MainWindow::on_btnOpenDir_clicked()
{
    QFileInfo appInfo(qApp->applicationDirPath());
    QString dirpath = appInfo.filePath() + "/Database/";
    QString dir = "file:" + dirpath;
    QDesktopServices::openUrl(QUrl(dir, QUrl::TolerantMode));
}
