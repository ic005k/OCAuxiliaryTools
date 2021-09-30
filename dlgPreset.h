#ifndef DLGPRESET_H
#define DLGPRESET_H

#include <QDialog>
#include <QListWidgetItem>
#include <QVariantMap>

namespace Ui {
class dlgPreset;
}

class dlgPreset : public QDialog {
  Q_OBJECT

 public:
  explicit dlgPreset(QWidget *parent = nullptr);
  ~dlgPreset();
  Ui::dlgPreset *ui;

  QString strPresetPath;
  void loadDPAdd();
  QVariantMap map_sub;
  QVariantMap map_add;
  QVariantMap map;
  QVariantList map_patch;

  void loadACPIPatch();
  void loadKernelPatch();
 private slots:
  void on_btnAdd_clicked();

  void on_listDPAdd_itemDoubleClicked(QListWidgetItem *item);

  void on_listACPIPatch_itemDoubleClicked(QListWidgetItem *item);

  void on_listKernelPatch_itemDoubleClicked(QListWidgetItem *item);

 private:
};

#endif  // DLGPRESET_H
