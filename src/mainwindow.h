#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QCheckBox>
#include <QClipboard>
#include <QComboBox>
#include <QDateTime>
#include <QDebug>
#include <QDesktopServices>
#include <QDesktopWidget>
#include <QGuiApplication>
#include <QLatin1Char>
#include <QListWidgetItem>
#include <QMainWindow>
#include <QMimeData>
#include <QPainter>
#include <QProcess>
#include <QProxyStyle>
#include <QRegularExpression>
#include <QSaveFile>
#include <QScreen>
#include <QShortcut>
#include <QSpacerItem>
#include <QSplashScreen>
#include <QSplitter>
#include <QStandardItemModel>
#include <QStyledItemDelegate>
#include <QTableWidget>
#include <QTextBlock>
#include <QToolButton>
#include <QToolTip>
#include <QTranslator>
#include <QUndoStack>
#include <QUndoView>
#include <QUuid>

// 网络相关头文件
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
// JSON相关头文件
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "aboutdialog.h"
#include "autoupdatedialog.h"
#include "dlgMountESP.h"
#include "dlgOCValidate.h"
#include "dlgParameters.h"
#include "dlgPreset.h"
#include "dlgdatabase.h"
#include "dlgkernelpatch.h"
#include "dlgmisc.h"
#include "dlgnewkeyfield.h"
#include "dlgpreference.h"
#include "filesystemwatcher.h"
#include "plistparser.h"
#include "plistserializer.h"
#include "recentfiles.h"
#include "syncocdialog.h"
#include "tooltip.h"
#include "ui_aboutdialog.h"
#include "ui_autoupdatedialog.h"
#include "ui_dlgMountESP.h"
#include "ui_dlgOCValidate.h"
#include "ui_dlgParameters.h"
#include "ui_dlgPreset.h"
#include "ui_dlgdatabase.h"
#include "ui_dlgkernelpatch.h"
#include "ui_dlgnewkeyfield.h"
#include "ui_dlgpreference.h"
#include "ui_syncocdialog.h"

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

  dlgNewKeyField* myDlgNewKeyField;
  dlgPreference* myDlgPreference;
  dlgKernelPatch* myDlgKernelPatch;
  bool blOCValidateError = false;
  dlgMountESP* dlgMESP;
  QString pathSource;
  QString dataBaseDir, userDataBaseDir;
  QLabel* lblVer;
  QToolButton* btnBak;
  bool blReadLeftTable;
  bool blWriteLeftTable;
  QStringList listNVRAMAdd;
  QStringList listNVRAMDel;
  QStringList listNVRAMLs;
  QStringList listDPAdd;
  QStringList listDPDel;
  QString strConfigPath;
  void updateIconStatus();
  QTimer* timer;
  QList<QCheckBox*> listUICheckBox;
  QList<QLabel*> listUILabel;
  void lineEditSetText();
  QStringList strIconList;
  QStringList strIconListSel;
  QString strOrgMd5;
  QString strByModiMd5;
  QAction* cutAction;
  QAction* copyAction;
  QAction* pasteAction;
  QNetworkAccessManager* manager;

  QList<QVariant> NoteValues;
  QList<QString> NoteKeys;
  bool find = false;
  bool txtEditASCIITextChanged = false;
  bool txtEditHexTextChanged = false;
  QLabel* labelConversion;
  bool InitEdit = false;
  bool lineEditModifyed = false;
  QVector<QTabWidget*> mainTabList;
  void initScanPolicyValue();
  void initDisplayLevelValue();
  void initPickerAttributesValue();
  void initTargetValue();
  void initColorValue();
  void findCheckBox(QString findText);
  void findLabel(QString findText);
  void findLineEdit(QString findText);
  void findComboBox(QString findText);
  void findTableHeader(QString findText);
  void findTabText(QString findText);
  QVariantMap setEditValue(QVariantMap map, QWidget* tab);
  QVariantMap setCheckBoxValue(QVariantMap map, QWidget* tab);
  QVariantMap setComboBoxValue(QVariantMap map, QWidget* tab);
  void getCheckBoxValue(QVariantMap map, QWidget* tab);
  void getEditValue(QVariantMap map, QWidget* tab);
  void getComboBoxValue(QVariantMap map, QWidget* tab);
  void setPalette(QWidget* w, QColor backColor, QColor textColor);

  void setCheckBoxWidth(QCheckBox* cbox);
  QString getDatabaseVer();
  QIntValidator* IntValidator = new QIntValidator;
  QString getTableFieldDataType(QTableWidget* table);
  void setStatusBarText(QTableWidget* table);
  QString stringInt =
      "TableLengthBaseSkipCountLimitSkipTableLengthAddressSizeSpeed";
  QString stringData = "OemTableIdTableSignatureFindMaskReplaceReplaceMaskMask";

  void getValue(QVariantMap map, QWidget* tab);
  QVariantMap setValue(QVariantMap map, QWidget* tab);
  dlgOCValidate* dlgOCV;
  dlgParameters* dlgPar;
  AutoUpdateDialog* dlgAutoUpdate;
  SyncOCDialog* dlgSyncOC;
  dlgPreset* dlgPresetValues;
  dlgMisc* dlgMiscBootArgs;
  Tooltip* myToolTip;

  bool lineEditEnter = false;
  bool isGetEFI = false;
  bool mac = false;
  bool osx1012 = false;
  bool win = false;
  bool linuxOS = false;

  QString strTableHeaderToolTip;

  int getMainHeight();
  int getMainWidth();

  void clearAllTableSelection();

  QTableWidget* getLeftTable(QTableWidget* table);
  int getLetfTableCurrentRow(QTableWidget* table);
  void init_CopyPasteLine();
  void pasteLine(QTableWidget* w, QAction* pasteAction);
  void copyLine(QTableWidget* w, QAction* copyAction);
  void cutLine(QTableWidget* w, QAction* cutAction, QAction* copyAction);
  void tablePopMenu(QTableWidget* w, QAction* cutAction, QAction* copyAction,
                    QAction* pasteAction, QAction* showtipAction,
                    QMenu* popMenu);
  void loadRightTable(QTableWidget* t0, QTableWidget* t);
  void endPasteLine(QTableWidget* w, int row, QString colText0);
  void endDelLeftTable(QTableWidget* t0);

  QAction* clearTextsAction;

  static QObjectList getAllUIControls(QObject* parent);
  static QObjectList getAllCheckBox(QObjectList lstUIControls);
  static QObjectList getAllTableWidget(QObjectList lstUIControls);
  static QObjectList getAllLineEdit(QObjectList lstUIControls);
  static QObjectList getAllLabel(QObjectList lstUIControls);
  static QObjectList getAllComboBox(QObjectList lstUIControls);
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

  QWidget* currentMainTabWidget;

  void clearCheckBoxMarker();
  void clearLabelMarker();
  void clearComboBoxMarker();
  void clearLineEditMarker();
  void clearTableHeaderMarker();

  RecentFiles* m_recentFiles;
  QLineEdit* lineEdit;
  QTableWidget* myTable = new QTableWidget;
  void initLineEdit(QTableWidget* Table, int previousRow, int previousColumn,
                    int currentRow, int currentColumn);
  void removeWidget(QTableWidget* table);
  void removeAllLineEdit();
  bool writeINI = false;

  void ParserACPI(QVariantMap map);
  void ParserBooter(QVariantMap map);
  void ParserDP(QVariantMap map);
  void ParserKernel(QVariantMap map, QString subitem);
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

  void initui_ACPI();
  void initui_Booter();
  void initui_DP();
  void initui_Kernel();
  void initui_Misc();
  void initui_NVRAM();
  void initui_PlatformInfo();
  void initui_UEFI();
  void init_enabled_data(QTableWidget* table, int row, int column, QString str);
  void enabled_change(QTableWidget* table, int row, int column, int cc);
  void add_item(QTableWidget* table, int total_column);
  void del_item(QTableWidget* table);

  void write_ini(QTableWidget* table, QTableWidget* mytable, int i);
  void read_ini(QTableWidget* table, QTableWidget* mytable, int i);

  void init_key_class_value(QTableWidget* table, QTableWidget* subtable);

  void write_value_ini(QTableWidget* table, QTableWidget* subtable, int i);
  void read_value_ini(QTableWidget* table, QTableWidget* mytable, int i);

  QByteArray HexStringToByteArray(QString HexString);
  QByteArray HexStrToByte(QString value);
  QString ByteToHexStr(QByteArray ba);

  QString title;
  bool loading = false;
  void about();

  QComboBox* cboxDataClass;
  int c_row = 0;

  QComboBox* cboxArch;

  QComboBox* cboxReservedMemoryType;

  aboutDialog* aboutDlg;
  dlgDatabase* myDatabase;
  void addACPIItem(QStringList FileName);

  void addKexts(QStringList FileName);

  void addEFITools(QStringList FileName);

  void addEFIDrivers(QStringList FileName);

  int deleteDirfile(QString dirName);

  bool DeleteDirectory(const QString& path);

  bool copyDirectoryFiles(const QString& fromDir, const QString& toDir,
                          bool coverFileIfExist);

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

  int getTextWidth(QString str, QWidget* w);
  void init_TableStyle();
  void openDir(QString strDir);
  void currentCellChanged(QTableWidget* t, int previousRow, int previousColumn,
                          int currentRow, int currentColumn);
  void MoveItem(QTableWidget* t, bool up);
  void CheckChange(QTableWidget* tw, int arg1, QToolButton* btnDel);
  void CellEnter(int row, QTableWidget* tw);

  void ShowAutoUpdateDlg(bool Database);
  void setConversionWidgetVisible(bool v);
  void EnterPress();

  void init_value(QVariantMap map_fun, QTableWidget* table,
                  QTableWidget* subtable, int currentRow);
  void AddNvramAdd(QVariantMap map_add, int currentRow, bool blPreset);
  void checkFiles(QTableWidget* table);
  void sortForKexts();

  void readLeftTable(QTableWidget* t0, QTableWidget* t);
  void readLeftTableOnlyValue(QTableWidget* t0, QTableWidget* t);
  QString getMD5(QString targetFile);

  bool IsProcessExist(QString processName);
  void markColor(QTableWidget* table, QString path, int col);
  QString getKextVersion(QString kextFile);

  void changeOpenCore(bool blDEV);
  static QObjectList getAllToolButton(QObjectList lstUIControls);
  QWidget* getSubTabWidget(int m, int s);
  void init_setWindowModified();
  void setWM();
  void smart_UpdateKeyField();
  void set_AutoColWidth(QTableWidget* w, bool autoColWidth);
  void set_InitLineEdit(QTableWidget* t, int row, int column);

  void set_InitCheckBox(QTableWidget* t, int row, int column);
  void oc_Validate(bool show);
  void init_AutoColumnWidth();
  static QObjectList getAllFrame(QObjectList lstUIControls);
  void init_Table(int index);
  void init_FindResults();

  void init_Label(QLabel* w);
  void init_CheckBox(QCheckBox* w);
 public slots:
  void DisplayLevel();
  void ScanPolicy();
  void PickerAttributes();
  void ExposeSensitiveData();
  void on_actionFind_triggered();
  void on_btnNVRAMAdd_Add_clicked();
  void on_btnDPAdd_Add0_clicked();
  void on_btnACPIPatch_Add_clicked();
  void on_btnKernelPatchAdd_clicked();
  void on_btnNVRAMAdd_Add0_clicked();
  void on_btnNVRAMAdd_Del0_clicked();
  void on_btnNVRAMLS_Add0_clicked();
  void on_btnNVRAMDel_Add0_clicked();
  void cellEnteredSlot(int row, int column);
  void on_actionOnline_Download_Updates_triggered();
  void on_actionDatabase_triggered();
  void on_actionPreferences_triggered();
  void on_actionGenerateEFI_triggered();

 protected:
  void dragEnterEvent(QDragEnterEvent* e) override;
  void dropEvent(QDropEvent* e) override;
  void closeEvent(QCloseEvent* event) override;
  virtual void resizeEvent(QResizeEvent* event) override;
  void keyPressEvent(QKeyEvent* event) override;
  void keyReleaseEvent(QKeyEvent* event) override;
  void paintEvent(QPaintEvent* event) override;
  bool eventFilter(QObject* obj, QEvent* event) override;
  void mousePressEvent(QMouseEvent*) override;
  void mouseReleaseEvent(QMouseEvent*) override;
  void mouseMoveEvent(QMouseEvent*) override;

#ifndef QT_NO_CONTEXTMENU
  void contextMenuEvent(QContextMenuEvent* event) override;
#endif  // QT_NO_CONTEXTMENU

 protected slots:

 private slots:
  void Target();
  void readResultPassHash();
  void clearFindTexts();
  void copyText(QListWidget* listW);
  void on_line1();
  void on_line2();
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

  void on_btnKernelPatchDel_clicked();

  void on_btnACPIAdd_Del_clicked();

  void on_btnACPIDel_Add_clicked();

  void on_btnACPIDel_Del_clicked();

  void on_btnACPIPatch_Del_clicked();

  void on_btnBooter_Add_clicked();

  void on_btnBooter_Del_clicked();

  void on_btnDPDel_Add0_clicked();

  void on_btnDPDel_Del0_clicked();

  void on_btnDPDel_Add_clicked();

  void on_btnDPDel_Del_clicked();

  void on_btnACPIAdd_Add_clicked();

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

  void on_btnNVRAMAdd_Del_clicked();

  void on_btnNVRAMDel_Add_clicked();

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

  void on_btnSystemProductName_clicked();

  void on_btnSystemUUID_clicked();

  void on_table_kernel_Force_cellClicked(int row, int column);

  void on_table_kernel_Force_currentCellChanged(int currentRow,
                                                int currentColumn,
                                                int previousRow,
                                                int previousColumn);

  void on_btnKernelForce_Add_clicked();

  void on_btnKernelForce_Del_clicked();

  void on_table_uefi_ReservedMemory_currentCellChanged(int currentRow,
                                                       int currentColumn,
                                                       int previousRow,
                                                       int previousColumn);

  void on_btnHelp();

  void on_tabTotal_tabBarClicked(int index);

  void on_tabTotal_currentChanged(int index);

  void on_btnDevices_add_clicked();

  void on_btnDevices_del_clicked();

  void on_cboxUpdateSMBIOSMode_currentIndexChanged(const QString& arg1);

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

  void on_btnBooterPatchAdd_clicked();

  void on_btnBooterPatchDel_clicked();

  void on_table_Booter_patch_cellClicked(int row, int column);

  void on_table_Booter_patch_currentCellChanged(int currentRow,
                                                int currentColumn,
                                                int previousRow,
                                                int previousColumn);

  void on_btnCheckUpdate();

  void replyFinished(QNetworkReply* reply);

  void on_editIntTarget_textChanged(const QString& arg1);
  void on_editIntHaltLevel_textChanged(const QString& arg1);

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

  void lineEdit_textEdited(const QString& arg1);

  void on_table_nv_ls_currentCellChanged(int currentRow, int currentColumn,
                                         int previousRow, int previousColumn);

  void on_table_acpi_add_currentCellChanged(int currentRow, int currentColumn,
                                            int previousRow,
                                            int previousColumn);

  void on_table_nv_add0_currentCellChanged(int currentRow, int currentColumn,
                                           int previousRow, int previousColumn);

  void on_table_acpi_del_currentCellChanged(int currentRow, int currentColumn,
                                            int previousRow,
                                            int previousColumn);

  void on_table_acpi_patch_currentCellChanged(int currentRow, int currentColumn,
                                              int previousRow,
                                              int previousColumn);

  void on_table_booter_currentCellChanged(int currentRow, int currentColumn,
                                          int previousRow, int previousColumn);

  void on_tableBlessOverride_currentCellChanged(int currentRow,
                                                int currentColumn,
                                                int previousRow,
                                                int previousColumn);

  void on_tableEntries_currentCellChanged(int currentRow, int currentColumn,
                                          int previousRow, int previousColumn);

  void on_tableTools_currentCellChanged(int currentRow, int currentColumn,
                                        int previousRow, int previousColumn);

  void on_tableDevices_currentCellChanged(int currentRow, int currentColumn,
                                          int previousRow, int previousColumn);

  void on_table_uefi_drivers_currentCellChanged(int currentRow,
                                                int currentColumn,
                                                int previousRow,
                                                int previousColumn);

  void on_table_dp_add0_currentCellChanged(int currentRow, int currentColumn,
                                           int previousRow, int previousColumn);

  void on_table_dp_del0_currentCellChanged(int currentRow, int currentColumn,
                                           int previousRow, int previousColumn);

  void on_table_dp_del_currentCellChanged(int currentRow, int currentColumn,
                                          int previousRow, int previousColumn);

  void on_table_nv_del0_currentCellChanged(int currentRow, int currentColumn,
                                           int previousRow, int previousColumn);

  void on_table_nv_ls0_currentCellChanged(int currentRow, int currentColumn,
                                          int previousRow, int previousColumn);

  void on_table_nv_del_currentCellChanged(int currentRow, int currentColumn,
                                          int previousRow, int previousColumn);

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

  void on_actionNewWindow_triggered();

  void on_actionGo_to_the_previous_triggered();

  void on_actionGo_to_the_next_triggered();

  void on_mycboxFind_currentTextChanged(const QString& arg1);

  void on_listFind_currentRowChanged(int currentRow);

  void on_listFind_itemClicked(QListWidgetItem* item);

  void on_actionBug_Report_triggered();

  void on_actionQuit_triggered();

  void on_actionUpgrade_OC_triggered();

  void on_actionDiscussion_Forum_triggered();

  void on_mycboxTextColor_currentIndexChanged(int index);

  void on_mycboxBackColor_currentIndexChanged(int index);

  void on_editIntConsoleAttributes_textChanged(const QString& arg1);

  void on_btnGetPassHash_clicked();

  void on_toolButton_clicked();

  void on_calendarWidget_selectionChanged();

  void on_btnROM_clicked();

  void on_myeditPassInput_textChanged(const QString& arg1);

  void on_myeditPassInput_returnPressed();

  void on_actionOcvalidate_triggered();

  void on_actionMountEsp_triggered();

  void on_btnExportMaster_triggered();

  void on_editDatPasswordHash_textChanged(const QString& arg1);

  void on_btnImportMaster_triggered();

  void on_editDatPasswordSalt_textChanged(const QString& arg1);

  void on_btnOpenACPIDir_clicked();

  void on_btnOpenDriversDir_clicked();

  void on_btnOpenToolsDir_clicked();

  void on_btnOpenKextDir_clicked();

  void on_btnDisplayLevel_clicked();

  void on_btnScanPolicy_clicked();

  void on_btnPickerAttributes_clicked();

  void on_btnExposeSensitiveData_clicked();

  void on_actionPlist_editor_triggered();

  void on_actionDSDT_SSDT_editor_triggered();

  void on_actionDifferences_triggered();

  void setWM_RightTable();

  void on_btnUp_clicked();

  void on_btnDown_clicked();

  void on_btnUp_UEFI_Drivers_clicked();

  void on_btnDown_UEFI_Drivers_clicked();

  void on_actionLatest_Release_triggered();

  void on_txtEditHex_textChanged(const QString& arg1);

  void on_txtEditASCII_textChanged(const QString& arg1);

  void on_listSub_currentRowChanged(int currentRow);

  void on_listMain_currentRowChanged(int currentRow);

  void on_btnUpdateHex_clicked();

  void on_actionOpen_Directory_triggered();

  void on_actionOpen_database_directory_triggered();

  void on_btnDPAddPreset_clicked();

  void on_btnACPIPatch_clicked();

  void on_btnPresetKernelPatch_clicked();

  void on_btnPresetNVAdd_clicked();

  void on_btnPresetNVDelete_clicked();

  void on_btnPresetNVLegacy_clicked();

  void on_actionAutoChkUpdate_triggered();

  void on_actionEdit_Presets_triggered();

  void on_btnNo_clicked();

  void on_btnYes_clicked();

  void on_btnAddbootArgs_clicked();

  void updateStatus();

  void on_table_nv_add0_itemClicked(QTableWidgetItem* item);

  void on_btnKextPreset_clicked();

  void on_actionOCAuxiliaryToolsDoc_triggered();

  void on_actionBackup_EFI_triggered();

  void on_comboBoxACPI_currentTextChanged(const QString& arg1);

  void on_comboBoxBooter_currentIndexChanged(const QString& arg1);

  void on_comboBoxKernel_currentIndexChanged(const QString& arg1);

  void on_comboBoxUEFI_currentIndexChanged(const QString& arg1);

  void on_actionOpenCore_DEV_triggered();

  void on_actionDocumentation_triggered();

  void on_btnHide_clicked();

  void on_actionMove_Up_triggered();

  void on_actionMove_Down_triggered();

  void on_actionAdd_triggered();

  void on_actionDelete_triggered();

  void on_mycboxEmulate_currentTextChanged(const QString& arg1);

  void on_btnImport_clicked();

  void on_btnExport_clicked();

  void on_actionDEBUG_triggered();

  void on_actionInitDatabaseLinux_triggered();

  void on_actionNew_Key_Field_triggered();

  void on_btnSystemSerialNumber_clicked();

  void on_editSystemSerialNumber_textChanged(const QString& arg1);

  void on_editSystemUUID_textChanged(const QString& arg1);

  void on_editMLB_textChanged(const QString& arg1);

  void on_editDatROM_textChanged(const QString& arg1);

  void on_actionOpenCoreChineseDoc_triggered();

  void dataClassChange_uefi_drivers();

  void on_btnUEFIUnload_Add_clicked();

  void on_btnUEFIUnload_Del_clicked();

  void on_table_uefi_Unload_currentCellChanged(int currentRow,
                                               int currentColumn,
                                               int previousRow,
                                               int previousColumn);

  void on_table_uefi_Unload_cellDoubleClicked(int row, int column);

 private:
  bool isUnload = false;
  bool isDrag;
  QPoint m_position;
  bool LoadRightTable = false;
  bool AddCboxFindItem = false;
  void acpi_cellDoubleClicked();
  void booter_cellDoubleClicked();
  void dp_cellDoubleClicked();
  void kernel_cellDoubleClicked();
  void misc_cellDoubleClicked();
  void nvram_cellDoubleClicked();
  void pi_cellDoubleClicked();
  void uefi_cellDoubleClicked();
  void init_listMainSub();
  void init_CopyCheckbox();
  void init_CopyLabel();
  void init_LineEditDataCheck();
  void init_FileMenu();
  void init_EditMenu();
  void init_HelpMenu();
  void init_UndoRedo();
  void goResultsCheckbox(QString objName);
  void goResultsTable(QString objName, int index);
  void goResultsLabel(QString objName);
  void goResultsLineEdit(QString objName);
  void goResultsComboBox(QString objName);
  bool autoCheckUpdate = false;

  QString orgComboBoxStyle;
  QString orgLineEditStyle;
  QString orgLabelStyle;
  QString orgCheckBoxStyle;
  void initRecentFilesForToolBar();

  QMenu* reFileMenu;

  void setTableEditTriggers();

  int parse_UpdateJSON(QString str);

  QStringList backColor;
  QStringList textColor;

  QString CurrentDateTime;

  void reg_win();

  void mount_esp();

  void runAdmin(QString file, QString arg);

  QString getSystemProductName(QString arg1);

  QProcess* gs;
  QProcess* di;
  QProcess* chkdata;
  QProcess* chkdataPassHash;
  QVector<QString> IniFile;

  QVector<int> textColorInt;
  QVector<int> backColorInt;

  void method(QVector<unsigned int> nums, unsigned int sum);
  void method(QVector<unsigned int> nums, unsigned int sum,
              QVector<unsigned int> list, int index);

  QVector<QCheckBox*> chk;

  QVector<unsigned int> v;

  QVector<int> value;

  void init_MainUI();

  void init_hardware_info();

  bool closeSave = false;

  void clear_temp_data();

  QUndoStack* undoStack = nullptr;
  QUndoView* undoView = nullptr;
  QAction* undoAction = nullptr;
  QAction* redoAction = nullptr;

  void setListMainIcon();
  bool editExclusion(QLineEdit* w, QString name);
  QString getSubTabStr(int tabIndex);
  void initExposeSensitiveDataValue();

  void setPopMenuEnabled(QString qfile, QTableWidget* w, QAction* cutAction,
                         QAction* pasteAction, QAction* copyAction);
  void setPopMenuEnabledForDP_NVRAM(QTableWidget* w, QAction* pasteAction);
  void goResultsTableHeader(QString objName);
  QVector<QTableWidget*> tableList0;
  QVector<QTableWidget*> tableList;
  QString getReReCount(QTableWidget* w, QString text);
  QString getUrl(QVariantList list);
  void init_InitialValue();
  void setUIMargin();
  bool One = false;
  void openFileAfter();
  bool FindTextChange = false;
  void showDlgPar(bool b1, bool b2, bool b3, bool b4);

  void init_ToolButtonStyle();

  void init_ToolBarIcon();
  QToolButton* btn0;
  QToolButton* btnClear;
  void checkSystemAudioVolume();
  QObjectList getAllGridLayout(QObjectList lstUIControls);
  void init_Widgets();
  void load_LastFile();
  void init_ToolBar();
  void init_SearchUI();
};

#endif  // MAINWINDOW_H
