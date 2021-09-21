#include "autoupdatedialog.h"
#include "mainwindow.h"
#include "ui_autoupdatedialog.h"
extern MainWindow* mw_one;

AutoUpdateDialog::AutoUpdateDialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::AutoUpdateDialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("Automatic updates"));
    ui->progressBar->setTextVisible(false);
    Init();
    tempDir = QDir::homePath() + "/tempocat/";
    mw_one->deleteDirfile(tempDir);
    ui->label->setVisible(false);
}

AutoUpdateDialog::~AutoUpdateDialog()
{
    delete ui;
}

void AutoUpdateDialog::Init()
{
    manager = new QNetworkAccessManager(this);
    myfile = new QFile(this);
}

void AutoUpdateDialog::doProcessReadyRead() //读取并写入
{
    while (!reply->atEnd()) {
        QByteArray ba = reply->readAll();
        myfile->write(ba);
    }
}
void AutoUpdateDialog::doProcessFinished()
{
    myfile->close();
}
void AutoUpdateDialog::doProcessDownloadProgress(qint64 recv_total, qint64 all_total) //显示
{
    ui->progressBar->setMaximum(all_total);
    ui->progressBar->setValue(recv_total);
    if (recv_total == all_total) {
        ui->btnStartUpdate->setEnabled(true);
        this->repaint();
        if (mw_one->win)
            ui->label->setVisible(true);
    }

    setWindowTitle(GetFileSize(recv_total) + " -> " + GetFileSize(all_total));
}
void AutoUpdateDialog::doProcessError(QNetworkReply::NetworkError code)
{
    qDebug() << code;
}

void AutoUpdateDialog::on_btnStartUpdate_clicked()
{
    ui->btnStartUpdate->setEnabled(false);
    startUpdate();
}

void AutoUpdateDialog::startUpdate()
{
    ui->btnStartUpdate->setEnabled(false);
    this->repaint();

    QFileInfo appInfo(qApp->applicationDirPath());
    QString str;
    if (mw_one->mac) {
        str = tempDir + "OCAuxiliaryTools.app.zip";
    }
    if (mw_one->win) {
        str = tempDir + "OCAT-Win64.zip";
    }
    QDir dir;
    dir.setCurrent(tempDir);

    qApp->exit();

    QProcess *p = new QProcess;
    QString strPath;
    if (mw_one->mac) {
        strPath = appInfo.path().replace("OCAuxiliaryTools.app/Contents", "");
        p->start("unzip", QStringList() << "-o" << str << "-d" << strPath);
        p->waitForFinished();
    }
    if (mw_one->win) {
        strPath = appInfo.filePath(); //.replace("OCAT-Win64", "");

        p->start(appInfo.filePath() + "/unzip.exe", QStringList() << "-o" << str << "-d" << strPath);
    }

    QProcess *p1 = new QProcess;
    QStringList arguments;
    QString fn = "";
    arguments << fn;
    if (mw_one->mac) {
        p1->start(strPath + "/OCAuxiliaryTools.app", arguments);
    }
    if (mw_one->win) {
        //p1->start(appInfo.filePath() + "/OCAuxiliaryTools.exe", arguments);
    }
    p1->waitForStarted();
}
void AutoUpdateDialog::startDownload()
{
    ui->btnStartUpdate->setEnabled(false);
    this->repaint();

    QNetworkRequest request;
    request.setUrl(QUrl(strUrl));

    reply = manager->get(request); //发送请求
    connect(reply, &QNetworkReply::readyRead, this, &AutoUpdateDialog::doProcessReadyRead); //可读
    connect(reply, &QNetworkReply::finished, this, &AutoUpdateDialog::doProcessFinished); //结束
    connect(reply, &QNetworkReply::downloadProgress, this, &AutoUpdateDialog::doProcessDownloadProgress); //大小
    connect(reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error), this, &AutoUpdateDialog::doProcessError); //异常
    QStringList list = strUrl.split("/");
    filename = list.at(list.length() - 1);
    QDir dir;
    if (dir.mkpath(tempDir)) { }
    QString file = tempDir + filename;

    myfile->setFileName(file);
    bool ret = myfile->open(QIODevice::WriteOnly | QIODevice::Truncate); //创建文件
    if (!ret) {
        QMessageBox::warning(this, "warning", "Failed to open.");
        return;
    }
    ui->progressBar->setValue(0);
    ui->progressBar->setMinimum(0);
}

void AutoUpdateDialog::closeEvent(QCloseEvent* event)
{
    Q_UNUSED(event);
    reply->close();
}

QString AutoUpdateDialog::GetFileSize(qint64 size)
{
    if (!size) {
        return "0 Bytes";
    }
    static QStringList SizeNames;
    if (SizeNames.empty()) {
        SizeNames << " Bytes"
                  << " KB"
                  << " MB"
                  << " GB"
                  << " TB"
                  << " PB"
                  << " EB"
                  << " ZB"
                  << " YB";
    }
    int i = qFloor(qLn(size) / qLn(1024));
    return QString::number(size * 1.0 / qPow(1000, qFloor(i)),
               'f', (i > 1) ? 2 : 0)
        + SizeNames.at(i);
}
