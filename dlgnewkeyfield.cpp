#include "dlgnewkeyfield.h"

#include "Method.h"
#include "mainwindow.h"
#include "ui_dlgnewkeyfield.h"
#include "ui_mainwindow.h"

extern MainWindow* mw_one;
extern QString ocVer, strIniFile, strAppName;
extern QString ocVerDev;
extern QString ocFrom;
extern QString ocFromDev;
extern bool blDEV;
extern Method* mymethod;
extern QString SaveFileName;

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

  chk->setContextMenuPolicy(Qt::CustomContextMenu);
  QMenu* menu = new QMenu();
  QAction* act = new QAction(tr("Delete"));
  QAction* actRename = new QAction(tr("Rename"));
  menu->addAction(act);
  menu->addAction(actRename);
  connect(act, &QAction::triggered, [=]() {
    mw_one->ui->cboxFind->lineEdit()->clear();
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

  lbl->setContextMenuPolicy(Qt::CustomContextMenu);
  QMenu* menu = new QMenu();
  QAction* act = new QAction(tr("Delete"));
  QAction* actRename = new QAction(tr("Rename"));
  menu->addAction(act);
  menu->addAction(actRename);

  connect(act, &QAction::triggered, [=]() {
    mw_one->ui->cboxFind->lineEdit()->clear();
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
