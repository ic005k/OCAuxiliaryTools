#ifndef SYNCOCDIALOG_H
#define SYNCOCDIALOG_H

#include <QCryptographicHash>
#include <QDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPlainTextEdit>
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

  bool writefile = false;
  QProgressBar *progInfo;
  bool dlEnd = true;
  QString bufferJson;
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

  void readCheckStateINI();
  void setListWidgetStyle();
  void init_Sync_OC_Table();

  int get_PathCol(QTableWidget *t, QString pathText);
  void init_ItemColor();

  void on_ProgBarvalueChanged(QProgressBar *m_bar);
  QString getKextHtmlInfo(QString url, bool writeFile);
  QString getKextDevDL(QString bufferJson, QString kextName);
  void saveWindowsPos();
  void resizeWindowsPos();
  public slots:
  void on_btnStop_clicked();
  void on_listOpenCore_currentRowChanged(int currentRow);

  void query(QNetworkReply *reply);
 private slots:
  void on_btnStartSync_clicked();

  void on_listOpenCore_itemClicked(QListWidgetItem *item);

  void on_btnCheckUpdate_clicked();

  void on_btnUpdate_clicked();

  void on_btnSelectAll_clicked();

  void on_btnClearAll_clicked();

  void on_tableKexts_itemSelectionChanged();

  void on_btnGetOC_clicked();

  void on_comboOCVersions_currentTextChanged(const QString &arg1);

  void on_btnImport_clicked();

  void on_editOCDevSource_currentTextChanged(const QString &arg1);

  void on_btnSet_clicked();

  void on_btnGetLastOC_clicked();

 private:
  void setListWidgetColor(QString color);
  QNetworkAccessManager *mgr;

  QLabel *lblVer;
  QLabel *lblTxt;
  QCheckBox *checkBox;

  void init_InfoShow();
  QString getJsonUrl();

 protected:
  void closeEvent(QCloseEvent *event);
  void resizeEvent(QResizeEvent *event);
  void keyPressEvent(QKeyEvent *event);
};

#endif  // SYNCOCDIALOG_H
