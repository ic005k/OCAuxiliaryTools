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

 public slots:

 private slots:
  void on_tableDatabase_cellDoubleClicked(int row, int column);

  void on_btnFind_clicked();

  void on_editFind_textChanged(const QString &arg1);

  void on_editFind_returnPressed();

  void on_tableDatabaseFind_cellDoubleClicked(int row, int column);

  void on_btnRefreshAll_clicked();

 private:
};

#endif  // DLGDATABASE_H
