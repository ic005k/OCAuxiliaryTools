#include "Method.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

extern MainWindow* mw_one;
Method* method;

Method::Method(QWidget* parent)
    : QMainWindow(parent)
{
    method = new Method;
}

void Method::goTable(QTableWidget* table)
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

    //Booter
    if (table == mw_one->ui->table_booter) {

        mw_one->ui->listMain->setCurrentRow(1);
        mw_one->ui->listSub->setCurrentRow(0);
    }
    if (table == mw_one->ui->table_Booter_patch) {

        mw_one->ui->listMain->setCurrentRow(1);
        mw_one->ui->listSub->setCurrentRow(1);
    }

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
