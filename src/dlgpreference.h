#ifndef DLGPREFERENCE_H
#define DLGPREFERENCE_H

#include <QDialog>
#include <QProcess>
#include <QProgressBar>
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
  void refreshKextUrl(bool writeTable);
  void on_btnFind_clicked();
  void saveKextUrl();

 private:
  void writeIni(QString key, int arg1);
  QStringList listKexts;
  QStringList listFind;
  bool isRefresh = false;
  QString CurrentText;

  void find(QString arg1);

  void writeTable(QStringList listFind);

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
  void on_chkHideToolbar_stateChanged(int arg1);
  void on_chkDatabase_stateChanged(int arg1);
  void on_chkBackupEFI_stateChanged(int arg1);
  void on_chkMountESP_stateChanged(int arg1);
  void on_chkRecentOpen_stateChanged(int arg1);
  void on_chkOpenDir_stateChanged(int arg1);
  void on_chkBoxLastFile_clicked(bool checked);
  void on_btnPing_clicked();
  void on_chkSmartKey_clicked(bool checked);
  void on_btnDownloadKexts_clicked();
  void on_btnStop_clicked();
  void on_myeditFind_textChanged(const QString &arg1);

  void on_tableKextUrl_itemChanged(QTableWidgetItem *item);
  void on_myeditFind_returnPressed();
  void on_rbtnToken_clicked();
  void on_btnTokenHelp_clicked();
};

#endif  // DLGPREFERENCE_H
