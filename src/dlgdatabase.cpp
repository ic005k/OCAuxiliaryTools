#include "dlgdatabase.h"

#include "Method.h"
#include "mainwindow.h"
#include "ui_dlgdatabase.h"

extern MainWindow *mw_one;
extern Method *mymethod;
extern QString SaveFileName, strIniFile, strAppName, strAppExePath;
extern bool blDEV;
extern int red;

dlgDatabase::dlgDatabase(QWidget *parent)
    : QDialog(parent), ui(new Ui::dlgDatabase) {
  ui->setupUi(this);
  ui->btnAMD->hide();
  ui->btnAMDOnline->hide();

  Method::init_UIWidget(this, red);
  processPing = new QProcess;
  connect(processPing, SIGNAL(readyReadStandardOutput()), this,
          SLOT(on_readoutput()));
  connect(processPing, SIGNAL(readyReadStandardError()), this,
          SLOT(on_readerror()));

  ui->editFind->setClearButtonEnabled(true);

  // tableDatabase->setEditTriggers(QAbstractItemView::NoEditTriggers);

  QTableWidgetItem *id0;
  ui->tableDatabase->setColumnCount(2);
  ui->tableDatabase->setColumnWidth(0, 400);
  id0 = new QTableWidgetItem(tr("Configs"));
  ui->tableDatabase->setHorizontalHeaderItem(0, id0);
  ui->tableDatabase->setColumnWidth(1, 220);
  id0 = new QTableWidgetItem(tr("Comment"));
  ui->tableDatabase->setHorizontalHeaderItem(1, id0);

  ui->tableDatabase->setAlternatingRowColors(true);
  ui->tableDatabase->horizontalHeader()->setStretchLastSection(true);
  ui->tableDatabase->horizontalHeader()->setHidden(false);
  ui->tableDatabase->setSelectionBehavior(
      QAbstractItemView::SelectItems);  //设置选择行为时每次选择一行或单个条目

  for (int i = 0; i < ui->tableDatabase->columnCount(); i++) {
    ui->tableDatabase->horizontalHeader()->setSectionResizeMode(
        i, QHeaderView::ResizeToContents);
  }

  // Find Table
  ui->tableDatabaseFind->horizontalHeader()->setHidden(true);
  ui->tableDatabaseFind->setAlternatingRowColors(true);
  ui->tableDatabaseFind->setHidden(true);
  ui->tableDatabaseFind->horizontalHeader()->setStretchLastSection(true);
  ui->tableDatabaseFind->setEditTriggers(QAbstractItemView::NoEditTriggers);
  ui->tableDatabaseFind->horizontalHeader()->setSectionResizeMode(
      0, QHeaderView::ResizeToContents);
}

dlgDatabase::~dlgDatabase() { delete ui; }

void dlgDatabase::init_Database(QStringList files) {
  ui->tableDatabase->setRowCount(0);
  ui->tableDatabase->setRowCount(files.count());

  QString dirpath = strAppExePath + "/Database/BaseConfigs/";
  for (int i = 0; i < files.count(); i++) {
    QTableWidgetItem *newItem1;
    newItem1 = new QTableWidgetItem(files.at(i));
    ui->tableDatabase->setItem(i, 0, newItem1);

    newItem1 =
        new QTableWidgetItem(mymethod->readPlistComment(dirpath + files.at(i)));
    ui->tableDatabase->setItem(i, 1, newItem1);
  }

  listItemModi.clear();
  for (int i = 0; i < files.count(); i++) {
    listItemModi.append(false);
  }

  setWindowTitle(tr("Configuration file database") + " : " +
                 mw_one->getDatabaseVer());
}

void dlgDatabase::closeEvent(QCloseEvent *event) {
  Q_UNUSED(event);

  QFileInfo appInfo(qApp->applicationDirPath());
  QString dirpath = appInfo.filePath() + "/Database/BaseConfigs/";
  for (int i = 0; i < ui->tableDatabase->rowCount(); i++) {
    QString plistFile =
        dirpath + ui->tableDatabase->item(i, 0)->text().trimmed();
    if (listItemModi.at(i))
      mymethod->writePlistComment(
          plistFile, ui->tableDatabase->item(i, 1)->text().trimmed());
  }
}

void dlgDatabase::keyPressEvent(QKeyEvent *event) {
  switch (event->key()) {
    case Qt::Key_Escape:
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

  if (event->modifiers() == Qt::ControlModifier) {
    if (event->key() == Qt::Key_M) {
      this->setWindowState(Qt::WindowMaximized);
    }
  }
}

void dlgDatabase::on_tableDatabase_cellDoubleClicked(int row, int column) {
  get_EFI(row, column, ui->tableDatabase);
}

void dlgDatabase::get_EFI(int row, int column, QTableWidget *table) {
  if (column != 0) return;

  QString dirpath = strAppExePath + "/Database/BaseConfigs/";
  QString file = table->item(row, 0)->text();

  if (blDEV) {
    if (file == "SampleCustom.plist" || file == "Sample.plist") {
      dirpath = mw_one->userDataBaseDir + "BaseConfigs/";
    }
  }

  mw_one->isGetEFI = true;
  mw_one->openFile(dirpath + file);
  mw_one->isGetEFI = false;

  close();

  mymethod->generateEFI(file);
}

void dlgDatabase::on_btnFind_clicked() {
  QString text = ui->editFind->text().trimmed();
  if (text == "") return;

  ui->tableDatabase->setFocus();
  // tableDatabase->setSelectionMode(QAbstractItemView::MultiSelection);
  // tableDatabase->clearSelection();
  ui->tableDatabaseFind->setRowCount(0);
  int count = 0;
  for (int i = 0; i < ui->tableDatabase->rowCount(); i++) {
    QString str = ui->tableDatabase->item(i, 0)->text();
    QString str1 = str;
    str1.replace(".plist", "");

    if (str1.toLower().contains(text.toLower())) {
      ui->tableDatabaseFind->setRowCount(count + 1);
      QTableWidgetItem *newItem1;
      newItem1 = new QTableWidgetItem(str);
      ui->tableDatabaseFind->setItem(count, 0, newItem1);

      count++;

    } else {
      // tableDatabase->selectRow(i);
    }
  }

  if (count > 0)
    ui->tableDatabaseFind->setHidden(false);
  else
    ui->tableDatabaseFind->setHidden(true);

  ui->editFind->setFocus();

  ui->lblCount->setText(QString::number(count));

  // tableDatabase->setSelectionMode(QAbstractItemView::ExtendedSelection);
}

void dlgDatabase::on_editFind_textChanged(const QString &arg1) {
  if (arg1 == "") {
    ui->lblCount->setText("0");
    ui->tableDatabaseFind->setHidden(true);
    return;
  }

  on_btnFind_clicked();
}

void dlgDatabase::on_editFind_returnPressed() { on_btnFind_clicked(); }

void dlgDatabase::on_tableDatabaseFind_cellDoubleClicked(int row, int column) {
  get_EFI(row, column, ui->tableDatabaseFind);
}

void dlgDatabase::on_readoutput() {
  // QString str = processPing->readAllStandardOutput();
}

void dlgDatabase::on_readerror() {
  QMessageBox::information(0, "Error", processPing->readAllStandardError());
}

void dlgDatabase::on_tableDatabase_itemDoubleClicked(QTableWidgetItem *item) {
  Q_UNUSED(item);
}

void dlgDatabase::on_btnIntel_clicked() {
  QString qtManulFile =
      strAppExePath + "/Database/BaseConfigs/Instructions_Intel.md";
  QDesktopServices::openUrl(QUrl::fromLocalFile(qtManulFile));
}

void dlgDatabase::on_btnAMD_clicked() {
  QString qtManulFile =
      strAppExePath + "/Database/BaseConfigs/Instructions_AMD.md";
  QDesktopServices::openUrl(QUrl::fromLocalFile(qtManulFile));
}

void dlgDatabase::on_btnOpenDir_clicked() {
  QString dirpath = strAppExePath + "/Database/BaseConfigs/";
  QString dir = "file:" + dirpath;
  QDesktopServices::openUrl(QUrl(dir, QUrl::TolerantMode));
}

void dlgDatabase::on_btnIntelOnline_clicked() {
  QUrl url(QString("https://github.com/ic005k/" + strAppName +
                   "/blob/master/Database/"
                   "BaseConfigs/Instructions_Intel.md"));
  QDesktopServices::openUrl(url);
}

void dlgDatabase::on_btnAMDOnline_clicked() {
  QUrl url(QString("https://github.com/ic005k/" + strAppName +
                   "/blob/master/Database/"
                   "BaseConfigs/Instructions_AMD.md"));
  QDesktopServices::openUrl(url);
}

void dlgDatabase::on_btnGenerateEFI_clicked() {
  if (ui->tableDatabase->hasFocus()) {
    if (!ui->tableDatabase->currentIndex().isValid()) return;
    int row = ui->tableDatabase->currentRow();
    on_tableDatabase_cellDoubleClicked(row, 0);
  }

  if (ui->tableDatabaseFind->hasFocus()) {
    if (!ui->tableDatabaseFind->currentIndex().isValid()) return;
    int row = ui->tableDatabaseFind->currentRow();
    on_tableDatabaseFind_cellDoubleClicked(row, 0);
  }
}

void dlgDatabase::on_tableDatabase_currentItemChanged(
    QTableWidgetItem *current, QTableWidgetItem *previous) {
  Q_UNUSED(current);
  Q_UNUSED(previous);
  if (ui->tableDatabase->currentColumn() == 1) {
    int row = ui->tableDatabase->currentRow();
    listItemModi.removeAt(row);
    listItemModi.insert(row, true);
  }
}
