#include "syncocdialog.h"

#include "mainwindow.h"
#include "ui_syncocdialog.h"

extern MainWindow* mw_one;

SyncOCDialog::SyncOCDialog(QWidget* parent)
    : QDialog(parent), ui(new Ui::SyncOCDialog) {
  ui->setupUi(this);
}

SyncOCDialog::~SyncOCDialog() { delete ui; }

void SyncOCDialog::on_btnStartSync_clicked() {
  bool ok = true;
  for (int i = 0; i < ui->listSource->count(); i++) {
    ui->listSource->setCurrentRow(i);
    ui->listTarget->setCurrentRow(i);

    QFile::remove(ui->listTarget->item(i)->text());

    ok = QFile::copy(ui->listSource->item(i)->text(),
                     ui->listTarget->item(i)->text());
  }

  if (ui->chkIncludeResource->isChecked())
    mw_one->copyDirectoryFiles(sourceResourcesDir, targetResourcesDir, true);

  QMessageBox box;
  box.setStyleSheet("QLabel{min-width:500 px;}");
  if (ok) {
    box.setText(tr("Successfully synced to OpenCore: ") +
                mw_one->getDatabaseVer());
    box.exec();

    close();
  }
}

void SyncOCDialog::on_listSource_currentRowChanged(int currentRow) {
  if (currentRow < 0) return;

  QString sourceModi, targetModi, sourceFile, targetFile, sourceHash,
      targetHash;
  ui->listTarget->setCurrentRow(currentRow);

  sourceFile = ui->listSource->item(currentRow)->text();
  targetFile = ui->listTarget->item(currentRow)->text();

  QCryptographicHash hashTest(QCryptographicHash::Md5);

  QFile f1(sourceFile);
  f1.open(QFile::ReadOnly);
  hashTest.addData(&f1);                   //添加数据
  sourceHash = hashTest.result().toHex();  //获得结果并转换为16进制输出
  f1.close();

  QFile f2(targetFile);
  f2.open(QFile::ReadOnly);
  hashTest.reset();  // 重置（很重要）
  hashTest.addData(&f2);
  targetHash = hashTest.result().toHex();
  f2.close();

  QFileInfo fiSource(sourceFile);
  QFileInfo fiTarget(targetFile);
  sourceModi = fiSource.lastModified().toString();
  targetModi = fiTarget.lastModified().toString();
  ui->lblSourceLastModi->setText(tr("Source file last modified") + " : " +
                                 sourceModi + "\nmd5    " + sourceHash);
  ui->lblTargetLastModi->setText(tr("Target file last modified") + " : " +
                                 targetModi + "\nmd5    " + targetHash);

  if (sourceHash != targetHash) {
    ui->listTarget->item(currentRow)->setBackgroundColor(QColor(Qt::red));
    ui->listTarget->item(currentRow)->setForeground(QBrush(Qt::white));
  } else if (mw_one->red > 55)
    ui->listTarget->item(currentRow)->setBackgroundColor(Qt::white);
}
