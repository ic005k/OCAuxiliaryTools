#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDateTime>
#include <QDialog>
#include <QFileInfo>

namespace Ui {
class aboutDialog;
}

class aboutDialog : public QDialog {
  Q_OBJECT

 public:
  explicit aboutDialog(QWidget* parent = nullptr);
  ~aboutDialog();
  Ui::aboutDialog* ui;
  QStringList listDownCount;

  void initInfoStr();

 private slots:

 private:
};

#endif  // ABOUTDIALOG_H
