#include "Method.h"

#include "BalloonTip.h"
#include "dlgdatabase.h"
#include "filesystemwatcher.h"
#include "mainwindow.h"
#include "plistparser.h"
#include "plistserializer.h"
#include "ui_dlgdatabase.h"
#include "ui_mainwindow.h"

extern MainWindow* mw_one;
extern QString SaveFileName, strIniFile, strAppName, strAppExePath;
extern QString CurVerison, ocVer, ocVerDev, ocFrom, ocFromDev, strOCFrom,
    strOCFromDev;
extern bool blDEV;
extern bool zh_cn;
extern QVariantMap mapTatol;
extern QProgressBar* progBar;

QString strACPI;
QString strKexts;
QString strDrivers;
QString strTools;
QStringList boolTypeList, intTypeList, dataTypeList;

Method::Method(QWidget* parent) : QMainWindow(parent) {
  manager = new QNetworkAccessManager(this);
  connect(manager, SIGNAL(finished(QNetworkReply*)), this,
          SLOT(replyFinished(QNetworkReply*)));
  managerDownLoad = new QNetworkAccessManager(this);
  myfile = new QFile(this);
  tempDir = QDir::homePath() + "/tempocat/";

  errorInfo = tr("Possible reasons:") + "\n\n" + tr("1.Network or URL error!") +
              "\n\n" +
              tr("2.The Github API is not accessed by token.(A limit of 60 "
                 "visits per hour). ") +
              "\n\n" +
              tr("3.Access the Github API via a token, but the token has a "
                 "problem (expired or incorrect).");
}

QStringList Method::getDLUrlList(QString url) {
  QString str_html = mw_one->dlgSyncOC->getKextHtmlInfo(url, false);
  if (str_html == "") {
    blBreak = true;
    return QStringList() << "";
  }
  QTextEdit* htmlEdit = new QTextEdit;
  htmlEdit->setPlainText(str_html);
  QStringList list1, list2;
  bool isRelease = false;
  for (int i = 0; i < htmlEdit->document()->lineCount(); i++) {
    QString strLine = getTextEditLineText(htmlEdit, i);
    if (strLine.trimmed().contains("RELEASE")) {
      isRelease = true;
      break;
    }
  }
  if (isRelease) {
    if (mw_one->dlgSyncOC->ui->btnGetOC->isEnabled()) {
      for (int i = 0; i < htmlEdit->document()->lineCount(); i++) {
        QString strLine = getTextEditLineText(htmlEdit, i);
        if (strLine.trimmed().contains("/releases/download/") &&
            strLine.trimmed().contains("RELEASE")) {
          list1 = strLine.split("\"");
          if (list1.count() > 1) {
            list2.append("https://github.com" + list1.at(1));
          }
          break;
        }
      }
    } else {
      if (mw_one->ui->actionDEBUG->isChecked()) {
        for (int i = 0; i < htmlEdit->document()->lineCount(); i++) {
          QString strLine = getTextEditLineText(htmlEdit, i);
          if (strLine.trimmed().contains("/releases/download/") &&
              strLine.trimmed().contains("OpenCore") &&
              strLine.trimmed().contains("DEBUG")) {
            list1 = strLine.split("\"");
            if (list1.count() > 1) {
              list2.append("https://github.com" + list1.at(1));
            }
            break;
          }
        }
      } else {
        for (int i = 0; i < htmlEdit->document()->lineCount(); i++) {
          QString strLine = getTextEditLineText(htmlEdit, i);
          if (strLine.trimmed().contains("/releases/download/") &&
              strLine.trimmed().contains("OpenCore") &&
              strLine.trimmed().contains("RELEASE")) {
            list1 = strLine.split("\"");
            if (list1.count() > 1) {
              list2.append("https://github.com" + list1.at(1));
            }
            break;
          }
        }
      }
    }
  } else {
    for (int i = 0; i < htmlEdit->document()->lineCount(); i++) {
      QString strLine = getTextEditLineText(htmlEdit, i);
      if (strLine.trimmed().contains("/releases/download/")) {
        list1 = strLine.split("\"");
        if (list1.count() > 1) {
          list2.append("https://github.com" + list1.at(1));
        }
        break;
      }
    }
  }

  if (list1.isEmpty() || list2.isEmpty()) {
    blBreak = true;
    return QStringList() << "";
  }

  return list2;
}

QString Method::getHTMLSource(QString URLSTR, bool writeFile) {
  const QString FILE_NAME = mw_one->strConfigPath + "code.txt";
  QString strProxy =
      mw_one->myDlgPreference->ui->comboBoxWeb->currentText().trimmed();
  URLSTR.replace("https://github.com/", strProxy);
  QUrl url(URLSTR);
  QNetworkAccessManager manager;

  if (mw_one->myDlgPreference->ui->chkProxy->isChecked()) {
    QNetworkProxy proxy;
    proxy.setType(QNetworkProxy::HttpProxy);
    proxy.setHostName(
        mw_one->myDlgPreference->ui->txtHostName->text().trimmed());
    int port = mw_one->myDlgPreference->ui->txtPort->text().trimmed().toInt();
    proxy.setPort(port);
    manager.setProxy(proxy);
  }

  QEventLoop loop;
  qDebug() << "Reading code form " << URLSTR;
  reply = manager.get(QNetworkRequest(url));
  isReply = true;
  QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
  progBar->setMaximum(0);
  mw_one->dlgSyncOC->on_ProgBarvalueChanged(progBar);
  loop.exec();
  isReply = false;

  if (blBreak) return "";

  QString code = reply->readAll();
  if (code == "") {
    mw_one->dlgSyncOC->on_btnStop_clicked();
    QMessageBox::critical(this, "", errorInfo);

    return "";
  }
  if (writeFile) {
    QFile file(FILE_NAME);
    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);
    out << code;
    file.close();
    // qDebug() << "Finished, the code have written to " << FILE_NAME;
  }

  return code;
}

void Method::getAllFolds(const QString& foldPath, QStringList& folds) {
  QDirIterator it(foldPath, QDir::Dirs | QDir::NoDotAndDotDot,
                  QDirIterator::Subdirectories);
  while (it.hasNext()) {
    it.next();
    QFileInfo fileInfo = it.fileInfo();
    folds << fileInfo.absoluteFilePath();
  }
}

void Method::getAllFiles(const QString& foldPath, QStringList& folds,
                         const QStringList& formats) {
  QDirIterator it(foldPath, QDir::Files | QDir::NoDotAndDotDot,
                  QDirIterator::Subdirectories);
  while (it.hasNext()) {
    it.next();
    QFileInfo fileInfo = it.fileInfo();
    if (formats.contains(fileInfo.suffix())) {  // 检测格式，按需保存
      folds << fileInfo.absoluteFilePath();
    }
  }
}

void Method::finishKextUpdate(bool blDatabase) {
  QStringList kextList, tempList;
  QStringList list0 = DirToFileList(tempDir, "*.kext");
  for (int i = 0; i < list0.count(); i++) {
    kextList.append(tempDir + list0.at(i));
  }

  QStringList folds;
  getAllFolds(tempDir, folds);
  // qDebug() << folds;
  for (int i = 0; i < folds.count(); i++) {
    QString strdir = folds.at(i);
    if (!strdir.contains("Debug")) {
      QStringList list3 = DirToFileList(strdir, "*.kext");
      for (int n = 0; n < list3.count(); n++)
        kextList.append(strdir + "/" + list3.at(n));
    }
  }

  QStringList files;
  QStringList fmt = QString("plist;jpg").split(';');
  getAllFiles(tempDir, files, fmt);
  // qDebug() << files;

  tempList = kextList;
  for (int i = 0; i < kextList.count(); i++) {
    QString str = kextList.at(i);
    for (int j = 0; j < tempList.count(); j++) {
      QString strTemp = tempList.at(j);
      if (str.length() > strTemp.length() && str.contains(strTemp)) {
        kextList.removeAt(i);
        i--;
      }
    }
  }

  if (blDatabase) {
    for (int i = 0; i < kextList.count(); i++) {
      QString dirSource, dirTargetDatabase;
      dirSource = kextList.at(i);
      QString Name = getFileName(dirSource);
      dirTargetDatabase =
          QDir::homePath() + "/.ocat/Database/EFI/OC/Kexts/" + Name;
      mw_one->copyDirectoryFiles(dirSource, dirTargetDatabase, true);
    }
  } else {
    QStringList list;
    list = DirToFileList(QDir::homePath() + "/.ocat/Database/EFI/OC/Kexts/",
                         "*.kext");
    for (int i = 0; i < list.count(); i++) {
      QString dirSource, dirTarget;
      dirSource =
          QDir::homePath() + "/.ocat/Database/EFI/OC/Kexts/" + list.at(i);
      QString Name = getFileName(dirSource);
      dirTarget = strKexts + Name;

      for (int j = 0; j < mw_one->dlgSyncOC->sourceKexts.count(); j++) {
        QString str_1 = mw_one->dlgSyncOC->ui->tableKexts->item(j, 3)->text();
        if (Name == str_1 && mw_one->dlgSyncOC->chkList.at(j)->isChecked())
          mw_one->copyDirectoryFiles(dirSource, dirTarget, true);

        qDebug() << dirSource << dirTarget;
      }
    }
  }

  mw_one->dlgSyncOC->ui->btnCheckUpdate->setEnabled(true);

  mw_one->dlgSyncOC->ui->labelShowDLInfo->setVisible(false);
  if (!blDatabase) mw_one->checkFiles(mw_one->ui->table_kernel_add);
  mw_one->repaint();
}

void Method::kextUpdate() {
  if (!mw_one->ui->actionUpgrade_OC->isEnabled()) return;
  if (mw_one->ui->table_kernel_add->rowCount() == 0) return;
  mw_one->myDlgPreference->refreshKextUrl(true);
  blBreak = false;
  isReply = false;

  mw_one->dlgSyncOC->ui->btnCheckUpdate->setEnabled(false);
  mw_one->repaint();
  QString test = "https://github.com/acidanthera/Lilu";

  for (int i = 0; i < mw_one->dlgSyncOC->sourceKexts.count(); i++) {
    if (blBreak) break;
    if (mw_one->dlgSyncOC->chkList.at(i)->isChecked()) {
      QString name =
          mw_one->dlgSyncOC->ui->tableKexts->item(i, 3)->text().trimmed();
      mw_one->dlgSyncOC->ui->tableKexts->setCurrentCell(i, 0);
      mw_one->dlgSyncOC->ui->tableKexts->setFocus();
      mw_one->dlgSyncOC->ui->tableKexts->setCellWidget(i, 3, progBar);

      kextName = name;
      if (!mw_one->dlgSyncOC->ui->chkKextsDev->isChecked()) {
        for (int j = 0;
             j < mw_one->myDlgPreference->ui->tableKextUrl->rowCount(); j++) {
          if (blBreak) break;
          QString txt = mw_one->myDlgPreference->ui->tableKextUrl->item(j, 0)
                            ->text()
                            .trimmed();
          test = mw_one->myDlgPreference->ui->tableKextUrl->item(j, 1)
                     ->text()
                     .trimmed();
          if (txt == name && test != "") {
            bool reGetUrl = true;
            QString strUrl;
            for (int m = 0; m < kextDLUrlList.count(); m++) {
              QString str_m = kextDLUrlList.at(m);
              QStringList list_m = str_m.split("|");
              if (list_m.at(0) == name) {
                reGetUrl = false;
                strUrl = list_m.at(1);
              }
            }
            reGetUrl = true;
            if (reGetUrl) {
              if (mw_one->myDlgPreference->ui->rbtnAPI->isChecked() ||
                  mw_one->myDlgPreference->ui->rbtnToken->isChecked())
                getLastReleaseFromUrl(test);
              if (mw_one->myDlgPreference->ui->rbtnWeb->isChecked())
                getLastReleaseFromHtml(test + "/releases");
            } else {
              startDownload(strUrl);
            }

            dlEnd = false;
            while (!dlEnd && !blBreak) {
              QCoreApplication::processEvents();
            }
          }
        }  // end for j=0
      }  // end !isDev
      else {
        QString strName = name;
        strName = strName.replace(".kext", 0);
        QString url = mw_one->dlgSyncOC->getKextDevDL(
            mw_one->dlgSyncOC->bufferJson, strName.trimmed());
        startDownload(url);

        dlEnd = false;
        while (!dlEnd && !blBreak) {
          QCoreApplication::processEvents();
        }
      }

    }  // end isChecked

  }  // end for i=0

  if (blBreak) return;
  finishKextUpdate(true);
}

void Method::downloadAllKexts() {
  blBreak = false;
  isReply = false;

  mw_one->repaint();
  QString test = "https://github.com/acidanthera/Lilu";

  for (int i = 0; i < mw_one->myDlgPreference->ui->tableKextUrl->rowCount();
       i++) {
    if (blBreak) break;

    QString name =
        mw_one->myDlgPreference->ui->tableKextUrl->item(i, 0)->text().trimmed();
    mw_one->myDlgPreference->ui->tableKextUrl->setCurrentCell(i, 1);
    mw_one->myDlgPreference->ui->tableKextUrl->setFocus();
    if (i > 0)
      mw_one->myDlgPreference->ui->tableKextUrl->removeCellWidget(i - 1, 1);
    mw_one->myDlgPreference->ui->tableKextUrl->setCellWidget(i, 1, progBar);

    kextName = name;

    if (blBreak) break;

    test =
        mw_one->myDlgPreference->ui->tableKextUrl->item(i, 1)->text().trimmed();

    bool reGetUrl = true;
    QString strUrl;
    for (int m = 0; m < kextDLUrlList.count(); m++) {
      QString str_m = kextDLUrlList.at(m);
      QStringList list_m = str_m.split("|");
      if (list_m.at(0) == name) {
        reGetUrl = false;
        strUrl = list_m.at(1);
      }
    }
    reGetUrl = true;
    if (reGetUrl) {
      if (mw_one->myDlgPreference->ui->rbtnAPI->isChecked() ||
          mw_one->myDlgPreference->ui->rbtnToken->isChecked())
        getLastReleaseFromUrl(test);
      if (mw_one->myDlgPreference->ui->rbtnWeb->isChecked())
        getLastReleaseFromHtml(test + "/releases");
    } else {
      startDownload(strUrl);
    }

    QElapsedTimer t;
    t.start();
    dlEnd = false;
    while (!dlEnd && !blBreak) {
      QCoreApplication::processEvents();
    }
  }

  if (blBreak) return;
  finishKextUpdate(true);
}

void Method::startDownload(QString strUrl) {
  if (blBreak) return;
  QString strTokyo, strSeoul, strOriginal, strSet, strTemp, strUrlOrg;
  strUrlOrg = strUrl;
  strOriginal = "https://github.com/";
  strTokyo = "https://download.fastgit.org/";
  strSeoul = "https://ghproxy.com/https://github.com/";
  strSet = mw_one->myDlgPreference->ui->comboBoxNet->currentText().trimmed();
  strTemp = strUrlOrg;
  strUrl = strTemp.replace("https://github.com/", strSet);

  QNetworkRequest request;
  request.setUrl(QUrl(strUrl));
  request.setHeader(QNetworkRequest::ContentTypeHeader,
                    "application/octet-stream");

  // github redirects the request, so this attribute must be set to true,
  // otherwise returns nothing from qt5.6

#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
  request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
#endif

  replyDL = managerDownLoad->get(request);
  isReplyDL = true;

  connect(replyDL, &QNetworkReply::readyRead, this,
          &Method::doProcessReadyRead);
  connect(replyDL, &QNetworkReply::finished, this, &Method::doProcessFinished);
  connect(replyDL, &QNetworkReply::downloadProgress, this,
          &Method::doProcessDownloadProgress);
  QObject::connect(replyDL, SIGNAL(error(QNetworkReply::NetworkError)), this,
                   SLOT(Quit()));

  QStringList list = strUrl.split("/");
  filename = list.at(list.length() - 1);

  QDir dir;
  if (dir.mkpath(tempDir)) {
  }
  QString file = tempDir + filename;

  myfile->setFileName(file);
  bool ret =
      myfile->open(QIODevice::WriteOnly | QIODevice::Truncate);  // 创建文件
  if (!ret) {
    mw_one->dlgSyncOC->ui->btnStop->click();
    QMessageBox::warning(this, tr("Warning"),
                         tr("File creation failed!") + "\n" + file + "\n\n" +
                             tr("Or if you are in a region that does not have "
                                "direct access to GitHub, please select the "
                                "mirror server in Preferences and try again."));
    return;
  }
  progBar->setValue(0);
  progBar->setMinimum(0);

  downloadTimer.start();
}

void Method::doProcessReadyRead() {
  while (!replyDL->atEnd()) {
    QByteArray ba = replyDL->readAll();
    myfile->write(ba);
  }
}

void Method::unZip(QString zipfile) {
  filename = zipfile;
  if (mw_one->win) {
    QProcess::execute(strAppExePath + "/unzip.exe",
                      QStringList()
                          << "-o" << tempDir + filename << "-d" << tempDir);
  } else
    QProcess::execute("unzip", QStringList() << "-o" << tempDir + filename
                                             << "-d" << tempDir);
  if (mw_one->dlgSyncOC->isCheckOC) {
    QString strSEFI = tempDir + "X64/EFI/";
    if (!QDir(strSEFI).exists() && blDEV) {
      QStringList files = DirToFileList(tempDir, "*.zip");
      qDebug() << files;
      for (int i = 0; i < files.count(); i++) {
        if (!files.at(i).contains("OpenCore")) {
          files.removeAt(i);
          i--;
        }
      }
      qDebug() << "OpenCore Files:" << files;

      if (mw_one->ui->actionDEBUG->isChecked()) {
        for (int i = 0; i < files.count(); i++) {
          if (files.at(i).contains("DEBUG")) filename = files.at(i);
        }
      } else {
        for (int i = 0; i < files.count(); i++) {
          if (files.at(i).contains("RELEASE")) filename = files.at(i);
        }
      }

      if (mw_one->win) {
        QProcess::execute(strAppExePath + "/unzip.exe",
                          QStringList()
                              << "-o" << tempDir + filename << "-d" << tempDir);
      } else
        QProcess::execute("unzip", QStringList() << "-o" << tempDir + filename
                                                 << "-d" << tempDir);
    }
    updateOpenCore();
  }
}

void Method::doProcessFinished() {
  if (!blCanBeUpdate) return;
  if (replyDL->error() == QNetworkReply::NoError) {
    myfile->flush();
    myfile->close();
    if (QFile(tempDir + filename).exists()) {
      unZip(filename);
    }
    dlEnd = true;
  } else {
    myfile->close();
    qDebug() << "There is an error in the network answer!";
  }

  if (mw_one->dlgSyncOC->isCheckOC) {
    delete progBar;
    mw_one->dlgSyncOC->isCheckOC = false;
    mw_one->dlgSyncOC->ui->btnGetOC->setEnabled(true);
    mw_one->dlgSyncOC->ui->btnGetLastOC->setEnabled(true);
  }
  isReplyDL = false;
}

void Method::updateOpenCore() {
  if (mw_one->dlgSyncOC->isCheckOC) {
    QList<bool> Results;
    QString tempDirBak = tempDir;
    QString strSEFI = tempDir + "X64/EFI/";
    QString fn = filename;
    QString dirName = fn.replace(".zip", "");

    if (!QDir(strSEFI).exists()) {
      strSEFI = tempDir + dirName + "/X64/EFI/";
      tempDir = tempDirBak + dirName + "/";
    }

    if (!QDir(strSEFI).exists()) {
      tempDir = tempDirBak;
      QMessageBox::information(
          this, "",
          tr("No update is currently available, or please check the update "
             "source for the OpenCore development version."));
      return;
    }

    QDir dir;
    dir.mkpath(mw_one->userDataBaseDir);
    dir.mkpath(mw_one->userDataBaseDir + "DEBUG/");
    dir.mkpath(mw_one->userDataBaseDir + "doc/");
    dir.mkpath(mw_one->userDataBaseDir + "BaseConfigs/");
    mw_one->deleteDirfile(mw_one->userDataBaseDir + "mac/");
    dir.mkpath(mw_one->userDataBaseDir + "mac/");
    mw_one->deleteDirfile(mw_one->userDataBaseDir + "win/");
    dir.mkpath(mw_one->userDataBaseDir + "win/");
    mw_one->deleteDirfile(mw_one->userDataBaseDir + "linux/");
    dir.mkpath(mw_one->userDataBaseDir + "linux/");

    if (!QDir(strSEFI).exists()) strSEFI = tempDir + "EFI/";
    QString strTEFI;
    if (!mw_one->ui->actionDEBUG->isChecked())
      strTEFI = mw_one->userDataBaseDir + "EFI/";
    else
      strTEFI = mw_one->userDataBaseDir + "DEBUG/EFI/";

    if (!QDir(strSEFI).exists()) Results.append(false);
    Results.append(mw_one->copyDirectoryFiles(strSEFI, strTEFI, true));

    // ACPI
    QString strSacpi = tempDir + "Docs/AcpiSamples/Binaries/";
    QString strTacpi = mw_one->userDataBaseDir + "EFI/OC/ACPI/";
    mw_one->copyDirectoryFiles(strSacpi, strTacpi, true);

    // Doc
    Results.append(mw_one->copyFileToPath(
        tempDir + "Docs/Configuration.pdf",
        mw_one->userDataBaseDir + "doc/Configuration.pdf", true));
    Results.append(mw_one->copyFileToPath(
        tempDir + "Docs/Differences.pdf",
        mw_one->userDataBaseDir + "doc/Differences.pdf", true));

    // Sample-plist
    Results.append(mw_one->copyFileToPath(
        tempDir + "Docs/Sample.plist",
        mw_one->userDataBaseDir + "BaseConfigs/Sample.plist", true));
    QString sa = tempDir + "Docs/SampleCustom.plist";
    if (!QFile(sa).exists()) sa = tempDir + "Docs/SampleFull.plist";
    Results.append(mw_one->copyFileToPath(
        sa, mw_one->userDataBaseDir + "BaseConfigs/SampleCustom.plist", true));

    // OC Validate
    if (!QFile(tempDir + "Utilities/ocvalidate/ocvalidate").exists()) {
      QMessageBox::information(this, "",
                               tr("Note: This version or update source does "
                                  "not contain Mac related files. This will "
                                  "affect the use of the APP under Mac.") +
                                   "\n\nocvalidate\nmacserial\nocpasswordgen");
    }
    mw_one->copyFileToPath(tempDir + "Utilities/ocvalidate/ocvalidate",
                           mw_one->userDataBaseDir + "mac/ocvalidate", true);

    if (!QFile(tempDir + "Utilities/ocvalidate/ocvalidate.exe").exists()) {
      QMessageBox::information(
          this, "",
          tr("Note: This version or update source does "
             "not contain Windows related files. This will "
             "affect the use of the APP under Windows.") +
              "\n\nocvalidate.exe\nmacserial.exe\nocpasswordgen.exe");
    }
    mw_one->copyFileToPath(tempDir + "Utilities/ocvalidate/ocvalidate.exe",
                           mw_one->userDataBaseDir + "win/ocvalidate.exe",
                           true);

    if (!QFile(tempDir + "Utilities/ocvalidate/ocvalidate.linux").exists()) {
      QMessageBox::information(
          this, "",
          tr("Note: This version or update source does "
             "not contain Linux related files. This will "
             "affect the use of the APP under Linux.") +
              "\n\nocvalidate.linux\nmacserial.linux\nocpasswordgen.linux");
    }

    mw_one->copyFileToPath(tempDir + "Utilities/ocvalidate/ocvalidate.linux",
                           mw_one->userDataBaseDir + "linux/ocvalidate", true);

    // Mac Serial
    mw_one->copyFileToPath(tempDir + "Utilities/macserial/macserial",
                           mw_one->userDataBaseDir + "mac/macserial", true);
    mw_one->copyFileToPath(tempDir + "Utilities/macserial/macserial.exe",
                           mw_one->userDataBaseDir + "win/macserial.exe", true);

    mw_one->copyFileToPath(tempDir + "Utilities/macserial/macserial.linux",
                           mw_one->userDataBaseDir + "linux/macserial", true);

    // OC Password Gen
    mw_one->copyFileToPath(tempDir + "Utilities/ocpasswordgen/ocpasswordgen",
                           mw_one->userDataBaseDir + "mac/ocpasswordgen", true);
    mw_one->copyFileToPath(
        tempDir + "Utilities/ocpasswordgen/ocpasswordgen.exe",
        mw_one->userDataBaseDir + "win/ocpasswordgen.exe", true);
    mw_one->copyFileToPath(
        tempDir + "Utilities/ocpasswordgen/ocpasswordgen.linux",
        mw_one->userDataBaseDir + "linux/ocpasswordgen", true);

    // Create Vault
    Results.append(mw_one->copyDirectoryFiles(
        tempDir + "/Utilities/CreateVault/",
        mw_one->userDataBaseDir + "mac/CreateVault/", true));

    bool isDo = true;
    for (int i = 0; i < Results.count(); i++) {
      if (Results.at(i) == false) isDo = false;
      qDebug() << Results.at(i) << QString::number(i + 1);
    }

    if (isDo) {
      QString file = filename;
      QStringList list = file.split("-");
      QString ver;

      if (list.count() == 3) {
        ver = list.at(1);
      }
      if (list.count() == 4) {
        ver = list.at(1) + " " + list.at(2);
      }

      QSettings Reg(strIniFile, QSettings::IniFormat);
      if (!blDEV) {
        Reg.setValue("ocVer", ver);
        ocVer = ver;
        if (mw_one->dlgSyncOC->ui->comboOCVersions->currentIndex() == 0) {
          Reg.setValue("maxVer", ver);
        }
      } else {
        Reg.setValue("ocVerDev", ver);
        ocVerDev = ver;
      }

      mw_one->changeOpenCore(blDEV);
      mw_one->dlgSyncOC->writeCheckStateINI();
      mw_one->dlgSyncOC->init_Sync_OC_Table();

      QMessageBox box;
      if (!blDEV)
        box.setText(tr("OpenCore Database has been successfully updated to") +
                    "  " + ocVer);
      else
        box.setText(tr("OpenCore Database has been successfully updated to") +
                    "  " + ocVerDev);
      box.exec();
    } else {
      if (blDEV) {
        QMessageBox::information(
            this, "",
            tr("No update is currently available, or please check the update "
               "source for the OpenCore development version."));
      }
    }
    tempDir = tempDirBak;
  }
}

void Method::doProcessDownloadProgress(qint64 recv_total,
                                       qint64 all_total)  // 显示
{
  if (blBreak) return;

  progBar->setMaximum(all_total);
  progBar->setValue(recv_total);
  mw_one->dlgSyncOC->on_ProgBarvalueChanged(progBar);

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

  mw_one->dlgSyncOC->ui->labelShowDLInfo->setText(
      tr("Download Progress") + " : " + GetFileSize(recv_total, 2) + " -> " +
      GetFileSize(all_total, 2) + "    " + strSpeed);

  if (recv_total == all_total) {
    if (recv_total < 1000) {
      blCanBeUpdate = false;
    } else
      blCanBeUpdate = true;
  }
}

void Method::getLastReleaseFromUrl(QString strUrl) {
  if (blBreak) return;
  // https://github.com/ic005k/" + strAppName;
  // https://api.github.com/repos/ic005k/" + strAppName + "/releases/latest
  QString strAPI =
      strUrl.replace("https://github.com/", "https://api.github.com/repos/") +
      "/releases/latest";
  QNetworkRequest quest;
  quest.setUrl(QUrl(strAPI));
  quest.setHeader(QNetworkRequest::UserAgentHeader, "RT-Thread ART");

  if (mw_one->myDlgPreference->ui->rbtnToken->isChecked()) {
    QString strToken = mw_one->myDlgPreference->ui->editToken->text().trimmed();
    if (strToken != "") {
      quest.setRawHeader("Authorization",
                         QString("token %1").arg(strToken).toUtf8());
      qDebug() << "strAPI=" << strAPI;
    }
  }

  manager->get(quest);
}

void Method::replyFinished(QNetworkReply* reply) {
  QString str = reply->readAll();
  parse_UpdateJSON(str);
  if (reply) reply->deleteLater();
}

void Method::parse_UpdateJSON(QString str) {
  QJsonParseError err_rpt;
  QJsonDocument root_Doc = QJsonDocument::fromJson(str.toUtf8(), &err_rpt);

  if (err_rpt.error != QJsonParseError::NoError) {
    mw_one->dlgSyncOC->on_btnStop_clicked();
    QMessageBox::critical(this, "", errorInfo);

    return;
  }

  QString Verison;
  QString strDLUrl;
  if (root_Doc.isObject()) {
    QJsonObject root_Obj = root_Doc.object();
    QVariantList list = root_Obj.value("assets").toArray().toVariantList();
    Verison = root_Obj.value("tag_name").toString();
    QStringList strDownloadUrlList;
    for (int i = 0; i < list.count(); i++) {
      QVariantMap map = list[i].toMap();
      strDownloadUrlList.append(map["browser_download_url"].toString());
    }

    for (int i = 0; i < strDownloadUrlList.count(); i++) {
      if (strDownloadUrlList.count() > 1) {
        QString str = strDownloadUrlList.at(i);
        if (!mw_one->dlgSyncOC->ui->btnCheckUpdate->isEnabled() ||
            !mw_one->myDlgPreference->ui->btnDownloadKexts->isEnabled()) {
          if (str.contains("RELEASE"))
            strDLUrl = str;
          else {
            QString str_n = kextName;
            if (str.contains(str_n.replace(".kext", "")) &&
                !str.toLower().contains("debug"))
              strDLUrl = str;
          }
        }

        if (!mw_one->dlgSyncOC->ui->btnGetOC->isEnabled()) {
          if (mw_one->ui->actionDEBUG->isChecked()) {
            if (str.contains("DEBUG")) strDLUrl = str;
          } else {
            if (str.contains("RELEASE")) strDLUrl = str;
          }
        }
      } else
        strDLUrl = strDownloadUrlList.at(0);
    }
  }
  strDLInfoList.clear();
  strDLInfoList = QStringList() << Verison << strDLUrl;
  qDebug() << strDLInfoList.at(0) << strDLInfoList.at(1);
  if (strDLUrl == "") {
    mw_one->dlgSyncOC->on_btnStop_clicked();
    QMessageBox::critical(this, "", errorInfo);

    return;
  }
  kextDLUrlList.append(kextName + "|" + strDLUrl);
  startDownload(strDLUrl);
}

void Method::getLastReleaseFromHtml(QString url) {
  QStringList strDownloadUrlList = getDLUrlList(url);
  if (strDownloadUrlList.at(0) == "") {
    blBreak = true;
    mw_one->dlgSyncOC->on_btnStop_clicked();
    return;
  }
  QString strDLUrl;
  for (int i = 0; i < strDownloadUrlList.count(); i++) {
    if (strDownloadUrlList.count() > 1) {
      QString str = strDownloadUrlList.at(i);
      if (!mw_one->dlgSyncOC->ui->btnCheckUpdate->isEnabled() ||
          !mw_one->myDlgPreference->ui->btnDownloadKexts->isEnabled()) {
        if (str.contains("RELEASE"))
          strDLUrl = str;
        else {
          QString str_n = kextName;
          if (str.contains(str_n.replace(".kext", "")) &&
              !str.toLower().contains("debug"))
            strDLUrl = str;
        }
      }

      if (!mw_one->dlgSyncOC->ui->btnGetOC->isEnabled()) {
        if (mw_one->ui->actionDEBUG->isChecked()) {
          if (str.contains("DEBUG")) strDLUrl = str;
        } else {
          if (str.contains("RELEASE")) strDLUrl = str;
        }
      }
    } else
      strDLUrl = strDownloadUrlList.at(0);
  }

  kextDLUrlList.append(kextName + "|" + strDLUrl);
  startDownload(strDLUrl);
}

void Method::delRightTableItem(QTableWidget* t0, QTableWidget* t) {
  if (t0->rowCount() == 0) return;
  mw_one->del_item(t0);
  if (t0->rowCount() == 0) {
    t->setRowCount(0);
  }
  if (t0->rowCount() > 0) emit t0->cellClicked(t0->currentRow(), 0);
  mw_one->setWindowModified(true);
  mw_one->updateIconStatus();
}

QObjectList Method::getAllToolButton(QObjectList lstUIControls) {
  QObjectList lstOfToolButton;
  foreach (QObject* obj, lstUIControls) {
    if (obj->metaObject()->className() == QStringLiteral("QToolButton")) {
      lstOfToolButton.append(obj);
    }
  }
  return lstOfToolButton;
}

QString Method::loadText(QString textFile) {
  QFileInfo fi(textFile);
  if (fi.exists()) {
    QFile file(textFile);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
      qDebug() << tr("Cannot read file %1:\n%2.")
                      .arg(QDir::toNativeSeparators(textFile),
                           file.errorString());

    } else {
      QTextStream in(&file);
      in.setCodec("UTF-8");
      QString text = in.readAll();
      return text;
    }
  }

  return "";
}

QString Method::getKextVersion(QString kextFile) {
  if (!QDir(kextFile).exists()) return tr("None");
  QString strInfo = kextFile + "/Contents/Info.plist";
  QTextEdit* txtEdit = new QTextEdit;
  txtEdit->setPlainText(loadText(strInfo));
  for (int i = 0; i < txtEdit->document()->lineCount(); i++) {
    QString str0 = getTextEditLineText(txtEdit, i).trimmed();
    str0.replace("</key>", "");
    str0.replace("<key>", "");
    if (str0 == "CFBundleVersion") {
      QString str1 = getTextEditLineText(txtEdit, i + 1).trimmed();
      str1.replace("<string>", "");
      str1.replace("</string>", "");
      return str1;
    }
  }

  return "";
}

QString Method::getTextEditLineText(QTextEdit* txtEdit, int i) {
  QTextBlock block = txtEdit->document()->findBlockByNumber(i);
  txtEdit->setTextCursor(QTextCursor(block));
  QString lineText = txtEdit->document()->findBlockByNumber(i).text().trimmed();
  return lineText;
}

bool Method::isKext(QString kextName) {
  QString str = kextName.mid(kextName.length() - 4, 4);
  if (str == "kext" || str == "dSYM")
    return true;
  else
    return false;
}

bool Method::isWhatFile(QString File, QString filter) {
  QFileInfo fi(File);
  if (fi.suffix() == filter) return true;

  return false;
}

QStringList Method::DirToFileList(QString strDir, QString strFilter) {
  QStringList list;
  QDir dir(strDir);
  if (!dir.exists()) {
    qInfo() << "path is non-existent...";
    return list;
  }
  if (strFilter == "*.kext")
    dir.setFilter(QDir::Dirs | QDir::NoSymLinks);
  else
    dir.setFilter(QDir::Files | QDir::NoSymLinks);
  QStringList filters;
  filters << strFilter;
  dir.setNameFilters(filters);
  list = dir.entryList();
  // qDebug() << list.count() << kextDir;
  return list;
}

bool Method::isEqualInList(QString str, QStringList list) {
  for (int i = 0; i < list.count(); i++) {
    if (str == list.at(i)) return true;
  }
  return false;
}

QString Method::getKextBin(QString kextName) {
  QString str0 = kextName.mid(0, kextName.length() - 5);
  QStringList tempList = str0.split("/");
  QString str1 = tempList.at(tempList.count() - 1);
  QString str2 = kextName + "/Contents/MacOS/" + str1;

  return str2;
}

void Method::setStatusBarTip(QWidget* w) {
  QString strStatus0 = w->toolTip();
  QString strStatus1;
  QStringList strList = strStatus0.split("----");
  if (strList.count() == 2) {
    QTextEdit* tempEdit = new QTextEdit;
    QLocale locale;
    if (locale.language() == QLocale::Chinese) {
      tempEdit->setText(strList.at(1));
      for (int m = 0; m < tempEdit->document()->lineCount(); m++) {
        QTextBlock block = tempEdit->document()->findBlockByNumber(m);
        tempEdit->setTextCursor(QTextCursor(block));
        QString lineText =
            tempEdit->document()->findBlockByNumber(m).text().trimmed();
        if (lineText.mid(0, 2) == "描述" || lineText.mid(0, 2) == "说明") {
          strStatus1 = lineText;
          break;
        }
      }

    } else {
      tempEdit->setText(strList.at(0));
      for (int m = 0; m < tempEdit->document()->lineCount(); m++) {
        QTextBlock block = tempEdit->document()->findBlockByNumber(m);
        tempEdit->setTextCursor(QTextCursor(block));
        QString lineText =
            tempEdit->document()->findBlockByNumber(m).text().trimmed();
        if (lineText.mid(0, 11) == "Description") {
          strStatus1 = lineText;
          break;
        }
      }
    }
  } else
    strStatus1 = strStatus0;
  w->setStatusTip(strStatus1);
}

void Method::set_nv_key(QString key, QString dataType) {
  bool re = false;

  for (int i = 0; i < mw_one->ui->table_nv_add->rowCount(); i++) {
    QString str;
    str = mw_one->ui->table_nv_add->item(i, 0)->text();
    if (str == key) {
      mw_one->ui->table_nv_add->setCurrentCell(i, 0);
      re = true;
    }
  }

  if (!re) {
    mw_one->on_btnNVRAMAdd_Add_clicked();

    mw_one->ui->table_nv_add->setItem(mw_one->ui->table_nv_add->rowCount() - 1,
                                      0, new QTableWidgetItem(key));

    QTableWidgetItem* newItem1 = new QTableWidgetItem(dataType);
    newItem1->setTextAlignment(Qt::AlignCenter);
    mw_one->ui->table_nv_add->setItem(mw_one->ui->table_nv_add->rowCount() - 1,
                                      1, newItem1);

    // 保存数据
    mw_one->write_ini(mw_one->ui->table_nv_add0, mw_one->ui->table_nv_add,
                      mw_one->ui->table_nv_add0->currentRow());
  }
}

void Method::goACPITable(QTableWidget* table) {
  // ACPI
  if (table == mw_one->ui->table_acpi_add) {
    mw_one->ui->listMain->setCurrentRow(0);
    mw_one->ui->listSub->setCurrentRow(0);
  }

  if (table == mw_one->ui->table_acpi_del) {
    mw_one->ui->listMain->setCurrentRow(0);
    mw_one->ui->listSub->setCurrentRow(1);
  }

  if (table == mw_one->ui->table_acpi_patch) {
    mw_one->ui->listMain->setCurrentRow(0);
    mw_one->ui->listSub->setCurrentRow(2);
  }
}

void Method::goBooterTable(QTableWidget* table) {
  // Booter
  if (table == mw_one->ui->table_booter) {
    mw_one->ui->listMain->setCurrentRow(1);
    mw_one->ui->listSub->setCurrentRow(0);
  }
  if (table == mw_one->ui->table_Booter_patch) {
    mw_one->ui->listMain->setCurrentRow(1);
    mw_one->ui->listSub->setCurrentRow(1);
  }
}

void Method::goDPTable(QTableWidget* table) {
  // DP
  if (table == mw_one->ui->table_dp_add0) {
    mw_one->ui->listMain->setCurrentRow(2);
    mw_one->ui->listSub->setCurrentRow(0);
  }
  if (table == mw_one->ui->table_dp_add) {
    mw_one->ui->listMain->setCurrentRow(2);
    mw_one->ui->listSub->setCurrentRow(0);
  }
  if (table == mw_one->ui->table_dp_del0) {
    mw_one->ui->listMain->setCurrentRow(2);
    mw_one->ui->listSub->setCurrentRow(1);
  }
  if (table == mw_one->ui->table_dp_del) {
    mw_one->ui->listMain->setCurrentRow(2);
    mw_one->ui->listSub->setCurrentRow(1);
  }
}

void Method::goKernelTable(QTableWidget* table) {
  // Kernel
  if (table == mw_one->ui->table_kernel_Force) {
    mw_one->ui->listMain->setCurrentRow(3);
    mw_one->ui->listSub->setCurrentRow(2);
  }

  if (table == mw_one->ui->table_kernel_add) {
    mw_one->ui->listMain->setCurrentRow(3);
    mw_one->ui->listSub->setCurrentRow(0);
  }

  if (table == mw_one->ui->table_kernel_block) {
    mw_one->ui->listMain->setCurrentRow(3);
    mw_one->ui->listSub->setCurrentRow(1);
  }

  if (table == mw_one->ui->table_kernel_patch) {
    mw_one->ui->listMain->setCurrentRow(3);
    mw_one->ui->listSub->setCurrentRow(3);
  }
}

void Method::goMiscTable(QTableWidget* table) {
  // Misc
  if (table == mw_one->ui->tableBlessOverride) {
    mw_one->ui->listMain->setCurrentRow(4);
    mw_one->ui->listSub->setCurrentRow(3);
  }

  if (table == mw_one->ui->tableEntries) {
    mw_one->ui->listMain->setCurrentRow(4);
    mw_one->ui->listSub->setCurrentRow(4);
  }

  if (table == mw_one->ui->tableTools) {
    mw_one->ui->listMain->setCurrentRow(4);
    mw_one->ui->listSub->setCurrentRow(5);
  }
}

void Method::goNVRAMTable(QTableWidget* table) {
  // NVRAM
  if (table == mw_one->ui->table_nv_add0) {
    mw_one->ui->listMain->setCurrentRow(5);
    mw_one->ui->listSub->setCurrentRow(0);
  }

  if (table == mw_one->ui->table_nv_add) {
    mw_one->ui->listMain->setCurrentRow(5);
    mw_one->ui->listSub->setCurrentRow(0);
  }

  if (table == mw_one->ui->table_nv_del0) {
    mw_one->ui->listMain->setCurrentRow(5);
    mw_one->ui->listSub->setCurrentRow(1);
  }

  if (table == mw_one->ui->table_nv_del) {
    mw_one->ui->listMain->setCurrentRow(5);
    mw_one->ui->listSub->setCurrentRow(1);
  }

  if (table == mw_one->ui->table_nv_ls0) {
    mw_one->ui->listMain->setCurrentRow(5);
    mw_one->ui->listSub->setCurrentRow(2);
  }

  if (table == mw_one->ui->table_nv_ls) {
    mw_one->ui->listMain->setCurrentRow(5);
    mw_one->ui->listSub->setCurrentRow(2);
  }
}

void Method::goTable(QTableWidget* table) {
  goACPITable(table);

  goBooterTable(table);

  goDPTable(table);

  goKernelTable(table);

  goMiscTable(table);

  goNVRAMTable(table);

  // PI
  if (table == mw_one->ui->tableDevices) {
    mw_one->ui->listMain->setCurrentRow(6);
    mw_one->ui->listSub->setCurrentRow(2);
  }

  // UEFI
  if (table == mw_one->ui->table_uefi_drivers) {
    mw_one->ui->listMain->setCurrentRow(7);
    mw_one->ui->listSub->setCurrentRow(3);
  }

  if (table == mw_one->ui->table_uefi_ReservedMemory) {
    mw_one->ui->listMain->setCurrentRow(7);
    mw_one->ui->listSub->setCurrentRow(8);
  }

  if (table == mw_one->ui->table_uefi_Unload) {
    mw_one->ui->listMain->setCurrentRow(7);
    mw_one->ui->listSub->setCurrentRow(9);
  }
}

QString Method::copyDrivers(QString pathSource, QString pathTarget) {
  // OC/Drivers

  QDir dir;
  QString strDatabase;
  QString pathOCDrivers = pathTarget + "OC/Drivers/";
  if (dir.mkpath(pathOCDrivers)) {
  }
  int pathCol =
      mw_one->dlgSyncOC->get_PathCol(mw_one->ui->table_uefi_drivers, "Path");
  for (int i = 0; i < mw_one->ui->table_uefi_drivers->rowCount(); i++) {
    QString file = mw_one->ui->table_uefi_drivers->item(i, pathCol)->text();
    QString str0 = pathSource + "EFI/OC/Drivers/" + file;
    if (!str0.contains("#")) {
      QFileInfo fi(str0);
      if (fi.exists())
        QFile::copy(str0, pathOCDrivers + file);
      else
        strDatabase = strDatabase + "EFI/OC/Drivers/" + file + "\n";
    }
  }

  return strDatabase;
}

QString Method::copyKexts(QString pathSource, QString pathTarget) {
  // OC/Kexts

  QDir dir;
  QString strDatabase;
  QString pathOCKexts = pathTarget + "OC/Kexts/";
  if (dir.mkpath(pathOCKexts)) {
  }
  int pathCol = mw_one->dlgSyncOC->get_PathCol(mw_one->ui->table_kernel_add,
                                               "BundlePath");
  for (int i = 0; i < mw_one->ui->table_kernel_add->rowCount(); i++) {
    QString file = mw_one->ui->table_kernel_add->item(i, pathCol)->text();
    QString str0 = pathSource + "EFI/OC/Kexts/" + file;
    QDir kextDir(str0);

    if (!str0.contains("#")) {
      if (kextDir.exists())
        mw_one->copyDirectoryFiles(str0, pathOCKexts + file, true);
      else
        strDatabase = strDatabase + "EFI/OC/Kexts/" + file + "\n";
    }
  }

  return strDatabase;
}

QString Method::copyACPI(QString pathSource, QString pathTarget) {
  // OC/ACPI

  QDir dir;
  QString strDatabase;
  QString pathOCACPI = pathTarget + "OC/ACPI/";
  if (dir.mkpath(pathOCACPI)) {
  }

  int pathCol =
      mw_one->dlgSyncOC->get_PathCol(mw_one->ui->table_acpi_add, "Path");
  for (int i = 0; i < mw_one->ui->table_acpi_add->rowCount(); i++) {
    QString file = mw_one->ui->table_acpi_add->item(i, pathCol)->text();
    QString file1 = pathSource + "EFI/OC/ACPI/" + file;
    if (QFile(file1).exists())
      QFile::copy(pathSource + "EFI/OC/ACPI/" + file, pathOCACPI + file);
    else
      strDatabase = strDatabase + "EFI/OC/ACPI/" + file + "\n";
  }

  return strDatabase;
}

QString Method::copyTools(QString pathSource, QString pathTarget) {
  // OC/Tools

  QDir dir;
  QString strDatabase;
  QString pathOCTools = pathTarget + "OC/Tools/";
  if (dir.mkpath(pathOCTools)) {
  }
  int pathCol = mw_one->dlgSyncOC->get_PathCol(mw_one->ui->tableTools, "Path");
  for (int i = 0; i < mw_one->ui->tableTools->rowCount(); i++) {
    QString file = mw_one->ui->tableTools->item(i, pathCol)->text();
    QString str0 = pathSource + "EFI/OC/Tools/" + file;
    if (!str0.contains("#")) {
      QFileInfo fi(str0);
      if (fi.exists())
        QFile::copy(str0, pathOCTools + file);
      else
        strDatabase = strDatabase + "EFI/OC/Tools/" + file + "\n";
    }
  }

  return strDatabase;
}

void Method::generateEFI(QString file) {
  QDir dir;
  QString strDatabase;

  QString str = QDir::homePath() + "/.ocat/Database/";
  QString pathSource;
  if (!mw_one->ui->actionDEBUG->isChecked())
    pathSource = mw_one->userDataBaseDir;
  else
    pathSource = mw_one->userDataBaseDir + "DEBUG/";

  QString pathTarget = QDir::homePath() + "/Desktop/EFI/";

  mw_one->deleteDirfile(pathTarget);

  if (dir.mkpath(pathTarget)) {
  }

  // BOOT
  QString pathBoot = pathTarget + "BOOT/";
  if (dir.mkpath(pathBoot)) {
  }
  QFile::copy(pathSource + "EFI/BOOT/BOOTx64.efi", pathBoot + "BOOTx64.efi");

  // ACPI
  strDatabase = copyACPI(str, pathTarget) + strDatabase;

  // Drivers
  strDatabase = copyDrivers(pathSource, pathTarget) + strDatabase;

  // Kexts
  strDatabase = copyKexts(str, pathTarget) + strDatabase;

  // OC/Resources
  QString pathOCResources = pathTarget + "OC/Resources/";
  mw_one->copyDirectoryFiles(str + "EFI/OC/Resources/", pathOCResources, true);

  // Tools
  strDatabase = copyTools(pathSource, pathTarget) + strDatabase;

  // OC/OpenCore.efi
  QFile::copy(pathSource + "EFI/OC/OpenCore.efi",
              pathTarget + "OC/OpenCore.efi");

  // OC/config.plist
  mw_one->SavePlist(pathTarget + "OC/config.plist");

  QString strFrom = "\n\n" + tr("From") + " : " + file;
  QMessageBox box;
  if (strDatabase != "")
    box.setText(tr("Finished generating the EFI folder on the desktop.") +
                "\n" +
                tr("The following files do not exist in the database at the "
                   "moment, please add them yourself:") +
                "\n" + strDatabase + strFrom);
  else
    box.setText(tr("Finished generating the EFI folder on the desktop.") +
                strFrom);

  mw_one->setFocus();
  box.exec();
  mw_one->ui->mycboxFind->setFocus();
  mw_one->openFile(pathTarget + "OC/config.plist");
  mw_one->ui->actionUpgrade_OC->setEnabled(true);
}

void Method::on_btnExportMaster() {
  QFileDialog fd;
  QString defname;
  int index = mw_one->ui->tabTotal->currentIndex();

  defname = getTabTextName(index);

  QString FileName =
      fd.getSaveFileName(this, tr("Save File"), defname,
                         tr("Config file(*.plist);;All files(*.*)"));
  if (FileName.isEmpty()) return;

  QVariantMap OpenCore;

  switch (index) {
    case 0:
      OpenCore["ACPI"] = mw_one->SaveACPI();

      break;

    case 1:
      OpenCore["Booter"] = mw_one->SaveBooter();
      break;

    case 2:
      OpenCore["DeviceProperties"] = mw_one->SaveDeviceProperties();
      break;

    case 3:
      OpenCore["Kernel"] = mw_one->SaveKernel();
      break;

    case 4:
      OpenCore["Misc"] = mw_one->SaveMisc();
      break;

    case 5:
      OpenCore["NVRAM"] = mw_one->SaveNVRAM();
      break;

    case 6:
      OpenCore["PlatformInfo"] = mw_one->SavePlatformInfo();
      break;

    case 7:
      OpenCore["UEFI"] = mw_one->SaveUEFI();
      break;
  }

  PListSerializer::toPList(OpenCore, FileName);
}

QString Method::getTabTextName(int index) {
  for (int i = 0; i < mw_one->ui->tabTotal->tabBar()->count(); i++) {
    if (i == index) {
      return mw_one->ui->tabTotal->tabText(index) + ".plist";
      break;
    }
  }

  return "";
}

void Method::on_btnImportMaster() {
  QFileDialog fd;
  QString defname;
  int index = mw_one->ui->tabTotal->currentIndex();
  defname = getTabTextName(index);

  QString FileName =
      fd.getOpenFileName(this, tr("Open File"), defname,
                         tr("Config file(*.plist);;All files(*.*)"));
  if (FileName.isEmpty()) return;

  mw_one->loading = true;

  QFile file(FileName);
  QVariantMap map = PListParser::parsePList(&file).toMap();

  switch (index) {
    case 0:
      // ACPI
      mw_one->init_Table(0);

      mw_one->ParserACPI(map);

      break;

    case 1:
      // Booter
      mw_one->init_Table(1);

      mw_one->ParserBooter(map);
      break;

    case 2:
      // DP
      mw_one->init_Table(2);

      mw_one->ParserDP(map);
      break;

    case 3:
      // Kernel
      mw_one->init_Table(3);

      mw_one->ParserKernel(map, "Add");
      mw_one->ParserKernel(map, "Block");
      mw_one->ParserKernel(map, "Emulate");
      mw_one->ParserKernel(map, "Force");
      mw_one->ParserKernel(map, "Patch");
      mw_one->ParserKernel(map, "Quirks");
      mw_one->ParserKernel(map, "Scheme");
      break;

    case 4:
      // Misc
      mw_one->init_Table(4);

      mw_one->ParserMisc(map);
      break;

    case 5:
      // NVRAM
      mw_one->init_Table(5);

      mw_one->ParserNvram(map);
      break;

    case 6:
      // PI
      mw_one->init_Table(6);

      mw_one->ParserPlatformInfo(map);
      break;

    case 7:
      // UEFI
      mw_one->init_Table(7);

      mw_one->ParserUEFI(map);
      break;
  }

  mw_one->loading = false;
}

void Method::findDP(QTableWidget* t, QString findText) {
  // DP
  if (t == mw_one->ui->table_dp_add0) {
    if (t->rowCount() > 0) {
      for (int j = 0; j < t->rowCount(); j++) {
        t->setCurrentCell(j, 0);
        mw_one->findTable(mw_one->ui->table_dp_add, findText);
      }
    }
  }

  if (t == mw_one->ui->table_dp_del0) {
    if (t->rowCount() > 0) {
      for (int j = 0; j < t->rowCount(); j++) {
        t->setCurrentCell(j, 0);
        mw_one->findTable(mw_one->ui->table_dp_del, findText);
      }
    }
  }
}

void Method::findNVRAM(QTableWidget* t, QString findText) {
  // NVRAM
  if (t == mw_one->ui->table_nv_add0) {
    if (t->rowCount() > 0) {
      for (int j = 0; j < t->rowCount(); j++) {
        t->setCurrentCell(j, 0);
        mw_one->findTable(mw_one->ui->table_nv_add, findText);
      }
    }
  }

  if (t == mw_one->ui->table_nv_del0) {
    if (t->rowCount() > 0) {
      for (int j = 0; j < t->rowCount(); j++) {
        t->setCurrentCell(j, 0);
        mw_one->findTable(mw_one->ui->table_nv_del, findText);
      }
    }
  }

  if (t == mw_one->ui->table_nv_ls0) {
    if (t->rowCount() > 0) {
      for (int j = 0; j < t->rowCount(); j++) {
        t->setCurrentCell(j, 0);
        mw_one->findTable(mw_one->ui->table_nv_ls, findText);
      }
    }
  }
}

void Method::findTable(QString findText) {
  // Table  2
  mw_one->listOfTableWidget.clear();
  mw_one->listOfTableWidget =
      mw_one->getAllTableWidget(mw_one->getAllUIControls(mw_one->ui->tabTotal));
  mw_one->listOfTableWidgetResults.clear();
  for (int i = 0; i < mw_one->listOfTableWidget.count(); i++) {
    QTableWidget* t;
    t = (QTableWidget*)mw_one->listOfTableWidget.at(i);

    // DP
    findDP(t, findText);

    // NVRAM
    findNVRAM(t, findText);

    if (t != mw_one->ui->table_dp_add && t != mw_one->ui->table_dp_del &&
        t != mw_one->ui->table_nv_add && t != mw_one->ui->table_nv_del &&
        t != mw_one->ui->table_nv_ls)
      mw_one->findTable(t, findText);
  }
}

void Method::UpdateStatusBarInfo() {
  QObjectList listTable;
  QTableWidget* t;

  QWidget* w = mw_one->getSubTabWidget(mw_one->ui->listMain->currentRow(),
                                       mw_one->ui->listSub->currentRow());
  listTable = mw_one->getAllTableWidget(mw_one->getAllUIControls(w));

  if (listTable.count() > 1) {
    for (int i = 0; i < listTable.count(); i++) {
      t = (QTableWidget*)listTable.at(i);
      if (!t->currentIndex().isValid()) return;
      if (t->hasFocus())
        emit t->cellClicked(t->currentRow(), t->currentColumn());
    }
  } else if (listTable.count() == 1) {
    t = (QTableWidget*)listTable.at(0);
    if (!t->currentIndex().isValid()) return;
    emit t->cellClicked(t->currentRow(), t->currentColumn());
  }
}

void Method::addFileSystemWatch(QString strOpenFile) {
  QString strPath;

  QFileInfo fi(strOpenFile);

  strPath = fi.path();
  strACPI = strPath + "/ACPI/";
  strKexts = strPath + "/Kexts/";
  strDrivers = strPath + "/Drivers/";
  strTools = strPath + "/Tools/";

  FileSystemWatcher::addWatchPath(strACPI);
  FileSystemWatcher::addWatchPath(strKexts);
  FileSystemWatcher::addWatchPath(strDrivers);
  FileSystemWatcher::addWatchPath(strTools);
}

void Method::removeFileSystemWatch(QString strOpenFile) {
  QString strPath;

  QFileInfo fi(strOpenFile);
  if (!fi.exists()) return;

  strPath = fi.path();
  strACPI = strPath + "/ACPI/";
  strKexts = strPath + "/Kexts/";
  strDrivers = strPath + "/Drivers/";
  strTools = strPath + "/Tools/";

  FileSystemWatcher::removeWatchPath(strACPI);
  FileSystemWatcher::removeWatchPath(strKexts);
  FileSystemWatcher::removeWatchPath(strDrivers);
  FileSystemWatcher::removeWatchPath(strTools);
}

QStringList Method::delDuplication(QStringList FileName, QTableWidget* table,
                                   int col) {
  for (int i = 0; i < table->rowCount(); i++) {
    QString str0 = table->item(i, col)->text().trimmed();
    for (int j = 0; j < FileName.count(); j++) {
      QFileInfo fi(FileName.at(j));
      if (str0 == fi.fileName()) {
        FileName.removeAt(j);
      }
    }
  }
  return FileName;
}

void Method::OCValidationProcessing() {
  return;
  if (mw_one->ui->chkPickerAudioAssist->isChecked())
    mw_one->ui->chkAudioSupport->setChecked(true);

  //
  for (int i = 0; i < mw_one->ui->table_uefi_drivers->rowCount(); i++) {
    QString str = mw_one->ui->table_uefi_drivers->item(i, 0)->text().trimmed();
    if (str == "Ps2KeyboardDxe.efi")
      mw_one->ui->chkKeySupport->setChecked(true);
  }
}

QString Method::GetFileSize(const qint64& size, int precision) {
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

void Method::TextEditToFile(QTextEdit* txtEdit, QString fileName) {
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

QString Method::getFileName(QString file) {
  QStringList list = file.split("/");
  return list.at(list.count() - 1);
}

void Method::cancelKextUpdate() {
  if (isReply) {
    emit reply->finished();
    isReply = false;
  }

  if (isReplyDL) {
    replyDL->error();
    replyDL->abort();
    isReplyDL = false;
  }

  blBreak = true;
}

void Method::writeLeftTable(QTableWidget* t0, QTableWidget* t) {
  if (!t0->currentIndex().isValid()) return;
  if (mw_one->blReadLeftTable) {
    return;
  }
  QStringList listAdd;
  if (t0 == mw_one->ui->table_nv_add0) listAdd = mw_one->listNVRAMAdd;
  if (t0 == mw_one->ui->table_dp_add0) listAdd = mw_one->listDPAdd;
  QString strLeft = t0->currentItem()->text().trimmed();
  for (int i = 0; i < listAdd.count(); i++) {
    QString str = listAdd.at(i);
    QStringList list = str.split("*|*");
    if (list.count() == 4) {
      if (strLeft == list.at(0)) {
        listAdd.removeAt(i);
        i--;
      }
    }
  }

  for (int i = 0; i < t->rowCount(); i++) {
    listAdd.append(strLeft + "*|*" + t->item(i, 0)->text().trimmed() + "*|*" +
                   t->item(i, 1)->text().trimmed() + "*|*" +
                   t->item(i, 2)->text().trimmed());
  }

  if (t0 == mw_one->ui->table_nv_add0) mw_one->listNVRAMAdd = listAdd;
  if (t0 == mw_one->ui->table_dp_add0) mw_one->listDPAdd = listAdd;
}

void Method::writeLeftTableOnlyValue(QTableWidget* t0, QTableWidget* t) {
  if (!t0->currentIndex().isValid()) return;
  if (mw_one->blReadLeftTable) {
    return;
  }
  QStringList listAdd;
  if (t0 == mw_one->ui->table_nv_del0) listAdd = mw_one->listNVRAMDel;
  if (t0 == mw_one->ui->table_nv_ls0) listAdd = mw_one->listNVRAMLs;
  if (t0 == mw_one->ui->table_dp_del0) listAdd = mw_one->listDPDel;
  QString strLeft = t0->currentItem()->text().trimmed();
  for (int i = 0; i < listAdd.count(); i++) {
    QString str = listAdd.at(i);
    QStringList list = str.split("*|*");
    if (list.count() == 3) {
      if (strLeft == list.at(1) && t0->objectName() == list.at(0)) {
        listAdd.removeAt(i);
        i--;
      }
    }
  }

  for (int i = 0; i < t->rowCount(); i++) {
    listAdd.append(t0->objectName() + "*|*" + strLeft + "*|*" +
                   t->item(i, 0)->text().trimmed());
  }

  if (t0 == mw_one->ui->table_nv_del0) mw_one->listNVRAMDel = listAdd;
  if (t0 == mw_one->ui->table_nv_ls0) mw_one->listNVRAMLs = listAdd;
  if (t0 == mw_one->ui->table_dp_del0) mw_one->listDPDel = listAdd;
}

void Method::kextPreset() {
  mw_one->dlgPresetValues->blKext = true;

  mw_one->dlgPresetValues->blACPIPatch = false;
  mw_one->dlgPresetValues->blDPAdd = false;
  mw_one->dlgPresetValues->blKernelPatch = false;
  mw_one->dlgPresetValues->blNVDelete = false;
  mw_one->dlgPresetValues->blNVLegacy = false;
  mw_one->dlgPresetValues->blNVAdd = false;

  mw_one->dlgPresetValues->listKextPreset = DirToFileList(
      QDir::homePath() + "/.ocat/Database/EFI/OC/Kexts/", "*.kext");
  mw_one->dlgPresetValues->ui->listPreset->clear();
  mw_one->dlgPresetValues->ui->listPreset->addItems(
      mw_one->dlgPresetValues->listKextPreset);
  for (int i = 0; i < mw_one->dlgPresetValues->ui->listPreset->count(); i++) {
    mw_one->dlgPresetValues->ui->listPreset->item(i)->setCheckState(
        Qt::Unchecked);
    QString str0 =
        mw_one->dlgPresetValues->ui->listPreset->item(i)->text().trimmed();
    for (int j = 0; j < mw_one->ui->table_kernel_add->rowCount(); j++) {
      QString str = mw_one->ui->table_kernel_add->item(j, 0)->text().trimmed();
      if (str0 == str)
        mw_one->dlgPresetValues->ui->listPreset->item(i)->setCheckState(
            Qt::Checked);
    }
  }
  mw_one->dlgPresetValues->setModal(true);
  mw_one->dlgPresetValues->show();
}

void Method::mount_esp_mac(QString strEfiDisk) {
  QString str5 = "diskutil mount " + strEfiDisk;
  QString str_ex = "do shell script " +
                   QString::fromLatin1("\"%1\"").arg(str5) +
                   " with administrator privileges";

  QString fileName = mw_one->strConfigPath + "qtocc.applescript";
  QFile fi(fileName);
  if (fi.exists()) fi.remove();

  QSaveFile file(fileName);
  QString errorMessage;
  if (file.open(QFile::WriteOnly | QFile::Text)) {
    QTextStream out(&file);
    out << str_ex;
    if (!file.commit()) {
      errorMessage =
          tr("Cannot write file %1:\n%2.")
              .arg(QDir::toNativeSeparators(fileName), file.errorString());
    }
  } else {
    errorMessage =
        tr("Cannot open file %1 for writing:\n%2.")
            .arg(QDir::toNativeSeparators(fileName), file.errorString());
  }

  QProcess* dm = new QProcess;
  dm->execute("osascript", QStringList() << fileName);
  delete dm;
}

QString Method::getDriverVolInfo(QString strDisk) {
  QStringList listPartitionVol = getAllVolForDisk(strDisk);
  QString str3 = "";
  for (int i = 0; i < listPartitionVol.count(); i++) {
    str3 = str3 + listPartitionVol.at(i) + " , ";
  }
  str3 = str3.mid(0, str3.length() - 3);

  return str3;
}

QStringList Method::getAllVolForDisk(QString strDisk) {
  QStringList listPartition;
  QStringList listPartitionVol;
  QString strDriverName = getDriverName(strDisk);
  QStringList listAllDisk = getDiskList();
  for (int i = 0; i < listAllDisk.count(); i++) {
    QString name = getDriverName(listAllDisk.at(i));
    if (name == strDriverName) {
      listPartition.clear();
      listPartition = getPartitionList(listAllDisk.at(i));

      QStringList list = getVolListForPartition(listPartition);
      for (int j = 0; j < list.count(); j++) {
        listPartitionVol.append(list.at(j));
      }
    }
  }

  return listPartitionVol;
}

QStringList Method::getVolListForPartition(QStringList listPartition) {
  QStringList listPartitionVol;

  for (int i = 0; i < listPartition.count(); i++) {
    QString str = getVolForPartition(listPartition.at(i));
    if (str != "") listPartitionVol.append(str);
  }
  qDebug() << listPartitionVol;
  return listPartitionVol;
}

QString Method::getVolForPartition(QString strPartition) {
  QString str, str1;
  QTextEdit* textEdit = new QTextEdit;
  QProcess* processDriverInfo = new QProcess;
  processDriverInfo->start("diskutil", QStringList() << "info" << strPartition);
  processDriverInfo->waitForFinished();
  QString result = processDriverInfo->readAll();
  textEdit->append(result);
  int count = textEdit->document()->lineCount();
  for (int j = 0; j < count; j++) {
    QString line = getTextEditLineText(textEdit, j).trimmed();
    if (line.contains("Volume Name:")) {
      QStringList list = line.split(":");
      if (list.count() == 2) {
        str = list.at(1);
        QString str2 = str.trimmed();
        if (str2 != "") {
          if (!str2.contains("Not applicable")) {
            str1 = str2;
            break;
          }
        }
      }
    }
  }

  return str1;
}

QStringList Method::getPartitionList(QString strDisk) {
  QProcess* processDriverInfo = new QProcess;
  processDriverInfo->start("diskutil", QStringList() << "list");
  processDriverInfo->waitForFinished();
  QTextEdit* textEdit = new QTextEdit;
  QString result = processDriverInfo->readAll();
  textEdit->append(result);
  int count = textEdit->document()->lineCount();
  QStringList listPartition;
  for (int i = 0; i < count; i++) {
    QString line = getTextEditLineText(textEdit, i).trimmed();
    QStringList list0 = line.split(" ");
    QString str = list0.at(list0.count() - 1);
    str = str.trimmed();
    if (str.contains(strDisk) && str != strDisk) {
      listPartition.append(str);
    }
  }

  qDebug() << listPartition;
  return listPartition;
}

QString Method::getDriverName(QString strDisk) {
  QProcess* processDriverInfo = new QProcess;
  processDriverInfo->start("diskutil", QStringList() << "info" << strDisk);
  processDriverInfo->waitForFinished();

  QTextEdit* textEdit = new QTextEdit;
  QString result = processDriverInfo->readAll();
  textEdit->append(result);

  QString str0, str1;
  QStringList strList;
  int count = textEdit->document()->lineCount();
  for (int i = 0; i < count; i++) {
    str0 = textEdit->document()->findBlockByNumber(i).text().trimmed();
    if (str0.contains("Media Name:")) {
      strList = str0.split(":");
      if (strList.count() > 0)
        str1 = strList.at(1);
      else
        str1 = "";
      str1 = str1.trimmed();
      break;
    }
  }

  return str1;
}

QStringList Method::getDiskList() {
  QProcess* processDriverInfo = new QProcess;
  processDriverInfo->start("diskutil", QStringList() << "list");
  processDriverInfo->waitForFinished();
  QTextEdit* textEdit = new QTextEdit;
  QString result = processDriverInfo->readAll();
  textEdit->append(result);
  int count = textEdit->document()->lineCount();
  QStringList listPartition;
  for (int i = 0; i < count; i++) {
    QString line = getTextEditLineText(textEdit, i).trimmed();
    if (line.contains("/dev/disk")) {
      QStringList list1 = line.split(" ");
      QString str_0 = list1.at(0);
      str_0 = str_0.trimmed();
      str_0.replace("/dev/", "");
      listPartition.append(str_0.trimmed());
    }
  }
  return listPartition;
}

void Method::backupEFI() {
  if (!mw_one->ui->actionUpgrade_OC->isEnabled()) {
    QMessageBox box;
    box.setText(
        tr("The EFI directory is incomplete and the backup cannot be "
           "completed. Please check if the [EFI/OC] and [EFI/OC/Drivers] "
           "directories exist?"));
    box.exec();
    return;
  }
  QString strEFI = QDir::fromNativeSeparators(SaveFileName);
  QString strZipName, strBakTargetDir, str;
  strEFI.replace("/OC/" + QFileInfo(SaveFileName).fileName(), "");
  QStringList list = strEFI.split("/");
  strZipName = list.at(list.count() - 1);
  for (int i = 0; i < list.count() - 1; i++) {
    str = str + list.at(i) + "/";
  }
  strEFI = str;
  strBakTargetDir = QDir::homePath() + "/Desktop/Backup EFI/";
  QString strDate, strTime;
  QDate date = QDate::currentDate();
  QTime time = QTime::currentTime();
  strDate = QString::number(date.year()) + "-" + QString::number(date.month()) +
            "-" + QString::number(date.day());
  QString h, m, s;
  h = QString::number(time.hour());
  m = QString::number(time.minute());
  s = QString::number(time.second());
  if (h.length() == 1) h = "0" + h;
  if (m.length() == 1) m = "0" + m;
  if (s.length() == 1) s = "0" + s;

  strTime = h + "-" + m + "-" + s;
  QString strTargetName =
      strBakTargetDir + strDate + "-" + strTime + "-" + strZipName + ".zip";
  strTargetName = "\"" + strTargetName + "\"";
  QDir::setCurrent(strEFI);
  QDir dir;
  if (dir.mkpath(strBakTargetDir)) {
  }

  if (mw_one->win) {
    QString strZipExe = strAppExePath + "/zip.exe";
    strZipExe = "\"" + strZipExe + "\"";
    QTextEdit* txtEdit = new QTextEdit;
    txtEdit->append(strZipExe + " -q -r " + strTargetName + " " + strZipName);
    QString fileName = strAppExePath + "/bak.bat";
    TextEditToFile(txtEdit, fileName);
    QProcess::execute("cmd.exe", QStringList() << "/c" << fileName);
  } else {
    QProcess::execute("zip", QStringList() << "-q"
                                           << "-r"
                                           << strBakTargetDir + strDate + "-" +
                                                  strTime + "-" + strZipName
                                           << strZipName);
  }
}

void Method::init_PresetQuirks(QComboBox* comboBox, QString quirksFile) {
  QFileInfo appInfo(qApp->applicationDirPath());
  QString strPresetQuirks =
      appInfo.filePath() + "/Database/preset/" + quirksFile;
  comboBox->clear();
  comboBox->addItem("None");
  if (QFile(strPresetQuirks).exists()) {
    QTextEdit* txtEdit = new QTextEdit;
    txtEdit->setPlainText(loadText(strPresetQuirks));
    for (int i = 0; i < txtEdit->document()->lineCount(); i++) {
      QString strLine = getTextEditLineText(txtEdit, i).trimmed();
      if (strLine.mid(0, 3) == "CPU") {
        QStringList list = strLine.split(":");
        if (list.count() == 2) {
          comboBox->addItem(list.at(1));
        }
      }
    }
  }
}

QStringList Method::getMarkerQuirks(QString arg1, QString strItem, QWidget* tab,
                                    QString quirksFile) {
  QStringList l2 = QStringList() << "";

  QFileInfo appInfo(qApp->applicationDirPath());
  QString strPresetQuirks =
      appInfo.filePath() + "/Database/preset/" + quirksFile;
  if (QFile(strPresetQuirks).exists()) {
    QTextEdit* txtEdit = new QTextEdit;
    txtEdit->setPlainText(loadText(strPresetQuirks));
    for (int i = 0; i < txtEdit->document()->lineCount(); i++) {
      QString strLine = getTextEditLineText(txtEdit, i).trimmed();
      if (strLine.mid(0, 3) == "CPU") {
        QStringList list = strLine.split(":");
        if (list.count() == 2) {
          if (list.at(1) == arg1) {
            QString strQuriks;
            if (strItem == "ACPI")
              strQuriks = getTextEditLineText(txtEdit, i + 1);
            if (strItem == "Booter")
              strQuriks = getTextEditLineText(txtEdit, i + 1);
            if (strItem == "Kernel")
              strQuriks = getTextEditLineText(txtEdit, i + 1);
            if (strItem == "UEFI")
              strQuriks = getTextEditLineText(txtEdit, i + 1);
            QStringList l1 = strQuriks.split(":");
            if (l1.count() == 2) {
              QString str = l1.at(1);
              l2 = str.split(" ");
              for (int j = 0; j < l2.count(); j++) {
                qDebug() << l2.at(j);
              }
            }
            break;
          }
        }
      }
    }
  }

  qDebug() << "------";

  QList<QObject*> listChkBox =
      mw_one->getAllCheckBox(mw_one->getAllUIControls(tab));
  QFont font;
  for (int i = 0; i < listChkBox.count(); i++) {
    QCheckBox* w = (QCheckBox*)listChkBox.at(i);
    font.setBold(false);
    font.setItalic(false);
    w->setFont(font);
  }
  for (int i = 0; i < listChkBox.count(); i++) {
    QCheckBox* w = (QCheckBox*)listChkBox.at(i);

    for (int j = 0; j < l2.count(); j++) {
      if (w->text() == l2.at(j)) {
        font.setBold(true);
        font.setItalic(true);
        w->setFont(font);
      }
    }
  }

  if (arg1 == "None") {
    for (int i = 0; i < listChkBox.count(); i++) {
      QCheckBox* w = (QCheckBox*)listChkBox.at(i);
      font.setBold(false);
      font.setItalic(false);
      w->setFont(font);
    }
  }

  return l2;
}

void Method::setToolTip(QWidget* w, QString strTitle) {
  QFileInfo appInfo(qApp->applicationDirPath());
  QString strToolTipFile = appInfo.filePath() + "/Database/preset/toolTip.txt";
  if (QFile(strToolTipFile).exists()) {
    QTextEdit* txtEdit = new QTextEdit;
    txtEdit->setPlainText(loadText(strToolTipFile));

    for (int k = 0; k < txtEdit->document()->lineCount(); k++) {
      QString strLine = getTextEditLineText(txtEdit, k).trimmed();
      if (strLine.mid(0, 3) == "***") {
        QString str = strLine.replace("*", "").trimmed();
        if (str == strTitle) {
          QTextEdit* tipEdit = new QTextEdit;
          for (int n = k + 1; n < txtEdit->document()->lineCount(); n++) {
            QString line = getTextEditLineText(txtEdit, n).trimmed();
            if (line.mid(0, 3) != "***") {
              tipEdit->append(line);
              w->setToolTip(tipEdit->toPlainText());
            } else {
              break;
            }
          }

          break;
        }
      }
    }
  }
}

void Method::autoTip() {
  int m = mw_one->ui->listMain->currentRow();
  int s = mw_one->ui->listSub->currentRow();
  QWidget* tab = mw_one->getSubTabWidget(m, s);
  mw_one->listUICheckBox.clear();
  QObjectList listOfCheckBox =
      mw_one->getAllCheckBox(mw_one->getAllUIControls(tab));
  for (int i = 0; i < listOfCheckBox.count(); i++) {
    QCheckBox* w = (QCheckBox*)listOfCheckBox.at(i);
    mw_one->listUICheckBox.append(w);
  }

  for (int i = 0; i < mw_one->listUICheckBox.count(); i++) {
    QCheckBox* chk = mw_one->listUICheckBox.at(i);

    if (chk->geometry().contains(tab->mapFromGlobal(QCursor::pos()))) {
      mw_one->timer->stop();
      QElapsedTimer t;
      t.start();
      while (t.elapsed() < 2000) {
        QCoreApplication::processEvents();
      }

      if (chk->geometry().contains(tab->mapFromGlobal(QCursor::pos()))) {
        mw_one->myToolTip->popup(QCursor::pos(), chk->text() + "\n\n",
                                 chk->toolTip());

      } else
        mw_one->timer->start(2000);
      break;
    }
  }

  mw_one->listUILabel.clear();
  QObjectList listOfLabel = mw_one->getAllLabel(mw_one->getAllUIControls(tab));
  for (int i = 0; i < listOfLabel.count(); i++) {
    QLabel* w = (QLabel*)listOfLabel.at(i);
    mw_one->listUILabel.append(w);
  }

  for (int i = 0; i < mw_one->listUILabel.count(); i++) {
    QLabel* chk = mw_one->listUILabel.at(i);

    if (chk->geometry().contains(tab->mapFromGlobal(QCursor::pos()))) {
      mw_one->timer->stop();
      QElapsedTimer t;
      t.start();
      while (t.elapsed() < 2000) {
        QCoreApplication::processEvents();
      }

      if (chk->geometry().contains(tab->mapFromGlobal(QCursor::pos()))) {
        mw_one->myToolTip->popup(QCursor::pos(), chk->text() + "\n\n",
                                 chk->toolTip());

      } else
        mw_one->timer->start(2000);
      break;
    }
  }
}

QString Method::readPlistComment(QString plistFile) {
  QTextEdit* txtEdit = new QTextEdit;
  QString strComment = "";
  txtEdit->setPlainText(loadText(plistFile));
  for (int i = 0; i < txtEdit->document()->lineCount(); i++) {
    QString lineTxt = getTextEditLineText(txtEdit, i).trimmed();
    if (lineTxt.mid(0, 6) == "<key>#") {
      lineTxt.replace("<key>", "");
      lineTxt.replace("#", "");
      lineTxt.replace("</key>", "");
      if (lineTxt.trimmed() == "ConfigComment") {
        QString strValue = getTextEditLineText(txtEdit, i + 1).trimmed();
        strValue.replace("<string>", "");
        strValue.replace("</string>", "");
        strComment = strValue.trimmed();
        break;
      }
    }
  }

  return strComment;
}

void Method::writePlistComment(QString plistFile, QString strComment) {
  // if (strComment.trimmed().length() == 0) return;
  QTextEdit* txtEdit = new QTextEdit;
  txtEdit->setPlainText(loadText(plistFile));
  bool yes = false;
  QString line1, line2;
  for (int i = 0; i < txtEdit->document()->lineCount(); i++) {
    QString lineTxt = getTextEditLineText(txtEdit, i).trimmed();
    if (lineTxt.mid(0, 6) == "<key>#") {
      lineTxt.replace("<key>", "");
      lineTxt.replace("#", "");
      lineTxt.replace("</key>", "");
      if (lineTxt.trimmed() == "ConfigComment") {
        yes = true;
        QString str = getTextEditLineText(txtEdit, i + 1).trimmed();
        str.replace(str, "<string>" + strComment + "</string>");
        QTextBlock block = txtEdit->document()->findBlockByNumber(i + 1);

        QTextCursor cursor(block);
        block = block.next();
        cursor.select(QTextCursor::BlockUnderCursor);
        cursor.removeSelectedText();
        cursor.insertText("\n" + str);

        break;
      }
    }
  }
  if (!yes) {
    line1 = "<key>#ConfigComment</key>";
    line2 = "<string>" + strComment + "</string>";

    for (int i = 0; i < txtEdit->document()->lineCount(); i++) {
      QString lineTxt = getTextEditLineText(txtEdit, i).trimmed();
      if (lineTxt == "<key>ACPI</key>") {
        txtEdit->insertPlainText(line1 + "\n" + line2 + "\n");

        break;
      }
    }
  }
  TextEditToFile(txtEdit, plistFile);
}

#ifdef Q_OS_MAC
void Method::init_MacVerInfo(QString ver) {
  QString str1 = qApp->applicationDirPath();
  QString infoFile = str1.replace("MacOS", "Info.plist");

  QTextEdit* edit = new QTextEdit;
  edit->setPlainText(loadText(infoFile));

  bool write = false;

  for (int i = 0; i < edit->document()->lineCount(); i++) {
    QString lineTxt = getTextEditLineText(edit, i).trimmed();

#if (QT_VERSION >= QT_VERSION_CHECK(5, 15, 0))
    if (lineTxt == "<key>CFBundleShortVersionString</key>" ||
        lineTxt == "<key>CFBundleVersion</key>") {
      QString nextTxt = getTextEditLineText(edit, i + 1).trimmed();
      if (nextTxt != "<string>" + ver + "</string>") {
        QTextBlock block = edit->document()->findBlockByNumber(i + 1);
        QTextCursor cursor(block);
        block = block.next();
        cursor.select(QTextCursor::BlockUnderCursor);
        cursor.removeSelectedText();
        cursor.insertText("\n        <string>" + ver + "</string>");

        write = true;
      }
    }
#endif

#if (QT_VERSION < QT_VERSION_CHECK(5, 15, 0))
    if (lineTxt == "<key>CFBundleGetInfoString</key>") {
      QString nextTxt = getTextEditLineText(edit, i + 1).trimmed();
      if (nextTxt != "<string>" + ver + "</string>") {
        QTextBlock block = edit->document()->findBlockByNumber(i + 1);
        QTextCursor cursor(block);
        block = block.next();
        cursor.select(QTextCursor::BlockUnderCursor);
        cursor.removeSelectedText();
        cursor.insertText("\n        <string>" + ver + "</string>");

        write = true;
      }
    }

#endif
  }

  if (write) {
    TextEditToFile(edit, infoFile);
  }
}
#endif

QString Method::readPlist(QString plistFile, QString key) {
  QTextEdit* edit = new QTextEdit;
  edit->setPlainText(loadText(plistFile));
  for (int i = 0; i < edit->document()->lineCount(); i++) {
    QString lineTxt = getTextEditLineText(edit, i).trimmed();
    if (lineTxt == "<key>" + key + "</key>") {
      QString str = getTextEditLineText(edit, i + 1).trimmed();
      str = str.replace("<string>", "");
      str = str.replace("</string>", "");
      return str;
    }
  }

  return "";
}

void Method::writePlist(QString plistFile, QString key, QString value) {
  QTextEdit* edit = new QTextEdit;
  edit->setPlainText(loadText(plistFile));
  for (int i = 0; i < edit->document()->lineCount(); i++) {
    QString lineTxt = getTextEditLineText(edit, i).trimmed();
    if (lineTxt == "<key>" + key + "</key>") {
      QString strSpace = "            ";
      QTextBlock block = edit->document()->findBlockByNumber(i + 1);
      QTextCursor cursor(block);
      block = block.next();
      cursor.select(QTextCursor::BlockUnderCursor);
      cursor.removeSelectedText();
      cursor.insertText("\n" + strSpace + "<string>" + value + "</string>");

      TextEditToFile(edit, plistFile);
      break;
    }
  }
}

bool Method::isKeyExists(QString plistFile, QString key) {
  QTextEdit* edit = new QTextEdit;
  edit->setPlainText(loadText(plistFile));
  for (int i = 0; i < edit->document()->lineCount(); i++) {
    QString lineTxt = getTextEditLineText(edit, i).trimmed();
    if (lineTxt == "<key>" + key + "</key>") {
      return true;
    }
  }

  return false;
}

void Method::show_Tip(QString strText, QString strTip) {
  if (strTip.trimmed().length() == 0) return;
  QString str = strTip;
  QStringList list = str.split("----");
  if (list.count() == 2) {
    if (!zh_cn)
      strTip = list.at(0);
    else
      strTip = list.at(1);
  } else
    strTip = str;
  strTip = strTip.trimmed();
  int ms = strTip.length() * 135;
  if (ms < 2000) ms = 2000;

  int dir = 10;
  QScreen* screen = QGuiApplication::primaryScreen();
  QRect screenRect = screen->availableVirtualGeometry();
  int w0 = screenRect.width();
  int h0 = screenRect.height();
  int w1, w2, h1, h2;
  w1 = w0 * 0.33;
  w2 = w0 * 0.66;
  h1 = h0 * 0.33;
  h2 = h0 * 0.66;

  int x1 = QCursor::pos().x();
  int y1 = QCursor::pos().y();

  if (x1 < w1 && y1 < h1) dir = 10;
  if (x1 > w1 && y1 < h1) dir = 2;
  if (x1 < w1 && y1 > h2) dir = 7;
  if (x1 > w2 && y1 > h2) dir = 4;
  if (x1 < w1 && y1 > h1 && y1 < h2) dir = 9;
  if (x1 > w1 && x1 < w2 && y1 < h1) dir = 12;
  if (x1 > w2 && y1 > h1 && y1 < h2) dir = 3;
  if (x1 > w1 && x1 < w2 && y1 > h2) dir = 6;

  BalloonTip::showBalloon(QMessageBox::Information, strText, strTip,
                          QCursor::pos(), ms, true, dir);
}

QVariantList Method::get_TableData(QTableWidget* t) {
  QVariantList arrayList;
  QVariantMap AddSub;
  for (int i = 0; i < t->rowCount(); i++) {
    if (t->columnCount() > 1) {
      for (int j = 0; j < t->columnCount(); j++) {
        QString strCol = t->horizontalHeaderItem(j)->text();
        QStringList list = strCol.split("\n");
        if (list.count() == 2) strCol = list.at(1);
        if (isBool(strCol))
          AddSub[strCol] = mw_one->getBool(t, i, j);
        else if (isInt(strCol))
          AddSub[strCol] = t->item(i, j)->text().toLongLong();
        else if (isData(strCol))
          AddSub[strCol] = mw_one->HexStrToByte(t->item(i, j)->text());
        else
          AddSub[strCol] = t->item(i, j)->text();
      }
      arrayList.append(AddSub);
    }

    if (t->columnCount() == 1) arrayList.append(t->item(i, 0)->text());
  }
  return arrayList;
}

void Method::set_TableData(QTableWidget* t, QVariantList mapList) {
  int rowTotal = t->rowCount();
  for (int i = 0; i < mapList.count(); i++) {
    t->setRowCount(t->rowCount() + 1);
    QVariantMap map = mapList.at(i).toMap();

    for (int k = 0; k < t->columnCount(); k++) {
      QString strCol = t->horizontalHeaderItem(k)->text();
      if (isBool(strCol)) {
        mw_one->init_enabled_data(t, i + rowTotal, k, "true");
      } else {
        QTableWidgetItem* newItem1 = new QTableWidgetItem("");
        t->setItem(i + rowTotal, k, newItem1);
      }
    }

    if (map.count() == 0) {  // 代表列，从0开始
      QTableWidgetItem* newItem1 =
          new QTableWidgetItem(mapList.at(i).toString());
      t->setItem(i + rowTotal, 0, newItem1);
    }

    if (map.count() > 0 && t->columnCount() == 1) {
      if (map.keys().contains("Path")) {
        QTableWidgetItem* newItem1;
        newItem1 = new QTableWidgetItem(map["Path"].toString());
        t->setItem(i + rowTotal, 0, newItem1);
      }
    }

    if (map.count() > 0 && t->columnCount() > 1) {
      for (int j = 0; j < t->columnCount(); j++) {
        QString strCol = t->horizontalHeaderItem(j)->text();
        QStringList list = strCol.split("\n");
        if (list.count() == 2) strCol = list.at(1);

        if (isBool(strCol)) {
          QString str = map[strCol].toString();
          mw_one->init_enabled_data(t, i + rowTotal, j, str);

          if (t->item(i + rowTotal, j)->text() == "") {
            if (strCol == "Load") {
              str = map["Load"].toString();
              if (str == "Enabled") str = "true";
              if (str == "Disabled") str = "false";
              if (str == "") str = "true";
              mw_one->init_enabled_data(t, i + rowTotal, j, str);
            } else if (strCol == "LoadEarly") {
              mw_one->init_enabled_data(t, i + rowTotal, j, "false");
            } else
              mw_one->init_enabled_data(t, i + rowTotal, j, "false");
          }
        } else if (isData(strCol)) {
          QTableWidgetItem* newItem1 = new QTableWidgetItem(
              mw_one->ByteToHexStr(map[strCol].toByteArray()));
          t->setItem(i + rowTotal, j, newItem1);
        } else {
          QString str = map[strCol].toString();

          QTableWidgetItem* newItem1;
          newItem1 = new QTableWidgetItem(str);
          if (strCol == "Arch" || strCol == "Count" || strCol == "Limit" ||
              strCol == "Skip" || strCol == "Strategy" || strCol == "Flavour" ||
              strCol == "Load")
            newItem1->setTextAlignment(Qt::AlignCenter);

          t->setItem(i + rowTotal, j, newItem1);

          if (t->item(i + rowTotal, j)->text() == "") {
            if (strCol == "Arch") {
              newItem1 = new QTableWidgetItem("Any");
              newItem1->setTextAlignment(Qt::AlignCenter);
              t->setItem(i + rowTotal, j, newItem1);
            }
            if (strCol == "Strategy") {
              newItem1 = new QTableWidgetItem("Disable");
              newItem1->setTextAlignment(Qt::AlignCenter);
              t->setItem(i + rowTotal, j, newItem1);
            }
            if (strCol == "Flavour") {
              newItem1 = new QTableWidgetItem("Auto");
              newItem1->setTextAlignment(Qt::AlignCenter);
              t->setItem(i + rowTotal, j, newItem1);
            }

            if (strCol == "Load") {
              str = map["Enabled"].toString();
              if (str == "true") str = "Enabled";
              if (str == "false") str = "Disabled";
              if (str == "") str = "Enabled";

              newItem1 = new QTableWidgetItem(str);
              newItem1->setTextAlignment(Qt::AlignCenter);
              t->setItem(i + rowTotal, j, newItem1);
            }
          }
        }
      }
    }
  }
}

bool Method::isInt(QString strCol) {
  QStringList list = QStringList() << "TableLength"
                                   << "Count"
                                   << "Limit"
                                   << "Skip"
                                   << "BaseSkip"
                                   << "Address"
                                   << "Size"
                                   << "Speed";
  list = intTypeList;
  for (int i = 0; i < list.count(); i++) {
    if (strCol == list.at(i)) return true;
  }
  return false;
}

bool Method::isData(QString strCol) {
  QStringList list = QStringList() << "TableSignature"
                                   << "OemTableId"
                                   << "Find"
                                   << "Replace"
                                   << "Mask"
                                   << "ReplaceMask";
  list = dataTypeList;
  for (int i = 0; i < list.count(); i++) {
    if (strCol == list.at(i)) return true;
  }
  return false;
}

bool Method::isBool(QString strCol) {
  QStringList list = QStringList() << "Enabled"
                                   << "LoadEarly"
                                   << "All"
                                   << "Auxiliary"
                                   << "TextMode"
                                   << "RealPath";
  list = boolTypeList;
  for (int i = 0; i < list.count(); i++) {
    if (strCol == list.at(i)) return true;
  }
  return false;
}

void Method::init_Table(QTableWidget* t, QStringList listHeaders) {
  t->horizontalHeader()->setFixedHeight(30);
  QFont font;
  font.setBold(true);
  t->horizontalHeader()->setFont(font);
  t->setColumnCount(0);
  if (listHeaders.count() == 0) {
    t->setColumnCount(1);
    t->setHorizontalHeaderLabels(QStringList() << "");
    t->horizontalHeader()->setStretchLastSection(true);
  } else {
    t->setColumnCount(listHeaders.count());

    if (listHeaders.removeOne("MinKernel")) listHeaders.append("MinKernel");
    if (listHeaders.removeOne("MaxKernel")) listHeaders.append("MaxKernel");
    if (listHeaders.removeOne("TableSignature"))
      listHeaders.append("TableSignature");
    if (listHeaders.removeOne("Arch")) listHeaders.append("Arch");
    if (listHeaders.removeOne("All")) listHeaders.append("All");

    if (listHeaders.removeOne("Path")) listHeaders.insert(0, "Path");
    if (listHeaders.removeOne("Identifier"))
      listHeaders.insert(0, "Identifier");
    if (listHeaders.removeOne("Name")) listHeaders.insert(0, "Name");

    t->setHorizontalHeaderLabels(listHeaders);
    t->horizontalHeader()->setStretchLastSection(false);
  }
  t->setAlternatingRowColors(true);

  for (int i = 0; i < listHeaders.count(); i++) {
    QString txt = t->horizontalHeaderItem(i)->text();
    if (txt == "Type") {
      t->setColumnWidth(i, 150);
    }
  }
}

QStringList Method::get_HorizontalHeaderList(QString main, QString sub) {
  QStringList list;
  QVariantMap mapMain, mapSub;
  QVariantList maplist;
  mapMain = mapTatol[main].toMap();
  maplist = mapMain[sub].toList();
  if (maplist.count() > 0) {
    mapSub = maplist.at(0).toMap();
    list = mapSub.keys();

    for (int i = 0; i < list.count(); i++) {
      QString name = list.at(i);
      QString type = mapSub[name].typeName();
      if (type == "bool") {
        boolTypeList.removeOne(name);
        boolTypeList.append(name);
      }
      if (type == "qlonglong") {
        intTypeList.removeOne(name);
        intTypeList.append(name);
      }
      if (type == "QByteArray") {
        dataTypeList.removeOne(name);
        dataTypeList.append(name);
      }
    }

    return list;
  }
  return list;
}

void Method::add_OneLine(QTableWidget* t) {
  t->setRowCount(t->rowCount() + 1);
  for (int k = 0; k < t->columnCount(); k++) {
    QString strCol = t->horizontalHeaderItem(k)->text();
    if (isBool(strCol)) {
      mw_one->init_enabled_data(t, t->rowCount() - 1, k, "true");
    } else if (strCol == "Arch") {
      QTableWidgetItem* newItem1 = new QTableWidgetItem("Any");
      newItem1->setTextAlignment(Qt::AlignCenter);
      t->setItem(t->rowCount() - 1, k, newItem1);
    } else if (strCol == "PlistPath") {
      QTableWidgetItem* newItem1 = new QTableWidgetItem("Contents/Info.plist");
      t->setItem(t->rowCount() - 1, k, newItem1);
    } else if (strCol == "Flavour") {
      QTableWidgetItem* newItem1 = new QTableWidgetItem("Auto");
      t->setItem(t->rowCount() - 1, k, newItem1);
    } else if (strCol == "Type") {
      QTableWidgetItem* newItem1 = new QTableWidgetItem("Reserved");
      t->setItem(t->rowCount() - 1, k, newItem1);
    } else if (strCol == "Strategy") {
      QTableWidgetItem* newItem1 = new QTableWidgetItem("Disable");
      t->setItem(t->rowCount() - 1, k, newItem1);
    } else {
      QTableWidgetItem* newItem1 = new QTableWidgetItem("");
      t->setItem(t->rowCount() - 1, k, newItem1);
    }
  }
  t->setFocus();
  t->setCurrentCell(t->rowCount() - 1, 0);
  mw_one->setWM();
}

QString Method::setLineEditQss(QLineEdit* txt, int radius, int borderWidth,
                               const QString& normalColor,
                               const QString& focusColor) {
  QStringList list;
  list.append(QString("QLineEdit{border-style:none;padding:3px;border-radius:%"
                      "1px;border:%2px solid %3;}")
                  .arg(radius)
                  .arg(borderWidth)
                  .arg(normalColor));
  list.append(QString("QLineEdit:focus{border:%1px solid %2;}")
                  .arg(borderWidth)
                  .arg(focusColor));

  QString qss = list.join("");
  txt->setStyleSheet(qss);
  return qss;
}

QString Method::setComboBoxQss(QComboBox* txt, int radius, int borderWidth,
                               const QString& normalColor,
                               const QString& focusColor) {
  QStringList list;
  list.append(QString("QComboBox{border-style:none;padding:3px;border-radius:%"
                      "1px;border:%2px solid %3;}")
                  .arg(radius)
                  .arg(borderWidth)
                  .arg(normalColor));
  list.append(QString("QComboBox:focus{border:%1px solid %2;}")
                  .arg(borderWidth)
                  .arg(focusColor));
  list.append(
      QString("QComboBox::down-arrow{image:url(:/icon/"
              "add_bottom.png);width:10px;height:10px;right:2px;}"));
  list.append(QString(
      "QComboBox::drop-down{subcontrol-origin:padding;subcontrol-position:top "
      "right;width:15px;border-left-width:0px;border-left-style:solid;border-"
      "top-right-radius:3px;border-bottom-right-radius:3px;border-left-color:#"
      "B6B6B6;}"));
  list.append(QString("QComboBox::drop-down:on{top:1px;}"));

  QString qss = list.join("");
  txt->setStyleSheet(qss);
  return qss;
}

void Method::init_UIWidget(QWidget* uiw, int red) {
  QObjectList listObject;
  listObject = MainWindow::getAllLineEdit(MainWindow::getAllUIControls(uiw));
  for (int i = 0; i < listObject.count(); i++) {
    QLineEdit* w1 = (QLineEdit*)listObject.at(i);
    if (w1->objectName() != "") {
      w1->setAttribute(Qt::WA_MacShowFocusRect, 0);
      if (red > 55)
        setLineEditQss(w1, 6, 1, "#C0C0C0", "#4169E1");
      else
        setLineEditQss(w1, 6, 1, "#2A2A2A", "#4169E1");
    }
  }

  listObject.clear();
  listObject = MainWindow::getAllComboBox(MainWindow::getAllUIControls(uiw));
  for (int i = 0; i < listObject.count(); i++) {
    QComboBox* w1 = (QComboBox*)listObject.at(i);
    if (!w1->objectName().contains("mycboxFind")) {
      w1->setAttribute(Qt::WA_MacShowFocusRect, 0);
      if (red > 55)
        setComboBoxQss(w1, 6, 1, "#C0C0C0", "#4169E1");
      else
        setComboBoxQss(w1, 6, 1, "#2A2A2A", "#4169E1");
    }
  }
}

void Method::Quit() { qDebug() << replyDL->error(); }
