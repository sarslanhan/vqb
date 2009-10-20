#ifndef VQBGLOBAL_H
#define VQBGLOBAL_H

#include <QString>
#include <QStringList>

class VqbGlobal
{
public:
    static QStringList literalTypes();
    static QString typeRegExp(QString type);

    static QString constructObject(bool filterOn, QString relation, QString expression, QString type);//gets the canonical form for the object of a triple
    static QString randomVarName();

    static QMap<QString, QString>  prefixes();
    static QString prefixForm(QString uri);    
    static QString addPrefixes(QString query);

};

#endif // VQBGLOBAL_H
