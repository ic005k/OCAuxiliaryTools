#ifndef METHOD_H
#define METHOD_H

#include <QDir>
#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QObject>
#include <QTableWidget>
#include <QTextEdit>
#include <QWidget>

class Method : public QMainWindow {
  Q_OBJECT
 public:
  explicit Method(QWidget* parent = nullptr);

  QString getMD5(QString targetFile);
  void goTable(QTableWidget* table);
  QWidget* getSubTabWidget(int m, int s);
  void goACPITable(QTableWidget* table);
  void goBooterTable(QTableWidget* table);
  void goDPTable(QTableWidget* table);
  void goKernelTable(QTableWidget* table);
  void goMiscTable(QTableWidget* table);
  void goNVRAMTable(QTableWidget* table);

  void findTable(QString findText);
  void init_Table(int index);
  QString getTabTextName(int index);

  QString copyTools(QString pathSource, QString pathTarget);
  QString copyACPI(QString pathSource, QString pathTarget);
  QString copyKexts(QString pathSource, QString pathTarget);
  QString copyDrivers(QString pathSource, QString pathTarget);
  void findDP(QTableWidget* t, QString findText);
  void findNVRAM(QTableWidget* t, QString findText);

  void set_nv_key(QString key, QString dataType);

  void UpdateStatusBarInfo();
  void addFileSystemWatch(QString strOpenFile);

  void removeFileSystemWatch(QString strOpenFile);
  QStringList deDuplication(QStringList FileName, QTableWidget* table, int col);
  void markColor(QTableWidget* table, QString path, int col);
  void OCValidationProcessing();
  void setStatusBarTip(QWidget* w);

  bool isKext(QString kextName);
  QString getKextBin(QString kextName);
  QString loadText(QString textFile);
  QString getKextVersion(QString kextFile);
  QString getTextEditLineText(QTextEdit* txtEdit, int i);

 public slots:
  void on_GenerateEFI();
  void on_btnExportMaster();
  void on_btnImportMaster();

 signals:
};

#endif  // METHOD_H
