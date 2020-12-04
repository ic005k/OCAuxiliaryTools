#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QCheckBox>
#include <QComboBox>
#include <QDateTime>
#include <QDebug>
#include <QDesktopServices>
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
#include <QUuid>

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

    RecentFiles* m_recentFiles;

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

    void write_ini(QString table_name, QTableWidget* mytable, int i);
    void read_ini(QString table_name, QTableWidget* mytable, int i);

    void init_key_class_value(QTableWidget* table, QTableWidget* subtable);
    void init_value(QVariantMap map_fun, QTableWidget* table,
        QTableWidget* subtable);
    void write_value_ini(QString tablename, QTableWidget* subtable, int i);
    void read_value_ini(QString table_name, QTableWidget* mytable, int i);

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

public slots:

protected:
    void dragEnterEvent(QDragEnterEvent* e);
    void dropEvent(QDropEvent* e);
    void closeEvent(QCloseEvent* event);

#ifndef QT_NO_CONTEXTMENU
    void contextMenuEvent(QContextMenuEvent* event); // override;
#endif // QT_NO_CONTEXTMENU

private slots:
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
    void ReservedMemoryTypeChange();

    void on_btnOpen_clicked();

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

    void on_btnGenerate_clicked();

    void on_btnSystemUUID_clicked();

    void on_table_kernel_Force_cellClicked(int row, int column);

    void on_table_kernel_Force_currentCellChanged(int currentRow,
        int currentColumn,
        int previousRow,
        int previousColumn);

    void on_btnKernelForce_Add_clicked();

    void on_btnKernelForce_Del_clicked();

    void on_btnMountEsp_clicked();

    void on_table_uefi_ReservedMemory_currentCellChanged(int currentRow,
        int currentColumn,
        int previousRow,
        int previousColumn);

    void on_btnHelp_clicked();

    void on_btnExportMaster_clicked();

    void on_btnImportMaster_clicked();

    void on_tabTotal_tabBarClicked(int index);

    void on_tabTotal_currentChanged(int index);

    void on_btnDevices_add_clicked();

    void on_btnDevices_del_clicked();

    void on_cboxUpdateSMBIOSMode_currentIndexChanged(const QString& arg1);

    void on_chk01_clicked();

    void on_chk02_clicked();

    void on_chk04_clicked();

    void on_chk08_clicked();

    void on_editExposeSensitiveData_textChanged(const QString& arg1);

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

    void on_editScanPolicy_textChanged(const QString& arg1);

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

    void on_editDisplayLevel_textChanged(const QString& arg1);

    void on_btnDLSetAll_clicked();

    void on_btnDLClear_clicked();

    void on_chkPA1_clicked();

    void on_chkPA2_clicked();

    void on_chkPA3_clicked();

    void on_chkPA4_clicked();

    void on_editPickerAttributes_textChanged(const QString& arg1);

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

    void on_chkFadtEnableReset_stateChanged(int arg1);

    void on_chkNormalizeHeaders_stateChanged(int arg1);

    void on_chkRebaseRegions_stateChanged(int arg1);

    void on_chkResetHwSig_stateChanged(int arg1);

    void on_chkResetLogoStatus_stateChanged(int arg1);

    void on_chkAvoidRuntimeDefrag_stateChanged(int arg1);

    void on_chkEnableWriteUnprotector_stateChanged(int arg1);

    void on_chkRebuildAppleMemoryMap_stateChanged(int arg1);

    void on_chkEnableSafeModeSlide_stateChanged(int arg1);

    void on_chkDevirtualiseMmio_stateChanged(int arg1);

    void on_chkForceExitBootServices_stateChanged(int arg1);

    void on_chkSetupVirtualMap_stateChanged(int arg1);

    void on_chkAllowRelocationBlock_stateChanged(int arg1);

    void on_chkDisableSingleUser_stateChanged(int arg1);

    void on_chkProtectMemoryRegions_stateChanged(int arg1);

    void on_chkSignalAppleOS_stateChanged(int arg1);

    void on_chkDisableVariableWrite_stateChanged(int arg1);

    void on_chkProtectSecureBoot_stateChanged(int arg1);

    void on_chkSyncRuntimePermissions_stateChanged(int arg1);

    void on_chkDiscardHibernateMap_stateChanged(int arg1);

    void on_chkProtectUefiServices_stateChanged(int arg1);

    void on_chkProvideCustomSlide_stateChanged(int arg1);

    void on_chkAppleCpuPmCfgLock_stateChanged(int arg1);

    void on_chkDisableIoMapper_stateChanged(int arg1);

    void on_chkLapicKernelPanic_stateChanged(int arg1);

    void on_chkExternalDiskIcons_stateChanged(int arg1);

    void on_chkExtendBTFeatureFlags_stateChanged(int arg1);

    void on_chkForceSecureBootScheme_stateChanged(int arg1);

    void on_chkPowerTimeoutKernelPanic_stateChanged(int arg1);

    void on_chkLegacyCommpage_stateChanged(int arg1);

    void on_chkAppleXcpmForceBoost_stateChanged(int arg1);

    void on_chkAppleXcpmExtraMsrs_stateChanged(int arg1);

    void on_chkIncreasePciBarSize_stateChanged(int arg1);

    void on_chkCustomSMBIOSGuid_stateChanged(int arg1);

    void on_chkXhciPortLimit_stateChanged(int arg1);

    void on_chkThirdPartyDrives_stateChanged(int arg1);

    void on_chkDisableLinkeditJettison_stateChanged(int arg1);

    void on_chkPanicNoKextDump_stateChanged(int arg1);

    void on_chkDisableRtcChecksum_stateChanged(int arg1);

    void on_chkAppleXcpmCfgLock_stateChanged(int arg1);

    void on_chkFuzzyMatch_stateChanged(int arg1);

    void on_chkHideAuxiliary_stateChanged(int arg1);

    void on_chkPickerAudioAssist_stateChanged(int arg1);

    void on_chkPollAppleHotKeys_stateChanged(int arg1);

    void on_chkShowPicker_stateChanged(int arg1);

    void on_chkAppleDebug_stateChanged(int arg1);

    void on_chkApplePanic_stateChanged(int arg1);

    void on_chkDisableWatchDog_stateChanged(int arg1);

    void on_chkSysReport_stateChanged(int arg1);

    void on_chkSerialInit_stateChanged(int arg1);

    void on_chkAllowNvramReset_stateChanged(int arg1);

    void on_chkAllowSetDefault_stateChanged(int arg1);

    void on_chkAuthRestart_stateChanged(int arg1);

    void on_chkBlacklistAppleUpdate_stateChanged(int arg1);

    void on_chkEnablePassword_stateChanged(int arg1);

    void on_chkLegacyEnable_stateChanged(int arg1);

    void on_chkLegacyOverwrite_stateChanged(int arg1);

    void on_chkWriteFlash_stateChanged(int arg1);

    void on_chkAutomatic_stateChanged(int arg1);

    void on_chkCustomMemory_stateChanged(int arg1);

    void on_chkUpdateDataHub_stateChanged(int arg1);

    void on_chkUpdateNVRAM_stateChanged(int arg1);

    void on_chkUpdateSMBIOS_stateChanged(int arg1);

    void on_chkAdviseWindows_stateChanged(int arg1);

    void on_chkSpoofVendor_stateChanged(int arg1);

    void on_chkEnableJumpstart_stateChanged(int arg1);

    void on_chkJumpstartHotPlug_stateChanged(int arg1);

    void on_chkHideVerbose_stateChanged(int arg1);

    void on_chkGlobalConnect_stateChanged(int arg1);

    void on_chkDeduplicateBootOrder_stateChanged(int arg1);

    void on_chkIgnoreInvalidFlexRatio_stateChanged(int arg1);

    void on_chkReleaseUsbOwnership_stateChanged(int arg1);

    void on_chkRequestBootVarRouting_stateChanged(int arg1);

    void on_chkUnblockFsConnect_stateChanged(int arg1);

    void on_chkAudioSupport_stateChanged(int arg1);

    void on_chkPlayChime_stateChanged(int arg1);

    void on_chkConnectDrivers_stateChanged(int arg1);

    void on_chkKeyFiltering_stateChanged(int arg1);

    void on_chkKeySwap_stateChanged(int arg1);

    void on_chkKeySupport_stateChanged(int arg1);

    void on_chkPointerSupport_stateChanged(int arg1);

    void on_chkClearScreenOnModeSwitch_stateChanged(int arg1);

    void on_chkDirectGopRendering_stateChanged(int arg1);

    void on_chkReconnectOnResChange_stateChanged(int arg1);

    void on_chkReplaceTabWithSpace_stateChanged(int arg1);

    void on_chkForceResolution_stateChanged(int arg1);

    void on_chkProvideConsoleGop_stateChanged(int arg1);

    void on_chkIgnoreTextInGraphics_stateChanged(int arg1);

    void on_chkUgaPassThrough_stateChanged(int arg1);

    void on_chkSanitiseClearScreen_stateChanged(int arg1);

    void on_chkAppleAudio_stateChanged(int arg1);

    void on_chkAppleBootPolicy_stateChanged(int arg1);

    void on_chkAppleDebugLog_stateChanged(int arg1);

    void on_chkAppleEvent_stateChanged(int arg1);

    void on_chkAppleImg4Verification_stateChanged(int arg1);

    void on_chkAppleSecureBoot_stateChanged(int arg1);

    void on_chkAppleRtcRam_stateChanged(int arg1);

    void on_chkAppleKeyMap_stateChanged(int arg1);

    void on_chkAppleImageConversion_stateChanged(int arg1);

    void on_chkAppleFramebufferInfo_stateChanged(int arg1);

    void on_chkAppleSmcIo_stateChanged(int arg1);

    void on_chkAppleUserInterfaceTheme_stateChanged(int arg1);

    void on_chkDataHub_stateChanged(int arg1);

    void on_chkOSInfo_stateChanged(int arg1);

    void on_chkHashServices_stateChanged(int arg1);

    void on_chkFirmwareVolume_stateChanged(int arg1);

    void on_chkDeviceProperties_stateChanged(int arg1);

    void on_chkUnicodeCollation_stateChanged(int arg1);

    void on_editProvideMaxSlide_textChanged(const QString& arg1);

    void on_editCpuid1Data_textChanged(const QString& arg1);

    void on_editCpuid1Mask_textChanged(const QString& arg1);

    void on_chkDummyPowerManagement_stateChanged(int arg1);

    void on_editMaxKernel_textChanged(const QString& arg1);

    void on_editMinKernel_textChanged(const QString& arg1);

    void on_cboxKernelArch_currentIndexChanged(const QString& arg1);

    void on_cboxKernelCache_currentIndexChanged(const QString& arg1);

    void on_editConsoleAttributes_textChanged(const QString& arg1);

    void on_editTakeoffDelay_textChanged(const QString& arg1);

    void on_editTimeout_textChanged(const QString& arg1);

    void on_cboxHibernateMode_currentIndexChanged(const QString& arg1);

    void on_cboxPickerMode_currentIndexChanged(const QString& arg1);

    void on_editDisplayDelay_textChanged(const QString& arg1);

    void on_editTarget_textChanged(const QString& arg1);

    void on_editHaltLevel_textChanged(const QString& arg1);

    void on_editApECID_textChanged(const QString& arg1);

    void on_cboxBootProtect_currentIndexChanged(const QString& arg1);

    void on_editPasswordHash_textChanged(const QString& arg1);

    void on_editPasswordSalt_textChanged(const QString& arg1);

    void on_cboxDmgLoading_currentIndexChanged(const QString& arg1);

    void on_cboxVault_currentIndexChanged(const QString& arg1);

    void on_cboxSecureBootModel_currentIndexChanged(const QString& arg1);

    void on_editSystemSerialNumber_textChanged(const QString& arg1);

    void on_editMLB_textChanged(const QString& arg1);

    void on_editSystemUUID_textChanged(const QString& arg1);

    void on_cboxSystemMemoryStatus_currentIndexChanged(const QString& arg1);

    void on_editProcessorTypeGeneric_textChanged(const QString& arg1);

    void on_editROM_textChanged(const QString& arg1);

    void on_editARTFrequency_textChanged(const QString& arg1);

    void on_editBoardProduct_textChanged(const QString& arg1);

    void on_editBoardRevision_textChanged(const QString& arg1);

    void on_editDevicePathsSupported_textChanged(const QString& arg1);

    void on_editFSBFrequency_textChanged(const QString& arg1);

    void on_editInitialTSC_textChanged(const QString& arg1);

    void on_editPlatformName_textChanged(const QString& arg1);

    void on_editSmcBranch_textChanged(const QString& arg1);

    void on_editSmcPlatform_textChanged(const QString& arg1);

    void on_editSmcRevision_textChanged(const QString& arg1);

    void on_editStartupPowerEvents_textChanged(const QString& arg1);

    void on_editSystemProductName_textChanged(const QString& arg1);

    void on_editSystemSerialNumber_data_textChanged(const QString& arg1);

    void on_editSystemUUID_data_textChanged(const QString& arg1);

    void on_editDataWidth_textChanged(const QString& arg1);

    void on_editTotalWidth_textChanged(const QString& arg1);

    void on_editErrorCorrection_textChanged(const QString& arg1);

    void on_editType_textChanged(const QString& arg1);

    void on_editFormFactor_textChanged(const QString& arg1);

    void on_editTypeDetail_textChanged(const QString& arg1);

    void on_editMaxCapacity_textChanged(const QString& arg1);

    void on_editBID_textChanged(const QString& arg1);

    void on_editFirmwareFeatures_textChanged(const QString& arg1);

    void on_editFirmwareFeaturesMask_textChanged(const QString& arg1);

    void on_editMLB_2_textChanged(const QString& arg1);

    void on_editROM_2_textChanged(const QString& arg1);

    void on_editSystemUUID_PNVRAM_textChanged(const QString& arg1);

    void on_editBIOSReleaseDate_textChanged(const QString& arg1);

    void on_editBIOSVendor_textChanged(const QString& arg1);

    void on_editBIOSVersion_textChanged(const QString& arg1);

    void on_editBoardAssetTag_textChanged(const QString& arg1);

    void on_editBoardLocationInChassis_textChanged(const QString& arg1);

    void on_editBoardManufacturer_textChanged(const QString& arg1);

    void on_editBoardProduct_2_textChanged(const QString& arg1);

    void on_editBoardSerialNumber_textChanged(const QString& arg1);

    void on_editBoardType_textChanged(const QString& arg1);

    void on_editBoardVersion_textEdited(const QString& arg1);

    void on_editChassisAssetTag_textEdited(const QString& arg1);

    void on_editChassisManufacturer_textEdited(const QString& arg1);

    void on_editChassisSerialNumber_textEdited(const QString& arg1);

    void on_editChassisType_textEdited(const QString& arg1);

    void on_editSystemVersion_textEdited(const QString& arg1);

    void on_editSystemUUID_2_textEdited(const QString& arg1);

    void on_editSystemSerialNumber_2_textEdited(const QString& arg1);

    void on_editSystemSKUNumber_textEdited(const QString& arg1);

    void on_editSystemProductName_2_textEdited(const QString& arg1);

    void on_editSystemManufacturer_textEdited(const QString& arg1);

    void on_editSystemFamily_textEdited(const QString& arg1);

    void on_editSmcVersion_textEdited(const QString& arg1);

    void on_editProcessorType_textEdited(const QString& arg1);

    void on_editPlatformFeature_textEdited(const QString& arg1);

    void on_editFirmwareFeaturesMask_2_textEdited(const QString& arg1);

    void on_editFirmwareFeatures_2_textEdited(const QString& arg1);

    void on_editChassisVersion_textEdited(const QString& arg1);

    void on_editExitBootServicesDelay_textEdited(const QString& arg1);

    void on_editTscSyncTimeout_textEdited(const QString& arg1);

    void on_editMinDate_textEdited(const QString& arg1);

    void on_editMinVersion_textEdited(const QString& arg1);

    void on_editAudioCodec_textEdited(const QString& arg1);

    void on_editAudioDevice_textEdited(const QString& arg1);

    void on_editAudioOut_textEdited(const QString& arg1);

    void on_editMinimumVolume_textEdited(const QString& arg1);

    void on_editVolumeAmplifier_textEdited(const QString& arg1);

    void on_editKeyForgetThreshold_textEdited(const QString& arg1);

    void on_editKeyMergeThreshold_textEdited(const QString& arg1);

    void on_cboxKeySupportMode_currentIndexChanged(const QString& arg1);

    void on_editPointerSupportMode_textEdited(const QString& arg1);

    void on_editTimerResolution_textEdited(const QString& arg1);

    void on_cboxConsoleMode_currentIndexChanged(const QString& arg1);

    void on_cboxResolution_currentIndexChanged(const QString& arg1);

    void on_cboxTextRenderer_currentIndexChanged(const QString& arg1);

    void on_table_dp_del_cellClicked(int row, int column);

    void on_tableBlessOverride_cellClicked(int row, int column);

    void on_table_nv_del_cellClicked(int row, int column);

    void on_table_nv_ls_cellClicked(int row, int column);

    void on_tableDevices_cellClicked(int row, int column);

    void on_table_uefi_drivers_cellClicked(int row, int column);

    void on_btnOcvalidate_clicked();

    void readResultCheckData();

private:
    Ui::MainWindow* ui;

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

    int pav1, pav2, pav3, pav4;
    QVector<QCheckBox*> chk_pa;
    QVector<int> v_pa;
    void PickerAttributes();
    bool pickerAttributes = false;

    QString SaveFileName;

    void init_menu();

    bool closeSave = false;

    void clear_temp_data();

    void on_btnParse_clicked();

    void on_btnSerialize_clicked();

    void on_btnQuickOpen1_clicked();

    void on_btnQuickOpen2_clicked();

    void on_btnTestWrite_clicked();
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
