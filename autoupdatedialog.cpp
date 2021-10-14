#include "autoupdatedialog.h"

#include "mainwindow.h"
#include "ui_autoupdatedialog.h"

extern MainWindow* mw_one;

AutoUpdateDialog::AutoUpdateDialog(QWidget* parent)
    : QDialog(parent), ui(new Ui::AutoUpdateDialog) {
  ui->setupUi(this);

  QFileInfo fi(qAppName());
  strLinuxTargetFile = fi.absoluteFilePath();

  setWindowTitle("");
  ui->progressBar->setTextVisible(false);
  Init();
  tempDir = QDir::homePath() + "/tempocat/";
  mw_one->deleteDirfile(tempDir);
  ui->label->setVisible(false);
}

AutoUpdateDialog::~AutoUpdateDialog() { delete ui; }

void AutoUpdateDialog::Init() {
  //"https://raw.fastgit.org/ic005k/"  //HK
  strWinUrl =
      "https://ghproxy.com/https://raw.githubusercontent.com/ic005k/"
      "QtOpenCoreConfigDatabase/main/win.zip";

  strMacUrl =
      "https://ghproxy.com/https://raw.githubusercontent.com/ic005k/"
      "QtOpenCoreConfigDatabase/main/Contents.zip";

  strMacClassicalUrl =
      "https://ghproxy.com/https://github.com/ic005k/QtOpenCoreConfigDatabase/"
      "releases/download/1.0.0/Contents.zip";

  strLinuxUrl =
      "https://ghproxy.com/https://github.com/ic005k/QtOpenCoreConfigDatabase/"
      "releases/download/1.0.0/OCAuxiliaryTools-Linux-x86_64.AppImage";

  strDatabaseUrl =
      "https://ghproxy.com/https://raw.githubusercontent.com/ic005k/"
      "QtOpenCoreConfigDatabase/main/Database.zip";

  managerDownLoad = new QNetworkAccessManager(this);
  myfile = new QFile(this);
}

void AutoUpdateDialog::doProcessReadyRead()  //读取并写入
{
  while (!reply->atEnd()) {
    QByteArray ba = reply->readAll();
    myfile->write(ba);
  }
}

void AutoUpdateDialog::doProcessFinished() { myfile->close(); }

void AutoUpdateDialog::doProcessDownloadProgress(qint64 recv_total,
                                                 qint64 all_total)  //显示
{
  ui->progressBar->setMaximum(all_total);
  ui->progressBar->setValue(recv_total);

  setWindowTitle(tr("Download Progress") + " : " + GetFileSize(recv_total, 2) +
                 " -> " + GetFileSize(all_total, 2));
  // setWindowTitle(tr("Download Progress") + " : " +
  // QString::number(recv_total) +
  //               " -> " + QString::number(all_total));

  if (recv_total == all_total) {
    if (recv_total < 10000) {
      return;
    }
    ui->btnStartUpdate->setEnabled(true);
    ui->btnUpdateDatabase->setEnabled(true);
    this->repaint();
    if ((mw_one->win) && ui->btnStartUpdate->isVisible()) {
      ui->label->setVisible(false);
    }

    if (mw_one->linuxOS) {
      QProcess* p = new QProcess;
      p->start("chmod", QStringList() << "+x" << tempDir + filename);
    }
  }
}

void AutoUpdateDialog::doProcessError(QNetworkReply::NetworkError code) {
  qDebug() << code;
}

void AutoUpdateDialog::on_btnStartUpdate_clicked() {
  ui->btnStartUpdate->setEnabled(false);
  startUpdate();
}

void AutoUpdateDialog::startUpdate() {
  ui->btnStartUpdate->setEnabled(false);
  this->repaint();

  QString strZip, strPath, strExec, strUnzip;
  QFileInfo appInfo(qApp->applicationDirPath());
  strZip = tempDir + filename;

  QDir dir;
  dir.setCurrent(tempDir);

  qApp->exit();

  if (mw_one->mac || mw_one->osx1012) {
    QTextEdit* txtEdit = new QTextEdit();
    QString strTarget = appInfo.path().replace("Contents", "");
    strTarget = strTarget + ".";
    strTarget = "\"" + strTarget + "\"";
    if (mw_one->mac) {
      txtEdit->append("hdiutil mount -mountpoint /Volumes/ocat " + strZip);
      txtEdit->append(
          "cp -R -p -f "
          "/Volumes/ocat/OCAuxiliaryTools.app/. " +
          strTarget);

      txtEdit->append("hdiutil eject /Volumes/ocat");
    }
    if (mw_one->osx1012) {
      txtEdit->append("hdiutil mount -mountpoint /Volumes/ocat1012 " + strZip);
      txtEdit->append(
          "cp -R -p -f "
          "/Volumes/ocat1012/OCAuxiliaryTools.app/. " +
          strTarget);

      txtEdit->append("hdiutil eject /Volumes/ocat1012");
    }

    strPath = appInfo.path().replace("Contents", "");
    strExec = strPath.mid(0, strPath.length() - 1);
    strExec = "\"" + strExec + "\"";
    txtEdit->append("open " + strExec);

    QString fileName = tempDir + "upocat.sh";
    TextEditToFile(txtEdit, fileName);

    QProcess::startDetached("bash", QStringList() << fileName);
  }

  if (mw_one->win) {
    strPath = appInfo.filePath();

    QTextEdit* txtEdit = new QTextEdit();
    strUnzip = strPath + "/unzip.exe";
    strUnzip = "\"" + strUnzip + "\"";
    strZip = "\"" + strZip + "\"";
    strPath = "\"" + strPath + "\"";
    strExec = qApp->applicationFilePath();
    strExec = "\"" + strExec + "\"";
    QString strCommand1, strCommand2;
    QString strx = "\"" + tempDir + "\"";
    strCommand1 = strUnzip + " -o " + strZip + " -d " + strx;
    QString stry = tempDir + QFileInfo(filename).baseName();
    stry = "\"" + stry + "\"";
    strCommand2 = "xcopy " + stry + " " + strPath + " /s/y";
    txtEdit->append(strCommand1 + " && " + strCommand2 + " && " + strExec);

    QString fileName = tempDir + "upocat.bat";
    TextEditToFile(txtEdit, fileName);

    QProcess::startDetached("cmd.exe", QStringList() << "/c" << fileName);
  }

  if (mw_one->linuxOS) {
    QTextEdit* txtEdit = new QTextEdit();
    strZip = "\"" + strZip + "\"";
    strLinuxTargetFile = "\"" + strLinuxTargetFile + "\"";
    txtEdit->append("cp -f " + strZip + " " + strLinuxTargetFile);
    txtEdit->append(strLinuxTargetFile);

    QString fileName = tempDir + "upocat.sh";
    TextEditToFile(txtEdit, fileName);

    QProcess::execute("chmod", QStringList() << "+x" << fileName);
    QProcess::startDetached("bash", QStringList() << fileName);
  }
}

void AutoUpdateDialog::startDownload(bool Database) {
  setWindowTitle("");
  ui->label->setVisible(false);
  ui->btnStartUpdate->setEnabled(false);
  ui->btnUpdateDatabase->setEnabled(false);
  this->repaint();

  if (!Database) {
    ui->btnStartUpdate->setVisible(true);
    ui->btnUpdateDatabase->setVisible(false);
  } else {
    strUrl = strDatabaseUrl;
    ui->btnStartUpdate->setVisible(false);
    ui->btnUpdateDatabase->setVisible(true);
  }

  QString str0, str1, str2;

  str0 = "https://download.fastgit.org/";            // 日本东京
  str1 = "https://ghproxy.com/https://github.com/";  // 韩国首尔
  str2 = strUrl.replace("https://github.com/", str0);
  strUrl = str2;

  QNetworkRequest request;
  request.setUrl(QUrl(strUrl));

  reply = managerDownLoad->get(request);  //发送请求
  connect(reply, &QNetworkReply::readyRead, this,
          &AutoUpdateDialog::doProcessReadyRead);  //可读
  connect(reply, &QNetworkReply::finished, this,
          &AutoUpdateDialog::doProcessFinished);  //结束
  connect(reply, &QNetworkReply::downloadProgress, this,
          &AutoUpdateDialog::doProcessDownloadProgress);  //大小
  connect(reply,
          QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error),
          this, &AutoUpdateDialog::doProcessError);  //异常
  QStringList list = strUrl.split("/");
  filename = list.at(list.length() - 1);
  QDir dir;
  if (dir.mkpath(tempDir)) {
  }
  QString file = tempDir + filename;

  myfile->setFileName(file);
  bool ret =
      myfile->open(QIODevice::WriteOnly | QIODevice::Truncate);  //创建文件
  if (!ret) {
    QMessageBox::warning(this, "warning", "Failed to open.");
    return;
  }
  ui->progressBar->setValue(0);
  ui->progressBar->setMinimum(0);
}

void AutoUpdateDialog::closeEvent(QCloseEvent* event) {
  Q_UNUSED(event);
  reply->close();
}

void AutoUpdateDialog::on_btnUpdateDatabase_clicked() {
  QFileInfo appInfo(qApp->applicationDirPath());
  QString strZip;
  strZip = tempDir + "Database.zip";

  QDir dir;
  dir.setCurrent(tempDir);

  QProcess* p = new QProcess;
  QString strPath;
  strPath = appInfo.filePath();
  if (mw_one->mac || mw_one->osx1012) {
    p->start("unzip", QStringList() << "-o" << strZip << "-d" << strPath);
  }
  if (mw_one->win) {
    p->start(strPath + "/unzip.exe", QStringList()
                                         << "-o" << strZip << "-d" << strPath);
  }

  close();
}

void AutoUpdateDialog::TextEditToFile(QTextEdit* txtEdit, QString fileName) {
  QFile* file;
  file = new QFile;
  file->setFileName(fileName);
  bool ok = file->open(QIODevice::WriteOnly);
  if (ok) {
    QTextStream out(file);
    out << txtEdit->toPlainText();
    file->close();
    delete file;
  }
}

void AutoUpdateDialog::keyPressEvent(QKeyEvent* event) {
  switch (event->key()) {
    case Qt::Key_Escape:

      break;

    case Qt::Key_Return:

      break;

    case Qt::Key_Backspace:

      break;

    case Qt::Key_Space:

      break;

    case Qt::Key_F1:

      break;
  }

  if (event->modifiers() == Qt::ControlModifier) {
    if (event->key() == Qt::Key_M) {
      this->setWindowState(Qt::WindowMaximized);
    }
  }
}

QString AutoUpdateDialog::GetFileSize(const qint64& size, int precision) {
  double sizeAsDouble = size;
  static QStringList measures;
  if (measures.isEmpty())
    measures << QCoreApplication::translate("QInstaller", "bytes")
             << QCoreApplication::translate("QInstaller", "KiB")
             << QCoreApplication::translate("QInstaller", "MiB")
             << QCoreApplication::translate("QInstaller", "GiB")
             << QCoreApplication::translate("QInstaller", "TiB")
             << QCoreApplication::translate("QInstaller", "PiB")
             << QCoreApplication::translate("QInstaller", "EiB")
             << QCoreApplication::translate("QInstaller", "ZiB")
             << QCoreApplication::translate("QInstaller", "YiB");
  QStringListIterator it(measures);
  QString measure(it.next());
  while (sizeAsDouble >= 1024.0 && it.hasNext()) {
    measure = it.next();
    sizeAsDouble /= 1024.0;
  }
  return QString::fromLatin1("%1 %2")
      .arg(sizeAsDouble, 0, 'f', precision)
      .arg(measure);
}

QString AutoUpdateDialog::getFormatBybytes(qint64 size) {
  char unit;
  const char* units[] = {" Bytes", " kB", " MB", " GB"};
  for (unit = -1; (++unit < 3) && (size > 1023); size /= 1024)
    ;
  return QString::number(size, 'f', 1) + units[unit];
}
