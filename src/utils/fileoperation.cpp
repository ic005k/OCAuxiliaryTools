#include "fileoperation.h"

#include <QDir>
#include <QDebug>

bool FileOperation::copyDirectoryFiles(const QString &fromDir, const QString &toDir, bool coverFileIfExist)
{
    QDir sourceDir(fromDir);
    QDir targetDir(toDir);
    if (!targetDir.exists()) { /**< 如果目标目录不存在，则进行创建 */
        if (!targetDir.mkdir(targetDir.absolutePath())) {
            qWarning() << "Error: Create target Dir failed!";
            return false;
        }
    }

    QFileInfoList fileInfoList = sourceDir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
    foreach (QFileInfo fileInfo, fileInfoList) {
        if (fileInfo.isDir()) { /**< 当为目录时，递归的进行copy */
            if (!copyDirectoryFiles(fileInfo.filePath(),
                                    targetDir.filePath(fileInfo.fileName()),
                                    coverFileIfExist))
                return false;
        } else { /**< 当允许覆盖操作时，将旧文件进行删除操作 */
            if (coverFileIfExist && targetDir.exists(fileInfo.fileName())) {
                targetDir.remove(fileInfo.fileName());
            }

            /// 进行文件copy
            if (!QFile::copy(fileInfo.filePath(),
                             targetDir.filePath(fileInfo.fileName()))) {
                return false;
            }
        }
    }
    return true;
}

bool FileOperation::copyFileToPath(QString sourceFile, QString toFile, bool coverFileIfExist)
{
    toFile.replace("\\", "/");
    if (sourceFile == toFile) {
        return true;
    }
    if (!QFile::exists(sourceFile)) {
        return false;
    }

    bool exist = QFile::exists(toFile);
    if (exist) {
        if (coverFileIfExist)
            QFile::remove(toFile);
    }

    if (!QFile::copy(sourceFile, toFile)) {
        return false;
    }
    return true;
}
