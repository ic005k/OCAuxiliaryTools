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

    QComboBox *cboxDataClass;
    int c_row = 0;
    QComboBox *cboxArch;


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

    void on_btnQuickOpen3_clicked();

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
};
#endif // MAINWINDOW_H
