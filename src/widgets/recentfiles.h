#ifndef RECENTFILES_H
#define RECENTFILES_H

#include <QMainWindow>
#include <QObject>
#include <QSettings>
#include <QStringList>

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
class RecentFiles : public QObject {
  Q_OBJECT
 public:
  explicit RecentFiles(
      QMainWindow* parent = NULL); /* Parent mainwindow, just for proper
                  heirarchy, not actually used outside QObject constr */
  ~RecentFiles();

  void attachToMenuAfterItem(QMenu* menu, QString text, const char* slotName);

  QStringList getRecentFiles() const;
  void setMostRecentFile(const QString fileName);
  QString strippedName(const QString& fullFileName);

  void setMenuEnabled(bool tf);

  int numberOfRecentFilesToSave();

  static const int MaxRecentFiles = 21;  ///< Max number of names we keep.

  void clearHistory();
 public slots:
  /// The application can set the number of recent files retained/reported here
  void setNumOfRecentFiles(int n);

 signals:
  void openFile(QString fileName);
  void newMaxFilesShown(int);

 private slots:
  void openRecentFile();

 private:
  void purgeMissingFilesFromList(QStringList& recentFileList);
  void updateRecentFiles(QSettings& settings);

  QMenu* m_recentMenu;
  QAction* m_recentMenuTriggeredAction;
  QAction* m_recentFileActions[MaxRecentFiles];
};

#endif  // RECENTFILES_H
