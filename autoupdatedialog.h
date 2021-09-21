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

namespace Ui {
class AutoUpdateDialog;
}

class AutoUpdateDialog : public QDialog {
    Q_OBJECT

public:
    explicit AutoUpdateDialog(QWidget* parent = nullptr);
    ~AutoUpdateDialog();

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

protected:
    void closeEvent(QCloseEvent* event);
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void readResult();

private:
    Ui::AutoUpdateDialog* ui;
};

#endif // AUTOUPDATEDIALOG_H
