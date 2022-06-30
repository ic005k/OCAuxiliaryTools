#include "filesystemwatcher.h"

#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QMainWindow>

#include "Method.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

extern MainWindow* mw_one;
extern QString SaveFileName;
extern Method* mymethod;
extern QString strACPI;
extern QString strKexts;
extern QString strDrivers;
extern QString strTools;

FileSystemWatcher* FileSystemWatcher::m_pInstance = NULL;

FileSystemWatcher::FileSystemWatcher(QObject* parent) : QObject(parent) {}

// 监控文件或目录
void FileSystemWatcher::addWatchPath(QString path) {
  // qDebug() << QString("Add to watch: %1").arg(path);

  if (m_pInstance == NULL) {
    m_pInstance = new FileSystemWatcher();
    m_pInstance->m_pSystemWatcher = new QFileSystemWatcher();

    // 连接QFileSystemWatcher的directoryChanged和fileChanged信号到相应的槽
    connect(m_pInstance->m_pSystemWatcher, SIGNAL(directoryChanged(QString)),
            m_pInstance, SLOT(directoryUpdated(QString)));
    connect(m_pInstance->m_pSystemWatcher, SIGNAL(fileChanged(QString)),
            m_pInstance, SLOT(fileUpdated(QString)));
  }

  // 添加监控路径
  m_pInstance->m_pSystemWatcher->addPath(path);

  // 如果添加路径是一个目录，保存当前内容列表
  QFileInfo file(path);
  if (file.isDir()) {
    const QDir dirw(path);
    m_pInstance->m_currentContentsMap[path] = dirw.entryList(
        QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files, QDir::DirsFirst);
  }
}

void FileSystemWatcher::removeWatchPath(QString path) {
  m_pInstance->m_pSystemWatcher->removePath(path);
}

// 只要任何监控的目录更新（添加、删除、重命名），就会调用。
void FileSystemWatcher::directoryUpdated(const QString& path) {
  qDebug() << QString("Directory updated: %1").arg(path);

  // 比较最新的内容和保存的内容找出区别(变化)
  QStringList currEntryList = m_currentContentsMap[path];
  const QDir dir(path);

  QStringList newEntryList = dir.entryList(
      QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files, QDir::DirsFirst);

  QSet<QString> newDirSet;
  QSet<QString> currentDirSet;
#if (QT_VERSION <= QT_VERSION_CHECK(5, 9, 9))
  {
    newDirSet = QSet<QString>::fromList(newEntryList);  //旧
  }
#endif
#if (QT_VERSION >= QT_VERSION_CHECK(5, 15, 0))
  {
    newDirSet = QSet<QString>(newEntryList.begin(),
                              newEntryList.end());  //新
  }
#endif

#if (QT_VERSION <= QT_VERSION_CHECK(5, 9, 9))
  {
    currentDirSet = QSet<QString>::fromList(currEntryList);  //旧
  }
#endif
#if (QT_VERSION >= QT_VERSION_CHECK(5, 15, 0))
  {
    currentDirSet = QSet<QString>(currEntryList.begin(),
                                  currEntryList.end());  //新
  }
#endif

  // 添加了文件
  QSet<QString> newFiles = newDirSet - currentDirSet;
  QStringList newFile = newFiles.values();

  // 文件已被移除
  QSet<QString> deletedFiles = currentDirSet - newDirSet;
  QStringList deleteFile = deletedFiles.values();

  // 更新当前设置
  m_currentContentsMap[path] = newEntryList;

  if (!newFile.isEmpty() && !deleteFile.isEmpty()) {
    // 文件/目录重命名
    if ((newFile.count() == 1) && (deleteFile.count() == 1)) {
      qDebug() << QString("File Renamed from %1 to %2")
                      .arg(deleteFile.first())
                      .arg(newFile.first());

      if (path == strACPI) {
        for (int i = mw_one->ui->table_acpi_add->rowCount() - 1; i > -1; i--) {
          QString str0 = mw_one->ui->table_acpi_add->item(i, 0)->text();
          for (int j = 0; j < deleteFile.count(); j++) {
            if (str0 == deleteFile.at(j)) {
              mw_one->ui->table_acpi_add->setItem(
                  i, 0, new QTableWidgetItem(newFile.first()));

              mw_one->setWindowModified(true);
              mw_one->updateIconStatus();
            }
          }
        }
      }

      if (path == strKexts) {
        for (int i = mw_one->ui->table_kernel_add->rowCount() - 1; i > -1;
             i--) {
          QString str0 = mw_one->ui->table_kernel_add->item(i, 0)->text();
          for (int j = 0; j < deleteFile.count(); j++) {
            if (str0.contains(deleteFile.at(j))) {
              QString str = str0.replace(deleteFile.at(j), newFile.first());
              mw_one->ui->table_kernel_add->setItem(i, 0,
                                                    new QTableWidgetItem(str));

              mw_one->setWindowModified(true);
              mw_one->updateIconStatus();
            }
          }
        }
      }

      if (path == strDrivers) {
        for (int i = mw_one->ui->table_uefi_drivers->rowCount() - 1; i > -1;
             i--) {
          QString str0 = mw_one->ui->table_uefi_drivers->item(i, 0)->text();
          for (int j = 0; j < deleteFile.count(); j++) {
            if (str0 == deleteFile.at(j)) {
              mw_one->ui->table_uefi_drivers->setItem(
                  i, 0, new QTableWidgetItem(newFile.first()));

              mw_one->setWindowModified(true);
              mw_one->updateIconStatus();
            }
          }
        }
      }

      if (path == strTools) {
        for (int i = mw_one->ui->tableTools->rowCount() - 1; i > -1; i--) {
          QString str0 = mw_one->ui->tableTools->item(i, 0)->text();
          for (int j = 0; j < deleteFile.count(); j++) {
            if (str0 == deleteFile.at(j)) {
              mw_one->ui->tableTools->setItem(
                  i, 0, new QTableWidgetItem(newFile.first()));

              mw_one->setWindowModified(true);
              mw_one->updateIconStatus();
            }
          }
        }
      }
    }
  } else {
    // 添加新文件/目录至Dir
    if (!newFile.isEmpty()) {
      qDebug() << "New Files/Dirs added: " << newFile;

      if (path == strACPI) {
        QStringList tempList;
        for (int i = 0; i < newFile.count(); i++) {
          QFileInfo fi(strACPI + newFile.at(i));
          if (fi.suffix().toLower() == "aml") {
            tempList.append(strACPI + newFile.at(i));
          }
        }

        mw_one->addACPIItem(tempList);
      }

      if (path == strKexts) {
        QStringList tempList;
        for (int i = 0; i < newFile.count(); i++) {
          QFileInfo fi(strKexts + newFile.at(i));
          QString fileSuffix;
#ifdef Q_OS_WIN32
          fileSuffix = fi.suffix().toLower();
#endif

#ifdef Q_OS_LINUX
          fileSuffix = fi.suffix().toLower();
#endif

#ifdef Q_OS_MAC
          QString str1 = newFile.at(i);
          QString str2 = str1.mid(str1.length() - 4, 4);
          fileSuffix = str2.toLower();
#endif

          if (fileSuffix == "kext") {
            tempList.append(strKexts + newFile.at(i));
          }
        }

        QElapsedTimer t;
        t.start();
        int a;
        if (tempList.count() < 10) a = 3000;
        if (tempList.count() > 10) a = 5000;
        while (t.elapsed() < a) {
          QCoreApplication::processEvents();
        }

        mw_one->addKexts(tempList);
      }

      if (path == strDrivers) {
        QStringList tempList;
        for (int i = 0; i < newFile.count(); i++) {
          QFileInfo fi(strDrivers + newFile.at(i));
          if (fi.suffix().toLower() == "efi") {
            tempList.append(strDrivers + newFile.at(i));
          }
        }
        mw_one->addEFIDrivers(tempList);
      }

      if (path == strTools) {
        QStringList tempList;
        for (int i = 0; i < newFile.count(); i++) {
          QFileInfo fi(strTools + newFile.at(i));
          if (fi.suffix().toLower() == "efi") {
            tempList.append(strTools + newFile.at(i));
          }
        }
        mw_one->addEFITools(tempList);
      }

      foreach (QString file, newFile) {
        // 处理操作每个新文件....
      }
    }

    // 从Dir中删除文件/目录
    if (!deleteFile.isEmpty()) {
      qDebug() << "Files/Dirs deleted: " << deleteFile;

      if (path == strACPI) {
        for (int i = mw_one->ui->table_acpi_add->rowCount() - 1; i > -1; i--) {
          QString str0 = mw_one->ui->table_acpi_add->item(i, 0)->text();
          for (int j = 0; j < deleteFile.count(); j++) {
            if (str0 == deleteFile.at(j)) {
              mw_one->ui->table_acpi_add->removeRow(i);

              mw_one->setWindowModified(true);
              mw_one->updateIconStatus();
            }
          }
        }
      }

      if (path == strKexts) {
        for (int i = mw_one->ui->table_kernel_add->rowCount() - 1; i > -1;
             i--) {
          QString str0 = mw_one->ui->table_kernel_add->item(i, 0)->text();
          for (int j = 0; j < deleteFile.count(); j++) {
            if (str0.contains(deleteFile.at(j))) {
              mw_one->ui->table_kernel_add->removeRow(i);

              mw_one->setWindowModified(true);
              mw_one->updateIconStatus();
            }
          }
        }
      }

      if (path == strDrivers) {
        for (int i = mw_one->ui->table_uefi_drivers->rowCount() - 1; i > -1;
             i--) {
          QString str0 = mw_one->ui->table_uefi_drivers->item(i, 0)->text();
          for (int j = 0; j < deleteFile.count(); j++) {
            if (str0.contains(deleteFile.at(j))) {
              mw_one->ui->table_uefi_drivers->removeRow(i);

              mw_one->setWindowModified(true);
              mw_one->updateIconStatus();
            }
          }
        }
      }

      if (path == strTools) {
        for (int i = mw_one->ui->tableTools->rowCount() - 1; i > -1; i--) {
          QString str0 = mw_one->ui->tableTools->item(i, 0)->text();
          for (int j = 0; j < deleteFile.count(); j++) {
            if (str0.contains(deleteFile.at(j))) {
              mw_one->ui->tableTools->removeRow(i);

              mw_one->setWindowModified(true);
              mw_one->updateIconStatus();
            }
          }
        }
      }

      foreach (QString file, deleteFile) {
        // 处理操作每个被删除的文件....
      }
    }
  }
}

// 文件修改时调用
void FileSystemWatcher::fileUpdated(const QString& path) {
  mw_one->strByModiMd5 = mw_one->getMD5(SaveFileName);
  if (mw_one->strOrgMd5 != mw_one->strByModiMd5) {
    mw_one->ui->frameTip->setHidden(false);
  } else
    return;
  mw_one->ui->lblFileName->setText(tr("The file has been modified by another "
                                      "program. Do you want to reload?") +
                                   "    " + QString("%1").arg(path) +
                                   "\n\nMD5: " + mw_one->strOrgMd5 +
                                   "  -->  MD5: " + mw_one->strByModiMd5);
}
