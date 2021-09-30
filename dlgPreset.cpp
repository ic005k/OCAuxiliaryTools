#include "dlgPreset.h"

#include "mainwindow.h"
#include "ui_dlgPreset.h"
#include "ui_mainwindow.h"
extern MainWindow* mw_one;
dlgPreset::dlgPreset(QWidget* parent) : QDialog(parent), ui(new Ui::dlgPreset) {
  ui->setupUi(this);

  QFileInfo appInfo(qApp->applicationDirPath());
  strPresetPath = appInfo.filePath() + "/Database/preset/";
}

dlgPreset::~dlgPreset() { delete ui; }

void dlgPreset::loadDPAdd() {
  ui->listDPAdd->clear();
  QString strPlistFile = strPresetPath + "DPAdd.plist";
  QFile file(strPlistFile);

  map.clear();
  map = PListParser::parsePList(&file).toMap();
  map = map["DeviceProperties"].toMap();
  if (map.isEmpty()) return;

  // Add
  map_add.clear();
  map_add = map["Add"].toMap();
  ui->listDPAdd->clear();
  for (int i = 0; i < map_add.count(); i++) {
    ui->listDPAdd->addItem(map_add.keys().at(i));
  }

  if (ui->listDPAdd->count() > 0) ui->listDPAdd->setCurrentRow(0);

  file.close();
}

void dlgPreset::loadACPIPatch() {
  ui->listACPIPatch->clear();
  QString strPlistFile = strPresetPath + "ACPIPatch.plist";
  QFile file(strPlistFile);

  map.clear();
  map = PListParser::parsePList(&file).toMap();
  map = map["ACPI"].toMap();
  if (map.isEmpty()) return;

  //分析Patch
  map_patch.clear();
  map_patch = map["Patch"].toList();
  for (int i = 0; i < map_patch.count(); i++) {
    QVariantMap map3 = map_patch.at(i).toMap();
    QString strItem = map3["Comment"].toString();
    ui->listACPIPatch->addItem(strItem);
  }

  if (ui->listACPIPatch->count() > 0) ui->listACPIPatch->setCurrentRow(0);

  file.close();
}

void dlgPreset::loadKernelPatch() {
  ui->listKernelPatch->clear();
  QString strPlistFile = strPresetPath + "KernelPatch.plist";
  QFile file(strPlistFile);

  map.clear();
  map = PListParser::parsePList(&file).toMap();
  map = map["Kernel"].toMap();
  if (map.isEmpty()) return;

  //分析Patch
  map_patch.clear();
  map_patch = map["Patch"].toList();
  for (int i = 0; i < map_patch.count(); i++) {
    QVariantMap map3 = map_patch.at(i).toMap();
    QString strItem = map3["Comment"].toString();
    ui->listKernelPatch->addItem(strItem);
  }

  if (ui->listKernelPatch->count() > 0) ui->listKernelPatch->setCurrentRow(0);

  file.close();
}

void dlgPreset::on_btnAdd_clicked() {
  // DPAdd
  if (ui->listDPAdd->isVisible()) {
    int row = ui->listDPAdd->currentRow();
    QString strCurrentItem = ui->listDPAdd->item(row)->text();
    QString strItem;
    QStringList strItemList;
    strItemList = strCurrentItem.split("|");
    if (strItemList.count() > 1) strItem = strItemList.at(0);

    bool re = false;

    for (int i = 0; i < mw_one->ui->table_dp_add0->rowCount(); i++) {
      if (mw_one->ui->table_dp_add0->item(i, 0)->text().trimmed() ==
          strItem.trimmed()) {
        re = true;
        mw_one->ui->table_dp_add0->setCurrentCell(i, 0);
      }
    }

    if (!re) {
      mw_one->on_btnDPAdd_Add0_clicked();
      int table_row = mw_one->ui->table_dp_add0->currentRow();
      QTableWidgetItem* newItem1 = new QTableWidgetItem(strItem);
      mw_one->ui->table_dp_add0->setItem(table_row, 0, newItem1);

      //加载子条目
      map_sub.clear();
      map_sub = map_add[map_add.keys().at(row)].toMap();
      mw_one->ui->table_dp_add->setRowCount(
          map_sub.keys().count());  //子键的个数

      for (int j = 0; j < map_sub.keys().count(); j++) {
        // QTableWidgetItem *newItem1;
        newItem1 = new QTableWidgetItem(map_sub.keys().at(j));  //键
        mw_one->ui->table_dp_add->setItem(j, 0, newItem1);

        QString dtype = map_sub[map_sub.keys().at(j)].typeName();
        QString ztype;
        if (dtype == "QByteArray") ztype = "Data";
        if (dtype == "QString") ztype = "String";
        if (dtype == "qlonglong") ztype = "Number";

        newItem1 = new QTableWidgetItem(ztype);  //数据类型
        newItem1->setTextAlignment(Qt::AlignCenter);
        mw_one->ui->table_dp_add->setItem(j, 1, newItem1);

        QString type_name = map_sub[map_sub.keys().at(j)].typeName();
        if (type_name == "QByteArray") {
          QByteArray tohex = map_sub[map_sub.keys().at(j)].toByteArray();
          QString va = tohex.toHex().toUpper();
          newItem1 = new QTableWidgetItem(va);

        } else
          newItem1 =
              new QTableWidgetItem(map_sub[map_sub.keys().at(j)].toString());
        mw_one->ui->table_dp_add->setItem(j, 2, newItem1);
      }

      //保存子条目里面的数据，以便以后加载
      mw_one->write_ini(mw_one->ui->table_dp_add0, mw_one->ui->table_dp_add,
                        table_row);
    }
  }

  // ACPIPatch
  if (ui->listACPIPatch->isVisible()) {
    bool re = false;
    for (int i = 0; i < mw_one->ui->table_acpi_patch->rowCount(); i++) {
      QVariantMap map3 = map_patch.at(ui->listACPIPatch->currentRow()).toMap();
      QString strFind = mw_one->ByteToHexStr(map3["Find"].toByteArray());
      QString strReplace = mw_one->ByteToHexStr(map3["Replace"].toByteArray());

      if (strFind == mw_one->ui->table_acpi_patch->item(i, 3)->text() &&
          strReplace == mw_one->ui->table_acpi_patch->item(i, 4)->text()) {
        re = true;
        mw_one->ui->table_acpi_patch->setCurrentCell(
            i, mw_one->ui->table_acpi_patch->currentColumn());
      }
    }
    if (!re) {
      mw_one->on_btnACPIPatch_Add_clicked();

      mw_one->AddACPIPatch(map_patch, ui->listACPIPatch->currentRow(),
                           mw_one->ui->table_acpi_patch->rowCount() - 1);
    }
  }

  // KernalPatch
  if (ui->listKernelPatch->isVisible()) {
    bool re = false;
    for (int i = 0; i < mw_one->ui->table_kernel_patch->rowCount(); i++) {
      QVariantMap map3 =
          map_patch.at(ui->listKernelPatch->currentRow()).toMap();
      QString strBase = map3["Base"].toString();
      QString strFind = mw_one->ByteToHexStr(map3["Find"].toByteArray());
      QString strReplace = mw_one->ByteToHexStr(map3["Replace"].toByteArray());

      if (strBase == mw_one->ui->table_kernel_patch->item(i, 1)->text() &&
          strFind == mw_one->ui->table_kernel_patch->item(i, 3)->text() &&
          strReplace == mw_one->ui->table_kernel_patch->item(i, 4)->text()) {
        re = true;
        mw_one->ui->table_kernel_patch->setCurrentCell(
            i, mw_one->ui->table_kernel_patch->currentColumn());
      }
    }
    if (!re) {
      mw_one->on_btnKernelPatchAdd_clicked();

      mw_one->AddKernelPatch(map_patch, ui->listKernelPatch->currentRow(),
                             mw_one->ui->table_kernel_patch->rowCount() - 1);
    }
  }
}
