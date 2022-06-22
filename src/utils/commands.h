/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef COMMANDS_H
#define COMMANDS_H

#include <QLineEdit>
#include <QUndoCommand>

#include "mainwindow.h"
#include "ui_mainwindow.h"

class DeleteCommand : public QUndoCommand {
 public:
  explicit DeleteCommand(bool writeINI, bool loadINI, QTableWidget* table0,
                         int table0CurrentRow, QTableWidget* table, int row,
                         QString text, QStringList fieldList,
                         QUndoCommand* parent = nullptr);

  void undo() override;
  void redo() override;

 private:
  QTableWidget* m_table;
  QTableWidget* m_table0;
  int m_row;
  int m_table0CurrentRow;

  QString m_text;
  QStringList m_fieldList;

  bool m_loadINI;
  bool m_writeINI;
};

class AddCommand : public QUndoCommand {
 public:
  AddCommand(QTableWidget* table, int row, int col, QString text,
             QUndoCommand* parent = nullptr);
  ~AddCommand();

  void undo() override;
  void redo() override;

 private:
  QTableWidget* m_table;
  int m_row;
  int m_col;
  QString m_text;
};

class EditCommand : public QUndoCommand {
 public:
  EditCommand(bool textAlignCenter, QString oldText, QTableWidget* table,
              int row, int col, QString text, QUndoCommand* parent = nullptr);
  ~EditCommand();

  void undo() override;
  void redo() override;

 private:
  QTableWidget* m_table;
  int m_row;
  int m_col;
  QString m_text;
  QString m_oldText;
  bool m_textAlignCenter;
};

class CopyPasteLineCommand : public QUndoCommand {
 public:
  CopyPasteLineCommand(QTableWidget* table, int row, int col, QString text,
                       QStringList colTextList, QString oldColText0,
                       bool writeini, bool writevalueini,
                       int leftTableCurrentRow, QUndoCommand* parent = nullptr);
  ~CopyPasteLineCommand();

  void undo() override;
  void redo() override;

 private:
  QTableWidget* m_table;
  int m_row;
  int m_col;
  QString m_text;
  QStringList m_colTextList;
  QString m_oldColText0;
  bool m_writeini;
  bool m_writevalueini;
  int m_leftTableCurrentRow;
};

QString createCommandString(QString cmdStr);

#endif
