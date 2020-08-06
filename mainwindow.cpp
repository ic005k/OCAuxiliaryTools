#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "plistparser.h"
#include "plistserializer.h"

#include <QFileDialog>
#include <QSettings>
#include <QBuffer>
#include <QDebug>
#include <QMessageBox>

QString PlistFileName;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    test(false);  //是否显示测试按钮

    //setWindowIcon(QIcon(QStringLiteral(":/icon.png")));

    title = "OpenCore配置文件编辑器 V0.6.0";
    setWindowTitle(title);

    initui_acpi();
    initui_booter();
    initui_dp();
    initui_kernel();
    initui_misc();
    initui_nvram();
    initui_PlatformInfo();
    initui_UEFI();

    ui->tabTotal->setCurrentIndex(0);
    ui->tabACPI->setCurrentIndex(0);
    ui->tabBooter->setCurrentIndex(0);
    ui->tabDP->setCurrentIndex(0);
    ui->tabKernel->setCurrentIndex(0);
    ui->tabMisc->setCurrentIndex(0);
    ui->tabNVRAM->setCurrentIndex(0);
    ui->tabPlatformInfo->setCurrentIndex(0);
    ui->tabUEFI->setCurrentIndex(0);

    //主菜单
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::on_btnOpen_clicked);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::on_btnSave_clicked);
    connect(ui->actionSave_As, &QAction::triggered, this, &MainWindow::on_btnSaveAs_clicked);
    connect(ui->actionAbout_2, &QAction::triggered, this, &MainWindow::about);

    ui->btnSave->setEnabled(false);
    ui->actionSave->setEnabled(false);

    QFileInfo appInfo(qApp->applicationFilePath());
    ui->statusbar->showMessage(tr("最后的编译时间(Last modified): ") + appInfo.lastModified().toString("yyyy-MM-dd hh:mm:ss"));

}

MainWindow::~MainWindow()
{
    delete ui;
}




void MainWindow::on_btnOpen_clicked()
{
    QFileDialog fd;

    PlistFileName = fd.getOpenFileName(this,"配置文件","","配置文件(*.plist);;所有文件(*.*)");
    if(!PlistFileName.isEmpty())
        setWindowTitle(title + "    " + PlistFileName);
    else
        return;

    QFile file(PlistFileName);
    QVariantMap map = PListParser::parsePList(&file).toMap();

    loadding = true;

    ParserACPI(map);
    ParserBooter(map);
    ParserDP(map);
    ParserKernel(map);
    ParserMisc(map);
    ParserNvram(map);
    ParserPlatformInfo(map);
    ParserUEFI(map);

    loadding = false;

    ui->btnSave->setEnabled(true);
    ui->actionSave->setEnabled(true);
}

void MainWindow::on_btnTestWrite_clicked()
{
        QString  plistPath = QDir::homePath() + "/xx.plist";
        qDebug() << plistPath;

        //QString  plistPath = "E:/xxx.plist";

        QSettings Reg(plistPath, QSettings::NativeFormat);

        Reg.setValue("ACPI", true);
        qDebug() << Reg.value("ACPI");

        int i = 1;
        QString str_key = "/key";
        QString str_class = "/class";
        QString str_value ="/value";

        Reg.setValue(QString::number(i) + str_key,"key1");
        Reg.setValue("1/class","class1");
        Reg.setValue("1/value","value1");

        qDebug() << Reg.value("1/key").toString();
        qDebug() << Reg.value("1/class").toString();
        qDebug() << Reg.value("1/value").toString();



}

void MainWindow::on_btnTestRead_clicked()
{

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
    //QVariantMap map = PListParser::parsePList(&buffer).toMap();
    QFile file(QDir::homePath() + "/com.xx.plist");
    QVariantMap map = PListParser::parsePList(&file).toMap();
    //QString val = map["test"].toString();
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
    //sub = foo["SomeKey"].toMap();
    QString data = "2147483648";
    sub["test"] = 12345;
    qDebug() << data.toLongLong();
    sub["A"] = 1;

    foo["SomeOtherKey"] = sub;


    foo["test2"] = true;
    foo["test3"] = data.toLongLong();

    //qDebug() << "PList:\n" << PListSerializer::toPList(foo).toNSString();

    PListSerializer::toPList(foo , QDir::homePath() + "/test.plist");
}

void MainWindow::ParserACPI(QVariantMap map)
{
    map = map["ACPI"].toMap();
    if(map.isEmpty())
        return;

    //分析"Add"
    QVariantList map_add = map["Add"].toList();
    //qDebug() << map_add;
    ui->table_acpi_add->setRowCount(map_add.count());//设置行的列数
    for(int i = 0;i < map_add.count(); i++)
    {
        QVariantMap map3 = map_add.at(i).toMap();

        QTableWidgetItem *newItem1;
        newItem1 = new QTableWidgetItem(map3["Path"].toString());
        ui->table_acpi_add->setItem(i, 0, newItem1);
        newItem1 = new QTableWidgetItem(map3["Comment"].toString());
        ui->table_acpi_add->setItem(i, 1, newItem1);

        init_enabled_data(ui->table_acpi_add , i , 2 , map3["Enabled"].toString());


    }

    //分析Delete
    QVariantList map_del = map["Delete"].toList();
    //qDebug() << map_del;
    ui->table_acpi_del->setRowCount(map_del.count());//设置行的列数
    for(int i = 0;i < map_del.count(); i++)
    {
        QVariantMap map3 = map_del.at(i).toMap();

        QTableWidgetItem *newItem1;
        newItem1 = new QTableWidgetItem(ByteToHexStr(map3["TableSignature"].toByteArray()));
        ui->table_acpi_del->setItem(i, 0, newItem1);

        newItem1 = new QTableWidgetItem(ByteToHexStr(map3["OemTableId"].toByteArray()));
        ui->table_acpi_del->setItem(i, 1, newItem1);

        newItem1 = new QTableWidgetItem(map3["TableLength"].toString());
        ui->table_acpi_del->setItem(i, 2, newItem1);

        newItem1 = new QTableWidgetItem(map3["Comment"].toString());
        ui->table_acpi_del->setItem(i, 3, newItem1);

        init_enabled_data(ui->table_acpi_del , i , 4 , map3["All"].toString());

        init_enabled_data(ui->table_acpi_del , i , 5 , map3["Enabled"].toString());

    }

    //分析Patch
    QVariantList map_patch = map["Patch"].toList();
    ui->table_acpi_patch->setRowCount(map_patch.count());//设置行的列数
    for(int i = 0;i < map_patch.count(); i++)
    {
        QVariantMap map3 = map_patch.at(i).toMap();

        QTableWidgetItem *newItem1;
        newItem1 = new QTableWidgetItem(map3["TableSignature"].toString());
        ui->table_acpi_patch->setItem(i, 0, newItem1);

        newItem1 = new QTableWidgetItem(map3["OemTableId"].toString());
        ui->table_acpi_patch->setItem(i, 1, newItem1);

        newItem1 = new QTableWidgetItem(map3["TableLength"].toString());
        ui->table_acpi_patch->setItem(i, 2, newItem1);

        //newItem1 = new QTableWidgetItem(map3["Find"].toString());
        ui->table_acpi_patch->setItem(i, 3, new QTableWidgetItem(ByteToHexStr(map3["Find"].toByteArray())));

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

        init_enabled_data(ui->table_acpi_patch , i , 11 , map3["Enabled"].toString());

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

    QTableWidgetItem *id0;
    //ACPI-Add
    ui->table_acpi_add->setColumnWidth(0,450);
    id0 = new QTableWidgetItem(tr("文件\nPath"));
    ui->table_acpi_add->setHorizontalHeaderItem(0, id0);

    ui->table_acpi_add->setColumnWidth(1,560);
    id0 = new QTableWidgetItem(tr("注释\nComment"));
    ui->table_acpi_add->setHorizontalHeaderItem(1, id0);

    id0 = new QTableWidgetItem(tr("启用\nEnabled"));
    ui->table_acpi_add->setHorizontalHeaderItem(2, id0);

    //ACPI-Delete
    id0=new QTableWidgetItem(tr("表签名\nTableSignature"));
    ui->table_acpi_del->setHorizontalHeaderItem(0, id0);

    ui->table_acpi_del->setColumnWidth(1,250);
    id0 = new QTableWidgetItem(tr("OemTableId"));
    ui->table_acpi_del->setHorizontalHeaderItem(1,id0);

    id0 = new QTableWidgetItem(tr("表长度\nTableLength"));
    ui->table_acpi_del->setHorizontalHeaderItem(2,id0);

    ui->table_acpi_del->setColumnWidth(3,560);
    id0 = new QTableWidgetItem(tr("注释\nComment"));
    ui->table_acpi_del->setHorizontalHeaderItem(3,id0);

    id0 = new QTableWidgetItem(tr("全部\nAll"));
    ui->table_acpi_del->setHorizontalHeaderItem(4,id0);

    id0 = new QTableWidgetItem(tr("启用\nEnabled"));
    ui->table_acpi_del->setHorizontalHeaderItem(5,id0);

    //ACPI-Patch
    ui->table_acpi_patch->setColumnWidth(0,100);
    id0=new QTableWidgetItem(tr("表签名\nTableSignature"));
    ui->table_acpi_patch->setHorizontalHeaderItem(0, id0);

    id0 = new QTableWidgetItem(tr("OemTableId"));
    ui->table_acpi_patch->setHorizontalHeaderItem(1,id0);

    id0 = new QTableWidgetItem(tr("表长度\nTableLength"));
    ui->table_acpi_patch->setHorizontalHeaderItem(2,id0);

    ui->table_acpi_patch->setColumnWidth(3,200);
    id0 = new QTableWidgetItem(tr("查找\nFind"));
    ui->table_acpi_patch->setHorizontalHeaderItem(3,id0);

    ui->table_acpi_patch->setColumnWidth(4,200);
    id0 = new QTableWidgetItem(tr("替换\nReplace"));
    ui->table_acpi_patch->setHorizontalHeaderItem(4,id0);

    ui->table_acpi_patch->setColumnWidth(5,300);
    id0 = new QTableWidgetItem(tr("注释\nComment"));
    ui->table_acpi_patch->setHorizontalHeaderItem(5,id0);

    id0 = new QTableWidgetItem(tr("Mask"));
    ui->table_acpi_patch->setHorizontalHeaderItem(6,id0);

    id0 = new QTableWidgetItem(tr("替换Mask\nReplaceMask"));
    ui->table_acpi_patch->setHorizontalHeaderItem(7,id0);

    id0 = new QTableWidgetItem(tr("替换计数\nCount"));
    ui->table_acpi_patch->setHorizontalHeaderItem(8,id0);

    id0 = new QTableWidgetItem(tr("限制\nLimit"));
    ui->table_acpi_patch->setHorizontalHeaderItem(9,id0);

    id0 = new QTableWidgetItem(tr("跳过\nSkip"));
    ui->table_acpi_patch->setHorizontalHeaderItem(10,id0);

    id0 = new QTableWidgetItem(tr("启用\nEnabled"));
    ui->table_acpi_patch->setHorizontalHeaderItem(11,id0);


}

void MainWindow::initui_booter()
{
    QTableWidgetItem *id0;

    ui->table_booter->setColumnWidth(0,450);
    id0 = new QTableWidgetItem(tr("地址\nAddress"));
    ui->table_booter->setHorizontalHeaderItem(0, id0);

    ui->table_booter->setColumnWidth(1,600);
    id0 = new QTableWidgetItem(tr("注释\nComment"));
    ui->table_booter->setHorizontalHeaderItem(1, id0);

    id0 = new QTableWidgetItem(tr("启用\nEnabled"));
    ui->table_booter->setHorizontalHeaderItem(2, id0);

}

void MainWindow::ParserBooter(QVariantMap map)
{
    map = map["Booter"].toMap();
    if(map.isEmpty())
        return;

    QVariantList map_add = map["MmioWhitelist"].toList();

    //qDebug() << map_add;
    ui->table_booter->setRowCount(map_add.count());//设置行的列数
    for(int i = 0;i < map_add.count(); i++)
    {
        QVariantMap map3 = map_add.at(i).toMap();

        QTableWidgetItem *newItem1;
        newItem1 = new QTableWidgetItem(map3["Address"].toString());
        ui->table_booter->setItem(i, 0, newItem1);

        newItem1 = new QTableWidgetItem(map3["Comment"].toString());
        ui->table_booter->setItem(i, 1, newItem1);

        init_enabled_data(ui->table_booter , i , 2 , map3["Enabled"].toString());

    }

    //Quirks
    QVariantMap map_quirks = map["Quirks"].toMap();

    ui->chkAvoidRuntimeDefrag->setChecked(map_quirks["AvoidRuntimeDefrag"].toBool());
    ui->chkDevirtualiseMmio->setChecked(map_quirks["DevirtualiseMmio"].toBool());
    ui->chkDisableSingleUser->setChecked(map_quirks["DisableSingleUser"].toBool());
    ui->chkDisableVariableWrite->setChecked(map_quirks["DisableVariableWrite"].toBool());
    ui->chkDiscardHibernateMap->setChecked(map_quirks["DiscardHibernateMap"].toBool());
    ui->chkEnableSafeModeSlide->setChecked(map_quirks["EnableSafeModeSlide"].toBool());
    ui->chkEnableWriteUnprotector->setChecked(map_quirks["EnableWriteUnprotector"].toBool());
    ui->chkForceExitBootServices->setChecked(map_quirks["ForceExitBootServices"].toBool());
    ui->chkProtectMemoryRegions->setChecked(map_quirks["ProtectMemoryRegions"].toBool());
    ui->chkProtectSecureBoot->setChecked(map_quirks["ProtectSecureBoot"].toBool());
    ui->chkProtectUefiServices->setChecked(map_quirks["ProtectUefiServices"].toBool());
    ui->chkProvideCustomSlide->setChecked(map_quirks["ProvideCustomSlide"].toBool());
    ui->editProvideMaxSlide->setText(map_quirks["ProvideMaxSlide"].toString());
    ui->chkRebuildAppleMemoryMap->setChecked(map_quirks["RebuildAppleMemoryMap"].toBool());
    ui->chkSetupVirtualMap->setChecked(map_quirks["SetupVirtualMap"].toBool());
    ui->chkSignalAppleOS->setChecked(map_quirks["SignalAppleOS"].toBool());
    ui->chkSyncRuntimePermissions->setChecked(map_quirks["SyncRuntimePermissions"].toBool());




}

void MainWindow::initui_dp()
{
    QTableWidgetItem *id0;

    //Add
    ui->table_dp_add0->setColumnWidth(0,400);
    ui->table_dp_add0->setMinimumWidth(400);
    id0 = new QTableWidgetItem(tr("PCI设备列表\nPCILists"));
    ui->table_dp_add0->setHorizontalHeaderItem(0, id0);


    ui->table_dp_add->setColumnWidth(0,360);
    id0 = new QTableWidgetItem(tr("键\nKey"));
    ui->table_dp_add->setHorizontalHeaderItem(0, id0);

    ui->table_dp_add->setColumnWidth(2,250);
    id0 = new QTableWidgetItem(tr("值\nValue"));
    ui->table_dp_add->setHorizontalHeaderItem(2, id0);

    id0 = new QTableWidgetItem(tr("数据类型\nClass"));
    ui->table_dp_add->setHorizontalHeaderItem(1, id0);

    //Delete

    ui->table_dp_del0->setColumnWidth(0,400);
    ui->table_dp_del0->setMinimumWidth(400);
    id0 = new QTableWidgetItem(tr("PCI设备列表\nPCILists"));
    ui->table_dp_del0->setHorizontalHeaderItem(0, id0);

    ui->table_dp_del->setColumnWidth(0,780);
    id0 = new QTableWidgetItem(tr("值\nValue"));
    ui->table_dp_del->setHorizontalHeaderItem(0, id0);


}

void MainWindow::ParserDP(QVariantMap map)
{
    map = map["DeviceProperties"].toMap();
    if(map.isEmpty())
        return;

    //Add
    QVariantMap map_add = map["Add"].toMap();

    QVariantMap map_sub;

    ui->table_dp_add0->setRowCount(map_add.count());
    QTableWidgetItem *newItem1;
    for(int i = 0; i < map_add.count(); i ++)
    {
        newItem1 = new QTableWidgetItem(map_add.keys().at(i));
        ui->table_dp_add0->setItem(i, 0, newItem1);


        //加载子条目
        map_sub = map_add[map_add.keys().at(i)].toMap();
        ui->table_dp_add->setRowCount(map_sub.keys().count()); //子键的个数


        for(int j = 0; j < map_sub.keys().count(); j++)
        {

            //QTableWidgetItem *newItem1;
            newItem1 = new QTableWidgetItem(map_sub.keys().at(j));//键
            ui->table_dp_add->setItem(j, 0, newItem1);

            QString dtype = map_sub[map_sub.keys().at(j)].typeName();
            QString ztype;
            if(dtype == "QByteArray")
                ztype = "Data";
            if(dtype == "QString")
                ztype = "String";
            if(dtype == "qlonglong")
                ztype = "Number";

            newItem1 = new QTableWidgetItem(ztype);//数据类型
            newItem1->setTextAlignment(Qt::AlignCenter);
            ui->table_dp_add->setItem(j, 1, newItem1);

            QString type_name = map_sub[map_sub.keys().at(j)].typeName();
            if( type_name == "QByteArray")
            {
                QByteArray tohex = map_sub[map_sub.keys().at(j)].toByteArray();
                QString va = tohex.toHex().toUpper();
                newItem1 = new QTableWidgetItem(va);

            }
            else
            newItem1 = new QTableWidgetItem(map_sub[map_sub.keys().at(j)].toString());
            ui->table_dp_add->setItem(j, 2, newItem1);

        }

        //保存子条目里面的数据，以便以后加载

        write_ini("table_dp_add0" , ui->table_dp_add, i);


    }

    int last = ui->table_dp_add0->rowCount();
    ui->table_dp_add0->setCurrentCell(last - 1 , 0);


    //Delete
    init_value(map["Delete"].toMap() , ui->table_dp_del0 , ui->table_dp_del);



}

void MainWindow::initui_kernel()
{
    QTableWidgetItem *id0;
    //Add
    ui->table_kernel_add->setColumnWidth(0,250);
    id0 = new QTableWidgetItem(tr("捆绑路径\nBundlePath"));
    ui->table_kernel_add->setHorizontalHeaderItem(0, id0);

    ui->table_kernel_add->setColumnWidth(1,250);
    id0 = new QTableWidgetItem(tr("注释\nComment"));
    ui->table_kernel_add->setHorizontalHeaderItem(1, id0);

    ui->table_kernel_add->setColumnWidth(2,250);
    id0 = new QTableWidgetItem(tr("二进制文件路径\nExecutablePath"));
    ui->table_kernel_add->setHorizontalHeaderItem(2, id0);

    ui->table_kernel_add->setColumnWidth(0,250);
    id0 = new QTableWidgetItem(tr("Plist路径\nPlistPath"));
    ui->table_kernel_add->setHorizontalHeaderItem(3, id0);

    ui->table_kernel_add->setColumnWidth(0,250);
    id0 = new QTableWidgetItem(tr("最小内核\nMinKernel"));
    ui->table_kernel_add->setHorizontalHeaderItem(4, id0);

    ui->table_kernel_add->setColumnWidth(0,250);
    id0 = new QTableWidgetItem(tr("最大内核\nMaxKernel"));
    ui->table_kernel_add->setHorizontalHeaderItem(5, id0);

    ui->table_kernel_add->setColumnWidth(0,250);
    id0 = new QTableWidgetItem(tr("启用\nEnabled"));
    ui->table_kernel_add->setHorizontalHeaderItem(6, id0);

    //Block
    ui->table_kernel_block->setColumnWidth(0,350);
    id0 = new QTableWidgetItem(tr("标识符\nIdentifier"));
    ui->table_kernel_block->setHorizontalHeaderItem(0, id0);

    ui->table_kernel_block->setColumnWidth(1,350);
    id0 = new QTableWidgetItem(tr("注释\nComment"));
    ui->table_kernel_block->setHorizontalHeaderItem(1, id0);

    ui->table_kernel_block->setColumnWidth(2,250);
    id0 = new QTableWidgetItem(tr("最小内核\nMinKernel"));
    ui->table_kernel_block->setHorizontalHeaderItem(2, id0);

    ui->table_kernel_block->setColumnWidth(3,250);
    id0 = new QTableWidgetItem(tr("最大内核\nMaxKernel"));
    ui->table_kernel_block->setHorizontalHeaderItem(3, id0);

    ui->table_kernel_block->setColumnWidth(0,250);
    id0 = new QTableWidgetItem(tr("启用\nEnabled"));
    ui->table_kernel_block->setHorizontalHeaderItem(4, id0);

    //Patch
    ui->table_kernel_patch->setColumnWidth(0,300);
    id0 = new QTableWidgetItem(tr("标识符\nIdentifier"));
    ui->table_kernel_patch->setHorizontalHeaderItem(0, id0);

    ui->table_kernel_patch->setColumnWidth(0,350);
    id0 = new QTableWidgetItem(tr("基本\nBase"));
    ui->table_kernel_patch->setHorizontalHeaderItem(1, id0);

    ui->table_kernel_patch->setColumnWidth(0,350);
    id0 = new QTableWidgetItem(tr("注释\nIdentifier"));
    ui->table_kernel_patch->setHorizontalHeaderItem(2, id0);

    ui->table_kernel_patch->setColumnWidth(3,300);
    id0 = new QTableWidgetItem(tr("查找\nFind"));
    ui->table_kernel_patch->setHorizontalHeaderItem(3, id0);

    ui->table_kernel_patch->setColumnWidth(4,300);
    id0 = new QTableWidgetItem(tr("替换\nReplace"));
    ui->table_kernel_patch->setHorizontalHeaderItem(4, id0);

    ui->table_kernel_patch->setColumnWidth(0,350);
    id0 = new QTableWidgetItem(tr("Mask"));
    ui->table_kernel_patch->setHorizontalHeaderItem(5, id0);

    ui->table_kernel_patch->setColumnWidth(0,350);
    id0 = new QTableWidgetItem(tr("替换Mask\nReplaceMask"));
    ui->table_kernel_patch->setHorizontalHeaderItem(6, id0);

    ui->table_kernel_patch->setColumnWidth(0,350);
    id0 = new QTableWidgetItem(tr("最小内核\nMinKernel"));
    ui->table_kernel_patch->setHorizontalHeaderItem(7, id0);

    ui->table_kernel_patch->setColumnWidth(0,350);
    id0 = new QTableWidgetItem(tr("最大内核\nMaxKernel"));
    ui->table_kernel_patch->setHorizontalHeaderItem(8, id0);

    ui->table_kernel_patch->setColumnWidth(0,350);
    id0 = new QTableWidgetItem(tr("替换次数\nCount"));
    ui->table_kernel_patch->setHorizontalHeaderItem(9, id0);

    ui->table_kernel_patch->setColumnWidth(0,350);
    id0 = new QTableWidgetItem(tr("限制\nLimit"));
    ui->table_kernel_patch->setHorizontalHeaderItem(10, id0);

    ui->table_kernel_patch->setColumnWidth(0,350);
    id0 = new QTableWidgetItem(tr("跳过次数\nSkip"));
    ui->table_kernel_patch->setHorizontalHeaderItem(11, id0);

    ui->table_kernel_patch->setColumnWidth(0,350);
    id0 = new QTableWidgetItem(tr("启用\nEnabled"));
    ui->table_kernel_patch->setHorizontalHeaderItem(12, id0);



}

void MainWindow::ParserKernel(QVariantMap map)
{
    map = map["Kernel"].toMap();
    if(map.isEmpty())
        return;

    //分析"Add"
    QVariantList map_add = map["Add"].toList();

    ui->table_kernel_add->setRowCount(map_add.count());//设置行的列数
    for(int i = 0;i < map_add.count(); i++)
    {
        QVariantMap map3 = map_add.at(i).toMap();

        QTableWidgetItem *newItem1;

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


        init_enabled_data(ui->table_kernel_add , i , 6 , map3["Enabled"].toString());

    }

    //Block
    QVariantList map_block = map["Block"].toList();

    ui->table_kernel_block->setRowCount(map_block.count());//设置行的列数
    for(int i = 0;i < map_block.count(); i++)
    {
        QVariantMap map3 = map_block.at(i).toMap();


        QTableWidgetItem *newItem1;

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

        init_enabled_data(ui->table_kernel_block , i , 4 , map3["Enabled"].toString());



    }

    //Patch
    QVariantList map_patch = map["Patch"].toList();

    ui->table_kernel_patch->setRowCount(map_patch.count());//设置行的列数
    for(int i = 0;i < map_patch.count(); i++)
    {
        QVariantMap map3 = map_patch.at(i).toMap();

        QByteArray ba = map3["Find"].toByteArray();

        QTableWidgetItem *newItem1;

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

        init_enabled_data(ui->table_kernel_patch , i , 12 , map3["Enabled"].toString());



    }
    //Emulate
    QVariantMap map_Emulate = map["Emulate"].toMap();
    ui->editCpuid1Data->setText(ByteToHexStr(map_Emulate["Cpuid1Data"].toByteArray()));
    ui->editCpuid1Mask->setText(ByteToHexStr(map_Emulate["Cpuid1Mask"].toByteArray()));


    //Quirks
    QVariantMap map_quirks = map["Quirks"].toMap();

    ui->chkAppleCpuPmCfgLock->setChecked(map_quirks["AppleCpuPmCfgLock"].toBool());
    ui->chkAppleXcpmCfgLock->setChecked(map_quirks["AppleXcpmCfgLock"].toBool());
    ui->chkAppleXcpmExtraMsrs->setChecked(map_quirks["AppleXcpmExtraMsrs"].toBool());
    ui->chkAppleXcpmForceBoost->setChecked(map_quirks["AppleXcpmForceBoost"].toBool());
    ui->chkCustomSMBIOSGuid->setChecked(map_quirks["CustomSMBIOSGuid"].toBool());
    ui->chkDisableIoMapper->setChecked(map_quirks["DisableIoMapper"].toBool());
    ui->chkDisableRtcChecksum->setChecked(map_quirks["DisableRtcChecksum"].toBool());
    ui->chkDummyPowerManagement->setChecked(map_quirks["DummyPowerManagement"].toBool());
    ui->chkExternalDiskIcons->setChecked(map_quirks["ExternalDiskIcons"].toBool());
    ui->chkIncreasePciBarSize->setChecked(map_quirks["IncreasePciBarSize"].toBool());
    ui->chkLapicKernelPanic->setChecked(map_quirks["LapicKernelPanic"].toBool());
    ui->chkPanicNoKextDump->setChecked(map_quirks["PanicNoKextDump"].toBool());
    ui->chkPowerTimeoutKernelPanic->setChecked(map_quirks["PowerTimeoutKernelPanic"].toBool());
    ui->chkThirdPartyDrives->setChecked(map_quirks["ThirdPartyDrives"].toBool());
    ui->chkXhciPortLimit->setChecked(map_quirks["XhciPortLimit"].toBool());


}

void MainWindow::initui_misc()
{
    //Boot
    ui->cboxHibernateMode->addItem("None");
    ui->cboxHibernateMode->addItem("Auto");
    ui->cboxHibernateMode->addItem("RTC");
    ui->cboxHibernateMode->addItem("NVRAM");

    ui->cboxPickerMode->addItem("Builtin");
    ui->cboxPickerMode->addItem("External");
    ui->cboxPickerMode->addItem("Apple");

    //Security
    ui->cboxBootProtect->addItem("None");
    ui->cboxBootProtect->addItem("Bootstrap");

    ui->cboxVault->addItem("Optional");
    ui->cboxVault->addItem("Basic");
    ui->cboxVault->addItem("Secure");

    //BlessOverride
    QTableWidgetItem *id0;
    ui->tableBlessOverride->setColumnWidth(0,1150);
    id0 = new QTableWidgetItem(tr("自定义启动项绝对路径\nBlessOverride"));
    ui->tableBlessOverride->setHorizontalHeaderItem(0, id0);

    //Entries
    ui->tableEntries->setColumnWidth(0,550);
    id0 = new QTableWidgetItem(tr("路径\nPath"));
    ui->tableEntries->setHorizontalHeaderItem(0, id0);

    id0 = new QTableWidgetItem(tr("参数\nArguments"));
    ui->tableEntries->setHorizontalHeaderItem(1, id0);

    id0 = new QTableWidgetItem(tr("名称\nName"));
    ui->tableEntries->setHorizontalHeaderItem(2, id0);

    ui->tableEntries->setColumnWidth(3,250);
    id0 = new QTableWidgetItem(tr("注释\nComment"));
    ui->tableEntries->setHorizontalHeaderItem(3, id0);

    id0 = new QTableWidgetItem(tr("辅助\nAuxiliary"));
    ui->tableEntries->setHorizontalHeaderItem(4, id0);

    id0 = new QTableWidgetItem(tr("启用\nEnabled"));
    ui->tableEntries->setHorizontalHeaderItem(5, id0);

    //Tools
    ui->tableTools->setColumnWidth(0 , 450);
    id0 = new QTableWidgetItem(tr("路径\nPath"));
    ui->tableTools->setHorizontalHeaderItem(0, id0);

    id0 = new QTableWidgetItem(tr("参数\nArguments"));
    ui->tableTools->setHorizontalHeaderItem(1, id0);

    id0 = new QTableWidgetItem(tr("名称\nName"));
    ui->tableTools->setHorizontalHeaderItem(2, id0);

    ui->tableTools->setColumnWidth(3,250);
    id0 = new QTableWidgetItem(tr("注释\nComment"));
    ui->tableTools->setHorizontalHeaderItem(3, id0);

    id0 = new QTableWidgetItem(tr("辅助\nAuxiliary"));
    ui->tableTools->setHorizontalHeaderItem(4, id0);

    id0 = new QTableWidgetItem(tr("启用\nEnabled"));
    ui->tableTools->setHorizontalHeaderItem(5, id0);


}

void MainWindow::ParserMisc(QVariantMap map)
{
    map = map["Misc"].toMap();
    if(map.isEmpty())
        return;

    //分析"Boot"
    QVariantMap map_boot = map["Boot"].toMap();

    ui->editConsoleAttributes->setText(map_boot["ConsoleAttributes"].toString());

    QString hm = map_boot["HibernateMode"].toString();
    if(hm.trimmed() == "None")
        ui->cboxHibernateMode->setCurrentIndex(0);
    if(hm.trimmed() == "Auto")
        ui->cboxHibernateMode->setCurrentIndex(1);
    if(hm.trimmed() == "RTC")
        ui->cboxHibernateMode->setCurrentIndex(2);
    if(hm.trimmed() == "NVRAM")
        ui->cboxHibernateMode->setCurrentIndex(3);

    ui->chkHideAuxiliary->setChecked(map_boot["HideAuxiliary"].toBool());
    ui->editPickerAttributes->setText(map_boot["PickerAttributes"].toString());
    ui->chkPickerAudioAssist->setChecked(map_boot["PickerAudioAssist"].toBool());

    hm = map_boot["PickerMode"].toString();
    if(hm.trimmed() == "Builtin")
        ui->cboxPickerMode->setCurrentIndex(0);
    if(hm.trimmed() == "External")
        ui->cboxPickerMode->setCurrentIndex(1);
    if(hm.trimmed() == "Apple")
        ui->cboxPickerMode->setCurrentIndex(2);

    ui->chkPollAppleHotKeys->setChecked(map_boot["PollAppleHotKeys"].toBool());
    ui->chkShowPicker->setChecked(map_boot["ShowPicker"].toBool());
    ui->editTakeoffDelay->setText(map_boot["TakeoffDelay"].toString());
    ui->editTimeout->setText(map_boot["Timeout"].toString());

    //Debug
    QVariantMap map_debug = map["Debug"].toMap();
    ui->chkAppleDebug->setChecked(map_debug["AppleDebug"].toBool());
    ui->chkApplePanic->setChecked(map_debug["ApplePanic"].toBool());
    ui->chkDisableWatchDog->setChecked(map_debug["DisableWatchDog"].toBool());

    ui->editDisplayDelay->setText(map_debug["DisplayDelay"].toString());

    ui->editDisplayLevel->setText(map_debug["DisplayLevel"].toString());

    ui->chkSerialInit->setChecked(map_debug["SerialInit"].toBool());
    ui->chkSysReport->setChecked(map_debug["SysReport"].toBool());

    ui->editTarget->setText(map_debug["Target"].toString());

    //Security
    QVariantMap map_security = map["Security"].toMap();
    ui->chkAllowNvramReset->setChecked(map_security["AllowNvramReset"].toBool());
    ui->chkAllowSetDefault->setChecked(map_security["AllowSetDefault"].toBool());
    ui->chkAuthRestart->setChecked(map_security["AuthRestart"].toBool());

    hm = map_security["BootProtect"].toString();
    if(hm.trimmed() == "None")
        ui->cboxBootProtect->setCurrentIndex(0);
    if(hm.trimmed() == "Bootstrap")
        ui->cboxBootProtect->setCurrentIndex(1);

    ui->editExposeSensitiveData->setText(map_security["ExposeSensitiveData"].toString());

    ui->editHaltLevel->setText(map_security["HaltLevel"].toString());
    ui->editScanPolicy->setText(map_security["ScanPolicy"].toString());

    hm = map_security["Vault"].toString();
    if(hm.trimmed() == "Optional")
        ui->cboxVault->setCurrentIndex(0);
    if(hm.trimmed() == "Basic")
        ui->cboxVault->setCurrentIndex(1);
    if(hm.trimmed() == "Secure")
        ui->cboxVault->setCurrentIndex(2);


    //BlessOverride(数组)
    QVariantList map_BlessOverride = map["BlessOverride"].toList();
    ui->tableBlessOverride->setRowCount(map_BlessOverride.count());//设置行的列数
    for(int i = 0;i < map_BlessOverride.count(); i++)
    {
        QTableWidgetItem *newItem1;
        newItem1 = new QTableWidgetItem(map_BlessOverride.at(i).toString());
        ui->tableBlessOverride->setItem(i, 0, newItem1);
    }

    //Entries
    QVariantList map_Entries = map["Entries"].toList();
    ui->tableEntries->setRowCount(map_Entries.count());//设置行的列数
    for(int i = 0;i < map_Entries.count(); i++)
    {
        QVariantMap map3 = map_Entries.at(i).toMap();

        QTableWidgetItem *newItem1;

        newItem1 = new QTableWidgetItem(map3["Path"].toString());
        ui->tableEntries->setItem(i, 0, newItem1);

        newItem1 = new QTableWidgetItem(map3["Arguments"].toString());
        ui->tableEntries->setItem(i, 1, newItem1);

        newItem1 = new QTableWidgetItem(map3["Name"].toString());
        newItem1->setTextAlignment(Qt::AlignCenter);
        ui->tableEntries->setItem(i, 2, newItem1);

        newItem1 = new QTableWidgetItem(map3["Comment"].toString());
        ui->tableEntries->setItem(i, 3, newItem1);

        init_enabled_data(ui->tableEntries , i , 4 , map3["Auxiliary"].toString());
        init_enabled_data(ui->tableEntries , i , 5 , map3["Enabled"].toString());
    }

    //Tools
    QVariantList map_Tools = map["Tools"].toList();
    ui->tableTools->setRowCount(map_Tools.count());//设置行的列数
    for(int i = 0;i < map_Tools.count(); i++)
    {
        QVariantMap map3 = map_Tools.at(i).toMap();

        QTableWidgetItem *newItem1;

        newItem1 = new QTableWidgetItem(map3["Path"].toString());
        ui->tableTools->setItem(i, 0, newItem1);

        newItem1 = new QTableWidgetItem(map3["Arguments"].toString());
        ui->tableTools->setItem(i, 1, newItem1);

        newItem1 = new QTableWidgetItem(map3["Name"].toString());
        newItem1->setTextAlignment(Qt::AlignCenter);
        ui->tableTools->setItem(i, 2, newItem1);

        newItem1 = new QTableWidgetItem(map3["Comment"].toString());
        ui->tableTools->setItem(i, 3, newItem1);

        init_enabled_data(ui->tableTools , i , 4 , map3["Auxiliary"].toString());

        init_enabled_data(ui->tableTools , i , 5 , map3["Enabled"].toString());
    }



}

void MainWindow::initui_nvram()
{
    QTableWidgetItem *id0;

    //Add
    ui->table_nv_add0->setColumnWidth(0,500);
    ui->table_nv_add0->setMinimumWidth(500);
    id0 = new QTableWidgetItem(tr("UUID"));
    ui->table_nv_add0->setHorizontalHeaderItem(0, id0);


    ui->table_nv_add->setColumnWidth(0,300);
    id0 = new QTableWidgetItem(tr("键\nKey"));
    ui->table_nv_add->setHorizontalHeaderItem(0, id0);

    ui->table_nv_add->setColumnWidth(2,350);
    id0 = new QTableWidgetItem(tr("值\nValue"));
    ui->table_nv_add->setHorizontalHeaderItem(2, id0);

    id0 = new QTableWidgetItem(tr("数据类型\nClass"));
    ui->table_nv_add->setHorizontalHeaderItem(1, id0);

    //Delete

    ui->table_nv_del0->setColumnWidth(0,600);
    id0 = new QTableWidgetItem(tr("UUID"));
    ui->table_nv_del0->setHorizontalHeaderItem(0, id0);

    ui->table_nv_del->setColumnWidth(0,600);
    id0 = new QTableWidgetItem(tr("值\nValue"));
    ui->table_nv_del->setHorizontalHeaderItem(0, id0);



    //LegacySchema

    ui->table_nv_ls0->setColumnWidth(0,600);
    id0 = new QTableWidgetItem(tr("UUID"));
    ui->table_nv_ls0->setHorizontalHeaderItem(0, id0);

    ui->table_nv_ls->setColumnWidth(0,600);
    id0 = new QTableWidgetItem(tr("值\nValue"));
    ui->table_nv_ls->setHorizontalHeaderItem(0, id0);


}

void MainWindow::ParserNvram(QVariantMap map)
{
    map = map["NVRAM"].toMap();
    if(map.isEmpty())
        return;

    //Add
    QVariantMap map_add = map["Add"].toMap();

    QVariantMap map_sub;

    ui->table_nv_add0->setRowCount(map_add.count());
    QTableWidgetItem *newItem1;
    for(int i = 0; i < map_add.count(); i ++)
    {
        newItem1 = new QTableWidgetItem(map_add.keys().at(i));
        ui->table_nv_add0->setItem(i, 0, newItem1);


        //加载子条目
        map_sub = map_add[map_add.keys().at(i)].toMap();
        ui->table_nv_add->setRowCount(map_sub.keys().count()); //子键的个数


        for(int j = 0; j < map_sub.keys().count(); j++)
        {

            //QTableWidgetItem *newItem1;
            newItem1 = new QTableWidgetItem(map_sub.keys().at(j));//键
            ui->table_nv_add->setItem(j, 0, newItem1);

            QString dtype = map_sub[map_sub.keys().at(j)].typeName();
            QString ztype;
            if(dtype == "QByteArray")
                ztype = "Data";
            if(dtype == "QString")
                ztype = "String";
            if(dtype == "qlonglong")
                ztype = "Number";
            newItem1 = new QTableWidgetItem(ztype);//数据类型
            newItem1->setTextAlignment(Qt::AlignCenter);
            ui->table_nv_add->setItem(j, 1, newItem1);

            QString type_name = map_sub[map_sub.keys().at(j)].typeName();
            if( type_name == "QByteArray")
            {
                QByteArray tohex = map_sub[map_sub.keys().at(j)].toByteArray();
                QString va = tohex.toHex().toUpper();
                newItem1 = new QTableWidgetItem(va);
            }
            else
            newItem1 = new QTableWidgetItem(map_sub[map_sub.keys().at(j)].toString());
            ui->table_nv_add->setItem(j, 2, newItem1);

        }

        //保存子条目里面的数据，以便以后加载

        write_ini("table_nv_add0" , ui->table_nv_add, i);


    }

    int last = ui->table_nv_add0->rowCount();
    ui->table_nv_add0->setCurrentCell(last - 1 , 0);



    //Delete
    init_value(map["Delete"].toMap() , ui->table_nv_del0 , ui->table_nv_del);

    //LegacySchema
    init_value(map["LegacySchema"].toMap() , ui->table_nv_ls0 , ui->table_nv_ls);

    //分析Quirks
    ui->chkLegacyEnable->setChecked(map["LegacyEnable"].toBool());
    ui->chkLegacyOverwrite->setChecked(map["LegacyOverwrite"].toBool());
    ui->chkWriteFlash->setChecked(map["WriteFlash"].toBool());


}

void MainWindow::write_ini(QString table_name , QTableWidget *mytable, int i)
{
    QString plistPath = QDir::homePath() + "/" + table_name + QString::number(i + 1) + ".ini";
    //qDebug() << plistPath;
    QFile file(plistPath);
    if(file.exists()) //如果文件存在，则先删除它
        file.remove();

    //QSettings Reg(plistPath, QSettings::NativeFormat);
    QSettings Reg(plistPath, QSettings::IniFormat);//全平台都采用ini格式

    for(int k = 0; k < mytable->rowCount(); k++)
    {
        Reg.setValue(QString::number(k + 1) + "/key", mytable->item(k , 0)->text());
        Reg.setValue(QString::number(k + 1) + "/class", mytable->item(k , 1)->text());
        Reg.setValue(QString::number(k + 1) + "/value", mytable->item(k , 2)->text());
    }

    //记录总数
    Reg.setValue("total" , mytable->rowCount());
}

void MainWindow::read_ini(QString table_name , QTableWidget *mytable , int i)
{
    QString plistPath = QDir::homePath() + "/" + table_name + QString::number(i + 1) + ".ini";
    //qDebug() << plistPath;
    QFile file(plistPath);
    if(file.exists())
    {
        QSettings Reg(plistPath, QSettings::IniFormat);
        int count = Reg.value("total").toInt();
        mytable->setRowCount(count);
        QTableWidgetItem *newItem1;
        for(int k = 0; k < count; k++)
        {


            newItem1 = new QTableWidgetItem(Reg.value(QString::number(k + 1) + "/key").toString());
            mytable->setItem(k, 0, newItem1);

            newItem1 = new QTableWidgetItem(Reg.value(QString::number(k + 1) + "/class").toString());
            newItem1->setTextAlignment(Qt::AlignCenter);
            mytable->setItem(k, 1, newItem1);

            newItem1 = new QTableWidgetItem(Reg.value(QString::number(k + 1) + "/value").toString());
            mytable->setItem(k, 2, newItem1);

        }

    }

}

void MainWindow::read_value_ini(QString table_name , QTableWidget *mytable , int i)
{
    QString plistPath = QDir::homePath() + "/" + table_name + QString::number(i + 1) + ".ini";
    //qDebug() << plistPath;
    QFile file(plistPath);
    if(file.exists())
    {
        QSettings Reg(plistPath, QSettings::IniFormat);
        int count = Reg.value("total").toInt();
        mytable->setRowCount(count);
        QTableWidgetItem *newItem1;
        for(int k = 0; k < count; k++)
        {


            newItem1 = new QTableWidgetItem(Reg.value(QString::number(k + 1) + "/key").toString());
            mytable->setItem(k, 0, newItem1);


        }

    }

}

void MainWindow::on_table_dp_add0_cellClicked(int row, int column)
{
    //读取ini数据并加载到table_dp_add中
    //qDebug() << row;

    loadding = true;

    if(column == 0)
        read_ini("table_dp_add0" , ui->table_dp_add , row);

    loadding = false;
}

void MainWindow::on_table_dp_add_itemSelectionChanged()
{

}

void MainWindow::on_table_dp_add_itemChanged(QTableWidgetItem *item)
{
    if(item->text().isEmpty())
    {

    }
    //当条目有修改时，重新写入数据
    if(!loadding)  //数据已经加载完成后
        write_ini("table_dp_add0" , ui->table_dp_add, ui->table_dp_add0->currentRow());

}

void MainWindow::on_table_nv_add0_cellClicked(int row, int column)
{
    loadding = true;

    if(column == 0)
        read_ini("table_nv_add0" , ui->table_nv_add , row);

    loadding = false;
}

void MainWindow::on_table_nv_add_itemChanged(QTableWidgetItem *item)
{
    if(item->text().isEmpty())
    {

    }

    //当条目有修改时，重新写入数据
    if(!loadding)  //数据已经加载完成后
        write_ini("table_nv_add0" , ui->table_nv_add, ui->table_nv_add0->currentRow());

}

void MainWindow::init_value(QVariantMap map_fun , QTableWidget *table, QTableWidget *subtable)
{
    //QVariantMap map_del = map["Delete"].toMap();

    table->setRowCount(map_fun.count());
    subtable->setRowCount(0);//归零，清除之前的残留
    QTableWidgetItem *newItem1;
    for(int i = 0; i < map_fun.count(); i ++)
    {
        newItem1 = new QTableWidgetItem(map_fun.keys().at(i));
        table->setItem(i, 0, newItem1);


        //加载子条目
        QVariantList map_sub_list = map_fun[map_fun.keys().at(i)].toList(); //是个数组
        subtable->setRowCount(map_sub_list.count()); //子键的个数
        //qDebug() << map_sub_list.count();

        for(int j = 0; j < map_sub_list.count(); j++)
        {

            //qDebug() << map_sub_list.at(j).toString();
            newItem1 = new QTableWidgetItem(map_sub_list.at(j).toString());//键
            subtable->setItem(j, 0, newItem1);



        }

        //保存子条目里面的数据，以便以后加载
        write_value_ini(table->objectName() , subtable , i);


    }

    int last = table->rowCount();
    table->setCurrentCell(last - 1 , 0);
}

void MainWindow::write_value_ini(QString tablename , QTableWidget *subtable , int i)
{



        QString plistPath = QDir::homePath() + "/" + tablename + QString::number(i + 1) + ".ini";
        //qDebug() << plistPath;
        QFile file(plistPath);
        if(file.exists()) //如果文件存在，则先删除它
            file.remove();
        //QSettings Reg(plistPath, QSettings::NativeFormat);
        QSettings Reg(plistPath, QSettings::IniFormat);//全平台都采用ini格式

        for(int k = 0; k < subtable->rowCount(); k++)
        {
            Reg.setValue(QString::number(k + 1) + "/key", subtable->item(k , 0)->text());

        }

        //记录总数
        Reg.setValue("total" , subtable->rowCount());

}

void MainWindow::on_table_nv_del0_cellClicked(int row, int column)
{
    loadding = true;

    if(column == 0)
        read_value_ini(ui->table_nv_del0->objectName() , ui->table_nv_del , row);

    loadding = false;
}

void MainWindow::on_table_nv_ls0_cellClicked(int row, int column)
{
    loadding = true;

    if(column == 0)
        read_value_ini(ui->table_nv_ls0->objectName() , ui->table_nv_ls , row);

    loadding = false;
}

void MainWindow::on_table_nv_del_itemChanged(QTableWidgetItem *item)
{
    if(item->text().isEmpty())
    {

    }

    if(!loadding)
        write_value_ini(ui->table_nv_del0->objectName() , ui->table_nv_del , ui->table_nv_del0->currentRow());
}

void MainWindow::on_table_nv_ls_itemChanged(QTableWidgetItem *item)
{
    if(item->text().isEmpty())
    {

    }

    if(!loadding)
        write_value_ini(ui->table_nv_ls0->objectName() , ui->table_nv_ls , ui->table_nv_ls0->currentRow());
}

void MainWindow::on_table_dp_del0_cellClicked(int row, int column)
{
    loadding = true;

    if(column == 0)
        read_value_ini(ui->table_dp_del0->objectName() , ui->table_dp_del , row);

    loadding = false;

}

void MainWindow::on_table_dp_del_itemChanged(QTableWidgetItem *item)
{
    if(item->text().isEmpty())
    {

    }


    if(!loadding)
        write_value_ini(ui->table_dp_del0->objectName() , ui->table_dp_del , ui->table_dp_del0->currentRow());
}

void MainWindow::initui_PlatformInfo()
{
    ui->cboxUpdateSMBIOSMode->addItem("TryOverwrite");
    ui->cboxUpdateSMBIOSMode->addItem("Create");
    ui->cboxUpdateSMBIOSMode->addItem("Overwrite");
    ui->cboxUpdateSMBIOSMode->addItem("Custom");

}

void MainWindow::ParserPlatformInfo(QVariantMap map)
{
    map = map["PlatformInfo"].toMap();
    if(map.isEmpty())
        return;

    ui->chkAutomatic->setChecked(map["Automatic"].toBool());
    ui->chkUpdateDataHub->setChecked(map["UpdateDataHub"].toBool());
    ui->chkUpdateNVRAM->setChecked(map["UpdateNVRAM"].toBool());
    ui->chkUpdateSMBIOS->setChecked(map["UpdateSMBIOS"].toBool());

    QString usm = map["UpdateSMBIOSMode"].toString();
    if(usm.trimmed() == "TryOverwrite")
        ui->cboxUpdateSMBIOSMode->setCurrentIndex(0);
    if(usm.trimmed() == "Create")
        ui->cboxUpdateSMBIOSMode->setCurrentIndex(1);
    if(usm.trimmed() == "Overwrite")
        ui->cboxUpdateSMBIOSMode->setCurrentIndex(2);
    if(usm.trimmed() == "Custom")
        ui->cboxUpdateSMBIOSMode->setCurrentIndex(3);

    //Generic
    QVariantMap mapGeneric = map["Generic"].toMap();
    ui->chkAdviseWindows->setChecked(mapGeneric["AdviseWindows"].toBool());
    ui->editMLB->setText(mapGeneric["MLB"].toString());

    QByteArray ba = mapGeneric["ROM"].toByteArray();
    QString va = ba.toHex().toUpper();
    ui->editROM->setText(va);

    ui->chkSpoofVendor->setChecked(mapGeneric["SpoofVendor"].toBool());
    ui->editSystemProductName->setText(mapGeneric["SystemProductName"].toString());
    ui->editSystemSerialNumber->setText(mapGeneric["SystemSerialNumber"].toString());
    ui->editSystemUUID->setText(mapGeneric["SystemUUID"].toString());

}

void MainWindow::initui_UEFI()
{
    //Drivers
    QTableWidgetItem *id0;

    ui->table_uefi_drivers->setColumnWidth(0,1200);
    id0 = new QTableWidgetItem(tr("驱动名称\nDrivers"));
    ui->table_uefi_drivers->setHorizontalHeaderItem(0, id0);

    //KeySupportMode
    ui->cboxKeySupportMode->addItem("Auto");
    ui->cboxKeySupportMode->addItem("V1");
    ui->cboxKeySupportMode->addItem("V2");
    ui->cboxKeySupportMode->addItem("AMI");

    //Output
    ui->cboxConsoleMode->setEditable(true);
    ui->cboxConsoleMode->addItem("Max");

    ui->cboxResolution->setEditable(true);
    ui->cboxResolution->addItem("Max");

    ui->cboxTextRenderer->addItem("BuiltinGraphics");
    ui->cboxTextRenderer->addItem("SystemGraphics");
    ui->cboxTextRenderer->addItem("SystemText");
    ui->cboxTextRenderer->addItem("SystemGeneric");


    //ReservedMemory

    ui->table_uefi_ReservedMemory->setColumnWidth(0,300);
    id0 = new QTableWidgetItem(tr("地址\nAddress"));
    ui->table_uefi_ReservedMemory->setHorizontalHeaderItem(0, id0);

    ui->table_uefi_ReservedMemory->setColumnWidth(1,360);
    id0 = new QTableWidgetItem(tr("注释\nComment"));
    ui->table_uefi_ReservedMemory->setHorizontalHeaderItem(1, id0);

    ui->table_uefi_ReservedMemory->setColumnWidth(2,300);
    id0 = new QTableWidgetItem(tr("大小\nSize"));
    ui->table_uefi_ReservedMemory->setHorizontalHeaderItem(2, id0);

    id0 = new QTableWidgetItem(tr("启用\nEnabled"));
    ui->table_uefi_ReservedMemory->setHorizontalHeaderItem(3, id0);


}

void MainWindow::ParserUEFI(QVariantMap map)
{
    map = map["UEFI"].toMap();
    if(map.isEmpty())
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
    ui->chkPlayChime->setChecked(map_audio["PlayChime"].toBool());

    ui->editAudioCodec->setText(map_audio["AudioCodec"].toString());
    ui->editAudioDevice->setText(map_audio["AudioDevice"].toString());
    ui->editAudioOut->setText(map_audio["AudioOut"].toString());
    ui->editMinimumVolume->setText(map_audio["MinimumVolume"].toString());
    ui->editVolumeAmplifier->setText(map_audio["VolumeAmplifier"].toString());

    //Drivers
    QTableWidgetItem *id0;
    QVariantList map_Drivers = map["Drivers"].toList(); //数组
    ui->table_uefi_drivers->setRowCount(map_Drivers.count());
    for(int i = 0; i < map_Drivers.count(); i ++)
    {


        id0 = new QTableWidgetItem(map_Drivers.at(i).toString());
        ui->table_uefi_drivers->setItem(i , 0 , id0);

    }

    ui->chkConnectDrivers->setChecked(map["ConnectDrivers"].toBool());

    //分析Input
    QVariantMap map_input = map["Input"].toMap();
    ui->chkKeyFiltering->setChecked(map_input["KeyFiltering"].toBool());
    ui->chkKeySupport->setChecked(map_input["KeySupport"].toBool());
    ui->chkKeySwap->setChecked(map_input["KeySwap"].toBool());
    ui->chkPointerSupport->setChecked(map_input["PointerSupport"].toBool());

    ui->editKeyForgetThreshold->setText(map_input["KeyForgetThreshold"].toString());
    ui->editKeyMergeThreshold->setText(map_input["KeyMergeThreshold"].toString());
    ui->editPointerSupportMode->setText(map_input["PointerSupportMode"].toString());
    ui->editTimerResolution->setText(map_input["TimerResolution"].toString());

    QString ksm = map_input["KeySupportMode"].toString();
    if(ksm.trimmed() == "Auto")
        ui->cboxKeySupportMode->setCurrentIndex(0);
    if(ksm.trimmed() == "V1")
        ui->cboxKeySupportMode->setCurrentIndex(1);
    if(ksm.trimmed() == "V2")
        ui->cboxKeySupportMode->setCurrentIndex(2);
    if(ksm.trimmed() == "AMI")
        ui->cboxKeySupportMode->setCurrentIndex(3);

    //Output
    QVariantMap map_output = map["Output"].toMap();
    ui->chkClearScreenOnModeSwitch->setChecked(map_output["ClearScreenOnModeSwitch"].toBool());
    ui->chkDirectGopRendering->setChecked(map_output["DirectGopRendering"].toBool());
    ui->chkIgnoreTextInGraphics->setChecked(map_output["IgnoreTextInGraphics"].toBool());
    ui->chkProvideConsoleGop->setChecked(map_output["ProvideConsoleGop"].toBool());
    ui->chkReconnectOnResChange->setChecked(map_output["ReconnectOnResChange"].toBool());
    ui->chkReplaceTabWithSpace->setChecked(map_output["ReplaceTabWithSpace"].toBool());
    ui->chkSanitiseClearScreen->setChecked(map_output["SanitiseClearScreen"].toBool());
    ui->chkUgaPassThrough->setChecked(map_output["UgaPassThrough"].toBool());


    ui->cboxConsoleMode->setCurrentText(map_output["ConsoleMode"].toString());
    ui->cboxResolution->setCurrentText(map_output["Resolution"].toString());
    ui->cboxTextRenderer->setCurrentText(map_output["TextRenderer"].toString());

    //ProtocolOverrides
    QVariantMap map_po = map["ProtocolOverrides"].toMap();
    ui->chkAppleAudio->setChecked(map_po["AppleAudio"].toBool());
    ui->chkAppleBootPolicy->setChecked(map_po["AppleBootPolicy"].toBool());
    ui->chkAppleDebugLog->setChecked(map_po["AppleDebugLog"].toBool());
    ui->chkAppleEvent->setChecked(map_po["AppleEvent"].toBool());
    ui->chkAppleFramebufferInfo->setChecked(map_po["AppleFramebufferInfo"].toBool());
    ui->chkAppleImageConversion->setChecked(map_po["AppleImageConversion"].toBool());
    ui->chkAppleKeyMap->setChecked(map_po["AppleKeyMap"].toBool());
    ui->chkAppleRtcRam->setChecked(map_po["AppleRtcRam"].toBool());
    ui->chkAppleSmcIo->setChecked(map_po["AppleSmcIo"].toBool());
    ui->chkAppleUserInterfaceTheme->setChecked(map_po["AppleUserInterfaceTheme"].toBool());
    ui->chkDataHub->setChecked(map_po["DataHub"].toBool());
    ui->chkDeviceProperties->setChecked(map_po["DeviceProperties"].toBool());
    ui->chkFirmwareVolume->setChecked(map_po["FirmwareVolume"].toBool());
    ui->chkHashServices->setChecked(map_po["HashServices"].toBool());
    ui->chkOSInfo->setChecked(map_po["OSInfo"].toBool());
    ui->chkUnicodeCollation->setChecked(map_po["UnicodeCollation"].toBool());

    //Quirks
    QVariantMap map_uefi_Quirks = map["Quirks"].toMap();
    ui->chkDeduplicateBootOrder->setChecked(map_uefi_Quirks["DeduplicateBootOrder"].toBool());
    ui->chkIgnoreInvalidFlexRatio->setChecked(map_uefi_Quirks["IgnoreInvalidFlexRatio"].toBool());
    ui->chkReleaseUsbOwnership->setChecked(map_uefi_Quirks["ReleaseUsbOwnership"].toBool());
    ui->chkRequestBootVarRouting->setChecked(map_uefi_Quirks["RequestBootVarRouting"].toBool());
    ui->chkUnblockFsConnect->setChecked(map_uefi_Quirks["UnblockFsConnect"].toBool());

    ui->editExitBootServicesDelay->setText(map_uefi_Quirks["ExitBootServicesDelay"].toString());
    ui->editTscSyncTimeout->setText(map_uefi_Quirks["TscSyncTimeout"].toString());

    //ReservedMemory
    QTableWidgetItem *newItem1;
    QVariantList map3 = map["ReservedMemory"].toList();
    ui->table_uefi_ReservedMemory->setRowCount(map3.count());
    for(int i = 0; i < map3.count(); i++)
    {


        QVariantMap map_sub = map3.at(i).toMap();
        newItem1 = new QTableWidgetItem(map_sub["Address"].toString());
        ui->table_uefi_ReservedMemory->setItem(i, 0, newItem1);

        newItem1 = new QTableWidgetItem(map_sub["Comment"].toString());
        ui->table_uefi_ReservedMemory->setItem(i, 1, newItem1);

        newItem1 = new QTableWidgetItem(map_sub["Size"].toString());
        ui->table_uefi_ReservedMemory->setItem(i, 2, newItem1);

        init_enabled_data(ui->table_uefi_ReservedMemory , i , 3 , map_sub["Enabled"].toString());




    }


}



void MainWindow::on_btnSave_clicked()
{
    //QString FileName = QDir::homePath() + "/test.plist";

    SavePlist(PlistFileName);
}

bool MainWindow::getBool(QTableWidget *table, int row, int column)
{
    QString be = table->item(row , column)->text();
    if(be.trimmed() == "true")
        return  true;
    if(be.trimmed() == "false")
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

    PListSerializer::toPList(OpenCore , FileName);

}

QVariantMap MainWindow::SaveACPI()
{
    //ACPI
    //Add
    QVariantMap acpiMap;
    QVariantList acpiAdd;
    QVariantMap acpiAddSub;

    for(int i = 0; i < ui->table_acpi_add->rowCount(); i ++)
    {
        acpiAddSub["Path"] = ui->table_acpi_add->item(i , 0)->text();
        acpiAddSub["Comment"] = ui->table_acpi_add->item(i , 1)->text();
        acpiAddSub["Enabled"] = getBool(ui->table_acpi_add , i , 2);

        acpiAdd.append(acpiAddSub); //最后一层
        acpiMap["Add"] = acpiAdd; //第二层


    }


    //Delete
    QVariantList acpiDel;
    QVariantMap acpiDelSub;
    QString str;

    for(int i = 0; i < ui->table_acpi_del->rowCount(); i ++)
    {

        str = ui->table_acpi_del->item(i , 0)->text();
        acpiDelSub["TableSignature"] = HexStrToByte(str);

        str = ui->table_acpi_del->item(i , 1)->text();
        acpiDelSub["OemTableId"] = HexStrToByte(str);

        acpiDelSub["TableLength"] = ui->table_acpi_del->item(i , 2)->text().toLongLong();
        acpiDelSub["Comment"] = ui->table_acpi_del->item(i , 3)->text();
        acpiDelSub["All"] = getBool(ui->table_acpi_del , i , 4);
        acpiDelSub["Enabled"] = getBool(ui->table_acpi_del , i , 5);

        acpiDel.append(acpiDelSub); //最后一层
        acpiMap["Delete"] = acpiDel; //第二层


    }


    //Patch
    QVariantList acpiPatch;
    QVariantMap acpiPatchSub;


    for(int i = 0; i < ui->table_acpi_patch->rowCount(); i ++)
    {

        str = ui->table_acpi_patch->item(i , 0)->text();
        acpiPatchSub["TableSignature"] = HexStrToByte(str);

        str = ui->table_acpi_patch->item(i , 1)->text();
        acpiPatchSub["OemTableId"] = HexStrToByte(str);


        acpiPatchSub["TableLength"] = ui->table_acpi_patch->item(i , 2)->text().toLongLong();
        acpiPatchSub["Find"] = HexStrToByte(ui->table_acpi_patch->item(i , 3)->text());
        acpiPatchSub["Replace"] = HexStrToByte(ui->table_acpi_patch->item(i , 4)->text());
        acpiPatchSub["Comment"] = ui->table_acpi_patch->item(i , 5)->text();
        acpiPatchSub["Mask"] = ui->table_acpi_patch->item(i , 6)->text().toLatin1();
        acpiPatchSub["ReplaceMask"] = ui->table_acpi_patch->item(i , 7)->text().toLatin1();
        acpiPatchSub["Count"] = ui->table_acpi_patch->item(i , 8)->text().toLongLong();
        acpiPatchSub["Limit"] = ui->table_acpi_patch->item(i , 9)->text().toLongLong();
        acpiPatchSub["Skip"] = ui->table_acpi_patch->item(i , 10)->text().toLongLong();
        acpiPatchSub["Enabled"] = getBool(ui->table_acpi_patch , i , 11);

        acpiPatch.append(acpiPatchSub); //最后一层
        acpiMap["Patch"] = acpiPatch; //第二层


    }

    //Quirks
    QVariantMap acpiQuirks;
    acpiQuirks["FadtEnableReset"] = getChkBool(ui->chkFadtEnableReset);
    acpiQuirks["NormalizeHeaders"] = getChkBool(ui->chkNormalizeHeaders);
    acpiQuirks["RebaseRegions"] = getChkBool(ui->chkRebaseRegions);
    acpiQuirks["ResetHwSig"] = getChkBool(ui->chkResetHwSig);
    acpiQuirks["ResetLogoStatus"] = getChkBool(ui->chkResetLogoStatus);

    acpiMap["Quirks"] = acpiQuirks;


    return  acpiMap;

}



QVariantMap MainWindow::SaveBooter()
{
    //MmioWhitelist
    QVariantMap subMap;
    QVariantList arrayList;
    QVariantMap valueList;

    for(int i = 0; i < ui->table_booter->rowCount(); i ++)
    {

        QString str = ui->table_booter->item(i ,0)->text().trimmed();
        valueList["Address"] = str.toLongLong();

        valueList["Comment"] = ui->table_booter->item(i , 1)->text();

        valueList["Enabled"] = getBool(ui->table_booter , i , 2);

        arrayList.append(valueList); //最后一层
        subMap["MmioWhitelist"] = arrayList; //第二层


    }

    //Quirks
    QVariantMap mapQuirks;
    mapQuirks["AvoidRuntimeDefrag"] = getChkBool(ui->chkAvoidRuntimeDefrag);
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


    return  subMap;


}

QVariantMap MainWindow::SaveDeviceProperties()
{
    //Add
    QVariantMap subMap;
    QVariantMap dictList;
    QVariantMap valueList;
    QVariantList arrayList;
    for(int i = 0; i < ui->table_dp_add0->rowCount(); i ++)
    {

        valueList.clear(); //先必须清理下列表，很重要
        //先加载表中的数据
        ui->table_dp_add0->setCurrentCell(i , 0);
        on_table_dp_add0_cellClicked(i , 0);

        for(int k = 0; k < ui->table_dp_add->rowCount(); k ++)
        {
            QString dataType = ui->table_dp_add->item(k , 1)->text(); //数据类型
            QString value = ui->table_dp_add->item(k , 2)->text();
            if(dataType == "String")
                valueList[ui->table_dp_add->item(k , 0)->text()] = value;
            if(dataType == "Data")
            {

                //将以字符串方式显示的16进制原样转换成QByteArray
                valueList[ui->table_dp_add->item(k , 0)->text()] = HexStrToByte(value);


            }
            if(dataType == "Number")
                valueList[ui->table_dp_add->item(k , 0)->text()] = value.toLongLong();

        }
        dictList[ui->table_dp_add0->item(i , 0)->text()] = valueList;

    }
    subMap["Add"] = dictList;

    //Delete
    dictList.clear(); //先清理之前的数据
    for(int i = 0; i < ui->table_dp_del0->rowCount(); i ++)
    {

        valueList.clear(); //先必须清理下列表，很重要
        arrayList.clear();

        //先加载表中的数据
        ui->table_dp_del0->setCurrentCell(i , 0);
        on_table_dp_del0_cellClicked(i , 0);

        for(int k = 0; k < ui->table_dp_del->rowCount(); k ++)
        {
            arrayList.append(ui->table_dp_del->item(k , 0)->text());
        }
        dictList[ui->table_dp_del0->item(i , 0)->text()] = arrayList;

    }
    subMap["Delete"] = dictList;

    return subMap;

}

QVariantMap MainWindow::SaveKernel()
{
    //Add
    QVariantMap subMap;
    QVariantList dictList;
    QVariantMap valueList;
    for(int i = 0; i < ui->table_kernel_add->rowCount(); i ++)
    {
        valueList["BundlePath"] = ui->table_kernel_add->item(i , 0)->text();
        valueList["Comment"] = ui->table_kernel_add->item(i , 1)->text();
        valueList["ExecutablePath"] = ui->table_kernel_add->item(i , 2)->text();
        valueList["PlistPath"] = ui->table_kernel_add->item(i , 3)->text();
        valueList["MinKernel"] = ui->table_kernel_add->item(i , 4)->text();
        valueList["MaxKernel"] = ui->table_kernel_add->item(i , 5)->text();
        valueList["Enabled"] = getBool(ui->table_kernel_add , i , 6);

        dictList.append(valueList);
        subMap["Add"] = dictList;
    }

    //Block
    dictList.clear();//必须先清理之前的数据，否则之前的数据会加到当前数据里面来
    valueList.clear();
    for(int i = 0; i < ui->table_kernel_block->rowCount(); i ++)
    {
        valueList["Identifier"] = ui->table_kernel_block->item(i , 0)->text();
        valueList["Comment"] = ui->table_kernel_block->item(i , 1)->text();
        valueList["MinKernel"] = ui->table_kernel_block->item(i , 2)->text();
        valueList["MaxKernel"] = ui->table_kernel_block->item(i , 3)->text();
        valueList["Enabled"] = getBool(ui->table_kernel_block , i , 4);

        dictList.append(valueList);
        subMap["Block"] = dictList;
    }

    //Patch
    dictList.clear();
    valueList.clear();
    for(int i = 0; i < ui->table_kernel_patch->rowCount(); i ++)
    {
        valueList["Identifier"] = ui->table_kernel_patch->item(i , 0)->text();
        valueList["Base"] = ui->table_kernel_patch->item(i , 1)->text();
        valueList["Comment"] = ui->table_kernel_patch->item(i , 2)->text();
        valueList["Find"] = HexStrToByte(ui->table_kernel_patch->item(i , 3)->text());
        valueList["Replace"] = HexStrToByte(ui->table_kernel_patch->item(i , 4)->text());
        valueList["Mask"] = HexStrToByte(ui->table_kernel_patch->item(i , 5)->text());
        valueList["ReplaceMask"] = HexStrToByte(ui->table_kernel_patch->item(i , 6)->text());
        valueList["MinKernel"] = ui->table_kernel_patch->item(i , 7)->text();
        valueList["MaxKernel"] = ui->table_kernel_patch->item(i , 8)->text();
        valueList["Count"] = ui->table_kernel_patch->item(i , 9)->text().toLongLong();
        valueList["Limit"] = ui->table_kernel_patch->item(i , 10)->text().toLongLong();
        valueList["Skip"] = ui->table_kernel_patch->item(i , 11)->text().toLongLong();
        valueList["Enabled"] = getBool(ui->table_kernel_patch , i , 12);

        dictList.append(valueList);
        subMap["Patch"] = dictList;
    }

    //Emulate
    QVariantMap mapValue;
    mapValue["Cpuid1Data"] = HexStrToByte(ui->editCpuid1Data->text());
    mapValue["Cpuid1Mask"] = HexStrToByte(ui->editCpuid1Mask->text());
    subMap["Emulate"] = mapValue;

    //Quirks
    QVariantMap mapQuirks;
    mapQuirks["AppleCpuPmCfgLock"] = getChkBool(ui->chkAppleCpuPmCfgLock);
    mapQuirks["AppleXcpmCfgLock"] = getChkBool(ui->chkAppleXcpmCfgLock);
    mapQuirks["AppleXcpmExtraMsrs"] = getChkBool(ui->chkAppleXcpmExtraMsrs);
    mapQuirks["AppleXcpmForceBoost"] = getChkBool(ui->chkAppleXcpmForceBoost);
    mapQuirks["CustomSMBIOSGuid"] = getChkBool(ui->chkCustomSMBIOSGuid);
    mapQuirks["DisableIoMapper"] = getChkBool(ui->chkDisableIoMapper);
    mapQuirks["DisableRtcChecksum"] = getChkBool(ui->chkDisableRtcChecksum);
    mapQuirks["DummyPowerManagement"] = getChkBool(ui->chkDummyPowerManagement);
    mapQuirks["ExternalDiskIcons"] = getChkBool(ui->chkExternalDiskIcons);
    mapQuirks["IncreasePciBarSize"] = getChkBool(ui->chkIncreasePciBarSize);
    mapQuirks["LapicKernelPanic"] = getChkBool(ui->chkLapicKernelPanic);
    mapQuirks["PanicNoKextDump"] = getChkBool(ui->chkPanicNoKextDump);
    mapQuirks["PowerTimeoutKernelPanic"] = getChkBool(ui->chkPowerTimeoutKernelPanic);
    mapQuirks["ThirdPartyDrives"] = getChkBool(ui->chkThirdPartyDrives);
    mapQuirks["XhciPortLimit"] = getChkBool(ui->chkXhciPortLimit);

    subMap["Quirks"] = mapQuirks;


    return  subMap;

}

QVariantMap MainWindow::SaveMisc()
{
    QVariantMap subMap;
    QVariantList dictList;
    QVariantMap valueList;

    //BlessOverride
    for(int i = 0; i < ui->tableBlessOverride->rowCount(); i++)
    {
        dictList.append(ui->tableBlessOverride->item(i , 0)->text());
    }
    subMap["BlessOverride"] = dictList;

    //Boot
    valueList["ConsoleAttributes"] = ui->editConsoleAttributes->text().toLongLong();
    valueList["HibernateMode"] = ui->cboxHibernateMode->currentText();
    valueList["HideAuxiliary"] = getChkBool(ui->chkHideAuxiliary);
    valueList["PickerAttributes"] = ui->editPickerAttributes->text().toLongLong();
    valueList["PickerAudioAssist"] = getChkBool(ui->chkPickerAudioAssist);
    valueList["PickerMode"] = ui->cboxPickerMode->currentText();
    valueList["PollAppleHotKeys"] = getChkBool(ui->chkPollAppleHotKeys);
    valueList["ShowPicker"] = getChkBool(ui->chkShowPicker);
    valueList["TakeoffDelay"] = ui->editTakeoffDelay->text().toLongLong();
    valueList["Timeout"] = ui->editTimeout->text().toLongLong();

    subMap["Boot"] = valueList;

    //Debug
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

    //Entries
    valueList.clear();
    dictList.clear();
    for(int i = 0; i < ui->tableEntries->rowCount(); i ++)
    {
        valueList["Path"] = ui->tableEntries->item(i , 0)->text();
        valueList["Arguments"] = ui->tableEntries->item(i , 1)->text();
        valueList["Name"] = ui->tableEntries->item(i , 2)->text();
        valueList["Comment"] = ui->tableEntries->item(i , 3)->text();
        valueList["Auxiliary"] = getBool(ui->tableEntries , i ,4);
        valueList["Enabled"] = getBool(ui->tableEntries , i ,5);

        dictList.append(valueList);

    }
    subMap["Entries"] = dictList;

    //Security
    valueList.clear();

    valueList["AllowNvramReset"] = getChkBool(ui->chkAllowNvramReset);
    valueList["AllowSetDefault"] = getChkBool(ui->chkAllowSetDefault);
    valueList["AuthRestart"] = getChkBool(ui->chkAuthRestart);

    valueList["BootProtect"] = ui->cboxBootProtect->currentText();
    valueList["Vault"] = ui->cboxVault->currentText();

    valueList["ExposeSensitiveData"] = ui->editExposeSensitiveData->text().toLongLong();

    valueList["HaltLevel"] = ui->editHaltLevel->text().toLongLong(nullptr , 10);
    qDebug() << ui->editHaltLevel->text().toLongLong(nullptr , 16);

    valueList["ScanPolicy"] = ui->editScanPolicy->text().toLongLong();

    subMap["Security"] = valueList;

    //Tools
    valueList.clear();
    dictList.clear();
    for(int i = 0; i < ui->tableTools->rowCount(); i ++)
    {
        valueList["Path"] = ui->tableTools->item(i , 0)->text();
        valueList["Arguments"] = ui->tableTools->item(i , 1)->text();
        valueList["Name"] = ui->tableTools->item(i , 2)->text();
        valueList["Comment"] = ui->tableTools->item(i , 3)->text();
        valueList["Auxiliary"] = getBool(ui->tableTools , i ,4);
        valueList["Enabled"] = getBool(ui->tableTools , i ,5);

        dictList.append(valueList);

    }
    subMap["Tools"] = dictList;



    return  subMap;
}

QVariantMap MainWindow::SaveNVRAM()
{
    //Add
    QVariantMap subMap;
    QVariantMap dictList;
    QVariantList arrayList;
    QVariantMap valueList;

    for(int i = 0; i < ui->table_nv_add0->rowCount(); i ++)
    {

        valueList.clear(); //先必须清理下列表，很重要
        //先加载表中的数据
        ui->table_nv_add0->setCurrentCell(i , 0);
        on_table_nv_add0_cellClicked(i , 0);

        for(int k = 0; k < ui->table_nv_add->rowCount(); k ++)
        {
            QString dataType = ui->table_nv_add->item(k , 1)->text(); //数据类型
            QString value = ui->table_nv_add->item(k , 2)->text();
            if(dataType == "String")
                valueList[ui->table_nv_add->item(k , 0)->text()] = value;
            if(dataType == "Data")
            {

                //将以字符串方式显示的16进制原样转换成QByteArray
                valueList[ui->table_nv_add->item(k , 0)->text()] = HexStrToByte(value);


            }
            if(dataType == "Number")
                valueList[ui->table_nv_add->item(k , 0)->text()] = value.toLongLong();

        }
        dictList[ui->table_nv_add0->item(i , 0)->text()] = valueList;

    }
    subMap["Add"] = dictList;

    //Delete
    dictList.clear(); //先清理之前的数据
    for(int i = 0; i < ui->table_nv_del0->rowCount(); i ++)
    {

        valueList.clear(); //先必须清理下列表，很重要
        arrayList.clear();

        //先加载表中的数据
        ui->table_nv_del0->setCurrentCell(i , 0);
        on_table_nv_del0_cellClicked(i , 0);

        for(int k = 0; k < ui->table_nv_del->rowCount(); k ++)
        {
            arrayList.append(ui->table_nv_del->item(k , 0)->text());
        }
        dictList[ui->table_nv_del0->item(i , 0)->text()] = arrayList;

    }
    subMap["Delete"] = dictList;

    //LegacySchema
    dictList.clear(); //先清理之前的数据
    for(int i = 0; i < ui->table_nv_ls0->rowCount(); i ++)
    {

        valueList.clear(); //先必须清理下列表，很重要
        arrayList.clear();

        //先加载表中的数据
        ui->table_nv_ls0->setCurrentCell(i , 0);
        on_table_nv_ls0_cellClicked(i , 0);

        for(int k = 0; k < ui->table_nv_ls->rowCount(); k ++)
        {
            arrayList.append(ui->table_nv_ls->item(k , 0)->text());
        }
        dictList[ui->table_nv_ls0->item(i , 0)->text()] = arrayList;

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

    //Generic
    valueList["AdviseWindows"] = getChkBool(ui->chkAdviseWindows);
    valueList["MLB"] = ui->editMLB->text();
    valueList["ROM"] = HexStrToByte(ui->editROM->text());
    valueList["SpoofVendor"] = getChkBool(ui->chkSpoofVendor);
    valueList["SystemProductName"] = ui->editSystemProductName->text();
    valueList["SystemSerialNumber"] = ui->editSystemSerialNumber->text();
    valueList["SystemUUID"] = ui->editSystemUUID->text();

    subMap["Generic"] = valueList;

    subMap["Automatic"] = getChkBool(ui->chkAutomatic);
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

    //APFS
    dictList["EnableJumpstart"] = getChkBool(ui->chkEnableJumpstart);
    dictList["GlobalConnect"] = getChkBool(ui->chkGlobalConnect);
    dictList["HideVerbose"] = getChkBool(ui->chkHideVerbose);
    dictList["JumpstartHotPlug"] = getChkBool(ui->chkJumpstartHotPlug);
    dictList["MinDate"] = ui->editMinDate->text().toLongLong();
    dictList["MinVersion"] = ui->editMinVersion->text().toLongLong();
    subMap["APFS"] = dictList;

    //Audio
    dictList.clear();
    dictList["AudioCodec"] = ui->editAudioCodec->text().toLongLong();
    dictList["AudioDevice"] = ui->editAudioDevice->text();
    dictList["AudioOut"] = ui->editAudioOut->text().toLongLong();
    dictList["AudioSupport"] = getChkBool(ui->chkAudioSupport);
    dictList["MinimumVolume"] = ui->editMinimumVolume->text().toLongLong();
    dictList["PlayChime"] = getChkBool(ui->chkPlayChime);
    dictList["VolumeAmplifier"] = ui->editVolumeAmplifier->text().toLongLong();
    subMap["Audio"] = dictList;

    //Drivers
    arrayList.clear();
    for(int i = 0; i < ui->table_uefi_drivers->rowCount(); i++)
    {
        arrayList.append(ui->table_uefi_drivers->item(i , 0)->text());
    }
    subMap["Drivers"] = arrayList;
    subMap["ConnectDrivers"] = getChkBool(ui->chkConnectDrivers);

    //Input
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


    //Output
    dictList.clear();
    dictList["ClearScreenOnModeSwitch"] = getChkBool(ui->chkClearScreenOnModeSwitch);
    dictList["DirectGopRendering"] = getChkBool(ui->chkDirectGopRendering);
    dictList["IgnoreTextInGraphics"] = getChkBool(ui->chkIgnoreTextInGraphics);
    dictList["ProvideConsoleGop"] = getChkBool(ui->chkProvideConsoleGop);
    dictList["ReconnectOnResChange"] = getChkBool(ui->chkReconnectOnResChange);
    dictList["ReplaceTabWithSpace"] = getChkBool(ui->chkReplaceTabWithSpace);
    dictList["SanitiseClearScreen"] = getChkBool(ui->chkSanitiseClearScreen);
    dictList["UgaPassThrough"] = getChkBool(ui->chkUgaPassThrough);

    dictList["ConsoleMode"] = ui->cboxConsoleMode->currentText();
    dictList["Resolution"] = ui->cboxResolution->currentText();
    dictList["TextRenderer"] = ui->cboxTextRenderer->currentText();

    subMap["Output"] = dictList;


    //ProtocolOverrides
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

    subMap["ProtocolOverrides"] = dictList;

    //Quirks
    dictList.clear();
    dictList["DeduplicateBootOrder"] = getChkBool(ui->chkDeduplicateBootOrder);
    dictList["IgnoreInvalidFlexRatio"] = getChkBool(ui->chkIgnoreInvalidFlexRatio);
    dictList["ReleaseUsbOwnership"] = getChkBool(ui->chkReleaseUsbOwnership);
    dictList["RequestBootVarRouting"] = getChkBool(ui->chkRequestBootVarRouting);
    dictList["UnblockFsConnect"] = getChkBool(ui->chkUnblockFsConnect);

    dictList["ExitBootServicesDelay"] = ui->editExitBootServicesDelay->text().toLongLong();
    dictList["TscSyncTimeout"] = ui->editTscSyncTimeout->text().toLongLong();

    subMap["Quirks"] = dictList;


    //ReservedMemory
    arrayList.clear();
    valueList.clear();
    for(int i = 0; i < ui->table_uefi_ReservedMemory->rowCount(); i++)
    {
        valueList["Address"] = ui->table_uefi_ReservedMemory->item(i , 0)->text().toLongLong();
        valueList["Comment"] = ui->table_uefi_ReservedMemory->item(i , 1)->text();
        valueList["Size"] = ui->table_uefi_ReservedMemory->item(i , 2)->text().toLongLong();
        valueList["Enabled"] = getBool(ui->table_uefi_ReservedMemory , i ,3);

        arrayList.append(valueList);
    }
    subMap["ReservedMemory"] = arrayList;


    return  subMap;

}

bool MainWindow::getChkBool(QCheckBox *chkbox)
{
    if(chkbox->isChecked())
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
    ba.resize(len/2);
    for(int i = 0; i < len/2; i++)
    {

        //qDebug() << i << k;

        byte.push_back(value.mid(k , 2));
        ba[k/2] = byte[k/2].toUInt(nullptr , 16);
        k = k + 2;


    }

    /*QString c1 , c2 , c3 , c4;
    c1 = value.mid(0 , 2);
    c2 = value.mid(2 , 2);
    c3 = value.mid(4 , 2);
    c4 = value.mid(6 , 2);

    ba.resize(4);
    ba[0] = c1.toUInt(nullptr , 16);
    ba[1] = c2.toUInt(nullptr , 16);
    ba[2] = c3.toUInt(nullptr , 16);
    ba[3] = c4.toUInt(nullptr , 16);*/

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
        if(!s.isEmpty())
        {
            char c = s.toInt(&ok,16)&0xFF;
            if(ok){
                ret.append(c);
            }else{
                qDebug()<<"非法的16进制字符："<<s;
                QMessageBox::warning(0,tr("错误："),QString("非法的16进制字符: \"%1\"").arg(s));
            }
        }
    }
    qDebug()<<ret;
    return ret;
}

void MainWindow::on_table_acpi_add_cellClicked(int row, int column)
{

    enabled_change(ui->table_acpi_add , row , column , 2);


}

void MainWindow::init_enabled_data(QTableWidget *table , int row , int column , QString str)
{

    QTableWidgetItem *chkbox = new QTableWidgetItem(str);

    table->setItem(row, column, chkbox);
    table->item(row , column)->setTextAlignment(Qt::AlignCenter);
    if(str == "true")

        chkbox->setCheckState(Qt::Checked);
    else

        chkbox->setCheckState(Qt::Unchecked);

}

void MainWindow::enabled_change(QTableWidget *table , int row , int column , int cc)
{
    if(column == cc)
    {
        if(table->item(row , column)->checkState() == Qt::Checked)
        {

            table->item(row , column)->setTextAlignment(Qt::AlignCenter);
            table->item(row , column)->setText("true");

        }
        else
        {
            if(table->item(row , column)->checkState() == Qt::Unchecked)
            {

                table->item(row , column)->setTextAlignment(Qt::AlignCenter);
                table->item(row , column)->setText("false");

            }
        }

    }
}



void MainWindow::on_table_acpi_del_cellClicked(int row, int column)
{
    enabled_change(ui->table_acpi_del , row , column , 4);
    enabled_change(ui->table_acpi_del , row , column , 5);
}

void MainWindow::on_table_acpi_patch_cellClicked(int row, int column)
{
    enabled_change(ui->table_acpi_patch , row , column , 11);
}

void MainWindow::on_table_booter_cellClicked(int row, int column)
{
    enabled_change(ui->table_booter , row , column , 2);
}

void MainWindow::on_table_kernel_add_cellClicked(int row, int column)
{
    enabled_change(ui->table_kernel_add , row , column , 6);
}

void MainWindow::on_table_kernel_block_cellClicked(int row, int column)
{
    enabled_change(ui->table_kernel_block , row , column , 4);
}

void MainWindow::on_table_kernel_patch_cellClicked(int row, int column)
{
    enabled_change(ui->table_kernel_patch , row , column , 12);
}

void MainWindow::on_tableEntries_cellClicked(int row, int column)
{
    enabled_change(ui->tableEntries , row , column , 5);

    enabled_change(ui->tableEntries , row , column , 4);
}

void MainWindow::on_tableTools_cellClicked(int row, int column)
{
    enabled_change(ui->tableTools , row , column , 5);

    enabled_change(ui->tableTools , row , column , 4);
}

void MainWindow::on_table_uefi_ReservedMemory_cellClicked(int row, int column)
{
    enabled_change(ui->table_uefi_ReservedMemory , row , column , 3);
}



void MainWindow::on_btnKernelPatchAdd_clicked()
{
    add_item(ui->table_kernel_patch , 12);
    init_enabled_data(ui->table_kernel_patch , ui->table_kernel_patch->rowCount() - 1 , 12 , "true");
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

    loadding = true;

    ParserACPI(map);
    ParserBooter(map);
    ParserDP(map);
    ParserKernel(map);
    ParserMisc(map);
    ParserNvram(map);
    ParserPlatformInfo(map);
    ParserUEFI(map);

    loadding = false;
}

void MainWindow::on_btnQuickOpen2_clicked()
{
    QString FileName = QDir::homePath() + "/SampleFull.plist";

    QFile file(FileName);
    QVariantMap map = PListParser::parsePList(&file).toMap();

    loadding = true;

    ParserACPI(map);
    ParserBooter(map);
    ParserDP(map);
    ParserKernel(map);
    ParserMisc(map);
    ParserNvram(map);
    ParserPlatformInfo(map);
    ParserUEFI(map);

    loadding = false;

}

void MainWindow::on_btnQuickOpen3_clicked()
{
    QString FileName = QDir::homePath() + "/test.plist";

    QFile file(FileName);
    QVariantMap map = PListParser::parsePList(&file).toMap();

    loadding = true;

    ParserACPI(map);
    ParserBooter(map);
    ParserDP(map);
    ParserKernel(map);
    ParserMisc(map);
    ParserNvram(map);
    ParserPlatformInfo(map);
    ParserUEFI(map);

    loadding = false;
}

void MainWindow::add_item(QTableWidget *table , int total_column)
{
    int t = table->rowCount();
    table->setRowCount(t + 1);

    //用""初始化各项值
    for(int i = 0; i < total_column; i++)
    {
        table->setItem(t , i , new QTableWidgetItem(""));
    }
    table->setFocus();
    table->setCurrentCell(t , 0);
}

void MainWindow::del_item(QTableWidget *table)
{
    int t = table->currentRow();
    table->removeRow(t);
    table->setFocus();
    table->setCurrentCell(t - 1 , 0);
    if(t == 0)
        table->setCurrentCell(0 , 0);

}

void MainWindow::on_btnACPIAdd_Del_clicked()
{
    del_item(ui->table_acpi_add);
}

void MainWindow::on_btnACPIDel_Add_clicked()
{
    add_item(ui->table_acpi_del , 5);

    init_enabled_data(ui->table_acpi_del , ui->table_acpi_del->rowCount() - 1 , 4 , "false");
    init_enabled_data(ui->table_acpi_del , ui->table_acpi_del->rowCount() - 1 , 5 , "true");
}

void MainWindow::on_btnACPIDel_Del_clicked()
{
    del_item(ui->table_acpi_del);
}

void MainWindow::on_btnACPIPatch_Add_clicked()
{
    add_item(ui->table_acpi_patch , 11);
    init_enabled_data(ui->table_acpi_patch , ui->table_acpi_patch->rowCount() - 1 , 11 , "true");
}

void MainWindow::on_btnACPIPatch_Del_clicked()
{
    del_item(ui->table_acpi_patch);
}

void MainWindow::on_btnBooter_Add_clicked()
{
    add_item(ui->table_booter , 2);
    init_enabled_data(ui->table_booter , ui->table_booter->rowCount() - 1 , 2 , "true");
}

void MainWindow::on_btnBooter_Del_clicked()
{
    del_item(ui->table_booter);
}

void MainWindow::on_btnDP_Add0_clicked()
{
    //add_item(ui->table_dp_add0);
}

void MainWindow::on_btnDP_Del0_clicked()
{
    del_item(ui->table_dp_add0);
}

void MainWindow::on_btnDP_Add_clicked()
{
    //add_item(ui->table_dp_add);
}

void MainWindow::on_btnDP_Del_clicked()
{
    del_item(ui->table_dp_add);
}

void MainWindow::on_btnDPDel_Add0_clicked()
{
    add_item(ui->table_dp_del0 , 1);
    ui->table_dp_del->setRowCount(0); //先清除右边表中的所有条目
    on_btnDPDel_Add_clicked(); //同时右边增加一个新条目

    write_value_ini(ui->table_dp_del0->objectName() , ui->table_dp_del , ui->table_dp_del0->rowCount() - 1);

}

void MainWindow::on_btnDPDel_Del0_clicked()
{
    //先记住被删的条目位置
    int delindex = ui->table_dp_del0->currentRow();
    int count = ui->table_dp_del0->rowCount();

    QString qz = QDir::homePath() + "/" + ui->table_dp_del0->objectName();
    QFile file(qz + QString::number(delindex + 1) + ".ini");
    del_item(ui->table_dp_del0);
    if(file.exists())
        file.remove();

    //改名，以适应新的索引
    if(delindex < count)
    {
        for(int i = delindex; i < ui->table_dp_del0->rowCount(); i ++)
        {
            QFile file(qz + QString::number(i + 2) + ".ini");
            file.rename(qz + QString::number(i + 1) + ".ini");

        }
    }

    if(ui->table_dp_del0->rowCount() == 0)
    {
        ui->table_dp_del->setRowCount(0);
    }

    on_table_dp_del0_cellClicked(ui->table_dp_del0->currentRow() , 0);
}

void MainWindow::on_btnDPDel_Add_clicked()
{
    if(ui->table_dp_del0->rowCount() == 0)
        return;

    loadding = true;
    add_item(ui->table_dp_del , 1);
    loadding = false;

    //保存数据
    write_value_ini(ui->table_dp_del0->objectName() , ui->table_dp_del , ui->table_dp_del0->currentRow());
}

void MainWindow::on_btnDPDel_Del_clicked()
{
    del_item(ui->table_dp_del);

    //保存数据
    write_value_ini(ui->table_dp_del0->objectName() , ui->table_dp_del , ui->table_dp_del0->currentRow());
}

void MainWindow::on_btnACPIAdd_Add_clicked()
{
    QFileDialog fd;

    QString FileName = fd.getOpenFileName(this,"文件","","文件(*.aml);;所有文件(*.*)");
    if(FileName.isEmpty())

        return;

    //QFile file(FileName);
    int row = ui->table_acpi_add->rowCount() + 1;

    ui->table_acpi_add->setRowCount(row);
    ui->table_acpi_add->setItem(row - 1 , 0 , new QTableWidgetItem(QFileInfo(FileName).fileName()));
    ui->table_acpi_add->setItem(row - 1 , 1 , new QTableWidgetItem(""));
    init_enabled_data(ui->table_acpi_add , row - 1 , 2 , "true");

    ui->table_acpi_add->setFocus();
    ui->table_acpi_add->setCurrentCell(row - 1 , 0);

}

void MainWindow::on_btnDPAdd_Add0_clicked()
{
    add_item(ui->table_dp_add0 , 1);
    ui->table_dp_add->setRowCount(0); //先清除右边表中的所有条目
    on_btnDPAdd_Add_clicked(); //同时右边增加一个新条目
    write_ini(ui->table_dp_add0->objectName() , ui->table_dp_add , ui->table_dp_add0->rowCount() - 1);

}

void MainWindow::on_btnDPAdd_Del0_clicked()
{
    //先记住被删的条目位置
    int delindex = ui->table_dp_add0->currentRow();
    int count = ui->table_dp_add0->rowCount();

    QString qz = QDir::homePath() + "/" + ui->table_dp_add0->objectName();
    QFile file(qz + QString::number(delindex + 1) + ".ini");
    del_item(ui->table_dp_add0);
    if(file.exists())
        file.remove();

    //改名，以适应新的索引
    if(delindex < count)
    {
        for(int i = delindex; i < ui->table_dp_add0->rowCount(); i ++)
        {
            QFile file(qz + QString::number(i + 2) + ".ini");
            file.rename(qz + QString::number(i + 1) + ".ini");

        }
    }

    if(ui->table_dp_add0->rowCount() == 0)
    {
        ui->table_dp_add->setRowCount(0);
    }

    on_table_dp_add0_cellClicked(ui->table_dp_add0->currentRow() , 0);
}

void MainWindow::on_btnDPAdd_Add_clicked()
{
    if(ui->table_dp_add0->rowCount() == 0)
        return;

    loadding = true;
    add_item(ui->table_dp_add , 3);
    loadding = false;

    //保存数据
    write_ini(ui->table_dp_add0->objectName() , ui->table_dp_add , ui->table_dp_add0->currentRow());
}

void MainWindow::on_btnDPAdd_Del_clicked()
{
    del_item(ui->table_dp_add);
    write_ini(ui->table_dp_add0->objectName() , ui->table_dp_add , ui->table_dp_add0->currentRow());
}

void MainWindow::on_btnKernelAdd_Add_clicked()
{
    QFileDialog fd;
    QStringList FileName;

#ifdef Q_OS_WIN32
// win
   FileName.append(fd.getExistingDirectory());
   //qDebug() << FileName[0];
#endif

#ifdef Q_OS_LINUX
// linux
   FileName.append(fd.getExistingDirectory());
#endif

#ifdef Q_OS_MAC
// mac
   FileName = fd.getOpenFileNames(this,"kext文件","","kext文件(*.kext);;所有文件(*.*)");
#endif


    int file_count = FileName.count();

    if(file_count == 0 || FileName[0] == "")

        return;

    for(int j = 0; j < file_count; j++)
    {
        QFileInfo fileInfo(FileName[j]);

        QFileInfo fileInfoList;
        QString filePath = fileInfo.absolutePath();
        //qDebug() << filePath;
        QDir fileDir(filePath + "/" + fileInfo.fileName() + "/Contents/MacOS/");
        //qDebug() << fileDir;
        if(fileDir.exists())//如果目录存在，则遍历里面的文件
        {
            fileDir.setFilter(QDir::Files); //只遍历本目录
            QFileInfoList fileList = fileDir.entryInfoList();
            int fileCount = fileList.count();
            for(int i = 0; i < fileCount; i++)//一般只有一个二进制文件
            {
                fileInfoList = fileList[i];
            }
        }

        QTableWidget *t = new QTableWidget;
        t = ui->table_kernel_add;
        int row = t->rowCount() + 1;

        t->setRowCount(row);
        t->setItem(row - 1 , 0 , new QTableWidgetItem(QFileInfo(FileName[j]).fileName()));
        t->setItem(row - 1 , 1 , new QTableWidgetItem(""));
        t->setItem(row - 1 , 2 , new QTableWidgetItem("Contents/MacOS/" + fileInfoList.fileName()));
        t->setItem(row - 1 , 3 , new QTableWidgetItem("Contents/Info.plist"));
        t->setItem(row - 1 , 4 , new QTableWidgetItem(""));
        t->setItem(row - 1 , 5 , new QTableWidgetItem(""));
        init_enabled_data(t , row - 1 , 6 , "true");


        //如果里面还有PlugIns目录，则需要继续遍历插件目录
        QDir piDir(filePath + "/" + fileInfo.fileName() + "/Contents/PlugIns/");
        //qDebug() << piDir;
        if(piDir.exists())
        {

            piDir.setFilter(QDir::Dirs); //过滤器：只遍历里面的目录
            QFileInfoList fileList = piDir.entryInfoList();
            int fileCount = fileList.count();
            QVector<QString> kext_file;
            qDebug() << fileCount;
            for(int i = 0; i < fileCount; i++)//找出里面的kext文件(目录）
            {
                kext_file.push_back(fileList[i].fileName());
                qDebug() << kext_file.at(i);
            }

            if(fileCount >= 3) //里面有目录
            {
                for(int i = 0; i < fileCount - 2; i++)
                {
                    QDir fileDir(filePath + "/" + fileInfo.fileName() + "/Contents/PlugIns/" + kext_file[i + 2] + "/Contents/MacOS/");
                    if(fileDir.exists())
                    {
                        //qDebug() << fileDir;
                        fileDir.setFilter(QDir::Files); //只遍历本目录里面的文件
                        QFileInfoList fileList = fileDir.entryInfoList();
                        int fileCount = fileList.count();
                        for(int i = 0; i < fileCount; i++)//一般只有一个二进制文件
                        {
                            fileInfoList = fileList[i];
                        }

                        //写入到表里
                        int row = t->rowCount() + 1;

                        t->setRowCount(row);
                        t->setItem(row - 1 , 0 , new QTableWidgetItem(QFileInfo(FileName[j]).fileName() + "/Contents/PlugIns/" + kext_file[i + 2]));
                        t->setItem(row - 1 , 1 , new QTableWidgetItem(""));
                        t->setItem(row - 1 , 2 , new QTableWidgetItem("Contents/MacOS/" + fileInfoList.fileName()));
                        t->setItem(row - 1 , 3 , new QTableWidgetItem("Contents/Info.plist"));
                        t->setItem(row - 1 , 4 , new QTableWidgetItem(""));
                        t->setItem(row - 1 , 5 , new QTableWidgetItem(""));
                        init_enabled_data(t , row - 1 , 6 , "true");

                    }
                }
            }

        }


        t->setFocus();
        t->setCurrentCell(row - 1 , 0);

    }




}

void MainWindow::on_btnKernelBlock_Add_clicked()
{
    add_item(ui->table_kernel_block , 4);
    init_enabled_data(ui->table_kernel_block , ui->table_kernel_block->currentRow() , 4 , "true");
}

void MainWindow::on_btnKernelBlock_Del_clicked()
{
    del_item(ui->table_kernel_block);
}

void MainWindow::on_btnMiscBO_Add_clicked()
{
    add_item(ui->tableBlessOverride , 1);
    ui->tableBlessOverride->setItem(ui->tableBlessOverride->currentRow() , 0 ,new QTableWidgetItem(""));
}

void MainWindow::on_btnMiscBO_Del_clicked()
{
    del_item(ui->tableBlessOverride);
}

void MainWindow::on_btnMiscEntries_Add_clicked()
{
    add_item(ui->tableEntries , 5);

    init_enabled_data(ui->tableEntries , ui->tableEntries->rowCount() - 1 , 4 , "false");
    init_enabled_data(ui->tableEntries , ui->tableEntries->rowCount() - 1 , 5 , "true");
}

void MainWindow::on_btnMiscTools_Add_clicked()
{
    add_item(ui->tableTools , 5);

    init_enabled_data(ui->tableTools , ui->tableTools->rowCount() - 1 , 4 , "false");
    init_enabled_data(ui->tableTools , ui->tableTools->rowCount() - 1 , 5 , "true");
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
    add_item(ui->table_nv_add0 , 1);
    ui->table_nv_add->setRowCount(0); //先清除右边表中的所有条目
    on_btnNVRAMAdd_Add_clicked(); //同时右边增加一个新条目

    write_ini(ui->table_nv_add0->objectName() , ui->table_nv_add , ui->table_nv_add0->rowCount() - 1);

}

void MainWindow::on_btnNVRAMAdd_Add_clicked()
{
    if(ui->table_nv_add0->rowCount() == 0)
        return;

    loadding = true;
    add_item(ui->table_nv_add , 3);
    loadding = false;

    //保存数据
    write_ini(ui->table_nv_add0->objectName() , ui->table_nv_add , ui->table_nv_add0->currentRow());
}

void MainWindow::on_btnNVRAMAdd_Del0_clicked()
{
    //先记住被删的条目位置
    int delindex = ui->table_nv_add0->currentRow();
    int count = ui->table_nv_add0->rowCount();

    QString qz = QDir::homePath() + "/" + ui->table_nv_add0->objectName();
    QFile file(qz + QString::number(delindex + 1) + ".ini");
    del_item(ui->table_nv_add0);
    if(file.exists())
        file.remove();

    //改名，以适应新的索引
    if(delindex < count)
    {
        for(int i = delindex; i < ui->table_nv_add0->rowCount(); i ++)
        {
            QFile file(qz + QString::number(i + 2) + ".ini");
            file.rename(qz + QString::number(i + 1) + ".ini");

        }
    }

    if(ui->table_nv_add0->rowCount() == 0)
    {
        ui->table_nv_add->setRowCount(0);
    }

    on_table_nv_add0_cellClicked(ui->table_nv_add0->currentRow() , 0);
}

void MainWindow::on_btnNVRAMAdd_Del_clicked()
{
    del_item(ui->table_nv_add);
    write_ini(ui->table_nv_add0->objectName() , ui->table_nv_add , ui->table_nv_add0->currentRow());
}

void MainWindow::on_btnNVRAMDel_Add0_clicked()
{
    add_item(ui->table_nv_del0 , 1);
    ui->table_nv_del->setRowCount(0); //先清除右边表中的所有条目
    on_btnNVRAMDel_Add_clicked(); //同时右边增加一个新条目

    write_value_ini(ui->table_nv_del0->objectName() , ui->table_nv_del , ui->table_nv_del0->rowCount() - 1);

}

void MainWindow::on_btnNVRAMDel_Add_clicked()
{
    if(ui->table_nv_del0->rowCount() == 0)
        return;

    loadding = true;
    add_item(ui->table_nv_del , 1);
    loadding = false;

    //保存数据
    write_value_ini(ui->table_nv_del0->objectName() , ui->table_nv_del , ui->table_nv_del0->currentRow());

}

void MainWindow::on_btnNVRAMLS_Add0_clicked()
{
    add_item(ui->table_nv_ls0 , 1);
    ui->table_nv_ls->setRowCount(0); //先清除右边表中的所有条目
    on_btnNVRAMLS_Add_clicked(); //同时右边增加一个新条目

    write_value_ini(ui->table_nv_ls0->objectName() , ui->table_nv_ls , ui->table_nv_ls0->rowCount() - 1);

}

void MainWindow::on_btnNVRAMLS_Add_clicked()
{
    if(ui->table_nv_ls0->rowCount() == 0)
        return;

    loadding = true;
    add_item(ui->table_nv_ls , 1);
    loadding = false;

    //保存数据
    write_value_ini(ui->table_nv_ls0->objectName() , ui->table_nv_ls , ui->table_nv_ls0->currentRow());

}

void MainWindow::on_btnNVRAMDel_Del0_clicked()
{
    //先记住被删的条目位置
    int delindex = ui->table_nv_del0->currentRow();
    int count = ui->table_nv_del0->rowCount();

    QString qz = QDir::homePath() + "/" + ui->table_nv_del0->objectName();
    QFile file(qz + QString::number(delindex + 1) + ".ini");
    del_item(ui->table_nv_del0);
    if(file.exists())
        file.remove();

    //改名，以适应新的索引
    if(delindex < count)
    {
        for(int i = delindex; i < ui->table_nv_del0->rowCount(); i ++)
        {
            QFile file(qz + QString::number(i + 2) + ".ini");
            file.rename(qz + QString::number(i + 1) + ".ini");

        }
    }

    if(ui->table_nv_del0->rowCount() == 0)
    {
        ui->table_nv_del->setRowCount(0);
    }

    on_table_nv_del0_cellClicked(ui->table_nv_del0->currentRow() , 0);
}

void MainWindow::on_btnNVRAMLS_Del0_clicked()
{
    //先记住被删的条目位置
    int delindex = ui->table_nv_ls0->currentRow();
    int count = ui->table_nv_ls0->rowCount();

    QString qz = QDir::homePath() + "/" + ui->table_nv_ls0->objectName();
    QFile file(qz + QString::number(delindex + 1) + ".ini");
    del_item(ui->table_nv_ls0);
    if(file.exists())
        file.remove();

    //改名，以适应新的索引
    if(delindex < count)
    {
        for(int i = delindex; i < ui->table_nv_ls0->rowCount(); i ++)
        {
            QFile file(qz + QString::number(i + 2) + ".ini");
            file.rename(qz + QString::number(i + 1) + ".ini");

        }
    }

    if(ui->table_nv_ls0->rowCount() == 0)
    {
        ui->table_nv_ls->setRowCount(0);
    }

    on_table_nv_ls0_cellClicked(ui->table_nv_ls0->currentRow() , 0);
}

void MainWindow::on_btnNVRAMDel_Del_clicked()
{
    del_item(ui->table_nv_del);

    //保存数据
    write_value_ini(ui->table_nv_del0->objectName() , ui->table_nv_del , ui->table_nv_del0->currentRow());
}

void MainWindow::on_btnNVRAMLS_Del_clicked()
{
    del_item(ui->table_nv_ls);

    //保存数据
    write_value_ini(ui->table_nv_ls0->objectName() , ui->table_nv_ls , ui->table_nv_ls0->currentRow());
}

void MainWindow::on_btnUEFIRM_Add_clicked()
{
    add_item(ui->table_uefi_ReservedMemory , 3);
    init_enabled_data(ui->table_uefi_ReservedMemory , ui->table_uefi_ReservedMemory->rowCount() - 1 , 3 , "true");
}

void MainWindow::on_btnUEFIRM_Del_clicked()
{
    del_item(ui->table_uefi_ReservedMemory);
}

void MainWindow::on_btnUEFIDrivers_Add_clicked()
{
    QFileDialog fd;

    QString FileName = fd.getOpenFileName(this,"文件","","文件(*.efi);;所有文件(*.*)");
    if(FileName.isEmpty())

        return;

    //QFile file(FileName);
    int row = ui->table_uefi_drivers->rowCount() + 1;

    ui->table_uefi_drivers->setRowCount(row);
    ui->table_uefi_drivers->setItem(row - 1 , 0 , new QTableWidgetItem(QFileInfo(FileName).fileName()));

    ui->table_uefi_drivers->setFocus();
    ui->table_uefi_drivers->setCurrentCell(row - 1 , 0);
}

void MainWindow::on_btnUEFIDrivers_Del_clicked()
{
    del_item(ui->table_uefi_drivers);
}

void MainWindow::on_btnKernelAdd_Up_clicked()
{
    QTableWidget *t = new QTableWidget;
    t = ui->table_kernel_add;

    if(t->currentRow() == 0)
        return;

    int cr = t->currentRow();
    //先将上面的内容进行备份
    QString item[7];
    item[0] = t->item(cr - 1 , 0)->text();
    item[1] = t->item(cr - 1 , 1)->text();
    item[2] = t->item(cr - 1 , 2)->text();
    item[3] = t->item(cr - 1 , 3)->text();
    item[4] = t->item(cr - 1 , 4)->text();
    item[5] = t->item(cr - 1 , 5)->text();
    item[6] = t->item(cr - 1 , 6)->text();
    //将下面的内容移到上面
    t->item(cr - 1 , 0)->setText(t->item(cr , 0)->text());
    t->item(cr - 1 , 1)->setText(t->item(cr , 1)->text());
    t->item(cr - 1 , 2)->setText(t->item(cr , 2)->text());
    t->item(cr - 1 , 3)->setText(t->item(cr , 3)->text());
    t->item(cr - 1 , 4)->setText(t->item(cr , 4)->text());
    t->item(cr - 1 , 5)->setText(t->item(cr , 5)->text());
    t->item(cr - 1 , 6)->setText(t->item(cr , 6)->text());
    if(t->item(cr , 6)->text() == "true")
        t->item(cr - 1 , 6)->setCheckState(Qt::Checked);
    else
        t->item(cr - 1 , 6)->setCheckState(Qt::Unchecked);

    //最后将之前的备份恢复到下面
    t->item(cr , 0)->setText(item[0]);
    t->item(cr , 1)->setText(item[1]);
    t->item(cr , 2)->setText(item[2]);
    t->item(cr , 3)->setText(item[3]);
    t->item(cr , 4)->setText(item[4]);
    t->item(cr , 5)->setText(item[5]);
    t->item(cr , 6)->setText(item[6]);
    if(item[6] == "true")
        t->item(cr , 6)->setCheckState(Qt::Checked);
    else
        t->item(cr , 6)->setCheckState(Qt::Unchecked);

    t->setCurrentCell(cr - 1 , t->currentColumn());

}

void MainWindow::on_btnKernelAdd_Down_clicked()
{
    QTableWidget *t = new QTableWidget;
    t = ui->table_kernel_add;

    if(t->currentRow() == t->rowCount() - 1)
        return;

    int cr = t->currentRow();
    //先将下面的内容进行备份
    QString item[7];
    item[0] = t->item(cr + 1 , 0)->text();
    item[1] = t->item(cr + 1 , 1)->text();
    item[2] = t->item(cr + 1 , 2)->text();
    item[3] = t->item(cr + 1 , 3)->text();
    item[4] = t->item(cr + 1 , 4)->text();
    item[5] = t->item(cr + 1 , 5)->text();
    item[6] = t->item(cr + 1 , 6)->text();
    //将上面的内容移到下面
    t->item(cr + 1 , 0)->setText(t->item(cr , 0)->text());
    t->item(cr + 1 , 1)->setText(t->item(cr , 1)->text());
    t->item(cr + 1 , 2)->setText(t->item(cr , 2)->text());
    t->item(cr + 1 , 3)->setText(t->item(cr , 3)->text());
    t->item(cr + 1 , 4)->setText(t->item(cr , 4)->text());
    t->item(cr + 1 , 5)->setText(t->item(cr , 5)->text());
    t->item(cr + 1 , 6)->setText(t->item(cr , 6)->text());
    if(t->item(cr , 6)->text() == "true")
        t->item(cr + 1 , 6)->setCheckState(Qt::Checked);
    else
        t->item(cr + 1 , 6)->setCheckState(Qt::Unchecked);

    //最后将之前的备份恢复到上面
    t->item(cr , 0)->setText(item[0]);
    t->item(cr , 1)->setText(item[1]);
    t->item(cr , 2)->setText(item[2]);
    t->item(cr , 3)->setText(item[3]);
    t->item(cr , 4)->setText(item[4]);
    t->item(cr , 5)->setText(item[5]);
    t->item(cr , 6)->setText(item[6]);
    if(item[6] == "true")
        t->item(cr , 6)->setCheckState(Qt::Checked);
    else
        t->item(cr , 6)->setCheckState(Qt::Unchecked);

    t->setCurrentCell(cr + 1 , t->currentColumn());

}

void MainWindow::test(bool test)
{
    if(test)
    {
        ui->btnTestWrite->setVisible(1);
        ui->btnTestRead->setVisible(1);

        ui->btnParse->setVisible(1);
        ui->btnSerialize->setVisible(1);

        ui->btnQuickOpen1->setVisible(1);
        ui->btnQuickOpen2->setVisible(1);
        ui->btnQuickOpen3->setVisible(1);

    }
    else
    {
        ui->btnTestWrite->setVisible(0);
        ui->btnTestRead->setVisible(0);

        ui->btnParse->setVisible(0);
        ui->btnSerialize->setVisible(0);

        ui->btnQuickOpen1->setVisible(0);
        ui->btnQuickOpen2->setVisible(0);
        ui->btnQuickOpen3->setVisible(0);
    }
}

void MainWindow::on_btnSaveAs_clicked()
{
    QFileDialog fd;

    PlistFileName = fd.getSaveFileName(this,"配置文件","","配置文件(*.plist);;所有文件(*.*)");
    if(!PlistFileName.isEmpty())
        setWindowTitle(title + "    " + PlistFileName);
    else
        return;

    SavePlist(PlistFileName);

    ui->btnSave->setEnabled(true);
    ui->actionSave->setEnabled(true);

}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Application"),
             tr("OpenCoreConfig is an open source full-platform OpenCore configuration file manager."));
}

void MainWindow::on_btnKernelAdd_Del_clicked()
{
    del_item(ui->table_kernel_add);
}

void MainWindow::on_table_dp_add_cellClicked(int row, int column)
{

    if(column == 1)
    {

        cboxDataClass = new QComboBox;
        cboxDataClass->addItem("Data");
        cboxDataClass->addItem("String");
        cboxDataClass->addItem("Number");
        cboxDataClass->addItem("");
        connect(cboxDataClass, SIGNAL(currentIndexChanged(QString)), this, SLOT(dataClassChange_dp()));
        c_row = row;

        ui->table_dp_add->setCellWidget(row , column , cboxDataClass);
        cboxDataClass->setCurrentText(ui->table_dp_add->item(row , 1)->text());

    }

}

void MainWindow::on_table_dp_add_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    if(currentRow == 0 && currentColumn == 0 && previousColumn == 0)
    {

    }

    ui->table_dp_add->removeCellWidget(previousRow , 1);

}

void MainWindow::dataClassChange_dp()
{
    ui->table_dp_add->item(c_row , 1)->setText(cboxDataClass->currentText());
}

void MainWindow::dataClassChange_nv()
{
    ui->table_nv_add->item(c_row , 1)->setText(cboxDataClass->currentText());
}

void MainWindow::on_table_nv_add_cellClicked(int row, int column)
{

    if(column == 1)
    {
        cboxDataClass = new QComboBox;
        cboxDataClass->addItem("Data");
        cboxDataClass->addItem("String");
        cboxDataClass->addItem("Number");
        cboxDataClass->addItem("");
        connect(cboxDataClass, SIGNAL(currentIndexChanged(QString)), this, SLOT(dataClassChange_nv()));
        c_row = row;


        ui->table_nv_add->setCellWidget(row , column , cboxDataClass);
        cboxDataClass->setCurrentText(ui->table_nv_add->item(row , 1)->text());


    }
}

void MainWindow::on_table_nv_add_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    if(currentRow == 0 && currentColumn == 0 && previousColumn == 0)
    {

    }

    ui->table_nv_add->removeCellWidget(previousRow , 1);
}
