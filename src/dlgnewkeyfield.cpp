#include "dlgnewkeyfield.h"

#include "Method.h"
#include "mainwindow.h"
#include "ui_dlgnewkeyfield.h"
#include "ui_mainwindow.h"

extern MainWindow* mw_one;
extern Method* mymethod;
extern QString ocVer, strIniFile, strAppName, strAppExePath, ocVerDev, ocFrom,
    ocFromDev, SaveFileName;
extern bool blDEV;
extern int red;
bool isSmartKey;
QWidgetList listOCATWidgetHideList, listOCATWidgetDelList;
QStringList listKey, listType, listValue;

dlgNewKeyField::dlgNewKeyField(QWidget* parent)
    : QDialog(parent), ui(new Ui::dlgNewKeyField) {
  ui->setupUi(this);
}

dlgNewKeyField::~dlgNewKeyField() { delete ui; }

void dlgNewKeyField::on_btnAdd_clicked() {
  if (ui->editKeyName->text().trimmed() == "") return;

  int main = mw_one->ui->listMain->currentRow();
  int sub = mw_one->ui->listSub->currentRow();
  QWidget* tab = mw_one->getSubTabWidget(main, sub);

  if (ui->cboxKeyType->currentText() == tr("bool")) {
    QString text = ui->editKeyName->text().trimmed();
    QString ObjectName = "chk" + text;
    bool re = false;
    QObjectList listOfCheckBox =
        mw_one->getAllCheckBox(mw_one->getAllUIControls(tab));
    for (int i = 0; i < listOfCheckBox.count(); i++) {
      QCheckBox* w = (QCheckBox*)listOfCheckBox.at(i);
      if (w->objectName() == ObjectName) re = true;
    }
    if (!re) {
      add_CheckBox(tab, ObjectName, text);
      saveNewKey(ObjectName, main, sub);
      mw_one->setWM();
    } else
      QMessageBox::critical(
          this, "", tr("The key field already exists and cannot be added."));

  } else {
    QString ObjectName, text;
    text = ui->editKeyName->text().trimmed();
    if (ui->cboxKeyType->currentText() == tr("string"))
      ObjectName = "edit" + text;

    if (ui->cboxKeyType->currentText() == tr("integer"))
      ObjectName = "editInt" + text;

    if (ui->cboxKeyType->currentText() == tr("data"))
      ObjectName = "editDat" + text;

    bool re = false;
    QObjectList listOfEdit =
        mw_one->getAllLineEdit(mw_one->getAllUIControls(tab));
    for (int i = 0; i < listOfEdit.count(); i++) {
      QLineEdit* w = (QLineEdit*)listOfEdit.at(i);
      if (w->objectName() == ObjectName) re = true;
    }
    if (!re) {
      add_LineEdit(tab, ObjectName, text);
      saveNewKey(ObjectName, main, sub);
      mw_one->setWM();
    } else
      QMessageBox::critical(
          this, "", tr("The key field already exists and cannot be added."));
  }

  mw_one->init_setWindowModified();
}

void dlgNewKeyField::saveNewKey(QString ObjectName, int main, int sub) {
  QSettings Reg(strIniFile, QSettings::IniFormat);
  Reg.setValue("key" + ObjectName, ObjectName + "|" + QString::number(main) +
                                       "|" + QString::number(sub));
}

QStringList dlgNewKeyField::getAllNewKey() {
  QStringList list;
  QSettings Reg(strIniFile, QSettings::IniFormat);
  for (int i = 0; i < Reg.allKeys().count(); i++) {
    QString str = Reg.allKeys().at(i);
    if (str.mid(0, 3) == "key" && Reg.value(str).toString() != "") {
      list.append(str);
    }
  }

  return list;
}

QList<int> dlgNewKeyField::getKeyMainSub(QString Key) {
  QList<int> list;
  QSettings Reg(strIniFile, QSettings::IniFormat);
  QString str = Reg.value(Key).toString();
  QStringList list1 = str.split("|");

  if (list1.count() == 3) {
    QString strM, strS;
    strM = list1.at(1);
    strS = list1.at(2);
    int main = strM.toInt();
    int sub = strS.toInt();
    list.append(main);
    list.append(sub);

    return list;
  }
  return list;
}

void dlgNewKeyField::readNewKey(QWidget* tab, QString Key) {
  QSettings Reg(strIniFile, QSettings::IniFormat);
  QString str = Reg.value(Key).toString();

  QStringList list1 = str.split("|");
  if (list1.count() == 3) {
    QString text;
    QString ObjectName = list1.at(0);
    QString name = ObjectName;
    if (ObjectName.mid(0, 3) == "chk") {
      text = name.replace("chk", "");
      add_CheckBox(tab, ObjectName, text);
    }

    if (ObjectName.mid(0, 4) == "edit" && ObjectName.mid(0, 7) != "editInt" &&
        ObjectName.mid(0, 7) != "editDat") {
      text = name.replace("edit", "");
      add_LineEdit(tab, ObjectName, text);
    }
    if (ObjectName.mid(0, 7) == "editInt") {
      text = name.replace("editInt", "");
      add_LineEdit(tab, ObjectName, text);
    }
    if (ObjectName.mid(0, 7) == "editDat") {
      text = name.replace("editDat", "");
      add_LineEdit(tab, ObjectName, text);
    }
  }
}

void dlgNewKeyField::add_CheckBox(QWidget* tab, QString ObjectName,
                                  QString text) {
  QCheckBox* chk = new QCheckBox();
  if (!isSmartKey) {
    QFont font;
    font.setBold(true);
    chk->setFont(font);
  }
  chk->setText(text);
  chk->setObjectName(ObjectName);
  chk->setFixedHeight(18);
  chk->setContextMenuPolicy(Qt::CustomContextMenu);
  QMenu* menu = new QMenu();
  QAction* actDelete = new QAction(tr("Delete"));
  QAction* actRename = new QAction(tr("Rename"));
  if (!isSmartKey) {
    menu->addAction(actDelete);
    menu->addAction(actRename);
  }
  connect(actDelete, &QAction::triggered, [=]() {
    mw_one->ui->mycboxFind->lineEdit()->clear();
    tab->layout()->removeWidget(chk);
    delete (chk);

    removeKey(ObjectName);
    mw_one->setWM();
  });

  connect(actRename, &QAction::triggered, [=]() {
    QString oldObjectName = ObjectName;
    QString newObjName;
    bool ok;
    QString newText =
        QInputDialog::getText(tab, tr("Rename"), tr("Key Name:"),
                              QLineEdit::Normal, chk->text(), &ok);
    if (ok && !newText.isEmpty()) {
      if (newText == chk->text()) return;
      chk->setText(newText);
      newObjName = "chk" + newText;
      chk->setObjectName(newObjName);

      int main, sub;
      main = getKeyMainSub("key" + ObjectName).at(0);
      sub = getKeyMainSub("key" + ObjectName).at(1);
      saveNewKey(newObjName, main, sub);
      removeKey(oldObjectName);
    }

    mw_one->setWM();
  });

  if (!isSmartKey) {
    connect(chk, &QCheckBox::customContextMenuRequested,
            [=](const QPoint& pos) {
              Q_UNUSED(pos);
              menu->exec(QCursor::pos());
            });
  }

  QObjectList listObj;
  listObj = MainWindow::getAllFrame(MainWindow::getAllUIControls(tab));
  bool isDo = false;
  for (int i = 0; i < listObj.count(); i++) {
    QFrame* w = (QFrame*)listObj.at(i);
    if (w->objectName().contains("chk")) {
      w->layout()->addWidget(chk);
      isDo = true;
      break;
    }
  }

  if (!isDo) tab->layout()->addWidget(chk);

  if (isSmartKey) {
    listOCATWidgetDelList.removeOne(chk);
    listOCATWidgetDelList.append(chk);
  }

  qDebug() << "Added Widget: " << tab->objectName() << "=>"
           << chk->objectName();
}

QLineEdit* dlgNewKeyField::add_LineEdit(QWidget* tab, QString ObjectName,
                                        QString text) {
  QHBoxLayout* hbox = new QHBoxLayout();
  QFrame* frame = new QFrame();
  frame->setLayout(hbox);
  frame->layout()->setContentsMargins(0, 0, 0, 0);
  frame->layout()->setSpacing(1);

  QLabel* lbl = new QLabel();
  if (!isSmartKey) {
    QFont font;
    font.setBold(true);
    lbl->setFont(font);
  }
  lbl->setText(text);
  lbl->setObjectName("lbl" + text);
  frame->setObjectName("frame" + text);
  QLineEdit* edit = new QLineEdit();
  edit->setObjectName(ObjectName);

  QRegularExpression regxData("[A-Fa-f0-9 ]{0,1024}");
  QRegularExpression regxNumber("^-?\[0-9]*$");
  if (ObjectName.mid(0, 7) == "editInt") {
    QValidator* validator = new QRegularExpressionValidator(regxNumber, edit);
    edit->setValidator(validator);
    edit->setPlaceholderText(tr("Integer"));
  }
  if (ObjectName.mid(0, 7) == "editDat") {
    QValidator* validator = new QRegularExpressionValidator(regxData, edit);
    edit->setValidator(validator);
    edit->setPlaceholderText(tr("Hexadecimal"));
  }

  lbl->setContextMenuPolicy(Qt::CustomContextMenu);
  QMenu* menu = new QMenu();
  QAction* actDelete = new QAction(tr("Delete"));
  QAction* actRename = new QAction(tr("Rename"));
  if (!isSmartKey) {
    menu->addAction(actDelete);
    menu->addAction(actRename);
  }

  connect(actDelete, &QAction::triggered, [=]() {
    mw_one->ui->mycboxFind->lineEdit()->clear();
    tab->layout()->removeWidget(frame);
    delete (frame);

    removeKey(ObjectName);
    mw_one->setWM();
  });

  connect(actRename, &QAction::triggered, [=]() {
    QString oldObjectName = ObjectName;
    QString newObjName;
    bool ok;
    QString newText =
        QInputDialog::getText(tab, tr("Rename"), tr("Key Name:"),
                              QLineEdit::Normal, lbl->text(), &ok);
    if (ok && !newText.isEmpty()) {
      if (newText == lbl->text()) return;
      lbl->setText(newText);
      if (ObjectName.mid(0, 4) == "edit" && ObjectName.mid(0, 7) != "editInt" &&
          ObjectName.mid(0, 7) != "editDat")
        newObjName = "edit" + newText;
      if (ObjectName.mid(0, 7) == "editInt") newObjName = "editInt" + newText;
      if (ObjectName.mid(0, 7) == "editDat") newObjName = "editDat" + newText;
      edit->setObjectName(newObjName);

      int main, sub;
      main = getKeyMainSub("key" + ObjectName).at(0);
      sub = getKeyMainSub("key" + ObjectName).at(1);
      saveNewKey(newObjName, main, sub);
      removeKey(oldObjectName);
    }

    mw_one->setWM();
  });

  if (!isSmartKey) {
    connect(lbl, &QCheckBox::customContextMenuRequested,
            [=](const QPoint& pos) {
              Q_UNUSED(pos);
              menu->exec(QCursor::pos());
            });
  }

  hbox->addWidget(lbl);
  hbox->addWidget(edit);
  frame->setMaximumHeight(25);

  QObjectList listObj;
  listObj = MainWindow::getAllFrame(MainWindow::getAllUIControls(tab));
  bool isDo = false;
  for (int i = 0; i < listObj.count(); i++) {
    QFrame* w = (QFrame*)listObj.at(i);
    if (w->objectName().contains("edit")) {
      w->layout()->addWidget(frame);
      isDo = true;
      break;
    }
  }

  if (!isDo) tab->layout()->addWidget(frame);

  if (isSmartKey) {
    listOCATWidgetDelList.removeOne(frame);
    listOCATWidgetDelList.append(frame);
  }

  if (edit->objectName() == "editBootProtect" && edit->text() == "") {
    edit->setText("None");
  }
  if (edit->objectName() == "editCustomDelays") {
    if (edit->text() == "" || edit->text() == "true" || edit->text() == "false")
      edit->setText("Auto");
  }

  edit->setAttribute(Qt::WA_MacShowFocusRect, 0);
  if (red > 55)
    Method::setLineEditQss(edit, 6, 1, "#C0C0C0", "#4169E1");
  else
    Method::setLineEditQss(edit, 6, 1, "#2A2A2A", "#4169E1");

  qDebug() << "Added Widget: " << tab->objectName() << "=>"
           << edit->objectName();
  return edit;
}

void dlgNewKeyField::removeKey(QString ObjectName) {
  QSettings Reg(strIniFile, QSettings::IniFormat);
  Reg.remove("key" + ObjectName);
}

QStringList dlgNewKeyField::check_SampleFile(QVariantMap mapTatol, QWidget* tab,
                                             QString MainName,
                                             QString SubName) {
  isSmartKey = true;
  QStringList ResultsList;
  listType.clear();
  listValue.clear();
  listKey.clear();
  QStringList listOCAT, listOCATKey, listOCATType, listSample, listSampleKey,
      listSampleValue, listSampleType;

  QWidgetList listOCATWidget;

  listSample = get_KeyTypeValue(mapTatol, MainName, SubName);
  for (int i = 0; i < listSample.count(); i++) {
    QString str = listSample.at(i);
    QStringList list0 = str.split("|");
    if (list0.count() == 2) {
      listSampleKey.append(list0.at(0));
      listSampleType.append(list0.at(0) + "|" + list0.at(1));
      listSampleValue.append("");
    }
    if (list0.count() == 3) {
      listSampleKey.append(list0.at(0));
      listSampleType.append(list0.at(0) + "|" + list0.at(1));
      listSampleValue.append(list0.at(2));
    }
  }
  listKey = listSampleKey;
  listType = listSampleType;
  listValue = listSampleValue;

  QObjectList listObj;
  listObj = MainWindow::getAllCheckBox(MainWindow::getAllUIControls(tab));
  for (int i = 0; i < listObj.count(); i++) {
    QCheckBox* chkbox = (QCheckBox*)listObj.at(i);
    if (chkbox->objectName().mid(0, 3) == "chk") {
      QString text = chkbox->text();
      listOCAT.append(text + "|" + chkbox->objectName());
      listOCATKey.append(text);
      listOCATWidget.append(chkbox);
      listOCATType.append(text + "|bool");
    }
  }
  listObj.clear();
  listObj = MainWindow::getAllComboBox(MainWindow::getAllUIControls(tab));
  for (int i = 0; i < listObj.count(); i++) {
    QComboBox* w = (QComboBox*)listObj.at(i);
    QString ObjName = w->objectName();
    if (ObjName.mid(0, 4) == "cbox") {
      QString text = ObjName.replace("cbox", "");
      listOCAT.append(text + "|" + w->objectName());
      listOCATKey.append(text);
      listOCATType.append(text + "|QString");
      listOCATWidget.append(w);
    }
  }
  listObj.clear();
  listObj = MainWindow::getAllLineEdit(MainWindow::getAllUIControls(tab));
  for (int i = 0; i < listObj.count(); i++) {
    QLineEdit* w = (QLineEdit*)listObj.at(i);
    QString ObjName = w->objectName();
    if (ObjName.contains("_")) {
      QStringList strList = ObjName.split("_");
      ObjName = strList.at(0);
    }
    if (ObjName.mid(0, 4) == "edit") {
      QString obj, obj1, obj2, obj3;
      obj = ObjName;
      obj1 = ObjName;
      obj2 = ObjName;
      obj3 = ObjName;
      QString text;
      if (ObjName.mid(0, 4) == "edit" && ObjName.mid(0, 7) != "editInt" &&
          ObjName.mid(0, 7) != "editDat") {
        text = obj1.replace("edit", "");
        listOCATType.append(text + "|QString");
      }
      if (ObjName.mid(0, 7) == "editDat") {
        text = obj2.replace("editDat", "");
        listOCATType.append(text + "|QByteArray");
      }
      if (ObjName.mid(0, 7) == "editInt") {
        text = obj3.replace("editInt", "");
        listOCATType.append(text + "|qlonglong");
      }

      listOCAT.append(text + "|" + obj);
      listOCATKey.append(text);
      listOCATWidget.append(w);
    }
  }

  for (int i = 0; i < listSampleType.count(); i++) {
    QString str = listSampleType.at(i);
    if (listOCATType.removeOne(str) && listSampleType.removeOne(str)) i--;
  }
  if (listOCATType.count() > 0 && listSampleType.count() > 0) {
    for (int i = 0; i < listSampleType.count(); i++) {
      QString str = listSampleType.at(i);
      QStringList list = str.split("|");
      QString Key;
      for (int n = 0; n < listOCATType.count(); n++) {
        QString str1 = listOCATType.at(n);
        QStringList list1 = str1.split("|");
        if (list.count() == 2 && list1.count() == 2) {
          if (list.at(0) == list1.at(0) && list.at(1) != list1.at(1)) {
            Key = list.at(0);
            if (list.at(1) == "bool") {
              set_WidgetHide(listOCATWidget, Key);
              set_LblHide(tab, Key);
              add_CheckBox(tab, "chk" + Key, Key);
            }
            if (list.at(1) == "QString") {
              set_WidgetHide(listOCATWidget, Key);
              set_LblHide(tab, Key);
              add_LineEdit(tab, "edit" + Key, Key);
            }
            if (list.at(1) == "QByteArray") {
              set_WidgetHide(listOCATWidget, Key);
              set_LblHide(tab, Key);
              add_LineEdit(tab, "editDat" + Key, Key);
            }
            if (list.at(1) == "qlonglong") {
              set_WidgetHide(listOCATWidget, Key);
              set_LblHide(tab, Key);
              add_LineEdit(tab, "editInt" + Key, Key);
            }
          }
        }
      }
    }
  }

  if (listOCATKey == listSampleKey) {
    qDebug() << MainName << "-->" << SubName << "Ok...";
  } else {
    for (int i = 0; i < listSampleKey.count(); i++) {
      QString strSampleKey = listSampleKey.at(i);
      for (int j = 0; j < listOCATKey.count(); j++) {
        if (strSampleKey == listOCATKey.at(j)) {
          listSampleKey.removeAt(i);
          i--;
          listOCATKey.removeAt(j);
          listOCAT.removeAt(j);
          listOCATWidget.removeAt(j);
          j--;
        }
      }
    }
    if (listOCATKey.count() > 0) {
      // if (MainName == "NVRAM")
      //  qDebug() << listOCATKey << listOCAT << listSampleKey;
      for (int i = 0; i < listOCAT.count(); i++) {
        QWidget* w = listOCATWidget.at(i);
        w->setHidden(true);
        listOCATWidgetHideList.removeOne(w);
        listOCATWidgetHideList.append(w);

        QString obj = w->objectName();
        if (obj.mid(0, 4) == "edit" || obj.mid(0, 4) == "cbox") {
          QString txt = get_WidgetText(w);
          if (txt.contains("_")) {
            txt = txt.split("_").at(0);
          }
          set_LblHide(tab, txt);
          set_BtnHide(tab, txt);
        }
      }
    }
    if (listSampleKey.count() > 0) {
      // qDebug() << listSampleKey;
      for (int i = 0; i < listSampleKey.count(); i++) {
        QString Key = listSampleKey.at(i);
        for (int j = 0; j < listSample.count(); j++) {
          QString str1 = listSample.at(j);
          QStringList list = str1.split("|");
          if (Key == list.at(0)) {
            // Add Widget
            if (list.at(1) == "bool") {
              add_CheckBox(tab, "chk" + Key, Key);
            }
            if (list.at(1) == "qlonglong") {
              add_LineEdit(tab, "editInt" + Key, Key);
            }
            if (list.at(1) == "QString") {
              add_LineEdit(tab, "edit" + Key, Key);
            }
            if (list.at(1) == "QByteArray") {
              add_LineEdit(tab, "editDat" + Key, Key);
            }
          }
        }
      }
    }
  }

  isSmartKey = false;
  return ResultsList;
}

void dlgNewKeyField::set_BtnHide(QWidget* tab, QString Key) {
  QObjectList wl =
      MainWindow::getAllToolButton(MainWindow::getAllUIControls(tab));
  for (int m = 0; m < wl.count(); m++) {
    QToolButton* lbl = (QToolButton*)wl.at(m);
    if (lbl->objectName().replace("btn", "") == Key) {
      lbl->setHidden(true);
      listOCATWidgetHideList.removeOne(lbl);
      listOCATWidgetHideList.append(lbl);
    }
  }
}

void dlgNewKeyField::set_LblHide(QWidget* tab, QString Key) {
  QWidgetList wl = get_AllLabelList(tab);
  for (int m = 0; m < wl.count(); m++) {
    QLabel* lbl = (QLabel*)wl.at(m);
    if (lbl->text() == Key) {
      lbl->setHidden(true);
      listOCATWidgetHideList.removeOne(lbl);
      listOCATWidgetHideList.append(lbl);
    }
  }
}

void dlgNewKeyField::set_WidgetHide(QWidgetList listOCATWidget, QString Key) {
  for (int m = 0; m < listOCATWidget.count(); m++) {
    QWidget* w = listOCATWidget.at(m);
    QString txt = w->objectName();
    if (txt.contains(Key)) {
      w->setHidden(true);
      listOCATWidgetHideList.removeOne(w);
      listOCATWidgetHideList.append(w);
    }
  }
}

QStringList dlgNewKeyField::get_KeyTypeValue(QVariantMap mapTatol,
                                             QString MainName,
                                             QString SubName) {
  QStringList list0;
  QVariantMap mapMain, map;
  QStringList keyList;
  mapMain = mapTatol[MainName].toMap();
  if (SubName != "") {
    QVariantMap mapSub = mapMain[SubName].toMap();
    keyList = mapSub.keys();
    map = mapSub;

    if (SubName == "Serial") {
      QVariantMap mapMisc = mapTatol["Misc"].toMap();
      QVariantMap mapSerial = mapMisc["Serial"].toMap();
      QVariantMap mapCustom = mapSerial["Custom"].toMap();
      for (int i = 0; i < mapCustom.count(); i++) {
        keyList.append(mapCustom.keys().at(i));
      }
    }
  } else {
    keyList = mapMain.keys();
    map = mapMain;
  }

  for (int i = 0; i < keyList.count(); i++) {
    QString name = keyList.at(i);
    QString type = map[name].typeName();
    list0.append(name + "|" + type);
  }
  // if (MainName == "NVRAM") qDebug() << list0;
  if (SubName == "") {
    for (int i = 0; i < list0.count(); i++) {
      QString str = list0.at(i);
      if (str.contains("QVariantMap")) {
        list0.removeAt(i);
        i--;
      }
    }
  }
  // if (MainName == "NVRAM") qDebug() << list0;
  return list0;
}

QWidgetList dlgNewKeyField::get_AllLabelList(QWidget* tab) {
  QObjectList listObj;
  QWidgetList wl;
  listObj = MainWindow::getAllLabel(MainWindow::getAllUIControls(tab));
  for (int i = 0; i < listObj.count(); i++) {
    QLabel* w = (QLabel*)listObj.at(i);
    wl.append(w);
  }
  return wl;
}

QString dlgNewKeyField::get_WidgetText(QWidget* w) {
  QString obj = w->objectName();
  if (obj.mid(0, 3) == "chk") return obj.replace("chk", "");
  if (obj.mid(0, 4) == "cbox") return obj.replace("cbox", "");
  if (obj.mid(0, 7) == "editInt") return obj.replace("editInt", "");
  if (obj.mid(0, 7) == "editDat") return obj.replace("editDat", "");
  if (obj.mid(0, 4) == "edit" && obj.mid(0, 7) != "editInt" &&
      obj.mid(0, 7) != "editDat")
    return obj.replace("edit", "");
  return "";
}
