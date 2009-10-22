#include "vqbglobal.h"

#include <QMap>
#include <KRandom>

QStringList VqbGlobal::literalTypes()
{
    //FIXME?: don't create it every time
    return QStringList() << "URI" << "var" << "Literal" << "string" << "int";// <<"decimal" << "dateTime" <<;
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
         return QString("((\"[\\S\\s]*\"\\^\\^)?<[\\S\\s]*>)|(\\S+:\\S+)|(\"[\\S\\s]*\")");//"[<[\\w+://]?\\S*>]|[w+:w+]");//URI
    }
    else if(type=="int") {
        return QString("([+-]?\\d+)?");
    }
    else if(type=="dateTime") {
         return QString("(0[1-9]|[12][0-9]|3[01])[/]((0[1-9])|(1[012]))/\\d\\d\\d\\d");
    }
    else return QString();
}

QString VqbGlobal::constructObject(bool filterOn, QString relation, QString expression, QString type)
{
    QString finalForm;

    if(filterOn) {//Filter ON
        QString var = VqbGlobal::randomVarName();//QString("?v" +  QString::number(KRandom::random() % 80 + 20)) ;
        if (relation == "contains") {
            finalForm.append( var + " . FILTER regex(" + var + ", '" + expression + "', 'i') . ") ;
        }
        else if (relation == "equals") {
            finalForm.append( var + " . FILTER regex(" + var + ", '^" + expression + "$', 'i') . ");
        }
        return finalForm;
    }
    //else //Filter OFF

    if( type == "URI" ) {
        finalForm += expression;
    }
    else if ( type == "var" ) {
        finalForm += expression;
    }
    else if ( type == "Literal" ) {
        finalForm += "\"" + expression + "\"";
    }
    else if ( type == "string" ) {
        finalForm += "\"" + expression + "\"^^<http://www.w3.org/2001/XMLSchema#string>" ;
    }
    else if ( type == "int" ) {
        finalForm += "\"" + expression + "\"^^<http://www.w3.org/2001/XMLSchema#int>" ;
    }
    else if ( type == "dateTime" ) {
        finalForm += "\"" + expression + "\"^^<http://www.w3.org/2001/XMLSchema#dateTime>" ;
    }
    else if ( type == "decimal" ) {
        finalForm += "\"" + expression + "\"^^<http://www.w3.org/2001/XMLSchema#decimal>" ;
    }

    return finalForm;
}

QString VqbGlobal::randomVarName()
{
    return QString("?v" +  QString::number(KRandom::random() % 80 + 20)) ;
}

    
QString  VqbGlobal::prefixForm( QString uri )
{
    QString prefUri = uri;
    bool prefixFound;

    foreach( QString p, prefixes().values() ) {        
        if( prefUri.contains( p ) ) {
            prefUri.replace( p, prefixes().keys( p ).first() + ":" );
            prefixFound = true;
            break;
        }
    }
    //if no prefix found, surround predicate with < and >
    if( !prefixFound ) {
        if(!prefUri.startsWith("<")) {
            prefUri.prepend("<");
        }
        if(!prefUri.endsWith(">")) {
            prefUri.append(">");
        }
    }
    return prefUri;
}

static QMap<QString, QString> prefixList;
QMap<QString, QString>  VqbGlobal::prefixes()
{
    if ( prefixList.empty() ) {
    //build prefix list
    prefixList.insert("rdf", "http://www.w3.org/1999/02/22-rdf-syntax-ns#");
    prefixList.insert("rdfs", "http://www.w3.org/2000/01/rdf-schema#");
    prefixList.insert("sioc", "http://rdfs.org/sioc/ns#");
    prefixList.insert("rss", "http://purl.org/rss/1.0/");
    prefixList.insert("dc", "http://purl.org/dc/elements/1.1/");
    prefixList.insert("dcterms", "http://purl.org/dc/terms/");
    prefixList.insert("foaf", "http://xmlns.com/foaf/0.1/");
    prefixList.insert("nrl", "http://www.semanticdesktop.org/ontologies/2007/08/15/nrl#");
    prefixList.insert("xesam", "http://freedesktop.org/standards/xesam/1.0/core#");
    prefixList.insert("nao", "http://www.semanticdesktop.org/ontologies/2007/08/15/nao#");
    prefixList.insert("protege", "http://protege.stanford.edu/system#" );
    prefixList.insert("robots","");
    prefixList.insert("owl","http://www.w3.org/2002/07/owl#");
    prefixList.insert("dbp","http://dbpedia.org/property/");
    prefixList.insert("swrc","http://swrc.ontoware.org/ontology#");
    prefixList.insert("d2rq","http://www.wiwiss.fu-berlin.de/suhl/bizer/D2RQ/0.1#");
    prefixList.insert("nie","http://www.semanticdesktop.org/ontologies/2007/01/19/nie#");
    prefixList.insert("dcmitype","http://purl.org/dc/dcmitype/");


    }

    return prefixList;
}

QString VqbGlobal::addPrefixes(QString query)
{
    foreach( QString p, prefixes().keys() ) {
        if( query.contains( p + ":" ) ) {
            QString prefix = "PREFIX " + p + ": <" + prefixes().value( p ) + ">" ;
            if( !query.contains( prefix ) ) {
                query.prepend( prefix + "\n" );
            }
        }
    }
        return query;
}
