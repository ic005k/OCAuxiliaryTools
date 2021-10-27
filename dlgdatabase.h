#ifndef DLGDATABASE_H
#define DLGDATABASE_H

#include <QDialog>

namespace Ui {
class dlgDatabase;
}

class dlgDatabase : public QDialog {
  Q_OBJECT

 public:
  explicit dlgDatabase(QWidget *parent = nullptr);
  ~dlgDatabase();
  Ui::dlgDatabase *ui;

  void refreshKextUrl();

  void saveKextUrl();
 public slots:

 private slots:
  void on_tableDatabase_cellDoubleClicked(int row, int column);

  void on_btnFind_clicked();

  void on_editFind_textChanged(const QString &arg1);

  void on_editFind_returnPressed();

  void on_tableDatabaseFind_cellDoubleClicked(int row, int column);

  void on_btnRefreshAll_clicked();

  void on_btnAdd_clicked();

  void on_btnDel_clicked();

  void on_btnTest_clicked();

  void on_comboBoxNet_currentTextChanged(const QString &arg1);

  void on_btnOpenUrl_clicked();

 private:
 protected:
  void closeEvent(QCloseEvent *event);
  void keyPressEvent(QKeyEvent *event);
};

#endif  // DLGDATABASE_H
