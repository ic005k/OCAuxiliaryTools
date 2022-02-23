#ifndef SYNCOCDIALOG_H
#define SYNCOCDIALOG_H

#include <QCryptographicHash>
#include <QDialog>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QProgressBar>
#include <QScrollBar>
#include <QTableWidget>

namespace Ui {
class SyncOCDialog;
}

class SyncOCDialog : public QDialog {
  Q_OBJECT

 public:
  explicit SyncOCDialog(QWidget *parent = nullptr);
  ~SyncOCDialog();
  Ui::SyncOCDialog *ui;

  QString downLink;
  void writeCheckStateINI();
  bool isCheckOC = false;
  QStringList sourceKexts, targetKexts, sourceOpenCore, targetOpenCore;
  QString sourceResourcesDir;
  QString targetResourcesDir;
  QStringList sourceFileList;
  QStringList targetFileList;

  QList<QLabel *> textList;
  QList<QLabel *> verList;
  QList<QCheckBox *> chkList;

  bool eventFilter(QObject *o, QEvent *e);
  void initKextList();
  void readCheckStateINI();
  void setListWidgetStyle();
  void init_Sync_OC_Table();

  int get_PathCol(QTableWidget *t, QString pathText);
  void init_ItemColor();
public slots:
  void on_btnStop_clicked();
  void on_listOpenCore_currentRowChanged(int currentRow);

 private slots:
  void on_btnStartSync_clicked();

  void on_listKexts_currentRowChanged(int currentRow);

  void on_listKexts_itemClicked(QListWidgetItem *item);

  void on_listOpenCore_itemClicked(QListWidgetItem *item);

  void on_btnCheckUpdate_clicked();

  void on_btnUpdate_clicked();

  void on_btnSelectAll_clicked();

  void on_btnClearAll_clicked();

  void on_btnSettings_clicked();

  void on_tableKexts_itemSelectionChanged();

  void on_btnUpdateOC_clicked();

  void on_comboOCVersions_currentTextChanged(const QString &arg1);

  void on_btnImport_clicked();

  void on_editOCDevSource_currentTextChanged(const QString &arg1);

 private:
  void setListWidgetColor(QString color);

  QLabel *lblVer;
  QLabel *lblTxt;
  QCheckBox *checkBox;

  void addVerWidget(int currentRow, QString strTV, QString strSV,
                    QString strShowFileName);

 protected:
  void closeEvent(QCloseEvent *event);
  void resizeEvent(QResizeEvent *event);
  void keyPressEvent(QKeyEvent *event);
};

#endif  // SYNCOCDIALOG_H
