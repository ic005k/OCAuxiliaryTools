#ifndef FILEOPERATION_H
#define FILEOPERATION_H

#include <QObject>

class FileOperation : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief copyDirectoryFiles 拷贝文件夹
     * @param fromDir 源文件夹
     * @param toDir 目的文件夹
     * @param coverFileIfExist 是否覆盖已存在文件
     * @return 拷贝成功与否
     */
    static bool copyDirectoryFiles(const QString& fromDir,
                                   const QString& toDir,
                                   bool coverFileIfExist);
    /**
     * @brief copyFileToPath 拷贝文件
     * @param sourceDir 源文件
     * @param toDir 目的文件
     * @param coverFileIfExist 是否覆盖已存在文件
     * @return 拷贝成功与否
     */
    static bool copyFileToPath(QString sourceFile,
                               QString toFile,
                               bool coverFileIfExist);

};

#endif // FILEOPERATION_H
