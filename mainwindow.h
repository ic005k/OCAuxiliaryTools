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
#include <QRegExp>
#include <QSaveFile>
#include <QSplitter>
#include <QStyledItemDelegate>
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
#include "dlgMountESP.h"
#include "dlgOCValidate.h"
#include "dlgdatabase.h"
#include "recentfiles.h"
#include "tooltip.h"
#include "ui_dlgMountESP.h"

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
    Ui::MainWindow* ui;

    void setPalette(QWidget* w, QColor backColor, QColor textColor);
    bool Initialization = false;
    void setCheckBoxWidth(QCheckBox* cbox);
    void mount_esp_mac(QString strEfiDisk);
    QString getDatabaseVer();
    QIntValidator* IntValidator = new QIntValidator;
    QString getTableFieldDataType(QTableWidget* table);
    void setStatusBarText(QTableWidget* table);
    QString stringInt = "TableLengthBaseSkipCountLimitSkipTableLengthAddressSizeSpeed";
    QString stringData = "OemTableIdTableSignatureFindMaskReplaceReplaceMaskMask";

    void getValue(QVariantMap map, QWidget* tab);
    QVariantMap setValue(QVariantMap map, QWidget* tab);
    dlgOCValidate* dlgOCV;

    bool lineEditEnter = false;
    bool RefreshAllDatabase = false;

    bool mac = false;
    bool win = false;
    bool linuxOS = false;

    QString strTableHeaderToolTip;

    Tooltip* myToolTip = new Tooltip;
    int getMainHeight();
    int getMainWidth();

    void clearAllTableSelection();

    QTableWidget* getLeftTable(QTableWidget* table);
    int getLetfTableCurrentRow(QTableWidget* table);
    void init_CopyPasteLine();
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
    QObjectList listOfTableWidgetHeaderResults;
    QBrush brushTableHeaderBackground;
    QBrush brushTableHeaderForeground;
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
    void clearTableHeaderMarker();

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
    void on_actionFind_triggered();

protected:
    void dragEnterEvent(QDragEnterEvent* e) override;
    void dropEvent(QDropEvent* e) override;
    void closeEvent(QCloseEvent* event) override;
    virtual void resizeEvent(QResizeEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override; //键盘按下事件
    void keyReleaseEvent(QKeyEvent* event) override; //键盘松开事件
    void paintEvent(QPaintEvent* event) override;
    bool eventFilter(QObject* obj, QEvent* event) override;

#ifndef QT_NO_CONTEXTMENU
    void contextMenuEvent(QContextMenuEvent* event) override;
#endif // QT_NO_CONTEXTMENU

private slots:
    void readResultPassHash();
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
    void on_editIntExposeSensitiveData_textChanged(const QString& arg1);
    void on_editIntScanPolicy_textChanged(const QString& arg1);
    void on_editIntDisplayLevel_textChanged(const QString& arg1);
    void on_editIntPickerAttributes_textChanged(const QString& arg1);

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

    void on_chkPA6_clicked();

    void on_table_dp_add0_itemChanged(QTableWidgetItem* item);

    void on_table_dp_del0_itemChanged(QTableWidgetItem* item);

    void on_table_nv_add0_itemChanged(QTableWidgetItem* item);

    void on_table_nv_del0_itemChanged(QTableWidgetItem* item);

    void on_table_nv_ls0_itemChanged(QTableWidgetItem* item);

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
    void on_editIntTarget_textChanged(const QString& arg1);
    void on_editIntHaltLevel_textChanged(const QString& arg1);

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

    void on_actionGo_to_the_previous_triggered();

    void on_actionGo_to_the_next_triggered();

    void on_cboxFind_currentIndexChanged(int index);

    void on_cboxFind_currentTextChanged(const QString& arg1);

    void on_listFind_currentRowChanged(int currentRow);

    void on_cboxFind_currentIndexChanged(const QString& arg1);

    void on_listFind_itemClicked(QListWidgetItem* item);

    void setWM();

    void on_actionBug_Report_triggered();

    void on_actionQuit_triggered();

    void on_actionUpgrade_OC_triggered();

    void on_actionDiscussion_Forum_triggered();

    void on_cboxTextColor_currentIndexChanged(int index);

    void on_cboxBackColor_currentIndexChanged(int index);

    void on_editIntConsoleAttributes_textChanged(const QString& arg1);

    void on_chkT1_clicked();

    void on_chkT2_clicked();

    void on_chkT3_clicked();

    void on_chkT4_clicked();

    void on_chkT5_clicked();

    void on_chkT6_clicked();

    void on_chkT7_clicked();

    void on_btnGetPassHash_clicked();

    void on_toolButton_clicked();

    void on_calendarWidget_selectionChanged();

    void on_btnROM_clicked();

    void on_editPassInput_textChanged(const QString& arg1);

    void on_editPassInput_returnPressed();

private:
    void init_listMainSub();
    void CopyCheckbox();
    void CopyLabel();
    void LineEditDataCheck();
    bool autoCheckUpdate = false;
    bool OpenFileValidate = false;

    QString orgComboBoxStyle;
    QString orgLineEditStyle;
    QString orgLabelStyle;
    QString orgCheckBoxStyle;
    void initRecentFilesForToolBar();

    dlgDatabase* myDatabase;

    QAction* pTrailingAction;

    QMenu* reFileMenu;

    void setTableEdit();

    QNetworkAccessManager* manager;
    int parse_UpdateJSON(QString str);

    QString CurVerison = "V1.0";
    QString ocVer;

    QStringList backColor;
    QStringList textColor;

    QString CurrentDateTime;

    void reg_win();

    void mount_esp();

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

    QVector<int> textColorInt;
    QVector<int> backColorInt;
    void Target();

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

    int pav1, pav2, pav3, pav4, pav5, pav6;
    QVector<int> value;
    QVector<QCheckBox*> chk_pa;
    QVector<int> v_pa;
    void PickerAttributes();
    bool pickerAttributes = false;

    void init_Menu();

    void init_setWindowModified();

    void init_hardware_info();

    bool closeSave = false;

    bool osx1012 = false;

    void clear_temp_data();

    QUndoStack* undoStack = nullptr;
    QUndoView* undoView = nullptr;
    QAction* undoAction = nullptr;
    QAction* redoAction = nullptr;

    void setComboBoxStyle(QComboBox* w);
};

#endif // MAINWINDOW_H
