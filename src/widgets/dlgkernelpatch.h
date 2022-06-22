#ifndef DLGKERNELPATCH_H
#define DLGKERNELPATCH_H

#include <QDebug>
#include <QDialog>
#include <QDir>
#include <QFileInfo>
#include <QListWidgetItem>

namespace Ui {
class dlgKernelPatch;
}

class dlgKernelPatch : public QDialog {
  Q_OBJECT

 public:
  explicit dlgKernelPatch(QWidget *parent = nullptr);
  ~dlgKernelPatch();
  Ui::dlgKernelPatch *ui;

  void loadFiles();

  void appendKernelPatch(QString PlistFileName);
 private slots:
  void on_btnAdd_clicked();

  void on_btnOpenDir_clicked();

  void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

 private:
  QString dirpath;
};

#endif  // DLGKERNELPATCH_H
