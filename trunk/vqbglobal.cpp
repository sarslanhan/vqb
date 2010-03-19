#include "vqbglobal.h"

#include <QMap>
#include <KRandom>

QStringList VqbGlobal::objectTypes()
{
    //FIXME?: don't create it every time
    return QStringList() << "URI" << "var" << "Literal" << "string" << "int";// <<"decimal" << "dateTime" <<;
}

QString VqbGlobal::typeRegExp(QString type)
{
    if(type=="string") {
         return QString("[\\S\\s]*");
    }
    else if(type=="Literal") {
        return QString("(\"[\\S\\s]*\"\\^\\^((<[\\S\\s]*>)|(\\S+:\\S+)))|(\"[\\S\\s]*\"(@[a-z][a-z])?)");
    }
    else if(type=="var") {
        return QString("([\\?\\$]\\w+)");
    }
    else if (type == "URI") {
        return QString("(<[\\S\\s]*>)|(\\w+:\\w+)");
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
            finalForm.append( var + " . FILTER regex(" + var + ", '" + expression + "', 'i') ") ;
        }
        else if (relation == "equals") {
            finalForm.append( var + " . FILTER regex(" + var + ", '^" + expression + "$', 'i') ");
        }
        return finalForm;
    }
    //else //Filter OFF

    if( type == "URI" || type == "var" || type == "Literal") {
        finalForm += expression;
    }
    else if ( type == "string" ) {
        //FIXME: needs to be escaped
        finalForm += "\"" + expression + "\"^^xsd:string" ;
    }
    else if ( type == "int" ) {
        finalForm += "\"" + expression + "\"^^xsd:int" ;
    }
    else if ( type == "dateTime" ) {
        finalForm += "\"" + expression + "\"^^xsd:dateTime" ;
    }
    else if ( type == "decimal" ) {
        finalForm += "\"" + expression + "\"^^xsd:decimal" ;
    }

    //finalForm += " . ";

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
    if( prefixFound ) {
        prefUri.remove("<");
        prefUri.remove(">");
    }
    else {
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
    prefixList.insert("protege", "http://protege.stanford.edu/system#" );
    prefixList.insert("owl","http://www.w3.org/2002/07/owl#");
    prefixList.insert("dbp","http://dbpedia.org/property/");
    prefixList.insert("swrc","http://swrc.ontoware.org/ontology#");
    prefixList.insert("d2rq","http://www.wiwiss.fu-berlin.de/suhl/bizer/D2RQ/0.1#");
    prefixList.insert("dcmitype","http://purl.org/dc/dcmitype/");
    prefixList.insert("geo","http://www.w3.org/2003/01/geo/wgs84_pos#");
    prefixList.insert("xsd","http://www.w3.org/2001/XMLSchema#");
    prefixList.insert("swrc","http://swrc.ontoware.org/ontology#");
    prefixList.insert("swc","http://data.semanticweb.org/ns/swc/ontology#");    

    prefixList.insert("xesam", "http://freedesktop.org/standards/xesam/1.0/core#");
    prefixList.insert("nrl", "http://www.semanticdesktop.org/ontologies/2007/08/15/nrl#");
    prefixList.insert("nie","http://www.semanticdesktop.org/ontologies/2007/01/19/nie#");
    prefixList.insert("nao", "http://www.semanticdesktop.org/ontologies/2007/08/15/nao#");
    prefixList.insert("nfo","http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#");
    prefixList.insert("nexif","http://www.semanticdesktop.org/ontologies/2007/05/10/nexif#");
    prefixList.insert("nco","http://www.semanticdesktop.org/ontologies/2007/03/22/nco#");
    prefixList.insert("nmo","http://www.semanticdesktop.org/ontologies/2007/03/22/nmo#");
    prefixList.insert("nid3","http://www.semanticdesktop.org/ontologies/2007/05/10/nid3#");
    prefixList.insert("ncal","http://www.semanticdesktop.org/ontologies/2007/04/02/ncal#");
    prefixList.insert("pimo","http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#");

    prefixList.insert("tmo","http://www.semanticdesktop.org/ontologies/2008/05/20/tmo#");
    prefixList.insert("virtrdf","http://www.openlinksw.com/schemas/virtrdf#");
    prefixList.insert("virtrdf-data-formats","http://www.openlinksw.com/virtrdf-data-formats#");

    }

    return prefixList;
}

QString VqbGlobal::addPrefixes(QString query)
{
    //FIXME: replace everything with prefixes

    //add PREFIX definitions
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
