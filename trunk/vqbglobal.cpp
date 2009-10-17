#include "vqbglobal.h"

QStringList VqbGlobal::literalTypes()
{
    //FIXME?: don't create it every time
    return QStringList() << "URI" << "var" << "string" << "int" << "Literal";// <<"decimal" << "dateTime" <<;
}

QString VqbGlobal::typeRegExp(QString type)
{
    if(type=="string" || type=="Literal") {
         return QString("[\\S\\s]*");
    }
    else if(type=="var") {
        return QString("([\\?\\$]\\w+)");
    }
    else if(type=="URI") {
         return QString("((\"[\\S\\s]*\"\\^\\^)?<[\\S\\s]*>)|(\\S+:\\S+)");//"[<[\\w+://]?\\S*>]|[w+:w+]");//URI
    }
    else if(type=="int") {
        return QString("([+-]?\\d+)?");
    }
    else if(type=="dateTime") {
         return QString("(0[1-9]|[12][0-9]|3[01])[/]((0[1-9])|(1[012]))/\\d\\d\\d\\d");
    }
    else return QString();
}

QString VqbGlobal::getCanonicalForm(QString expression, QString type)
{
    QString canonicalForm;

    if( type == "URI" ) {
        canonicalForm = expression;
    }
    else if ( type == "var" ) {
        canonicalForm = expression;
    }
    else if ( type == "Literal" ) {
        canonicalForm = "\"" + expression + "\"";
    }
    else if ( type == "string" ) {
        canonicalForm = "\"" + expression + "\"^^<http://www.w3.org/2001/XMLSchema#string>" ;
    }
    else if ( type == "int" ) {
        canonicalForm = "\"" + expression + "\"^^<http://www.w3.org/2001/XMLSchema#int>" ;
    }
    else if ( type == "dateTime" ) {
        canonicalForm = "\"" + expression + "\"^^<http://www.w3.org/2001/XMLSchema#dateTime>" ;
    }
    else if ( type == "decimal" ) {
        canonicalForm = "\"" + expression + "\"^^<http://www.w3.org/2001/XMLSchema#decimal>" ;
    }

    return canonicalForm;
}
