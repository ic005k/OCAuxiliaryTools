#ifndef SYNCOCDIALOG_H
#define SYNCOCDIALOG_H

#include <QCryptographicHash>
#include <QDialog>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QScrollBar>

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
  QStringList sourceFileList;
  QStringList targetFileList;

 private slots:
  void on_btnStartSync_clicked();

  void on_listSource_currentRowChanged(int currentRow);

  void on_listSource_itemClicked(QListWidgetItem *item);

  void on_listTarget_itemClicked(QListWidgetItem *item);

  void on_listTarget_currentRowChanged(int currentRow);

  void on_btnUpKexts_clicked();

  void on_btnStop_clicked();

  void on_btnUpdate_clicked();

 private:
  void setListWidgetStyle();
  void setListWidgetColor(QString color);

 protected:
  void closeEvent(QCloseEvent *event);
};

#endif  // SYNCOCDIALOG_H
