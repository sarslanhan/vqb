#ifndef VQBGLOBAL_H
#define VQBGLOBAL_H

#include <QString>
#include <QStringList>

class VqbGlobal
{
public:
    static QStringList literalTypes();
    static QString typeRegExp(QString type);
    static QString getCanonicalForm(QString expression, QString type);
};

#endif // VQBGLOBAL_H
