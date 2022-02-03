#ifndef DLGDATABASE_H
#define DLGDATABASE_H

#include <QDialog>
#include <QProcess>
#include <QTableWidgetItem>

namespace Ui {
class dlgDatabase;
}

class dlgDatabase : public QDialog {
  Q_OBJECT

 public:
  explicit dlgDatabase(QWidget *parent = nullptr);
  ~dlgDatabase();
  Ui::dlgDatabase *ui;

  QList<bool> listItemModi;
  QProcess *processPing;

  void on_btnFind_clicked();
  void saveKextUrl();

  void get_EFI(int row, int column, QTableWidget *table);
  void init_Database(QStringList files);
 public slots:

 private slots:
  void on_tableDatabase_cellDoubleClicked(int row, int column);

  void on_editFind_textChanged(const QString &arg1);

  void on_editFind_returnPressed();

  void on_tableDatabaseFind_cellDoubleClicked(int row, int column);

  void on_readoutput();
  void on_readerror();

  void on_tableDatabase_itemDoubleClicked(QTableWidgetItem *item);

  void on_btnIntel_clicked();

  void on_btnAMD_clicked();

  void on_btnOpenDir_clicked();

  void on_btnIntelOnline_clicked();

  void on_btnAMDOnline_clicked();

  void on_btnGenerateEFI_clicked();

  void on_tableDatabase_currentItemChanged(QTableWidgetItem *current,
                                           QTableWidgetItem *previous);

 private:
 protected:
  void closeEvent(QCloseEvent *event);
  void keyPressEvent(QKeyEvent *event);
};

#endif  // DLGDATABASE_H
