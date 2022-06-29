#include "dlgPreset.h"

#include "Method.h"
#include "mainwindow.h"
#include "ui_dlgPreset.h"
#include "ui_mainwindow.h"
extern MainWindow* mw_one;
extern Method* mymethod;
extern QString strAppExePath;
dlgPreset::dlgPreset(QWidget* parent) : QDialog(parent), ui(new Ui::dlgPreset) {
  ui->setupUi(this);

  QFileInfo appInfo(qApp->applicationDirPath());
  strPresetFile = appInfo.filePath() + "/Database/preset/PreSet.plist";
}

dlgPreset::~dlgPreset() { delete ui; }

void dlgPreset::loadPreset(QString strMain, QString strSub, QString strComment,
                           QListWidget* list) {
  list->clear();
  map.clear();
  map_patch.clear();
  map_add.clear();

  QFile file(strPresetFile);

  map = PListParser::parsePList(&file).toMap();
  map = map[strMain].toMap();
  if (map.isEmpty()) return;

  //分析Patch
  if (strComment.length() > 0) {
    map_patch = map[strSub].toList();
    for (int i = 0; i < map_patch.count(); i++) {
      QVariantMap map3 = map_patch.at(i).toMap();
      QString strItem = map3[strComment].toString();
      list->addItem(strItem);
    }
  } else {
    // DP->Add

    map_add = map[strSub].toMap();
    for (int i = 0; i < map_add.count(); i++) {
      list->addItem(map_add.keys().at(i));
    }
  }

  if (list->count() > 0) list->setCurrentRow(0);

  file.close();
}

void dlgPreset::on_btnAdd_clicked() {
  if (ui->listPreset->currentRow() < 0) return;
  // DPAdd
  if (blDPAdd) {
    int row = ui->listPreset->currentRow();
    QString strCurrentItem = ui->listPreset->item(row)->text();
    QString strItem;
    QStringList strItemList;
    strItemList = strCurrentItem.split("|");
    if (strItemList.count() > 1)
      strItem = strItemList.at(0);
    else
      strItem = strCurrentItem;

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

      mymethod->writeLeftTable(mw_one->ui->table_dp_add0,
                               mw_one->ui->table_dp_add);
    }
  }

  // ACPIPatch
  if (blACPIPatch) {
    bool re = false;
    for (int i = 0; i < mw_one->ui->table_acpi_patch->rowCount(); i++) {
      QVariantMap map3 = map_patch.at(ui->listPreset->currentRow()).toMap();
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
      QVariantList mapList;
      mapList.append(map_patch.at(ui->listPreset->currentRow()));
      Method::set_TableData(mw_one->ui->table_acpi_patch, mapList);
    }
  }

  // KernalPatch
  if (blKernelPatch) {
    bool re = false;
    for (int i = 0; i < mw_one->ui->table_kernel_patch->rowCount(); i++) {
      QVariantMap map3 = map_patch.at(ui->listPreset->currentRow()).toMap();
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
    }
  }

  // NVRAM Add
  if (blNVAdd) {
    bool re = false;
    for (int i = 0; i < mw_one->ui->table_nv_add0->rowCount(); i++) {
      QString str0 = mw_one->ui->table_nv_add0->item(i, 0)->text();
      QString str1 = ui->listPreset->item(ui->listPreset->currentRow())->text();
      if (str0 == str1) {
        re = true;
        mw_one->ui->table_nv_add0->setCurrentCell(i, 0);
      }
    }

    if (!re) {
      mw_one->on_btnNVRAMAdd_Add0_clicked();
      mw_one->AddNvramAdd(map_add, ui->listPreset->currentRow(), true);
      mymethod->writeLeftTable(mw_one->ui->table_nv_add0,
                               mw_one->ui->table_nv_add);
    }
  }

  // NVRAM Delete
  if (blNVDelete) {
    bool re = false;
    for (int i = 0; i < mw_one->ui->table_nv_del0->rowCount(); i++) {
      QString str0 = mw_one->ui->table_nv_del0->item(i, 0)->text();
      QString str1 = ui->listPreset->item(ui->listPreset->currentRow())->text();
      if (str0 == str1) {
        re = true;
        mw_one->ui->table_nv_del0->setCurrentCell(i, 0);
      }
    }

    if (!re) {
      mw_one->on_btnNVRAMDel_Add0_clicked();
      mw_one->init_value(map_add, mw_one->ui->table_nv_del0,
                         mw_one->ui->table_nv_del,
                         ui->listPreset->currentRow());

      mymethod->writeLeftTableOnlyValue(mw_one->ui->table_nv_del0,
                                        mw_one->ui->table_nv_del);
    }
  }

  // NVRAM Legacy
  if (blNVLegacy) {
    bool re = false;
    for (int i = 0; i < mw_one->ui->table_nv_ls0->rowCount(); i++) {
      QString str0 = mw_one->ui->table_nv_ls0->item(i, 0)->text();
      QString str1 = ui->listPreset->item(ui->listPreset->currentRow())->text();
      if (str0 == str1) {
        re = true;
        mw_one->ui->table_nv_ls0->setCurrentCell(i, 0);
      }
    }

    if (!re) {
      mw_one->on_btnNVRAMLS_Add0_clicked();
      mw_one->init_value(map_add, mw_one->ui->table_nv_ls0,
                         mw_one->ui->table_nv_ls, ui->listPreset->currentRow());
      mymethod->writeLeftTableOnlyValue(mw_one->ui->table_nv_ls0,
                                        mw_one->ui->table_nv_ls);
    }
  }

  addKextPresets();
}

void dlgPreset::addKextPresets() {
  if (blKext) {
    QStringList list;
    QString strPath = QDir::homePath() + "/.ocat/Database/EFI/OC/Kexts/";
    for (int i = 0; i < ui->listPreset->count(); i++) {
      if (ui->listPreset->item(i)->checkState() == Qt::Checked) {
        list.append(strPath + ui->listPreset->item(i)->text());
      }
    }

    mw_one->addKexts(list);
  }
}

void dlgPreset::on_listPreset_itemDoubleClicked(QListWidgetItem* item) {
  Q_UNUSED(item);
  ui->btnAdd->clicked();
}
