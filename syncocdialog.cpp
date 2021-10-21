#include "syncocdialog.h"

#include "Method.h"
#include "mainwindow.h"
#include "ui_syncocdialog.h"

extern MainWindow* mw_one;
extern QString ocVer;
extern Method* mymethod;

SyncOCDialog::SyncOCDialog(QWidget* parent)
    : QDialog(parent), ui(new Ui::SyncOCDialog) {
  ui->setupUi(this);
  setWindowTitle(tr("Sync OC") + " -> " + ocVer);
}

SyncOCDialog::~SyncOCDialog() { delete ui; }

void SyncOCDialog::on_btnStartSync_clicked() {
  bool ok = true;
  for (int i = 0; i < ui->listSource->count(); i++) {
    ui->listSource->setCurrentRow(i);
    ui->listTarget->setCurrentRow(i);

    // 数据库里面必须要有这个文件（源文件必须存在）
    if (QFileInfo(ui->listSource->item(i)->text()).exists()) {
      QFile::remove(ui->listTarget->item(i)->text());

      ok = QFile::copy(ui->listSource->item(i)->text(),
                       ui->listTarget->item(i)->text());
    }
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

  if (mymethod->isKext(sourceFile))
    sourceHash = mymethod->getMD5(mymethod->getKextBin(sourceFile));
  else
    sourceHash = mymethod->getMD5(sourceFile);
  if (mymethod->isKext(targetFile))
    targetHash = mymethod->getMD5(mymethod->getKextBin(targetFile));
  else
    targetHash = mymethod->getMD5(targetFile);

  QFileInfo fiSource(sourceFile);
  QFileInfo fiTarget(targetFile);
  sourceModi = fiSource.lastModified().toString();
  targetModi = fiTarget.lastModified().toString();

  ui->lblSourceLastModi->setText(fiSource.fileName() + "\n\n" +
                                 tr("Source file last modified") + " : " +
                                 sourceModi + "\nmd5    " + sourceHash);
  ui->lblTargetLastModi->setText(tr("Target file last modified") + " : " +
                                 targetModi + "\nmd5    " + targetHash);

  if (sourceHash != targetHash) {
    // ui->listTarget->item(currentRow)->setBackgroundColor(QColor(Qt::red));
    // ui->listTarget->item(currentRow)->setForeground(QBrush(Qt::white));
    ui->listTarget->item(currentRow)->setIcon(QIcon(":/icon/red.svg"));
    ui->listSource->item(currentRow)->setIcon(QIcon(":/icon/red.svg"));

    blSame = false;
  } else {
    ui->listTarget->item(currentRow)->setIcon(QIcon(":/icon/green.svg"));
    ui->listSource->item(currentRow)->setIcon(QIcon(":/icon/green.svg"));
    // if (mw_one->red > 55)
    //  ui->listTarget->item(currentRow)->setBackgroundColor(Qt::white);
  }

  ui->listSource->setFocus();
  QScrollBar* scrollBar;
  scrollBar = ui->listTarget->verticalScrollBar();
  scrollBar->setValue(ui->listSource->verticalScrollBar()->value());
}

void SyncOCDialog::on_listSource_itemClicked(QListWidgetItem* item) {
  Q_UNUSED(item);
  on_listSource_currentRowChanged(ui->listSource->currentRow());
}
