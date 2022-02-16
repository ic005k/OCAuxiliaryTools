#include "dlgOCValidate.h"

#include "mainwindow.h"
#include "ui_dlgOCValidate.h"
#include "ui_mainwindow.h"

extern MainWindow* mw_one;
extern QString SaveFileName;

dlgOCValidate::dlgOCValidate(QWidget* parent)
    : QDialog(parent), ui(new Ui::dlgOCValidate) {
  ui->setupUi(this);
  ui->textEdit->setReadOnly(true);

  ui->listOCValidate->setVisible(false);
  ui->btnClose->setVisible(false);
  ui->btnGo->setVisible(false);
  ui->btnCreateVault->setDefault(true);

  ui->textEdit->setContextMenuPolicy(Qt::CustomContextMenu);
  QAction* copyAction = new QAction(tr("Copy"));
  QAction* searchAction = new QAction(tr("Search..."));

  QMenu* copyMenu = new QMenu(this);

  copyMenu->addAction(searchAction);
  copyMenu->addAction(copyAction);

  connect(copyAction, &QAction::triggered, [=]() {
    QString str = ui->textEdit->textCursor().selectedText();

    QClipboard* clipboard = QApplication::clipboard();
    clipboard->setText(str.trimmed());
  });

  connect(searchAction, &QAction::triggered, [=]() {
    QString str = ui->textEdit->textCursor().selectedText().trimmed();

    mw_one->ui->mycboxFind->setCurrentText(str);
    mw_one->on_actionFind_triggered();
  });

  connect(ui->textEdit, &QTextEdit::customContextMenuRequested,
          [=](const QPoint& pos) {
            Q_UNUSED(pos);

            QString str = ui->textEdit->textCursor().selectedText().trimmed();
            if (str.count() > 0) {
              searchAction->setEnabled(true);
              copyAction->setEnabled(true);
            } else {
              searchAction->setEnabled(false);
              copyAction->setEnabled(false);
            }

            copyMenu->exec(QCursor::pos());
          });
}

dlgOCValidate::~dlgOCValidate() { delete ui; }

void dlgOCValidate::setTextOCV(QString str) {
  ui->textEdit->clear();
  ui->textEdit->append(str);

  ui->listOCValidate->clear();
  for (int i = 0; i < ui->textEdit->document()->lineCount(); i++) {
    QTextBlock block = ui->textEdit->document()->findBlockByNumber(i);
    ui->textEdit->setTextCursor(QTextCursor(block));
    QString lineText =
        ui->textEdit->document()->findBlockByNumber(i).text().trimmed();
    ui->listOCValidate->addItem(lineText);
  }
}

void dlgOCValidate::on_btnClose_clicked() { this->close(); }

void dlgOCValidate::setGoEnabled(bool enabled) {
  if (enabled)
    ui->btnGo->setEnabled(true);
  else
    ui->btnGo->setEnabled(false);
}

void dlgOCValidate::on_btnGo_clicked() {
  QString str0, str1;
  QStringList strList, strList1;
  str0 = ui->listOCValidate->item(ui->listOCValidate->currentRow())->text();
  if (str0.contains("contains")) {
    strList = str0.trimmed().split("contains");
    str1 = strList.at(0);
    strList = str1.split("->");
    qDebug() << strList.at(0) << strList.at(1) << strList.at(2);
    str1 = strList.at(1);
    strList1 = str1.split("[");
    goMainList(strList.at(0), strList1.at(0));
  }
}

void dlgOCValidate::goMainList(QString value, QString subValue) {
  if (value == "ACPI") {
    mw_one->ui->listMain->setCurrentRow(0);
    if (subValue == "Add") mw_one->ui->listSub->setCurrentRow(0);
    if (subValue == "Delete") mw_one->ui->listSub->setCurrentRow(1);
    if (subValue == "Patch") mw_one->ui->listSub->setCurrentRow(2);
    if (subValue == "Quirks") mw_one->ui->listSub->setCurrentRow(3);
  }
  if (value == "Booter") mw_one->ui->listMain->setCurrentRow(1);
  if (value == "DeviceProperties") mw_one->ui->listMain->setCurrentRow(2);
  if (value == "Kernel") mw_one->ui->listMain->setCurrentRow(3);
  if (value == "Misc") mw_one->ui->listMain->setCurrentRow(4);
  if (value == "NVRAM") mw_one->ui->listMain->setCurrentRow(5);
  if (value == "PlatformInfo") mw_one->ui->listMain->setCurrentRow(6);
  if (value == "UEFI") mw_one->ui->listMain->setCurrentRow(7);
}

void dlgOCValidate::on_btnCreateVault_clicked() {
  QFileInfo fi(SaveFileName);
  QString DirName = fi.path().mid(0, fi.path().count() - 3);
  QString strTar = DirName + "/OC";
  if (!QDir(strTar).exists()) return;

  QString warningStr =
      tr("Please make sure you know the Vault completely and that you have "
         "backed up the EFI beforehand, otherwise the OC may not boot!");
  int ret =
      QMessageBox::warning(this, "", warningStr, tr("Cancel"), tr("Sure"));
  if (ret != 1) {
    return;
  }

  ui->btnCreateVault->setEnabled(false);
  repaint();

  QString dirpath = mw_one->userDataBaseDir + "mac/CreateVault/";
  QString fileName;
  if (ui->chkSignature->isChecked())
    fileName = dirpath + "sign.command";
  else
    fileName = dirpath + "create_vault.sh";

  ui->textEdit->clear();
  process = new QProcess;
  process->start("bash", QStringList() << fileName << strTar);
  connect(process, SIGNAL(finished(int)), this, SLOT(readResult(int)));
  connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(readData()));
}

void dlgOCValidate::readResult(int exitCode) {
  if (exitCode == 0) {
    ui->btnCreateVault->setEnabled(true);
    repaint();
  }
}

void dlgOCValidate::readData() {
  QString result = process->readAllStandardOutput();

  ui->textEdit->append(result);
}
