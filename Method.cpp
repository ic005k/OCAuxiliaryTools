#include "Method.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

extern MainWindow* mw_one;
Method* mymethod;

Method::Method(QWidget* parent)
    : QMainWindow(parent)
{
    mymethod = new Method;
}

QWidget* Method::getSubTabWidget(int m, int s)
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
