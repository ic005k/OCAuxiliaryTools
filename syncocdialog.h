#ifndef SYNCOCDIALOG_H
#define SYNCOCDIALOG_H

#include <QCryptographicHash>
#include <QDialog>
#include <QListWidgetItem>
#include <QMessageBox>

namespace Ui {
class SyncOCDialog;
}

class SyncOCDialog : public QDialog {
  Q_OBJECT

 public:
  explicit SyncOCDialog(QWidget *parent = nullptr);
  ~SyncOCDialog();
  Ui::SyncOCDialog *ui;
  QString sourceResourcesDir;
  QString targetResourcesDir;

 private slots:
  void on_btnStartSync_clicked();

  void on_listSource_currentRowChanged(int currentRow);

 private:
};

#endif  // SYNCOCDIALOG_H
