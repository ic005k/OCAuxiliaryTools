#ifndef RECENTFILES_H
#define RECENTFILES_H

#include <QObject>
#include <QStringList>
#include <QMainWindow>
#include <QSettings>

class QMenu;
class QAction;

#define recentFileCount "recentFiles/numOfRecentFiles"
#define recentFileListId "recentFiles/fileList"

/**
 * @brief The RecentFiles class manages a list of recently accessed files.
 *
 * It handles user-settable number of most recently accessed files, provides
 * a sub-menu that allows the user to select recently used files for opening.
 */
class RecentFiles : public QObject
{
    Q_OBJECT
public:
    explicit RecentFiles(QMainWindow *parent=NULL); /* Parent mainwindow, just for proper heirarchy, not actually used outside QObject constr */
    ~RecentFiles();

    /// Inserts the sub-menu into another Menu
    ///  param menu   The parent menu where the sub-menu should be inserted
    ///  param text   Text of menu item after which Recent menu is inserted
    void attachToMenuAfterItem(QMenu *menu, QString text, const char *slotName);

    QStringList getRecentFiles() const;                     ///< application calls this to get list of recent files
    void        setMostRecentFile(const QString fileName);  ///< called when each new file is opened
    QString     strippedName(const QString &fullFileName);  ///< returns filename from full path

    void        setMenuEnabled(bool tf);

    /// returns how many recent files are being remenbered.  see setNumOfRecentFiles()
    int         numberOfRecentFilesToSave();

    static const int MaxRecentFiles = 15;  ///< Max number of names we keep.

public slots:
    /// The application can set the number of recent files retained/reported here
    void setNumOfRecentFiles(int n);

signals:
    void openFile(QString fileName); ///< emitted when user selects item from "Open Recent" sub-menu
    void newMaxFilesShown(int);     ///< tells observers that the number of recent files being remembered has changed.

private slots:
    void openRecentFile(); ///< menu items signal this when user selects item from "Open Recent" sub-menu

private:
    void purgeMissingFilesFromList(QStringList &recentFileList);
    void updateRecentFiles(QSettings &settings);  ///< call this with each new filename

    QMenu *m_recentMenu;
    QAction *m_recentMenuTriggeredAction;
    QAction *m_recentFileActions[MaxRecentFiles]; ///< QActions created in file menu for storing recent files
};

#endif // RECENTFILES_H
