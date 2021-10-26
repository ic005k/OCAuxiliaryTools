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
  bool blSame;
  QStringList sourceFileList;
  QStringList targetFileList;

 private slots:
  void on_btnStartSync_clicked();

  void on_listSource_currentRowChanged(int currentRow);

  void on_listSource_itemClicked(QListWidgetItem *item);

  void on_listTarget_itemClicked(QListWidgetItem *item);

  void on_listTarget_currentRowChanged(int currentRow);

 private:
  void setListWidgetStyle();
  void setListWidgetColor(QString color);
};

#endif  // SYNCOCDIALOG_H
