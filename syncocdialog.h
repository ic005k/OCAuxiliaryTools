#ifndef SYNCOCDIALOG_H
#define SYNCOCDIALOG_H

#include <QCryptographicHash>
#include <QDialog>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QProgressBar>
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
  QProgressBar *progBar;
  QList<QLabel *> textList;
  QList<QLabel *> verList;
  QList<QCheckBox *> chkList;

  bool eventFilter(QObject *o, QEvent *e);
  void initKextList();
  void readCheckStateINI();
 private slots:
  void on_btnStartSync_clicked();

  void on_listSource_currentRowChanged(int currentRow);

  void on_listSource_itemClicked(QListWidgetItem *item);

  void on_listTarget_itemClicked(QListWidgetItem *item);

  void on_listTarget_currentRowChanged(int currentRow);

  void on_btnUpKexts_clicked();

  void on_btnStop_clicked();

  void on_btnUpdate_clicked();

  void on_btnSelectAll_clicked();

  void on_btnClearAll_clicked();

 private:
  void setListWidgetStyle();
  void setListWidgetColor(QString color);

  QLabel *lblVer;
  QLabel *lblTxt;
  QCheckBox *checkBox;

  void addVerWidget(int currentRow, QString strTV, QString strSV,
                    QString strShowFileName);

  void writeCheckStateINI();

 protected:
  void closeEvent(QCloseEvent *event);
  void resizeEvent(QResizeEvent *event);
  void keyPressEvent(QKeyEvent *event);
};

#endif  // SYNCOCDIALOG_H
