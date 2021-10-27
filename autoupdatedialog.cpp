#include "autoupdatedialog.h"

#include "mainwindow.h"
#include "ui_autoupdatedialog.h"

extern MainWindow* mw_one;

AutoUpdateDialog::AutoUpdateDialog(QWidget* parent)
    : QDialog(parent), ui(new Ui::AutoUpdateDialog) {
  ui->setupUi(this);

  tmrUpdateShow = new QTimer(this);
  connect(tmrUpdateShow, SIGNAL(timeout()), this, SLOT(UpdateTextShow()));

  QFileInfo fi(qAppName());
  strLinuxTargetFile = fi.absoluteFilePath();

  setWindowTitle("");
  ui->progressBar->setTextVisible(false);
  Init();
  tempDir = QDir::homePath() + "/tempocat/";
  mw_one->deleteDirfile(tempDir);
  ui->progressBar->setVisible(false);
}

AutoUpdateDialog::~AutoUpdateDialog() { delete ui; }

void AutoUpdateDialog::Init() {
  //"https://raw.fastgit.org/"  //HK
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

void AutoUpdateDialog::doProcessReadyRead() {
  while (!reply->atEnd()) {
    QByteArray ba = reply->readAll();
    myfile->write(ba);
  }
}

void AutoUpdateDialog::doProcessFinished() {
  if (reply->error() == QNetworkReply::NoError) {
    myfile->flush();
    myfile->close();
    if (!blCanBeUpdate) return;
    ui->btnStartUpdate->setEnabled(true);
    ui->btnUpdateDatabase->setEnabled(true);
    this->repaint();

    if (mw_one->linuxOS) {
      QProcess* p = new QProcess;
      p->start("chmod", QStringList() << "+x" << tempDir + filename);
    }

  } else {
    QMessageBox::critical(NULL, tr("Error"), "Failed!!!");
  }
}

void AutoUpdateDialog::doProcessDownloadProgress(qint64 recv_total,
                                                 qint64 all_total)  //显示
{
  ui->progressBar->setMaximum(all_total);
  ui->progressBar->setValue(recv_total);

  // calculate the download speed
  double speed = recv_total * 1000.0 / downloadTimer.elapsed();
  QString unit;
  if (speed < 1024) {
    unit = "bytes/sec";
  } else if (speed < 1024 * 1024) {
    speed /= 1024;
    unit = "kB/s";
  } else {
    speed /= 1024 * 1024;
    unit = "MB/s";
  }

  QString strSpeed =
      QString::fromLatin1("%1 %2").arg(speed, 3, 'f', 1).arg(unit);

  setWindowTitle(tr("Download Progress") + " : " + GetFileSize(recv_total, 2) +
                 " -> " + GetFileSize(all_total, 2) + "    " + strSpeed);

  if (recv_total == all_total) {
    if (recv_total < 100000) {
      blCanBeUpdate = false;
    } else
      blCanBeUpdate = true;
  }
}

void AutoUpdateDialog::doProcessError(QNetworkReply::NetworkError code) {
  Q_UNUSED(code);
  qDebug() << "Error : " << reply->error();
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

  QString strTokyo, strSeoul, strOriginal;

  strOriginal = "https://github.com/";
  strTokyo = "https://download.fastgit.org/";
  strSeoul = "https://ghproxy.com/https://github.com/";
  // strUrl.replace("https://github.com/", strTokyo);

  QNetworkRequest request;
  request.setUrl(QUrl(strUrl));
  request.setHeader(QNetworkRequest::ContentTypeHeader,
                    "application/octet-stream");

  reply = managerDownLoad->get(request);  //发送请求

  connect(reply, &QNetworkReply::readyRead, this,
          &AutoUpdateDialog::doProcessReadyRead);  //可读
  connect(reply, &QNetworkReply::finished, this,
          &AutoUpdateDialog::doProcessFinished);  //结束
  connect(reply, &QNetworkReply::downloadProgress, this,
          &AutoUpdateDialog::doProcessDownloadProgress);  //大小
  // connect(reply,
  //        QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error),
  //        this, &AutoUpdateDialog::doProcessError);  //异常

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

  downloadTimer.start();
}

void AutoUpdateDialog::closeEvent(QCloseEvent* event) {
  Q_UNUSED(event);
  // myfile->close();
  // reply->close();
  // reply->deleteLater();
  processWget->close();
  processWget->kill();
  delete processWget;
  tmrUpdateShow->stop();
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

QString AutoUpdateDialog::GetFileSize(qint64 size) {
  if (size < 0) return "0";
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
  return QString::number(size * 1.0 / qPow(1000, qFloor(i)), 'f',
                         (i > 1) ? 2 : 0) +
         SizeNames.at(i);
}

void AutoUpdateDialog::startWgetDownload() {
  ui->btnStartUpdate->setEnabled(false);
  ui->btnStartUpdate->setDefault(true);
  ui->btnUpdateDatabase->setEnabled(false);
  ui->btnUpdateDatabase->setVisible(false);
  ui->textEdit->clear();
  ui->textEdit->setReadOnly(true);
  ui->progressBar->setMinimum(0);
  ui->progressBar->setMaximum(0);
  setWindowTitle("");
  if (mw_one->mac || mw_one->osx1012) ui->textEdit->setFont(QFont("Menlo", 12));

  QStringList list = strUrlOrg.split("/");
  filename = list.at(list.length() - 1);
  QDir dir;
  if (dir.mkpath(tempDir)) {
  }
  QString file = tempDir + filename;

  QString strTokyo, strSeoul, strOriginal, strTest, strSet, strTemp;

  strOriginal = "https://github.com/";
  strTokyo = "https://download.fastgit.org/";
  strSeoul = "https://ghproxy.com/https://github.com/";
  strTest = "https://gh.api.99988866.xyz/https://github.com/";
  // if (mw_one->zh_cn) strUrl.replace("https://github.com/", strTokyo);
  strSet = mw_one->myDatabase->ui->comboBoxNet->currentText().trimmed();
  strTemp = strUrlOrg;
  strUrl = strTemp.replace("https://github.com/", strSet);
  // qDebug() << strUrl << strSet;

  processWget = new QProcess(this);

  connect(processWget, SIGNAL(finished(int)), this, SLOT(readResult(int)));

  QString strExec;
  if (mw_one->mac || mw_one->osx1012) {
    // strExec = mw_one->strAppExePath + "/wget";
    strExec = mw_one->strAppExePath + "/aria2c";
    connect(processWget, SIGNAL(readyReadStandardOutput()), this,
            SLOT(onReadData()));
    processWget->setReadChannel(QProcess::StandardOutput);
    ui->textEdit->append("Source: " + strUrl);
    ui->textEdit->append("Target: " + tempDir + filename);

    QDir::setCurrent(tempDir);
    processWget->start(strExec, QStringList() << "--allow-overwrite=true"
                                              << "--file-allocation=none"
                                              << "-l"
                                              << "-"
                                              << "--log-level=warn"
                                              << "--log-level=info" << strUrl);
    // processWget->start(strExec, QStringList()
    //                                 << "-v"
    //                                 << "-o" << tempDir + filename << strUrl);
  } else {
    if (mw_one->win) strExec = mw_one->strAppExePath + "/wget.exe";
    if (mw_one->linuxOS) strExec = "wget";

    processWget->start(strExec, QStringList()
                                    << "-v"
                                    << "-O" << file << "-o"
                                    << tempDir + "info.txt" << strUrl);
    tmrUpdateShow->start(100);
  }
  // processWget->start("curl", QStringList() << "-O" << strUrl);
  // processWget->start("ping", QStringList() << "www.qq.com");
  processWget->waitForStarted();
}

void AutoUpdateDialog::readResult(int exitCode) {
  if (exitCode == 0) {
    tmrUpdateShow->stop();
    ui->progressBar->setMaximum(100);
    ui->btnStartUpdate->setEnabled(true);
    ui->btnUpdateDatabase->setEnabled(true);
    setWindowTitle("");
    if (mw_one->win || mw_one->linuxOS) UpdateTextShow();

    if (mw_one->linuxOS) {
      QProcess* p = new QProcess;
      p->start("chmod", QStringList() << "+x" << tempDir + filename);
    }
  }
}

void AutoUpdateDialog::onReadData() {
  QString result = processWget->readAllStandardOutput();
  ui->textEdit->append(result);
  ui->textEdit->moveCursor(QTextCursor::End);

  QTextEdit* editTemp = new QTextEdit;
  editTemp->setText(ui->textEdit->toPlainText());
  for (int i = editTemp->document()->lineCount() - 1; i > 0; i--) {
    QTextBlock block = editTemp->document()->findBlockByNumber(i);
    editTemp->setTextCursor(QTextCursor(block));
    QString lineText =
        editTemp->document()->findBlockByNumber(i).text().trimmed();
    if (lineText.mid(0, 2) == "[#") {
      setWindowTitle(lineText);
      break;
    }
  }
}

void AutoUpdateDialog::UpdateTextShow() {
  QString strInfoFile = tempDir + "info.txt";
  if (!QFileInfo(strInfoFile).exists()) return;

  QFile* file = new QFile;
  file->setFileName(strInfoFile);
  bool ok = file->open(QIODevice::ReadOnly);
  if (ok) {
    QTextStream in(file);
    QString strOrg = in.readAll().trimmed();
    QString strCur = ui->textEdit->toPlainText().trimmed();
    if (strCur != strOrg) {
      ui->textEdit->setText(strOrg);
      ui->textEdit->moveCursor(QTextCursor::End);
    }

    file->close();
    delete file;

  } else {
    tmrUpdateShow->stop();
    QMessageBox::information(this, "Error Message",
                             "Open File:" + file->errorString());
    return;
  }
}
