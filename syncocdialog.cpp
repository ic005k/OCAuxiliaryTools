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
      if (chkList.at(i)->isChecked()) {
        if (strSV >= strTV || strTV == "None") {
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

bool SyncOCDialog::eventFilter(QObject* o, QEvent* e) {
  if (o == lblVer && (e->type() == QEvent::MouseButtonPress ||
                      e->type() == QEvent::MouseButtonRelease)) {
    e->accept();
    return true;
  }

  return QWidget::eventFilter(o, e);
}

void SyncOCDialog::addVerWidget(int currentRow, QString strTV, QString strSV,
                                QString strShowFileName) {
  if (mw_one->mac || mw_one->osx1012)
    verList.at(currentRow)->setFont(QFont("Menlo", 12));
  if (mw_one->win) verList.at(currentRow)->setFont(QFont("consolas"));

  ui->listSource->item(currentRow)->setText("        " + strShowFileName);
  verList.at(currentRow)->setText(strTV + "    " + strSV + " ");

  QString strStyleSel = "QLabel {color: #e6e6e6;background-color: none;}";
  QString strStyle = "QLabel {color: #2c2c2c;background-color: none;}";
  for (int i = 0; i < ui->listSource->count(); i++) {
    if (i == currentRow)
      verList.at(currentRow)->setStyleSheet(strStyleSel);
    else
      verList.at(i)->setStyleSheet(strStyle);
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

  sourceHash = mw_one->getMD5(mymethod->getKextBin(sourceFile));
  targetHash = mw_one->getMD5(mymethod->getKextBin(targetFile));

  QFileInfo fiSource(sourceFile);
  QFileInfo fiTarget(targetFile);
  sourceModi = fiSource.lastModified().toString();
  targetModi = fiTarget.lastModified().toString();

  QString strShowFileName, strSV, strTV;
  strShowFileName = fiSource.fileName();
  strSV = mymethod->getKextVersion(sourceFile);
  strTV = mymethod->getKextVersion(targetFile);

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

  } else {
    if (!QFile(sourceFile).exists()) {
      ui->listSource->item(currentRow)->setIcon(QIcon(":/icon/ok.png"));
    }
    if (!QFile(targetFile).exists()) {
      ui->listSource->item(currentRow)->setIcon(QIcon(":/icon/no.png"));
    }
    if (QFile(sourceFile).exists() && QFile(targetFile).exists()) {
      if (strSV > strTV) {
        ui->listSource->item(currentRow)->setIcon(QIcon(":/icon/no.png"));

      } else {
        ui->listSource->item(currentRow)->setIcon(QIcon(":/icon/ok.png"));
      }
    }
  }

  addVerWidget(currentRow, strTV, strSV, strShowFileName);

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
  strShowFileName = fiSource.fileName();
  ui->lblTargetLastModi_2->setText(
      strShowFileName + "\n" + tr("Current File: ") + "md5    " + targetHash);
  ui->lblSourceLastModi_2->setText(tr("Available File: ") + "md5    " +
                                   sourceHash);

  if (sourceHash != targetHash) {
    ui->listTarget->item(currentRow)->setIcon(QIcon(":/icon/no.png"));

  } else {
    ui->listTarget->item(currentRow)->setIcon(QIcon(":/icon/ok.png"));
  }
}

void SyncOCDialog::closeEvent(QCloseEvent* event) {
  if (!ui->btnUpKexts->isEnabled()) event->ignore();
  writeCheckStateINI();
}

void SyncOCDialog::writeCheckStateINI() {
  QString qfile = QDir::homePath() + "/.config/QtOCC/chk.ini";
  // QString strTag = QDir::fromNativeSeparators(SaveFileName);
  QString strTag = SaveFileName;
  strTag.replace("/", "-");
  QString str_0;
  QSettings Reg(qfile, QSettings::IniFormat);
  for (int i = 0; i < ui->listSource->count(); i++) {
    str_0 = ui->listSource->item(i)->text().trimmed();
    Reg.setValue(strTag + str_0, chkList.at(i)->isChecked());
  }

  for (int i = 0; i < ui->listTarget->count(); i++) {
    Reg.setValue(strTag + ui->listTarget->item(i)->text().trimmed(),
                 ui->listTarget->item(i)->checkState());
  }
}

void SyncOCDialog::keyPressEvent(QKeyEvent* event) {
  switch (event->key()) {
    case Qt::Key_Escape:
      if (!ui->btnUpKexts->isEnabled())
        return;
      else
        close();
      break;

    case Qt::Key_Return:

      break;

    case Qt::Key_Backspace:

      break;

    case Qt::Key_Space:

      break;

    case Qt::Key_F1:

      break;
  }
}

void SyncOCDialog::on_btnUpKexts_clicked() {
  if (ui->listSource->count() == 0) return;
  ui->btnUpdate->setEnabled(false);
  repaint();

  ui->labelShowDLInfo->setVisible(true);
  progBar = new QProgressBar(this);
  progBar->setTextVisible(false);
  progBar->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  progBar->setStyleSheet(
      "QProgressBar{border:0px solid #FFFFFF;"
      "height:30;"
      "background:rgba(25,255,25,0);"
      "text-align:right;"
      "color:rgb(255,255,255);"
      "border-radius:6px;}"

      "QProgressBar:chunk{"
      "border-radius:6px;"
      "background-color:rgba(0,255,0,100);"
      "}");

  mymethod->kextUpdate();

  int n = ui->listSource->currentRow();
  for (int i = 0; i < ui->listSource->count(); i++) {
    if (chkList.at(i)->isChecked()) {
      QString sourceFile = mw_one->sourceKexts.at(i);
      QString targetFile = mw_one->targetKexts.at(i);
      QString strSV, strTV;
      strSV = mymethod->getKextVersion(sourceFile);
      strTV = mymethod->getKextVersion(targetFile);
      if ((strSV > strTV || strTV == "None") && QDir(sourceFile).exists()) {
        ui->btnUpdate->setEnabled(true);
        repaint();
      }
    }
  }

  for (int i = 0; i < ui->listSource->count(); i++) {
    if (ui->listSource->itemWidget(ui->listSource->item(i)) == progBar) {
      ui->listSource->removeItemWidget(ui->listSource->item(i));
      writeCheckStateINI();
      initKextList();
      readCheckStateINI();
    }
  }

  for (int i = 0; i < ui->listSource->count(); i++)
    ui->listSource->setCurrentRow(i);

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
    chkList.at(i)->setChecked(true);
}

void SyncOCDialog::on_btnClearAll_clicked() {
  for (int i = 0; i < ui->listSource->count(); i++)
    chkList.at(i)->setChecked(false);
}

void SyncOCDialog::resizeEvent(QResizeEvent* event) { Q_UNUSED(event); }

void SyncOCDialog::initKextList() {
  chkList.clear();
  textList.clear();
  verList.clear();

  QString strStyle = "QLabel {color: #2c2c2c;background-color: none;}";

  for (int i = 0; i < ui->listSource->count(); i++) {
    QWidget* w = new QWidget(this);
    QHBoxLayout* layout = new QHBoxLayout(w);
    layout->setMargin(0);
    layout->setDirection(QBoxLayout::LeftToRight);

    checkBox = new QCheckBox(w);
    chkList.append(checkBox);

    lblTxt = new QLabel(w);
    lblTxt->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    textList.append(lblTxt);

    lblVer = new QLabel(w);
    lblVer->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    lblVer->setStyleSheet(strStyle);
    verList.append(lblVer);

    layout->addSpacing(4);
    layout->addWidget(checkBox, 0, Qt::AlignLeft | Qt::AlignAbsolute);
    layout->addSpacing(4);
    layout->addWidget(lblTxt, 0, Qt::AlignLeft | Qt::AlignAbsolute);
    layout->addWidget(lblVer, 0, Qt::AlignRight | Qt::AlignAbsolute);

    w->setLayout(layout);
    ui->listSource->setItemWidget(ui->listSource->item(i), w);
  }
}

void SyncOCDialog::readCheckStateINI() {
  QString qfile = QDir::homePath() + "/.config/QtOCC/chk.ini";
  QString strTag = SaveFileName;
  strTag.replace("/", "-");
  QSettings Reg(qfile, QSettings::IniFormat);
  for (int i = 0; i < ui->listSource->count(); i++) {
    QString str_0 = ui->listSource->item(i)->text().trimmed();
    QString strValue = strTag + str_0;
    bool yes = false;
    for (int m = 0; m < Reg.allKeys().count(); m++) {
      if (Reg.allKeys().at(m).contains(strValue)) {
        yes = true;
      }
    }
    if (yes) {
      bool strCheck = Reg.value(strValue).toBool();
      chkList.at(i)->setChecked(strCheck);
    }
  }

  for (int i = 0; i < ui->listTarget->count(); i++) {
    QString strValue = strTag + ui->listTarget->item(i)->text().trimmed();
    bool yes = false;
    for (int m = 0; m < Reg.allKeys().count(); m++) {
      if (Reg.allKeys().at(m).contains(strValue)) {
        yes = true;
      }
    }
    if (yes) {
      int strCheck = Reg.value(strValue).toInt();
      if (strCheck == 2) ui->listTarget->item(i)->setCheckState(Qt::Checked);
      if (strCheck == 0) ui->listTarget->item(i)->setCheckState(Qt::Unchecked);
    }
  }
}
