#include "Method.h"

#include "dlgdatabase.h"
#include "filesystemwatcher.h"
#include "mainwindow.h"
#include "plistparser.h"
#include "plistserializer.h"
#include "ui_dlgdatabase.h"
#include "ui_mainwindow.h"

extern MainWindow* mw_one;
extern QString SaveFileName;

QString strACPI;
QString strKexts;
QString strDrivers;
QString strTools;

Method::Method(QWidget* parent) : QMainWindow(parent) {
  manager = new QNetworkAccessManager(this);
  connect(manager, SIGNAL(finished(QNetworkReply*)), this,
          SLOT(replyFinished(QNetworkReply*)));
  managerDownLoad = new QNetworkAccessManager(this);
  myfile = new QFile(this);
  tempDir = QDir::homePath() + "/tempocat/";
}

QStringList Method::getDLUrlList(QString url) {
  QString strLast = getHTMLSource(url, false);
  if (strLast == "") {
    blBreak = true;
    return QStringList() << "";
  }
  QStringList list0 = strLast.split("\"");
  QString str_url;
  if (list0.count() > 2) str_url = list0.at(1);

  QString str_html = getHTMLSource(str_url, false);
  if (str_html == "") {
    blBreak = true;
    return QStringList() << "";
  }
  QTextEdit* htmlEdit = new QTextEdit;
  htmlEdit->setPlainText(str_html);
  QStringList list1, list2;
  for (int i = 0; i < htmlEdit->document()->lineCount(); i++) {
    QString strLine = getTextEditLineText(htmlEdit, i);
    if (strLine.trimmed().contains("/releases/download/")) {
      list1 = strLine.split("\"");
      if (list1.count() > 1) {
        list2.append("https://github.com" + list1.at(1));
      }
    }
  }

  if (list1.isEmpty() || list2.isEmpty()) return QStringList() << "";

  return list2;
}

QString Method::getHTMLSource(QString URLSTR, bool writeFile) {
  const QString FILE_NAME = QDir::homePath() + "/.config/QtOCC/code.txt";
  QString strProxy =
      mw_one->myDatabase->ui->comboBoxWeb->currentText().trimmed();
  URLSTR.replace("https://github.com/", strProxy);
  QUrl url(URLSTR);
  QNetworkAccessManager manager;
  QEventLoop loop;
  // qDebug() << "Reading code form " << URLSTR;

  QNetworkReply* reply = manager.get(QNetworkRequest(url));
  QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
  loop.exec();

  QString code = reply->readAll();
  if (code == "") {
    QMessageBox::critical(this, "", tr("Network error!"));

    mw_one->dlgSyncOC->ui->btnUpKexts->setEnabled(true);
    blBreak = true;
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
    if (formats.contains(fileInfo.suffix())) {  //检测格式，按需保存
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
      QString dirSource, dirTargetDatabase, dirTargetLinux;
      dirSource = kextList.at(i);
      QString Name = getFileName(dirSource);
      dirTargetDatabase =
          mw_one->strAppExePath + "/Database/EFI/OC/Kexts/" + Name;
      dirTargetLinux = QDir::homePath() + "/Kexts/" + Name;

      if (!mw_one->linuxOS)
        mw_one->copyDirectoryFiles(dirSource, dirTargetDatabase, true);
      if (mw_one->linuxOS)
        mw_one->copyDirectoryFiles(dirSource, dirTargetLinux, true);
    }
  } else {
    QStringList list;
    if (!mw_one->linuxOS)
      list = DirToFileList(mw_one->strAppExePath + "/Database/EFI/OC/Kexts/",
                           "*.kext");
    else
      list = DirToFileList(QDir::homePath() + "/Kexts/", "*.kext");
    for (int i = 0; i < list.count(); i++) {
      QString dirSource, dirTarget;
      if (!mw_one->linuxOS)
        dirSource =
            mw_one->strAppExePath + "/Database/EFI/OC/Kexts/" + list.at(i);
      else
        dirSource = QDir::homePath() + "/Kexts/" + list.at(i);
      QString Name = getFileName(dirSource);
      dirTarget = strKexts + Name;

      for (int j = 0; j < mw_one->dlgSyncOC->ui->listSource->count(); j++) {
        QString str_1 = mw_one->dlgSyncOC->ui->listSource->item(j)->text();
        QStringList list_1 = str_1.split("|");
        QString Name_0, Name_1;
        if (list_1.count() > 0) Name_0 = list_1.at(0);
        Name_1 = Name_0.trimmed();
        if (Name == Name_1 &&
            mw_one->dlgSyncOC->ui->listSource->item(j)->checkState() ==
                Qt::Checked) {
          mw_one->copyDirectoryFiles(dirSource, dirTarget, true);

          qDebug() << dirSource << dirTarget;
        }
      }
    }
  }

  mw_one->dlgSyncOC->ui->btnUpKexts->setEnabled(true);

  mw_one->dlgSyncOC->progBar->setHidden(true);
  mw_one->dlgSyncOC->ui->labelShowDLInfo->setVisible(false);
  if (!blDatabase) mw_one->checkFiles();
  mw_one->repaint();
  int n = mw_one->dlgSyncOC->ui->listSource->currentRow();
  for (int i = 0; i < mw_one->dlgSyncOC->ui->listSource->count(); i++)
    mw_one->dlgSyncOC->ui->listSource->setCurrentRow(i);
  mw_one->dlgSyncOC->ui->listSource->setCurrentRow(n);
}

void Method::kextUpdate() {
  if (!mw_one->ui->actionUpgrade_OC->isEnabled()) return;
  if (mw_one->ui->table_kernel_add->rowCount() == 0) return;
  mw_one->myDatabase->refreshKextUrl();
  blBreak = false;

  mw_one->dlgSyncOC->ui->btnUpKexts->setEnabled(false);
  mw_one->repaint();
  QString test = "https://github.com/acidanthera/Lilu";

  for (int i = 0; i < mw_one->dlgSyncOC->ui->listSource->count(); i++) {
    if (blBreak) break;
    if (mw_one->dlgSyncOC->ui->listSource->item(i)->checkState() ==
        Qt::Checked) {
      QString name_1 =
          mw_one->dlgSyncOC->ui->listSource->item(i)->text().trimmed();
      QStringList list_1 = name_1.split("|");
      QString name_0, name;
      if (list_1.count() > 0) name_0 = list_1.at(0);
      name = name_0.trimmed();
      kextName = name;
      for (int j = 0; j < mw_one->myDatabase->ui->tableKextUrl->rowCount();
           j++) {
        if (blBreak) break;
        QString txt =
            mw_one->myDatabase->ui->tableKextUrl->item(j, 0)->text().trimmed();
        test =
            mw_one->myDatabase->ui->tableKextUrl->item(j, 1)->text().trimmed();
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
          if (reGetUrl) {
            if (mw_one->myDatabase->ui->rbtnAPI->isChecked())
              getLastReleaseFromUrl(test);
            if (mw_one->myDatabase->ui->rbtnWeb->isChecked())
              getLastReleaseFromHtml(test + "/releases/latest");
          } else {
            startDownload(strUrl);
          }
          mw_one->dlgSyncOC->ui->listSource->setItemWidget(
              mw_one->dlgSyncOC->ui->listSource->item(i),
              mw_one->dlgSyncOC->progBar);
          mw_one->dlgSyncOC->ui->listSource->setCurrentRow(i);
          QElapsedTimer t;
          t.start();
          dlEnd = false;
          while (!dlEnd && !blBreak) {
            QCoreApplication::processEvents();
          }
        }
      }
    }
  }

  if (blBreak) return;
  finishKextUpdate(true);
}

void Method::startDownload(QString strUrl) {
  QString strTokyo, strSeoul, strOriginal, strSet, strTemp, strUrlOrg;
  strUrlOrg = strUrl;
  strOriginal = "https://github.com/";
  strTokyo = "https://download.fastgit.org/";
  strSeoul = "https://ghproxy.com/https://github.com/";
  strSet = mw_one->myDatabase->ui->comboBoxNet->currentText().trimmed();
  strTemp = strUrlOrg;
  strUrl = strTemp.replace("https://github.com/", strSet);

  QNetworkRequest request;
  request.setUrl(QUrl(strUrl));
  request.setHeader(QNetworkRequest::ContentTypeHeader,
                    "application/octet-stream");

  replyDL = managerDownLoad->get(request);

  connect(replyDL, &QNetworkReply::readyRead, this,
          &Method::doProcessReadyRead);
  connect(replyDL, &QNetworkReply::finished, this, &Method::doProcessFinished);
  connect(replyDL, &QNetworkReply::downloadProgress, this,
          &Method::doProcessDownloadProgress);

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
    mw_one->dlgSyncOC->ui->btnUpKexts->setEnabled(true);
    mw_one->repaint();

    QMessageBox::warning(this, "warning", "File creation failed!\n" + file);
    return;
  }
  mw_one->dlgSyncOC->progBar->setValue(0);
  mw_one->dlgSyncOC->progBar->setMinimum(0);

  downloadTimer.start();

  mw_one->dlgSyncOC->progBar->setHidden(false);
  mw_one->dlgSyncOC->progBar->setVisible(true);
}

void Method::doProcessReadyRead() {
  while (!replyDL->atEnd()) {
    QByteArray ba = replyDL->readAll();
    myfile->write(ba);
  }
}

void Method::doProcessFinished() {
  if (!blCanBeUpdate) return;
  if (replyDL->error() == QNetworkReply::NoError) {
    myfile->flush();
    myfile->close();
    if (QFileInfo(tempDir + filename).exists()) {
      if (mw_one->win) {
        QProcess::execute(mw_one->strAppExePath + "/unzip.exe",
                          QStringList()
                              << "-o" << tempDir + filename << "-d" << tempDir);
      } else
        QProcess::execute("unzip", QStringList() << "-o" << tempDir + filename
                                                 << "-d" << tempDir);
    }
    dlEnd = true;

  } else {
    mw_one->dlgSyncOC->ui->btnUpKexts->setEnabled(true);
    mw_one->repaint();
    myfile->close();
    QMessageBox::critical(NULL, "replyDL Error",
                          "There is an error in the network answer!");
  }
}

void Method::doProcessDownloadProgress(qint64 recv_total,
                                       qint64 all_total)  //显示
{
  if (blBreak) return;
  mw_one->dlgSyncOC->progBar->setMaximum(all_total);
  mw_one->dlgSyncOC->progBar->setValue(recv_total);

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
      kextName + "\n" + tr("Download Progress") + " : " +
      GetFileSize(recv_total, 2) + " -> " + GetFileSize(all_total, 2) + "    " +
      strSpeed);

  if (recv_total == all_total) {
    if (recv_total < 1000) {
      blCanBeUpdate = false;
    } else
      blCanBeUpdate = true;
  }
}

void Method::getLastReleaseFromUrl(QString strUrl) {
  // https://github.com/ic005k/QtOpenCoreConfig
  // https://api.github.com/repos/ic005k/QtOpenCoreConfig/releases/latest
  QString strAPI =
      strUrl.replace("https://github.com/", "https://api.github.com/repos/") +
      "/releases/latest";
  QNetworkRequest quest;
  quest.setUrl(QUrl(strAPI));
  quest.setHeader(QNetworkRequest::UserAgentHeader, "RT-Thread ART");
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
    QMessageBox::critical(this, "", tr("Network error!"));

    mw_one->dlgSyncOC->ui->btnUpKexts->setEnabled(true);
    blBreak = true;
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
        if (str.contains("RELEASE"))
          strDLUrl = str;
        else
          strDLUrl = strDownloadUrlList.at(0);
      } else
        strDLUrl = strDownloadUrlList.at(0);
    }
  }
  strDLInfoList.clear();
  strDLInfoList = QStringList() << Verison << strDLUrl;
  qDebug() << strDLInfoList.at(0) << strDLInfoList.at(1);
  if (strDLUrl == "") {
    blBreak = true;

    mw_one->dlgSyncOC->ui->btnUpKexts->setEnabled(true);
    return;
  }
  kextDLUrlList.append(kextName + "|" + strDLUrl);
  startDownload(strDLUrl);
}

void Method::getLastReleaseFromHtml(QString url) {
  QStringList strDownloadUrlList = getDLUrlList(url);
  if (strDownloadUrlList.at(0) == "") {
    blBreak = true;
    return;
  }
  QString strDLUrl;
  for (int i = 0; i < strDownloadUrlList.count(); i++) {
    if (strDownloadUrlList.count() > 1) {
      QString str = strDownloadUrlList.at(i);
      if (str.contains("RELEASE"))
        strDLUrl = str;
      else
        strDLUrl = strDownloadUrlList.at(0);
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
  if (t0->rowCount() > 0) t0->cellClicked(t0->currentRow(), 0);
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
      QMessageBox::warning(
          this, tr("Application"),
          tr("Cannot read file %1:\n%2.")
              .arg(QDir::toNativeSeparators(textFile), file.errorString()));

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
  // qDebug() << str0 << str1 << str2;
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

    //保存数据
    mw_one->write_ini(mw_one->ui->table_nv_add0, mw_one->ui->table_nv_add,
                      mw_one->ui->table_nv_add0->currentRow());
  }
}

QWidget* Method::getSubTabWidget(int m, int s) {
  for (int j = 0; j < mw_one->mainTabList.count(); j++) {
    if (j == m) {
      for (int i = 0; i < mw_one->mainTabList.at(j)->tabBar()->count(); i++) {
        if (i == s) return mw_one->mainTabList.at(j)->widget(i);
      }
    }
  }

  return NULL;
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
}

QString Method::copyDrivers(QString pathSource, QString pathTarget) {
  // OC/Drivers

  QDir dir;
  QString strDatabase;
  QString pathOCDrivers = pathTarget + "OC/Drivers/";
  if (dir.mkpath(pathOCDrivers)) {
  }
  for (int i = 0; i < mw_one->ui->table_uefi_drivers->rowCount(); i++) {
    QString file = mw_one->ui->table_uefi_drivers->item(i, 0)->text();
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
  for (int i = 0; i < mw_one->ui->table_kernel_add->rowCount(); i++) {
    QString file = mw_one->ui->table_kernel_add->item(i, 0)->text();
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

  for (int i = 0; i < mw_one->ui->table_acpi_add->rowCount(); i++) {
    QString file = mw_one->ui->table_acpi_add->item(i, 0)->text();
    QFileInfo fi(pathSource + "EFI/OC/ACPI/" + file);
    if (fi.exists())
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
  for (int i = 0; i < mw_one->ui->tableTools->rowCount(); i++) {
    QString file = mw_one->ui->tableTools->item(i, 0)->text();
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

void Method::generateEFI() {
  QDir dir;
  QString strDatabase;

  QFileInfo appInfo(qApp->applicationDirPath());
  QString pathSource = appInfo.filePath() + "/Database/";

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
  strDatabase = copyACPI(pathSource, pathTarget) + strDatabase;

  // Drivers
  strDatabase = copyDrivers(pathSource, pathTarget) + strDatabase;

  // Kexts
  strDatabase = copyKexts(pathSource, pathTarget) + strDatabase;

  // OC/Resources
  QString pathOCResources = pathTarget + "OC/Resources/";
  mw_one->copyDirectoryFiles(pathSource + "EFI/OC/Resources/", pathOCResources,
                             true);

  // Tools
  strDatabase = copyTools(pathSource, pathTarget) + strDatabase;

  // OC/OpenCore.efi
  QFile::copy(pathSource + "EFI/OC/OpenCore.efi",
              pathTarget + "OC/OpenCore.efi");

  // OC/Config.plist
  mw_one->SavePlist(pathTarget + "OC/Config.plist");

  QMessageBox box;
  if (strDatabase != "")
    box.setText(tr("Finished generating the EFI folder on the desktop.") +
                "\n" +
                tr("The following files do not exist in the database at the "
                   "moment, please add them yourself:") +
                "\n" + strDatabase);
  else
    box.setText(tr("Finished generating the EFI folder on the desktop."));

  mw_one->setFocus();
  box.exec();
  mw_one->ui->cboxFind->setFocus();
  mw_one->openFile(pathTarget + "OC/Config.plist");
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
      init_Table(0);

      mw_one->ParserACPI(map);

      break;

    case 1:
      // Booter
      init_Table(1);

      mw_one->ParserBooter(map);
      break;

    case 2:
      // DP
      init_Table(2);

      mw_one->ParserDP(map);
      break;

    case 3:
      // Kernel
      init_Table(3);

      mw_one->ParserKernel(map);
      break;

    case 4:
      // Misc
      init_Table(4);

      mw_one->ParserMisc(map);
      break;

    case 5:
      // NVRAM
      init_Table(5);

      mw_one->ParserNvram(map);
      break;

    case 6:
      // PI
      init_Table(6);

      mw_one->ParserPlatformInfo(map);
      break;

    case 7:
      // UEFI
      init_Table(7);

      mw_one->ParserUEFI(map);
      break;
  }

  mw_one->loading = false;
}

void Method::init_Table(int index) {
  if (index == -1) {
    mw_one->listOfTableWidget.clear();
    mw_one->listOfTableWidget = mw_one->getAllTableWidget(
        mw_one->getAllUIControls(mw_one->ui->tabTotal));
    for (int i = 0; i < mw_one->listOfTableWidget.count(); i++) {
      QTableWidget* w = (QTableWidget*)mw_one->listOfTableWidget.at(i);

      w->setRowCount(0);
    }
  } else {
    for (int i = 0; i < mw_one->ui->tabTotal->tabBar()->count(); i++) {
      if (index == i) {
        mw_one->listOfTableWidget.clear();
        mw_one->listOfTableWidget = mw_one->getAllTableWidget(
            mw_one->getAllUIControls(mw_one->ui->tabTotal->widget(i)));
        for (int j = 0; j < mw_one->listOfTableWidget.count(); j++) {
          QTableWidget* w = (QTableWidget*)mw_one->listOfTableWidget.at(j);

          w->setRowCount(0);
        }
      }
    }
  }
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

  QWidget* w = getSubTabWidget(mw_one->ui->listMain->currentRow(),
                               mw_one->ui->listSub->currentRow());
  listTable = mw_one->getAllTableWidget(mw_one->getAllUIControls(w));

  if (listTable.count() > 1) {
    for (int i = 0; i < listTable.count(); i++) {
      t = (QTableWidget*)listTable.at(i);
      if (!t->currentIndex().isValid()) return;
      if (t->hasFocus()) t->cellClicked(t->currentRow(), t->currentColumn());
    }
  } else if (listTable.count() == 1) {
    t = (QTableWidget*)listTable.at(0);
    if (!t->currentIndex().isValid()) return;
    t->cellClicked(t->currentRow(), t->currentColumn());
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

QStringList Method::deDuplication(QStringList FileName, QTableWidget* table,
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
  blBreak = true;

  mw_one->dlgSyncOC->ui->btnUpKexts->setEnabled(true);
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
    QStringList list = str.split("|");
    if (list.count() == 4) {
      if (strLeft == list.at(0)) {
        listAdd.removeAt(i);
        i--;
      }
    }
  }

  for (int i = 0; i < t->rowCount(); i++) {
    listAdd.append(strLeft + "|" + t->item(i, 0)->text().trimmed() + "|" +
                   t->item(i, 1)->text().trimmed() + "|" +
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
    QStringList list = str.split("|");
    if (list.count() == 3) {
      if (strLeft == list.at(1) && t0->objectName() == list.at(0)) {
        listAdd.removeAt(i);
        i--;
      }
    }
  }

  for (int i = 0; i < t->rowCount(); i++) {
    listAdd.append(t0->objectName() + "|" + strLeft + "|" +
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
      mw_one->strAppExePath + "/Database/EFI/OC/Kexts/", "*.kext");
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

  QString fileName = QDir::homePath() + "/.config/QtOCC/qtocc.applescript";
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
}

QString Method::getDriverInfo(QString strDisk, QString strDiskVol) {
  QProcess* processDriverInfo = new QProcess;
  processDriverInfo->start("diskutil", QStringList() << "info" << strDisk);
  processDriverInfo->waitForFinished();

  QTextEdit* textEdit = new QTextEdit;
  QTextCodec* gbkCodec = QTextCodec::codecForName("UTF-8");
  QString result = gbkCodec->toUnicode(processDriverInfo->readAll());
  textEdit->append(result);

  QString str0, str1, str2;
  QStringList strList, strList1;
  int count = textEdit->document()->lineCount();
  for (int i = 0; i < count; i++) {
    str0 = textEdit->document()->findBlockByNumber(i).text().trimmed();
    if (str0.contains("Media Name:")) {
      strList = str0.split(":");
    }
  }

  if (strList.count() > 0)
    str1 = strList.at(1);
  else
    str1 = "";
  str1 = str1.trimmed();

  processDriverInfo->start("diskutil", QStringList() << "info" << strDiskVol);
  processDriverInfo->waitForFinished();

  textEdit = new QTextEdit;
  gbkCodec = QTextCodec::codecForName("UTF-8");
  result = gbkCodec->toUnicode(processDriverInfo->readAll());
  textEdit->append(result);
  count = textEdit->document()->lineCount();
  for (int i = 0; i < count; i++) {
    str0 = textEdit->document()->findBlockByNumber(i).text().trimmed();
    if (str0.contains("Volume Name:")) {
      strList1 = str0.split(":");
    }
  }

  if (strList1.count() > 0)
    str2 = strList1.at(1);
  else
    str2 = "";
  str2 = str2.trimmed();

  return str1 + " | " + str2;
}
