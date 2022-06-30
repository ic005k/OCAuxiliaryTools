#pragma once

// Qt includes
#include <QDomElement>
#include <QIODevice>
#include <QVariant>
#include <QVariantList>
#include <QVariantMap>

class PListParser {

public:
    static QVariant parsePList(QIODevice* device);

private:
    static QVariant parseElement(const QDomElement& e);
    static QVariantList parseArrayElement(const QDomElement& node);
    static QVariantMap parseDictElement(const QDomElement& element);
};
