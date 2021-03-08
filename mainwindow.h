#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QCheckBox>
#include <QClipboard>
#include <QComboBox>
#include <QDateTime>
#include <QDebug>
#include <QDesktopServices>
#include <QLatin1Char>
#include <QListWidgetItem>
#include <QMainWindow>
#include <QMimeData>
#include <QPainter>
#include <QProcess>
#include <QProxyStyle>
#include <QSaveFile>
#include <QSplitter>
#include <QTableWidget>
#include <QTextBlock>
#include <QTextCodec>
#include <QToolTip>
#include <QTranslator>
#include <QUndoStack>
#include <QUndoView>
#include <QUuid>

//网络相关头文件
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
//JSON相关头文件
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "aboutdialog.h"
#include "dlgdatabase.h"
#include "recentfiles.h"

#ifdef Q_OS_WIN32
#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#endif

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    void initCopyPasteLine();
    void loadReghtTable(QTableWidget* t0, QTableWidget* t);
    void endPasteLine(QTableWidget* w, int row, QString colText0);
    void endDelLeftTable(QTableWidget* t0);

    QAction* clearTextsAction;

    QObjectList getAllUIControls(QObject* parent);
    QObjectList getAllCheckBox(QObjectList lstUIControls);
    QObjectList getAllTableWidget(QObjectList lstUIControls);
    QObjectList getAllLineEdit(QObjectList lstUIControls);
    QObjectList getAllLabel(QObjectList lstUIControls);
    QObjectList getAllComboBox(QObjectList lstUIControls);
    QObjectList listOfCheckBox;
    QObjectList listOfTableWidget;
    QObjectList listOfLabel;
    QObjectList listOfLineEdit;
    QObjectList listOfComboBox;
    void findTable(QTableWidget* t, QString text);
    int findCount = 0;
    QObjectList listOfCheckBoxResults;
    QObjectList listOfTableWidgetResults;
    QObjectList listOfLabelResults;
    QObjectList listOfLineEditResults;
    QObjectList listOfComboBoxResults;
    QStringList listNameResults;
    int indexOfResults = 0;
    void goResults(int index);
    QWidget* currentTabWidget;
    QWidget* getSubTabWidget(int m, int s);
    QWidget* currentMainTabWidget;
    int red = 0;
    void clearCheckBoxMarker();
    void clearLabelMarker();
    void clearComboBoxMarker();
    void clearLineEditMarker();

    RecentFiles* m_recentFiles;
    QLineEdit* lineEdit;
    QTableWidget* myTable = new QTableWidget;
    void initLineEdit(QTableWidget* Table, int previousRow, int previousColumn, int currentRow, int currentColumn);
    void removeWidget(QTableWidget* table);
    void removeAllLineEdit();
    bool writeINI = false;
    void goTable(QTableWidget* table);

    void ParserACPI(QVariantMap map);
    void ParserBooter(QVariantMap map);
    void ParserDP(QVariantMap map);
    void ParserKernel(QVariantMap map);
    void ParserMisc(QVariantMap map);
    void ParserNvram(QVariantMap map);
    void ParserPlatformInfo(QVariantMap map);
    void ParserUEFI(QVariantMap map);

    void openFile(QString PlistFileName);

    void SavePlist(QString FileName);
    bool getBool(QTableWidget* table, int row, int column);
    bool getChkBool(QCheckBox* chkbox);
    QVariantMap SaveACPI();
    QVariantMap SaveBooter();
    QVariantMap SaveDeviceProperties();
    QVariantMap SaveKernel();
    QVariantMap SaveMisc();
    QVariantMap SaveNVRAM();
    QVariantMap SavePlatformInfo();
    QVariantMap SaveUEFI();

    void initui_acpi();
    void initui_booter();
    void initui_dp();
    void initui_kernel();
    void initui_misc();
    void initui_nvram();
    void initui_PlatformInfo();
    void initui_UEFI();
    void init_enabled_data(QTableWidget* table, int row, int column, QString str);
    void enabled_change(QTableWidget* table, int row, int column, int cc);
    void add_item(QTableWidget* table, int total_column);
    void del_item(QTableWidget* table);

    void write_ini(QTableWidget* table, QTableWidget* mytable, int i);
    void read_ini(QTableWidget* table, QTableWidget* mytable, int i);

    void init_key_class_value(QTableWidget* table, QTableWidget* subtable);
    void init_value(QVariantMap map_fun, QTableWidget* table, QTableWidget* subtable);
    void write_value_ini(QTableWidget* table, QTableWidget* subtable, int i);
    void read_value_ini(QTableWidget* table, QTableWidget* mytable, int i);

    QByteArray HexStringToByteArray(QString HexString);
    QByteArray HexStrToByte(QString value);
    QString ByteToHexStr(QByteArray ba);

    QString title;
    bool loading = false;

    void test(bool test);

    void about();

    void loadLocal();
    bool zh_cn = false;

    QComboBox* cboxDataClass;
    int c_row = 0;

    QComboBox* cboxArch;

    QComboBox* cboxReservedMemoryType;

    aboutDialog* aboutDlg;

    void addACPIItem(QStringList FileName);

    void addKexts(QStringList FileName);

    void addEFITools(QStringList FileName);

    void addEFIDrivers(QStringList FileName);

    int deleteDirfile(QString dirName);

    bool DeleteDirectory(const QString& path);

    bool copyDirectoryFiles(const QString& fromDir, const QString& toDir, bool coverFileIfExist);

    bool copyFileToPath(QString sourceDir, QString toDir, bool coverFileIfExist);

    QString getWMIC(const QString& cmd);
    QString getCpuName();
    QString getCpuId();
    QString getCpuCoresNum();
    QString getCpuLogicalProcessorsNum();
    QString getDiskNum();
    QString getBaseBordNum();
    QString getBiosNum();
    QString getMainboardName();
    QString getMainboardUUID();
    QString getMainboardVendor();

    QString getMacInfo(const QString& cmd);

public slots:
    void on_GenerateEFI();

protected:
    void dragEnterEvent(QDragEnterEvent* e) override;
    void dropEvent(QDropEvent* e) override;
    void closeEvent(QCloseEvent* event) override;
    virtual void resizeEvent(QResizeEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override; //键盘按下事件
    void keyReleaseEvent(QKeyEvent* event) override; //键盘松开事件

#ifndef QT_NO_CONTEXTMENU
    void contextMenuEvent(QContextMenuEvent* event) override;
#endif // QT_NO_CONTEXTMENU

private slots:
    void clearFindTexts();
    void copyText(QListWidget* listW);
    void OpenDir_clicked();
    void on_ShareConfig();
    void on_Database();
    void on_line1();
    void on_line2();
    void on_line3();
    void on_line4();
    void on_line5();

    void on_line20();
    void on_line21();

    void recentOpen(QString filename);

    void on_nv1();
    void on_nv2();
    void on_nv3();
    void on_nv4();
    void on_nv5();
    void on_nv6();
    void on_nv7();
    void on_nv8();
    void on_nv9();
    void on_nv10();
    void on_nv11();
    void on_nv12();

    void on_nv01();
    void on_nv02();
    void on_nv03();
    void on_nv04();

    void show_menu(QPoint);
    void show_menu0(QPoint);

    void dataClassChange_dp();
    void dataClassChange_nv();
    void readResult();
    void readResultSystemInfo();
    void readResultDiskInfo();
    void arch_addChange();
    void arch_ForceChange();
    void arch_blockChange();
    void arch_patchChange();
    void arch_Booter_patchChange();
    void ReservedMemoryTypeChange();

    void on_btnOpen();

    void on_table_dp_add0_cellClicked(int row, int column);

    void on_table_dp_add_itemSelectionChanged();

    void on_table_dp_add_itemChanged(QTableWidgetItem* item);

    void on_table_nv_add0_cellClicked(int row, int column);

    void on_table_nv_add_itemChanged(QTableWidgetItem* item);

    void on_table_nv_del0_cellClicked(int row, int column);

    void on_table_nv_ls0_cellClicked(int row, int column);

    void on_table_nv_del_itemChanged(QTableWidgetItem* item);

    void on_table_nv_ls_itemChanged(QTableWidgetItem* item);

    void on_table_dp_del0_cellClicked(int row, int column);

    void on_table_dp_del_itemChanged(QTableWidgetItem* item);

    void on_btnSave();

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

    void on_btnSaveAs();

    void on_btnKernelAdd_Del_clicked();

    void on_table_dp_add_cellClicked(int row, int column);

    void on_table_dp_add_currentCellChanged(int currentRow, int currentColumn,
        int previousRow, int previousColumn);

    void on_table_nv_add_cellClicked(int row, int column);

    void on_table_nv_add_currentCellChanged(int currentRow, int currentColumn,
        int previousRow, int previousColumn);

    void on_table_kernel_add_currentCellChanged(int currentRow, int currentColumn,
        int previousRow,
        int previousColumn);

    void on_table_kernel_block_currentCellChanged(int currentRow,
        int currentColumn,
        int previousRow,
        int previousColumn);

    void on_table_kernel_patch_currentCellChanged(int currentRow,
        int currentColumn,
        int previousRow,
        int previousColumn);

    void on_cboxSystemProductName_currentIndexChanged(const QString& arg1);
    void on_editExposeSensitiveData_textChanged(const QString& arg1);
    void on_editScanPolicy_textChanged(const QString& arg1);
    void on_editDisplayLevel_textChanged(const QString& arg1);
    void on_editPickerAttributes_textChanged(const QString& arg1);

    void on_btnGenerate_clicked();

    void on_btnSystemUUID_clicked();

    void on_table_kernel_Force_cellClicked(int row, int column);

    void on_table_kernel_Force_currentCellChanged(int currentRow,
        int currentColumn,
        int previousRow,
        int previousColumn);

    void on_btnKernelForce_Add_clicked();

    void on_btnKernelForce_Del_clicked();

    void on_btnMountEsp();

    void on_table_uefi_ReservedMemory_currentCellChanged(int currentRow,
        int currentColumn,
        int previousRow,
        int previousColumn);

    void on_btnHelp();

    void on_btnExportMaster();

    void on_btnImportMaster();

    void on_tabTotal_tabBarClicked(int index);

    void on_tabTotal_currentChanged(int index);

    void on_btnDevices_add_clicked();

    void on_btnDevices_del_clicked();

    void on_cboxUpdateSMBIOSMode_currentIndexChanged(const QString& arg1);

    void on_chk01_clicked();

    void on_chk02_clicked();

    void on_chk04_clicked();

    void on_chk08_clicked();

    void on_chk1_clicked();

    void on_chk2_clicked();

    void on_chk3_clicked();

    void on_chk4_clicked();

    void on_chk5_clicked();

    void on_chk6_clicked();

    void on_chk7_clicked();

    void on_chk8_clicked();

    void on_chk9_clicked();

    void on_chk10_clicked();

    void on_chk11_clicked();

    void on_chk12_clicked();

    void on_chk13_clicked();

    void on_chk14_clicked();

    void on_chk15_clicked();

    void on_chk16_clicked();

    void on_chkD1_clicked();

    void on_chkD2_clicked();

    void on_chkD3_clicked();

    void on_chkD4_clicked();

    void on_chkD5_clicked();

    void on_chkD6_clicked();

    void on_chkD7_clicked();

    void on_chkD8_clicked();

    void on_chkD9_clicked();

    void on_chkD10_clicked();

    void on_chkD11_clicked();

    void on_chkD12_clicked();

    void on_chkD13_clicked();

    void on_chkD14_clicked();

    void on_chkD15_clicked();

    void on_chkD16_clicked();

    void on_chkD17_clicked();

    void on_chkD18_clicked();

    void on_chkD19_clicked();

    void on_btnDLSetAll_clicked();

    void on_btnDLClear_clicked();

    void on_chkPA1_clicked();

    void on_chkPA2_clicked();

    void on_chkPA3_clicked();

    void on_chkPA4_clicked();

    void on_table_acpi_add_itemChanged(QTableWidgetItem* item);

    void on_table_acpi_add_currentItemChanged(QTableWidgetItem* current,
        QTableWidgetItem* previous);

    void on_table_acpi_del_itemChanged(QTableWidgetItem* item);

    void on_table_acpi_patch_itemChanged(QTableWidgetItem* item);

    void on_table_booter_itemChanged(QTableWidgetItem* item);

    void on_table_dp_add0_itemChanged(QTableWidgetItem* item);

    void on_table_dp_del0_itemChanged(QTableWidgetItem* item);

    void on_table_kernel_add_itemChanged(QTableWidgetItem* item);

    void on_table_kernel_block_itemChanged(QTableWidgetItem* item);

    void on_table_kernel_Force_itemChanged(QTableWidgetItem* item);

    void on_table_kernel_patch_itemChanged(QTableWidgetItem* item);

    void on_tableBlessOverride_itemChanged(QTableWidgetItem* item);

    void on_tableEntries_itemChanged(QTableWidgetItem* item);

    void on_tableTools_itemChanged(QTableWidgetItem* item);

    void on_table_nv_add0_itemChanged(QTableWidgetItem* item);

    void on_table_nv_del0_itemChanged(QTableWidgetItem* item);

    void on_table_nv_ls0_itemChanged(QTableWidgetItem* item);

    void on_tableDevices_itemChanged(QTableWidgetItem* item);

    void on_table_uefi_drivers_itemChanged(QTableWidgetItem* item);

    void on_table_uefi_ReservedMemory_itemChanged(QTableWidgetItem* item);

    void on_cboxKernelArch_currentIndexChanged(const QString& arg1);

    void on_cboxKernelCache_currentIndexChanged(const QString& arg1);

    void on_table_dp_del_cellClicked(int row, int column);

    void on_tableBlessOverride_cellClicked(int row, int column);

    void on_table_nv_del_cellClicked(int row, int column);

    void on_table_nv_ls_cellClicked(int row, int column);

    void on_tableDevices_cellClicked(int row, int column);

    void on_table_uefi_drivers_cellClicked(int row, int column);

    void on_btnOcvalidate();

    void readResultCheckData();

    void on_btnBooterPatchAdd_clicked();

    void on_btnBooterPatchDel_clicked();

    void on_table_Booter_patch_cellClicked(int row, int column);

    void on_table_Booter_patch_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

    void on_btnCheckUpdate();

    void replyFinished(QNetworkReply* reply);

    void on_chkPA5_clicked();
    void on_editTarget_textChanged(const QString& arg1);
    void on_editHaltLevel_textChanged(const QString& arg1);

    void on_tabACPI_currentChanged(int index);

    void on_listMain_itemSelectionChanged();

    void on_listSub_itemSelectionChanged();

    void on_table_dp_add0_itemSelectionChanged();

    void on_table_dp_del0_itemSelectionChanged();

    void on_table_nv_add0_itemSelectionChanged();

    void on_table_nv_del0_itemSelectionChanged();

    void on_table_nv_ls0_itemSelectionChanged();

    void on_table_acpi_add_itemEntered(QTableWidgetItem* item);

    void on_table_acpi_add_cellEntered(int row, int column);

    void lineEdit_textChanged(const QString& arg1);

    void setEditText();

    void lineEdit_textEdited(const QString& arg1);

    void on_table_nv_ls_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

    void on_table_acpi_add_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

    void on_table_nv_add0_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

    void on_table_acpi_del_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

    void on_table_acpi_patch_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

    void on_table_booter_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

    void on_tableBlessOverride_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

    void on_tableEntries_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

    void on_tableTools_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

    void on_tableDevices_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

    void on_table_uefi_drivers_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

    void on_table_dp_add0_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

    void on_table_dp_del0_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

    void on_table_dp_del_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

    void on_tabBooter_currentChanged(int index);

    void on_tabDP_currentChanged(int index);

    void on_tabKernel_currentChanged(int index);

    void on_tabMisc_currentChanged(int index);

    void on_tabNVRAM_currentChanged(int index);

    void on_tabPlatformInfo_currentChanged(int index);

    void on_tabUEFI_currentChanged(int index);

    void on_table_nv_del0_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

    void on_table_nv_ls0_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

    void on_table_nv_del_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

    void on_table_dp_add0_cellDoubleClicked(int row, int column);

    void on_table_dp_add_cellDoubleClicked(int row, int column);

    void on_table_dp_del0_cellDoubleClicked(int row, int column);

    void on_table_acpi_add_cellDoubleClicked(int row, int column);

    void on_table_acpi_del_cellDoubleClicked(int row, int column);

    void on_table_acpi_patch_cellDoubleClicked(int row, int column);

    void on_table_booter_cellDoubleClicked(int row, int column);

    void on_table_Booter_patch_cellDoubleClicked(int row, int column);

    void on_table_kernel_add_cellDoubleClicked(int row, int column);

    void on_table_kernel_block_cellDoubleClicked(int row, int column);

    void on_table_kernel_Force_cellDoubleClicked(int row, int column);

    void on_table_kernel_patch_cellDoubleClicked(int row, int column);

    void on_tableBlessOverride_cellDoubleClicked(int row, int column);

    void on_tableEntries_cellDoubleClicked(int row, int column);

    void on_tableTools_cellDoubleClicked(int row, int column);

    void on_table_nv_add0_cellDoubleClicked(int row, int column);

    void on_table_nv_add_cellDoubleClicked(int row, int column);

    void on_table_nv_del0_cellDoubleClicked(int row, int column);

    void on_table_nv_del_cellDoubleClicked(int row, int column);

    void on_table_nv_ls0_cellDoubleClicked(int row, int column);

    void on_table_nv_ls_cellDoubleClicked(int row, int column);

    void on_tableDevices_cellDoubleClicked(int row, int column);

    void on_table_uefi_drivers_cellDoubleClicked(int row, int column);

    void on_table_uefi_ReservedMemory_cellDoubleClicked(int row, int column);

    void on_table_dp_del_cellDoubleClicked(int row, int column);

    void on_editTargetHex_textChanged(const QString& arg1);

    void on_actionNewWindow_triggered();

    void on_pushButton_clicked();

    void on_actionFind_triggered();

    void on_actionGo_to_the_previous_triggered();

    void on_actionGo_to_the_next_triggered();

    void on_cboxFind_currentIndexChanged(int index);

    void on_cboxFind_currentTextChanged(const QString& arg1);

    void on_listFind_currentRowChanged(int currentRow);

    void on_cboxFind_currentIndexChanged(const QString& arg1);

    void on_listFind_itemClicked(QListWidgetItem* item);

    void on_table_acpi_add_cellPressed(int row, int column);

    void setWM();

private:
    Ui::MainWindow* ui;

    void initRecentFilesForToolBar();

    dlgDatabase* myDatabase;

    QAction* pTrailingAction;

    QMenu* reFileMenu;

    void setTableEdit();

    QNetworkAccessManager* manager;
    int parse_UpdateJSON(QString str);
    bool mac = false;
    bool win = false;
    bool linuxOS = false;

    QString CurVerison = "V1.0";

    QString CurrentDateTime;

    void reg_win();

    void mount_esp();

    void mount_esp_mac(QString strEfiDisk);

    void runAdmin(QString file, QString arg);

    QString getSystemProductName(QString arg1);

    QProcess* gs;

    QProcess* si;

    QProcess* di;

    QProcess* chkdata;

    QVector<QString> IniFile;

    void init_tr_str();

    int ExposeSensitiveData();

    QString strArch;
    QString strBundlePath;
    QString strComment;
    QString strEnabled;
    QString strExecutablePath;
    QString strMaxKernel;
    QString strMinKernel;
    QString strPlistPath;

    int v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16;
    void method(QVector<int> nums, int sum);
    void method(QVector<int> nums, int sum, QVector<int> list, int index);
    void ScanPolicy();
    bool scanPolicy = false;
    QVector<QCheckBox*> chk;
    QVector<int> v;

    unsigned int vd1, vd2, vd3, vd4, vd5, vd6, vd7, vd8, vd9, vd10, vd11, vd12,
        vd13, vd14, vd15, vd16, vd17, vd18, vd19;
    void DisplayLevel();

    void methodDisplayLevel(QVector<unsigned int> nums, unsigned int sum);
    void methodDisplayLevel(QVector<unsigned int> nums, unsigned int sum,
        QVector<unsigned int> list, int index);
    QVector<QCheckBox*> chkDisplayLevel;
    QVector<unsigned int> vDisplayLevel;

    bool click = false;

    int pav1, pav2, pav3, pav4, pav5;
    QVector<QCheckBox*> chk_pa;
    QVector<int> v_pa;
    void PickerAttributes();
    bool pickerAttributes = false;

    void init_menu();

    void init_setWindowModified();

    void init_hardware_info();

    bool closeSave = false;

    bool osx1012 = false;

    void clear_temp_data();

    void on_btnParse_clicked();

    void on_btnSerialize_clicked();

    void on_btnQuickOpen1_clicked();

    void on_btnQuickOpen2_clicked();

    void on_btnTestWrite_clicked();

    QUndoStack* undoStack = nullptr;
    QUndoView* undoView = nullptr;
    QAction* undoAction = nullptr;
    QAction* redoAction = nullptr;
};

class CustomTabStyle1 : public QProxyStyle {
public:
    QSize sizeFromContents(ContentsType type, const QStyleOption* option,
        const QSize& size, const QWidget* widget) const
    {
        QSize tabsize = QProxyStyle::sizeFromContents(type, option, size, widget);
        if (type == QStyle::CT_TabBarTab) {
            tabsize.transpose();
            tabsize.rwidth() = 105;
            tabsize.rheight() = 75;
        }

        return tabsize;
    }

    void drawControl(ControlElement element, const QStyleOption* option,
        QPainter* painter, const QWidget* widget) const
    {
        if (element == CE_TabBarTabLabel) {
            if (const QStyleOptionTab* tab = qstyleoption_cast<const QStyleOptionTab*>(option)) {
                QRect allRect = tab->rect;
                painter->setFont(QFont("微软雅黑", 18, QFont::Bold));
                if (tab->state & QStyle::State_Selected) {
                    painter->save();
                    painter->setPen(0x3399FF);
                    painter->setBrush(QBrush(0x3399FF));
                    painter->drawRect(allRect.adjusted(2, 2, -2, -2));
                    painter->restore();
                }

                QTextOption option;
                option.setAlignment(Qt::AlignCenter);
                if (tab->state & QStyle::State_Selected) {
                    painter->setPen(0xf8fcff);
                } else {
                    painter->setPen(0x5d5d5d);
                }

                painter->drawText(allRect, tab->text, option);
                return;
            }
        }
        if (element == CE_TabBarTab) {
            QProxyStyle::drawControl(element, option, painter, widget);
        }
    }
};

class CustomTabStyle2 : public QProxyStyle {
public:
    QSize sizeFromContents(ContentsType type, const QStyleOption* option,
        const QSize& size, const QWidget* widget) const
    {
        QSize s = QProxyStyle::sizeFromContents(type, option, size, widget);
        if (type == QStyle::CT_TabBarTab) {
            s.transpose();
            s.rwidth() = 110;
            s.rheight() = 75;
        }
        return s;
    }

    void drawControl(ControlElement element, const QStyleOption* option,
        QPainter* painter, const QWidget* widget) const
    {
        if (element == CE_TabBarTabLabel) {
            if (const QStyleOptionTab* tab = qstyleoption_cast<const QStyleOptionTab*>(option)) {
                // QRect allRect = tab->rect;

                QTextOption option;
                option.setAlignment(Qt::AlignCenter);

                QStyleOptionTab opt(*tab);
                opt.shape = QTabBar::RoundedNorth;

                // QIcon icon(":/acpi.png");
                // opt.icon = icon;

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
                painter->setFont(QFont("", 15, QFont::Bold));
#endif

                QProxyStyle::drawControl(element, &opt, painter, widget);

                return;
            }
        }

        QProxyStyle::drawControl(element, option, painter, widget);
    }
};

class CustomTabStyle3 : public QProxyStyle {
public:
    QSize sizeFromContents(ContentsType type, const QStyleOption* option,
        const QSize& size, const QWidget* widget) const
    {
        QSize tabsize = QProxyStyle::sizeFromContents(type, option, size, widget);
        if (type == QStyle::CT_TabBarTab) {
            tabsize.transpose();
            tabsize.rwidth() = 105;
            tabsize.rheight() = 75;
        }
        return tabsize;
    }

    void drawControl(ControlElement element, const QStyleOption* option,
        QPainter* painter, const QWidget* widget) const
    {
        if (element == CE_TabBarTabLabel) {
            if (const QStyleOptionTab* tab = qstyleoption_cast<const QStyleOptionTab*>(option)) {
                QRect allRect = tab->rect;
                if (tab->state & QStyle::State_Selected) {
                    painter->save();
                    painter->setPen(0x89cfff);
                    painter->setBrush(QBrush(0x89cfff));
                    painter->drawRect(allRect.adjusted(0, 0, -0, -0));
                    painter->restore();
                }

                QTextOption option;
                option.setAlignment(Qt::AlignCenter);

                if (tab->state & QStyle::State_Selected) {
                    painter->setPen(0xf8fcff);
                } else {
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
                painter->drawText(allRect, tab->text, option);
                return;
            }
        }
        if (element == CE_TabBarTab) {
            QProxyStyle::drawControl(element, option, painter, widget);
        }
    }
};

class CustomTabStyle4 : public QProxyStyle {
public:
    QSize sizeFromContents(ContentsType type, const QStyleOption* option,
        const QSize& size, const QWidget* widget) const
    {
        QSize s = QProxyStyle::sizeFromContents(type, option, size, widget);
        if (type == QStyle::CT_TabBarTab) {
            s.transpose();
            s.rwidth() = 105;
            s.rheight() = 75;
        }
        return s;
    }
    void drawControl(ControlElement element, const QStyleOption* option,
        QPainter* painter, const QWidget* widget) const
    {
        if (element == CE_TabBarTabLabel) {
            if (const QStyleOptionTab* tab = qstyleoption_cast<const QStyleOptionTab*>(option)) {
                QRect allRect = tab->rect;
                allRect.setWidth(allRect.width() + 0);
                allRect.setHeight(allRect.height() + 0);

                if (tab->state & QStyle::State_Selected) {

                    painter->save();
                    painter->setBrush(QBrush(0x004ea1));

                    painter->drawRect(allRect.adjusted(0, 0, 0, 0));

                    // painter->drawRoundedRect(tab->rect, 5, 5);
                    painter->restore();
                }

                else if (tab->state & QStyle::State_MouseOver) {
                    painter->save();
                    // painter->setBrush(QBrush(0x004ea1));
                    painter->setBrush(QBrush(0x7B68EE));

                    painter->drawRect(allRect.adjusted(0, 0, 0, 0));
                    // painter->drawRoundedRect(allRect, 5, 5);

                    painter->restore();
                } else {
                    painter->save();
                    painter->setBrush(QBrush(0x78aadc));

                    painter->drawRect(allRect.adjusted(0, 0, 0, 0));
                    // painter->drawRoundedRect(allRect, 5, 5);

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

class CustomTabStyle5 : public QProxyStyle {
public:
    QSize sizeFromContents(ContentsType type, const QStyleOption* option,
        const QSize& size, const QWidget* widget) const
    {
        QSize s = QProxyStyle::sizeFromContents(type, option, size, widget);
        if (type == QStyle::CT_TabBarTab) {
            s.transpose();

#ifdef Q_OS_WIN32
            s.rwidth() = 105;
            s.rheight() = 75;
#endif

#ifdef Q_OS_LINUX
            s.rwidth() = 105;
            s.rheight() = 75;
#endif

#ifdef Q_OS_MAC
            s.rwidth() = 105;
            s.rheight() = 75;
#endif
        }
        return s;
    }
    void drawControl(ControlElement element, const QStyleOption* option,
        QPainter* painter, const QWidget* widget) const
    {
        if (element == CE_TabBarTabLabel) {
            if (const QStyleOptionTab* tab = qstyleoption_cast<const QStyleOptionTab*>(option)) {
                QRect allRect = tab->rect;
                allRect.setWidth(allRect.width() - 2);
                allRect.setHeight(allRect.height() - 2);

                if (tab->state & QStyle::State_Selected) {

                    painter->save();
                    painter->setBrush(QBrush(0x004ea1));

                    //painter->drawRect(allRect.adjusted(0, 0, 0, 0));

                    painter->drawRoundedRect(tab->rect, 5, 5);
                    painter->restore();
                }

                else if (tab->state & QStyle::State_MouseOver) {
                    painter->save();
                    // painter->setBrush(QBrush(0x004ea1));
                    painter->setBrush(QBrush(0x7B68EE));
                    painter->drawRoundedRect(allRect, 8, 8);
                    painter->restore();
                } else {
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
