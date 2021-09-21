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
    QStringList list = strUrl.split("/");
    QString filename = list.at(list.length() - 1);
    if (recv_total == all_total) {
        //QString data = "Download successful--" + filename;
        ui->pushButton_2->setEnabled(true);
        this->repaint();
        //startUpdate();
    }

    setWindowTitle(QString::number(recv_total) + "->" + QString::number(all_total));
}
void AutoUpdateDialog::doProcessError(QNetworkReply::NetworkError code)
{
    qDebug() << code;
}

void AutoUpdateDialog::on_pushButton_clicked()
{
    startDownload();
}

void AutoUpdateDialog::on_pushButton_2_clicked()
{
    ui->pushButton_2->setVisible(false);
    startUpdate();
}

void AutoUpdateDialog::readResult()
{
    QFileInfo appInfo(qApp->applicationDirPath());
    QProcess* p1 = new QProcess;
    p1->setEnvironment(p1->environment());
    p1->start(appInfo.path() + "/OCAuxiliaryTools.app");
}

void AutoUpdateDialog::on_pushButton_3_clicked()
{
}

void AutoUpdateDialog::startUpdate()
{
    ui->pushButton_2->setEnabled(false);
    this->repaint();

    QFileInfo appInfo(qApp->applicationDirPath());

    QString str = tempDir + "OCAuxiliaryTools.app.zip";
    QDir dir;
    dir.setCurrent(tempDir);

    qApp->exit();

    QProcess* p = new QProcess;
    QString strPath = appInfo.path().replace("OCAuxiliaryTools.app/Contents", "");
    p->start("unzip", QStringList() << "-o" << str << "-d" << strPath);
    //connect(p, SIGNAL(finished(int)), this, SLOT(readResult()));
    p->waitForFinished();

    QProcess* p1 = new QProcess;
    p1->start(strPath + "/OCAuxiliaryTools.app");
    p1->waitForStarted();
}
void AutoUpdateDialog::startDownload()
{
    ui->pushButton_2->setEnabled(false);
    this->repaint();
    //中国香港：https://raw.fastgit.org/ic005k/QtOpenCoreConfigDatabase/main/OCAuxiliaryTools.app.zip
    //韩国首尔：https://ghproxy.com/https://raw.githubusercontent.com/ic005k/QtOpenCoreConfigDatabase/main/OCAuxiliaryTools.app.zip

    if (mw_one->mac)
        strUrl = "https://ghproxy.com/https://raw.githubusercontent.com/ic005k/QtOpenCoreConfigDatabase/main/OCAuxiliaryTools.app.zip";
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
