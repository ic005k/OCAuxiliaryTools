#ifndef DLGPREFERENCE_H
#define DLGPREFERENCE_H

#include <QDialog>
#include <QProcess>
#include <QTableWidgetItem>

namespace Ui {
class dlgPreference;
}

class dlgPreference : public QDialog {
  Q_OBJECT

 public:
  explicit dlgPreference(QWidget *parent = nullptr);
  ~dlgPreference();
  Ui::dlgPreference *ui;

  QList<bool> listItemModi;
  QProcess *processPing;
  void refreshKextUrl();
  void on_btnFind_clicked();
  void saveKextUrl();

 private:
  void writeIni(QString key, int arg1);

 protected:
 protected:
  void closeEvent(QCloseEvent *event);
  void keyPressEvent(QKeyEvent *event);
 private slots:
  void on_btnAdd_clicked();
  void on_btnDel_clicked();
  void on_btnOpenUrl_clicked();
  void on_rbtnAPI_clicked();
  void on_rbtnWeb_clicked();
  void on_btnTest_clicked();
  void on_comboBoxNet_currentTextChanged(const QString &arg1);
  void on_comboBoxWeb_currentTextChanged(const QString &arg1);
  void on_chkShowVolName_clicked(bool checked);
  void on_tableDatabase_itemChanged(QTableWidgetItem *item);
  void on_chkHideToolbar_stateChanged(int arg1);
  void on_chkDatabase_stateChanged(int arg1);
  void on_chkBackupEFI_stateChanged(int arg1);
  void on_chkMountESP_stateChanged(int arg1);
  void on_chkRecentOpen_stateChanged(int arg1);
  void on_chkOpenDir_stateChanged(int arg1);
  void on_chkBoxLastFile_clicked(bool checked);
  void on_btnPing_clicked();
  void on_chkSmartKey_clicked(bool checked);
};

#endif  // DLGPREFERENCE_H
