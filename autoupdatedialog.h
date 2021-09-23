#ifndef AUTOUPDATEDIALOG_H
#define AUTOUPDATEDIALOG_H

#include <QDebug>
#include <QDialog>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QProcess>
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
    QNetworkAccessManager* manager;
    QNetworkReply* reply;
    QFile* myfile;
    void Init();
    void doProcessReadyRead();
    void doProcessFinished();
    void doProcessDownloadProgress(qint64, qint64);
    void doProcessError(QNetworkReply::NetworkError code);

    void startUpdate();
    void startDownload();
    QString strUrl;
    QString strWinUrl;
    QString strMacUrl;

    QString GetFileSize(qint64 size);

protected:
    void closeEvent(QCloseEvent* event);
private slots:

    void on_btnStartUpdate_clicked();

private:
};

#endif // AUTOUPDATEDIALOG_H
