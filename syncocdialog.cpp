#include "syncocdialog.h"

#include "Method.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_syncocdialog.h"

extern MainWindow* mw_one;
extern QString ocVer;
extern Method* mymethod;
extern QString SaveFileName;

SyncOCDialog::SyncOCDialog(QWidget* parent)
    : QDialog(parent), ui(new Ui::SyncOCDialog) {
  ui->setupUi(this);
  setWindowTitle(tr("Sync OC") + " -> " + ocVer);
  QString listStyleMain =
      "QListWidget{outline:0px;}"
      "QListWidget::item:selected{background:rgb(0,124,221); border:0px "
      "blue;margin:1px,1px,1px,1px;border-radius:6;"
      "color:white}";

  ui->listSource->setStyleSheet(listStyleMain);
  ui->listTarget->setStyleSheet(listStyleMain);
  int size = 25;
  ui->listSource->setIconSize(QSize(15, 15));
  ui->listTarget->setIconSize(QSize(15, 15));
  ui->listSource->setGridSize(QSize(size, size));
  ui->listTarget->setGridSize(QSize(size, size));

  ui->label->setFixedHeight(40);
  ui->label->setFixedWidth(40);
  ui->label->setText("");

  ui->label->setStyleSheet(
      "QLabel{"
      "border-image:url(:/icon/tip.png) 4 4 4 4 stretch stretch;"
      "}");
  ui->btnStartSync->setDefault(true);
  ui->labelShowDLInfo->setVisible(false);
  ui->labelShowDLInfo->setText("");
  ui->progressBarKext->setHidden(true);
  ui->btnUpdate->setEnabled(false);

  ui->listSource->setViewMode(QListView::ListMode);
  ui->listSource->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

  ui->listTarget->setViewMode(QListView::ListMode);
  ui->listTarget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
}

SyncOCDialog::~SyncOCDialog() { delete ui; }

void SyncOCDialog::on_btnStartSync_clicked() {
  if (!ui->btnUpKexts->isEnabled()) return;
  bool ok = true;
  // Kexts
  for (int i = 0; i < mw_one->sourceKexts.count(); i++) {
    QString strSou = mw_one->sourceKexts.at(i);
    QString strTar = mw_one->targetKexts.at(i);
    QString strSV, strTV;
    strSV = mymethod->getKextVersion(strSou);
    strTV = mymethod->getKextVersion(strTar);
    if (QDir(strSou).exists()) {
      if (ui->listSource->item(i)->checkState() == Qt::Checked) {
        if (strSV >= strTV) {
          mw_one->copyDirectoryFiles(strSou, strTar, true);
        }
      }
    }
  }

  // OpenCore
  for (int i = 0; i < mw_one->sourceOpenCore.count(); i++) {
    QString strSou = mw_one->sourceOpenCore.at(i);
    QString strTar = mw_one->targetOpenCore.at(i);
    if (QFile(strSou).exists()) {
      if (ui->listTarget->item(i)->checkState() == Qt::Checked) {
        QFile::remove(strTar);
        ok = QFile::copy(strSou, strTar);
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
    mw_one->checkFiles();
  }
}

void SyncOCDialog::on_listSource_currentRowChanged(int currentRow) {
  if (currentRow < 0) return;

  ui->listSource->item(ui->listSource->currentRow())
      ->setForeground(QBrush(Qt::black));
  ui->listSource->item(ui->listSource->currentRow())
      ->setBackground(QBrush(QColor("#FFD39B")));

  QString sourceModi, targetModi, sourceFile, targetFile, sourceHash,
      targetHash;

  sourceFile = mw_one->sourceKexts.at(currentRow);
  targetFile = mw_one->targetKexts.at(currentRow);

  if (mymethod->isKext(sourceFile))
    sourceHash = mw_one->getMD5(mymethod->getKextBin(sourceFile));
  else
    sourceHash = mw_one->getMD5(sourceFile);
  if (mymethod->isKext(targetFile))
    targetHash = mw_one->getMD5(mymethod->getKextBin(targetFile));
  else
    targetHash = mw_one->getMD5(targetFile);

  QFileInfo fiSource(sourceFile);
  QFileInfo fiTarget(targetFile);
  sourceModi = fiSource.lastModified().toString();
  targetModi = fiTarget.lastModified().toString();

  QString strShowFileName, strSV, strTV;
  if (!mymethod->isKext(sourceFile))
    strShowFileName = fiSource.fileName();
  else {
    strShowFileName = fiSource.fileName();
    strSV = mymethod->getKextVersion(sourceFile);
    strTV = mymethod->getKextVersion(targetFile);
  }

  ui->lblTargetLastModi->setText(strShowFileName + "\n" + tr("Current File: ") +
                                 "\n" + strTV + "  md5    " + targetHash);
  ui->lblSourceLastModi->setText(tr("Available File: ") + "\n" + strSV +
                                 "  md5    " + sourceHash);
  bool defUS = false;
  mw_one->myDatabase->refreshKextUrl();
  for (int i = 0; i < mw_one->myDatabase->ui->tableKextUrl->rowCount(); i++) {
    QString str =
        mw_one->myDatabase->ui->tableKextUrl->item(i, 0)->text().trimmed();
    QString str1 = QFileInfo(targetFile).fileName();
    if (str1 == str || str1.mid(0, 3) == "SMC" || str1.contains("AppleALC") ||
        str1.contains("BlueTool") || str1.contains("VoodooPS2Controller") ||
        str1.contains("VoodooI2C") || str1.contains("Brcm") ||
        str1.contains("IntelSnowMausi"))
      defUS = true;
  }

  if (!defUS) {
    ui->listSource->item(currentRow)->setIcon(QIcon(":/icon/nous.png"));
    QString str = strShowFileName + "  |  " + strTV;
    ui->listSource->item(currentRow)->setText(str);
  } else {
    if (!QFile(sourceFile).exists()) {
      ui->listSource->item(currentRow)->setIcon(QIcon(":/icon/ok.png"));
      QString str = strShowFileName + "  |  " + strTV;
      ui->listSource->item(currentRow)->setText(str);
    }
    if (!QFile(targetFile).exists()) {
      ui->listSource->item(currentRow)->setIcon(QIcon(":/icon/no.png"));
      QString str = strShowFileName + "  |  " + strTV;
      ui->listSource->item(currentRow)->setText(str);
    }
    if (QFile(sourceFile).exists() && QFile(targetFile).exists()) {
      if (strSV > strTV) {
        ui->listSource->item(currentRow)->setIcon(QIcon(":/icon/no.png"));
        QString str = strShowFileName + "  |  " + strTV + "  |  " + strSV;
        ui->listSource->item(currentRow)->setText(str);
      } else {
        ui->listSource->item(currentRow)->setIcon(QIcon(":/icon/ok.png"));
        QString str = strShowFileName + "  |  " + strTV;
        ui->listSource->item(currentRow)->setText(str);
      }
    }
  }

  if (sourceHash != targetHash) {
  } else {
  }
}

void SyncOCDialog::on_listSource_itemClicked(QListWidgetItem* item) {
  Q_UNUSED(item);
}

void SyncOCDialog::on_listTarget_itemClicked(QListWidgetItem* item) {
  Q_UNUSED(item);
}

void SyncOCDialog::setListWidgetStyle() {
  QString fileName = mw_one->sourceOpenCore.at(ui->listTarget->currentRow());
  if (mymethod->isWhatFile(fileName, "efi")) {
    setListWidgetColor("#E0EEEE");
  }
  if (mymethod->isWhatFile(fileName, "efi") && fileName.contains("/Tools/")) {
    setListWidgetColor("#FFEFDB");
  }
  if (mymethod->isWhatFile(fileName, "efi") && fileName.contains("/Drivers/")) {
    setListWidgetColor("#F5F5DC");
  }
}

void SyncOCDialog::setListWidgetColor(QString color) {
  ui->listTarget->item(ui->listTarget->currentRow())
      ->setForeground(QBrush(Qt::black));
  ui->listTarget->item(ui->listTarget->currentRow())
      ->setBackground(QBrush(QColor(color)));
}

void SyncOCDialog::on_listTarget_currentRowChanged(int currentRow) {
  if (currentRow < 0) return;

  setListWidgetStyle();

  QString sourceModi, targetModi, sourceFile, targetFile, sourceHash,
      targetHash;

  sourceFile = mw_one->sourceOpenCore.at(currentRow);
  targetFile = mw_one->targetOpenCore.at(currentRow);

  if (mymethod->isKext(sourceFile))
    sourceHash = mw_one->getMD5(mymethod->getKextBin(sourceFile));
  else
    sourceHash = mw_one->getMD5(sourceFile);
  if (mymethod->isKext(targetFile))
    targetHash = mw_one->getMD5(mymethod->getKextBin(targetFile));
  else
    targetHash = mw_one->getMD5(targetFile);

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

  ui->lblSourceLastModi_2->setText(tr("Available File: ") + "md5    " +
                                   sourceHash);
  ui->lblTargetLastModi_2->setText(
      strShowFileName + "\n\n" + tr("Current File: ") + "md5    " + targetHash);

  if (sourceHash != targetHash) {
    ui->listTarget->item(currentRow)->setIcon(QIcon(":/icon/no.png"));

  } else {
    ui->listTarget->item(currentRow)->setIcon(QIcon(":/icon/ok.png"));
  }
}

void SyncOCDialog::closeEvent(QCloseEvent* event) {
  Q_UNUSED(event);
  QString qfile = QDir::homePath() + "/.config/QtOCC/QtOCC.ini";
  // QString strTag = QDir::fromNativeSeparators(SaveFileName);
  QString strTag = SaveFileName;
  strTag.replace("/", "-");
  QString str_0, str_1;
  QSettings Reg(qfile, QSettings::IniFormat);
  for (int i = 0; i < ui->listSource->count(); i++) {
    str_0 = ui->listSource->item(i)->text().trimmed();
    QStringList list_0 = str_0.split("|");
    if (list_0.count() > 0) str_1 = list_0.at(0);
    Reg.setValue(strTag + str_1.trimmed(),
                 ui->listSource->item(i)->checkState());
  }
  for (int i = 0; i < ui->listTarget->count(); i++) {
    Reg.setValue(strTag + ui->listTarget->item(i)->text().trimmed(),
                 ui->listTarget->item(i)->checkState());
  }
}

void SyncOCDialog::on_btnUpKexts_clicked() {
  ui->btnUpdate->setEnabled(false);
  repaint();
  mymethod->kextUpdate();

  int n = ui->listSource->currentRow();
  for (int i = 0; i < ui->listSource->count(); i++) {
    ui->listSource->setCurrentRow(i);
    if (ui->listSource->item(i)->checkState() == Qt::Checked) {
      QString sourceFile = mw_one->sourceKexts.at(i);
      QString targetFile = mw_one->targetKexts.at(i);
      QString strSV, strTV;
      strSV = mymethod->getKextVersion(sourceFile);
      strTV = mymethod->getKextVersion(targetFile);
      if (strSV > strTV || strTV == "None") {
        ui->btnUpdate->setEnabled(true);
        repaint();
      }
    }
  }
  ui->listSource->setCurrentRow(n);
}

void SyncOCDialog::on_btnStop_clicked() { mymethod->cancelKextUpdate(); }

void SyncOCDialog::on_btnUpdate_clicked() {
  mymethod->finishKextUpdate(false);
  ui->btnUpdate->setEnabled(false);
  repaint();

  int n = ui->listSource->currentRow();
  for (int i = 0; i < ui->listSource->count(); i++) {
    ui->listSource->setCurrentRow(i);
  }
  ui->listSource->setCurrentRow(n);
}

void SyncOCDialog::on_btnSelectAll_clicked() {
  for (int i = 0; i < ui->listSource->count(); i++)
    ui->listSource->item(i)->setCheckState(Qt::Checked);
}

void SyncOCDialog::on_btnClearAll_clicked() {
  for (int i = 0; i < ui->listSource->count(); i++)
    ui->listSource->item(i)->setCheckState(Qt::Unchecked);
}
