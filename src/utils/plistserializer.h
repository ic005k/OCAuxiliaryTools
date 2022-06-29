#pragma once

// Qt includes
#include <QDomDocument>
#include <QDomElement>
#include <QIODevice>
#include <QString>
#include <QVariant>
#include <QVariantList>
#include <QVariantMap>

class PListSerializer {
 public:
  static QString toPList(const QVariant& variant, QString FileName);

  static bool fileValidation(QString FileName);

 private:
  static QDomElement serializeElement(QDomDocument& doc,
                                      const QVariant& variant);
  static QDomElement serializeMap(QDomDocument& doc, const QVariantMap& map);
  static QDomElement serializeList(QDomDocument& doc, const QVariantList& list);
};
