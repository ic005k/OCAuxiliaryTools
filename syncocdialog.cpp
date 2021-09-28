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

  QString sourceModi, targetModi;
  ui->listTarget->setCurrentRow(currentRow);

  QFileInfo fiSource(ui->listSource->item(currentRow)->text());
  QFileInfo fiTarget(ui->listTarget->item(currentRow)->text());
  sourceModi = fiSource.lastModified().toString();
  targetModi = fiTarget.lastModified().toString();
  ui->lblSourceLastModi->setText(tr("Source file last modified") + " : " +
                                 sourceModi);
  ui->lblTargetLastModi->setText(tr("Target file last modified") + " : " +
                                 targetModi);

  if (sourceModi != targetModi && !mw_one->osx1012) {
    ui->listTarget->item(currentRow)->setBackgroundColor(QColor(Qt::red));
    ui->listTarget->item(currentRow)->setForeground(QBrush(Qt::white));
  } else if (mw_one->red > 55)
    ui->listTarget->item(currentRow)->setBackgroundColor(Qt::white);
}
