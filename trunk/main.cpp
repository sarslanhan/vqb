#include <QtGui/QApplication>
#include "vqbform.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    VqbForm w;
    w.show();
    return a.exec();
}
