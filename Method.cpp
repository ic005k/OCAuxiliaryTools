#include "Method.h"
#include "mainwindow.h"
#include "plistserializer.h"
#include "ui_mainwindow.h"

extern MainWindow* mw_one;
Method* mymethod;

Method::Method(QWidget* parent)
    : QMainWindow(parent)
{
    mymethod = new Method;
}

QWidget* Method::getSubTab1(int m, int s)
{
    if (m == 0) {

        for (int i = 0; i < mw_one->ui->tabACPI->tabBar()->count(); i++) {
            if (i == s)
                return mw_one->ui->tabACPI->widget(i);
        }
    }

    if (m == 1) {

        for (int i = 0; i < mw_one->ui->tabBooter->tabBar()->count(); i++) {
            if (i == s)
                return mw_one->ui->tabBooter->widget(i);
        }
    }

    if (m == 2) {

        for (int i = 0; i < mw_one->ui->tabDP->tabBar()->count(); i++) {
            if (i == s)
                return mw_one->ui->tabDP->widget(i);
        }
    }

    if (m == 3) {

        for (int i = 0; i < mw_one->ui->tabKernel->tabBar()->count(); i++) {
            if (i == s)
                return mw_one->ui->tabKernel->widget(i);
        }
    }

    return NULL;
}

QWidget* Method::getSubTab2(int m, int s)
{
    if (m == 4) {

        for (int i = 0; i < mw_one->ui->tabMisc->tabBar()->count(); i++) {
            if (i == s)
                return mw_one->ui->tabMisc->widget(i);
        }
    }

    if (m == 5) {

        for (int i = 0; i < mw_one->ui->tabNVRAM->tabBar()->count(); i++) {
            if (i == s)
                return mw_one->ui->tabNVRAM->widget(i);
        }
    }

    if (m == 6) {

        for (int i = 0; i < mw_one->ui->tabPlatformInfo->tabBar()->count(); i++) {
            if (i == s)
                return mw_one->ui->tabPlatformInfo->widget(i);
        }
    }

    if (m == 7) {

        for (int i = 0; i < mw_one->ui->tabUEFI->tabBar()->count(); i++) {
            if (i == s)
                return mw_one->ui->tabUEFI->widget(i);
        }
    }

    return NULL;
}

QWidget* Method::getSubTabWidget(int m, int s)
{
    QWidget* w;
    w = getSubTab1(m, s);
    w = getSubTab2(m, s);

    return w;
}

void Method::goACPITable(QTableWidget* table)
{
    //ACPI
    if (table == mw_one->ui->table_acpi_add) {
        mw_one->ui->listMain->setCurrentRow(0);
        mw_one->ui->listSub->setCurrentRow(0);
    }

    if (table == mw_one->ui->table_acpi_del) {

        mw_one->ui->listMain->setCurrentRow(0);
        mw_one->ui->listSub->setCurrentRow(1);
    }

    if (table == mw_one->ui->table_acpi_patch) {

        mw_one->ui->listMain->setCurrentRow(0);
        mw_one->ui->listSub->setCurrentRow(2);
    }
}

void Method::goBooterTable(QTableWidget* table)
{
    //Booter
    if (table == mw_one->ui->table_booter) {

        mw_one->ui->listMain->setCurrentRow(1);
        mw_one->ui->listSub->setCurrentRow(0);
    }
    if (table == mw_one->ui->table_Booter_patch) {

        mw_one->ui->listMain->setCurrentRow(1);
        mw_one->ui->listSub->setCurrentRow(1);
    }
}

void Method::goDPTable(QTableWidget* table)
{
    //DP
    if (table == mw_one->ui->table_dp_add0) {

        mw_one->ui->listMain->setCurrentRow(2);
        mw_one->ui->listSub->setCurrentRow(0);
    }
    if (table == mw_one->ui->table_dp_add) {

        mw_one->ui->listMain->setCurrentRow(2);
        mw_one->ui->listSub->setCurrentRow(0);
    }
    if (table == mw_one->ui->table_dp_del0) {

        mw_one->ui->listMain->setCurrentRow(2);
        mw_one->ui->listSub->setCurrentRow(1);
    }
    if (table == mw_one->ui->table_dp_del) {

        mw_one->ui->listMain->setCurrentRow(2);
        mw_one->ui->listSub->setCurrentRow(1);
    }
}

void Method::goKernelTable(QTableWidget* table)
{
    //Kernel
    if (table == mw_one->ui->table_kernel_Force) {

        mw_one->ui->listMain->setCurrentRow(3);
        mw_one->ui->listSub->setCurrentRow(2);
    }

    if (table == mw_one->ui->table_kernel_add) {

        mw_one->ui->listMain->setCurrentRow(3);
        mw_one->ui->listSub->setCurrentRow(0);
    }

    if (table == mw_one->ui->table_kernel_block) {

        mw_one->ui->listMain->setCurrentRow(3);
        mw_one->ui->listSub->setCurrentRow(1);
    }

    if (table == mw_one->ui->table_kernel_patch) {

        mw_one->ui->listMain->setCurrentRow(3);
        mw_one->ui->listSub->setCurrentRow(3);
    }
}

void Method::goMiscTable(QTableWidget* table)
{
    //Misc
    if (table == mw_one->ui->tableBlessOverride) {

        mw_one->ui->listMain->setCurrentRow(4);
        mw_one->ui->listSub->setCurrentRow(3);
    }

    if (table == mw_one->ui->tableEntries) {

        mw_one->ui->listMain->setCurrentRow(4);
        mw_one->ui->listSub->setCurrentRow(4);
    }

    if (table == mw_one->ui->tableTools) {

        mw_one->ui->listMain->setCurrentRow(4);
        mw_one->ui->listSub->setCurrentRow(5);
    }
}

void Method::goNVRAMTable(QTableWidget* table)
{
    //NVRAM
    if (table == mw_one->ui->table_nv_add0) {

        mw_one->ui->listMain->setCurrentRow(5);
        mw_one->ui->listSub->setCurrentRow(0);
    }

    if (table == mw_one->ui->table_nv_add) {

        mw_one->ui->listMain->setCurrentRow(5);
        mw_one->ui->listSub->setCurrentRow(0);
    }

    if (table == mw_one->ui->table_nv_del0) {

        mw_one->ui->listMain->setCurrentRow(5);
        mw_one->ui->listSub->setCurrentRow(1);
    }

    if (table == mw_one->ui->table_nv_del) {

        mw_one->ui->listMain->setCurrentRow(5);
        mw_one->ui->listSub->setCurrentRow(1);
    }

    if (table == mw_one->ui->table_nv_ls0) {

        mw_one->ui->listMain->setCurrentRow(5);
        mw_one->ui->listSub->setCurrentRow(2);
    }

    if (table == mw_one->ui->table_nv_ls) {

        mw_one->ui->listMain->setCurrentRow(5);
        mw_one->ui->listSub->setCurrentRow(2);
    }
}

void Method::goTable(QTableWidget* table)
{
    goACPITable(table);

    goBooterTable(table);

    goDPTable(table);

    goKernelTable(table);

    goMiscTable(table);

    goNVRAMTable(table);

    //PI
    if (table == mw_one->ui->tableDevices) {

        mw_one->ui->listMain->setCurrentRow(6);
        mw_one->ui->listSub->setCurrentRow(2);
    }

    //UEFI
    if (table == mw_one->ui->table_uefi_drivers) {

        mw_one->ui->listMain->setCurrentRow(7);
        mw_one->ui->listSub->setCurrentRow(3);
    }

    if (table == mw_one->ui->table_uefi_ReservedMemory) {

        mw_one->ui->listMain->setCurrentRow(7);
        mw_one->ui->listSub->setCurrentRow(8);
    }
}

void Method::on_GenerateEFI()
{

    QDir dir;
    QString strDatabase;

    QFileInfo appInfo(qApp->applicationDirPath());
    QString pathSource = appInfo.filePath() + "/Database/";

    QString pathTarget = QDir::homePath() + "/Desktop/EFI/";

    mw_one->deleteDirfile(pathTarget);

    if (dir.mkpath(pathTarget)) { }

    //BOOT
    QString pathBoot = pathTarget + "BOOT/";
    if (dir.mkpath(pathBoot)) { }
    QFile::copy(pathSource + "EFI/BOOT/BOOTx64.efi", pathBoot + "BOOTx64.efi");

    //OC/ACPI
    QString pathOCACPI = pathTarget + "OC/ACPI/";
    if (dir.mkpath(pathOCACPI)) { }
    for (int i = 0; i < mw_one->ui->table_acpi_add->rowCount(); i++) {

        QString file = mw_one->ui->table_acpi_add->item(i, 0)->text();
        QFileInfo fi(pathSource + "EFI/OC/ACPI/" + file);
        if (fi.exists())
            QFile::copy(pathSource + "EFI/OC/ACPI/" + file, pathOCACPI + file);
        else
            strDatabase = strDatabase + "EFI/OC/ACPI/" + file + "\n";
    }

    //OC/Bootstrap（在新版OC中已弃用）
    //QString pathOCBootstrap = pathTarget + "OC/Bootstrap/";
    //if (dir.mkpath(pathOCBootstrap)) { }
    //QFile::copy(pathSource + "EFI/OC/Bootstrap/Bootstrap.efi", pathOCBootstrap + "Bootstrap.efi");

    //OC/Drivers
    QString pathOCDrivers = pathTarget + "OC/Drivers/";
    if (dir.mkpath(pathOCDrivers)) { }
    for (int i = 0; i < mw_one->ui->table_uefi_drivers->rowCount(); i++) {

        QString file = mw_one->ui->table_uefi_drivers->item(i, 0)->text();
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
    for (int i = 0; i < mw_one->ui->table_kernel_add->rowCount(); i++) {

        QString file = mw_one->ui->table_kernel_add->item(i, 0)->text();
        QString str0 = pathSource + "EFI/OC/Kexts/" + file;
        QDir kextDir(str0);

        if (!str0.contains("#")) {

            if (kextDir.exists())
                mw_one->copyDirectoryFiles(str0, pathOCKexts + file, true);
            else
                strDatabase = strDatabase + "EFI/OC/Kexts/" + file + "\n";
        }
    }

    //OC/Resources
    QString pathOCResources = pathTarget + "OC/Resources/";
    mw_one->copyDirectoryFiles(pathSource + "EFI/OC/Resources/", pathOCResources, true);

    //OC/Tools
    QString pathOCTools = pathTarget + "OC/Tools/";
    if (dir.mkpath(pathOCTools)) { }
    for (int i = 0; i < mw_one->ui->tableTools->rowCount(); i++) {

        QString file = mw_one->ui->tableTools->item(i, 0)->text();
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
    mw_one->SavePlist(pathTarget + "OC/Config.plist");

    QMessageBox box;
    if (strDatabase != "")
        box.setText(tr("Finished generating the EFI folder on the desktop.") + "\n" + tr("The following files do not exist in the database at the moment, please add them yourself:") + "\n" + strDatabase);
    else
        box.setText(tr("Finished generating the EFI folder on the desktop."));

    mw_one->setFocus();
    box.exec();
    mw_one->ui->cboxFind->setFocus();
}

void Method::on_btnExportMaster()
{
    QFileDialog fd;
    QString defname;
    int index = mw_one->ui->tabTotal->currentIndex();

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
        OpenCore["ACPI"] = mw_one->SaveACPI();

        break;

    case 1:
        OpenCore["Booter"] = mw_one->SaveBooter();
        break;

    case 2:
        OpenCore["DeviceProperties"] = mw_one->SaveDeviceProperties();
        break;

    case 3:
        OpenCore["Kernel"] = mw_one->SaveKernel();
        break;

    case 4:
        OpenCore["Misc"] = mw_one->SaveMisc();
        break;

    case 5:
        OpenCore["NVRAM"] = mw_one->SaveNVRAM();
        break;

    case 6:
        OpenCore["PlatformInfo"] = mw_one->SavePlatformInfo();
        break;

    case 7:
        OpenCore["UEFI"] = mw_one->SaveUEFI();
        break;
    }

    PListSerializer::toPList(OpenCore, FileName);
}
