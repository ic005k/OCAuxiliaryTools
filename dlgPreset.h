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

  QString strPresetFile;
  QVariantMap map_sub;
  QVariantMap map_add;
  QVariantMap map;
  QVariantList map_patch;
  bool blDPAdd = false;
  bool blACPIPatch = false;
  bool blKernelPatch = false;
  bool blNVAdd = false;
  bool blNVDelete = false;
  bool blNVLegacy = false;
  bool blKext = false;
  QStringList listKextPreset;

  void loadPreset(QString strMain, QString strSub, QString strComment,
                  QListWidget *list);
  void addKextPresets();
 private slots:
  void on_btnAdd_clicked();

  void on_listPreset_itemDoubleClicked(QListWidgetItem *item);

 private:
};

#endif  // DLGPRESET_H
