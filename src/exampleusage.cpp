#include "PListParser.h"
#include "PListSerializer.h"
#include <QtCore>

void parseExample() {
    QByteArray sample = "\
<?xml version=\"1.0\" encoding=\"UTF-8\"?>\
<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\
<plist version=\"1.0\">\
<dict>\
    <key>SomeKey</key>\
    <string>Value1</string>\
    <key>MyOtherKey</key>\
    <string>Value2</string>\
</dict>\
</plist>";
	QBuffer buffer(&sample);
	QVariantMap map = PListParser::parsePList(&buffer).toMap();
	QString val = map["SomeKey"].toString();
	qDebug() << "got val:" << val;
}

void serializeExample() {
	QVariantMap foo;
	foo["SomeKey"] = "Value1";
	foo["SomeOtherKey"] = 10;
    qDebug() << "PList:\n" << PListSerializer::toPList(foo , QDir::homePath() + "test.plist");
}

/*int main() {
	parseExample();
	serializeExample();
    return 0;
}*/
