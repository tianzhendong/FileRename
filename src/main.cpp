#include <QApplication>
#include "widget.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    qRegisterMetaType<QHash<QByteArray,QStringList>>("QHash<QByteArray,QStringList>");
    Widget w;
    w.show();
    return QApplication::exec();
}
