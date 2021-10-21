#ifndef DLGMISC_H
#define DLGMISC_H

#include <QDialog>
#include <QListWidgetItem>

namespace Ui {
class dlgMisc;
}

class dlgMisc : public QDialog {
  Q_OBJECT

 public:
  explicit dlgMisc(QWidget *parent = nullptr);
  ~dlgMisc();

 private slots:
  void on_btnAdd_clicked();

  void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

 private:
  Ui::dlgMisc *ui;
  void addBootArgs(QString str);
};

#endif  // DLGMISC_H
