#include "dlgkernelpatch.h"

#include "Method.h"
#include "mainwindow.h"
#include "ui_dlgkernelpatch.h"
#include "ui_mainwindow.h"

extern MainWindow *mw_one;

dlgKernelPatch::dlgKernelPatch(QWidget *parent)
    : QDialog(parent), ui(new Ui::dlgKernelPatch) {
  ui->setupUi(this);
  QFileInfo appInfo(qApp->applicationDirPath());
  dirpath = appInfo.filePath() + "/Database/preset/Kernel-Patch/";
}

dlgKernelPatch::~dlgKernelPatch() { delete ui; }

void dlgKernelPatch::loadFiles() {
  QDir dir(dirpath);
  QStringList nameFilters;
  nameFilters << "*.plist";
  QStringList filesTemp =
      dir.entryList(nameFilters, QDir::Files | QDir::Readable, QDir::Name);
  QStringList files;
  for (int j = 0; j < filesTemp.count(); j++) {
    if (filesTemp.at(j).mid(0, 1) != ".") files.append(filesTemp.at(j));
  }

  ui->listWidget->clear();
  ui->listWidget->addItems(files);
  if (files.count() > 0) ui->listWidget->setCurrentRow(0);
}

void dlgKernelPatch::appendKernelPatch(QString PlistFileName) {
  if (!PlistFileName.isEmpty()) {
    if (!PListSerializer::fileValidation(PlistFileName)) {
      QMessageBox::warning(this, "", tr("Invalid plist file."), tr("OK"));
      return;
    }
  } else
    return;

  QFile file(PlistFileName);
  QVariantMap map = PListParser::parsePList(&file).toMap();
  if (map.isEmpty()) return;
  file.close();

  map = map["Kernel"].toMap();
  QVariantList listKP = map["Patch"].toList();
  Method::set_TableData(mw_one->ui->table_kernel_patch, listKP);
}

void dlgKernelPatch::on_btnAdd_clicked() {
  int row = ui->listWidget->currentRow();
  if (row < 0) return;
  appendKernelPatch(dirpath + ui->listWidget->item(row)->text());

  int total = mw_one->ui->table_kernel_patch->rowCount();
  mw_one->ui->table_kernel_patch->setCurrentCell(total - 1, 0);
}

void dlgKernelPatch::on_btnOpenDir_clicked() {
  QString dir = "file:" + dirpath;
  QDesktopServices::openUrl(QUrl(dir, QUrl::TolerantMode));
}

void dlgKernelPatch::on_listWidget_itemDoubleClicked(QListWidgetItem *item) {
  Q_UNUSED(item);
  ui->btnAdd->clicked();
}
