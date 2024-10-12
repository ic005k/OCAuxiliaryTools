#ifndef METHOD_H
#define METHOD_H

#include <QComboBox>
#include <QDir>
#include <QDirIterator>
#include <QElapsedTimer>
#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QNetworkProxy>
#include <QNetworkReply>
#include <QObject>
#include <QTableWidget>
#include <QTextEdit>
#include <QWidget>

class Method : public QMainWindow {
  Q_OBJECT
 public:
  explicit Method(QWidget *parent = nullptr);

  bool isReplyDL = false;
  bool isReply = false;
  QStringList kextDLUrlList;
  bool blBreak = false;
  QString kextName;
  bool dlEnd = false;
  QString tempDir;
  QString filename;
  QNetworkAccessManager *manager;
  QNetworkAccessManager *managerDownLoad;
  QNetworkReply *reply, *replyDL;
  QFile *myfile;
  QTimer *tmrUpdateShow;
  void doProcessReadyRead();
  void doProcessFinished();
  void doProcessDownloadProgress(qint64, qint64);
  QStringList strDLInfoList;

  void goTable(QTableWidget *table);
  void goACPITable(QTableWidget *table);
  void goBooterTable(QTableWidget *table);
  void goDPTable(QTableWidget *table);
  void goKernelTable(QTableWidget *table);
  void goMiscTable(QTableWidget *table);
  void goNVRAMTable(QTableWidget *table);

  void findTable(QString findText);

  QString getTabTextName(int index);

  QString copyTools(QString pathSource, QString pathTarget);
  QString copyACPI(QString pathSource, QString pathTarget);
  QString copyKexts(QString pathSource, QString pathTarget);
  QString copyDrivers(QString pathSource, QString pathTarget);
  void findDP(QTableWidget *t, QString findText);
  void findNVRAM(QTableWidget *t, QString findText);

  void set_nv_key(QString key, QString dataType);

  void UpdateStatusBarInfo();
  void addFileSystemWatch(QString strOpenFile);

  void removeFileSystemWatch(QString strOpenFile);
  QStringList delDuplication(QStringList FileName, QTableWidget *table,
                             int col);

  void OCValidationProcessing();
  void setStatusBarTip(QWidget *w);

  bool isKext(QString kextName);
  QString getKextBin(QString kextName);
  static QString loadText(QString textFile);
  QString getKextVersion(QString kextFile);
  static QString getTextEditLineText(QTextEdit *txtEdit, int i);

  QObjectList getAllToolButton(QObjectList lstUIControls);
  void delRightTableItem(QTableWidget *t0, QTableWidget *t);
  bool isEqualInList(QString str, QStringList list);
  static QStringList DirToFileList(QString strDir, QString strFilter);
  bool isWhatFile(QString File, QString filter);
  void getLastReleaseFromUrl(QString strUrl);
  void parse_UpdateJSON(QString str);
  void startDownload(QString strUrl);
  QString GetFileSize(const qint64 &size, int precision);
  bool blCanBeUpdate = false;
  void kextUpdate();
  void finishKextUpdate(bool blDatabase);
  void getAllFolds(const QString &foldPath, QStringList &folds);
  void getAllFiles(const QString &foldPath, QStringList &folds,
                   const QStringList &formats);
  static void TextEditToFile(QTextEdit *txtEdit, QString fileName);
  QString getFileName(QString file);
  void cancelKextUpdate();

  QString getHTMLSource(QString URLSTR, bool writeFile);
  QStringList getDLUrlList(QString url);
  void getLastReleaseFromHtml(QString url);

  void writeLeftTable(QTableWidget *t0, QTableWidget *t);

  void writeLeftTableOnlyValue(QTableWidget *t0, QTableWidget *t);
  void kextPreset();
  void mount_esp_mac(QString strEfiDisk);
  QString getDriverVolInfo(QString strDisk);
  void backupEFI();
  void init_PresetQuirks(QComboBox *comboBox, QString quirksFile);
  QStringList getMarkerQuirks(QString arg1, QString strItem, QWidget *tab,
                              QString quirksFile);
  void setToolTip(QWidget *w, QString strTitle);
  void autoTip();
  QString readPlistComment(QString plistFile);
  void writePlistComment(QString plistFile, QString strComment);
  QStringList getDiskList();
  QString getDriverName(QString strDisk);
  QStringList getAllVolForDisk(QString strDisk);
  QStringList getPartitionList(QString strDisk);
  QStringList getVolListForPartition(QStringList listPartition);
  QString getVolForPartition(QString strPartition);

  void updateOpenCore();
  static void init_MacVerInfo(QString ver);
  QString readPlist(QString plistFile, QString key);
  void writePlist(QString plistFile, QString key, QString value);
  bool isKeyExists(QString plistFile, QString key);
  void show_Tip(QString strText, QString strTip);
  static QVariantList get_TableData(QTableWidget *t);
  static bool isInt(QString strCol);
  static bool isData(QString strCol);
  static bool isBool(QString strCol);
  static void set_TableData(QTableWidget *t, QVariantList mapList);
  static void init_Table(QTableWidget *t, QStringList listHeaders);
  static QStringList get_HorizontalHeaderList(QString main, QString sub);
  static void add_OneLine(QTableWidget *t);
  void downloadAllKexts();
  void unZip(QString zipfile);
  static QString setLineEditQss(QLineEdit *txt, int radius, int borderWidth,
                                const QString &normalColor,
                                const QString &focusColor);
  static QString setComboBoxQss(QComboBox *txt, int radius, int borderWidth,
                                const QString &normalColor,
                                const QString &focusColor);
  static void init_UIWidget(QWidget *w, int red);

 public slots:
  void generateEFI(QString file);
  void on_btnExportMaster();
  void on_btnImportMaster();

  void replyFinished(QNetworkReply *reply);

  void Quit();

 private:
  QElapsedTimer downloadTimer;
  void cancelDownload();
  QString errorInfo;

 signals:
 private slots:
};

#endif  // METHOD_H
