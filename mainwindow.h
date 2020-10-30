#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QDateTime>
#include <QSplitter>
#include <QProcess>
#include <QTextCodec>
#include <QTextBlock>
#include <QUuid>
#include <QMimeData>
#include <QSaveFile>
#include <QTranslator>
#include <QDesktopServices>
#include <QToolTip>
#include <QProxyStyle>
#include <QPainter>
#include <QDebug>

#ifdef Q_OS_WIN32
#include <stdio.h>
#include<windows.h>
#include<tchar.h>
#endif

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void ParserACPI(QVariantMap map);//分析ACPI
    void ParserBooter(QVariantMap map);
    void ParserDP(QVariantMap map);
    void ParserKernel(QVariantMap map);
    void ParserMisc(QVariantMap map);
    void ParserNvram(QVariantMap map);
    void ParserPlatformInfo(QVariantMap map);
    void ParserUEFI(QVariantMap map);
    void openFile(QString PlistFileName);

    void SavePlist(QString FileName);
    bool getBool(QTableWidget *table , int row, int column);
    bool getChkBool(QCheckBox *chkbox);
    QVariantMap SaveACPI();
    QVariantMap SaveBooter();
    QVariantMap SaveDeviceProperties();
    QVariantMap SaveKernel();
    QVariantMap SaveMisc();
    QVariantMap SaveNVRAM();
    QVariantMap SavePlatformInfo();
    QVariantMap SaveUEFI();

    void initui_acpi();//初始化acpiui
    void initui_booter();
    void initui_dp();
    void initui_kernel();
    void initui_misc();
    void initui_nvram();
    void initui_PlatformInfo();
    void initui_UEFI();
    void init_enabled_data(QTableWidget *table , int row , int column , QString str);
    void enabled_change(QTableWidget *table , int row , int column , int cc);
    void add_item(QTableWidget *table , int total_column);
    void del_item(QTableWidget *table);

    void write_ini(QString table_name , QTableWidget *mytable, int i);
    void read_ini(QString table_name , QTableWidget *mytable , int i);

    void init_key_class_value(QTableWidget *table , QTableWidget *subtable);
    void init_value(QVariantMap map_fun , QTableWidget *table, QTableWidget *subtable);
    void write_value_ini(QString tablename , QTableWidget *subtable , int i);
    void read_value_ini(QString table_name , QTableWidget *mytable , int i);

    QByteArray HexStringToByteArray(QString HexString);
    QByteArray HexStrToByte(QString value);
    QString ByteToHexStr(QByteArray ba);

    QString title;
    bool loading = false; //数据是否加载中，用于 设备属性 等表

    void test(bool test);//用于测试按钮的可视

    void about();

    void loadLocal();
    bool zh_cn = false;

    QComboBox *cboxDataClass;
    int c_row = 0;

    QComboBox *cboxArch;

    QComboBox *cboxReservedMemoryType;


public slots:

protected:
    void dragEnterEvent (QDragEnterEvent *e);
    void dropEvent (QDropEvent *e);
    void closeEvent(QCloseEvent *event);


private slots:
    void dataClassChange_dp();
    void dataClassChange_nv();
    void readResult();
    void readResultSystemInfo();
    void readResultDiskInfo();
    void arch_addChange();
    void arch_ForceChange();
    void arch_blockChange();
    void arch_patchChange();
    void ReservedMemoryTypeChange();

    void on_btnOpen_clicked();

    void on_btnTestWrite_clicked();

    void on_btnParse_clicked();

    void on_btnSerialize_clicked();

    void on_table_dp_add0_cellClicked(int row, int column);

    void on_table_dp_add_itemSelectionChanged();

    void on_table_dp_add_itemChanged(QTableWidgetItem *item);

    void on_table_nv_add0_cellClicked(int row, int column);

    void on_table_nv_add_itemChanged(QTableWidgetItem *item);

    void on_table_nv_del0_cellClicked(int row, int column);

    void on_table_nv_ls0_cellClicked(int row, int column);

    void on_table_nv_del_itemChanged(QTableWidgetItem *item);

    void on_table_nv_ls_itemChanged(QTableWidgetItem *item);

    void on_table_dp_del0_cellClicked(int row, int column);

    void on_table_dp_del_itemChanged(QTableWidgetItem *item);

    void on_btnSave_clicked();

    void on_table_acpi_add_cellClicked(int row, int column);

    void on_table_acpi_del_cellClicked(int row, int column);

    void on_table_acpi_patch_cellClicked(int row, int column);

    void on_table_booter_cellClicked(int row, int column);

    void on_table_kernel_add_cellClicked(int row, int column);

    void on_table_kernel_block_cellClicked(int row, int column);

    void on_table_kernel_patch_cellClicked(int row, int column);

    void on_tableEntries_cellClicked(int row, int column);

    void on_tableTools_cellClicked(int row, int column);

    void on_table_uefi_ReservedMemory_cellClicked(int row, int column);

    void on_btnKernelPatchAdd_clicked();

    void on_btnKernelPatchDel_clicked();

    void on_btnQuickOpen1_clicked();

    void on_btnQuickOpen2_clicked();

    void on_btnACPIAdd_Del_clicked();

    void on_btnACPIDel_Add_clicked();

    void on_btnACPIDel_Del_clicked();

    void on_btnACPIPatch_Add_clicked();

    void on_btnACPIPatch_Del_clicked();

    void on_btnBooter_Add_clicked();

    void on_btnBooter_Del_clicked();

    void on_btnDPDel_Add0_clicked();

    void on_btnDPDel_Del0_clicked();

    void on_btnDPDel_Add_clicked();

    void on_btnDPDel_Del_clicked();

    void on_btnACPIAdd_Add_clicked();

    void on_btnDPAdd_Add0_clicked();

    void on_btnDPAdd_Del0_clicked();

    void on_btnDPAdd_Add_clicked();

    void on_btnDPAdd_Del_clicked();

    void on_btnKernelAdd_Add_clicked();

    void on_btnKernelBlock_Add_clicked();

    void on_btnKernelBlock_Del_clicked();

    void on_btnMiscBO_Add_clicked();

    void on_btnMiscBO_Del_clicked();

    void on_btnMiscEntries_Add_clicked();

    void on_btnMiscTools_Add_clicked();

    void on_btnMiscEntries_Del_clicked();

    void on_btnMiscTools_Del_clicked();

    void on_btnNVRAMAdd_Add0_clicked();

    void on_btnNVRAMAdd_Add_clicked();

    void on_btnNVRAMAdd_Del0_clicked();

    void on_btnNVRAMAdd_Del_clicked();

    void on_btnNVRAMDel_Add0_clicked();

    void on_btnNVRAMDel_Add_clicked();

    void on_btnNVRAMLS_Add0_clicked();

    void on_btnNVRAMLS_Add_clicked();

    void on_btnNVRAMDel_Del0_clicked();

    void on_btnNVRAMLS_Del0_clicked();

    void on_btnNVRAMDel_Del_clicked();

    void on_btnNVRAMLS_Del_clicked();

    void on_btnUEFIRM_Add_clicked();

    void on_btnUEFIRM_Del_clicked();

    void on_btnUEFIDrivers_Add_clicked();

    void on_btnUEFIDrivers_Del_clicked();

    void on_btnKernelAdd_Up_clicked();

    void on_btnKernelAdd_Down_clicked();

    void on_btnSaveAs_clicked();

    void on_btnKernelAdd_Del_clicked();

    void on_table_dp_add_cellClicked(int row, int column);

    void on_table_dp_add_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

    void on_table_nv_add_cellClicked(int row, int column);

    void on_table_nv_add_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

    void on_table_kernel_add_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

    void on_table_kernel_block_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

    void on_table_kernel_patch_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

    void on_cboxSystemProductName_currentIndexChanged(const QString &arg1);

    void on_btnGenerate_clicked();

    void on_btnSystemUUID_clicked();

    void on_table_kernel_Force_cellClicked(int row, int column);

    void on_table_kernel_Force_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

    void on_btnKernelForce_Add_clicked();

    void on_btnKernelForce_Del_clicked();

    void on_btnMountEsp_clicked();

    void on_table_uefi_ReservedMemory_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

    void on_btnHelp_clicked();

    void on_btnExportMaster_clicked();

    void on_btnImportMaster_clicked();

    void on_tabTotal_tabBarClicked(int index);

    void on_tabTotal_currentChanged(int index);

    void on_btnDevices_add_clicked();

    void on_btnDevices_del_clicked();

    void on_cboxUpdateSMBIOSMode_currentIndexChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    void reg_win();
    void mount_esp();
    void mount_esp_mac(QString strEfiDisk);
    void runAdmin(QString file, QString arg);
    QString getSystemProductName(QString arg1);
    QProcess *gs;
    QProcess *si;
    QProcess *di;
    QVector<QString> IniFile;

    void init_tr_str();

    QString strArch;
    QString strBundlePath;
    QString strComment;
    QString strEnabled;
    QString strExecutablePath;
    QString strMaxKernel;
    QString strMinKernel;
    QString strPlistPath;

};

class CustomTabStyle1 : public QProxyStyle//继承QProxyStyle
{
public:
    QSize sizeFromContents(ContentsType type, const QStyleOption *option,
                           const QSize &size, const QWidget *widget) const
    {
        QSize tabsize = QProxyStyle::sizeFromContents(type, option, size, widget);
        if (type == QStyle::CT_TabBarTab) {
            tabsize.transpose();
            tabsize.rwidth() = 105;
            tabsize.rheight() = 80;
        }
        return tabsize;
    }
    //重写函数drawControl，绘图控制
    //ControlElement控制元件,option,QStyle选项，painter绘图，widget对象
    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
    {
        if (element == CE_TabBarTabLabel) {//如果元件是TabBarTabLabel
            if (const QStyleOptionTab *tab = qstyleoption_cast<const QStyleOptionTab *>(option)) {
                QRect allRect = tab->rect;//获取tab矩形框
                painter->setFont(QFont("楷体", 18, QFont::Bold));
                if (tab->state & QStyle::State_Selected) {
                    painter->save();
                    painter->setPen(0x555555);//设置颜色
                    painter->setBrush(QBrush(0x282828));//设置Brush颜色
                    painter->drawRect(allRect.adjusted(3, 3, -3, -3));
                    // painter->restore();//恢复
                }
                QTextOption option;//这里设置文本样式
                option.setAlignment(Qt::AlignCenter);//设置对其方式。居中对齐
                if (tab->state & QStyle::State_Selected) {
                    painter->setPen(0xf8fcff);//设置颜色
                }
                else {
                    painter->setPen(0x5d5d5d);
                }

                painter->drawText(allRect, tab->text, option);//绘制文本
                return;
            }
        }
        if (element == CE_TabBarTab) {
            QProxyStyle::drawControl(element, option, painter, widget);
        }
    }
};
class CustomTabStyle2 : public QProxyStyle
{
public:
    QSize sizeFromContents(ContentsType type, const QStyleOption *option,
        const QSize &size, const QWidget *widget) const
    {
        QSize s = QProxyStyle::sizeFromContents(type, option, size, widget);
        if (type == QStyle::CT_TabBarTab)
        {
            s.transpose();
            s.rwidth() = 110;
            s.rheight() = 75;
        }
        return s;
    }

    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
    {

        if (element == CE_TabBarTabLabel)
        {

            if (const QStyleOptionTab *tab = qstyleoption_cast<const QStyleOptionTab *>(option))
            {

                //QRect allRect = tab->rect;

                QTextOption option;
                option.setAlignment(Qt::AlignCenter);

                QStyleOptionTab opt(*tab);
                opt.shape = QTabBar::RoundedNorth;

                //QIcon icon(":/acpi.png");
                //opt.icon = icon;



#ifdef Q_OS_WIN32
                opt.palette.setCurrentColorGroup(QPalette::Disabled);
                opt.state |= QStyle::State_Sunken;
                painter->setFont(QFont("微软雅黑", 9, QFont::Bold));
#endif

#ifdef Q_OS_LINUX
                painter->setFont(QFont("微软雅黑", 11, QFont::Bold));
#endif

#ifdef Q_OS_MAC
                opt.palette.setCurrentColorGroup(QPalette::Disabled);
                opt.state |= QStyle::State_Sunken;
                painter->setFont(QFont("微软雅黑", 15, QFont::Bold));
#endif

                //painter->drawText(allRect, tab->text, option);//绘制文本

                QProxyStyle::drawControl(element, &opt, painter, widget);

                return;
            }

        }

        QProxyStyle::drawControl(element, option, painter, widget);
    }
};

class CustomTabStyle3 : public QProxyStyle
{
public:
    QSize sizeFromContents(ContentsType type, const QStyleOption *option,
                           const QSize &size, const QWidget *widget) const
    {
        QSize tabsize = QProxyStyle::sizeFromContents(type, option, size, widget);
        if (type == QStyle::CT_TabBarTab) {
            tabsize.transpose();
            tabsize.rwidth() = 105;
            tabsize.rheight() = 75;
        }
        return tabsize;
    }
    //重写函数drawControl，绘图控制
    //ControlElement控制元件,option,QStyle选项，painter绘图，widget对象
    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
    {
        if (element == CE_TabBarTabLabel) {//如果元件是TabBarTabLabel
            if (const QStyleOptionTab *tab = qstyleoption_cast<const QStyleOptionTab *>(option)) {
                QRect allRect = tab->rect;//获取tab矩形框
                if (tab->state & QStyle::State_Selected) {
                    painter->save();
                    painter->setPen(0x89cfff);//设置颜色
                    painter->setBrush(QBrush(0x89cfff));//设置Brush颜色
                    painter->drawRect(allRect.adjusted(0, 0, -0, -0));
                    painter->restore();//恢复
                }

                QTextOption option;//这里设置文本样式
                option.setAlignment(Qt::AlignCenter);//设置对其方式。居中对齐

                if (tab->state & QStyle::State_Selected) {
                    painter->setPen(0xf8fcff);//设置颜色
                }
                else {
                    painter->setPen(0x5d5d5d);
                }
#ifdef Q_OS_WIN32
   painter->setFont(QFont("微软雅黑", 10, QFont::Bold));
#endif

#ifdef Q_OS_LINUX
  painter->setFont(QFont("微软雅黑", 12, QFont::Bold));
#endif

#ifdef Q_OS_MAC
  painter->setFont(QFont("微软雅黑", 16, QFont::Bold));
#endif
                painter->drawText(allRect, tab->text, option);//绘制文本
                return;
            }
        }
        if (element == CE_TabBarTab) {
            QProxyStyle::drawControl(element, option, painter, widget);
        }
    }
};

class CustomTabStyle4 : public QProxyStyle
{
public:
    QSize sizeFromContents(ContentsType type, const QStyleOption *option,
        const QSize &size, const QWidget *widget) const
    {
        QSize s = QProxyStyle::sizeFromContents(type, option, size, widget);
        if (type == QStyle::CT_TabBarTab) {
            s.transpose();
            s.rwidth() = 105; // 设置每个tabBar中item的大小
            s.rheight() = 75;
        }
        return s;
    }
    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
    {
        if (element == CE_TabBarTabLabel) {
            if (const QStyleOptionTab *tab = qstyleoption_cast<const QStyleOptionTab *>(option)) {
                QRect allRect = tab->rect;
                allRect.setWidth(allRect.width() + 0); //选中后的凸起
                allRect.setHeight(allRect.height() + 0);
                //选中状态
                if (tab->state & QStyle::State_Selected) {
                    //save用以保护坐标，restore用来退出状态
                    painter->save();
                    painter->setBrush(QBrush(0x004ea1));
                    //矩形
                    painter->drawRect(allRect.adjusted(0, 0, 0, 0));
                    //带有弧线矩形
                    //painter->drawRoundedRect(tab->rect, 5, 5);
                    painter->restore();
                }
                //hover状态
                else if(tab->state & QStyle::State_MouseOver){
                    painter->save();
                    //painter->setBrush(QBrush(0x004ea1));
                    painter->setBrush(QBrush(0x7B68EE));

                    painter->drawRect(allRect.adjusted(0, 0, 0, 0));
                    //painter->drawRoundedRect(allRect, 5, 5);

                    painter->restore();
                }
                else{
                    painter->save();
                    painter->setBrush(QBrush(0x78aadc));

                    painter->drawRect(allRect.adjusted(0, 0, 0, 0));//底色按钮
                    //painter->drawRoundedRect(allRect, 5, 5);

                    painter->restore();
                }

                QTextOption option;
                option.setAlignment(Qt::AlignCenter);

#ifdef Q_OS_WIN32
   painter->setFont(QFont("微软雅黑", 10, QFont::Bold));
#endif

#ifdef Q_OS_LINUX
  painter->setFont(QFont("微软雅黑", 12, QFont::Bold));
#endif

#ifdef Q_OS_MAC
  painter->setFont(QFont("微软雅黑", 16, QFont::Bold));
#endif


                painter->setPen(0xffffff);
                painter->drawText(allRect, tab->text, option);
                return;
            }
        }
        if (element == CE_TabBarTab) {
            QProxyStyle::drawControl(element, option, painter, widget);
        }
   }
};

class CustomTabStyle5 : public QProxyStyle
{
public:
    QSize sizeFromContents(ContentsType type, const QStyleOption *option,
        const QSize &size, const QWidget *widget) const
    {
        QSize s = QProxyStyle::sizeFromContents(type, option, size, widget);
        if (type == QStyle::CT_TabBarTab) {
            s.transpose();

#ifdef Q_OS_WIN32
            s.rwidth() = 330; // 设置每个tabBar中item的大小
            s.rheight() = 35;
#endif

#ifdef Q_OS_LINUX
            s.rwidth() = 230; // 设置每个tabBar中item的大小
            s.rheight() = 35;
#endif

#ifdef Q_OS_MAC
            s.rwidth() = 170; // 设置每个tabBar中item的大小
            s.rheight() = 35;
#endif

        }
        return s;
    }
    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
    {
        if (element == CE_TabBarTabLabel) {
            if (const QStyleOptionTab *tab = qstyleoption_cast<const QStyleOptionTab *>(option)) {
                QRect allRect = tab->rect;
                allRect.setWidth(allRect.width() - 2); //选中后的凸起
                allRect.setHeight(allRect.height() - 2);
                //选中状态
                if (tab->state & QStyle::State_Selected) {
                    //save用以保护坐标，restore用来退出状态
                    painter->save();
                    painter->setBrush(QBrush(0x004ea1));
                    //矩形
                    //painter->drawRect(allRect.adjusted(0, 0, 0, 0));
                    //带有弧线矩形
                    painter->drawRoundedRect(tab->rect, 5, 5);
                    painter->restore();
                }
                //hover状态
                else if(tab->state & QStyle::State_MouseOver){
                    painter->save();
                    //painter->setBrush(QBrush(0x004ea1));
                    painter->setBrush(QBrush(0x7B68EE));
                    painter->drawRoundedRect(allRect, 8, 8);
                    painter->restore();
                }
                else{
                    painter->save();
                    painter->setBrush(QBrush(0x78aadc));
                    painter->drawRoundedRect(allRect, 8, 8);
                    painter->restore();
                }
                QTextOption option;
                option.setAlignment(Qt::AlignCenter);

#ifdef Q_OS_WIN32
   painter->setFont(QFont("微软雅黑", 9, QFont::Bold));
#endif

#ifdef Q_OS_LINUX
  painter->setFont(QFont("微软雅黑", 10, QFont::Bold));
#endif

#ifdef Q_OS_MAC
  painter->setFont(QFont("微软雅黑", 13, QFont::Bold));
#endif
                painter->setPen(0xffffff);
                painter->drawText(allRect, tab->text, option);
                return;
            }
        }
        if (element == CE_TabBarTab) {
            QProxyStyle::drawControl(element, option, painter, widget);
        }
    }
};

#endif // MAINWINDOW_H
