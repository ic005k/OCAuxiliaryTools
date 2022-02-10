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
bool isSmartKey;
QWidgetList listOCATWidgetHideList;

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
  QHBoxLayout* hbox = new QHBoxLayout();
  QFrame* frame = new QFrame();
  frame->setLayout(hbox);
  frame->layout()->setContentsMargins(0, 0, 0, 0);
  frame->layout()->setSpacing(1);

  QCheckBox* chk = new QCheckBox();
  QFont font;
  font.setBold(true);
  chk->setFont(font);
  chk->setText(text);
  chk->setObjectName(ObjectName);

  if (!isSmartKey) chk->setContextMenuPolicy(Qt::CustomContextMenu);
  QMenu* menu = new QMenu();
  QAction* act = new QAction(tr("Delete"));
  QAction* actRename = new QAction(tr("Rename"));
  menu->addAction(act);
  menu->addAction(actRename);
  connect(act, &QAction::triggered, [=]() {
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

  connect(chk, &QCheckBox::customContextMenuRequested, [=](const QPoint& pos) {
    Q_UNUSED(pos);
    menu->exec(QCursor::pos());
  });

  QSpacerItem* hi =
      new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
  // hbox->addItem(hi);
  hbox->addWidget(chk);
  // hbox->addItem(hi);

  tab->layout()->addWidget(frame);
}

void dlgNewKeyField::add_LineEdit(QWidget* tab, QString ObjectName,
                                  QString text) {
  QHBoxLayout* hbox = new QHBoxLayout();
  QFrame* frame = new QFrame();
  frame->setLayout(hbox);
  frame->layout()->setContentsMargins(0, 0, 0, 0);
  frame->layout()->setSpacing(1);

  QLabel* lbl = new QLabel();
  QFont font;
  font.setBold(true);
  lbl->setFont(font);
  lbl->setText(text);
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
  QSpacerItem* hi =
      new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
  // hbox->addItem(hi);
  hbox->addWidget(lbl);
  hbox->addWidget(edit);
  // hbox->addItem(hi);

  if (!isSmartKey) lbl->setContextMenuPolicy(Qt::CustomContextMenu);
  QMenu* menu = new QMenu();
  QAction* act = new QAction(tr("Delete"));
  QAction* actRename = new QAction(tr("Rename"));
  menu->addAction(act);
  menu->addAction(actRename);

  connect(act, &QAction::triggered, [=]() {
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

  connect(lbl, &QCheckBox::customContextMenuRequested, [=](const QPoint& pos) {
    Q_UNUSED(pos);
    menu->exec(QCursor::pos());
  });

  tab->layout()->addWidget(frame);
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

  QStringList listOCAT, listOCATKey, listSample, listSampleKey;
  QWidgetList listOCATWidget;

  listSample = get_KeyTypeValue(mapTatol, MainName, SubName);
  for (int i = 0; i < listSample.count(); i++) {
    QString str = listSample.at(i);
    listSampleKey.append(str.split("|").at(0));
  }

  QObjectList listObj;
  listObj = MainWindow::getAllCheckBox(MainWindow::getAllUIControls(tab));
  for (int i = 0; i < listObj.count(); i++) {
    QCheckBox* chkbox = (QCheckBox*)listObj.at(i);
    if (chkbox->objectName().mid(0, 3) == "chk") {
      QString text = chkbox->text();
      listOCAT.append(text + "|" + chkbox->objectName());
      listOCATKey.append(text);
      listOCATWidget.append(chkbox);
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
      }
      if (ObjName.mid(0, 7) == "editDat") {
        text = obj2.replace("editDat", "");
      }
      if (ObjName.mid(0, 7) == "editInt") {
        text = obj3.replace("editInt", "");
      }

      listOCAT.append(text + "|" + obj);
      listOCATKey.append(text);
      listOCATWidget.append(w);
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
      qDebug() << listOCATKey << listOCAT;
      for (int i = 0; i < listOCAT.count(); i++) {
        QWidget* w = listOCATWidget.at(i);
        w->setHidden(true);
        bool re = false;
        for (int n = 0; n < listOCATWidgetHideList.count(); n++) {
          if (w == listOCATWidgetHideList.at(n)) re = true;
        }
        if (!re) listOCATWidgetHideList.append(w);
      }
    }
    if (listSampleKey.count() > 0) {
      qDebug() << listSampleKey;
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

QStringList dlgNewKeyField::get_KeyTypeValue(QVariantMap mapTatol,
                                             QString MainName,
                                             QString SubName) {
  QStringList list0;
  QVariantMap mapMain;
  mapMain = mapTatol[MainName].toMap();
  QVariantMap mapSub = mapMain[SubName].toMap();
  QStringList keyList = mapSub.keys();
  for (int i = 0; i < keyList.count(); i++) {
    QString name = keyList.at(i);
    QString type = mapSub[name].typeName();
    list0.append(name + "|" + type);
    if (type == "bool") {
      // qDebug() << name << type << mapSub[name].toBool();
    }
  }
  return list0;
}
