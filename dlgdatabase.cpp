#include "dlgdatabase.h"
#include "mainwindow.h"
#include "ui_dlgdatabase.h"

extern QTableWidget* tableDatabase;
extern MainWindow* mw_one;
extern QString SaveFileName;

dlgDatabase::dlgDatabase(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::dlgDatabase)
{
    ui->setupUi(this);

    ui->editFind->setClearButtonEnabled(true);

    tableDatabase = ui->tableDatabase;
    tableDatabase->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableDatabaseFind->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QTableWidgetItem* id0;

    ui->tableDatabase->setColumnWidth(0, 520);
    id0 = new QTableWidgetItem(tr("Config Database"));
    ui->tableDatabase->setHorizontalHeaderItem(0, id0);

    ui->tableDatabase->setAlternatingRowColors(true);
    tableDatabase->horizontalHeader()->setStretchLastSection(true); //设置充满表宽度
    ui->tableDatabaseFind->horizontalHeader()->setStretchLastSection(true);
    tableDatabase->horizontalHeader()->setHidden(true);
    ui->tableDatabaseFind->horizontalHeader()->setHidden(true);
    ui->tableDatabaseFind->setHidden(true);

    tableDatabase->setSelectionBehavior(QAbstractItemView::SelectRows); //设置选择行为时每次选择一行
}

dlgDatabase::~dlgDatabase()
{
    delete ui;
}

void dlgDatabase::on_tableDatabase_cellDoubleClicked(int row, int column)
{
    Q_UNUSED(row);
    Q_UNUSED(column);

    mw_one->RefreshAllDatabase = true;

    QFileInfo appInfo(qApp->applicationDirPath());

    QString dirpath = appInfo.filePath() + "/Database/";
    QString file = tableDatabase->currentItem()->text();
    mw_one->openFile(dirpath + file);
    mw_one->on_GenerateEFI();

    mw_one->RefreshAllDatabase = false;
}

void dlgDatabase::on_btnFind_clicked()
{
    QString text = ui->editFind->text().trimmed();
    if (text == "")
        return;

    tableDatabase->setFocus();
    tableDatabase->setSelectionMode(QAbstractItemView::MultiSelection);
    tableDatabase->clearSelection();
    ui->tableDatabaseFind->setRowCount(0);
    int count = 0;
    for (int i = 0; i < tableDatabase->rowCount(); i++) {
        tableDatabase->setCurrentCell(i, 0);
        QString str = tableDatabase->currentItem()->text();
        QFileInfo fi(str);
        if (fi.baseName().toLower().contains(text.toLower())) {
            ui->tableDatabaseFind->setRowCount(count + 1);
            QTableWidgetItem* newItem1;
            newItem1 = new QTableWidgetItem(str);
            ui->tableDatabaseFind->setItem(count, 0, newItem1);

            count++;

        } else {
            tableDatabase->selectRow(i);
        }
    }

    if (count > 0)
        ui->tableDatabaseFind->setHidden(false);
    else
        ui->tableDatabaseFind->setHidden(true);

    ui->editFind->setFocus();

    ui->lblCount->setText(QString::number(count));

    tableDatabase->setSelectionMode(QAbstractItemView::ExtendedSelection);
}

void dlgDatabase::on_editFind_textChanged(const QString& arg1)
{
    if (arg1 == "") {
        ui->lblCount->setText("0");
        ui->tableDatabaseFind->setHidden(true);
        return;
    }

    on_btnFind_clicked();
}

void dlgDatabase::on_editFind_returnPressed()
{
    on_btnFind_clicked();
}

void dlgDatabase::on_tableDatabaseFind_cellDoubleClicked(int row, int column)
{
    Q_UNUSED(row);
    Q_UNUSED(column);

    mw_one->RefreshAllDatabase = true;

    QFileInfo appInfo(qApp->applicationDirPath());

    QString dirpath = appInfo.filePath() + "/Database/";
    QString file = ui->tableDatabaseFind->currentItem()->text();
    mw_one->openFile(dirpath + file);

    mw_one->on_GenerateEFI();

    mw_one->RefreshAllDatabase = false;
}

void dlgDatabase::on_btnRefreshAll_clicked()
{
    ui->btnRefreshAll->setEnabled(false);
    this->repaint();

    mw_one->RefreshAllDatabase = true;

    QString bakFile;
    if (!SaveFileName.isEmpty()) {
        bakFile = SaveFileName;
    }

    ui->tableDatabase->setCurrentCell(0, 0);

    QFileInfo appInfo(qApp->applicationDirPath());
    QString dirpath = appInfo.filePath() + "/Database/";

    for (int i = 0; i < ui->tableDatabase->rowCount(); i++) {
        ui->tableDatabase->setCurrentCell(i, 0);
        QString file = ui->tableDatabase->currentItem()->text();
        mw_one->openFile(dirpath + file);
        mw_one->SavePlist(dirpath + file);
    }

    if (!bakFile.isEmpty())
        mw_one->openFile(bakFile);

    mw_one->RefreshAllDatabase = false;

    ui->btnRefreshAll->setEnabled(true);
    this->repaint();
}
