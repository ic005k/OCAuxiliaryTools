#include "recentfiles.h"

#include <QAction>
#include <QApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QMenu>
#include <QRegularExpression>
#include <QSettings>
#include <QString>
#include <QStringList>

extern bool zh_cn;

QString sanitizedFileName(const QString& name) {
  QString clean = name;
  return clean.replace(QRegularExpression("[^A-Za-z0-9]"), QString("_"));
}

RecentFiles::RecentFiles(QMainWindow* parent)
    : QObject(parent),
      m_recentMenu(new QMenu(parent)),
      m_recentMenuTriggeredAction(NULL)

{
  // create the sub-menu
  m_recentMenu->setTitle(tr("Open Recent..."));

  m_recentMenu->setObjectName("RecentMenu");

  // create an action for all possible entries in the sub-menu
  for (int i = 0; i < MaxRecentFiles; i++) {
    m_recentFileActions[i] = new QAction(m_recentMenu);
    m_recentFileActions[i]->setText("---");
    m_recentFileActions[i]->setVisible(false);
    connect(m_recentFileActions[i], SIGNAL(triggered()), this,
            SLOT(openRecentFile()));
    m_recentMenu->addAction(m_recentFileActions[i]);
  }

  // Set some defaults
  QSettings settings;

  if (!settings.value(recentFileCount).isValid())
    settings.setValue(recentFileCount, QVariant(4));

  // If there are no recent files, initialize an empty list
  if (!settings.allKeys().contains(recentFileListId)) {
    settings.setValue(recentFileListId, QVariant(QStringList()));
  }

  updateRecentFiles(settings);
}
RecentFiles::~RecentFiles() {
  // delete m_recentMenu; // might be bad because parent menu will delete.
  m_recentMenu = (QMenu*)NULL;
}

//在menu菜单项下插一个text名称的菜单项并关联slotname响应
void RecentFiles::attachToMenuAfterItem(
    QMenu* menu, /* menu that "Recent" sub-menu should be inserted into */
    QString
        text, /* Action in menu after which Recent menu should be inserted */
    const char* slotName) {
  QList<QAction*> actionList = menu->actions();

  int indexOfItemToFollow = 0;
  for (; indexOfItemToFollow < actionList.size(); indexOfItemToFollow++) {
    QAction* act = actionList.at(indexOfItemToFollow);
    if (!act->text().compare(text)) {
      break;
    }
  }

  int indexOfItemToPreceed = indexOfItemToFollow + 1;
  if (indexOfItemToPreceed >= actionList.size()) {
    // append to end of menu
    m_recentMenuTriggeredAction = menu->addMenu(m_recentMenu);
    m_recentMenuTriggeredAction->setObjectName("recentMenuAction");
  } else {
    // insert into menu
    QAction* before = actionList.at(indexOfItemToPreceed);
    m_recentMenuTriggeredAction = menu->insertMenu(before, m_recentMenu);
    m_recentMenuTriggeredAction->setObjectName("recentMenuAction");
    menu->insertSeparator(before);
  }

  connect(this, SIGNAL(openFile(QString)), parent(), slotName);
}

void RecentFiles::openRecentFile() {
  QAction* action = qobject_cast<QAction*>(sender());
  if (action) emit openFile(action->data().toString());
}

void RecentFiles::purgeMissingFilesFromList(QStringList& recentFileList) {
  for (QMutableStringListIterator i(recentFileList); i.hasNext();) {
    QString fileName = i.next();  // stash filename and advance iterator
    if (!QFile::exists(fileName)) {
      i.remove();  // removes item last jumped by i.next()
    }
  }
}

QString RecentFiles::strippedName(const QString& fullFileName) {
  return QFileInfo(fullFileName).fileName();
}

void RecentFiles::setMenuEnabled(bool tf) {
  m_recentMenuTriggeredAction->setEnabled(tf);
}

int RecentFiles::numberOfRecentFilesToSave() {
  QSettings settings;

  return settings.value(recentFileCount).toInt();
}

void RecentFiles::setMostRecentFile(const QString fileName) {
  if (fileName.isEmpty()) return;

  QSettings settings;
  QStringList recentFileList = settings.value(recentFileListId).toStringList();

  if (recentFileList.count() == MaxRecentFiles)  //防止数组越界
  {
    recentFileList.removeAt(MaxRecentFiles - 1);
  }

  recentFileList.removeAll(fileName);
  recentFileList.prepend(fileName);
  settings.setValue(recentFileListId, QVariant(recentFileList));

  updateRecentFiles(settings);

  qApp->setProperty("currentDirectory",
                    QVariant(QFileInfo(fileName).absoluteDir().absolutePath()));
}

void RecentFiles::setNumOfRecentFiles(int n) {
  QSettings settings;

  settings.setValue(recentFileCount, QVariant(n));

  updateRecentFiles(settings);

  // So a preference panel can be updated to show new value...
  emit newMaxFilesShown(n);
}

QStringList RecentFiles::getRecentFiles() const {
  QSettings settings;

  QStringList recentFileList = settings.value(recentFileListId).toStringList();
  return recentFileList;
}

void RecentFiles::clearHistory() {
  QSettings settings;
  QStringList recentFileList = settings.value(recentFileListId).toStringList();
  recentFileList.clear();
  settings.value(recentFileListId).clear();
  settings.setValue(recentFileListId, QVariant(recentFileList));
}

void RecentFiles::updateRecentFiles(QSettings& settings) {
  int numOfRecentFiles = settings.value(recentFileCount, QVariant(4)).toInt();

  QStringList MyRecentFileList =
      settings.value(recentFileListId).toStringList();
  purgeMissingFilesFromList(MyRecentFileList);
  settings.setValue(recentFileListId, QVariant(MyRecentFileList));

  // If there are no recent files, then the menu item (that would show the list)
  // should not be visible.
  if (m_recentMenuTriggeredAction) {
    if (numOfRecentFiles == 0) {
      m_recentMenuTriggeredAction->setVisible(false);
    } else {
      m_recentMenuTriggeredAction->setVisible(true);
    }
  }

  for (int j = 0; j < MaxRecentFiles; ++j) {
    if (j < MyRecentFileList.count() && j < numOfRecentFiles) {
      QString text = strippedName(MyRecentFileList[j]);
      m_recentFileActions[j]->setText(text);
      m_recentFileActions[j]->setData(MyRecentFileList[j]);
      m_recentFileActions[j]->setObjectName(sanitizedFileName(text));

      m_recentFileActions[j]->setVisible(true);
    } else {
      m_recentFileActions[j]->setVisible(false);
    }
  }

  for (int j = numOfRecentFiles; j < MyRecentFileList.count(); j++)
    m_recentFileActions[j]->setVisible(false);

  getRecentFiles();
}
