
#include "commands.h"

#include "Method.h"

extern MainWindow* mw_one;
extern Method* mymethod;

DeleteCommand::DeleteCommand(bool writeINI, bool loadINI, QTableWidget* table0,
                             int table0CurrentRow, QTableWidget* table, int row,
                             QString text, QStringList fieldList,
                             QUndoCommand* parent) {
  Q_UNUSED(parent);
  m_table = table;
  m_table0 = table0;
  m_row = row;
  m_table0CurrentRow = table0CurrentRow;
  m_text = text;

  m_fieldList = fieldList;
  m_loadINI = loadINI;
  m_writeINI = writeINI;

  setText(QObject::tr("Delete") + "  " + text);
}

void DeleteCommand::undo() {
  mw_one->clearAllTableSelection();

  mymethod->goTable(m_table);

  if (m_table0 != NULL && !m_loadINI) {
    m_table0->setCurrentCell(m_table0CurrentRow, 0);
  }

  mw_one->loading = true;

  m_table->insertRow(m_row);

  for (int i = 0; i < m_fieldList.count(); i++) {
    QString strItem = m_fieldList.at(i);

    if (strItem.trimmed() == "true" || strItem.trimmed() == "false") {
      mw_one->init_enabled_data(m_table, m_row, i, strItem.trimmed());

    } else {
      m_table->setItem(m_row, i, new QTableWidgetItem(strItem));
    }
  }

  m_table->setCurrentCell(m_row, 0);

  if (m_loadINI) {
    if (m_table == mw_one->ui->table_dp_add0 ||
        m_table == mw_one->ui->table_nv_add0)
      mw_one->readLeftTable(m_table, m_table0);

    if (m_table == mw_one->ui->table_dp_del0 ||
        m_table == mw_one->ui->table_nv_del0 ||
        m_table == mw_one->ui->table_nv_ls0)
      mw_one->readLeftTableOnlyValue(m_table, m_table0);
  }

  if (m_table0 != NULL && !m_loadINI) {
    if (m_writeINI)
      mymethod->writeLeftTable(m_table0, m_table);
    else
      mymethod->writeLeftTableOnlyValue(m_table0, m_table);
  }

  mw_one->loading = false;

  mw_one->checkFiles(m_table);
}

void DeleteCommand::redo() {
  mymethod->goTable(m_table);

  if (m_table0 != NULL && !m_loadINI) {
    m_table0->setCurrentCell(m_table0CurrentRow, 0);
  }

  if (m_loadINI) {
    mw_one->readLeftTable(m_table, m_table0);
  }

  mw_one->loading = true;

  m_table->removeRow(m_row);

  if (m_table0 != NULL) {
    mw_one->endDelLeftTable(m_table);
  }

  if (m_table0 != NULL && !m_loadINI) {
    if (m_writeINI)
      mymethod->writeLeftTable(m_table0, m_table);
    else
      mymethod->writeLeftTableOnlyValue(m_table0, m_table);
  }

  mw_one->loading = false;

  mw_one->checkFiles(m_table);
}

AddCommand::AddCommand(QTableWidget* table, int row, int col, QString text,
                       QUndoCommand* parent) {
  Q_UNUSED(parent);
  m_table = table;
  m_row = row;
  m_col = col;
  m_text = text;

  setText(QObject::tr("Add") + "  " + text);
}

AddCommand::~AddCommand() {}

void AddCommand::undo() {
  m_table->setItem(m_row, m_col, new QTableWidgetItem(""));
}

void AddCommand::redo() {
  m_table->setItem(m_row, m_col, new QTableWidgetItem(m_text));
}

// Edit
EditCommand::EditCommand(bool textAlignCenter, QString oldText,
                         QTableWidget* table, int row, int col, QString text,
                         QUndoCommand* parent) {
  Q_UNUSED(parent);
  m_table = table;
  m_row = row;
  m_col = col;
  m_text = text;
  m_oldText = oldText;
  m_textAlignCenter = textAlignCenter;

  setText(QObject::tr("Edit") + "  " + oldText);
}

EditCommand::~EditCommand() {}

void EditCommand::undo() {
  mymethod->goTable(m_table);

  mw_one->initLineEdit(m_table, m_row, m_col, m_row, m_col);

  mw_one->loading = true;
  mw_one->writeINI = true;

  QTableWidgetItem* item = new QTableWidgetItem(m_oldText);
  if (m_textAlignCenter) item->setTextAlignment(Qt::AlignCenter);
  m_table->setItem(m_row, m_col, item);
  m_table->setCurrentCell(m_row, m_col);

  m_table->setCellWidget(m_row, m_col, mw_one->lineEdit);
  mw_one->lineEdit->setText(m_oldText);

  mw_one->loading = false;
  mw_one->writeINI = false;

  mw_one->lineEditModifyed = false;
}

void EditCommand::redo() {
  mymethod->goTable(m_table);

  mw_one->initLineEdit(m_table, m_row, m_col, m_row, m_col);

  mw_one->loading = true;
  mw_one->writeINI = true;

  QTableWidgetItem* item = new QTableWidgetItem(m_text);
  if (m_textAlignCenter) item->setTextAlignment(Qt::AlignCenter);
  m_table->setItem(m_row, m_col, item);

  if (mw_one->lineEdit != NULL) {
    m_table->setCurrentCell(m_row, m_col);

    m_table->setCellWidget(m_row, m_col, mw_one->lineEdit);

    mw_one->lineEdit->setText(m_text);
  }

  mw_one->loading = false;
  mw_one->writeINI = false;

  mw_one->lineEditModifyed = false;
}

// CopyPasteLine
CopyPasteLineCommand::CopyPasteLineCommand(
    QTableWidget* table, int row, int col, QString text,
    QStringList colTextList, QString oldColText0, bool writeini,
    bool writevalueini, int leftTableCurrentRow, QUndoCommand* parent) {
  Q_UNUSED(parent);
  m_table = table;
  m_row = row;
  m_col = col;
  m_text = text;
  m_colTextList = colTextList;
  m_oldColText0 = oldColText0;
  m_writeini = writeini;
  m_writevalueini = writevalueini;
  m_leftTableCurrentRow = leftTableCurrentRow;

  setText(QObject::tr("Paste Line") + "  " + text);
}

CopyPasteLineCommand::~CopyPasteLineCommand() {}

void CopyPasteLineCommand::undo() {
  if (m_writeini || m_writevalueini)
    mw_one->getLeftTable(m_table)->setCurrentCell(m_leftTableCurrentRow, 0);

  m_table->removeRow(m_row);

  mw_one->endDelLeftTable(m_table);

  mymethod->goTable(m_table);

  if (m_writeini) {
    mymethod->writeLeftTable(mw_one->getLeftTable(m_table), m_table);
  }

  if (m_writevalueini) {
    mymethod->writeLeftTableOnlyValue(mw_one->getLeftTable(m_table), m_table);
  }

  mw_one->checkFiles(m_table);
}

void CopyPasteLineCommand::redo() {
  if (m_writeini || m_writevalueini)
    mw_one->getLeftTable(m_table)->setCurrentCell(m_leftTableCurrentRow, 0);

  m_table->insertRow(m_row);

  for (int i = 0; i < m_colTextList.count(); i++) {
    QString colText = m_colTextList.at(i);
    if (colText == "false" || colText == "true") {
      mw_one->init_enabled_data(m_table, m_row, i, colText);

    } else {
      m_table->setItem(m_row, i, new QTableWidgetItem(colText));
    }
  }

  mw_one->endPasteLine(m_table, m_row, m_oldColText0);

  mymethod->goTable(m_table);

  if (m_writeini) {
    mymethod->writeLeftTable(mw_one->getLeftTable(m_table), m_table);
  }

  if (m_writevalueini) {
    mymethod->writeLeftTableOnlyValue(mw_one->getLeftTable(m_table), m_table);
  }

  mw_one->checkFiles(m_table);
}

QString createCommandString(QString cmdStr) { return cmdStr; }
