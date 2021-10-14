#ifndef AUTOUPDATEDIALOG_H
#define AUTOUPDATEDIALOG_H

#include <QDebug>
#include <QDialog>
#include <QDir>
#include <QElapsedTimer>
#include <QFile>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QProcess>
#include <QTextEdit>
#include <QTextStream>
#include <QtMath>

namespace Ui {
class AutoUpdateDialog;
}

class AutoUpdateDialog : public QDialog {
  Q_OBJECT

 public:
  explicit AutoUpdateDialog(QWidget* parent = nullptr);
  ~AutoUpdateDialog();
  Ui::AutoUpdateDialog* ui;

  QString tempDir;
  QString filename;
  QNetworkAccessManager* managerDownLoad;
  QNetworkReply* reply;
  QFile* myfile;
  void Init();
  void doProcessReadyRead();
  void doProcessFinished();
  void doProcessDownloadProgress(qint64, qint64);
  void doProcessError(QNetworkReply::NetworkError code);

  void startUpdate();
  void startDownload(bool Database);
  QString strUrl;
  QString strWinUrl;
  QString strMacUrl;
  QString strDatabaseUrl;
  QString strMacClassicalUrl;
  QString strLinuxUrl;
  QString strLinuxTargetFile;

  void TextEditToFile(QTextEdit* txtEdit, QString fileName);

  QString getFormatBybytes(qint64 size);

  QString GetFileSize(const qint64& size, int precision);

  QString GetFileSize(qint64 size);

 protected:
  void closeEvent(QCloseEvent* event);
  void keyPressEvent(QKeyEvent* event);
 private slots:

  void on_btnStartUpdate_clicked();

  void on_btnUpdateDatabase_clicked();

 private:
  QElapsedTimer downloadTimer;
};

#endif  // AUTOUPDATEDIALOG_H
