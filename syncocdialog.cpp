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
  QString listStyleMain =
      "QListWidget{outline:0px;}"
      "QListWidget::item:selected{background:blue; border:0px "
      "blue;margin:1px,1px,1px,1px;border-radius:6;"
      "color:white}";
  ui->listSource->setStyleSheet(listStyleMain);
  ui->listTarget->setStyleSheet(listStyleMain);
  ui->listSource->setIconSize(QSize(15, 15));
  ui->listTarget->setIconSize(QSize(15, 15));
}

SyncOCDialog::~SyncOCDialog() { delete ui; }

void SyncOCDialog::on_btnStartSync_clicked() {
  bool ok = true;
  for (int i = 0; i < ui->listSource->count(); i++) {
    ui->listSource->setCurrentRow(i);
    ui->listTarget->setCurrentRow(i);

    // 数据库里面必须要有这个文件（源文件必须存在）
    QString strSou = ui->listSource->item(i)->text();
    QString strTar = ui->listTarget->item(i)->text();
    if (QFileInfo(strSou).exists()) {
      if (ui->listSource->item(i)->checkState() == Qt::Checked) {
        if (!mymethod->isKext(strSou)) {
          QFile::remove(strTar);
          ok = QFile::copy(strSou, strTar);
        } else {
          mw_one->copyDirectoryFiles(strSou, strTar, true);
        }
      }
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

  QString strShowFileName;
  if (!mymethod->isKext(sourceFile))
    strShowFileName = fiSource.fileName();
  else
    strShowFileName = fiSource.fileName() + "    " +
                      mymethod->getKextVersion(sourceFile) + "  ->  " +
                      mymethod->getKextVersion(targetFile);

  ui->lblSourceLastModi->setText(strShowFileName + "\n\n" +
                                 tr("Source file last modified") + " : " +
                                 sourceModi + "\nmd5    " + sourceHash);
  ui->lblTargetLastModi->setText(tr("Target file last modified") + " : " +
                                 targetModi + "\nmd5    " + targetHash);

  if (sourceHash != targetHash) {
    ui->listTarget->item(currentRow)->setIcon(QIcon(":/icon/no.png"));
    ui->listSource->item(currentRow)->setIcon(QIcon(":/icon/no.png"));

    blSame = false;
  } else {
    ui->listTarget->item(currentRow)->setIcon(QIcon(":/icon/ok.png"));
    ui->listSource->item(currentRow)->setIcon(QIcon(":/icon/ok.png"));
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

void SyncOCDialog::on_listTarget_itemClicked(QListWidgetItem* item) {
  Q_UNUSED(item);
}
