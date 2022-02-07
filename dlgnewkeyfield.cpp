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
  if (ui->cboxKeyType->currentText() == tr("bool")) {
    QString text = ui->editKeyName->text().trimmed();
    QString ObjectName = "chk" + text;

    QWidget* tab = mymethod->getSubTabWidget(main, sub);
    add_CheckBox(tab, ObjectName, text);

    saveNewKey(ObjectName, main, sub);

  } else {
    QString ObjectName, text;
    text = ui->editKeyName->text().trimmed();
    if (ui->cboxKeyType->currentText() == tr("string"))
      ObjectName = "edit" + text;

    if (ui->cboxKeyType->currentText() == tr("integer"))
      ObjectName = "editInt" + text;

    if (ui->cboxKeyType->currentText() == tr("data"))
      ObjectName = "editDat" + text;
    QWidget* tab = mymethod->getSubTabWidget(main, sub);
    add_LineEdit(tab, ObjectName, text);
    saveNewKey(ObjectName, main, sub);
  }
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
  qDebug() << list;
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
  chk->setText(text);
  chk->setObjectName(ObjectName);
  hbox->addWidget(chk);

  chk->setContextMenuPolicy(Qt::CustomContextMenu);
  QMenu* menu = new QMenu();
  QAction* act = new QAction(tr("Delete"));
  menu->addAction(act);
  connect(act, &QAction::triggered, [=]() {
    tab->layout()->removeWidget(frame);
    delete (frame);
    QSettings Reg(strIniFile, QSettings::IniFormat);
    Reg.setValue("key" + ObjectName, "");
  });

  connect(chk, &QCheckBox::customContextMenuRequested, [=](const QPoint& pos) {
    Q_UNUSED(pos);
    menu->exec(QCursor::pos());
  });

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
  lbl->setText(text);
  QLineEdit* edit = new QLineEdit();
  if (ObjectName.mid(0, 7) == "editInt")
    edit->setPlaceholderText(tr("Integer"));
  if (ObjectName.mid(0, 7) == "editDat")
    edit->setPlaceholderText(tr("Hexadecimal"));
  edit->setObjectName(ObjectName);

  hbox->addWidget(lbl);
  hbox->addWidget(edit);

  lbl->setContextMenuPolicy(Qt::CustomContextMenu);
  QMenu* menu = new QMenu();
  QAction* act = new QAction(tr("Delete"));
  menu->addAction(act);
  connect(act, &QAction::triggered, [=]() {
    tab->layout()->removeWidget(frame);
    delete (frame);
    QSettings Reg(strIniFile, QSettings::IniFormat);
    Reg.setValue("key" + ObjectName, "");
  });

  connect(lbl, &QCheckBox::customContextMenuRequested, [=](const QPoint& pos) {
    Q_UNUSED(pos);
    menu->exec(QCursor::pos());
  });

  tab->layout()->addWidget(frame);
}
