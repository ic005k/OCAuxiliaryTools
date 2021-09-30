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
  QString strPlistFile = strPresetPath + "DPAdd.plist";
  QFile file(strPlistFile);

  map = PListParser::parsePList(&file).toMap();
  map = map["DeviceProperties"].toMap();
  if (map.isEmpty()) return;

  // Add
  map_add = map["Add"].toMap();
  ui->listDPAdd->clear();
  for (int i = 0; i < map_add.count(); i++) {
    ui->listDPAdd->addItem(map_add.keys().at(i));
  }
}

void dlgPreset::on_btnAdd_clicked() {
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
          strItem.trimmed())
        re = true;
    }

    if (!re) {
      mw_one->on_btnDPAdd_Add0_clicked();
      int table_row = mw_one->ui->table_dp_add0->currentRow();
      QTableWidgetItem* newItem1 = new QTableWidgetItem(strItem);
      mw_one->ui->table_dp_add0->setItem(table_row, 0, newItem1);

      //加载子条目
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
}
