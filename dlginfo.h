#ifndef DLGINFO_H
#define DLGINFO_H

#include <QDialog>

namespace Ui {
class dlgInfo;
}

class dlgInfo : public QDialog {
  Q_OBJECT

 public:
  explicit dlgInfo(QWidget *parent = nullptr);
  Ui::dlgInfo *ui;
  ~dlgInfo();

 protected:
  void closeEvent(QCloseEvent *event) override;

 private:
};

#endif  // DLGINFO_H
